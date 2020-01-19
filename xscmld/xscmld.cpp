// xscmld.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <filesystem>
#include "../Decompiler/arg_parser.h"
#include "../klei_anim/KleiAnim/Binary.hpp"

wchar_t helpmsg[] =
L"帮助信息\n"
"参数：\n"
"/in ，存放将要反编译动画包的文件夹\n"
"/out ，可选，输出scml的路径\n\n\n";
using std::filesystem::path;
__declspec(dllexport) void decompile_scml(path animation,path build)
{

}

int wmain(int argc,wchar_t** argv)
{
#pragma warning(push)
#pragma warning (disable:26444)
	std::wcout.imbue(std::locale(""));
#pragma warning(pop)
	{ KleiAnim::Common::WideCharLog a(std::wcout); }
	ArgumentParser parser;
	parser.SetHelpMessage(helpmsg);
	parser.AddString(L"in");
	parser.AddString(L"out");

	parser.Parse(argc, argv);

	std::wstring out = parser.GetString(L"out");
	path in = parser.GetString(L"in");
	if (out[0] == L'\0')
		out = std::filesystem::absolute(in.parent_path() / L"编译输出\\");

}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
