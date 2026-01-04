#pragma once
#include <cstdint>
#include <type_traits>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <array>

#include <cassert>
#include "Offsets.h"
#include "Enums.h"

// 命名空间封装，避免全局命名污染
namespace memory_helpers {
	/**
	 * @brief 检查指针是否在有效用户地址空间范围内
	 * @param ptr 待检查的指针
	 * @return 若地址有效返回true，否则返回false
	 */
	inline bool IsValidUserAddress(const void* ptr) noexcept {
		// 用户态地址范围检查（可根据实际平台调整）    
		const uintptr_t addr = reinterpret_cast<uintptr_t>(ptr);
		return addr >= 0x10000 && addr <= 0x7FFFFFF00000;
	}
	/**
	 * @brief 获取基地址偏移指定字节后的字段指针
	 * @tparam FieldType 字段类型
	 * @param baseAddress 基地址（起始地址）
	 * @param offset 偏移字节数（非0）
	 * @return 成功返回字段指针，失败返回nullptr
	 */
	template<class FieldType>
	FieldType* GetFieldPointer(void* baseAddress, int32_t offset) noexcept {
		// 基地址有效性检查    
		if (!baseAddress || !IsValidUserAddress(baseAddress)) {
			return nullptr;
		}
		// 偏移为0时返回空（避免无意义操作）    
		if (offset == 0) {
			reinterpret_cast<FieldType*>(baseAddress);
		}
		// 计算目标地址（使用uintptr_t避免指针运算溢出）    
		const uintptr_t targetAddr =
			reinterpret_cast<uintptr_t>(baseAddress) + static_cast<uintptr_t>(offset);
		// 检查目标地址有效性（避免越界访问）    
		if (!IsValidUserAddress(reinterpret_cast<void*>(targetAddr))) {
			return nullptr;
		}
		return reinterpret_cast<FieldType*>(targetAddr);
	}
	/**
	 * @brief 从基地址偏移处读取字段值
	 * @tparam FieldType 字段类型
	 * @param baseAddress 基地址
	 * @param offset 偏移字节数
	 * @return 成功返回字段值，失败返回FieldType的默认构造值
	 */
	template<class FieldType>
	FieldType ReadField(const void* baseAddress, int32_t offset) noexcept {
		// 这里将const指针转为非const是为了复用GetFieldPointer，实际不会修改数据    
		const FieldType* fieldPtr = GetFieldPointer<FieldType>(
			const_cast<void*>(baseAddress), offset
		);
		if (fieldPtr) {
			return *fieldPtr;
		}
		// 对于基本类型返回0值，对于类类型返回默认构造对象    
		return FieldType();
	}
	/**
	 * @brief 向基地址偏移处写入字段值
	 * @tparam FieldType 字段类型
	 * @param baseAddress 基地址
	 * @param offset 偏移字节数
	 * @param data 要写入的数据（会被移动以提高性能）
	 * @return 写入成功返回true，失败返回false
	 */
	template<class FieldType>
	bool WriteField(void* baseAddress, int32_t offset, FieldType data) noexcept {
		FieldType* fieldPtr = GetFieldPointer<FieldType>(baseAddress, offset);
		if (fieldPtr) {
			// 使用移动赋值减少拷贝（对于不可移动类型会自动退化为拷贝）
			*fieldPtr = std::move(data);
			return true;
		}
		return false;
	}
}  // namespace memory_helpers


// 带格式的宏定义，确保展开后有适当的换行和缩进
#define DEFINE_STATIC_CLASS_METHODS(ClassType) \
public: \
    /** 获取该类的静态类指针 */ \
    static UClass* StaticClass() \
    { \
        static UClass* StaticClassPtr = nullptr; \
        if (StaticClassPtr == nullptr) \
        { \
            StaticClassPtr = reinterpret_cast<UClass*>(FindObject(L#ClassType,1)); \
        } \
        return StaticClassPtr; \
    } \
    \
    /** 获取该类的默认对象 */ \
    static ClassType* GetDefaultObj() \
    { \
        return reinterpret_cast<ClassType*>(StaticClass()->get_DefaultObject()); \
    }


// 宏定义：生成返回指定类型的静态函数
#define DEFINE_STATIC_FUNCTION_GET(ReturnType, FuncName) \
static ReturnType FuncName() \
{ \
    static UFunction* Func = nullptr; \
    if (Func == nullptr) \
    { \
        Func = StaticClass()->GetFunction(L#FuncName); \
    } \
    struct \
    { \
        ReturnType ReturnValue; \
    } Parms{}; \
    GetDefaultObj()->ProcessEvent(Func, &Parms); \
    return Parms.ReturnValue; \
}

#define DEFINE_FUNCTION_GET(ReturnType, FuncName) \
ReturnType FuncName() \
{ \
    static UFunction* Func = nullptr; \
    if (Func == nullptr) \
    { \
        Func = StaticClass()->GetFunction(L#FuncName); \
    } \
    struct \
    { \
        ReturnType ReturnValue; \
    } Parms{}; \
    ProcessEvent(Func, &Parms); \
    return Parms.ReturnValue; \
}

#define DEFINE_PROPERTY_GET(ReturnType, PropertyName) \
    ReturnType get_##PropertyName() \
    { \
        static DWORD offset = 0; \
        if (offset == 0) \
        { \
            offset = StaticClass()->GetOffset(L#PropertyName); \
        } \
        return memory_helpers::ReadField<ReturnType>(this, offset); \
    } 

#define DEFINE_PROPERTY_POINTER(ReturnType, PropertyName) \
    ReturnType* get_##PropertyName##Address() \
    { \
        static DWORD offset = 0; \
        if (offset == 0) \
        { \
            offset = StaticClass()->GetOffset(L#PropertyName); \
        } \
        return memory_helpers::GetFieldPointer<ReturnType>(this, offset); \
    } 




struct FVector2D final
{
public:
	using UnderlayingType = float;

	float                                         X;                                                 // 0x0000(0x0004)(Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	float                                         Y;                                                 // 0x0004(0x0004)(Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)

public:
	constexpr FVector2D(UnderlayingType X = 0, UnderlayingType Y = 0)
		: X(X), Y(Y)
	{
	}
	constexpr FVector2D(const FVector2D& other)
		: X(other.X), Y(other.Y)
	{
	}
	FVector2D& Normalize()
	{
		*this /= Magnitude();

		return *this;
	}
	FVector2D& operator*=(UnderlayingType Scalar)
	{
		*this = *this * Scalar;

		return *this;
	}
	FVector2D& operator*=(const FVector2D& Other)
	{
		*this = *this * Other;

		return *this;
	}
	FVector2D& operator+=(const FVector2D& Other)
	{
		*this = *this + Other;

		return *this;
	}
	FVector2D& operator-=(const FVector2D& Other)
	{
		*this = *this - Other;

		return *this;
	}
	FVector2D& operator/=(UnderlayingType Scalar)
	{
		*this = *this / Scalar;

		return *this;
	}
	FVector2D& operator/=(const FVector2D& Other)
	{
		*this = *this / Other;

		return *this;
	}
	FVector2D& operator=(const FVector2D& other)
	{
		X = other.X;
		Y = other.Y;

		return *this;
	}

	UnderlayingType Dot(const FVector2D& Other) const
	{
		return (X * Other.X) + (Y * Other.Y);
	}
	UnderlayingType GetDistanceTo(const FVector2D& Other) const
	{
		FVector2D DiffVector = Other - *this;

		return DiffVector.Magnitude();
	}
	FVector2D GetNormalized() const
	{
		return *this / Magnitude();
	}
	bool IsZero() const
	{
		return X == 0 && Y == 0;
	}
	UnderlayingType Magnitude() const
	{
		return std::sqrt((X * X) + (Y * Y));
	}
	bool operator!=(const FVector2D& Other) const
	{
		return X != Other.X || Y != Other.Y;
	}
	FVector2D operator*(UnderlayingType Scalar) const
	{
		return { X * Scalar, Y * Scalar };
	}
	FVector2D operator*(const FVector2D& Other) const
	{
		return { X * Other.X, Y * Other.Y };
	}
	FVector2D operator+(const FVector2D& Other) const
	{
		return { X + Other.X, Y + Other.Y };
	}
	FVector2D operator-(const FVector2D& Other) const
	{
		return { X - Other.X, Y - Other.Y };
	}
	FVector2D operator/(UnderlayingType Scalar) const
	{
		if (Scalar == 0)
			return *this;

		return { X / Scalar, Y / Scalar };
	}
	FVector2D operator/(const FVector2D& Other) const
	{
		if (Other.X == 0 || Other.Y == 0)
			return *this;

		return { X / Other.X, Y / Other.Y };
	}
	bool operator==(const FVector2D& Other) const
	{
		return X == Other.X && Y == Other.Y;
	}
};
struct FVector
{
public:
	using UnderlayingType = float;

	float                                         X;                                                 // 0x0000(0x0004)(Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	float                                         Y;                                                 // 0x0004(0x0004)(Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	float                                         Z;                                                 // 0x0008(0x0004)(Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)

public:
	constexpr FVector(UnderlayingType X = 0, UnderlayingType Y = 0, UnderlayingType Z = 0)
		: X(X), Y(Y), Z(Z)
	{
	}
	constexpr FVector(const FVector& other)
		: X(other.X), Y(other.Y), Z(other.Z)
	{
	}
	FVector& Normalize()
	{
		*this /= Magnitude();

		return *this;
	}
	FVector& operator*=(UnderlayingType Scalar)
	{
		*this = *this * Scalar;

		return *this;
	}
	FVector& operator*=(const FVector& Other)
	{
		*this = *this * Other;

		return *this;
	}
	FVector& operator+=(const FVector& Other)
	{
		*this = *this + Other;

		return *this;
	}
	FVector& operator-=(const FVector& Other)
	{
		*this = *this - Other;

		return *this;
	}
	FVector& operator/=(UnderlayingType Scalar)
	{
		*this = *this / Scalar;

		return *this;
	}
	FVector& operator/=(const FVector& Other)
	{
		*this = *this / Other;

		return *this;
	}
	FVector& operator=(const FVector& other)
	{
		X = other.X;
		Y = other.Y;
		Z = other.Z;

		return *this;
	}

	UnderlayingType Dot(const FVector& Other) const
	{
		return (X * Other.X) + (Y * Other.Y) + (Z * Other.Z);
	}
	UnderlayingType GetDistanceTo(const FVector& Other) const
	{
		FVector DiffVector = Other - *this;

		return DiffVector.Magnitude();
	}
	UnderlayingType GetDistanceToInMeters(const FVector& Other) const
	{
		return GetDistanceTo(Other) * static_cast<UnderlayingType>(0.01);
	}
	FVector GetNormalized() const
	{
		return *this / Magnitude();
	}
	bool IsZero() const
	{
		return X == 0 && Y == 0 && Z == 0;
	}
	UnderlayingType Magnitude() const
	{
		return std::sqrt((X * X) + (Y * Y) + (Z * Z));
	}
	bool operator!=(const FVector& Other) const
	{
		return X != Other.X || Y != Other.Y || Z != Other.Z;
	}
	FVector operator*(UnderlayingType Scalar) const
	{
		return { X * Scalar, Y * Scalar, Z * Scalar };
	}
	FVector operator*(const FVector& Other) const
	{
		return { X * Other.X, Y * Other.Y, Z * Other.Z };
	}
	FVector operator+(const FVector& Other) const
	{
		return { X + Other.X, Y + Other.Y, Z + Other.Z };
	}
	FVector operator-(const FVector& Other) const
	{
		return { X - Other.X, Y - Other.Y, Z - Other.Z };
	}
	FVector operator/(UnderlayingType Scalar) const
	{
		if (Scalar == 0)
			return *this;

		return { X / Scalar, Y / Scalar, Z / Scalar };
	}
	FVector operator/(const FVector& Other) const
	{
		if (Other.X == 0 || Other.Y == 0 || Other.Z == 0)
			return *this;

		return { X / Other.X, Y / Other.Y, Z / Other.Z };
	}
	bool operator==(const FVector& Other) const
	{
		return X == Other.X && Y == Other.Y && Z == Other.Z;
	}
};
struct FRotator final
{
public:
	using UnderlayingType = float;

	float                                         Pitch;                                             // 0x0000(0x0004)(Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	float                                         Yaw;                                               // 0x0004(0x0004)(Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	float                                         Roll;                                              // 0x0008(0x0004)(Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)

public:
	static UnderlayingType ClampAxis(UnderlayingType Angle)
	{
		Angle = std::fmod(Angle, static_cast<UnderlayingType>(360));
		if (Angle < static_cast<UnderlayingType>(0))
			Angle += static_cast<UnderlayingType>(360);

		return Angle;
	}
	static UnderlayingType NormalizeAxis(UnderlayingType Angle)
	{
		Angle = ClampAxis(Angle);
		if (Angle > static_cast<UnderlayingType>(180))
			Angle -= static_cast<UnderlayingType>(360);

		return Angle;
	}

	FRotator& Clamp()
	{
		Pitch = ClampAxis(Pitch);
		Yaw = ClampAxis(Yaw);
		Roll = ClampAxis(Roll);

		return *this;
	}
	constexpr FRotator(UnderlayingType Pitch = 0, UnderlayingType Yaw = 0, UnderlayingType Roll = 0)
		: Pitch(Pitch), Yaw(Yaw), Roll(Roll)
	{
	}
	constexpr FRotator(const FRotator& other)
		: Pitch(other.Pitch), Yaw(other.Yaw), Roll(other.Roll)
	{
	}
	FRotator& Normalize()
	{
		Pitch = NormalizeAxis(Pitch);
		Yaw = NormalizeAxis(Yaw);
		Roll = NormalizeAxis(Roll);

		return *this;
	}
	FRotator& operator*=(UnderlayingType Scalar)
	{
		*this = *this * Scalar;

		return *this;
	}
	FRotator& operator*=(const FRotator& Other)
	{
		*this = *this * Other;

		return *this;
	}
	FRotator& operator+=(const FRotator& Other)
	{
		*this = *this + Other;

		return *this;
	}
	FRotator& operator-=(const FRotator& Other)
	{
		*this = *this - Other;

		return *this;
	}
	FRotator& operator/=(UnderlayingType Scalar)
	{
		*this = *this / Scalar;

		return *this;
	}
	FRotator& operator/=(const FRotator& Other)
	{
		*this = *this / Other;

		return *this;
	}
	FRotator& operator=(const FRotator& other)
	{
		Pitch = other.Pitch;
		Yaw = other.Yaw;
		Roll = other.Roll;

		return *this;
	}

	FRotator GetNormalized() const
	{
		FRotator rotator = *this;
		rotator.Normalize();

		return rotator;
	}
	bool IsZero() const
	{
		return ClampAxis(Pitch) == 0 && ClampAxis(Yaw) == 0 && ClampAxis(Roll) == 0;
	}
	bool operator!=(const FRotator& Other) const
	{
		return Pitch != Other.Pitch || Yaw != Other.Yaw || Roll != Other.Roll;
	}
	FRotator operator*(UnderlayingType Scalar) const
	{
		return { Pitch * Scalar, Yaw * Scalar, Roll * Scalar };
	}
	FRotator operator*(const FRotator& Other) const
	{
		return { Pitch * Other.Pitch, Yaw * Other.Yaw, Roll * Other.Roll };
	}
	FRotator operator+(const FRotator& Other) const
	{
		return { Pitch + Other.Pitch, Yaw + Other.Yaw, Roll + Other.Roll };
	}
	FRotator operator-(const FRotator& Other) const
	{
		return { Pitch - Other.Pitch, Yaw - Other.Yaw, Roll - Other.Roll };
	}
	FRotator operator/(UnderlayingType Scalar) const
	{
		if (Scalar == 0)
			return *this;

		return { Pitch / Scalar, Yaw / Scalar, Roll / Scalar };
	}
	FRotator operator/(const FRotator& Other) const
	{
		if (Other.Pitch == 0 || Other.Yaw == 0 || Other.Roll == 0)
			return *this;

		return { Pitch / Other.Pitch, Yaw / Other.Yaw, Roll / Other.Roll };
	}
	bool operator==(const FRotator& Other) const
	{
		return Pitch == Other.Pitch && Yaw == Other.Yaw && Roll == Other.Roll;
	}
};
struct alignas(0x10) FQuat final
{
public:
	float                                         X;                                                 // 0x0000(0x0004)(Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	float                                         Y;                                                 // 0x0004(0x0004)(Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	float                                         Z;                                                 // 0x0008(0x0004)(Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	float                                         W;                                                 // 0x000C(0x0004)(Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
};
struct FTransform final
{
public:
	struct FQuat                                  Rotation;                                          // 0x0000(0x0010)(Edit, BlueprintVisible, SaveGame, IsPlainOldData, NoDestructor, NativeAccessSpecifierPublic)
	struct FVector                                translation;                                       // 0x0010(0x000C)(Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	uint8                                         Pad_1C[0x4];                                       // 0x001C(0x0004)(Fixing Size After Last Property [ Dumper-7 ])
	struct FVector                                Scale3D;                                           // 0x0020(0x000C)(Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	uint8                                         Pad_2C[0x4];                                       // 0x002C(0x0004)(Fixing Struct Size After Last Property [ Dumper-7 ])
};

struct FHitResult
{
public:
	uint8                                         Pad_1[0x3];                                        // 0x0001(0x0003)(Fixing Size After Last Property [ Dumper-7 ])
	int32                                         FaceIndex;                                         // 0x0004(0x0004)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	float                                         Time;                                              // 0x0008(0x0004)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	float                                         Distance;                                          // 0x000C(0x0004)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	FVector							              Location;                                          // 0x0010(0x000C)(NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	FVector							              ImpactPoint;                                       // 0x001C(0x000C)(NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	FVector							              Normal;                                            // 0x0028(0x000C)(NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	FVector							              ImpactNormal;                                      // 0x0034(0x000C)(NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	FVector							              TraceStart;                                        // 0x0040(0x000C)(NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	FVector							              TraceEnd;                                          // 0x004C(0x000C)(NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	float                                         PenetrationDepth;                                  // 0x0058(0x0004)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	int32                                         Item;                                              // 0x005C(0x0004)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	char                                          pad_0028[0x0028];
};

class FLinearColor
{
public:
	float                                              R;                                                         // 0x0000(0x0004) (Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	float                                              G;                                                         // 0x0004(0x0004) (Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	float                                              B;                                                         // 0x0008(0x0004) (Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	float                                              A;                                                         // 0x000C(0x0004) (Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)

	FLinearColor()
		: R(0), G(0), B(0), A(0)
	{
	}

	FLinearColor(float r, float g, float b, float a)
		: R(r),
		G(g),
		B(b),
		A(a)
	{
	}
	FLinearColor(unsigned int hex)
		:R((hex & 0xFF) / 255.0f),
		G(((hex >> 8) & 0xFF) / 255.0f),
		B(((hex >> 16) & 0xFF) / 255.0f),
		A(((hex >> 24) & 0xFF) / 255.0f)
	{
		if (A == 0.0f) A = 1.0f;
	}
	FLinearColor(uintptr_t r, uintptr_t g, uintptr_t b, uintptr_t a)
		: R((float)r / 255.f),
		G((float)g / 255.f),
		B((float)b / 255.f),
		A((float)a / 255.f)
	{
	}
	FLinearColor(uintptr_t r, uintptr_t g, uintptr_t b)
		: R((float)r / 255.f),
		G((float)g / 255.f),
		B((float)b / 255.f),
		A(1.0f)
	{
	}
	unsigned int to_uint() {
		return (int(R * 255.0f) & 0xff) | ((int(G * 255.0f) & 0xff) << 8) | ((int(B * 255.0f) & 0xff) << 16) | ((int(A * 255.0f) & 0xff) << 24);
	}
};


struct FEquipmentInfo final
{
public:
	uint64                                        ItemID;                                            // 0x0000(0x0008)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	uint64                                        Gid;                                               // 0x0008(0x0008)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	float                                         Health;                                            // 0x0010(0x0004)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	float                                         MaxHealth;                                         // 0x0014(0x0004)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	float                                         Durability;                                        // 0x0018(0x0004)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	float                                         MaxDurability;                                     // 0x001C(0x0004)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	float                                         TotalEquipSeceonds;                                // 0x0020(0x0004)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	float                                         LastEquipTimeSeconds;                              // 0x0024(0x0004)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	float                                         TotalApplyDamage;                                  // 0x0028(0x0004)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	uint8                                         Pad_2C[0x4];                                       // 0x002C(0x0004)(Fixing Struct Size After Last Property [ Dumper-7 ])
};




enum class ESlateColorStylingMode : uint8
{
	UseColor_Specified = 0,
	UseColor_Specified_Link = 1,
	UseColor_Foreground = 2,
	UseColor_Foreground_Subdued = 3,
	UseColor_MAX = 4,
};

struct alignas(0x08) FSlateColor final
{
public:
	struct FLinearColor                           SpecifiedColor;                                    // 0x0000(0x0010)(Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData, NoDestructor, Protected, HasGetValueTypeHash, NativeAccessSpecifierProtected)
	ESlateColorStylingMode                        ColorUseRule;                                      // 0x0010(0x0001)(Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData, NoDestructor, Protected, HasGetValueTypeHash, NativeAccessSpecifierProtected)
	uint8                                         Pad_11[0x17];                                      // 0x0011(0x0017)(Fixing Struct Size After Last Property [ Dumper-7 ])
};

struct Mapinfo
{
	float X = 0.f;
	float Y = 0.f;
	float W = 0.f;
	float H = 0.f;
	int MapX = 0;
	int MapY = 0;
};