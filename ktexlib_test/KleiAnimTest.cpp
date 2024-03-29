/***************************
* Encoding: GB2312
***************************/
#include "pch.h"


#include "../klei_anim/KleiAnim/common/anim_common.hpp"
#include "../klei_anim/KleiAnim/Binary.hpp"
#include "../klei_anim/KleiAnim/common/exceptions.hpp"
#include "../klei_anim/KleiAnim/Xml.hpp"
#include "../klei_anim/KleiAnim/Vertex.hpp"

#include <filesystem>
#include <fstream>
#include <vector>
#include <thread>
#include <mutex>
#include <charconv>
#include <chrono>
#include <initializer_list>

#include "CppUnitTest.h"
#include <Windows.h>
#include <iostream>

static_assert(sizeof(unsigned long long) == 8);
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using std::ios;




template<> std::wstring
Microsoft::VisualStudio::CppUnitTestFramework::ToString <KleiAnim::Common::Element>
(const KleiAnim::Common::Element& elem)
{
	std::wostringstream _s;
	_s << L"Name Hash = " << elem.name_hash << '\n'
		<< L"frame = " << elem.frame << '\n'
		<< L"a b c d: " << elem.a << ' ' << elem.b << ' ' << elem.c << ' ' << elem.d << '\n'
		<< L"tx ty: " << elem.tx << ' ' << elem.ty
		<< L"Z index = " << elem.z;
	return _s.str();
}

template<> std::wstring 
Microsoft::VisualStudio::CppUnitTestFramework::ToString<KleiAnim::Common::Facing>
(const KleiAnim::Common::Facing& f1)
{
	using facing = KleiAnim::Common::Facing;
	facing f = f1;
	std::wostringstream _s;
	switch (f)
	{
	case facing::All:
		_s << L"all direction";
		break;
	case facing::All45:
		_s << L"all 45";
		break;
	case facing::All90:
		_s << L"all 90";
		break;
	}

	if (f == facing::Down) 
		_s << L"down";
	
	if (f == facing::DownLeft) 
		_s << L"down left";
	
	if (f == facing::DownRight) 
		_s << L"down right";
	
	if (f == facing::Left) 
		_s << L"left";
	
	if (f == facing::Right) 
		_s << L"right";
	
	if (f == facing::Up) 
		_s << L"up";
	
	if (f == facing::UpLeft) 
		_s << L"up left";
	
	if (f == facing::UpRight) 
		_s << L"up right";
	
	if (f == facing::Invalid) 
		_s << L"invalid direction(0x00ui8)";
	
	return _s.str();
}

template<> std::wstring 
Microsoft::VisualStudio::CppUnitTestFramework::ToString<::KleiAnim::Common::BuildFrame>
(const ::KleiAnim::Common::BuildFrame& elem)
{
	std::wostringstream o;
	o << L"Frame number = " << elem.frame_number << L'\n';
	o << L"Duration = " << elem.duration << L'\n';
	o << L"x,y,w,h = " << elem.x << L',' << elem.y << L',' << elem.w << L',' << elem.h << L'\n';
	o << L"alpha_index = " << elem.vertbox_startindex << L'\n' << L"alpha_count = " << elem.alpha_count << L'\n';
	return o.str();
}

template<> std::wstring
Microsoft::VisualStudio::CppUnitTestFramework::ToString<std::vector<::KleiAnim::Common::BuildFrame>>
(const std::vector<::KleiAnim::Common::BuildFrame>& elem)
{
	std::wostringstream o;
	for (auto& node : elem)
	{
		o << L"{\n" << ToString(node) << L"}\n";
	}
	return o.str();
}



namespace ktexlibtest
{
	namespace CompareUnion
	{
		union cmp_elemnode
		{
			unsigned long long a[5];
			::KleiAnim::Common::Element elem;
		};
	}

	namespace map
	{
		struct anim_bin_reader : public KleiAnim::Common::BinaryFileBase
		{
			std::vector<KleiAnim::Common::Animation> animations;
		};
	}

	TEST_CLASS(KleiAnimTest)
	{
	public:
		TEST_METHOD(SingleThreadElementRead)
		{
			using namespace KleiAnim::Common;
			const Element excepted[]
			{
				{ 1,1,10000,0.1f,0.2f,0.3f,0.4f,0.5f,0.6f,0.7f },
				{ 2,1,10000,0.1f,0.2f,0.3f,0.4f,0.5f,0.6f,0.7f },
				{ 3,1,10000,0.1f,0.2f,0.3f,0.4f,0.5f,0.6f,0.7f },
				{ 4,1,10000,0.1f,0.2f,0.3f,0.4f,0.5f,0.6f,0.7f },
				{ 5,1,10000,1.1f,1.2f,1.3f,1.4f,1.5f,1.6f,1.7f },
				{ 6,1,10000,1.1f,1.2f,1.3f,1.4f,1.5f,1.6f,1.7f },
				{ 7,1,10000,1.1f,1.2f,1.3f,1.4f,1.5f,1.6f,1.7f },
				{ 8,1,10000,1.1f,1.2f,1.3f,1.4f,1.5f,1.6f,1.7f },
				{ 9,1,10000,2.1f,2.2f,2.3f,2.4f,2.5f,2.6f,2.7f },
				{ 10,1,10000,2.1f,2.2f,2.3f,2.4f,2.5f,2.6f,2.7f },
				{ 11,1,10000,2.1f,2.2f,2.3f,2.4f,2.5f,2.6f,2.7f },
				{ 12,1,10000,2.1f,2.2f,2.3f,2.4f,2.5f,2.6f,2.7f },
				{ 13,1,10000,3.1f,3.2f,3.3f,3.4f,3.5f,3.6f,3.7f },
				{ 14,1,10000,3.1f,3.2f,3.3f,3.4f,3.5f,3.6f,3.7f },
				{ 15,1,10000,3.1f,3.2f,3.3f,3.4f,3.5f,3.6f,3.7f },
				{ 16,1,10000,3.1f,3.2f,3.3f,3.4f,3.5f,3.6f,3.7f },
				{ 17,1,10000,4.1f,4.2f,4.3f,4.4f,4.5f,4.6f,4.7f },
				{ 18,1,10000,4.1f,4.2f,4.3f,4.4f,4.5f,4.6f,4.7f },
				{ 19,1,10000,4.1f,4.2f,4.3f,4.4f,4.5f,4.6f,4.7f },
				{ 20,1,10000,4.1f,4.2f,4.3f,4.4f,4.5f,4.6f,4.7f },
			};
			
			std::ifstream file("./elem_mtread.fakebin", ios::binary | ios::in);

			std::vector<Element> actual = st_read_elem(file, 20);
			for (size_t i = 0; i < 20; i++)
			{
				Assert::IsTrue(excepted[i] == actual[i], (L"NE,i = " + ToString(i)).c_str());
			}
		}

		TEST_METHOD(MultiThreadElementRead)
		{
			using namespace ::KleiAnim::Common;
			//small:28 elem refs,28 frame, 0 event, 1 anim
			//anim #1: idle, facing: all, hash:20 00 00 EE 
			//frame count: 28,
			std::vector<Element> actual;
			actual.reserve(20);

			//generated by TestElemGen.exe
			Element excepted[]
			{
				{ 1,1,10000,0.1f,0.2f,0.3f,0.4f,0.5f,0.6f,0.7f },
				{ 2,1,10000,0.1f,0.2f,0.3f,0.4f,0.5f,0.6f,0.7f },
				{ 3,1,10000,0.1f,0.2f,0.3f,0.4f,0.5f,0.6f,0.7f },
				{ 4,1,10000,0.1f,0.2f,0.3f,0.4f,0.5f,0.6f,0.7f },
				{ 5,1,10000,1.1f,1.2f,1.3f,1.4f,1.5f,1.6f,1.7f },
				{ 6,1,10000,1.1f,1.2f,1.3f,1.4f,1.5f,1.6f,1.7f },
				{ 7,1,10000,1.1f,1.2f,1.3f,1.4f,1.5f,1.6f,1.7f },
				{ 8,1,10000,1.1f,1.2f,1.3f,1.4f,1.5f,1.6f,1.7f },
				{ 9,1,10000,2.1f,2.2f,2.3f,2.4f,2.5f,2.6f,2.7f },
				{ 10,1,10000,2.1f,2.2f,2.3f,2.4f,2.5f,2.6f,2.7f },
				{ 11,1,10000,2.1f,2.2f,2.3f,2.4f,2.5f,2.6f,2.7f },
				{ 12,1,10000,2.1f,2.2f,2.3f,2.4f,2.5f,2.6f,2.7f },
				{ 13,1,10000,3.1f,3.2f,3.3f,3.4f,3.5f,3.6f,3.7f },
				{ 14,1,10000,3.1f,3.2f,3.3f,3.4f,3.5f,3.6f,3.7f },
				{ 15,1,10000,3.1f,3.2f,3.3f,3.4f,3.5f,3.6f,3.7f },
				{ 16,1,10000,3.1f,3.2f,3.3f,3.4f,3.5f,3.6f,3.7f },
				{ 17,1,10000,4.1f,4.2f,4.3f,4.4f,4.5f,4.6f,4.7f },
				{ 18,1,10000,4.1f,4.2f,4.3f,4.4f,4.5f,4.6f,4.7f },
				{ 19,1,10000,4.1f,4.2f,4.3f,4.4f,4.5f,4.6f,4.7f },
				{ 20,1,10000,4.1f,4.2f,4.3f,4.4f,4.5f,4.6f,4.7f },
			};

			//std::ifstream fakebin(, std::ios::binary | std::ios::in);
			mt_read_elem(20, actual, "./elem_mtread.fakebin", 0);

			Assert::AreEqual(size_t(20),actual.size(),L"Size not match");
		}

		TEST_METHOD(BinAnimRead)
		{
			using namespace KleiAnim;
			using std::filesystem::path;

			Assert::ExpectException<std::system_error>([] {Binary::AnimationReader(",\\anim_test.bin"); });

			try
			{
				auto before_create = std::chrono::high_resolution_clock::now().time_since_epoch().count();
				Binary::AnimationReader test_reader(path(L".\\anim_test.bin"));
				auto after_create = std::chrono::high_resolution_clock::now().time_since_epoch().count();
				Microsoft::VisualStudio::CppUnitTestFramework::Logger::WriteMessage((L"构造用时 " + ToString(after_create - before_create) + L"ns").c_str());
				//auto test_view = reinterpret_cast<map::anim_bin_reader*>(&test_reader);
				Assert::AreEqual<size_t>(1, test_reader.anim_count(), L"anim size != 1");
				const auto& anim0 = test_reader[0];

				Assert::AreEqual(anim0.facing, Common::Facing::All, L"facing != facing::all");

				Assert::AreEqual(anim0.name.c_str(), "idle", false, L"anim name != idle");

				Assert::AreEqual(size_t(40), anim0.frames.size(), L"frame size not match");

				for (const Common::AnimationFrame& frame : anim0.frames)
				{
					Assert::AreEqual(-142.293198f, frame.x, L"x not match");
					Assert::AreEqual(-18.273909f, frame.y, L"y not match");
					Assert::AreEqual(164.0f, frame.w, L"w not match");
					Assert::AreEqual(177.f, frame.h, L"h not match");

					Assert::AreEqual(size_t(0), frame.events.size(), L"has event?!");
					Assert::AreEqual(size_t(1), frame.elements.size(), L"elements size != 1");

					const auto& elem = frame.elements[0];
					Assert::AreEqual(0xA2984AF8U, elem.name_hash, L"name hash != 0xA2984AF8");
					Assert::AreEqual(0x7B212688U, elem.layer_hash, L"layer hash != 0x7B212688");
					Assert::AreEqual(0U, elem.frame, L"frame != 0");
					Assert::AreEqual(-5.0f, elem.z, L"Z != -5");

					Assert::AreEqual(0.0f, elem.a, L"u1 != 0.0");
					Assert::AreEqual(2.122715f, elem.b, L"u2 != 2.122715");
					Assert::AreEqual(-2.122715f, elem.c, L"v2 != 2.991668");
					Assert::AreEqual(0.0f, elem.d, L"v1 != 0.0");

					Assert::AreEqual(2.991668f, elem.tx, L"tx != 2.991668");
					Assert::AreEqual(-2.869065f, elem.ty, L"ty != -2.869065");
				}
			}
			catch (const std::invalid_argument& e)
			{
				wchar_t w_msg[100]= L"invalid_argument异常:";
				MultiByteToWideChar(936U, 0, e.what(), -1, w_msg + 20, 80);
				Assert::Fail(w_msg);
			}
			catch (const KleiAnim::Exception::invalid_file& e)
			{
				wchar_t w_msg[100]=L"invalid_file异常:";
				MultiByteToWideChar(936U, 0, e.what(), -1, w_msg + 16, 84);
				Assert::Fail(w_msg);
			}
		}

		TEST_METHOD(BinBuildRead)
		{
			using namespace KleiAnim;
			Binary::BuildReader test_reader("./build_test.bin");

			Assert::AreEqual("nato556", test_reader.name().c_str(), L"name");
			Assert::AreEqual(1u, test_reader.atlas_count(), L"atlas size != 1");
			Assert::AreEqual(1u, test_reader.symbol_count(), L"symbol count != 1");
			Assert::AreEqual(6u, test_reader.vertex_count(), L"vertex count != 6");

			Assert::AreEqual("atlas-0.tex", test_reader.atlas(0).name.c_str(), "atlas-0 name != atlas-0.tex");

			const auto& sym0 = test_reader[0];
			Assert::AreEqual(size_t(1), sym0.frames.size(), L"frame size != 1");

			auto& frame0 = sym0.frames[0];

			Assert::AreEqual(0u, frame0.frame_number, L"frame number != 0");
			Assert::AreEqual(1u, frame0.duration, L"duration != 1");

			Assert::AreEqual(-0.190720f, frame0.x, L"x != -0.190720");
			Assert::AreEqual(-2.152384f, frame0.y, L"y != -2.152384");
			Assert::AreEqual(64.0f, frame0.w, L"w != 64.0");
			Assert::AreEqual(64.0f, frame0.h, L"h != 64.0");

			Assert::AreEqual(6u, frame0.alpha_count, L"alpha count != 6");
			Assert::AreEqual(0u, frame0.vertbox_startindex, L"alpha index != 0");

			//todo:vertices assert
		}

		TEST_METHOD(BinBuildWrite)
		{
			using namespace KleiAnim::Binary;
			using namespace KleiAnim::Common;

			BuildBase TestBase(2, 3, "TestBuild",
				{
					Atlas{"atlas-0"}
				},
				{
					Symbol
					{
						0U,
						{
							BuildFrame(),
							BuildFrame{1,2,0,0,24.0f,24.0f,0,0}
						}
					},
					Symbol
					{
						1U,
						{
							BuildFrame{2,2,0,0,24.0f,24.0f,0,0}
						}
					}
				},
				{}
			);

			BuildWriter test_write("./test-write-build.bin",TestBase);
			test_write.writefile();

			BuildReader read_out("./test-write-build.bin");
			

			{
				using namespace std::string_literals;
				Assert::AreEqual("atlas-0"s, read_out.atlas(0).name);

				auto& sym0 = read_out[0];
				auto& sym1 = read_out[1];

				Assert::AreEqual(0U, sym0.name_hash);
				Assert::AreEqual(1U, sym1.name_hash);

				Assert::AreEqual(
					{
							BuildFrame(),
							BuildFrame{1,2,0,0,24.0f,24.0f,0,0}
					}, sym0.frames
				);
				Assert::AreEqual(
					{
							BuildFrame{2,2,0,0,24.0f,24.0f,0,0}
					}, sym1.frames
				);
			}

		}

		TEST_METHOD(BinAnimWrite)
		{
			using namespace KleiAnim::Binary;
			using namespace KleiAnim::Common;
			using namespace std::string_literals;
			using std::make_pair;

			AnimationBase base
			(
				{
					Animation
					{
						"test_anim"s,Facing::All,20,20.0f,
						{
							AnimationFrame
							{
								0.0f,1.0f,2.0f,3.0f,{Event(30)},{Element{10,0,0}}
							}
						}
					}
				}, { make_pair(20,"rootsymbol"s),make_pair(30,"event0"s),make_pair(10,"elem0"s),make_pair(0,"layer0"s) }
			);

			AnimationWriter testw(".\\test-write-anim.bin",base);
			testw.writefile();

			AnimationReader testr(".\\test-write-anim.bin");

			{
				auto& anim0 = *testr.begin();
				Assert::AreEqual(Facing::All, anim0.facing);
				Assert::AreEqual(20u, anim0.rootsym_hash);
				Assert::AreEqual(20.0f, anim0.frame_rate);

				{
					auto& f = *anim0.frames.begin();
					Assert::AreEqual(0.0f, f.x);;
					Assert::AreEqual(1.0f, f.y);
					Assert::AreEqual(2.0f, f.w);
					Assert::AreEqual(3.0f, f.h);

					Assert::AreEqual(30u, f.events[0].name_hash);

					{
						auto& elem = f.elements[0];
						Assert::AreEqual(10u, elem.name_hash);
						Assert::AreEqual(0u, elem.frame);
						Assert::AreEqual(0u, elem.layer_hash);
						//剩下的偷懒不测试了
					}
					
				}
			}
		}

		TEST_METHOD(AnimToXML)
		{
			KleiAnim::XML::AnimBin2XML("anim_test.bin","animation.test.xml");
		}

		TEST_METHOD(BuildToXML)
		{
			KleiAnim::XML::BuildBin2XML("build_test.bin", "build.test.xml");
		}

		TEST_METHOD(VerticesTest)
		{
			using namespace KleiAnim::Vertex;
			KleiAnim::Binary::BuildReader bin{ L".\\build_test.bin" };
			auto* g = MakeGroup(bin.vertices(0));
			auto i = FromGroup(*g);
			Assert::IsTrue(VaildateGroup(*g));
		}
	private:

		void mt_read_elem(const unsigned int count,
			std::vector<::KleiAnim::Common::Element>& out,
			const std::filesystem::path& path,
			const size_t pos)
		{
			using std::thread;
			using std::ios;
			std::mutex mtx_fin;
			std::mutex mtx_out;
			static const unsigned int t_limit = thread::hardware_concurrency();
			unsigned int finished = 0;
			for (unsigned int t_index = 0; t_index < t_limit; t_index++)
			//unsigned int t_index = 0;
			{
				thread(
					[&out, count, pos, path, &finished, &mtx_fin, &mtx_out] 
					(unsigned int cur_tid)
					{
						thread_local std::ifstream file(path, ios::binary | ios::in);
						thread_local size_t begin_pos = pos + (40Ui64 * count * cur_tid) / t_limit;
						::KleiAnim::Common::Element read_out{ 0,0,0,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f };

						file.seekg(begin_pos);
						//读取
						for (size_t i = 0; i < count / t_limit; i++)
						{
							file.read((char*)(&read_out), 40);
							mtx_out.lock();
							out.push_back(read_out);
							mtx_out.unlock();
						}

						file.close();

						mtx_fin.lock();
						finished++;
						mtx_fin.unlock();
					},
					t_index).detach();
			}

			while (finished != t_limit)
			{
				using namespace std::chrono;
				std::this_thread::sleep_for(1ms);
			}
		}

		std::vector<KleiAnim::Common::Element> st_read_elem(std::ifstream& f, unsigned int count)
		{
			using KleiAnim::Common::Element;
			std::vector<Element> ret;
			ret.resize(count);
			f.read((char*)ret.data(), static_cast<size_t>(count) * 40Ui64);
			return ret;
		}
	};
}