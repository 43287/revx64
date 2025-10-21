#pragma once

#include <windows.h>
#include <vector>
#include <string>
#include <cstdint>

#include <type_traits>

///
/// 工具杂项
///
/// 权限提升
/// 字符串操作





namespace utils
{
	namespace string {
		std::wstring StringToWString(const std::string& s, UINT codePage = CP_UTF8);
		std::string WStringToString(const std::wstring& ws, UINT codePage = CP_UTF8);
	}

	namespace tltool
	{
		std::string GetExeNameByPid(DWORD pid);
		DWORD getPidByName(const std::string& exeName);
		DWORD GetPidByClassNameA(const std::string& className);

	}

	namespace privilege {

	}

	namespace windows
	{
		uintptr_t getLocalFuncInModule(std::string moduleName, std::string funcName);
	}
}
