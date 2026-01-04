#include "../include/RayTracer.h"
#include <cmath>
#include <algorithm>

RayTracer::RayTracer()
    : m_bDebugDraw(false)
{
    // 初始化内部忽略列表
    m_InternalIgnoreList.Data = new AActor * [16];
    m_InternalIgnoreList.MaxElements = 16;
    m_InternalIgnoreList.NumElements = 0;
}

RayTracer::~RayTracer()
{
    if (m_InternalIgnoreList.Data) {
        delete[] m_InternalIgnoreList.Data;
        m_InternalIgnoreList.Data = nullptr;
    }
}

bool RayTracer::LineTraceSingle(
    const FVector& Start,
    const FVector& End,
    FHitResult& OutHit,
    ETraceTypeQuery TraceChannel,
    bool bTraceComplex,
    bool bIgnoreSelf)
{
    // 清空之前的结果
    memset(&OutHit, 0, sizeof(FHitResult));

    // 准备忽略列表
    TArray<AActor*> ignoreList;
    ignoreList.Data = new AActor * [m_IgnoredActors.size()];
    ignoreList.MaxElements = static_cast<int32_t>(m_IgnoredActors.size());
    ignoreList.NumElements = 0;

    for (AActor* actor : m_IgnoredActors) {
        ignoreList.Add(actor);
    }

    // 执行内部追踪
    bool result = InternalLineTrace(Start, End, OutHit, TraceChannel, bTraceComplex, ignoreList, bIgnoreSelf);

    // 清理
    delete[] ignoreList.Data;

    return result;
}

bool RayTracer::LineTraceSingle(
    const FVector& Start,
    const FVector& End,
    ETraceTypeQuery TraceChannel)
{
    FHitResult hitResult;
    return LineTraceSingle(Start, End, hitResult, TraceChannel, true, true);
}

bool RayTracer::IsVisible(const FVector& Start, const FVector& End)
{
    FHitResult hitResult;
    bool hit = LineTraceSingle(Start, End, hitResult, ETraceTypeQuery::TraceTypeQuery1, true, true);
    
    // 如果没有碰撞，说明可见
    return !hit || !hitResult.IsHit();
}

bool RayTracer::LineTraceMulti(
    const FVector& Start,
    const FVector& End,
    std::vector<FHitResult>& OutHits,
    ETraceTypeQuery TraceChannel)
{
    OutHits.clear();

    // 模拟多重追踪（实际应用中需要引擎支持）
    FHitResult hitResult;
    if (LineTraceSingle(Start, End, hitResult, TraceChannel, true, true)) {
        OutHits.push_back(hitResult);
        return true;
    }

    return false;
}

void RayTracer::SetIgnoredActors(const std::vector<AActor*>& Actors)
{
    m_IgnoredActors = Actors;
}

void RayTracer::AddIgnoredActor(AActor* Actor)
{
    if (Actor && !ShouldIgnoreActor(Actor)) {
        m_IgnoredActors.push_back(Actor);
    }
}

void RayTracer::ClearIgnoredActors()
{
    m_IgnoredActors.clear();
}

bool RayTracer::InternalLineTrace(
    const FVector& Start,
    const FVector& End,
    FHitResult& OutHit,
    ETraceTypeQuery TraceChannel,
    bool bTraceComplex,
    const TArray<AActor*>& ActorsToIgnore,
    bool bIgnoreSelf)
{
    // 填充射线起点和终点
    OutHit.TraceStart = Start;
    OutHit.TraceEnd = End;

    // 计算碰撞检测
    bool hit = CalculateIntersection(Start, End, OutHit);

    if (hit) {
        // 计算碰撞距离
        OutHit.Distance = OutHit.Location.Distance(Start);
        
        // 计算碰撞时间 (0-1)
        float totalDistance = End.Distance(Start);
        if (totalDistance > 0.0f) {
            OutHit.Time = OutHit.Distance / totalDistance;
        }

        OutHit.bBlockingHit = 1;
    }

    return hit;
}

bool RayTracer::CalculateIntersection(
    const FVector& Start,
    const FVector& End,
    FHitResult& OutHit)
{
    // 简化的碰撞检测算法
    // 实际应用中需要与游戏引擎集成
    
    FVector direction = End - Start;
    float distance = direction.Length();
    
    if (distance < 0.001f) {
        return false;
    }

    direction = direction.Normalize();

    // 模拟射线步进检测
    const int steps = 100;
    float stepSize = distance / steps;

    for (int i = 0; i < steps; ++i) {
        FVector currentPos = Start + (direction * (stepSize * i));
        
        // 这里需要实际的碰撞检测逻辑
        // 示例：检测是否在某个边界内
        
        // 暂时返回无碰撞
    }

    return false;
}

bool RayTracer::ShouldIgnoreActor(AActor* Actor) const
{
    if (!Actor) return true;

    for (const AActor* ignoredActor : m_IgnoredActors) {
        if (ignoredActor == Actor) {
            return true;
        }
    }

    return false;
}

// VisibilityChecker 实现

bool VisibilityChecker::HasLineOfSight(
    const FVector& From,
    const FVector& To,
    RayTracer& Tracer)
{
    return Tracer.IsVisible(From, To);
}

bool VisibilityChecker::IsInViewFrustum(
    const FVector& Point,
    const FVector& CameraLocation,
    const FVector& CameraForward,
    float FOV)
{
    FVector toPoint = Point - CameraLocation;
    toPoint = toPoint.Normalize();

    // 计算点积
    float dotProduct = toPoint.X * CameraForward.X + 
                       toPoint.Y * CameraForward.Y + 
                       toPoint.Z * CameraForward.Z;

    // 检查是否在视锥内
    float halfFOV = FOV * 0.5f * 3.14159f / 180.0f; // 转换为弧度
    float cosHalfFOV = cosf(halfFOV);

    return dotProduct >= cosHalfFOV;
}

float VisibilityChecker::GetDistance(const FVector& From, const FVector& To)
{
    return From.Distance(To);
}
