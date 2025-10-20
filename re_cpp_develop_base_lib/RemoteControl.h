#pragma once
namespace RemoteControl
{
	// Զ��Patch����ͨ��write�Ϳ���patch�����ﵥ��������Ϊ���з�������patchǰ������ݣ�����ָ�
	
	class remotePatch {

	public:
		struct patchRecord {
			std::vector<BYTE> orig;
			std::vector<BYTE> patch;
			bool applied = false;
		};
		;
		// patch,���Զ����浽�ṹ��
		template<typename T>
		static bool patch(HANDLE hProcess, uintptr_t addr, T& localBuffer);
		static bool patchByte(HANDLE hProcess,uintptr_t addr, BYTE data);
		static bool patchWord(HANDLE hProcess, uintptr_t addr, WORD data);
		static bool patchDword(HANDLE hProcess, uintptr_t addr, DWORD data);
		static bool patchQword(HANDLE hProcess, uintptr_t addr, DWORD64 data);

		static bool patch(HANDLE hProcess, uintptr_t addr, std::vector<BYTE> data);

	private:
		class patchManager {
			static std::unordered_map<uintptr_t, remotePatch::patchRecord> patchList;
			static std::vector<uintptr_t> patchedAddr;

			bool doPatch(uintptr_t addr, std::span<BYTE> patch = {});
			bool undoPatch(uintptr_t addr);
			bool delPatch(uintptr_t addr);
		};
	};

	// Զ��hook������patch��memory
	class remoteHook {
		enum class hookMethod {
			inlineHook,
			stealHook,
		};
	};

	// ��������
	class remoteCall {
		struct remoteCallArgs {
			union
			{
				DWORD64 value;
				uintptr_t addr;
			};
		};
		enum class callType {
			stdcall,
			ctypecall,
			fastcall,
			thiscall
		};
		// ������������Զ�����shellcode������
		virtual remoteCallArgs makeCallx64(LPVOID targetAddr, std::vector<remoteCallArgs>& args);
	};



	// dllע����
	class injector {
		enum class targetFunc {
			loadLibrary,
			ldrloadDll,
			manualMap,
			ime,
			setWindowsHookEx,
		};
		enum class openProcessMethod {
			openProcess,
			steal
		};
		enum class injectMethod {
			createRemoteThread,
			rtlCreateUserThread,
			ntCreateThreadEx,
			queueUserApc,
			processInstrumentationCallBack,
			etwCallback
		};
		enum class kernelMode {
			no,
			yes
		};

		//ע��shellcode��ִ��
		virtual bool injectCode(HANDLE hProcess, PVOID localCode);
		//ע��dll��ִ��,��װ���injectCode��ִֻ��localCode����Ҫ��ʵ��localCode��ת��localDLL
		virtual bool injectDll(HANDLE hProcess,PVOID localCode, PVOID localDLL);
		//����dllע��
		virtual bool injectDll(HANDLE hProcess, PVOID localDLL, targetFunc tf, openProcessMethod opm, injectMethod im, kernelMode km = kernelMode::no);
	};
};

