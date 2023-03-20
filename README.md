# FPSLearning

A project used to learn [Unreal Engine第一人称射击游戏完整制作流程](https://www.bilibili.com/video/BV1V34y117G1?t=92.0)

## Tip1(P28 Done)

When trying to get physics material through line trace, always remember to add code in line trace parameters like this

```cpp
FCollisionQueryParams Params;
Params.bReturnPhysicalMaterial = true;
```

## Tip2(P30)

The rocket launcher have some weird graphics clipping when aiming, the way to fix it is go to **ProjectSettings -> Engine -> GeneralSettings -> Settings** and change the **Near Clip Plane** number, set it to a small number, and the weird graphics clipping thing will be fixed.  
***Note that this requires a restart of editor to take effect.***

## Future Plan

- Merge this project with my previous [Inventory System](https://github.com/TDKArkham/SpatialInventory).  
- Replace the weapon assets with some other higher quality assets like [these](https://www.unrealengine.com/marketplace/zh-CN/profile/Rebel+Thorp?count=20&sortBy=effectiveDate&sortDir=DESC&start=0).  
- Optimize the logic and expend the project to make a S.T.A.L.K.E.R.-Fallout post-apocalypse game (A Fallout-like game but with a S.T.A.L.K.E.R. atmosphere).  
