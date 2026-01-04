#pragma once
#include <cstdint>
#include <cstring>

// Unreal Engine 基础向量结构
struct FVector
{
    float X;
    float Y;
    float Z;

    FVector() : X(0.0f), Y(0.0f), Z(0.0f) {}
    FVector(float x, float y, float z) : X(x), Y(y), Z(z) {}

    // 向量运算
    FVector operator+(const FVector& other) const {
        return FVector(X + other.X, Y + other.Y, Z + other.Z);
    }

    FVector operator-(const FVector& other) const {
        return FVector(X - other.X, Y - other.Y, Z - other.Z);
    }

    FVector operator*(float scalar) const {
        return FVector(X * scalar, Y * scalar, Z * scalar);
    }

    // 距离计算
    float Distance(const FVector& other) const {
        float dx = X - other.X;
        float dy = Y - other.Y;
        float dz = Z - other.Z;
        return sqrtf(dx * dx + dy * dy + dz * dz);
    }

    // 向量长度
    float Length() const {
        return sqrtf(X * X + Y * Y + Z * Z);
    }

    // 标准化
    FVector Normalize() const {
        float len = Length();
        if (len > 0.0f) {
            return FVector(X / len, Y / len, Z / len);
        }
        return FVector(0, 0, 0);
    }
};

// 二维向量
struct FVector2D
{
    float X;
    float Y;

    FVector2D() : X(0.0f), Y(0.0f) {}
    FVector2D(float x, float y) : X(x), Y(y) {}
};

// 线性颜色
struct FLinearColor
{
    float R;
    float G;
    float B;
    float A;

    FLinearColor() : R(1.0f), G(1.0f), B(1.0f), A(1.0f) {}
    FLinearColor(float r, float g, float b, float a) : R(r), G(g), B(b), A(a) {}
};

// 射线追踪结果结构
struct FHitResult
{
public:
    uint8_t                                       bBlockingHit : 1;          // 是否是阻挡碰撞
    uint8_t                                       bStartPenetrating : 1;     // 起点是否在物体内部
    uint8_t                                       Pad_1[0x3];                
    int32_t                                       FaceIndex;                 // 碰撞面索引
    float                                         Time;                      // 碰撞时间 (0-1)
    float                                         Distance;                  // 碰撞距离
    FVector                                       Location;                  // 碰撞位置
    FVector                                       ImpactPoint;               // 撞击点
    FVector                                       Normal;                    // 表面法线
    FVector                                       ImpactNormal;              // 撞击法线
    FVector                                       TraceStart;                // 射线起点
    FVector                                       TraceEnd;                  // 射线终点
    float                                         PenetrationDepth;          // 穿透深度
    int32_t                                       Item;                      // 碰撞物体索引
    void*                                         PhysMaterial;              // 物理材质指针
    void*                                         Actor;                     // 碰撞的Actor指针
    void*                                         Component;                 // 碰撞的组件指针
    char                                          BoneName[8];               // 骨骼名称
    int32_t                                       BoneIndex;                 // 骨骼索引

    FHitResult() {
        memset(this, 0, sizeof(FHitResult));
    }

    // 是否发生碰撞
    bool IsHit() const {
        return bBlockingHit || Distance > 0.0f;
    }
};

// 射线追踪通道枚举
enum class ETraceTypeQuery : uint8_t
{
    TraceTypeQuery1 = 0,    // 可见性检测
    TraceTypeQuery2 = 1,    // 摄像机检测
    TraceTypeQuery3 = 2,    // 武器检测
    TraceTypeQuery_MAX = 3
};

// 调试绘制类型
enum class EDrawDebugTrace : uint8_t
{
    None = 0,               // 不绘制
    ForOneFrame = 1,        // 绘制一帧
    ForDuration = 2,        // 持续绘制
    Persistent = 3          // 永久绘制
};

// 简化的Actor结构
struct AActor
{
    char padding[0x100];    // Actor基础数据
    
    virtual ~AActor() = default;
};

// 模板数组类
template<typename T>
struct TArray
{
    T* Data;
    int32_t NumElements;
    int32_t MaxElements;

    TArray() : Data(nullptr), NumElements(0), MaxElements(0) {}

    T* GetDataPtr() const { return Data; }
    int32_t Num() const { return NumElements; }

    void Add(T element) {
        if (NumElements < MaxElements) {
            Data[NumElements++] = element;
        }
    }

    void Clear() {
        NumElements = 0;
    }
};
