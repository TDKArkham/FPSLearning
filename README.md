# FPSLearning

A project used to learn [Unreal Engine第一人称射击游戏完整制作流程](https://www.bilibili.com/video/BV1V34y117G1?t=92.0)

## Tip1(P28 Done)

When trying to get physics material through line trace, always remember to add code in line trace parameters like this

```cpp
FCollisionQueryParams Params;
Params.bReturnPhysicalMaterial = true;
```

## Tip2(P30)

The rocket launcher have some weird clipping when aiming, the way to fix it is go to **ProjectSettings -> Engine -> GeneralSettings -> Settings** and change the **Near Clip Plane** number, set it to a small number, and the weird clipping thing will be fixed.
***Note that it requires a restart of editor to take effect.***
