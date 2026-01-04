#include "pch.h"
#include "Class.h"


void UE::FName::AppendString(const FName* InName, FString* OutFstring) {
	if (fnAppendString == nullptr) {
		InitInternal();
	}
	reinterpret_cast<void(*)(const FName*, FString*)>(fnAppendString)(InName, OutFstring);
}
//UObject
bool UE::UObject::HasTypeFlag(EClassCastFlags TypeFlags)
{
	return (DWORD64(get_Class()->get_CastFlags()) & DWORD64(TypeFlags));
}
bool UE::UObject::IsA(EClassCastFlags TypeFlags) {
	return (DWORD64(get_Class()->get_CastFlags()) & DWORD64(TypeFlags));
}
bool  UE::UObject::IsA(UClass* TypeClass) {
	return get_Class()->IsSubclassOf(TypeClass);
}

void UE::UObject::Free(void* ptr) {
	if (fnFree == nullptr) {
		InitInternal();
	}
	reinterpret_cast<void(*)(void*)>(fnFree)(ptr);
}
UE::UObject* UE::UObject::StaticFindObject(UClass* Clss, UObject* Outer, const wchar_t* Nane, bool ExactClass) {
	if (fnStaticFindObject == nullptr) {
		InitInternal();
	}
	return reinterpret_cast<UObject * (*)(UClass*, UObject*, const wchar_t*, bool)>(fnStaticFindObject)(Clss, Outer, Nane, ExactClass);
};


void UE::UObject::ProcessEvent(UFunction* Func, void* Params) {
	void* vtf = memory_helpers::ReadField<void*>(this, 0);
	if (vtf) {
		void* fnProcessEvent = memory_helpers::ReadField<void*>(vtf, Off::ProcessEvent::PEIndex * sizeof(void*));
		if (fnProcessEvent) {
			if (Func->get_FunctionFlagsAddress()) {
				auto& Flags = *Func->get_FunctionFlagsAddress();
				auto oldFlags = Flags;
				Flags |= 0x400;
				reinterpret_cast<void(*)(UObject*, UFunction*, void*)>(fnProcessEvent)(this, Func, Params);
				Flags = oldFlags;
			}
		}
	}
}
//FString
inline std::string UE::FString::ToString() const
{
	if (this)
	{
		// 构建UTF-16宽字符串（包含所有有效元素）
		std::wstring utf16Str;
		utf16Str.reserve(ElementSize);  // 预分配空间提升性能
		utf16Str.assign(Data, ElementSize);  // 使用更安全的assign替代memcpy
		return us2_to_gbk(utf16Str);
	}
	return "";
}

