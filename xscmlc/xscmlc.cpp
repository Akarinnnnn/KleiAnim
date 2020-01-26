// xscmlc.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <filesystem>
#include "../Decompiler/arg_parser.h"
using std::wcout;
using std::endl;
constexpr wchar_t helpmsg[] =
L"xscml动画编译器\n"
"用法：\n"
"/xscml xscml文件\n"
"/out 输出文件夹\n"
"\n\n";
using std::filesystem::exists;
using std::filesystem::path;
__declspec(dllexport) void CompileXScml(path xscml, path out);

int wmain(int argc, wchar_t** argv)
{
#pragma warning(push)
#pragma warning (disable:26444)
	std::wcout.imbue(std::locale(""));
#pragma warning(pop)

	ArgumentParser parser;
	parser.SetHelpMessage(helpmsg);
	parser.AddString(L"xscml");
	parser.AddString(L"out");

	std::filesystem::path xscml, out;
	xscml = parser.GetString(L"xscml");
	out = parser.GetString(L"out");

	if (!exists(xscml))
	{
		wcout << L"文件不存在," << xscml << endl;
		return 1;
	}

	try
	{
		std::filesystem::create_directories(out);
		CompileXScml(xscml, out);
	}
	catch (const std::exception& e)
	{
		std::cout << e.what();
		return 2;
	}
}

void CompileXScml(path xscml, path out)
{

}