#include "pch.h"
#include "Memory.h"


namespace Memory
{
	bool readRemoteMemory(HANDLE hProcess, LPCVOID remoteAddr, PVOID localBuffer, SIZE_T size, SIZE_T* outTransferred)
	{
		// 非跨页

		if (outTransferred)*outTransferred = 0;
		if (!hProcess || hProcess == INVALID_HANDLE_VALUE || remoteAddr == nullptr || localBuffer == nullptr || size == 0) {
			SetLastError(ERROR_INVALID_PARAMETER);
			return false;
		}
		// 检查是否跨页

		MEMORY_BASIC_INFORMATION mbiStart;
		size_t q1 = VirtualQueryEx(hProcess, remoteAddr, &mbiStart, sizeof(mbiStart));
		if (!q1)
		{
			SetLastError(ERROR_PARTIAL_COPY);
			return false;
		}

		const PBYTE endAddr = size + PBYTE(remoteAddr) - 1;
		MEMORY_BASIC_INFORMATION mbiEnd;
		size_t q2 = VirtualQueryEx(hProcess, endAddr, &mbiEnd, sizeof(mbiEnd));
		if (!q2|| 
			//mbiStart.BaseAddress != mbiEnd.BaseAddress|| //将跨页条件暂时取消
			mbiStart.Protect & PAGE_NOACCESS || 
			mbiStart.Protect & PAGE_GUARD)
		{
			SetLastError(ERROR_PARTIAL_COPY);
			return false;
		}
		size_t bytesRead = 0;
		BOOL ok = ReadProcessMemory(hProcess, remoteAddr, localBuffer, size, &bytesRead);
		if (!ok) {// 如果没有成功，可直接读ReadProcessMemory设置的error
			return false;
		}
		if (bytesRead != size) {
			SetLastError(ERROR_PARTIAL_COPY);
			if (outTransferred) *outTransferred = bytesRead;
			return false;
		}
		if (outTransferred) *outTransferred = bytesRead;
		return true;

	}

	bool writeRemoteMemory(HANDLE hProcess, LPVOID remoteAddr, PVOID localBuffer, SIZE_T size, SIZE_T* outTransferred)
	{
		if (outTransferred) *outTransferred = 0;

		if (!hProcess || hProcess == INVALID_HANDLE_VALUE || remoteAddr == nullptr || localBuffer == nullptr || size == 0) {
			SetLastError(ERROR_INVALID_PARAMETER);
			return false;
		}
		const BYTE* start = static_cast<const BYTE*>(remoteAddr);
		const BYTE* end = start + (size - 1);
		MEMORY_BASIC_INFORMATION mbiStart;
		MEMORY_BASIC_INFORMATION mbiEnd;
		if (!VirtualQueryEx(hProcess, start, &mbiStart, sizeof(mbiStart))||
			!VirtualQueryEx(hProcess, end, &mbiEnd, sizeof(mbiEnd))) 
		{
			SetLastError(ERROR_PARTIAL_COPY);
			return false;
		}
		// 判断是否同页
		if (mbiStart.AllocationBase != mbiEnd.AllocationBase || 
			mbiStart.BaseAddress != mbiEnd.BaseAddress||
			mbiStart.State != MEM_COMMIT||
			mbiStart.Protect&PAGE_GUARD|| mbiStart.Protect & PAGE_NOACCESS)
		{
			SetLastError(ERROR_PARTIAL_COPY);
			return false;
		}
		// 判断是否可写
		DWORD protect = mbiStart.Protect&0xff;
		if (!(protect == PAGE_EXECUTE_READWRITE ||
			protect == PAGE_READWRITE ||
			protect == PAGE_WRITECOPY ||
			protect == PAGE_EXECUTE_WRITECOPY))
		{
			SetLastError(ERROR_NOACCESS);
			return false;
		}
		SIZE_T bytesWritten = 0;
		BOOL ok = WriteProcessMemory(hProcess, remoteAddr, localBuffer, size, &bytesWritten);

		if (!ok) {// 直接使用WriteProcessMemory设置的错误码
			return false;
		}
		if (bytesWritten != size) {
			if (outTransferred) *outTransferred = bytesWritten;
			SetLastError(ERROR_PARTIAL_COPY);
			return false;
		}

		if (outTransferred) *outTransferred = bytesWritten;
		return true;
	}



}
