#pragma once
/// <summary>
/// 该空间用来实现字符串到汇编指令字节码的相互转换以及修改字节码等行为
/// </summary>
namespace Asm
{
	bool string2Asm(std::string_view asmStr, PVOID localBuffer);
	bool asm2String(std::string& asmStr, PVOID localBuffer);

	
};

