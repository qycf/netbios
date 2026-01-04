#pragma once
#include "Basic.h"
#include "Utils/utils.h"
namespace UE {
	template<typename ArrayElementType>
	class TArray;
	class FString;
	class FText;
	class UObject;
	class UClass;
	class UFunction;

	class UEngine;
	class UWorld;
	class UCanvas;
	class AHUD;
	class UBaseUIController;
	class UBatchUIController;
	class UGameViewportClient;
	class APlayerState;

	class AActor;
	class AGPWeaponBase;
	class AWeaponBase;

	class UVisual;
	class UWidget;
	class UUserWidget;
	class UBaseUIView;
	class UMapBaseHUDView;
	class UMiniMapHUDView;
	class UMiniMapHUDViewSafeHouse;
	class UWBP_MiniMap_Mobile_C;
	class UBigMapHUDView;
	class USOLBigMapHUDView_Main;
	class UWBP_Map_Main_PC_C;

	//基础结构类开始
	namespace Iterators
	{
		template<typename ArrayType>
		class TArrayIterator
		{
		private:
			TArray<ArrayType>& IteratedArray;
			int32 Index;

		public:
			TArrayIterator(const TArray<ArrayType>& Array, int32 StartIndex = 0x0)
				: IteratedArray(const_cast<TArray<ArrayType>&>(Array)), Index(StartIndex)
			{
			}

		public:
			inline int32 GetIndex() { return Index; }

			inline int32 IsValid() { return IteratedArray.IsValidIndex(GetIndex()); }

		public:
			inline TArrayIterator& operator++() { ++Index; return *this; }
			inline TArrayIterator& operator--() { --Index; return *this; }

			inline       ArrayType& operator*() { return IteratedArray[GetIndex()]; }
			inline const ArrayType& operator*() const { return IteratedArray[GetIndex()]; }

			inline       ArrayType* operator->() { return &IteratedArray[GetIndex()]; }
			inline const ArrayType* operator->() const { return &IteratedArray[GetIndex()]; }

			inline bool operator==(const TArrayIterator& Other) const { return &IteratedArray == &Other.IteratedArray && Index == Other.Index; }
			inline bool operator!=(const TArrayIterator& Other) const { return &IteratedArray != &Other.IteratedArray || Index != Other.Index; }
		};
	}
	template<typename T> inline Iterators::TArrayIterator<T> begin(const TArray<T>& Array) { return Iterators::TArrayIterator<T>(Array, 0); }
	template<typename T> inline Iterators::TArrayIterator<T> end(const TArray<T>& Array) { return Iterators::TArrayIterator<T>(Array, Array.Num()); }

	template<typename ArrayElementType>
	class TArray
	{
	private:
		template<typename ArrayElementType>
		friend class TAllocatedArray;

		template<typename SparseArrayElementType>
		friend class TSparseArray;

	protected:
		static constexpr uint64 ElementAlign = alignof(ArrayElementType);
		static constexpr uint64 ElementSize = sizeof(ArrayElementType);

	public:
		ArrayElementType* Data;
		int32 NumElements;
		int32 MaxElements;

	public:
		TArray()
			: Data(nullptr), NumElements(0), MaxElements(0)
		{
		}

		TArray(const TArray&) = default;

		TArray(TArray&&) = default;

	public:
		TArray& operator=(TArray&&) = default;
		TArray& operator=(const TArray&) = default;

	private:
		inline int32 GetSlack() const { return MaxElements - NumElements; }

		inline void VerifyIndex(int32 Index) const { if (!IsValidIndex(Index)) throw std::out_of_range("Index was out of range!"); }

		inline       ArrayElementType& GetUnsafe(int32 Index) { return Data[Index]; }
		inline const ArrayElementType& GetUnsafe(int32 Index) const { return Data[Index]; }

	public:
		/* Adds to the array if there is still space for one more element */
		inline bool Add(const ArrayElementType& Element)
		{
			if (GetSlack() <= 0)
				return false;

			Data[NumElements] = Element;
			NumElements++;

			return true;
		}

		inline bool Remove(int32 Index)
		{
			if (!IsValidIndex(Index))
				return false;

			NumElements--;

			for (int i = Index; i < NumElements; i++)
			{
				/* NumElements was decremented, acessing i + 1 is safe */
				Data[i] = Data[i + 1];
			}

			return true;
		}

		inline void Clear()
		{
			NumElements = 0;

			if (!Data)
				memset(Data, 0, NumElements * ElementSize);
		}

	public:
		inline int32 Num() const { return NumElements; }
		inline int32 Max() const { return MaxElements; }

		inline const ArrayElementType* GetDataPtr() const { return Data; }

		inline bool IsValidIndex(int32 Index) const { return Data && Index >= 0 && Index < NumElements; }

		inline bool IsValid() const { return Data && NumElements > 0 && MaxElements >= NumElements; }

	public:
		inline       ArrayElementType& operator[](int32 Index) { VerifyIndex(Index); return Data[Index]; }
		inline const ArrayElementType& operator[](int32 Index) const { VerifyIndex(Index); return Data[Index]; }

		inline bool operator==(const TArray<ArrayElementType>& Other) const { return Data == Other.Data; }
		inline bool operator!=(const TArray<ArrayElementType>& Other) const { return Data != Other.Data; }

		inline explicit operator bool() const { return IsValid(); };

	public:
		template<typename T> friend Iterators::TArrayIterator<T> begin(const TArray& Array);
		template<typename T> friend Iterators::TArrayIterator<T> end(const TArray& Array);
	};
	class FString : public TArray<wchar_t>
	{
	public:
		friend std::ostream& operator<<(std::ostream& Stream, const FString& Str) { return Stream << Str.ToString(); }

	public:
		using TArray::TArray;

		FString(const wchar_t* Str)
		{
			const uint32 NullTerminatedLength = static_cast<uint32>(wcslen(Str) + 0x1);

			Data = const_cast<wchar_t*>(Str);
			NumElements = NullTerminatedLength;
			MaxElements = NullTerminatedLength;
		}

		FString(wchar_t* Str, int32 Num, int32 Max)
		{
			Data = Str;
			NumElements = Num;
			MaxElements = Max;
		}

	public:
		inline std::string ToString() const;

		inline std::wstring ToWString() const
		{
			if (*this)
				return std::wstring(Data);

			return L"";
		}

	public:
		inline       wchar_t* CStr() { return Data; }
		inline const wchar_t* CStr() const { return Data; }

	public:
		inline bool operator==(const FString& Other) const { return Other ? NumElements == Other.NumElements && wcscmp(Data, Other.Data) == 0 : false; }
		inline bool operator!=(const FString& Other) const { return Other ? NumElements != Other.NumElements || wcscmp(Data, Other.Data) != 0 : true; }
	};
	class FFreableString : public FString
	{
	public:
		using FString::FString;

		FFreableString(uint32_t NumElementsToReserve)
		{
			if (NumElementsToReserve > 0x1000000)
				return;

			this->Data = static_cast<wchar_t*>(malloc(sizeof(wchar_t) * NumElementsToReserve));
			this->NumElements = 0;
			this->MaxElements = NumElementsToReserve;
		}

		~FFreableString()
		{
			/* If we're using FName::ToString the allocation comes from the engine and we can not free it. Just leak those 2048 bytes. */
			//if (Off::InSDK::Name::bIsUsingAppendStringOverToString)
			FreeArray();
		}

	public:
		inline void ResetNum()
		{
			this->NumElements = 0;
		}

	private:
		inline void FreeArray()
		{
			this->NumElements = 0;
			this->MaxElements = 0;
			if (this->Data) free(this->Data);
			this->Data = nullptr;
		}
	};
	namespace FTextImpl
	{
		class FTextData final
		{
		public:
			FString TextSource;
		public:
			FString get_TextSource() {
				return  memory_helpers::ReadField<FString>(this, Off::Text::InTextDataStringOffset);
			}
		};
	}
	class FText final {
	public:
		FTextImpl::FTextData* TextData;
		uint8   Pad_8[0x10];
	public:
		FTextImpl::FTextData* get_TextData() {
			return  memory_helpers::ReadField<FTextImpl::FTextData*>(this, Off::Text::TextDatOffset);
		}
	};
	class FName {
	private:
		static inline void (*fnAppendString)(const FName*, FString&) = nullptr;
	public:
		int32 ComparisonIndex;
		int32 Number;
	public:
		static void AppendString(const FName* InName, FString* OutFstring);
	private:
		static void InitInternal() {
			fnAppendString = reinterpret_cast<decltype(fnAppendString)>((DWORD_PTR)GetModuleHandleA(nullptr) + Off::Name::AppendNameToString);
		}
		std::wstring ToRawWString() {
			FFreableString TempString(1024);
			AppendString(this, &TempString);
			std::wstring OutputString = TempString.ToWString();
			return OutputString;
		}
	public:
		std::wstring c_wstr() {
			return ToRawWString();
		}
		std::string c_str() {
			return us2_to_gbk(ToRawWString());
		}
	};
	class UObject {
	private:
		static inline UObject* (*fnStaticFindObject)(UClass*, UObject*, const wchar_t*, bool) = nullptr;
		static inline void (*fnFree)(void*) = nullptr;
		static void InitInternal() {
			fnStaticFindObject = reinterpret_cast<decltype(fnStaticFindObject)>((DWORD_PTR)GetModuleHandleA(nullptr) + Off::UObject::StaticFindObject);
			fnFree = reinterpret_cast<decltype(fnFree)>((DWORD_PTR)GetModuleHandleA(nullptr) + Off::UObject::UEFree);

		}
	public:
		static void Free(void* ptr);
		static UObject* StaticFindObject(UClass* Clss, UObject* Outer, const wchar_t* Nane, bool ExactClass = false);
	public:
		template<class T = UClass>
		static inline T* FindObject(UObject* Outer, const wchar_t* Name, bool ExactClass = false)
		{
			return StaticFindObject(T::StaticClass(), Outer, Name, ExactClass)->Cast<T>();
		}
		template<typename UEType = UObject>
		static inline UEType* FindObject(const wchar_t* Name, int offset = 0)
		{
			return StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), Name + offset, false)->Cast<UEType>();
		}


	public:
		std::wstring GetName() {
			return get_Name().c_wstr();
		}
	public:
		template<typename To>
		To* Cast() { return static_cast<To*>(this); }
		void* GetVTableFunc(int index) {
			void* vtf = memory_helpers::ReadField<void*>(this, 0);
			if (vtf) {
				void* func = memory_helpers::ReadField<void*>(vtf, index * sizeof(void*));
				if (func) {
					return func;
				}
			}
			return nullptr;
		}
	public:
		UClass* get_Class() {
			return memory_helpers::ReadField<UClass*>(this, Off::UObject::Class);
		}
		UObject* get_Outer() {
			return memory_helpers::ReadField<UObject*>(this, Off::UObject::Outer);
		}
		EObjectFlags get_Flags() {
			return memory_helpers::ReadField<EObjectFlags>(this, Off::UObject::Flags);
		}
		FName get_Name() {
			return memory_helpers::ReadField<FName>(this, Off::UObject::Name);
		}
		int32 get_Index() {
			return memory_helpers::ReadField<int32>(this, Off::UObject::Index);
		}
	public:
		bool HasTypeFlag(EClassCastFlags TypeFlags);
		bool IsA(EClassCastFlags TypeFlags);
		bool IsA(UClass* TypeClass);
	public:
		void ProcessEvent(UFunction* Func, void* Params);

	};
	class FField
	{
	public:
		FField* get_Next() {
			return memory_helpers::ReadField <FField*>(this, Off::FField::Next);
		}
		UClass* get_Class() {
			return memory_helpers::ReadField <UClass*>(this, Off::FField::Class);
		}
		FName get_Name() {
			return memory_helpers::ReadField <FName>(this, Off::FField::Name);
		}
		int32 get_ObjFlags() {
			return memory_helpers::ReadField <int32>(this, Off::FField::Flags);
		}
	};
	class FProperty : public FField
	{
	public:
		int32 get_ArrayDim() {
			return memory_helpers::ReadField <int32>(this, Off::Property::ArrayDim);
		}
		int32 get_ElementSize() {
			return memory_helpers::ReadField <int32>(this, Off::Property::ElementSize);
		}
		uint64 get_PropertyFlags() {
			return memory_helpers::ReadField <int32>(this, Off::Property::PropertyFlags);
		}
		int32 get_Offset() {
			return memory_helpers::ReadField <int32>(this, Off::Property::Offset_Internal);
		}
	};
	class UField : public UObject {
	public:
		UField* get_Next() {
			return memory_helpers::ReadField<UField*>(this, Off::UField::Next);
		}
	};
	class UStruct :public UField {
	public:
		int32 get_Size() {
			return memory_helpers::ReadField<int32>(this, Off::UStruct::Size);
		}
		UStruct* get_Super() {
			return memory_helpers::ReadField<UStruct*>(this, Off::UStruct::SuperStruct);
		}
		int16 get_MinAlignment() {
			return memory_helpers::ReadField<int16>(this, Off::UStruct::MinAlignment);
		}
		UField* get_Children() {
			return memory_helpers::ReadField<UField*>(this, Off::UStruct::Children);
		}
		FField* get_ChildProperties() {
			return memory_helpers::ReadField<FField*>(this, Off::UStruct::ChildProperties);
		}
		bool IsSubclassOf(const UStruct* Base) {
			if (!Base)return false;
			for (UStruct* Struct = this; Struct; Struct = Struct->get_Super())
			{
				if (Struct == Base)
					return true;
			}
			return false;
		}
	};
	class UFunction : public UStruct {
	public:
		using FNativeFuncPtr = void (*)(void* Context, void* TheStack, void* Result);
		uint32_t* get_FunctionFlagsAddress() {
			return memory_helpers::GetFieldPointer<uint32_t>(this, Off::UFunction::FunctionFlags);
		}
		FNativeFuncPtr get_ExecFunction() {
			return memory_helpers::ReadField<FNativeFuncPtr>(this, Off::UFunction::ExecFunction);
		}
	};
	class UClass :public UStruct {
	public:
		EClassCastFlags get_CastFlags() {
			return memory_helpers::ReadField<EClassCastFlags>(this, Off::UClass::CastFlags);
		}
		UObject* get_DefaultObject() {
			return memory_helpers::ReadField<UObject*>(this, Off::UClass::ClassDefaultObject);
		}
	public:
		UFunction* GetFunction(const std::wstring& FuncName) {
			for (UField* Field = this->get_Children(); Field; Field = Field->get_Next())
			{
				if (Field->HasTypeFlag(EClassCastFlags::Function) && Field->GetName() == FuncName)
					return static_cast<class UFunction*>(Field);
			}
			return nullptr;

		}
		int32 GetOffset(const std::wstring& PropertyName) {
			for (FProperty* Property = (FProperty*)this->get_ChildProperties(); Property; Property = (FProperty*)Property->get_Next())
			{
				auto Offset = Property->get_Offset();
				if (Offset > 0 && Offset < 0xFFFF && Property->get_Name().c_wstr() == PropertyName) {
					return Offset;
				}
			}
			return NULL;
		}
	};
	//基础结构和类结束

	struct FDFMCommonItemRow
	{
		char    Pad_0[0x10];
		FName	ItemID;
		char    pad_18[0x30];
		int32   GameItemType;
		uint8   Pad_4C[0x4];
		FText   ShortName;// 0x0050(0x0018)
		int32_t Quality;// 0x0068 4
		char    pad_6C[0x6C];
		int32_t InitialGuidePrice;// 0x00D8  4
	};
	struct FInventoryItemInfo
	{
		char pad_00[0x578];
		FDFMCommonItemRow* ItemRow;//0x540
		uint8  Pad_580[0x70];
	};
	struct alignas(0x08) FFastArraySerializer
	{
	public:
		uint8                                         Pad_0[0x54];                                       // 0x0000(0x0054)(Fixing Size After Last Property [ Dumper-7 ])
		int32                                         ArrayReplicationKey;                               // 0x0054(0x0004)(ZeroConstructor, IsPlainOldData, RepSkip, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		uint8                                         Pad_58[0xA8];                                      // 0x0058(0x00A8)(Fixing Size After Last Property [ Dumper-7 ])
		EFastArraySerializerDeltaFlags                DeltaFlags;                                        // 0x0100(0x0001)(ZeroConstructor, IsPlainOldData, RepSkip, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPrivate)
		uint8                                         Pad_101[0x7];                                      // 0x0101(0x0007)(Fixing Struct Size After Last Property [ Dumper-7 ])
	};
	struct FItemArray final : public FFastArraySerializer
	{
	public:
		TArray<struct FInventoryItemInfo>             Items;                                             // 0x0108(0x0010)(ZeroConstructor, NativeAccessSpecifierPublic)
		uint8                                         Pad_118[0x8];                                      // 0x0118(0x0008)(Fixing Struct Size After Last Property [ Dumper-7 ])
	};
	struct FPointDropItem final
	{
	public:
		uint64                                        ItemID;                                            // 0x0000(0x0008)(Edit, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		FString						                  Description;                                       // 0x0008(0x0010)(Edit, ZeroConstructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		uint32                                        Number;                                            // 0x0018(0x0004)(Edit, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		uint32                                        Rarity;                                            // 0x001C(0x0004)(Edit, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		uint64                                        SubItemId;                                         // 0x0020(0x0008)(Edit, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		uint32                                        SubItemNumber;                                     // 0x0028(0x0004)(Edit, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		uint32                                        Weight;                                            // 0x002C(0x0004)(Edit, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		uint32                                        Price;                                             // 0x0030(0x0004)(Edit, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		uint32                                        SellPrice;                                         // 0x0034(0x0004)(Edit, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		uint64                                        Gid;                                               // 0x0038(0x0008)(Edit, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		bool                                          bAIDiedDrop;                                       // 0x0040(0x0001)(Edit, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		uint8                                         Pad_41[0x3];                                       // 0x0041(0x0003)(Fixing Size After Last Property [ Dumper-7 ])
		uint32                                        MinNumber;                                         // 0x0044(0x0004)(Edit, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		uint32                                        MaxNumber;                                         // 0x0048(0x0004)(Edit, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		uint32                                        Durability;                                        // 0x004C(0x0004)(Edit, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		uint32                                        DurabilityLimit;                                   // 0x0050(0x0004)(Edit, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		uint32                                        MinDurability;                                     // 0x0054(0x0004)(Edit, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		uint32                                        MaxDurability;                                     // 0x0058(0x0004)(Edit, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		uint32                                        MinDurabilityLimit;                                // 0x005C(0x0004)(Edit, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		uint32                                        MaxDurabilityLimit;                                // 0x0060(0x0004)(Edit, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		uint8                                         Pad_64[0x1C];                                      // 0x0064(0x001C)(Fixing Struct Size After Last Property [ Dumper-7 ])
	};

	class UEngine :public UObject {
		DEFINE_STATIC_CLASS_METHODS(UEngine);
		DEFINE_PROPERTY_GET(UGameViewportClient*, GameViewport);
	};
	class UWorld :public UObject {
		DEFINE_STATIC_CLASS_METHODS(UWorld);
	};

	class UCanvas :public UObject {
		DEFINE_STATIC_CLASS_METHODS(UCanvas);
		DEFINE_PROPERTY_GET(int, SizeX);
		DEFINE_PROPERTY_GET(int, SizeY);
	};
	class UGameViewportClient :public UObject {
		DEFINE_STATIC_CLASS_METHODS(UGameViewportClient);
		DEFINE_PROPERTY_GET(UWorld*, World);
	};

	//地图控制器
	class UBaseUIController :public UObject {
		DEFINE_STATIC_CLASS_METHODS(UBaseUIController);
		DEFINE_PROPERTY_GET(UBaseUIView*, View);
	};
	class UBatchUIController : public UBaseUIController {
		DEFINE_STATIC_CLASS_METHODS(UBatchUIController);
		DEFINE_PROPERTY_GET(TArray<UBaseUIController*>, ChildControllers);


	};
	class UMapBaseHUDController : public UBaseUIController {
		DEFINE_STATIC_CLASS_METHODS(UMapBaseHUDController);
	};
	class USOLMapHUDController : public UMapBaseHUDController {
		DEFINE_STATIC_CLASS_METHODS(USOLMapHUDController);
	};
	//控件
	class UVisual : public UObject {
		DEFINE_STATIC_CLASS_METHODS(UVisual);
	};
	class UWidget : public UVisual {
		DEFINE_STATIC_CLASS_METHODS(UWidget);
		DEFINE_FUNCTION_GET(FVector2D, GetDesiredSize);
	};
	class UUserWidget : public UWidget {
		DEFINE_STATIC_CLASS_METHODS(UUserWidget);
		DEFINE_FUNCTION_GET(FVector2D, GetAlignmentInViewport);
	};
	class UBaseUIView : public UUserWidget {
		DEFINE_STATIC_CLASS_METHODS(UBaseUIView);

	};
	//地图控件
	class UMapBaseHUDView : public UBaseUIView {
		DEFINE_STATIC_CLASS_METHODS(UMapBaseHUDView);
		DEFINE_PROPERTY_GET(FVector, WorldCenter);
		DEFINE_PROPERTY_GET(FVector, WorldExtent);
		DEFINE_PROPERTY_GET(FVector2D, MapSize);
		DEFINE_PROPERTY_GET(float, CurrentScale);
		DEFINE_PROPERTY_GET(FVector2D, CurrentMapUVPos);
	};
	//小地图控件
	class UMiniMapHUDView : public UMapBaseHUDView {
		DEFINE_STATIC_CLASS_METHODS(UMiniMapHUDView);
	};
	class UMiniMapHUDViewSafeHouse : public UMiniMapHUDView {
		DEFINE_STATIC_CLASS_METHODS(UMiniMapHUDViewSafeHouse);
	};
	class UWBP_MiniMap_Mobile_C : public UMiniMapHUDView {
		DEFINE_STATIC_CLASS_METHODS(UWBP_MiniMap_Mobile_C);
	};
	//大地图控件
	class UBigMapHUDView : public UMapBaseHUDView {
		DEFINE_STATIC_CLASS_METHODS(UBigMapHUDView);
		DEFINE_PROPERTY_GET(bool, preShouldDraw);
		Mapinfo get_Mapinfo() {
			static DWORD offset = 0; if (offset == 0) {
				offset = StaticClass()->GetOffset(L"BigMapCanvas");
				if (offset > 0x10) offset -= 0x20;
			} return memory_helpers::ReadField<Mapinfo>(this, offset);
		}
	};
	class USOLBigMapHUDView_Main : public UBigMapHUDView {
		DEFINE_STATIC_CLASS_METHODS(USOLBigMapHUDView_Main);
	};
	class UWBP_Map_Main_PC_C :public USOLBigMapHUDView_Main {
		DEFINE_STATIC_CLASS_METHODS(UWBP_Map_Main_PC_C);
	};

	class UActorComponent : public UObject {
		DEFINE_STATIC_CLASS_METHODS(UActorComponent);
	};
	class USceneComponent :public UActorComponent {
		DEFINE_STATIC_CLASS_METHODS(USceneComponent);
		DEFINE_FUNCTION_GET(bool, IsVisible);
		DEFINE_FUNCTION_GET(FVector, K2_GetComponentLocation);
		DEFINE_FUNCTION_GET(FRotator, K2_GetComponentRotation);
		DEFINE_FUNCTION_GET(FVector, K2_GetComponentScale);
		DEFINE_FUNCTION_GET(FTransform, K2_GetComponentToWorld);
	};
	class UGPTeamComponent :public UActorComponent {
		DEFINE_STATIC_CLASS_METHODS(UGPTeamComponent);
		int32_t GetTeamID() {
			static UFunction* Func = nullptr;
			if (Func == nullptr) Func = StaticClass()->GetFunction(L"GetTeamID");
			struct {
				int32_t ReturnValue;
			}Parms{  };
			this->ProcessEvent(Func, &Parms);
			return Parms.ReturnValue;
		}
		int32_t GetCamp() {
			static UFunction* Func = nullptr;
			if (Func == nullptr) Func = StaticClass()->GetFunction(L"GetCamp");
			struct {
				int32_t ReturnValue;
			}Parms{  };
			this->ProcessEvent(Func, &Parms);
			return Parms.ReturnValue;
		}
	};
	class UGPAttributeBaseComponent :public UActorComponent {
		DEFINE_STATIC_CLASS_METHODS(UGPAttributeBaseComponent);
	};
	class UGPHealthDataComponent :public UGPAttributeBaseComponent {
		DEFINE_STATIC_CLASS_METHODS(UGPHealthDataComponent);
		float GetHealth() {
			static UFunction* Func = nullptr;
			if (Func == nullptr) {
				Func = StaticClass()->GetFunction(L"GetHealth");
			}
			struct Parameters {
				float ReturnValue;
			}Parms{};
			ProcessEvent(Func, &Parms);
			return Parms.ReturnValue;
		}
		float GetHealthMax() {
			static UFunction* Func = nullptr;
			if (Func == nullptr) {
				Func = StaticClass()->GetFunction(L"GetHealthMax");
			}
			struct Parameters {
				float ReturnValue;
			}Parms{};
			ProcessEvent(Func, &Parms);
			return Parms.ReturnValue;
		}
	};

	class UPrimitiveComponent :public USceneComponent {
		DEFINE_STATIC_CLASS_METHODS(UPrimitiveComponent);
	};
	class UMeshComponent :public UPrimitiveComponent {
		DEFINE_STATIC_CLASS_METHODS(UMeshComponent);
	};
	class USkinnedMeshComponent :public UMeshComponent {
		DEFINE_STATIC_CLASS_METHODS(USkinnedMeshComponent);
		int32 GetBoneIndex(FName BoneName) {
			static UFunction* Func = nullptr;
			if (Func == nullptr)Func = StaticClass()->GetFunction(L"GetBoneIndex");
			struct {
				FName BoneName;
				int32 ReturnValue;
			} Params = { BoneName };
			this->ProcessEvent(Func, &Params);
			return Params.ReturnValue;
		}
		FName GetBoneName(int32 BoneIndex) {
			static UFunction* Func = nullptr;
			if (Func == nullptr)Func = StaticClass()->GetFunction(L"GetBoneName");
			struct {
				int32 BoneIndex;
				FName ReturnValue;
			} Params = { BoneIndex };
			this->ProcessEvent(Func, &Params);
			return Params.ReturnValue;
		}
		void TransformFromBoneSpace(const FName& BoneName, const FVector& InPosition, const FRotator& InRotation, FVector* OutPosition, FRotator* OutRotation) {
			static UFunction* Func = nullptr;
			if (Func == nullptr)Func = StaticClass()->GetFunction(L"TransformFromBoneSpace");
			struct {
				FName		BoneName;
				FVector		InPosition;
				FRotator	InRotation;
				FVector		OutPosition;
				FRotator	OutRotation;
			} Parms{ BoneName ,InPosition ,InRotation };
			this->ProcessEvent(Func, &Parms);
			if (OutPosition != nullptr)*OutPosition = Parms.OutPosition;
			if (OutRotation != nullptr)*OutRotation = Parms.OutRotation;
		}
		FVector GetBonePositionByName(const FName& BoneName) {
			FVector OutPosition;
			TransformFromBoneSpace(BoneName, FVector(1.0f, 1.0f, 1.0f), FRotator(1.0f, 1.0f, 1.0f), &OutPosition, nullptr);
			return OutPosition;
		}
		FVector GetBonePositionByIndex(int32 BoneIndex) {
			FVector OutPosition;
			TransformFromBoneSpace(GetBoneName(BoneIndex), FVector(1.0f, 1.0f, 1.0f), FRotator(1.0f, 1.0f, 1.0f), &OutPosition, nullptr);
			return OutPosition;
		}

	};
	class USkeletalMeshComponent :public USkinnedMeshComponent {
		DEFINE_STATIC_CLASS_METHODS(USkinnedMeshComponent);
	};

	class UCharacterEquipComponent :public UActorComponent {
		DEFINE_STATIC_CLASS_METHODS(UCharacterEquipComponent);
		static UCharacterEquipComponent* Get(AActor* Owner) {
			static UFunction* Func = nullptr;
			if (Func == nullptr) {
				Func = StaticClass()->GetFunction(L"Get");
			} struct Parameters {
				AActor* Owner;
				UCharacterEquipComponent* ReturnValue;
			}; Parameters Parms{ Owner };
			GetDefaultObj()->ProcessEvent(Func, &Parms);
			return Parms.ReturnValue;
		}
		FEquipmentInfo GetEquipmentInfoByType(EEquipmentType Type) {
			static UFunction* Func = nullptr;
			if (Func == nullptr) {
				Func = StaticClass()->GetFunction(L"GetEquipmentInfoByType");
			} struct Parameters {
				EEquipmentType Type;
				FEquipmentInfo ReturnValue;
			}; Parameters Parms{ Type };
			ProcessEvent(Func, &Parms);
			return Parms.ReturnValue;
		}
	};

	class AActor :public UObject {
		DEFINE_STATIC_CLASS_METHODS(AActor);
		FVector GetVelocity() {
			static UFunction* Func = nullptr;
			if (Func == nullptr) Func = StaticClass()->GetFunction(L"GetVelocity");
			struct {
				FVector ReturnValue;
			}Parms{ };
			ProcessEvent(Func, &Parms);
			return Parms.ReturnValue;
		}
		USceneComponent* K2_GetRootComponent() {
			static UFunction* Func = nullptr;
			if (Func == nullptr) Func = StaticClass()->GetFunction(L"K2_GetRootComponent");
			struct {
				USceneComponent* ReturnValue;
			}Parms{ };
			ProcessEvent(Func, &Parms);
			return Parms.ReturnValue;
		}
		bool ActorHasTag(FName Tag) {
			static class UFunction* Func = nullptr;
			if (Func == nullptr)Func = StaticClass()->GetFunction(L"ActorHasTag");
			struct
			{
				FName Tag;
				bool ReturnValue;
			}Parms{ Tag };
			this->ProcessEvent(Func, &Parms);
			return Parms.ReturnValue;
		}
		DEFINE_PROPERTY_GET(TArray<FName>, Tags);
		DEFINE_PROPERTY_GET(TArray<FName>, Layers);
	};

	class AHUD :public AActor {
		DEFINE_STATIC_CLASS_METHODS(AHUD);
	};
	class ABaseHUD :public AHUD {
		DEFINE_STATIC_CLASS_METHODS(ABaseHUD);
		DEFINE_PROPERTY_GET(TArray<UBatchUIController*>, Controllers);

	};

	class AController :public AActor {
		DEFINE_STATIC_CLASS_METHODS(AController);
		DEFINE_PROPERTY_GET(FRotator, ControlRotation);
	};
	class APlayerController :public AController {
		DEFINE_STATIC_CLASS_METHODS(APlayerController);
		AHUD* GetHUD() {
			static UFunction* Func = nullptr;
			if (Func == nullptr) Func = StaticClass()->GetFunction(L"GetHUD");
			struct
			{
				AHUD* ReturnValue;
			} Parms{ };
			ProcessEvent(Func, &Parms);
			return Parms.ReturnValue;
		}
		bool ProjectWorldLocationToScreen(FVector WorldLocation, FVector2D& ScreenLocation, bool bPlayerViewportRelative = false) {
			static UFunction* Func = nullptr;
			if (Func == nullptr) Func = StaticClass()->GetFunction(L"ProjectWorldLocationToScreen");
			struct
			{
				FVector WorldLocation;
				FVector2D ScreenLocation;
				bool bPlayerViewportRelative;
				bool ReturnValue;
			} Parms{ WorldLocation ,FVector2D(),bPlayerViewportRelative };
			ProcessEvent(Func, &Parms);
			ScreenLocation = Parms.ScreenLocation;
			return Parms.ReturnValue;
		}
	};
	class ABasePlayerController :public APlayerController {
		DEFINE_STATIC_CLASS_METHODS(ABasePlayerController);
	};
	class AGPNetworkPlayerController :public ABasePlayerController {
		DEFINE_STATIC_CLASS_METHODS(AGPNetworkPlayerController);
	};
	class AGPPlayerController :public AGPNetworkPlayerController {
		DEFINE_STATIC_CLASS_METHODS(AGPPlayerController);
	};

	class APlayerCameraManager :public AActor {
		DEFINE_STATIC_CLASS_METHODS(APlayerCameraManager);
		FRotator GetCameraRotation() {
			static UFunction* Func = nullptr;
			if (Func == nullptr) Func = StaticClass()->GetFunction(L"GetCameraRotation");
			struct {
				FRotator ReturnValue;
			}Parms{  };
			this->ProcessEvent(Func, &Parms);
			return Parms.ReturnValue;
		}
		FVector GetCameraLocation() {
			static UFunction* Func = nullptr;
			if (Func == nullptr) Func = StaticClass()->GetFunction(L"GetCameraLocation");
			struct {
				FVector ReturnValue;
			}Parms{  };
			this->ProcessEvent(Func, &Parms);
			return Parms.ReturnValue;
		}
	};

	class UGPWeaponManagerComponent :public UActorComponent {
		DEFINE_STATIC_CLASS_METHODS(UGPWeaponManagerComponent);
	};
	class UWeaponManagerComponent :public UGPWeaponManagerComponent {
		DEFINE_STATIC_CLASS_METHODS(UWeaponManagerComponent);

	};

	class APawn :public AActor {
		DEFINE_STATIC_CLASS_METHODS(APawn);
		DEFINE_PROPERTY_GET(APlayerState*, PlayerState)
	};
	class ACHARACTER :public APawn {
		DEFINE_STATIC_CLASS_METHODS(ACHARACTER);
	};
	class AIntCharacter :public ACHARACTER {
		DEFINE_STATIC_CLASS_METHODS(AIntCharacter);
		bool IsDead() {
			static UFunction* Func = nullptr;
			if (Func == nullptr)Func = StaticClass()->GetFunction(L"IsDead");
			struct {
				bool ReturnValue;
			} Parms{ true };
			this->ProcessEvent(Func, &Parms);
			return Parms.ReturnValue;
		}
	};
	class ACharacterBase :public AIntCharacter {
		DEFINE_STATIC_CLASS_METHODS(ACharacterBase);
	};
	class AGPCharacterBase :public ACharacterBase {
		DEFINE_STATIC_CLASS_METHODS(AGPCharacterBase);
		bool IsAI() {
			static UFunction* Func = nullptr;
			if (Func == nullptr) Func = StaticClass()->GetFunction(L"IsAI");
			struct {
				bool ReturnValue;
			}Parms{  };
			this->ProcessEvent(Func, &Parms);
			return Parms.ReturnValue;
		}
		UGPTeamComponent* GetTeamComp() {
			static UFunction* Func = nullptr;
			if (Func == nullptr) Func = StaticClass()->GetFunction(L"GetTeamComp");
			struct {
				UGPTeamComponent* ReturnValue;
			}Parms{  };
			this->ProcessEvent(Func, &Parms);
			return Parms.ReturnValue;
		}
		UGPHealthDataComponent* GetHealthComp() {
			static UFunction* Func = nullptr;
			if (Func == nullptr) Func = StaticClass()->GetFunction(L"GetHealthComp");
			struct {
				UGPHealthDataComponent* ReturnValue;
			}Parms{  };
			this->ProcessEvent(Func, &Parms);
			return Parms.ReturnValue;
		}
		DEFINE_PROPERTY_GET(AWeaponBase*, CacheCurWeapon);
	};
	class AGPCharacter :public AGPCharacterBase {
		DEFINE_STATIC_CLASS_METHODS(AGPCharacter);
	};
	class ADFMCharacter :public AGPCharacter {
		DEFINE_STATIC_CLASS_METHODS(ADFMCharacter);
		DEFINE_PROPERTY_GET(FName, NpcName);
		DEFINE_PROPERTY_GET(FName, PoolName);
		DEFINE_PROPERTY_GET(FText, PoolChosenName);
		DEFINE_PROPERTY_GET(FString, PoolChosenName_Buffer);
		FString GetPoolChosenName() {
			static UFunction* Func = nullptr; if (Func == nullptr) {
				Func = StaticClass()->GetFunction(L"GetPoolChosenName");
			} struct Parameters {
				FString ReturnValue;;
			}; Parameters Parms{}; ProcessEvent(Func, &Parms); return Parms.ReturnValue;
		}
	};
	class ADFMPlayerCharacter :public ADFMCharacter {
		DEFINE_STATIC_CLASS_METHODS(ADFMPlayerCharacter);
	};
	class ADFMRangeTargetCharacter :public ADFMPlayerCharacter {
		DEFINE_STATIC_CLASS_METHODS(ADFMRangeTargetCharacter);
	};
	class ADFMAbilityBeacon :public ADFMCharacter {
		DEFINE_STATIC_CLASS_METHODS(ADFMAbilityBeacon);
	};
	class ADFMAbilityDFMVehicle :public ADFMCharacter {
		DEFINE_STATIC_CLASS_METHODS(ADFMAbilityDFMVehicle);
	};
	class ADFMAICharacter :public ADFMCharacter {
		DEFINE_STATIC_CLASS_METHODS(ADFMAICharacter);
	};
	class ADFMAIAnimalCharacter :public ADFMAICharacter {
		DEFINE_STATIC_CLASS_METHODS(ADFMAIAnimalCharacter);
	};
	class ABP_DFMCharacter_AI_C :public ADFMCharacter {
		DEFINE_STATIC_CLASS_METHODS(ABP_DFMCharacter_AI_C);
	};
	class ADFMAIBoss07RCVCharacter :public ADFMCharacter {
		DEFINE_STATIC_CLASS_METHODS(ADFMAIBoss07RCVCharacter);
	};
	class ADFMNPCCharacter :public ADFMCharacter {
		DEFINE_STATIC_CLASS_METHODS(ADFMNPCCharacter);
	};


	class AGPWeaponBase :public AActor {
		DEFINE_STATIC_CLASS_METHODS(AGPWeaponBase)

	};
	class AWeaponBase :public AGPWeaponBase {
		DEFINE_STATIC_CLASS_METHODS(AWeaponBase);
		DEFINE_PROPERTY_GET(uint64_t, WeaponID);
		DEFINE_PROPERTY_GET(uint64_t, WeaponGuid);
		DEFINE_PROPERTY_GET(bool, bIsSupportWeapon);

	};


	class AInfo :public AActor {
		DEFINE_STATIC_CLASS_METHODS(AInfo);
	};
	class APlayerState :public AInfo {
		DEFINE_STATIC_CLASS_METHODS(APlayerState);
		DEFINE_PROPERTY_GET(int32_t, PlayerId);
		DEFINE_PROPERTY_GET(APawn*, PawnPrivate);
		DEFINE_PROPERTY_GET(FString, PlayerNamePrivate);
		DEFINE_PROPERTY_GET(uint8, bIsABot);
	};
	class ABasePlayerState :public APlayerState {
		DEFINE_STATIC_CLASS_METHODS(ABasePlayerState);
	};
	class AGPPlayerState :public ABasePlayerState {
		DEFINE_STATIC_CLASS_METHODS(AGPPlayerState);
	};
	class ADFMPlayerState :public AGPPlayerState {
		DEFINE_STATIC_CLASS_METHODS(ADFMPlayerState);
		DEFINE_PROPERTY_GET(int64_t, HeroID);
		DEFINE_PROPERTY_GET(EExitState, ExitState);
	};

	class AGPInteractorBase :public AActor {
		DEFINE_STATIC_CLASS_METHODS(AGPInteractorBase);
	};
	class AInteractorBase :public AGPInteractorBase {
		DEFINE_STATIC_CLASS_METHODS(AInteractorBase);
		DEFINE_PROPERTY_GET(EMarkingItemType, MarkingItemType);
	};
	class APickupBase :public AInteractorBase {
		DEFINE_STATIC_CLASS_METHODS(APickupBase);
		DEFINE_PROPERTY_GET(uint64, InventoryGID);
		DEFINE_PROPERTY_GET(FName, InventoryIdName);
		DEFINE_FUNCTION_GET(FText, GetItemName);

	};
	class AInventoryPickup : public APickupBase {
		DEFINE_STATIC_CLASS_METHODS(AInventoryPickup);
		DEFINE_PROPERTY_POINTER(FInventoryItemInfo, PickupItemInfo);
	};
	class AInventoryPickup_Container :public AInventoryPickup {
		DEFINE_STATIC_CLASS_METHODS(AInventoryPickup_Container);
		DEFINE_PROPERTY_GET(uint32, BoxLevel);
		DEFINE_PROPERTY_POINTER(FItemArray, RepItemArray);
		DEFINE_FUNCTION_GET(FSlateColor, GetLootingTextColor);


	};
	class AInventoryPickup_OpenBox :public AInventoryPickup_Container {
		DEFINE_STATIC_CLASS_METHODS(AInventoryPickup_OpenBox);
		bool get_bHasOpened() {
			static DWORD offset = 0; if (offset == 0) {
				offset = StaticClass()->GetOffset(L"bHasOpened");
			} return memory_helpers::ReadField<bool>(this, offset);
		};
	};
	class AInventoryPickup_DeadBody : public AInventoryPickup_Container {
		DEFINE_STATIC_CLASS_METHODS(AInventoryPickup_DeadBody);
		DEFINE_PROPERTY_GET(FString, PlayerName_Buffer);
		DEFINE_PROPERTY_GET(FText, PlayerName);
		DEFINE_PROPERTY_GET(bool, bIsAI);
		DEFINE_PROPERTY_GET(AGPCharacterBase*, LootingCharacterOwner);
		DEFINE_PROPERTY_GET(APlayerState*, OwnerPlayerState);
		DEFINE_PROPERTY_GET(bool, bLooted);
		DEFINE_FUNCTION_GET(FString, GetPlayerName);




	};


	class AInteractorBaseProxy :public AInteractorBase {
		DEFINE_STATIC_CLASS_METHODS(AInteractorBaseProxy);

	};
	class AInteractor_HackPC :public AInteractorBaseProxy {
		DEFINE_STATIC_CLASS_METHODS(AInteractor_HackPC);
		DEFINE_PROPERTY_GET(uint32, boxId);
		DEFINE_PROPERTY_GET(int32, Password);
		DEFINE_FUNCTION_GET(bool, IsMorseCodeSuccess);

	};

	class AInteractor_CodedLock :public AInteractorBaseProxy {
		DEFINE_STATIC_CLASS_METHODS(AInteractor_CodedLock);
		DEFINE_PROPERTY_GET(TArray<int32>, FixConfigPwd);
		DEFINE_PROPERTY_GET(TArray<int32>, Pwd);
		DEFINE_PROPERTY_GET(uint32, DecalNumForOnePwd);
		DEFINE_FUNCTION_GET(bool, IsPassWordCorrect);
	};

	class AInteractor_ExitTrigger :public AInteractorBase {
		DEFINE_STATIC_CLASS_METHODS(AInteractor_ExitTrigger);

	};
	class ABP_Interact_ExitTrigger_C :public AInteractor_ExitTrigger {
		DEFINE_STATIC_CLASS_METHODS(ABP_Interact_ExitTrigger_C);

	};
	class ABP_ExitTrigger_Sluice_C :public ABP_Interact_ExitTrigger_C {
		DEFINE_STATIC_CLASS_METHODS(ABP_ExitTrigger_Sluice_C);
	};

	class AInteractor_ExitPayment :public AInteractorBase {
		DEFINE_STATIC_CLASS_METHODS(AInteractor_ExitPayment);
	};
	class ABP_Interactor_ExitPayment_C :public AInteractor_ExitPayment {
		DEFINE_STATIC_CLASS_METHODS(ABP_Interactor_ExitPayment_C);

	};

	class APlayerExitBase :public AActor {
		DEFINE_STATIC_CLASS_METHODS(APlayerExitBase);
		DEFINE_PROPERTY_GET(bool, bExitActive)
	};

	class AInteractor_SignalStation :public AInteractorBaseProxy {
		DEFINE_STATIC_CLASS_METHODS(AInteractor_SignalStation);
	};

	class UBlueprintFunctionLibrary :public UObject {
		DEFINE_STATIC_CLASS_METHODS(UBlueprintFunctionLibrary);

	};
	class UGPScalabilityBlueprintTools : public UBlueprintFunctionLibrary {
		DEFINE_STATIC_CLASS_METHODS(UGPScalabilityBlueprintTools);
		DEFINE_STATIC_FUNCTION_GET(EMainFlowState, GetMainFlowState);
	};
	class UGPWeaponBlueprintLibrary :public UBlueprintFunctionLibrary {
		DEFINE_STATIC_CLASS_METHODS(UGPWeaponBlueprintLibrary);
		static APlayerController* GetPlayerController(UObject* WorldContext) {
			static UFunction* Func = nullptr;
			if (Func == nullptr) Func = StaticClass()->GetFunction(L"GetPlayerController");
			struct {
				UObject* WorldContext;
				APlayerController* ReturnValue;
			}Parms{ WorldContext };
			GetDefaultObj()->ProcessEvent(Func, &Parms);
			return Parms.ReturnValue;
		}
		static APlayerCameraManager* GetPlayerCameraManager(UObject* WorldContext) {
			static UFunction* Func = nullptr;
			if (Func == nullptr) Func = StaticClass()->GetFunction(L"GetPlayerCameraManager");
			struct {
				UObject* WorldContext;
				APlayerCameraManager* ReturnValue;
			}Parms{ WorldContext };
			GetDefaultObj()->ProcessEvent(Func, &Parms);
			return Parms.ReturnValue;
		}
		static UGPWeaponManagerComponent* GetWeaponManagerComponent(AActor* Actor) {
			static UFunction* Func = nullptr;
			if (Func == nullptr) Func = StaticClass()->GetFunction(L"GetWeaponManagerComponent");
			struct {
				AActor* Actor;
				UGPWeaponManagerComponent* ReturnValue;
			}Parms{ Actor };
			GetDefaultObj()->ProcessEvent(Func, &Parms);
			return Parms.ReturnValue;
		}
	};
	class UGameplayBlueprintHelper :public UBlueprintFunctionLibrary {
		DEFINE_STATIC_CLASS_METHODS(UGameplayBlueprintHelper);
		static AGPPlayerController* GetLocalGPPlayerController(UObject* WorldContext) {
			static UFunction* Func = nullptr;
			if (Func == nullptr) Func = StaticClass()->GetFunction(L"GetLocalGPPlayerController");
			struct {
				UObject* WorldContext;
				AGPPlayerController* ReturnValue;
			}Parms{ WorldContext };
			GetDefaultObj()->ProcessEvent(Func, &Parms);
			return Parms.ReturnValue;
		}
		static AGPCharacter* GetLocalGPCharacter(class UObject* WorldContext) {
			static UFunction* Func = nullptr;
			if (Func == nullptr) Func = StaticClass()->GetFunction(L"GetLocalGPCharacter");
			struct {
				UObject* WorldContext;
				AGPCharacter* ReturnValue;
			}Parms{ WorldContext };
			GetDefaultObj()->ProcessEvent(Func, &Parms);
			return Parms.ReturnValue;
		}
	};
	class UGameplayStatics :public UBlueprintFunctionLibrary {
		DEFINE_STATIC_CLASS_METHODS(UGameplayStatics);
		static void GetAllActorsOfClass(UObject* WorldContextObject, UClass* ActorClass, TArray<AActor*>* OutActors) {
			static UFunction* Func = nullptr;
			if (Func == nullptr) Func = StaticClass()->GetFunction(L"GetAllActorsOfClass");
			struct {
				UObject* WorldContextObject;
				UClass* ActorClass;
				TArray<AActor*> OutActors;
			}Parms{ WorldContextObject,ActorClass };
			GetDefaultObj()->ProcessEvent(Func, &Parms);
			if (OutActors != nullptr)
				*OutActors = std::move(Parms.OutActors);
		}
	};
	class UKismetTextLibrary :public UBlueprintFunctionLibrary {
		DEFINE_STATIC_CLASS_METHODS(UKismetTextLibrary);
		static FString Conv_TextToString(const FText& InText) {
			static UFunction* Func = nullptr;
			if (Func == nullptr) Func = StaticClass()->GetFunction(L"Conv_TextToString");
			struct {
				FText	InText;
				FString ReturnValue;
			}Parms{ InText };
			GetDefaultObj()->ProcessEvent(Func, &Parms);
			return Parms.ReturnValue;
		}
		static FText Conv_StringToText(const FString& InString) {
			static UFunction* Func = nullptr;
			if (Func == nullptr) Func = StaticClass()->GetFunction(L"Conv_StringToText");
			struct {
				FString InString;
				FText	ReturnValue;
			}Parms{ InString };
			GetDefaultObj()->ProcessEvent(Func, &Parms);
			return Parms.ReturnValue;
		}
	};
	class UKismetStringLibrary : public UObject {
		DEFINE_STATIC_CLASS_METHODS(UKismetStringLibrary);
		static FString Conv_NameToString(FName InName) {
			static UFunction* Func = nullptr;
			if (Func == nullptr)Func = StaticClass()->GetFunction(L"Conv_NameToString");
			struct {
				FName InName;
				FString ReturnValue;
			} Params = { InName };
			GetDefaultObj()->ProcessEvent(Func, &Params);
			return Params.ReturnValue;
		}
		static FName Conv_StringToName(const FString& InString) {
			static UFunction* Func = nullptr;
			if (Func == nullptr)Func = StaticClass()->GetFunction(L"Conv_StringToName");
			struct {
				FString InString;
				FName ReturnValue;
			} Params = { InString };
			GetDefaultObj()->ProcessEvent(Func, &Params);
			return Params.ReturnValue;
		}
	};

	class UKismetSystemLibrary :public UBlueprintFunctionLibrary {
		DEFINE_STATIC_CLASS_METHODS(UKismetSystemLibrary);
		static bool LineTraceSingle(UObject* WorldContextObject, const FVector& Start, const FVector& End, ETraceTypeQuery TraceChannel, bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, EDrawDebugTrace DrawDebugType, FHitResult* OutHit, bool bIgnoreSelf, const FLinearColor& TraceColor, const FLinearColor& TraceHitColor, float DrawTime) {
			static UFunction* Func = nullptr;
			if (Func == nullptr)Func = StaticClass()->GetFunction(L"LineTraceSingle");
			struct
			{
				UObject* WorldContextObject;
				FVector                 Start;
				FVector                 End;
				ETraceTypeQuery         TraceChannel;
				bool                    bTraceComplex;
				unsigned char           UnknownData_H2FF[0x2];
				TArray<AActor*>         ActorsToIgnore;
				EDrawDebugTrace         DrawDebugType;
				unsigned char           UnknownData_DZIR[0x3];
				FHitResult              OutHit;
				bool                    bIgnoreSelf;
				unsigned char           UnknownData_ACTN[0x3];
				FLinearColor            TraceColor;
				FLinearColor            TraceHitColor;
				float                   DrawTime;
				bool					ReturnValue;
			}Parms{};
			Parms.WorldContextObject = WorldContextObject;
			Parms.Start = Start;
			Parms.End = End;
			Parms.TraceChannel = TraceChannel;
			Parms.bTraceComplex = bTraceComplex;
			Parms.ActorsToIgnore = ActorsToIgnore;
			Parms.DrawDebugType = DrawDebugType;
			Parms.bIgnoreSelf = bIgnoreSelf;
			Parms.TraceColor = TraceColor;
			Parms.TraceHitColor = TraceHitColor;
			Parms.DrawTime = DrawTime;
			GetDefaultObj()->ProcessEvent(Func, &Parms);
			if (OutHit != nullptr)*OutHit = Parms.OutHit;
			return Parms.ReturnValue;
		}
		static bool LineTraceSingle(UObject* WorldContextObject, AActor* LocalGPCharacter, AActor* ActorsToIgnore, const FVector& TraceStart, const FVector& TraceEnd, FHitResult& UnKnowFHitResult) {
			thread_local TArray<AActor*> ActorsToIgnores;
			if (ActorsToIgnores.GetDataPtr() == nullptr) {
				ActorsToIgnores.Data = new AActor * [2];
				ActorsToIgnores.MaxElements = 2;
			}
			ActorsToIgnores.NumElements = 0;
			ActorsToIgnores.Add(LocalGPCharacter);
			ActorsToIgnores.Add(ActorsToIgnore);
			return LineTraceSingle(WorldContextObject, TraceStart, TraceEnd, ETraceTypeQuery::TraceTypeQuery1, true, ActorsToIgnores, EDrawDebugTrace::None, &UnKnowFHitResult, true, FLinearColor(1.f, 0.f, 0.f, 1.f), FLinearColor(0.f, 1.f, 0.f, 1.f), 5.0f);
		}
		static bool LineTraceSingle(UObject* WorldContextObject, AActor* LocalGPCharacter, AActor* ActorsToIgnore, const FVector& TraceStart, const FVector& TraceEnd) {
			FHitResult UnKnowFHitResult;
			memset(&UnKnowFHitResult, 0, sizeof(UnKnowFHitResult));
			return LineTraceSingle(WorldContextObject, LocalGPCharacter, ActorsToIgnore, TraceStart, TraceEnd, UnKnowFHitResult);
		}
	};
	class UMTAPI_UEngine :public UObject {
		DEFINE_STATIC_CLASS_METHODS(UMTAPI_UEngine);
		DEFINE_STATIC_FUNCTION_GET(UEngine*, GetEngine);
	};
	class UMTAPI_APlayerState :public UObject {
		DEFINE_STATIC_CLASS_METHODS(UMTAPI_APlayerState);
		static APawn* GetPawn(APlayerState* Self) {
			static UFunction* Func = nullptr;
			if (Func == nullptr) {
				Func = StaticClass()->GetFunction(L"GetPawn");
			}
			struct {
				APlayerState* Self;
				APawn* ReturnValue;
			} Parms{ Self };
			GetDefaultObj()->ProcessEvent(Func, &Parms);
			return Parms.ReturnValue;
		};
	};
	class UMTAPI_ACharacter : public UObject {
		DEFINE_STATIC_CLASS_METHODS(UMTAPI_ACharacter);
		static USkeletalMeshComponent* GetMesh(ACHARACTER* Self) {
			static UFunction* Func = nullptr;
			if (Func == nullptr) {
				Func = StaticClass()->GetFunction(L"GetMesh");
			}
			struct {
				ACHARACTER* Self;
				USkeletalMeshComponent* ReturnValue;
			} Parms{ Self };
			GetDefaultObj()->ProcessEvent(Func, &Parms);
			return Parms.ReturnValue;
		};


	};


}