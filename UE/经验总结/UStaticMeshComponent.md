SetStaticMesh 调用流程

1. 检查新Mesh是否与旧Mesh相同，相同则不必更新

2. 检查Mesh是否允许被更新，静态模型不允许更新，固定模型默认允许更新

3. 检查Mesh数据有效性

4. 在UActorComponent标记渲染状态为脏
   
   - bRenderStateDirty = true;
   
   - 在UWorld标记组件需要在帧尾更新MarkActorComponentForNeededEndOfFrameUpdate

5- 在UActorComponent更新物理状态

6- 确定模型是否影响导航网格

7- 在StreamingManager更新当前资产

8- 更新Bounds(包围体)

9- 在UMeshComponent标记材质索引为脏


