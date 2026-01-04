// YZVerify.cpp
#include "YZ.h"
#include <windows.h>
#include <wininet.h>
#include <sstream>
#pragma comment(lib, "wininet.lib")

static std::string HttpGet(const std::string& url) {
    HINTERNET hInet = InternetOpenA("Mozilla/5.0", INTERNET_OPEN_TYPE_PRECONFIG, nullptr, nullptr, 0);
    if (!hInet) return "";
    HINTERNET hUrl = InternetOpenUrlA(hInet, url.c_str(), nullptr, 0, INTERNET_FLAG_RELOAD | INTERNET_FLAG_NO_CACHE_WRITE, 0);
    if (!hUrl) { InternetCloseHandle(hInet); return ""; }
    std::stringstream ss;
    char buf[4096];
    DWORD read = 0;
    while (InternetReadFile(hUrl, buf, sizeof(buf), &read) && read)
        ss.write(buf, read);
    InternetCloseHandle(hUrl);
    InternetCloseHandle(hInet);
    return ss.str();
}

static std::string GetMachineCode() {
    DWORD sn = 0;
    GetVolumeInformationA("C:\\", nullptr, 0, &sn, nullptr, nullptr, nullptr, 0);
    char buf[16];
    sprintf_s(buf, "%08X", sn);
    return std::string(buf);
}

bool YZVerify(const std::string& card) {
    static std::string mark = GetMachineCode();
    std::string url = "https://yz.52tyun.com/api.php?api=kmlogon&app=23736&kami=" + card + "&markcode=" + mark;
    std::string resp = HttpGet(url);
    // ×î¼òµ¥µÄ JSON ÅÐ¶Ï
    return resp.find(R"("code":200)") != std::string::npos;
}