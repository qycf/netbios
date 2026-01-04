#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include "YZ.h"
#include <ShlObj_core.h>
#include <fstream>
#include <vector>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

static bool ConfirmBox(const wchar_t* text, const wchar_t* caption)
{
    return IDYES == MessageBoxW(nullptr, text, caption, MB_YESNO | MB_ICONQUESTION);
}

bool GetProcessPath(const wchar_t* procName, wchar_t* outPath, DWORD bufLen)
{
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnap == INVALID_HANDLE_VALUE) return false;

    PROCESSENTRY32W pe{ sizeof(pe) };
    for (BOOL b = Process32FirstW(hSnap, &pe); b; b = Process32NextW(hSnap, &pe))
    {
        if (_wcsicmp(pe.szExeFile, procName) == 0)
        {
            HANDLE hProc = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION,
                FALSE, pe.th32ProcessID);
            if (hProc)
            {
                QueryFullProcessImageNameW(hProc, 0, outPath, &bufLen);
                CloseHandle(hProc);
                CloseHandle(hSnap);
                return true;
            }
        }
    }
    CloseHandle(hSnap);
    return false;
}

bool KillProcessByName(const wchar_t* name)
{
    bool ok = false;
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnap == INVALID_HANDLE_VALUE) return false;

    PROCESSENTRY32W pe{ sizeof(pe) };
    for (BOOL b = Process32FirstW(hSnap, &pe); b; b = Process32NextW(hSnap, &pe))
    {
        if (_wcsicmp(pe.szExeFile, name) == 0)
        {
            HANDLE hProc = OpenProcess(PROCESS_TERMINATE, FALSE, pe.th32ProcessID);
            if (hProc)
            {
                TerminateProcess(hProc, 1);   // 1 = 退出码
                CloseHandle(hProc);
                ok = true;
            }
        }
    }
    CloseHandle(hSnap);
    return ok;
}

// 工具：读取整个文件（宽字符路径 -> 返回宽字符串内容）
// 能正确识别 UTF-8 BOM、UTF-16 LE BOM，否则按当前 ANSI (CP_ACP) 解码
static std::wstring ReadWholeFile(const std::wstring& filePath)
{
    HANDLE h = CreateFileW(filePath.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (h == INVALID_HANDLE_VALUE) return L"";

    LARGE_INTEGER size{};
    if (!GetFileSizeEx(h, &size) || size.QuadPart == 0) { CloseHandle(h); return L""; }

    std::vector<char> buf;
    buf.resize(static_cast<size_t>(size.QuadPart));
    DWORD read = 0;
    BOOL ok = ReadFile(h, buf.data(), static_cast<DWORD>(buf.size()), &read, nullptr);
    CloseHandle(h);
    if (!ok || read == 0) return L"";

    // BOM 检测并转换
    if (read >= 3 && (unsigned char)buf[0] == 0xEF && (unsigned char)buf[1] == 0xBB && (unsigned char)buf[2] == 0xBF) {
        // UTF-8 带 BOM
        int chars = MultiByteToWideChar(CP_UTF8, 0, buf.data() + 3, read - 3, nullptr, 0);
        if (chars <= 0) return L"";
        std::wstring out;
        out.resize(chars);
        MultiByteToWideChar(CP_UTF8, 0, buf.data() + 3, read - 3, &out[0], chars);
        return out;
    }
    if (read >= 2 && (unsigned char)buf[0] == 0xFF && (unsigned char)buf[1] == 0xFE) {
        // UTF-16 LE 带 BOM
        size_t wchars = (read - 2) / 2;
        if (wchars == 0) return L"";
        std::wstring out;
        out.resize(wchars);
        memcpy(&out[0], buf.data() + 2, wchars * sizeof(wchar_t));
        return out;
    }

    // 默认按 ANSI 编码 (CP_ACP) 转换为宽字符
    int chars = MultiByteToWideChar(CP_ACP, 0, buf.data(), read, nullptr, 0);
    if (chars <= 0) return L"";
    std::wstring out;
    out.resize(chars);
    MultiByteToWideChar(CP_ACP, 0, buf.data(), read, &out[0], chars);
    return out;
}

// 工具：写入整个文件（宽字符路径 -> 将宽字符串以 UTF-8（带 BOM）写入文件）
// 选择写入 UTF-8 带 BOM 确保跨工具读取一致性
static bool WriteWholeFile(const std::wstring& filePath, const std::wstring& content)
{
    // 转 UTF-8
    int utf8len = WideCharToMultiByte(CP_UTF8, 0, content.c_str(), static_cast<int>(content.size()), nullptr, 0, nullptr, nullptr);
    if (utf8len < 0) return false;
    std::vector<char> out;
    out.reserve(3 + utf8len);
    // 写入 UTF-8 BOM
    out.push_back(static_cast<char>(0xEF));
    out.push_back(static_cast<char>(0xBB));
    out.push_back(static_cast<char>(0xBF));
    out.resize(3 + utf8len);
    WideCharToMultiByte(CP_UTF8, 0, content.c_str(), static_cast<int>(content.size()), out.data() + 3, utf8len, nullptr, nullptr);

    HANDLE h = CreateFileW(filePath.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (h == INVALID_HANDLE_VALUE) return false;
    DWORD written = 0;
    BOOL ok = WriteFile(h, out.data(), static_cast<DWORD>(out.size()), &written, nullptr);
    CloseHandle(h);
    return ok && written == out.size();
}

static bool ConfirmBox(const char* text, const char* caption) {
    return IDYES == MessageBoxA(nullptr, text, caption, MB_YESNO | MB_ICONQUESTION);
}

// 一次性读取整个文本文件（ANSI/UTF-8 文件 -> 返回原始字节按 std::string）
// 如果文件以 UTF-8 BOM 开头，会去掉 BOM 并返回剩余字节（UTF-8 编码）
static std::string ReadWholeFile(const std::string& filePath) {
    std::ifstream ifs(filePath, std::ios::binary);
    if (!ifs) return "";
    std::string s((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
    // 去掉 UTF-8 BOM（如果存在）
    if (s.size() >= 3 && (unsigned char)s[0] == 0xEF && (unsigned char)s[1] == 0xBB && (unsigned char)s[2] == 0xBF) {
        s.erase(0, 3);
    }
    return s;
}

// 一次性写入整个文本文件（按原始字节写入）
// 推荐使用 WriteWholeFile(std::wstring) 来保证写入为 UTF-8 带 BOM（跨工具友好）
// 这里的 string 版本直接写入 bytes（不自动添加 BOM）
static bool WriteWholeFile(const std::string& filePath, const std::string& content) {
    std::ofstream ofs(filePath, std::ios::binary | std::ios::trunc);
    if (!ofs) return false;
    ofs.write(content.data(), static_cast<std::streamsize>(content.size()));
    return ofs.good();
}


int main() {
    SetConsoleTitleW(L"DeltaForce");

    wchar_t wDocPath[MAX_PATH];
    if (S_OK != SHGetFolderPathW(nullptr, CSIDL_MYDOCUMENTS, nullptr, SHGFP_TYPE_CURRENT, wDocPath))
    {
        MessageBoxW(nullptr, L"无法获取系统目录", L"错误", MB_ICONERROR);
        return -1;
    }

    std::wstring kmPath = wDocPath;
    kmPath += L"\\KM";          // 宽字符

    int ansiLen = WideCharToMultiByte(CP_ACP, 0, kmPath.c_str(), -1,
        nullptr, 0, nullptr, nullptr);
    std::string kmFile(ansiLen - 1, 0);
    WideCharToMultiByte(CP_ACP, 0, kmPath.c_str(), -1,
        &kmFile[0], ansiLen, nullptr, nullptr);

    // 2. 尝试读取上次卡密
    std::string cardKey;
    bool hasHistory = false;
    if (GetFileAttributesA(kmFile.c_str()) != INVALID_FILE_ATTRIBUTES)
    {
        std::string history = ReadWholeFile(kmFile);
        if (!history.empty())
        {
            hasHistory = true;
            if (ConfirmBox("检测到上次登录卡密，是否沿用？", "历史卡密"))
                cardKey = history;
        }
    }

    // 3. 若未沿用历史，则手动输入
    if (cardKey.empty())
    {
        std::cout << "请输入卡密：";
        std::getline(std::cin, cardKey);
    }

    // 4. 验证
    if (!YZVerify(cardKey))
    {
        MessageBoxA(nullptr, "卡密验证失败，请检查卡密输入是否正确", "验证失败", MB_ICONERROR);
        return -1;
    }

    // 5. 验证通过，保存卡密
    if (!WriteWholeFile(kmFile, cardKey))
    {
        MessageBoxA(nullptr, "保存卡密失败，可能需要管理员权限", "提示", MB_ICONWARNING);
    }

    MessageBoxA(nullptr, "验证成功 请打开游戏\n 点击确定 开始初始化(耐心等待)！", "提示", MB_ICONINFORMATION);


    HANDLE hRead = nullptr, hWrite = nullptr;
    SECURITY_ATTRIBUTES sa = { sizeof(sa), nullptr, TRUE };
    if (!CreatePipe(&hRead, &hWrite, &sa, 0)) {
        MessageBoxA(nullptr, "创建管道失败", "错误", MB_ICONERROR);
        return -1;
    }
    SetHandleInformation(hRead, HANDLE_FLAG_INHERIT, 0);

    STARTUPINFOA si = { 0 };
    PROCESS_INFORMATION pi = { 0 };
    si.cb = sizeof(si);
    si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
    si.wShowWindow = SW_HIDE;
    si.hStdOutput = hWrite;
    si.hStdError = hWrite;
    si.hStdInput = GetStdHandle(STD_INPUT_HANDLE);

    wchar_t selfPath[MAX_PATH];
    GetModuleFileNameW(NULL, selfPath, MAX_PATH);
    PathRemoveFileSpecW(selfPath);   
    PathAppendW(selfPath, L"netbios.dll");

    wchar_t path[MAX_PATH]{};
    if (!GetProcessPath(L"DeltaForceClient-Win64-Shipping.exe", path, MAX_PATH))
    {
        printf("[info] 请先启动游戏\n");
        Sleep(2000);
        return 0;
    }

    PathRemoveFileSpecW(path);

    wchar_t destPath[MAX_PATH];
    PathCombineW(destPath, path, L"netbios.dll");

    if (CopyFileW(selfPath, destPath, FALSE)) {
        
        if (KillProcessByName(L"DeltaForceClient-Win64-Shipping.exe")) {
            printf("[true] 注入成功 -> 请重新启动游戏 即可享受\n");
        }
        else {
			printf("[err] 进程控制失败，请手动退出游戏 重新启动\n");
        }
    }
    else {
        printf("[err] 加载失败，错误码：%lu\n", GetLastError());
    }
        

    Sleep(3000);
    return 0;
}