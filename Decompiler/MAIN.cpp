// Decompiler.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <filesystem>
#include "arg_parser.h"
#include "../klei_anim/KleiAnim/xml.hpp"
using namespace std::filesystem;

using std::filesystem::path;
wchar_t helpmsg[] = 
L"帮助信息\n"
"参数：\n"
"/in ，存放将要反编译动画的路径\n"
"/out ，可选，输出文件夹\n\n\n";
int wmain(int argc,wchar_t** argv)
{

	ArgumentParser parser;
	parser.SetHelpMessage(helpmsg);
	parser.AddString(L"in");
	parser.AddString(L"out");

	parser.Parse(argc, argv);

	try
	{
		path input = std::filesystem::canonical(parser.GetString(L"in"));
		std::wstring out = parser.GetString(L"out");
		if (out[0] == L'\0')
			out = std::filesystem::absolute(input / input.parent_path());
		std::filesystem::create_directories(out);//先创建文件夹，fopen等才能自动创建文件
		{
			using namespace KleiAnim::XML;
			try
			{
				AnimBin2XML(input / L"anim.bin", out / path(L"anim.xml"));
			}
			catch (const filesystem_error& e)
			{
				std::cout << "缺少anim.bin，跳过" << e.what();
			}
			try
			{
				BuildBin2XML(input / L"build.bin", out / path(L"build.xml"));
			}
			catch (const filesystem_error& e)
			{
				std::cout << "缺少build.bin，跳过" << e.what();
			}
		}
	}
	catch (const std::exception& e)
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
