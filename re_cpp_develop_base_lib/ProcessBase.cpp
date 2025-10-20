#include "pch.h"
#include "ProcessBase.h"
#include "memory.h"
#include "tlhelp32.h"
#include "Utils.h"

namespace Process
{
	anonymous::ProcessBase::~ProcessBase()
	{
		if (hProcess)
		{
			CloseHandle(hProcess);
		}
	}

	BaseProcessInfo anonymous::ProcessBase::collectByName(const std::string& exeName)
	{
		if (exeName.empty()) return {};
		auto pid = utils::tltool::getPidByName(exeName);
		if (!pid) return {};
		return {.name = exeName, .pid = pid, .hProcess = nullptr};
	}

	BaseProcessInfo anonymous::ProcessBase::collectByPid(DWORD pid)
	{
		if (!pid) return {};
		auto exeName = utils::tltool::GetExeNameByPid(pid);
		return {.name = exeName, .pid = pid, .hProcess = nullptr };
	}

	BaseProcessInfo anonymous::ProcessBase::collectByWindowName(std::string& className)
	{
		DWORD pid = utils::tltool::GetPidByClassNameA(className);
		return collectByPid(pid);
	}

	anonymous::ProcessBase& anonymous::ProcessBase::open(DWORD desiredAccess)
	{
		if (hProcess)return *this;
		HANDLE h = OpenProcess(desiredAccess, FALSE, pid);
		if (!h)
		{
			throw std::runtime_error("open process failed");
		}
		hProcess = h;
		return *this;
	}

	bool anonymous::ProcessBase::isOpen() const
	{
		return hProcess != 0;
	}

	DWORD anonymous::ProcessBase::getPid() const
	{
		return pid;
	}

	HANDLE anonymous::ProcessBase::getHandle() const
	{
		return hProcess;
	}

	std::string anonymous::ProcessBase::getExeName()
	{
		return name;
	}


	template <typename T>
	bool Process::read(LPCVOID addr, T& localBuffer)
	{
		Memory::readRemote(getHandle(), addr, localBuffer);
		return true;
	}

	template <typename T>
	bool Process::write(LPVOID addr, T& localBuffer)
	{
		Memory::writeRemote(getHandle(), addr, localBuffer);
		return true;
	}


	Process::Process(BaseProcessInfo info): ProcessBase(info)
	{
	}

	Process Process::initByName(const std::string& exeName)
	{
		auto d = ProcessBase::collectByName(exeName);
		return Process(d);
	}

	Process Process::initByPid(DWORD pid)
	{
		auto d = ProcessBase::collectByPid(pid);
		return Process(d);
	}

	Process Process::initByWindowName(std::string& className)
	{
		auto d = ProcessBase::collectByWindowName(className);
		return Process(d);
	}

	BYTE Process::readByte(LPVOID addr)
	{
		BYTE ret;
		if (!read(addr, ret))
		{
			throw std::runtime_error("read failed");
		}
		return ret;
	}
	WORD Process::readWord(LPVOID addr)
	{
		WORD ret;
		if (!read(addr, ret))
		{
			throw std::runtime_error("read failed");
		}
		return ret;
	}
	DWORD Process::readDword(LPVOID addr)
	{
		DWORD ret;
		if (!read(addr, ret))
		{
			throw std::runtime_error("read failed");
		}
		return ret;
	}
	DWORD64 Process::readQword(LPVOID addr)
	{
		DWORD64 ret;
		if (!read(addr, ret))
		{
			throw std::runtime_error("read failed");
		}
		return ret;
	}
	std::vector<BYTE> Process::readVector(LPVOID addr, size_t len)
	{
		std::vector<BYTE> ret;
		if (len == 0)return ret;
		ret.resize(len);
		auto requiredLen = len;
		if (!Memory::readRemoteMemory(getHandle(), addr, ret.data(), len, &requiredLen))
		{
			throw std::runtime_error("read failed");
		}
		// 此处未处理长度问题
		return ret;
	}

	void Process::writeByte(LPVOID addr, BYTE data)
	{
		if (!write(addr,data))
		{
			throw std::runtime_error("write failed");
		}
	}
	void Process::writeWord(LPVOID addr, WORD data)
	{
		if (!write(addr, data))
		{
			throw std::runtime_error("write failed");
		}
	}
	void Process::writeDword(LPVOID addr, DWORD data)
	{
		if (!write(addr, data))
		{
			throw std::runtime_error("write failed");
		}
	}
	void Process::writeQword(LPVOID addr, DWORD64 data)
	{
		if (!write(addr, data))
		{
			throw std::runtime_error("write failed");
		}
	}
	void Process::writeVector(LPVOID addr, std::vector<BYTE>& data)
	{
		if (data.empty())return;
		size_t total = data.size();
		size_t written = 0;
		if (!Memory::writeRemoteMemory(getHandle(),addr,data.data(),total,&written))
		{
			throw std::runtime_error("write failed");
		}
		// 未处理返回长度不同的情况
	}


}
