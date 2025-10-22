#pragma once
#include <vector>


namespace Memory
{
	// 底层基本读写封装
	bool readRemoteMemory(HANDLE hProcess, LPCVOID remoteAddr, PVOID localBuffer, SIZE_T size, SIZE_T* outTransferred = nullptr);
	bool writeRemoteMemory(HANDLE hProcess, LPVOID remoteAddr, PVOID localBuffer, SIZE_T size, SIZE_T* outTransferred = nullptr);

	// 读写封装，读写一个类型
	template<typename T>
	bool readRemote(HANDLE hProcess, LPCVOID remoteAddr, T& localBuffer)
	{
		static_assert(std::is_trivially_copyable_v<T>,"类型直接读取仅针对于平凡可拷贝类型");
		T tmp;
		size_t szRead;
		if (!readRemoteMemory(hProcess, remoteAddr,&tmp , sizeof(T), &szRead))
		{
			return false;
		}
		localBuffer = tmp;
		return true;
	}

	template<typename T>
	bool writeRemote(HANDLE hProcess, LPVOID remoteAddr, T& localBuffer)
	{
		static_assert(std::is_trivially_copyable_v<T>, "类型直接写入仅针对于平凡可拷贝类型");
		T tmp = localBuffer;
		size_t szWrite;
		if (!writeRemoteMemory(hProcess,remoteAddr,reinterpret_cast<PVOID>(tmp),sizeof(T),&szWrite))
		{
			return false;
		}
		return true;
	}



	///TODO:

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
