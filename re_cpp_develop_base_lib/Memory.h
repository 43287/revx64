#pragma once
#include <vector>


namespace Memory
{
	// �ײ������д��װ
	bool readRemoteMemory(HANDLE hProcess, LPCVOID remoteAddr, PVOID localBuffer, SIZE_T size, SIZE_T* outTransferred = nullptr);
	bool writeRemoteMemory(HANDLE hProcess, LPVOID remoteAddr, PVOID localBuffer, SIZE_T size, SIZE_T* outTransferred = nullptr);

	// ��д��װ����дһ������
	template<typename T>
	bool readRemote(HANDLE hProcess, LPCVOID remoteAddr, T& localBuffer);

	template<typename T>
	bool writeRemote(HANDLE hProcess, LPVOID remoteAddr, T& localBuffer);

	// ��ҳ��д���ݲ�ʵ��
	bool readRemoteCrossPage();
	bool writeRemoteCrossPage();

	// �ڴ�Ȩ��
	bool memProtect(DWORD& savedAccess, DWORD desiredAccess);

	// �ڴ����룬Ȩ���޸ĵ��ڴ����
	class MemManager {
		struct addressWithLength
		{
			uintptr_t startAddr;
			DWORD length;
		};
		std::vector<addressWithLength> addressOfAlloc = {};
		// �����ڴ�
		uintptr_t alloc(size_t size, DWORD desiredAccess);

		
	};
}
