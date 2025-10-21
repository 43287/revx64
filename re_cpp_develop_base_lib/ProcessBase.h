#pragma once
#include "pch.h"

namespace Process
{
	// retCode
	enum Scode :uint8_t
	{
		success,
	};
	struct BaseProcessInfo
	{
		std::string name;
		DWORD pid;
		HANDLE hProcess = NULL;
	};
	namespace anonymous
	{
		// 进程基类
		// 仅实现进程基本信息的读取和句柄保存和打开
		class ProcessBase
		{
		private://data
			union
			{
				DWORD control;
				struct
				{
					DWORD padding : 24;
					Scode retCode : 7;
					DWORD isOpened : 1;
				};
			};
			DWORD pid;
			std::string name;//主模块名
			HANDLE hProcess;//进程句柄

		private://init
			ProcessBase(
				DWORD pid,
				std::string_view exeName,
				HANDLE hProcess = nullptr,
				DWORD control = 0
			);
		protected:
			ProcessBase(BaseProcessInfo info) :control(0), pid(info.pid), name(info.name), hProcess(info.hProcess) {}
			~ProcessBase();
		public:
			//collect
			static BaseProcessInfo collectByName(const std::string& exeName);
			static BaseProcessInfo collectByPid(DWORD pid);
			static BaseProcessInfo collectByWindowName(std::string& className);
		public:
			ProcessBase& open(DWORD desiredAccess = PROCESS_ALL_ACCESS);

		public:
			//getPrivateData
			//Scode getError();
			bool isOpen() const;
			DWORD getPid() const;
			HANDLE getHandle() const;
			std::string getExeName();
		};
	}
	class Process;

	

	


	// 进程类，额外加入内存操作
	class Process :public anonymous::ProcessBase
	{
	private:
		Process(BaseProcessInfo info);
	public:
		static Process initByName(const std::string& exeName);
		static Process initByPid(DWORD pid);
		static Process initByWindowName(std::string& className);

	public:
		// 自定类型读写
		template<typename T>
		bool read(LPCVOID addr, T& localBuffer);
		template<typename T>
		bool write(LPVOID addr, T& localBuffer);

		// 读
		BYTE readByte(LPVOID addr);
		WORD readWord(LPVOID addr);
		DWORD readDword(LPVOID addr);
		DWORD64 readQword(LPVOID addr);
		std::vector<BYTE> readVector(LPVOID addr,size_t len);

		// 写
		void writeByte(LPVOID addr, BYTE data);
		void writeWord(LPVOID addr, WORD data);
		void writeDword(LPVOID addr, DWORD data);
		void writeQword(LPVOID addr, DWORD64 data);
		void writeVector(LPVOID addr, std::vector<BYTE>& data);


	public:
		uintptr_t getPEB();
		uintptr_t getBase();

	};

}
