#pragma once
#include <windows.h>
#include <psapi.h>
#include <intrin.h>
#include <vector>
#include <algorithm>
#include <string>
#include <fstream>
#include <locale>
#include <codecvt>
#include <chrono>
#include <io.h>
#include <direct.h>
#include <unordered_map>
#include <IPHLPAPI.h>
#include <cstdio>
#include <Windows.h>
#include <string>
#include <windows.h>
#include <algorithm>
#include <shlobj_core.h>
#include<tlhelp32.h>
#include <intrin.h>
#include <comdef.h>
#include <Wbemidl.h>
#include <psapi.h>
#include <string>
#include <vector>
#include <map>
#include <mutex>
#include <cctype>
#include <locale>
#include <codecvt>
#include <iostream>
#include <algorithm>
#include <fstream>
#pragma comment(lib,"ntdll.lib")
#pragma comment(lib,"Iphlpapi.lib")
#pragma comment(lib, "ntdll.lib")
#pragma comment(lib,"version.lib")

#include "obf.hpp"
#include "detours/detours.h"

typedef std::vector<unsigned char> bytes;
/*到字节集*/
namespace std {
	inline bytes to_bytes(int _val) {
		bytes bytes(sizeof(_val));
		memcpy(bytes.data(), &_val, sizeof(_val));
		return bytes;
	}
	inline bytes to_bytes(DWORD _val) {
		bytes bytes(sizeof(_val));
		memcpy(bytes.data(), &_val, sizeof(_val));
		return bytes;
	}
	inline bytes to_bytes(uint64_t _val) {
		bytes bytes(sizeof(_val));
		memcpy(bytes.data(), &_val, sizeof(_val));
		return bytes;
	}
	inline bytes to_bytes(const char* _val) {
		bytes bytes(strlen(_val));
		memcpy(bytes.data(), _val, bytes.size());
		return bytes;
	}
	inline bytes to_bytes(void* _val, size_t nSize) {
		bytes bytes(nSize);
		memcpy(bytes.data(), _val, nSize);
		return bytes;
	}
	inline bytes to_bytes(string _val) {
		bytes bytes;
		bytes.insert(bytes.end(), _val.begin(), _val.end());
		return bytes;
	}
	inline string to_string(bytes bytes) {
		string str;
		str.insert(str.begin(), bytes.begin(), bytes.end());
		return str;
	}
	inline string to_string(void* pPonint, size_t nSize) {
		string str;
		str.resize(nSize);
		memcpy((void*)str.data(), pPonint, nSize);
		return str;
	}
	inline int to_interger(bytes bytes) {
		return *(int*)bytes.data();
	}
}
/*编码转换*/
extern std::string gbk_to_utf8(const std::string& gbk);
extern std::string utf8_to_gbk(const std::string& utf8);
extern std::string us2_to_gbk(const std::wstring& wstr);
extern std::string us2_to_utf8(const std::wstring& wstr);
extern std::wstring gbk_to_us2(const std::string& gbk);
extern std::wstring utf8_to_us2(const std::string& utf8);
/*大小写转换*/
extern std::string toLower(std::string str);
extern std::string toUpper(std::string str);
extern std::wstring toLower(std::wstring str);
extern std::wstring toUpper(std::wstring str);
/*删除空白符*/
extern std::string& ltrim(std::string& str, const std::string& whitespace = " \t\n\r\f\v");
extern std::string& rtrim(std::string& str, const std::string& whitespace = " \t\n\r\f\v");
extern std::string& trim(std::string& str, const std::string& whitespace = " \t\n\r\f\v");
extern std::string trim_copy(std::string str, const std::string& whitespace = " \t\n\r\f\v");
/*读写文件*/
extern bytes ReadFile(const char* lpFilename, int max = 0);
extern bool WriteFile(const char* lpFilename, bytes bs);
extern std::string ReadFileLine(const std::string& lpFilename);
/*取模块名称*/
extern HMODULE GetModuleHandleExA(LPVOID lpBaseAddress);
extern std::string K32GetModuleBaseNameA(HMODULE hModule);
extern std::wstring K32GetModuleBaseNameW(HMODULE hModule);
extern std::string GetModuleFileNameA(HMODULE hModule);
extern std::wstring GetModuleFileNameW(HMODULE hModule);
extern std::string GetFullPathNameA(const std::string& szFileName);
extern std::wstring GetFullPathNameW(const std::wstring& szFileName);
/*获取指定目录*/
extern std::string SHGetKnownFolderPathA(REFKNOWNFOLDERID rfid);
extern std::wstring SHGetKnownFolderPathW(REFKNOWNFOLDERID rfid);
/*读配置文件*/
extern std::string GetPrivateProfileStringA(const std::string& lpFileName, const std::string& lpAppName, const std::string& lpKeyName, const std::string& lpDefault = "");
extern std::wstring GetPrivateProfileStringW(const std::wstring& lpFileName, const std::wstring& lpAppName, const std::wstring& lpKeyName, const std::wstring& lpDefault = L"");
extern uint32_t GetPrivateProfileIntA(const std::string& lpFileName, const std::string& lpAppName, const std::string& lpKeyName, UINT lpDefault = 0);
extern uint32_t GetPrivateProfileIntW(const std::wstring& lpFileName, const std::wstring& lpAppName, const std::wstring& lpKeyName, UINT lpDefault = 0);
/*判断文件是否存在*/
extern bool FileExistsA(const std::string& filename);
extern bool FileExistsW(const std::wstring& filename);
/*判断目录是否存在*/
extern bool FolderExistsA(const std::string& filename);
extern bool FolderExistsW(const std::wstring& filename);
/*取模块大小*/
extern DWORD GetModuleSize(HMODULE hModule);
extern FARPROC GetModuleProcAddress(LPCSTR lpModuleName, LPCSTR lpProcName);

bytes rc4(bytes m, bytes k);
void* rc4(cs_byte* m, size_t mLen, cs_byte* k, size_t kLen, cs_byte* out);

namespace Kernel
{
	extern HANDLE m_hTimerQueue;
	extern HANDLE m_hTimerQueueTimer;
	extern void _SafeCreateThread(WAITORTIMERCALLBACK CallBack, PVOID lpParam);
}

class Pattern
{
public:
	Pattern() = default;
	Pattern(uint64_t Address) : Address(Address) {}
	static Pattern FindPattern(const char* pattern);
	Pattern add(uint32_t val);
	Pattern sub(uint32_t val);
	Pattern Absolute();
	Pattern Deref();
	uint64_t get() const;
private:
	uint64_t Address = 0;
};

/*信息框*/
inline int MessageBoxTimeoutA(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption = nullptr, UINT uType = MB_SYSTEMMODAL | MB_ICONERROR, WORD wLanguageId = 0, DWORD dwMilliseconds = 5000) {
	static void* fnMessageBoxTimeOutA = nullptr;
	if (fnMessageBoxTimeOutA == nullptr) {
		fnMessageBoxTimeOutA = (void*)GetModuleProcAddress("user32.dll", "MessageBoxTimeoutA");
	}
	if (fnMessageBoxTimeOutA) {
		return reinterpret_cast<int(__stdcall*)(HWND, LPCSTR, LPCSTR, UINT, WORD, DWORD)>(fnMessageBoxTimeOutA)(hWnd, lpText, lpCaption, uType, wLanguageId, dwMilliseconds);
	}
	return MessageBoxA(hWnd, lpText, lpCaption, uType);
}
