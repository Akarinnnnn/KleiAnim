#include "../pch.h"
#include "Xml.hpp"

//cnm�Ҿ�����Ƕһ��pugixml
#include "..\thirdparty\pugixml.hpp"
#include <utility>

using namespace std::string_literals;
using pugi::xml_node;
using pugi::xpath_node;
using std::filesystem::path;

std::wstring ToString(const char* str)
{
	std::wostringstream o;
	o << str;
	return o.str();
}

std::wstring LicenseFromHash(uint32_t hash)
{
	using KleiAnim::Common::common_license_hash;
	switch (hash)
	{
	case common_license_hash[0]:
		return L"SPDX-License-Identifier: CC-BY-NC-4.0";
	case common_license_hash[1]:
		return L"SPDX-License-Identifier: MIT";
	case common_license_hash[2]:
		return L"SPDX-License-Identifier: GPL-3.0-or-later";
	default:
		return L"";
		break;
	}
}

void KleiAnim::XML::AnimBin2XML(std::filesystem::path binary, std::filesystem::path xmlpath)
{
	Binary::AnimationReader bin(binary);
	//static_cast����,dymanic_castû���麯��������
	AnimBin2XML(bin, xmlpath);
}

void KleiAnim::XML::AnimBin2XML(Binary::AnimationReader& binary, std::filesystem::path xmlpath)
{
	pugi::xml_document doc;
	auto anims = doc.append_child("Anims");
	unsigned int frame_idx = 0;
	std::string prevname;

	for (auto& animation : binary)
	{
		auto xanim = anims.append_child("anim");//xml anim

		if (animation.name == prevname)
		{
			KleiAnimLog::write() << L"�����ǰ�Ȩ��Ϣ��\n";
			std::cout << animation.name << std::endl;
			system("pause");
		}
		xanim.append_attribute("name").set_value(animation.name.c_str());
		xanim.append_attribute("root").set_value(binary.de_hash(animation.rootsym_hash).c_str());
		xanim.append_attribute("numframes").set_value(animation.frames.size());
		xanim.append_attribute("framerate").set_value(animation.frame_rate);



		for (auto& frame : animation.frames)
		{
			auto xf = xanim.append_child("frame");//xml frame
			frame_idx++;

			xf.append_attribute("idx").set_value(frame_idx);
			xf.append_attribute("x").set_value(frame.x);
			xf.append_attribute("y").set_value(frame.y);
			xf.append_attribute("w").set_value(frame.w);
			xf.append_attribute("h").set_value(frame.h);

			if (frame.events.size() != 0)
				for (auto event : frame.events)
					xf.append_child("event").set_value(binary.de_hash(event.name_hash).c_str());

			for (auto& element : frame.elements)
			{
				auto xelem = xf.append_child("element");//xml element

				xelem.append_attribute("name").set_value(binary.de_hash(element.name_hash).c_str());
				xelem.append_attribute("layername").set_value(binary.de_hash(element.layer_hash).c_str());
				xelem.append_attribute("frame").set_value(frame_idx);
				xelem.append_attribute("z_index").set_value(element.z);
				xelem.append_attribute("m_a").set_value(element.a);
				xelem.append_attribute("m_b").set_value(element.b);
				xelem.append_attribute("m_c").set_value(element.c);
				xelem.append_attribute("m_d").set_value(element.d);
				xelem.append_attribute("m_tx").set_value(element.tx);
				xelem.append_attribute("m_ty").set_value(element.ty);
			}
		}
	}
	auto license = binary.license_hash();
	if (license != 0)
	{
		std::wcout << L"���֤��Ϣ��" << LicenseFromHash(license)
			<< L"\n����ȷ������������֤����ʹ��" << std::endl;
		system("pause");
	}
	doc.save_file(xmlpath.c_str(), "  ", 1u, pugi::encoding_utf8);
}

void KleiAnim::XML::AnimBin2XML(Common::AnimationBase& binary, std::filesystem::path xmlpath)
{
	Binary::AnimationReader reader(binary);
	AnimBin2XML(reader, xmlpath);
}

void KleiAnim::XML::BuildBin2XML(std::filesystem::path binary, std::filesystem::path xmlpath)
{
	Binary::BuildReader bin(binary);
	BuildBin2XML(bin, xmlpath);
}

void KleiAnim::XML::BuildBin2XML(Common::BuildBase& binary, std::filesystem::path xmlpath)
{
	Binary::BuildReader bin(binary);
	BuildBin2XML(bin, xmlpath);
}

void KleiAnim::XML::BuildBin2XML(Binary::BuildReader& binary, std::filesystem::path xmlpath)
{
	pugi::xml_document doc;
	auto xroot = doc.append_child("Build");
	xroot.append_attribute("name").set_value(binary.name().c_str());
	std::string prevname;
	//symbol
	for (auto& sym : binary)
	{
		auto xsym = xroot.append_child("Symbol");
		std::string name = binary.de_hash(sym.name_hash);
		xsym.append_attribute("name").set_value(name.c_str());
		if (name == prevname)
		{
			KleiAnimLog::write() << L"�����ǰ�Ȩ��Ϣ��\n";
			std::cout << name << std::endl;
			system("pause");
		}
		for (auto& frame : sym.frames)
		{
			auto xframe = xsym.append_child("Frame");
			xframe.append_attribute("framenum").set_value(frame.frame_number);
			xframe.append_attribute("duration").set_value(frame.duration);
			//xframe.append_attribute("image").set_value(frame.) //build.bin��û�ж�Ӧ����Ϣ
			xframe.append_attribute("x").set_value(frame.x);
			xframe.append_attribute("y").set_value(frame.y);
			xframe.append_attribute("w").set_value(frame.w);
			xframe.append_attribute("h").set_value(frame.h);
		}
	}

	//atlas
	for (size_t atlas_i = 0; atlas_i < binary.atlas_count(); atlas_i++)
	{
		auto& atlas = binary.atlas(atlas_i);
		xroot.append_child("Atlas").append_attribute("name").set_value(atlas.name.c_str());
	}

	auto license = binary.license_hash();
	if (license != 0)
	{
		KleiAnimLog::write() << L"���֤��Ϣ��" << LicenseFromHash(license)
			<< L"\n����ȷ������������֤����ʹ��" << std::endl;
		system("pause");
	}

	doc.save_file(xmlpath.c_str(), "  ", 1u, pugi::encoding_utf8);
}


void build2bin(pugi::xml_document& doc, path&& outfile)
{
	using namespace KleiAnim;
	using KleiAnim::Common::hash;

	Binary::BuildWriter wb(outfile);
	

	wb.build_name = doc.child("Build").attribute("name").as_string();

	auto xsyms = doc.select_nodes("Build/Symbol");//xml symbols

	Common::SymbolNode sym{};
	for (auto& _xsym : xsyms)
	{
		auto xsym = _xsym.node();
		auto sym_name = xsym.attribute("name").as_string();

		unsigned int name_hash = hash(sym_name);
		sym.name_hash = name_hash;
		wb.add_hashstringpair(name_hash, sym_name);

		auto xframes = xsym.children("Frame");

		Common::BuildFrameNode frame{};
		for (auto& xframe : xframes)
		{
			frame.x = xframe.attribute("x").as_float();
			frame.y = xframe.attribute("y").as_float();
			frame.w = xframe.attribute("w").as_float();
			frame.h = xframe.attribute("h").as_float();

			frame.frame_number = xframe.attribute("framenum").as_uint();
			frame.duration = xframe.attribute("duration").as_uint();

			sym.frames.push_back(frame);
		}

		wb.add(sym);
	}

	wb.writefile();
}

void anim2bin(pugi::xml_document& doc, path&& outfile)
{
	using namespace KleiAnim;
	using KleiAnim::Common::hash;
	using std::string;
	using uint = unsigned int;

	Binary::AnimationWriter wb(outfile);
	Common::AnimationNode anim{};

	auto xanims = doc.select_nodes("Anims/anim");
	for (auto& _xanim : xanims)
	{
		pugi::xml_node xanim = _xanim.node();

		anim.name = xanim.attribute("name").as_string();
		string rootname = xanim.attribute("root").as_string();
		anim.rootsym_hash = hash(rootname);
		wb.add_hashstringpair(anim.rootsym_hash, rootname);

		
	}


	//throw std::exception(__FUNCTION__"\n----------NOT IMPLEMENTED----------");
}

void KleiAnim::XML::XML2Bin(std::filesystem::path xmlpath, std::filesystem::path outdir)
{
	using namespace std::string_literals;
	using pugi::xml_node;
	using pugi::xpath_node;

	pugi::xml_document doc;
	doc.load_file(xmlpath.c_str());

	auto first = doc.first_child();

	const char* name = first.name();

	if (name == "Anims"s)
	{
		anim2bin(doc, outdir / L"anim.bin");
		return;
	}

	if (name == "Build"s)
	{
		build2bin(doc, outdir / L"build.bin");
		return;
	}
	
	throw std::invalid_argument("������Ǹ��ٵ�xml�ĵ���·����" + outdir.string());
}

EXPORT_API void KleiAnim::XML::AnimBin2XML(const char8_t* in_path, const char8_t* out_path)
{
	AnimBin2XML(path(in_path), path(out_path));
}

EXPORT_API void KleiAnim::XML::BuildBin2XML(const char8_t* in_path, const char8_t* out_path)
{
	BuildBin2XML(path(in_path), path(out_path));
}

EXPORT_API void KleiAnim::XML::XML2Bin(const char8_t* in_path, const char8_t* out_path)
{
	XML2Bin(path(in_path), path(out_path));
}
