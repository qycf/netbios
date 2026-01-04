# 射线追踪（Ray/Line Trace）独立项目

## 项目说明
本项目从原游戏辅助工具中提取了射线追踪功能，可用于学习和研究 Unreal Engine 的射线检测机制。

## 主要功能
- **LineTraceSingle**: 单条射线追踪，检测从起点到终点之间是否有物体遮挡
- **FHitResult**: 存储射线碰撞结果的数据结构
- **可见性检测**: 判断两个物体之间是否有视线（Line of Sight）

## 核心组件
- `RayTracer.h/cpp`: 射线追踪核心逻辑
- `UnrealStructs.h`: Unreal Engine 基础数据结构
- `main.cpp`: 示例用法

## 编译要求
- Visual Studio 2019 或更高版本
- Windows SDK 10.0
- C++17 标准

## 使用示例
```cpp
// 创建射线追踪器
RayTracer tracer;

// 设置起点和终点
FVector start(0, 0, 0);
FVector end(100, 0, 0);

// 执行射线追踪
FHitResult hitResult;
bool hit = tracer.LineTraceSingle(start, end, hitResult);

if (hit) {
    // 检测到碰撞
    printf("Hit at distance: %.2f\n", hitResult.Distance);
}
```

## 注意事项
⚠️ 本项目仅用于学习和研究目的，不应用于任何违反游戏服务条款的行为。
