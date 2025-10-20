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
		// ���̻���
		// ��ʵ�ֽ��̻�����Ϣ�Ķ�ȡ�;������ʹ�
		class ProcessBase
		{
		private://data
			union
			{
				DWORD control;
				struct
				{
					char padding : 24;
					Scode retCode : 7;
					char isOpened : 1;
				};
			};
			DWORD pid;
			std::string name;//��ģ����
			HANDLE hProcess;//���̾��

		private://init
			ProcessBase(
				DWORD pid,
				std::string_view exeName,
				HANDLE hProcess = nullptr,
				DWORD control = 0
			) :
				control(control),
				pid(pid),
				name(exeName),
				hProcess(hProcess)
			{
			}
			ProcessBase(BaseProcessInfo info) :control(0), pid(info.pid), name(info.name), hProcess(info.hProcess) {}
			~ProcessBase();
		public:
			//init
			//static ProcessBase initByName(std::string& exeName, bool isOpen = false);
			//static ProcessBase initByPid(DWORD pid, bool isOpen = false);
			//static ProcessBase initByWindowName(std::string& exeName, bool isOpen = false);

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

	

	


	// �����࣬��������ڴ����
	class Process :public anonymous::ProcessBase
	{
	private:
		Process(BaseProcessInfo info);
	public:
		static Process initByName(const std::string& exeName);
		static Process initByPid(DWORD pid);
		static Process initByWindowName(std::string& className);

	public:
		// �Զ����Ͷ�д
		template<typename T>
		bool read(LPCVOID addr, T& localBuffer);
		template<typename T>
		bool write(LPVOID addr, T& localBuffer);

		// ��
		BYTE readByte(LPVOID addr);
		WORD readWord(LPVOID addr);
		DWORD readDword(LPVOID addr);
		DWORD64 readQword(LPVOID addr);
		std::vector<BYTE> readVector(LPVOID addr,size_t len);

		// д
		void writeByte(LPVOID addr, BYTE data);
		void writeWord(LPVOID addr, WORD data);
		void writeDword(LPVOID addr, DWORD data);
		void writeQword(LPVOID addr, DWORD64 data);
		void writeVector(LPVOID addr, std::vector<BYTE>& data);
	};

}
