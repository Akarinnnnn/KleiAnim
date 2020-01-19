// xscmld.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <filesystem>
#include "../Decompiler/arg_parser.h"
#include "../klei_anim/KleiAnim/Binary.hpp"
#include "decompile.hpp"
#include <pugixml.hpp>

wchar_t helpmsg[] =
L"帮助信息\n"
"参数：\n"
"/in ，存放将要反编译动画包的文件夹\n"
"/out ，可选，输出scml的路径\n\n\n";
using std::filesystem::path;

void KleiAnim::Tool::decompile_scml(path animation, path buildpath)
{
	pugi::xml_document scml;
	auto scml_root = scml.append_child("spriter_data");
	scml_root.append_attribute("scml_version").set_value("1.0");
	scml_root.append_attribute("generator").set_value("KleiAnim.xscmld");
	scml_root.append_attribute("generator_version").set_value("1.0.0");

	Binary::BuildReader build(buildpath);
	Binary::AnimationReader anim(animation);

	{
		static path picpath(L"pictures");

		auto pics = scml_root.append_child("folder");
		pics.append_attribute("id").set_value("0");
		pics.append_attribute("name").set_value("pictures");

		unsigned int picid = 0;
		for (auto& symbol : build)
		{
			auto file = pics.append_child("file");
			file.append_attribute("name").set_value((picpath / build.de_hash(symbol.name_hash)).c_str());
			file.append_attribute("id").set_value(picid);
			
			if (symbol.frames.size() == 1)
			{
				auto frame0 = symbol.frames[0];
				file.append_attribute("width").set_value(frame0.w);
				file.append_attribute("height").set_value(frame0.h);
				//TODO
			}
			else
			{
				//TODO:多帧
			}
		}
	}
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
