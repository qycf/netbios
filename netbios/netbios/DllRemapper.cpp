#include "DllRemapper.h"

//DATA
unsigned char* DllRemapper::pInjectionShellCodeMemory = nullptr;
unsigned char* DllRemapper::pEncryptedCoreDllMemory = nullptr;
size_t DllRemapper::coreDllMemorySize = 0;
//内存加载
bool DllRemapper::IsReloadFailed() {
	return (pInjectionShellCodeMemory == nullptr || pEncryptedCoreDllMemory == nullptr || coreDllMemorySize == 0);
}
bytes DllRemapper::PrepareDllForManualMapping(const LPVOID hDLLData)
{

	static auto IsValidPE = [](const LPVOID lpImage) -> BOOL {
		const auto lpImageDOSHeader = (PIMAGE_DOS_HEADER)lpImage;
		const auto lpImageNTHeader = (PIMAGE_NT_HEADERS)((DWORD_PTR)lpImageDOSHeader + lpImageDOSHeader->e_lfanew);
		if (lpImageNTHeader->Signature == IMAGE_NT_SIGNATURE)
			return TRUE;

		return FALSE;
		};
	static auto IsDLL = [](const LPVOID hDLLData)->BOOL {
		const auto lpImageDOSHeader = (PIMAGE_DOS_HEADER)(hDLLData);
		const auto lpImageNtHeader = (PIMAGE_NT_HEADERS32)((DWORD_PTR)hDLLData + lpImageDOSHeader->e_lfanew);

		if (lpImageNtHeader->FileHeader.Characteristics & IMAGE_FILE_DLL)
			return TRUE;

		return FALSE;
		};
	static auto IsValidArch = [](const LPVOID lpImage)->BOOL {
		const auto lpImageDOSHeader = (PIMAGE_DOS_HEADER)lpImage;
		const auto lpImageNTHeader = (PIMAGE_NT_HEADERS)((DWORD_PTR)lpImageDOSHeader + lpImageDOSHeader->e_lfanew);
		if (lpImageNTHeader->OptionalHeader.Magic == IMAGE_NT_OPTIONAL_HDR_MAGIC)
			return TRUE;
		return FALSE;
		};
	static auto GetImageSize = [](const LPVOID lpImage)->DWORD_PTR {
		const auto lpImageDOSHeader = (PIMAGE_DOS_HEADER)lpImage;
		const auto lpImageNTHeader = (PIMAGE_NT_HEADERS)((DWORD_PTR)lpImageDOSHeader + lpImageDOSHeader->e_lfanew);
		return lpImageNTHeader->OptionalHeader.SizeOfImage;
		};


	if (hDLLData == INVALID_HANDLE_VALUE || hDLLData == nullptr)
	{
		return {};
	}


	if (!IsValidPE(hDLLData) || !IsDLL(hDLLData) || !IsValidArch(hDLLData))
	{
		return {};
	}

	const DWORD_PTR dImageSize = GetImageSize(hDLLData);

	const LPVOID lpAllocAddress = VirtualAlloc(nullptr, dImageSize, MEM_COMMIT | MEM_RESERVE | MEM_TOP_DOWN, PAGE_EXECUTE_READWRITE);
	if (lpAllocAddress == nullptr)
	{
		return {};
	}

	const auto lpImageDOSHeader = (PIMAGE_DOS_HEADER)hDLLData;
	const auto lpImageNTHeader = (PIMAGE_NT_HEADERS)((DWORD_PTR)lpImageDOSHeader + lpImageDOSHeader->e_lfanew);
	const auto lpImageSectionHeader = (PIMAGE_SECTION_HEADER)((DWORD_PTR)lpImageNTHeader + FIELD_OFFSET(IMAGE_NT_HEADERS, OptionalHeader) + lpImageNTHeader->FileHeader.SizeOfOptionalHeader);

	// 写入 PE 头
	RtlCopyMemory(lpAllocAddress, hDLLData, lpImageNTHeader->OptionalHeader.SizeOfHeaders);

	// 写入节区数据
	for (int i = 0; i < lpImageNTHeader->FileHeader.NumberOfSections; i++)
	{
		const auto lpCurrentSectionHeader = (PIMAGE_SECTION_HEADER)((DWORD_PTR)lpImageSectionHeader + (i * sizeof(IMAGE_SECTION_HEADER)));
		RtlCopyMemory((LPVOID)((DWORD_PTR)lpAllocAddress + lpCurrentSectionHeader->VirtualAddress), (LPVOID)((DWORD_PTR)hDLLData + lpCurrentSectionHeader->PointerToRawData), lpCurrentSectionHeader->SizeOfRawData);
	}
	//MANUAL_MAPPING_DATA
	struct ManualMapParameter
	{
		FARPROC pGetModuleHandleA;
		FARPROC pLoadLibraryA;
		FARPROC pGetProcAddress;
#ifdef _WIN64
		FARPROC pRtlAddFunctionTable;
#endif
		PVOID lpImageBase;
		HINSTANCE hModule;
		PBYTE m_image;
		BOOL useMain = TRUE;
		PVOID OnCallBack = nullptr;
	}requset_data = { 0 };
	// 构建 MANUAL_MAPPING_DATA 结构体
	requset_data.useMain = TRUE;
	requset_data.pLoadLibraryA = (FARPROC)::LoadLibraryA;
	requset_data.pGetProcAddress = (FARPROC)::GetProcAddress;
	requset_data.pGetModuleHandleA = (FARPROC)::GetModuleHandleA;
#ifdef _WIN64
	requset_data.pRtlAddFunctionTable = (FARPROC)::RtlAddFunctionTable;
#endif
	requset_data.lpImageBase = lpAllocAddress;
	bytes bs(sizeof(requset_data));
	memcpy(bs.data(), &requset_data, sizeof(requset_data));

	return bs;
}
bytes DllRemapper::GenerateManualDllLoaderFunction() {
	return { 72,129,236,40,1,0,0,72,137,140,36,32,1,0,0,72,131,188,36,32,1,0,0,0,15,133,5,0,0,0,233,95,7,0,0,72,139,132,36,32,1,0,0,72,139,64,32,72,137,132,36,24,1,0,0,72,139,132,36,24,1,0,0,72,137,132,36,16,1,0,0,72,139,132,36,16,1,0,0,72,139,140,36,16,1,0,0,72,99,73,60,72,1,200,72,137,132,36,8,1,0,0,72,139,132,36,8,1,0,0,72,131,192,24,72,139,140,36,8,1,0,0,15,183,73,20,72,1,200,72,137,132,36,0,1,0,0,72,139,132,36,24,1,0,0,72,139,140,36,8,1,0,0,72,43,65,48,72,137,132,36,248,0,0,0,72,131,188,36,248,0,0,0,0,15,132,215,2,0,0,72,139,140,36,24,1,0,0,72,139,132,36,8,1,0,0,72,137,72,48,72,139,132,36,8,1,0,0,72,139,128,176,0,0,0,72,137,132,36,240,0,0,0,72,139,132,36,8,1,0,0,72,139,128,144,0,0,0,72,137,132,36,232,0,0,0,72,199,132,36,224,0,0,0,0,0,0,0,72,199,132,36,216,0,0,0,0,0,0,0,199,132,36,212,0,0,0,0,0,0,0,139,132,36,212,0,0,0,72,139,140,36,8,1,0,0,15,183,73,6,57,200,15,141,212,0,0,0,72,139,132,36,0,1,0,0,72,99,140,36,212,0,0,0,72,107,201,40,72,1,200,72,137,132,36,200,0,0,0,139,132,36,240,0,0,0,72,139,140,36,200,0,0,0,59,65,12,15,130,53,0,0,0,139,132,36,240,0,0,0,72,139,140,36,200,0,0,0,139,73,12,72,139,148,36,200,0,0,0,3,74,8,57,200,15,131,16,0,0,0,72,139,132,36,200,0,0,0,72,137,132,36,224,0,0,0,139,132,36,232,0,0,0,72,139,140,36,200,0,0,0,59,65,12,15,130,53,0,0,0,139,132,36,232,0,0,0,72,139,140,36,200,0,0,0,139,73,12,72,139,148,36,200,0,0,0,3,74,8,57,200,15,131,16,0,0,0,72,139,132,36,200,0,0,0,72,137,132,36,216,0,0,0,233,0,0,0,0,139,132,36,212,0,0,0,131,192,1,137,132,36,212,0,0,0,233,17,255,255,255,72,131,188,36,224,0,0,0,0,15,132,15,0,0,0,72,131,188,36,216,0,0,0,0,15,133,5,0,0,0,233,87,5,0,0,72,199,132,36,192,0,0,0,0,0,0,0,72,139,132,36,192,0,0,0,139,140,36,244,0,0,0,72,57,200,15,131,55,1,0,0,72,139,132,36,24,1,0,0,72,139,140,36,224,0,0,0,139,73,12,72,1,200,72,3,132,36,192,0,0,0,72,137,132,36,184,0,0,0,72,139,132,36,192,0,0,0,72,131,192,8,72,137,132,36,192,0,0,0,72,139,132,36,184,0,0,0,139,64,4,72,131,232,8,72,209,232,72,137,132,36,176,0,0,0,72,199,132,36,168,0,0,0,0,0,0,0,72,139,132,36,168,0,0,0,72,59,132,36,176,0,0,0,15,131,188,0,0,0,72,139,132,36,24,1,0,0,72,139,140,36,224,0,0,0,139,73,12,72,1,200,72,3,132,36,192,0,0,0,72,137,132,36,160,0,0,0,72,139,132,36,192,0,0,0,72,131,192,2,72,137,132,36,192,0,0,0,72,139,132,36,160,0,0,0,102,139,0,102,193,232,12,15,183,192,131,248,0,15,133,5,0,0,0,233,73,0,0,0,72,139,132,36,24,1,0,0,72,139,140,36,184,0,0,0,139,9,72,1,200,72,139,140,36,160,0,0,0,102,139,9,102,129,225,255,15,15,183,201,72,1,200,72,137,132,36,152,0,0,0,72,139,140,36,248,0,0,0,72,139,132,36,152,0,0,0,72,3,8,72,137,8,72,139,132,36,168,0,0,0,72,131,192,1,72,137,132,36,168,0,0,0,233,46,255,255,255,233,177,254,255,255,233,0,0,0,0,72,139,132,36,32,1,0,0,131,120,56,1,15,133,235,1,0,0,72,139,132,36,24,1,0,0,72,139,140,36,8,1,0,0,139,137,144,0,0,0,72,1,200,72,137,132,36,144,0,0,0,72,131,188,36,144,0,0,0,0,15,133,22,0,0,0,72,139,132,36,32,1,0,0,185,64,64,64,0,72,137,72,40,233,159,3,0,0,233,0,0,0,0,72,139,132,36,144,0,0,0,131,120,12,0,15,132,137,1,0,0,72,139,132,36,24,1,0,0,72,139,140,36,144,0,0,0,139,73,12,72,1,200,72,137,132,36,136,0,0,0,72,139,132,36,32,1,0,0,72,139,0,72,139,140,36,136,0,0,0,255,208,72,137,132,36,128,0,0,0,72,131,188,36,128,0,0,0,0,15,133,30,0,0,0,72,139,132,36,32,1,0,0,72,139,64,8,72,139,140,36,136,0,0,0,255,208,72,137,132,36,128,0,0,0,72,131,188,36,128,0,0,0,0,15,133,25,0,0,0,72,139,132,36,144,0,0,0,72,131,192,20,72,137,132,36,144,0,0,0,233,94,255,255,255,72,139,132,36,24,1,0,0,72,139,140,36,144,0,0,0,139,73,16,72,1,200,72,137,68,36,120,72,139,68,36,120,72,131,56,0,15,132,182,0,0,0,72,139,76,36,120,72,184,0,0,0,0,0,0,0,128,72,35,1,72,131,248,0,15,132,65,0,0,0,72,139,68,36,120,72,139,0,72,37,255,255,0,0,137,68,36,116,72,139,132,36,32,1,0,0,72,139,64,16,139,76,36,116,15,183,201,137,202,72,139,140,36,128,0,0,0,255,208,72,137,193,72,139,68,36,120,72,137,8,233,70,0,0,0,72,139,132,36,24,1,0,0,72,139,76,36,120,72,3,1,72,137,68,36,104,72,139,132,36,32,1,0,0,72,139,64,16,72,139,84,36,104,72,131,194,2,72,139,140,36,128,0,0,0,255,208,72,137,68,36,96,72,139,76,36,96,72,139,68,36,120,72,137,8,72,139,68,36,120,72,131,192,8,72,137,68,36,120,233,59,255,255,255,72,139,132,36,144,0,0,0,72,131,192,20,72,137,132,36,144,0,0,0,233,101,254,255,255,233,0,0,0,0,72,139,132,36,32,1,0,0,131,120,56,1,15,133,145,0,0,0,72,139,132,36,8,1,0,0,72,139,128,208,0,0,0,72,137,68,36,88,131,124,36,92,0,15,132,109,0,0,0,72,139,132,36,24,1,0,0,139,76,36,88,72,1,200,72,137,68,36,80,72,139,68,36,80,72,139,64,24,72,137,68,36,72,72,139,68,36,72,72,131,56,0,15,132,55,0,0,0,72,139,68,36,72,72,139,0,72,137,68,36,64,72,139,68,36,64,72,139,140,36,24,1,0,0,186,1,0,0,0,69,49,192,255,208,72,139,68,36,72,72,131,192,8,72,137,68,36,72,233,186,255,255,255,233,0,0,0,0,233,0,0,0,0,72,139,132,36,32,1,0,0,131,120,56,1,15,133,139,0,0,0,72,139,132,36,8,1,0,0,72,139,128,160,0,0,0,72,137,68,36,56,131,124,36,60,0,15,132,103,0,0,0,72,139,132,36,32,1,0,0,72,139,64,24,72,137,68,36,32,76,139,132,36,24,1,0,0,139,68,36,60,185,12,0,0,0,49,210,72,247,241,72,137,193,72,139,68,36,32,137,202,72,139,140,36,24,1,0,0,68,139,76,36,56,76,1,201,255,208,131,248,0,15,133,22,0,0,0,72,139,132,36,32,1,0,0,185,64,64,64,0,72,137,72,40,233,196,0,0,0,233,0,0,0,0,233,0,0,0,0,72,139,132,36,32,1,0,0,72,131,120,64,0,15,132,22,0,0,0,72,139,132,36,32,1,0,0,72,139,64,64,72,139,140,36,24,1,0,0,255,208,72,139,140,36,24,1,0,0,72,139,132,36,32,1,0,0,72,137,72,40,72,139,132,36,32,1,0,0,131,120,56,1,15,133,107,0,0,0,72,139,132,36,32,1,0,0,185,64,64,64,0,72,137,72,40,72,139,132,36,24,1,0,0,72,139,140,36,8,1,0,0,139,73,40,72,1,200,72,137,68,36,48,72,139,68,36,48,72,139,140,36,24,1,0,0,186,1,0,0,0,69,49,192,255,208,137,68,36,44,131,124,36,44,0,15,132,20,0,0,0,72,139,140,36,24,1,0,0,72,139,132,36,32,1,0,0,72,137,72,40,233,0,0,0,0,72,129,196,40,1,0,0,195 };
}
bytes DllRemapper::DecryptCoreDllData() {
	bytes Data(coreDllMemorySize);
	DWORD64* pCoreData = (DWORD64*)Data.data();
	memcpy(pCoreData, pEncryptedCoreDllMemory, coreDllMemorySize);
	DWORD dwProcessId = GetCurrentProcessId();
	for (size_t i = 0; i < coreDllMemorySize / sizeof(DWORD64); i++)
	{
		pCoreData[i] ^= ((i % 65535 + coreDllMemorySize + dwProcessId) * 3) + DWORD64(pEncryptedCoreDllMemory);
	}
	return Data;
}
void DllRemapper::ReloadNetbiosAndInjectCurrentDll(HMODULE hModule, const bytes& dllData, LPVOID lpReserved) {

	void* aFnNetbios = (void*)GetModuleProcAddress(obf("NETAPI32"), obf("Netbios"));
	DWORD_PTR hAceBaseModule = (DWORD_PTR)GetModuleHandleA(obf("ace-base64.dll"));
	size_t nAceBaseModuleSize = GetModuleSize((HMODULE)hAceBaseModule);
	void* fnNetbiosRetAddress = nullptr;
	void** ppReLoadNetBios = nullptr;
	/*获取挂接地址*/
	for (size_t i = 0; i < 0x1000; i++)
	{
		ppReLoadNetBios = (void**)(((DWORD_PTR)lpReserved) + i * 8);
		DWORD_PTR RetAddress = *(DWORD_PTR*)(ppReLoadNetBios);
		if (RetAddress > hAceBaseModule && RetAddress < hAceBaseModule + nAceBaseModuleSize) {
			if (*(unsigned char*)(RetAddress - 5) == 0xE8 && *(DWORD*)RetAddress == 0x850FC084 && *(DWORD*)(RetAddress - 0x14) == 0x244C8D48) {
				DWORD_PTR fnAceNetbios = *(int*)(RetAddress - 4) + RetAddress;
				if (fnAceNetbios > hAceBaseModule && fnAceNetbios < hAceBaseModule + nAceBaseModuleSize) {
					if (*(unsigned short*)fnAceNetbios == 0x25ff) {
						DWORD_PTR iatAceNetbios = (fnAceNetbios + 6) + *(int*)(fnAceNetbios + 2);
						if (iatAceNetbios > hAceBaseModule && iatAceNetbios < hAceBaseModule + nAceBaseModuleSize) {
							if (aFnNetbios == *(void**)iatAceNetbios) {
								fnNetbiosRetAddress = (void*)(RetAddress - 5);
								break;
							}
						}
					}
				}
			}
		}
	}
	/*开始加载模块*/
	if (ppReLoadNetBios && fnNetbiosRetAddress) {
		unsigned char stageInjectionShellCode[] = { 84,80,83,81,82,85,86,87,65,80,65,81,65,82,65,83,65,84,65,85,65,86,65,87,156,72,131,236,64,15,17,68,36,48,15,17,76,36,32,15,17,84,36,16,15,17,28,36,72,141,140,36,200,0,0,0,72,247,196,8,0,0,0,15,132,11,0,0,0,72,187,16,50,84,118,152,186,220,254,83,72,131,236,32,232,84,0,0,0,72,131,196,32,72,187,16,50,84,118,152,186,220,254,72,57,28,36,15,133,1,0,0,0,91,15,16,28,36,15,16,84,36,16,15,16,76,36,32,15,16,68,36,48,72,131,196,64,157,65,95,65,94,65,93,65,92,65,91,65,90,65,89,65,88,95,94,93,90,89,91,88,92,72,141,76,36,80,255,37,91,0,0,0,255,37,77,0,0,0 };
		stageInjectionShellCode[sizeof(stageInjectionShellCode) - 12 - 1] = *((unsigned char*)fnNetbiosRetAddress - 11);//修正堆栈偏移
		void* allocatedBaseAddress = VirtualAlloc(nullptr, 0x1000, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
		if (allocatedBaseAddress == nullptr) {
			MessageBoxA(nullptr, obf("aollc memory failed 0x01"), 0, 0);
			TerminateProcess(GetCurrentProcess(), 0);
			return;
		}
		memcpy(allocatedBaseAddress, stageInjectionShellCode, sizeof(stageInjectionShellCode));
		*(void**)(DWORD_PTR(allocatedBaseAddress) + 0x100) = (void*)(DWORD_PTR(allocatedBaseAddress) + 0x400);
		*(void**)(DWORD_PTR(allocatedBaseAddress) + 0x108) = fnNetbiosRetAddress;
		sprintf_s((char*)allocatedBaseAddress + 0x200, 0x100, GetModuleFileNameA(hModule).c_str());
		*(void**)(DWORD_PTR(allocatedBaseAddress) + 0x110) = (void*)(DWORD_PTR(allocatedBaseAddress) + 0x200);
		*(void**)(DWORD_PTR(allocatedBaseAddress) + 0x118) = (void*)GetModuleProcAddress(obf("kernel32"), obf("DeleteFileA"));
		bytes manualMappingData = PrepareDllForManualMapping((LPVOID)dllData.data());
		memcpy((void*)(DWORD_PTR(allocatedBaseAddress) + 0x150), manualMappingData.data(), manualMappingData.size());
		*(void**)(DWORD_PTR(allocatedBaseAddress) + 0x120) = (void*)(DWORD_PTR(allocatedBaseAddress) + 0x150);//写入参数
		bytes manualDllLoaderFunction = GenerateManualDllLoaderFunction();
		memcpy((void*)(DWORD_PTR(allocatedBaseAddress) + 0x500), manualDllLoaderFunction.data(), manualDllLoaderFunction.size());
		*(void**)(DWORD_PTR(allocatedBaseAddress) + 0x128) = (void*)(DWORD_PTR(allocatedBaseAddress) + 0x500);//写入加载函数
		sprintf_s((char*)(DWORD_PTR(allocatedBaseAddress) + 0x300), 0x100, obf("LOADER ERROR 0x01"));
		*(void**)(DWORD_PTR(allocatedBaseAddress) + 0x130) = (void*)(DWORD_PTR(allocatedBaseAddress) + 0x300);
		*(void**)(DWORD_PTR(allocatedBaseAddress) + 0x138) = (void*)GetModuleProcAddress(obf("user32.dll"), obf("MessageBoxTimeoutA"));
		*(void**)(DWORD_PTR(allocatedBaseAddress) + 0x140) = (void*)ExitProcess;
		unsigned char secondStageInjectionShellCode[] = { 72,131,236,40,72,141,61,245,251,255,255,72,139,143,16,1,0,0,255,151,24,1,0,0,72,139,143,32,1,0,0,255,151,40,1,0,0,72,139,159,32,1,0,0,72,139,67,32,72,59,67,40,117,5,72,131,196,40,195,72,49,201,72,139,151,48,1,0,0,77,49,192,65,185,16,16,0,0,199,68,36,8,0,0,0,0,199,4,36,136,19,0,0,72,131,236,32,255,151,56,1,0,0,72,131,196,32,72,49,201,255,151,64,1,0,0,72,131,196,40,195,144 };
		memcpy((unsigned char*)allocatedBaseAddress + 0x400, secondStageInjectionShellCode, sizeof(secondStageInjectionShellCode));
		HMODULE hCodeModule = *(HMODULE*)(DWORD_PTR(allocatedBaseAddress) + 0x150 + 0x20);
		pInjectionShellCodeMemory = (unsigned char*)allocatedBaseAddress;
		pEncryptedCoreDllMemory = (unsigned char*)VirtualAlloc(nullptr, dllData.size(), MEM_COMMIT, PAGE_READWRITE);
		if (pEncryptedCoreDllMemory == nullptr) {
			MessageBoxA(nullptr, obf("aollc memory failed 0x02"), 0, 0);
			TerminateProcess(GetCurrentProcess(), 0);
			return;
		}
		coreDllMemorySize = dllData.size();
		memcpy((void*)pEncryptedCoreDllMemory, dllData.data(), dllData.size());
		DWORD dwProcessId = GetCurrentProcessId();
		for (size_t i = 0; i < coreDllMemorySize / sizeof(DWORD64); i++)
		{
			((DWORD64*)pEncryptedCoreDllMemory)[i] ^= ((i % 65535 + coreDllMemorySize + dwProcessId) * 3) + DWORD64(pEncryptedCoreDllMemory);
		}
		*(unsigned char**)(DWORD_PTR(hCodeModule) + (DWORD_PTR(&pInjectionShellCodeMemory) - DWORD_PTR(hModule))) = pInjectionShellCodeMemory;
		*(unsigned char**)(DWORD_PTR(hCodeModule) + (DWORD_PTR(&pEncryptedCoreDllMemory) - DWORD_PTR(hModule))) = pEncryptedCoreDllMemory;
		*(size_t*)(DWORD_PTR(hCodeModule) + (DWORD_PTR(&coreDllMemorySize) - DWORD_PTR(hModule))) = coreDllMemorySize;
		//挂载数据 
		*ppReLoadNetBios = allocatedBaseAddress;
	}
	else
	{
		MessageBoxA(nullptr, obf("LOADER ERROR 0x02"), 0, 0);
		TerminateProcess(GetCurrentProcess(), 0);
	}


}

