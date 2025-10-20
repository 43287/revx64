#pragma once
namespace RemoteControl
{
	// 远程Patch，普通的write就可以patch，这里单独成类是为了有方法保存patch前后的内容，方便恢复
	
	class remotePatch {

	public:
		struct patchRecord {
			std::vector<BYTE> orig;
			std::vector<BYTE> patch;
			bool applied = false;
		};
		;
		// patch,并自动保存到结构体
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

	// 远程hook，基于patch和memory
	class remoteHook {
		enum class hookMethod {
			inlineHook,
			stealHook,
		};
	};

	// 主动调用
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
		// 主动传入参数自动构造shellcode并调用
		virtual remoteCallArgs makeCallx64(LPVOID targetAddr, std::vector<remoteCallArgs>& args);
	};



	// dll注入器
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

		//注入shellcode并执行
		virtual bool injectCode(HANDLE hProcess, PVOID localCode);
		//注入dll并执行,封装后的injectCode，只执行localCode，需要自实现localCode跳转到localDLL
		virtual bool injectDll(HANDLE hProcess,PVOID localCode, PVOID localDLL);
		//最顶层的dll注入
		virtual bool injectDll(HANDLE hProcess, PVOID localDLL, targetFunc tf, openProcessMethod opm, injectMethod im, kernelMode km = kernelMode::no);
	};
};

