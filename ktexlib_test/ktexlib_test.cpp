/***************************
* Encoding: GB2312
***************************/
#include "pch.h"
#include "CppUnitTest.h"
#include <filesystem>
#include <fstream>
#include <charconv>
#include "thirdparty/pugixml.hpp"
#include "../ktexlib_dymanic/KtexAtlas.h"

using namespace ktexlib;
using std::ifstream;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ktexlibtest
{
	namespace map
	{
		struct map_atlas
		{
			std::vector<Atlas::b_box> bboxes;
			std::filesystem::path path;
		};
	}

	TEST_CLASS(KTEXLib)
	{
	public:
		
		TEST_METHOD(Atlas)
		{
			ktexlib::KTEXFileOperation::mipmaps img
			{
				KTEXFileOperation::mipmapv2{
					64,64,0,0,nullptr
				},
				KTEXFileOperation::mipmapv2{
					17,16,1,0,nullptr
				},
				KTEXFileOperation::mipmapv2{
					63,65,2,0,nullptr
				}
			};
			Atlas::atlas xmlop(L".\\atlas_test.xml", img);
			//map::map_atlas& view_xmlop = *reinterpret_cast<map::map_atlas*>(&xmlop);//感谢百度，感谢CSDN热心网友
			xmlop.xmlgen();

			pugi::xml_document actual;
			pugi::xml_document excepted;
			actual.load_file(L".\\atlas_test.xml", pugi::parse_minimal);
			auto it = actual.first_element_by_path(L"Atlas/Elements").children(L"Element").begin();
			//#1
			{
				Assert::AreEqual(0.0078125, 
					wcstod((it->attribute(L"u1").value()), nullptr), 
					0.0000040, L"1->u1");
				Assert::AreEqual(0.9921875,
					wcstod((it->attribute(L"u2").value()), nullptr),
					0.0000040, L"1->u2");
				Assert::AreEqual(0.0078125,
					wcstod((it->attribute(L"v1").value()), nullptr),
					0.0000040, L"1->v1");
				Assert::AreEqual(0.9921875,
					wcstod((it->attribute(L"v2").value()), nullptr),
					0.0000040, L"1->v2");
				it++;
			}

			//#2
			{
				Assert::AreEqual(0.03125,
					wcstod((it->attribute(L"u1").value()), nullptr),
					0.0000040, L"2->u1");
				Assert::AreEqual(0.515625,
					wcstod((it->attribute(L"u2").value()), nullptr),
					0.0000040, L"2->u2");
				Assert::AreEqual(0.03125,
					wcstod((it->attribute(L"v1").value()), nullptr),
					0.0000040, L"2->v1");
				Assert::AreEqual(0.96875,
					wcstod((it->attribute(L"v2").value()), nullptr),
					0.0000040, L"2->v2");
				it++;
			}
			//#3
			{
				Assert::AreEqual(0.0078125,
					wcstod((it->attribute(L"u1").value()), nullptr),
					0.0000040, L"3->u1");
				Assert::AreEqual(0.9765625,
					wcstod((it->attribute(L"u2").value()), nullptr),
					0.0000040, L"3->u2");
				Assert::AreEqual(0.00390625,
					wcstod((it->attribute(L"v1").value()), nullptr),
					0.00000040, L"3->v1");
				Assert::AreEqual(0.050390625,
					wcstod((it->attribute(L"v2").value()), nullptr),
					0.0000040, L"3->v2");
				it++;
			}
		}
	};
}
