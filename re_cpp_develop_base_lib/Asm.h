#pragma once
/// <summary>
/// �ÿռ�����ʵ���ַ��������ָ���ֽ�����໥ת���Լ��޸��ֽ������Ϊ
/// </summary>
namespace Asm
{
	bool string2Asm(std::string_view asmStr, PVOID localBuffer);
	bool asm2String(std::string& asmStr, PVOID localBuffer);

	
};

