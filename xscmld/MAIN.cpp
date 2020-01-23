// xscmld.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <filesystem>
#include <cmath>
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

void KleiAnim::Tool::decompile_scml(path animation, path buildpath, float framerate)
{
	pugi::xml_document scml;
	auto scml_root = scml.append_child("spriter_data");
	scml_root.append_attribute("scml_version").set_value("1.0");
	scml_root.append_attribute("generator").set_value("KleiAnim.xscmld");
	scml_root.append_attribute("generator_version").set_value("1.0.0");

	Binary::BuildReader build(buildpath);
	Binary::AnimationReader anim(animation);

	//文件
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
				file.append_attribute("pivot_x").set_value("0");
				file.append_attribute("pivot_y").set_value("0");
			}
			else
			{
				//TODO:群友们出个主意，先留个单帧
				auto frame0 = symbol.frames[0];
				file.append_attribute("width").set_value(frame0.w);
				file.append_attribute("height").set_value(frame0.h);
				file.append_attribute("pivot_x").set_value("0");
				file.append_attribute("pivot_y").set_value("0");
			}
		}
	}

	//动画
	{
		auto bank = scml_root.append_child("entity");
		bank.append_attribute("id").set_value("0");
		bank.append_attribute("name")
			.set_value(animation.filename().replace_extension().c_str());
		uint32_t animation_id = 0;
		for (auto& animnode : anim)
		{
			auto xanim = bank.append_child("animation");
			xanim.append_attribute("name").set_value(animnode.name.c_str());
			xanim.append_attribute("id").set_value(animation_id);
			xanim.append_attribute("length").set_value(animnode.frames.size());
			auto mainline = xanim.append_child("mainline");
			//默认帧率30fps
			float interp_rate = framerate / animnode.frame_rate;
			{
				//TODO: 单物件的动画时间轴
				for (auto& frame : animnode.frames)
				{
					auto kf = mainline.append_child("key");//kf = key frame 关键帧
					for (auto& sym : frame.elements)
					{
						auto xobj = kf.append_child("object");
						xobj.append_attribute("folder").set_value("0");

					}
				}
				//TODO: 组装mainline
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
	parser.AddString(L"anim");
	parser.AddString(L"build");
	parser.AddString(L"out");

	parser.Parse(argc, argv);

	std::wstring out = parser.GetString(L"out");
	path anim = parser.GetString(L"anim");
	path build = parser.GetString(L"build");
	if (out[0] == L'\0')
		out = std::filesystem::absolute(anim.parent_path() / L"编译输出\\");

	KleiAnim::Tool::decompile_scml(anim, build);
}