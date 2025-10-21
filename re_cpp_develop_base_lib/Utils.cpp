#include "pch.h"
#include "Utils.h"
#include <tlhelp32.h>
#include <algorithm>
#include <cstring>

std::wstring utils::string::StringToWString(const std::string& s, UINT codePage)
{
    if (s.empty()) return {};
    int required = MultiByteToWideChar(codePage, 0, s.data(), (int)s.size(), nullptr, 0);
    if (required == 0) return {};
    std::wstring out;
    out.resize(required);
    MultiByteToWideChar(codePage, 0, s.data(), (int)s.size(), &out[0], required);
    return out;
}

std::string utils::string::WStringToString(const std::wstring& ws, UINT codePage)
{
    if (ws.empty()) return {};
    int required = WideCharToMultiByte(codePage, 0, ws.data(), (int)ws.size(), nullptr, 0, nullptr, nullptr);
    if (required == 0) return {};
    std::string out;
    out.resize(required);
    WideCharToMultiByte(codePage, 0, ws.data(), (int)ws.size(), &out[0], required, nullptr, nullptr);
    return out;
}

std::string utils::tltool::GetExeNameByPid(DWORD pid)
{
    HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snap == INVALID_HANDLE_VALUE) return {};

    PROCESSENTRY32W pe{};
    pe.dwSize = sizeof(pe);
    if (!Process32FirstW(snap, &pe)) {
        CloseHandle(snap);
        return {};
    }

    std::wstring result;
    do {
        if (pe.th32ProcessID == pid) {
            result = std::wstring(pe.szExeFile);
            break;
        }
    } while (Process32NextW(snap, &pe));

    CloseHandle(snap);
    return string::WStringToString(result);
}

DWORD utils::tltool::getPidByName(const std::string& exeName)
{
    if (exeName.empty()) return 0;

    std::wstring wExeName = utils::string::StringToWString(exeName);

    HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snap == INVALID_HANDLE_VALUE) return 0;

    PROCESSENTRY32W pe{};
    pe.dwSize = sizeof(pe);
    DWORD foundPid = 0;

    if (Process32FirstW(snap, &pe)) {
        do {
            if (_wcsicmp(pe.szExeFile, wExeName.c_str()) == 0) {
                foundPid = pe.th32ProcessID;
                break;
            }
        } while (Process32NextW(snap, &pe));
    }

    CloseHandle(snap);
    return foundPid;
}

DWORD utils::tltool::GetPidByClassNameA(const std::string& className)
{
    HWND hwnd = FindWindowA(className.c_str(), NULL);
    if (!hwnd) throw std::runtime_error("FindWindowA failed");
    DWORD pid = 0;
    GetWindowThreadProcessId(hwnd, &pid);
    return pid;
}

uintptr_t utils::windows::getLocalFuncInModule(std::string moduleName, std::string funcName)
{
    HMODULE hModule = GetModuleHandleA(moduleName.c_str());
    auto ret = GetProcAddress(hModule, funcName.c_str());
    return reinterpret_cast<uintptr_t>(ret);
}
