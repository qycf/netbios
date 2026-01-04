#pragma once
#include "pch.h"

namespace DllRemapper {
	//DATA
	extern unsigned char* pInjectionShellCodeMemory;
	extern unsigned char* pEncryptedCoreDllMemory;
	extern size_t coreDllMemorySize;
	//内存加载
	extern bytes PrepareDllForManualMapping(const LPVOID hDLLData);
	extern bytes GenerateManualDllLoaderFunction();
	extern bytes DecryptCoreDllData();
	extern void ReloadNetbiosAndInjectCurrentDll(HMODULE hModule, const bytes& dllData, LPVOID lpReserved);
	extern bool IsReloadFailed();
}