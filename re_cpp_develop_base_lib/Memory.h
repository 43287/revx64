#pragma once
#include <vector>


namespace Memory
{
	// 底层基本读写封装
	bool readRemoteMemory(HANDLE hProcess, LPCVOID remoteAddr, PVOID localBuffer, SIZE_T size, SIZE_T* outTransferred = nullptr);
	bool writeRemoteMemory(HANDLE hProcess, LPVOID remoteAddr, PVOID localBuffer, SIZE_T size, SIZE_T* outTransferred = nullptr);

	// 读写封装，读写一个类型
	template<typename T>
	bool readRemote(HANDLE hProcess, LPCVOID remoteAddr, T& localBuffer);

	template<typename T>
	bool writeRemote(HANDLE hProcess, LPVOID remoteAddr, T& localBuffer);

	// 跨页读写，暂不实现
	bool readRemoteCrossPage();
	bool writeRemoteCrossPage();

	// 内存权限
	bool memProtect(DWORD& savedAccess, DWORD desiredAccess);

	// 内存申请，权限修改等内存管理
	class MemManager {
		struct addressWithLength
		{
			uintptr_t startAddr;
			DWORD length;
		};
		std::vector<addressWithLength> addressOfAlloc = {};
		// 申请内存
		uintptr_t alloc(size_t size, DWORD desiredAccess);

		
	};
}
