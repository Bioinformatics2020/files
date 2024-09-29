# 基础用法

### 标准流程

git status  查看工作状态
git add xxx 跟踪新文件、将已修改的文件放入暂存区、标记文件冲突已解决
git commit -m "提交代码"   推送修改暂存区内容到本地git库中
git pull  拉取最新代码，与本地代码合并，解决冲突
git push  提交本地仓库到远程仓库

# git仓库

### git协议

Local 本地协议
    优点：创建简单，依靠局域网共享文件系统进行访问，使用本地的文件权限与网络权限进行管理。
    缺点：共享文件系统配置复杂，远程访问不方便，权限配置不方便
    /srv/git/project.git 本地库硬链接
    file:///srv/git/project.git 类似网络传输连接
HTTP
    智能HTTP协议 新版本，普遍使用
    哑(Dumb)HTTP协议 
SSH
    优点：架设对于专业人员简单，用户需要密匙访问
Git
    优点：简单快速

### 创建远程仓库

git init 初始化仓库
git config receive.denyCurrentBranch ignore 设置当前仓库为远程仓库

### 关联本地仓库到远程仓库

git init
git remote add origin D:\\GitServer
git add .
git commit -m "clone下来的源码"
git push -u origin master 指定推送到origin仓库的master分支

### 管理远程仓库

git remote 显示现有远程的列表
    -v 显示详细详细
    add <name> <url> 添加远程仓库
    remove <name> 移除远程仓库
    rename <oldname> <newname> 重命名远程仓库名称

### 创建本地仓库

git clone (url) 克隆远程仓库
Git clone http://用户名@125.01.02.03:10086/test/xiangmu.git 无法找到仓库时需要加入用户名称再拉取（输入密码后即拉取代码成功）
    --depth <深度> 创建"浅"克隆，忽略历史提交，仅克隆master分支最新版本，建议开发时不要使用这个选项
git pull --unshallow 拉取历史提交，使浅仓库变为与源仓库相同
git config remote.origin.fetch "+refs/heads/*:refs/remotes/origin/*" 修改分支配置选项为关联多个分支

# git分支

### 切换分支

git checkout 
    . 切换到当前分支（放弃工作区中全部的修改）
    (branchname) 切换到branchname分支。注意需要stash当前的工作区。未关联文件不受影响
    --orphan (branchname) 创建一个空分支
    -b (branchname) 创建并切换到新的分支.
    -b (branchname) (tagname) 基于某个标签对应的版本创建并切换到新的分支.
    -b (branchname) (hash) 基于某个hash对应的版本创建并切换到新的分支.
    --track origin/master 切换到指定的远程分支

### 文件版本回退

git checkout 提交hash值 文件或文件名

### 分支管理

#### 分支查看，创建，删除

git branch 列出本地的分支
    -r 列出远程分支
    -a 列出远程跟踪的分支和本地分支
    -d (branchname) 删除本地分支
    -vv 查看本地分支与对应的跟踪分支
    --merged 检查哪些分支已合并到主分支
    --no-merged 检查未合并到主分支的分支

git branch (branchname) 创建分支

#### 分支同步

git push origin --delete (branchname) 删除远程分支

git push origin (branchname):(branchname) 推送本地分支到远程分支(新建远程分支)

git fetch 更新远程分支

#### 分支合并

git merge (branchname) 合并某分支到当前分支
    遇到冲突时，修改代码解决冲突
    git status 查看unmerged状态的文件
    git add (filename) 标记文件冲突已解决
    git commit 解决完成冲突之后执行commit操作，提交本次merge行为

git rebase 变基操作，将一个分支的修改在另一个分支重新修改一遍
    [Git - 变基 (git-scm.com)](https://git-scm.com/book/zh/v2/Git-%E5%88%86%E6%94%AF-%E5%8F%98%E5%9F%BA)
    master 将当前分支变基到master分支
    --onto master branch1 branch2 当主分支上分离出branch1, branch1上分离出branch2，
        并且branch2完成开发后需要将branch2合并到master,但是保留branch1不变时执行命令，
        具体含义为取出brach2分支，找出它从brach1分支分歧之后的修改，并应用到master分支

git cherry-pick (hash) 将指定的commit应用于当前分支

    git cherry-pick hashA..hashB 将A到B之间的所有提交应用到当前分支

    -e 修复commit信息

### 标签

git tag 查看标签
  -l "通配符" 使用通配符查找标签
  (v0.1) 创建轻量标签
  -a (v0.1) -m "注释" 创建附注标签
  -a (v0.1) -m "注释" hsah 向指定版本创建附注标签
  -d (v0.1) 删除标签

git show (v0.1) 查看标签对应的版本信息

git push origin --tags 提交本地创建的所有标签
git push origin --delete <tagname> 删除远程标签

# git工作树

### 移动或删除

git mv <源文件> <目标文件>移动或重命名文件
  -f 强制移动(覆盖)
  -k 跳过错误操作
  -v 显示详细信息

git reset 删除的是已跟踪的文件，将已commit的回退。
  --hard <HASH> #返回到某个节点，不保留修改，已有的改动会丢失。
  --soft <HASH> #返回到某个节点, 保留修改，已有的改动会保留，在未提交中，git status或git diff可看。

git clean 删除未跟踪的文件
  -n 不实际删除，只是进行演练，展示将要进行的操作，有哪些文件将要被删除。（可先使用该命令参数，然后再决定是否执行）
  -f 删除文件
  -i 显示将要删除的文件
  -d 递归删除目录及文件（未跟踪的）
  -q 仅显示错误，成功删除的文件不显示
  -x 也删除被忽略的文档
  -X 仅删除被忽略的文档
  示例：
    -df     返回到某个节点，（未跟踪文件的删除）
    -nxdf （查看要删除的文件及目录，确认无误后再使用上面的命令进行删除）

### 查询

git status 工作树状态
  -s 以简短的形式给出输出，左侧为暂存区状态，右侧为工作区状态
  -u 显示未追踪文件
  --ignored 显示被忽略的文件

  输出提示：
  '' 未修改的
  M  已修改
  T  文件类型改变
  A  添加
  D  删除
  R  重新命名
  C  被复制
  U  更新但未合并
  ?  未被追踪的
  !  忽略不计

git ls-files 显示工作目录中的文件信息
    -o,--others 在输出中显示其他（即未跟踪）文件
    --exclude-standard 不显示标准的 Git 排除文件：.git/info/exclude、每个目录中的 .gitignore，以及用户的全局排除文件。

git check-ignore
    -v 文件名 输出文件被排除的位置

git stash 暂存工作区修改
    pop 应用上一次暂存文件并删除暂存记录
    list 显示暂存记录

### 修改提交

git commit --amend 修改commit信息
git commit --amend --author="name \<abc@qq.com>" --no-edit 修改commit作者、邮箱

git commit -a --amend --no-edit 重新修改代码并提交，替换掉最后一次提交

修改已提交的内容：

    [Git - 重写历史 (git-scm.com)](https://git-scm.com/book/zh/v2/Git-%E5%B7%A5%E5%85%B7-%E9%87%8D%E5%86%99%E5%8E%86%E5%8F%B2)
    基于git rebase -i选项，自某个版本开始，重写之后的所有提交，功能强大但是影响巨大
    通过交互式命令行完成，支持修改commit信息、修改commit顺序、压缩提交、拆分提交等
    在服务器上完成变基后，其它人需要使用git pull --rebase自动识别服务器的变基修改并在本地完成自动变基

    基于git filter-branch功能，可以更加自由的修改任何历史信息，但是指令比较复杂

# 配置文件

git config --global core.autocrlf false 关闭warning: LF 报错
git config --global --list 查看全局配置
git config --list 查看本地配置
git config --list --show-origin 显示所有配置

git凭证，需要在windows的凭证管理器中删除旧凭证，然后重新clone代码

git config --global user.name "unreal engine consumer" 全局修改用户名称
git config user.email "2252313806@qq.com" 当前仓库修改用户名称

# 特殊应用

### git解决冲突

git status 查看工作树状态
手动解决冲突
    1. git checkout --ours <fileName> 保留合并基准分支(本地的分支)改动的版本
    2. git checkout --theirs <fileName> 保留被合并分支(仓库的分支)改动的版本
    3. 手动修改
    4. git revert <fileName> 保留当前分支的内容
    在rebase中 基准分支是master
    在merge中基准分支是当前分支

git add <fileName> 添加到暂存区，表示冲突解决完成
git commit -m "xxx" 提交到本地仓库
git status 检查工作树状态
git pull --rebase 在本地完成分支合并，避免仓库上多条分支线
git push 提交到远程仓库

### 子模块

git submodule add https://xxx/gittest.git src/gittest 添加子模块
git submodule update --init --recursive 下载子模块

### 忽略文件

全局配置文件(~/.gitignore)
远程配置文件($PWD/.gitignore)
本地配置文件($PWD/.git/info/exlude)

### git与GitHub关联

ssh-keygen -t rsa -C "2252313806@qq.com" 生成SSH密匙
将生成文件里的公匙配置到GitHub setting SSH keys里面
ssh -T git@github.com 验证SSH连接

### git代码从一个仓库移动到另一个仓库

git remote add <branch1> <url> 绑定第一个仓库
git remote add <branch2> <url> 绑定第二个仓库
git pull branch1 master 拉取第一个仓库的提交
git push -u origin2 master 提交到第二个仓库

### 其它

git gc 压缩.git文件，将全量保存的历史版本改为部分全量部分保存差异
    --aggressive 更高的压缩比例，效果有限，不推荐

# .git文件

### config

项目配置选项

### description 描述/说明

GitWeb程序相关

### HEDA

文件目前被检出的分支

### hooks/

客户端或服务器的钩子脚本，默认提供多种示例

### info/

包含一个exclude文件，记录本地排除文件

### objects/

对象数据库，存储所有数据内容

### refs/

存储指向数据(分支、仓库、标签等)的指针
