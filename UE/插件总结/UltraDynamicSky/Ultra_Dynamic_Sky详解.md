# 变量说明

## 公开变量

### Basic Controls

Refresh Settings：刷新设置，修改公开参数之后会执行构造函数，构造函数中会将RefreshSettings设为false，通过这种技巧实现刷新效果。

Time of Day：一天中的时间，范围是0-2400，可转换为标准时间。开启自动时间流动时，该值的变化规律由Animate Time of Day相关的多个参数以及tick决定。该值应用于：日出日落切换事件、太阳光源角度、月亮光源角度、高度雾组件雾密度。

Sky Mode：天空模式、云彩模式。体积云、静态云、2D动态云、无云、2D动态云叠加颜色曲线、极光。不同的模式主决定以下属性：云层材质、体积云、体积极光、光线阴影、云雾粒子系统。

Cloud Coverage：云覆盖范围。从云逐渐变厚到出现浓雾。影响以下属性：云密度、云形状、云高度、日月光照云阴影、云自身阴影、月光体积散射强度、高度雾密度、高度雾雾高度衰减、高度雾雾内散射颜色、高度雾定向散射颜色、太阳光源颜色、太阳光照强度、太阳高光效果、太阳光轮效果、天光颜色、天光强度、天光立方体贴图、天空大气组件MIE各向异性、星光颜色、月光强度、极光强度等。

Overall Intensity：整体亮度。天空、云层、体积云、云雾粒子、高度雾、天光的整体光线强度。

Saturation：饱和度。云层与天空的饱和度。

Contrast：对比度。云层与天空的对比度。

Dawn Time：黎明时间。影响太阳与月亮随时间变化的角度，参与时间流速计算。

Dusk Time：黄昏时间。作用同上。

Night brightness：夜晚亮度。影响月光亮度、高度雾、天空颜色、整体亮度。

Directional_Lights_Absent_Brightness：方向光缺失时亮度。当太阳光角度过低或月亮角度低或月相不圆满时的亮度补偿。影响天光、高度雾、云层、天空。

Overcast_Night Brightness：阴天光照强度。当云层较厚(Cloud Coverage)时通过这个参数对光线强度进行补偿。影响月光亮度、高度雾、天空颜色、整体亮度。

Use Sky Mode Scalability Map：根据用户设置的视觉效果质量参数动态选择SkyMode。

Sky Mode_ScalabilityMap：视觉效果质量参数与SkyMode之间的映射表。

For Mobile Renderer：为移动端渲染。移动端许多效果不支持，开启这个选项之后会开启补偿的效果。包括：不启动云雾粒子系统；使太阳光、月光、天光强度直接受云覆盖范围影响(跳过云雾粒子系统)、关闭天光组件立方体贴图混合过渡功能。

### Animate Time of Day 时间自动变化

Animate Time of Day：开启时间自动流动。

Day Length：白天的长度，以分钟为单位。通过仅针对白天进行时间膨胀的思路，与日出与日落时间、Time Speed共同作用来决定时间的真实流动。

Night Length：夜晚的长度，同上。

Time Speed：全局时间流动的速度。

Sunset Event Time：发生日落事件的时间。

Sunrise Event Time：发生日出事件的时间。

### Volumetric Clouds 体积云

Layer_Height_Scale：体积云图层高度。

Bottom_Altitude：体积云底部高度。

SubNoise_Scale：用于云层侵蚀效果的3D平铺噪波纹理的比例。用于体积云、云雾粒子、云地面阴影的材质参数。

High Frequency Noise Layer：高频噪点。用于体积云、云雾粒子的材质参数。

Extinction Scale Top：云层顶部消光倍率。用于体积云、云雾粒子的材质参数。

Extinction Scale Bottom：云层底部消光倍率。用于体积云、云雾粒子的材质参数。

View Sample Count Scale：体积云视图样本数缩放。

Shadow Sample Scale：体积云视图样本数缩放。

Volumetric_Cloud Noise Type：体积云噪点类型。不同噪点产生不同云朵效果。

Volumetric_Custom Noise Texture：自定义体积云噪点材质。

Multiscattering Intensity：多重散射强度。强度越高效果越真实。用于体积云、云雾粒子的材质参数。

Multiscattering Occlusion Factor：多重散射遮挡因子，云层对散射光的吸收。。用于体积云、云雾粒子的材质参数。

Shadow Tracing Distance：体积云阴影追踪距离。

SubNoise_Erosion：云层侵蚀噪点。用于体积云、云雾粒子的材质参数。

Phase G：第一相函数的分散量。用于体积云、云雾粒子的材质参数。

Phase G 2：第二相函数的分散量。用于体积云、云雾粒子的材质参数。

MultiScattering Eccentricity：多重散射偏心率。用于体积云、云雾粒子的材质参数。

Distance to Sample Max Count：体积云采样数量。控制台变量r.VolumetricCloud.DistanceToSampleMaxCount 参数。

Volumetric_Clouds_Scale：体积云缩放，用来缩放整个体积云的云场。

Tracing Max Start Distance：体积云追踪起始最大距离。

Tracing Max Distance：体积云追踪最大距离。

Enable Fog Inside Clouds：启动云雾粒子效果。

### 2D Dynamic Cloud Appearance 2D动态云外观

Soften Cloud Layer 1：第一层云的柔化边缘。

Soften Cloud Layer 2：第二层云的柔化边缘。

Sharpen Outer Edge：锐化外边缘。

Cloud Tiling_Layer 1：第一层云平铺缩放比例。

Cloud Tiling_Layer 2：第二层云平铺缩放比例。

One Cloud Layer：设置为1层云。通过切换材质实现。

Cloud Height Layer 1：云层高度。

Cloud Height Layer 2：云层高度。

Overcast Swirling Texture：当云密度高时，云上叠加的漩涡纹理量。

Cloud Noise Type：云层噪点形状。

Custom Noise Texture：自定义噪点纹理。

Sun Lighting Intensity：太阳光强度。

Shine Variation：云轮廓变化。

Sun Highlight Radius Curve：太阳高亮半径曲线。太阳高亮半径随太阳角度变化。

Sun_Highlight_Intensity_Curve：太阳高亮强度曲线。太阳高亮强度随太阳角度变化。

Shine Intensity Curve：轮廓强度变化曲线。

Cloud Opacity：云透明度。

### 2D Dynamic Cloud Distribution 2D动态云分布

Horizon_Density_Multiplier：接近地平线的云密度的乘数。

Zenith_Density_Multiplier：天空顶部云密度的乘数。

Latitude_Gradient_Position：纬度梯度位置。从“地平线”到“天顶”值的梯度高度，在上面混合。

Latitude_Gradient_Width：纬度梯度宽度。从“地平线”到“天顶”值的梯度宽度，在上面混合。

Around_Sun_Density_Multiplier：太阳周围区域云密度的乘数。

Around_Sun_Density_Exponent：定义太阳周围区域的径向梯度的指数。越高，半径越小。

Around_Moon_Density_Multiplier：月亮周围区域云密度的乘数。

Around_Moon_Density_Exponent：定义月亮周围区域的径向梯度的指数。越高，半径越小。

### Cloud Movement 云层移动

Cloud Speed：云移动速度。

Cloud Direction：云移动方向。

Cloud Phase：云的相位，由云移动速度与移动时间共同组成。

Randomize Cloud Formation on Run：开始运行时使用随机的云初始位置。

### Cloud Shadows 云层地面阴影

Use Cloud Shadows：使用云阴影。通过光照函数材质实现日月的云层阴影。

Cloud Shadows Scale：云阴影缩放。

Cloud Shadows Intensity When Sunny：云覆盖范围较低时云阴影强度。

Cloud Shadows Intensity When Overcast：云覆盖范围较高时云阴影强度。

Cloud Shadows Softness When Sunny：云覆盖范围较低时云阴影边缘柔化程度。

Cloud Shadows Softness When Overcast：云覆盖范围较高时云阴影边缘柔化程度。

Force Light Functions On：强制启动光照函数。避免低配置平台自动关闭光照函数。

### cloud wisp 远景云彩

Cloud Wisps Opacity：远景云彩透明度。

Cloud Wisps Texture：远景云彩纹理贴图。

### Moon 月亮

Moon Casts Shadows：月亮投射阴影。当开启这个功能且太阳未投射阴影且月亮在地平线上时投射阴影。

Moon Scale：月亮与月亮图案缩放。

Moon Inclination：月亮倾斜角度。

Moon Phase：月相，满月、半月、无月等。

Change Moon Phase Once a Day：每天修改一次月相。

Moon Intensity：月亮强度。

Moon Color：月光与月亮颜色。

Moon Orbit Offset：月亮轨道偏移，月亮出现的时机与消失的时机偏移。

Moonlight Intensity：月光强度。

Moon Rotation：月亮旋转，月亮内部纹理图案的旋转。

Custom Moon Texture：月亮纹理，月亮上的自定义图案。

Use Custom Moon Texture：启用自定义纹理。

Moon Glow Intensity：月亮辉光强度。

Moon Angle：月亮绕z轴旋转角度。

Moon_Volumetric_Scattering_Intensity：月光组件体积散射强度，与与层厚度共同作用。

Moon Vertical Offset：月亮垂直位置偏移。月亮绕x轴旋转角度。

### Stars 星星

Stars_Intensity：星星亮度。

Stars_Color：星星颜色。

Stars Speed：星星闪动与移动速度。

Stars_Tiling：星星纹理UV缩放。

Stars Texture：星星纹理。

Night Sky Glow：夜空辉光。

Night Sky Glow Color：夜空辉光光线颜色。

Stars Phase：星星相位，星空整体位置偏移。

Stars Intensity Curve：星光强度曲线，随太阳高度变化。

### Sky Light 天空光照

Recapture Sky light Periodically：定时重新捕获天光，注意重新捕获的开销极高。

Sky Light Mode：天光捕获模式：定时实时捕获、立方体贴图、根据立方体贴图与光源颜色动态调整。

Sky Light Intensity：天空光照强度，与云密度、夜晚时间、全局光线补偿等多种因素共同作用于真实的光照强度。

Flat Sunny Cubemap：使用立方体贴图与光源颜色动态调整的天光模式时，少云情况下使用的立方体贴图。

Flat Overcast Cubemap：使用立方体贴图与光源颜色动态调整的天光模式时，多云情况下使用的立方体贴图。

Sky Light Dynamic Tinting Color Curve：使用立方体贴图与光源颜色动态调整的天光模式时，随太阳角度变化的天空颜色，与云、月等多种因素共同决定真实的光源颜色。

Custom Cubemap：使用立方体贴图天光模式时的立方体纹理。

Sky Light Color：在实时捕获或立方体贴图的天光模式下的天光颜色。

Sky Light Lower Hemisphere Color：天光低半球的颜色。

Sky Light Recapture Period (Seconds)：定时实时捕获的天光模式下捕获时间间隔，仅受游戏时间影响。

Scale Skylight Intensity at Night：夜晚的天空光照强度缩放程度。

Scale Skylight Intensity when Cloudy：多云的天空光照强度缩放程度。

### Height Fog 高度雾

Fog_Density：少云时高度雾雾密度

Fog_Inscattering_Color_Curve：高度雾雾内散射颜色曲线，受太阳高度决定。与云密度、太阳角度、全局光照补偿、月相等共同影响。

Fog_Directional_Inscattering_Color_Curve：高度雾定向散射颜色曲线，受太阳高度决定。与云密度、太阳角度、全局光照补偿等共同影响。

Use Fog Density Curve：启用高度雾雾密度曲线，启用后覆盖少云时高度雾密度。

Fog Density Curve：少云时高度雾雾密度曲线，随TimeOfDay变化。

FullCloudiness_Fog_Density：多云时高度雾雾密度。

Fog_Density_Distribution：云层密度对高度雾雾密度的影响程度。

Use Volumetric Fog：在高度雾中启动体积雾。

### Aurora 极光

Use Auroras：启用极光效果、该效果开销较高。

Aurora Intensity：极光效果强度。

Aurora Speed：极光变化速度。

Aurora Phase：极光相位。

Aurora Color 1：极光颜色参数1。

Aurora Color 2：极光颜色参数2。

Aurora Color 3：极光颜色参数3。

Aurora Intensity Curve：极光强度强度曲线，受太阳角度影响，与默认极光强度共同作用。

Volumetric Aurora Sample Count Scale：极光体积云视图样本范围。

### Component Selection and Mobility 组件选择与修改

Sun Light：太阳光选择模式：关闭默认组件、启用默认组件、使用自定义组件。

Custom Sun Light Actor：自定义太阳光组件

Sun_Mobility：太阳光组件移动性：静态、固定、可移动

Moon Light：月光选择模式：关闭默认组件、启用默认组件、使用自定义组件。

Custom Moon Light Actor：自定义月光组件

Moon_Mobility：月光组件移动性：静态、固定、可移动

Sky_Light：天光选择模式：关闭默认组件、启用默认组件、使用自定义组件。

Custom Sky Light Actor：自定义天光组件

Sky_Light_Mobility：天光组件移动性：静态、固定、可移动

Height Fog：高度雾选择模式：关闭默认组件、启用默认组件、使用自定义组件。

Custom Height Fog Actor：自定义高度雾组件

### Legacy Coloring Settings 颜色设置

Manually Select Sun Color：启动固定的太阳颜色。

Horizon Base Color (Legacy Color Curve)：天空地平线方向颜色，随太阳高度变化。

Cloud Dark Color (Legacy Color Curve)：夜晚云层颜色，随太阳高度变化。

Cloud Light Color (Legacy Color Curve)：白天云层颜色，随太阳高度变化。

Sun Color (Legacy Color Curve)：太阳颜色，随太阳高度变化。

Sun Cloudy Color Curve：多云的太阳颜色，随太阳高度变化。

Zenith Base Color (Legacy Color Curve)：天空顶部方向颜色，随太阳高度变化。

Directional_Intensity_Curve：随太阳或月亮角度变化的太阳或光线强度变化曲线。

### Advanced Settings (Legacy) 高级设置

Automatically Set Advanced Legacy Settings using Time of Day：启用新版的一套效果参数设置，与过时设置不可同时使用。

Sun Color：默认太阳颜色。仅在2D动态云叠加颜色曲线模式下开启。

Sun Highlight Radius：太阳高亮半径。

Automatically Set Sun Light Rotation：自动设置太阳随时间旋转。

Automatically Set Moon Light Rotation：自动设置月亮随时间旋转。

### Sun 太阳

Sun Angle：太阳绕z轴旋转角度。

Sun Casts Shadows：太阳投射阴影开关。

Sun Disk Intensity：太阳光圈强度。

Sun Shader Intensity：太阳对云层的阴影强度。

Sun Radius：太阳半径。

Sun Inclination：太阳倾斜角度。

Sun_Light_Intensity：太阳光线强度，与太阳角度、云层等叠加产生效果。

Sun_Volumetric_Scattering_Intensity：太阳体积散射强度。

Extend Dawn and Dusk：额外的日出日落时间，对黄昏与黎明的效果显示时间延长，将太阳角度变换改为不按照TimeOfDay线性变化，而是按指数变化。

Sun_Vertical_Offset：太阳的高度偏移。

### Exposure 曝光

Use_Exposure_Range：启用后处理组件，并设置最大最小的曝光值（当最大值大于最小值时开启自动曝光）。

Exposure_Min：自动曝光强度最小值。

Exposure_Max：自动曝光强度最大值。

## 私有变量

### Component 组件

Sun_LightComponent：实际的太阳光组件。

Moon_LightComponent：实际的月光组件。

SkyLightComponent：实际的天光组件。

Height Fog Component：实际的高度雾组件。

### Dynamic Material Instance 动态材质实例

Sky_MID：用于天空球的动态材质实例，根据云层模式、仅1层云开关、极光开关在10种材质实例中选择。

Sun Cloud Shadows MID：太阳光的光照材质函数。

Moon Cloud Shadows MID：月光的光照材质函数。

Volumetric_Clouds_MID：体积云组件的云材质实例。

Static Clouds MID：静态云模型的材质实例

Volumetric_Aurora_MID：体积云极光组件的云材质实例。

Inside_Clouds_Fog_MID：云雾粒子系统的粒子材质参数。

### 其它变量

Sun Yaw：记录太阳垂直方向旋转角度。

Night Filter Curve：光线过滤曲线，光线强度随太阳角度变化。

Move_Static/Stationary_Lights：标识即将移动静态或固定光源。在构造函数中是为了便于编辑器预览可以移动静态光源组件。

Internal Time of Day：Time of Day会收到日出日落影响，并不能准确反应太阳或月亮的高度，这个参数将时间转换为标准的6点对应日出，18点对应日落。便于太阳与月亮随时间变化的角度计算。

Runtime：记录当前处于运行时，运行时云的运动参数与编辑器模式下运动参数存在差异。

Clouds_Time：记录云朵运动时间，运动时间与相位对应，存在随机的初始相位，仅在运行时有效。

# 函数说明

## 事件

**Event Tick**：每帧更新以下属性或设置：

- 昼夜循环(Time Of Day)：根据时间动画开关、太阳移动属性、Tick时间、时间流速、日出日落时间、白天夜晚长度等属性更新。
- 日出日落事件：根据日出日落时间、当前时间来触发日出日落事件。
- 月相改变：开启月相改变功能时，每过一天，月相会改变一点。
- 重新捕获天空光照：开启重新捕获时，每隔一段时间会重新捕获天光，光线效果更好。
- 刷新日夜旋转：开启旋转时，日月位置会随时间变化。
- 重新应用所有变量：参考Update Active Variables函数。

**Event Begin Play：**标识进入游戏，用于修改运动移动模式，初始化随机的云朵相位。根据Use Sky Mode Scalability Map选择自动的天空模式并执行构造脚本，或者仅初始化静态变量与动态变量关联的效果。

**Midnight：**午夜事件，用来标识一天的结束，用于日出日落事件更新。

## 函数

Construction Script：构造函数，调用了真正的构造脚本函数ConstructionScript_Function。

### 更新天空

Set Sun and Moon Rotation：根据TimeOfDay更新日月光线旋转。

Update Active Variables：构造函数与Tick时执行，根据变量每帧进行改变，执行流程如下：

- 根据太阳与月亮位置，设置天空球中太阳与月亮位置与角度参数。
- 更新云阴影方向、云层厚度、阴影强度、边缘强度等一系列参数。
- 更新太阳光盘强度参数。
- 更新月亮与星星强度、相位、颜色等参数。
- 更新整体光照强度
- 更新天空颜色、天空光照强度、星光颜色、极光强度等由曲线传递的参数。
- 更新太阳光源颜色、强度、体积散射、阴影、高光等属性。
- 更新高度雾颜色密度等参数。
- 更新月光源颜色、强度、体积散射、阴影、高光等属性。
- 更新天光颜色、强度。
- 更新天空大气组件各向异性、散射范围。
- 更新体积云密度、高度、形状等属性。

Set Cloud Timing：更新云的移动，编辑器模式下与运行模式下移动原理不同。

Update Static Variables：构造函数或BeginPlay时执行，执行流程如下：

- 在体积云模式下执行：初始化体积云与云雾材质参数，初始化体积云采样数量、起始追踪最大距离、追踪最大距离、视图样本数范围、阴影视图样本数范围、图层底部高度、阴影追踪距离。
- 初始化低半球天光颜色、月光颜色、体积雾开关、太阳体积散射强度。
- 初始化天空球材质太阳相关参数、静态云材质参数、根据配置启动强制开启云阴影。
- 初始化天空球材质云层、星光相关参数、初始化太阳与月亮的光线材质函数(决定云层阴影)。
- 初始化天空球材质月光、星光相关参数。
- 未使用体积极光模式下初始化天空球材质极光相关属性，启用体积极光模式下初始化体积极光材质参数。

### 工具集

Get Time of Day in Real Time Format：将TimeOfDay转换为UE标准的时间码格式并输出。

Set Time of Day using Time Code：输入标准的时间码，并修改TimeOfDay。

Set Sky Mode with Scalability Map：根据用户视觉效果质量设置，适配对应的天空模式。

### 系统

**ConstructionScript_Function：**构造函数或天空模式切换时调用，执行流程如下：

- 创建日光与月光的两个材质函数实例，用来模拟云阴影。
- 检查日出日落时间，保证日出早于日落。
- 设置天空大气组件可视性，仅在2D动态云叠加颜色曲线天空模式下不可视。
- 初始化静态云网格体可视性与静态云材质，仅在静态云模式下启用。
- 初始化体积云组件可视性，仅在体积云模式下启用。
- 根据天气模式初始化云层属性，根据函数传入参数启用。
- 初始化太阳光源组件，读取组件选择配置属性决定使用哪一个光源组件以及组件可视性。
- 初始化月亮光源组件，读取组件选择配置属性决定使用哪一个光源组件以及组件可视性。
- 初始化天空光源组件，读取组件选择配置属性以及天光捕获模式决定使用哪一个光源组件以及组件可视性。
- 初始化高度雾组件，读取组件选择配置属性决定使用哪一个高度雾组件。
- 创建天空球材质实例，根据是否启用极光、是否仅1层云、是否启用天空大气组件、哪一张天空模式决定使用哪一种材质去创建材质实例。
- 创建体积云与云雾粒子材质，仅在体积云模式下启用。
- 启动云雾粒子系统，根据天气模式、云雾开关等参数决定是否启用。
- 启动极光体积云组件，创建体积云材质，仅在极光模式下启用。
- 启用自动亮度后期处理，根据开关启用。
- 更新日月旋转、更新静态变量、更新动态变量，将天空球可视。参考Set Sun and Moon Rotation、Update Active Variables、Update Static Variables函数。
- 更新天空光源组件，根据天空捕获模式捕获天空或设置立方体贴图。

Update Variables Controlled by Weather：获取天气类的属性并更新云朵相关变量。

Get Reference to Weather BP：获取场景中的超动态天气类。

## 宏

Adjust color for night brightness：将输入颜色叠加上夜深程度，夜晚颜色值更大。

Night Curve：表示夜深程度，同Night Filter。

Use_Sky_Atmosphere：是否使用天空大气组件，仅在使用2D动态云叠加颜色曲线天空模式下不启用。

Find Overall Intensity：全局光线强度。

Calculate Night Brightness：根据云层厚度、默认夜晚亮度、多云时夜晚亮度计算夜晚亮度，用于月光组件光线强度、参与全局光线强度计算、参与夜晚颜色调节。

Night Filter：表示夜深程度。同Night Curve。

Flat Cloudiness：表示云层是否简单，云实现方式决定了其它不同效果的参数设置。

# 被天气系统修改的参数

Cloud Coverage(云覆盖范围)、Cloud Speed(云移动速度)、Cloud Direction(云移动方向)、执行构造函数(ConstructionScript_Function)。