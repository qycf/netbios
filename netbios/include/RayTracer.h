#pragma once
#include "UnrealStructs.h"
#include <vector>

// 射线追踪器类
class RayTracer
{
public:
    RayTracer();
    ~RayTracer();

    // 主要射线追踪接口
    // 执行单条射线追踪，检测从起点到终点是否有物体遮挡
    bool LineTraceSingle(
        const FVector& Start,
        const FVector& End,
        FHitResult& OutHit,
        ETraceTypeQuery TraceChannel = ETraceTypeQuery::TraceTypeQuery1,
        bool bTraceComplex = true,
        bool bIgnoreSelf = true
    );

    // 简化版本 - 仅返回是否碰撞
    bool LineTraceSingle(
        const FVector& Start,
        const FVector& End,
        ETraceTypeQuery TraceChannel = ETraceTypeQuery::TraceTypeQuery1
    );

    // 检查两点之间是否可见（无遮挡）
    bool IsVisible(
        const FVector& Start,
        const FVector& End
    );

    // 多重射线追踪（检测路径上的所有物体）
    bool LineTraceMulti(
        const FVector& Start,
        const FVector& End,
        std::vector<FHitResult>& OutHits,
        ETraceTypeQuery TraceChannel = ETraceTypeQuery::TraceTypeQuery1
    );

    // 设置忽略的Actor列表
    void SetIgnoredActors(const std::vector<AActor*>& Actors);
    void AddIgnoredActor(AActor* Actor);
    void ClearIgnoredActors();

    // 调试功能
    void SetDebugDraw(bool enable) { m_bDebugDraw = enable; }
    bool IsDebugDrawEnabled() const { return m_bDebugDraw; }

private:
    // 内部追踪实现
    bool InternalLineTrace(
        const FVector& Start,
        const FVector& End,
        FHitResult& OutHit,
        ETraceTypeQuery TraceChannel,
        bool bTraceComplex,
        const TArray<AActor*>& ActorsToIgnore,
        bool bIgnoreSelf
    );

    // 计算射线与物体的碰撞
    bool CalculateIntersection(
        const FVector& Start,
        const FVector& End,
        FHitResult& OutHit
    );

    // 检查Actor是否应该被忽略
    bool ShouldIgnoreActor(AActor* Actor) const;

private:
    std::vector<AActor*> m_IgnoredActors;    // 忽略的Actor列表
    bool m_bDebugDraw;                        // 是否启用调试绘制
    TArray<AActor*> m_InternalIgnoreList;    // 内部使用的忽略列表
};

// 可见性检测辅助类
class VisibilityChecker
{
public:
    // 检查两点之间是否有直接视线
    static bool HasLineOfSight(
        const FVector& From,
        const FVector& To,
        RayTracer& Tracer
    );

    // 检查是否在视锥内
    static bool IsInViewFrustum(
        const FVector& Point,
        const FVector& CameraLocation,
        const FVector& CameraForward,
        float FOV
    );

    // 计算两点之间的距离
    static float GetDistance(
        const FVector& From,
        const FVector& To
    );
};
