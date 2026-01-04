#include "utils.h"


/*编码转换*/
std::string gbk_to_utf8(const std::string& gbk)
{
	// 1. GBK -> UTF‑16
	int wlen = MultiByteToWideChar(936, 0, gbk.c_str(), -1, nullptr, 0);
	if (wlen == 0) return "";
	std::wstring wstr(wlen, L'\0');
	MultiByteToWideChar(936, 0, gbk.c_str(), -1, &wstr[0], wlen);

	// 2. UTF‑16 -> UTF‑8
	int u8len = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
	if (u8len == 0) return "";
	std::string utf8(u8len, '\0');
	WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &utf8[0], u8len, nullptr, nullptr);
	return utf8.c_str();
}
std::string utf8_to_gbk(const std::string& utf8)
{
	// 1. UTF‑8 -> UTF‑16
	int wlen = MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), -1, nullptr, 0);
	if (wlen == 0) return "";
	std::wstring wstr(wlen, L'\0');
	MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), -1, &wstr[0], wlen);

	// 2. UTF‑16 -> GBK
	int glen = WideCharToMultiByte(936, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
	if (glen == 0) return "";
	std::string gbkStr(glen, '\0');
	WideCharToMultiByte(936, 0, wstr.c_str(), -1, &gbkStr[0], glen, nullptr, nullptr);
	return gbkStr.c_str();
}
std::string us2_to_gbk(const std::wstring& wstr)
{
	// UTF‑16 -> GBK
	int glen = WideCharToMultiByte(936, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
	if (glen == 0) return "";
	std::string gbkStr(glen, '\0');
	WideCharToMultiByte(936, 0, wstr.c_str(), -1, &gbkStr[0], glen, nullptr, nullptr);
	return gbkStr.c_str();
}
std::string us2_to_utf8(const std::wstring& wstr)
{
	// UTF‑16 -> UTF‑8
	int u8len = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
	if (u8len == 0) return "";
	std::string utf8(u8len, '\0');
	WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &utf8[0], u8len, nullptr, nullptr);
	return utf8.c_str();
}
std::wstring gbk_to_us2(const std::string& gbk)
{
	// GBK -> UTF‑16
	int wlen = MultiByteToWideChar(936, 0, gbk.c_str(), -1, nullptr, 0);
	if (wlen == 0) return L"";
	std::wstring wstr(wlen, L'\0');
	MultiByteToWideChar(936, 0, gbk.c_str(), -1, &wstr[0], wlen);
	return wstr.c_str();
}
std::wstring utf8_to_us2(const std::string& utf8)
{
	// GBK -> UTF‑16
	int wlen = MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), -1, nullptr, 0);
	if (wlen == 0) return L"";
	std::wstring wstr(wlen, L'\0');
	MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), -1, &wstr[0], wlen);
	return wstr.c_str();
}

/*删除空白符*/
std::string& ltrim(std::string& str, const std::string& whitespace) {
	str.erase(0, str.find_first_not_of(whitespace));
	return str;
}
std::string& rtrim(std::string& str, const std::string& whitespace) {
	str.erase(str.find_last_not_of(whitespace) + 1);
	return str;
}
std::string& trim(std::string& str, const std::string& whitespace) {
	return ltrim(rtrim(str, whitespace), whitespace);
}
std::string trim_copy(std::string str, const std::string& whitespace) {
	return trim(str, whitespace);
}
/*读写文件*/
bytes ReadFile(const char* lpFilename, int max) {
	bytes bs;
	FILE* fp;
	if (!fopen_s(&fp, lpFilename, "rb")) {
		fseek(fp, 0, SEEK_END);
		int length = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		if (max > 0 && length > max)length = max;
		bs.resize(length);
		fread_s(bs.data(), bs.size(), sizeof(unsigned char), bs.size(), fp);
		fclose(fp);
	}
	return bs;
}
bool WriteFile(const char* lpFilename, bytes bs) {
	FILE* fp;
	if (!fopen_s(&fp, lpFilename, "wb")) {
		fwrite(bs.data(), bs.size(), sizeof(unsigned char), fp);
		fclose(fp);
		return true;
	}

	return false;
}
std::string ReadFileLine(const std::string& lpFilename) {
	std::ifstream input_file(lpFilename);
	if (!input_file.is_open()) {
		return {};
	}
	std::string line;
	while (std::getline(input_file, line)) {
		line = trim(line);
		if (line.size() > 0)break;
	}
	return line;
}
/*取模块名称*/
HMODULE GetModuleHandleExA(LPVOID lpBaseAddress) {
	HMODULE hModule = nullptr;
	GetModuleHandleExA(6, LPCSTR(lpBaseAddress), &hModule);
	return hModule;
}
std::string K32GetModuleBaseNameA(HMODULE hModule) {
	char szCurName[MAX_PATH] = { 0 };
	GetModuleBaseNameA(GetCurrentProcess(), hModule, szCurName, MAX_PATH);
	return szCurName;
}
std::wstring K32GetModuleBaseNameW(HMODULE hModule) {
	wchar_t szCurName[MAX_PATH] = { 0 };
	GetModuleBaseNameW(GetCurrentProcess(), hModule, szCurName, MAX_PATH);
	return szCurName;
}
std::string GetModuleFileNameA(HMODULE hModule) {
	char szCurName[MAX_PATH] = { 0 };
	GetModuleFileNameA(hModule, szCurName, MAX_PATH);
	return szCurName;
}
std::wstring GetModuleFileNameW(HMODULE hModule) {
	wchar_t szCurName[MAX_PATH] = { 0 };
	GetModuleFileNameW(hModule, szCurName, MAX_PATH);
	return szCurName;
}
std::string GetFullPathNameA(const std::string& szFileName) {
	char szFullPathName[MAX_PATH] = { 0 };
	GetFullPathNameA(szFileName.c_str(), MAX_PATH, szFullPathName, nullptr);
	return szFullPathName;
}
std::wstring GetFullPathNameW(const std::wstring& szFileName) {
	wchar_t szFullPathName[MAX_PATH] = { 0 };
	GetFullPathNameW(szFileName.c_str(), MAX_PATH, szFullPathName, nullptr);
	return szFullPathName;
}
/*获取指定目录*/
std::string SHGetKnownFolderPathA(REFKNOWNFOLDERID rfid) {
	return us2_to_gbk(SHGetKnownFolderPathW(rfid));
}
std::wstring SHGetKnownFolderPathW(REFKNOWNFOLDERID rfid) {
	PWSTR pszDesktopPath = NULL;  // 宽字符指针，存储桌面路径
	HRESULT hr = SHGetKnownFolderPath(rfid, 0, NULL, &pszDesktopPath);
	std::wstring szPath;
	if (SUCCEEDED(hr))
	{
		szPath = pszDesktopPath;
		CoTaskMemFree(pszDesktopPath);  // 必须释放 API 分配的内存，否则会内存泄漏
	}
	return szPath;
}
/*读配置文件*/
std::string GetPrivateProfileStringA(const std::string& lpFileName, const std::string& lpAppName, const std::string& lpKeyName, const std::string& lpDefault) {
	char ReturnedString[1024] = { 0 };
	::GetPrivateProfileStringA(lpAppName.c_str(), lpKeyName.c_str(), lpDefault.c_str(), ReturnedString, sizeof(ReturnedString) - sizeof(char), lpFileName.c_str());
	return ReturnedString;
}
std::wstring GetPrivateProfileStringW(const std::wstring& lpFileName, const std::wstring& lpAppName, const std::wstring& lpKeyName, const std::wstring& lpDefault) {
	wchar_t ReturnedString[1024] = { 0 };
	::GetPrivateProfileStringW(lpAppName.c_str(), lpKeyName.c_str(), lpDefault.c_str(), ReturnedString, sizeof(ReturnedString) - sizeof(char), lpFileName.c_str());
	return ReturnedString;
}
uint32_t GetPrivateProfileIntA(const std::string& lpFileName, const std::string& lpAppName, const std::string& lpKeyName, UINT lpDefault) {
	return ::GetPrivateProfileIntA(lpAppName.c_str(), lpKeyName.c_str(), lpDefault, lpFileName.c_str());
}
uint32_t GetPrivateProfileIntW(const std::wstring& lpFileName, const std::wstring& lpAppName, const std::wstring& lpKeyName, UINT lpDefault) {
	return ::GetPrivateProfileIntW(lpAppName.c_str(), lpKeyName.c_str(), lpDefault, lpFileName.c_str());
}
/*判断文件是否存在*/
bool FileExistsA(const std::string& filename) {
	std::ifstream file(filename);
	return file.good();
}
bool FileExistsW(const std::wstring& filename) {
	std::ifstream file(filename);
	return file.good();
}
/*判断目录是否存在*/
bool FolderExistsA(const std::string& filename) {
	return _access(filename.c_str(), 0) != -1;
}
bool FolderExistsW(const std::wstring& filename) {
	return _waccess(filename.c_str(), 0) != -1;
}
/*取模块大小*/
DWORD GetModuleSize(HMODULE hModule) {
	if (hModule == nullptr) return 0;
	PBYTE pSrcData = (PBYTE)hModule;
	PIMAGE_DOS_HEADER pImageDosHeader;
	PIMAGE_NT_HEADERS pImageNtHeader;
	pImageDosHeader = (PIMAGE_DOS_HEADER)pSrcData;
	if (pImageDosHeader->e_magic != IMAGE_DOS_SIGNATURE)
	{
		return 0;
	}
	pImageNtHeader = (PIMAGE_NT_HEADERS)&pSrcData[pImageDosHeader->e_lfanew];
	if (pImageNtHeader->Signature != IMAGE_NT_SIGNATURE)
	{
		return 0;
	}
	return pImageNtHeader->OptionalHeader.SizeOfImage;
}
FARPROC GetModuleProcAddress(LPCSTR lpModuleName, LPCSTR lpProcName) {
	HMODULE hMod = GetModuleHandleA(lpModuleName);
	if (hMod == nullptr) {
		hMod = LoadLibraryA(lpModuleName);
		if (hMod == nullptr) {
			return nullptr;
		}
	}
	return GetProcAddress(hMod, lpProcName);
}

bytes rc4(bytes m, bytes k) {
	
	bytes res(m.size());
	rc4(m.data(), m.size(), k.data(), k.size(), res.data());
	
	return res;
}
void* rc4(cs_byte* m, size_t mLen, cs_byte* k, size_t kLen, cs_byte* out)
{
	int S[256];
	int T[256];

	int  count = (int)kLen;
	int i = 0, j = 0;

	for (int i = 0; i < 256; i++)
	{
		S[i] = i;
		int tmp = i % count;
		T[i] = k[tmp];
	}

	i = j = 0;

	for (int i = 0; i < 256; i++)
	{
		j = (j + S[i] + T[i]) % 256;
		int tmp;
		tmp = S[j];
		S[j] = S[i];
		S[i] = tmp;
	}

	int length = (int)mLen;
	i = j = 0;

	for (int p = 0; p < length; p++)
	{

		i = (i + 1) % 256;
		j = (j + S[i]) % 256;
		int tmp;
		tmp = S[j];
		S[j] = S[i];
		S[i] = tmp;

		int k = S[(S[i] + S[j]) % 256];
		out[p] = m[p] ^ k;
	}
	return out;

}

HANDLE Kernel::m_hTimerQueue = nullptr;
HANDLE Kernel::m_hTimerQueueTimer = nullptr;
void Kernel::_SafeCreateThread(WAITORTIMERCALLBACK CallBack, PVOID lpParam) {
	if (!CreateTimerQueueTimer(&m_hTimerQueueTimer, NULL, CallBack, lpParam, 0, 0, WT_EXECUTEDEFAULT))
	{
		DeleteTimerQueueTimer(NULL, m_hTimerQueueTimer, NULL);
		m_hTimerQueueTimer = NULL;
		return;
	}
}

Pattern Pattern::FindPattern(const char* pattern)
{
	static uintptr_t moduleAdressmm = 0;
	if (!moduleAdressmm)
		moduleAdressmm = (uintptr_t)GetModuleHandle(NULL);

	if (!moduleAdressmm)
		return 0;

	static auto patternToByteZmm = [](const char* pattern)
		{
			auto       bytesmm = std::vector<int>{};
			const auto startmm = const_cast<char*>(pattern);
			const auto endmm = const_cast<char*>(pattern) + strlen(pattern);

			for (auto currentmm = startmm; currentmm < endmm; ++currentmm)
			{
				if (*currentmm == '?')
				{
					++currentmm;
					if (*currentmm == '?')
						++currentmm;
					bytesmm.push_back(-1);
				}
				else { bytesmm.push_back(strtoul(currentmm, &currentmm, 16)); }
			}
			return bytesmm;
		};

	const auto dosHeadermm = (PIMAGE_DOS_HEADER)moduleAdressmm;
	const auto ntHeadersmm = (PIMAGE_NT_HEADERS)((std::uint8_t*)moduleAdressmm + dosHeadermm->e_lfanew);

	const auto sizeOfImage = ntHeadersmm->OptionalHeader.SizeOfImage;
	auto       patternBytesmm = patternToByteZmm(pattern);
	const auto scanBytesmm = reinterpret_cast<std::uint8_t*>(moduleAdressmm);

	const auto smm = patternBytesmm.size();
	const auto dmm = patternBytesmm.data();

	for (auto imm = 0ul; imm < sizeOfImage - smm; ++imm)
	{
		bool foundmm = true;
		for (auto jmm = 0ul; jmm < smm; ++jmm)
		{
			if (scanBytesmm[imm + jmm] != dmm[jmm] && dmm[jmm] != -1)
			{
				foundmm = false;
				break;
			}
		}
		if (foundmm) { return reinterpret_cast<uintptr_t>(&scanBytesmm[imm]); }
	}
	return NULL;
}
Pattern Pattern::add(uint32_t val)
{
	if (Address == NULL) {
		return *this;
	}
	Address += val;
	return *this;
}
Pattern Pattern::sub(uint32_t val)
{
	if (Address == NULL) {
		return *this;
	}
	Address -= val;
	return *this;
}
Pattern Pattern::Absolute()
{
	if (Address == NULL) {
		return *this;
	}
	long dwOffset = *(long*)Address;
	Address = Address + dwOffset + 4;
	return *this;
}
Pattern Pattern::Deref()
{
	if (Address == NULL) {
		return *this;
	}
	Address = *(uint64_t*)Address;
	return *this;
}
uint64_t Pattern::get() const
{
	return Address;
}


