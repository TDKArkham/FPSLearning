# FPSLearning
A project used to learn [Unreal Engine第一人称射击游戏完整制作流程](https://www.bilibili.com/video/BV1V34y117G1?t=92.0)

## Tip1(P28 Done):
Always remember to add this code in line trace parameters like this
```cpp
FCollisionQueryParams Params;
Params.bReturnPhysicalMaterial = true;
```
when you are trying to get physics material through line trace.
