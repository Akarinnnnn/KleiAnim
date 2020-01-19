// animc.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include <iostream>
#include "../Decompiler/arg_parser.h"
#include "../klei_anim/KleiAnim/Xml.hpp"
#include <filesystem>
using std::filesystem::path;
wchar_t helpmsg[] =
L"帮助信息\n"
"参数：\n"
"/anim ，动画xml文件\n"
"/build，Build xml文件\n"
"/out ，可选，输出文件夹\n\n\n";
int wmain(int argc,wchar_t** argv)
{
#pragma warning(push)
#pragma warning (disable:26444)
	std::wcout.imbue(std::locale(""));
#pragma warning(pop)
	{
		KleiAnim::Common::WideCharLog a(std::wcout);
	}
	ArgumentParser parser;
	parser.SetHelpMessage(helpmsg);
	parser.AddString(L"anim");
	parser.AddString(L"build");
	parser.AddString(L"out");

	parser.Parse(argc, argv);
	try
	{
	path build = std::filesystem::canonical(parser.GetString(L"build"));
	path anim = std::filesystem::canonical(parser.GetString(L"anim"));
	std::wstring out = parser.GetString(L"out");

	if (out[0] == L'\0')
		out = std::filesystem::absolute(anim.parent_path() / L"编译输出\\");
	{
		std::filesystem::create_directory(out);

		using namespace KleiAnim::XML;
		try { XML2Bin(anim, out); }
		catch (const std::exception & e) { std::cout << e.what() << std::endl; }

		try { XML2Bin(build, out); }
		catch (const std::exception & e) { std::cout << e.what() << std::endl; }
	}
	}
	catch (const std::exception & e)
	{
		std::cout << e.what() << std::endl;
	}
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
