/***************************
* Encoding: GB2312
* KleiAnim�Ĺ��ò��֣������ṹ�嶨�弰���ú���
***************************/
#pragma once

#include <string>
#include <list>
#include <vector>
#include <map>
#include "export.h"
#include <iostream>
#include <fstream>
#include <sstream>
//#include <endian>
//#include <type_traits>
//assert size
static_assert(sizeof(unsigned int) == 4,"unsigned int must be 4 bytes long");
static_assert(sizeof(unsigned short) == 2,"unsigned short must be 2 bytes long");
static_assert(sizeof(float) == 4,"float must be 4 bytes long");
//static_assert(sizeof(KleiAnim::Common::Element) == 40);

//assert align
static_assert(alignof(unsigned short) == 2,"unsigned short must be 2-byte aligned");
static_assert(alignof(float) == 4,"float must be 4-byte aligned");
static_assert(alignof(unsigned int) == 4,"unsigned int must be 4-byte aligned");


//assert LE(C++ 20)
//ȷ��Ŀ�������С�˻�
//static_assert(std::endian::native == std::endian::little);


namespace KleiAnim
{
	namespace Common
	{

		/// <summary>
		/// Ĭ�������std::wcout
		/// </summary>
		class WideCharLog
		{
		public:
			WideCharLog();
			void EXPORT_API AddFile(std::filesystem::path path);			
#pragma region �ڲ�����
#ifdef KLEIANIM_EXPORTS
			static WideCharLog& write();
			WideCharLog& operator<<(const wchar_t* str)
			{
				std::wcout << str;
				for (auto& stream : streams)
					stream << str;
				return *this;
			}
			WideCharLog& operator<<(std::wstring&& str)
			{
				std::wcout << str;
				for (auto& stream : streams)
					stream << str;
				return *this;
			}

			WideCharLog& operator<<(std::wostream& (__cdecl* func)(std::wostream&))
			{
				std::wcout << func;
				for (auto& stream : streams)
					stream << func;
				return *this;
			}
			WideCharLog& operator<<(uint64_t num)
			{
				std::wcout << num;
				for (auto& stream : this->streams)
					stream << num;

				return *this;
			}
			WideCharLog& operator<<(uint32_t num)
			{
				std::wcout << num;
				for (auto& stream : this->streams)
					stream << num;

				return *this;
			}

			WideCharLog& operator<<(int64_t num)
			{
				std::wcout << num;
				for (auto& stream : this->streams)
					stream << num;

				return *this;
			}
			WideCharLog& operator<<(int32_t num)
			{
				std::wcout << num;
				for (auto& stream : this->streams)
					stream << num;

				return *this;
			}
			WideCharLog& operator<<(float num)
			{
				std::wcout << num;
				for (auto& stream : this->streams)
					stream << num;

				return *this;
			}
			WideCharLog& operator<<(double num)
			{
				std::wcout << num;
				for (auto& stream : this->streams)
					stream << num;

				return *this;
			}
			WideCharLog& operator<<(char num)
			{
				std::wcout << num;
				for (auto& stream : this->streams)
					stream << num;

				return *this;
			}

			template<typename T>
			WideCharLog& operator<< (T& ref)
			{
				static_assert(!std::is_enum<T>() && !std::is_arithmetic<T>());
				std::wcout << ref;
				for (auto& stream : this->streams)
					stream << ref;
				return *this;
			}
#pragma endregion
#endif // KLEIANIM_EXPORTS
		private:
			std::list<std::wofstream> streams;

		};

		///<summary>
		///ͼƬ����Flag
		///</summary>
		enum class Facing : unsigned char
		{
			/// <summary>��</summary>
			Right = 1,

			/// <summary>��</summary>
			Up = 1 << 1,

			/// <summary>��</summary>
			Left = 1 << 2,

			/// <summary>��</summary>
			Down = 1 << 3,

			/// <summary>����</summary>
			UpRight = 1 << 4,

			/// <summary>����</summary>
			UpLeft = 1 << 5,

			/// <summary>����</summary>
			DownRight = 1 << 6,

			/// <summary>����</summary>
			DownLeft = 1 << 7,

			/// <summary>��������</summary>
			All90 = Up | Left | Right | Down,

			/// <summary>�ĸ���</summary>
			All45 = UpRight | UpLeft | DownLeft | DownRight,

			/// <summary>���г���</summary>
			All = 0xFFUi8,

			/// <summary>�ⲻ���ܣ���</summary>
			Invalid = 0
		};

		/// <summary>
		/// ������bin�ļ��Ĺ��ò���
		/// </summary>
		struct EXPORT_API BinaryFileBase
		{
			unsigned int cc4 = 0;
			signed int version = 0;

			BinaryFileBase() = default;

			BinaryFileBase(const std::map<unsigned int, std::string> & string_table) :str_table(string_table)
			{

			}

			/// <summary>��ϣ���ַ�����</summary>
			std::map<unsigned int, std::string> str_table;
		};

		/// <summary>
		/// OpenGL����
		/// </summary>
		struct AlphaVertex
		{
			float a = 0, b = 0, c = 0, d = 0, e = 0, f = 0;
		};
		union VertexStyle
		{
			AlphaVertex avn;
			float arr[6];
		};
		/// <summary>
		/// Build�е�֡�ڵ�
		/// </summary>
		struct BuildFrame
		{
			/// <summary>֡��</summary>
			unsigned int frame_number = 0;

			/// <summary>����ʱ��</summary>
			unsigned int duration = 1;

			/// <summary>��ײ��� xywh����</summary>
			float x = 0, y = 0, w = 0, h = 0;


			unsigned int vertbox_startindex = 0;
			std::vector<Common::AlphaVertex> vertices;
		};

		/// <summary>
		/// ���Žڵ�
		/// </summary>
		struct Symbol
		{
			unsigned int name_hash = 0;
			std::vector<BuildFrame> frames;
		};

		/// <summary>
		/// �¼��ڵ�
		/// </summary>
		struct EXPORT_API Event
		{
			/// <summary>���ƵĹ�ϣ</summary>
			unsigned int name_hash;

			operator unsigned int()
			{
				return name_hash;
			}
			
			Event() : name_hash(0) {}
			Event(unsigned int h):name_hash(h) {}
		};

		/// <summary>
		/// Ԫ�ؽڵ�
		/// </summary>
		struct Element
		{
			/// <summary>��ϣ</summary>
			unsigned int name_hash = 0;
			unsigned int frame = 0;
			unsigned int layer_hash = 0;

			float a = 0, b = 0, c = 0, d = 0;
			float tx = 0, ty = 0;
			float z = 1.0f;
		};

		/// <summary>
		/// Animation�е�֡�ڵ�
		/// </summary>
		struct AnimationFrame
		{
			float x = 0, y = 0, w = 0, h = 0;

			std::vector<Event> events;//event count + event
			std::vector<Element> elements;//elem count + elems
		};

		/// <summary>
		/// �����ڵ�
		/// </summary>
		struct Animation
		{
			std::string name;

			/// <summary>����</summary>
			Facing facing = Facing::All;

			/// <summary>�����Ź�ϣ</summary>
			unsigned int rootsym_hash = 0;

			/// <summary>֡��</summary>
			float frame_rate = 10.0f;

			std::vector<AnimationFrame> frames;
		};

		/// <summary>
		/// Atlas
		/// </summary>
		struct Atlas
		{
			std::string name;
		};

		/// <summary>
		/// �����ַ����Ĺ�ϣֵ
		/// </summary>
		/// <param name="s">�������ϣ���ַ���</param>
		/// <returns>��ϣֵ</returns>
		/// <created>Fa��,2019/7/22</created>
		/// <changed>Fa��,2019/7/22</changed>
		unsigned int EXPORT_API hash(std::string&& s);

		/// <summary>
		/// �����ַ����Ĺ�ϣֵ
		/// </summary>
		/// <param name="s">�������ϣ���ַ���</param>
		/// <returns>��ϣֵ</returns>
		/// <created>Fa��,2019/7/22</created>
		/// <changed>Fa��,2019/7/22</changed>
		unsigned int EXPORT_API hash(std::string& s);


		/// <summary>
		/// ��ȡbin�ļ��е�һ���ַ������ǹ�ϣ���ַ�����
		/// </summary>
		/// <param name="f">���������н����ַ������ڵ�λ��</param>
		/// <returns>��ȡ�����ַ���</returns>
		/// <created>Fa��,2019/7/24</created>
		/// <changed>Fa��,2019/7/24</changed>
		std::string read_str(std::istream& f);

		/// <summary>
		/// ��ȡ��ϣ�����ַ�����
		/// </summary>
		/// <param name="f">�ļ����������н����ַ���������λ��</param>
		/// <returns>�ַ�����</returns>
		/// <created>Fa��,2019/7/24</created>
		/// <changed>Fa��,2019/7/24</changed>
		std::map<unsigned int, std::string> read_strhashtable(std::ifstream& f);

		/// <summary>
		/// ��ȡԪ�ؽڵ���ַ���(<see cref="std::wstring"/>)��ʾ����֧��wstring
		/// </summary>
		/// <param name="elem">Ԫ�ؽڵ�</param>
		/// <example><c>wstring elem_str = std::move(ToString(elem));</c></example>
		/// <created>Fa��,2019/7/27</created>
		/// <changed>Fa��,2019/7/27</changed>
		std::wstring EXPORT_API ToString(const Element& elem);

		inline std::wstring ToString(const BuildFrame& elem)
		{
			std::wostringstream o;
			o << L"Frame number = " << elem.frame_number << L'\n';
			o << L"Duration = " << elem.duration << L'\n';
			o << L"x,y,w,h = " << elem.x << L',' << elem.y << L',' << elem.w << L',' << elem.h << L'\n';
			o << L"alpha_index = " << elem.vertbox_startindex << L'\n' << L"vertcount = " << elem.vertices.size() << L'\n';
			return o.str();
		}

		template <typename T>
		inline bool operator==(const T& lhs, const T& rhs)
		{
			static_assert(sizeof(T) % sizeof(size_t) == 0);

			constexpr size_t size = sizeof(T) / sizeof(size_t);
			auto* l = reinterpret_cast<const size_t*>(&lhs);
			auto* r = reinterpret_cast<const size_t*>(&rhs);

			for (size_t i = 0; i < size; i++)
				if (*(l + i) != *(r + i)) return false;
			return true;
		}

		

		/// <summary>
		/// �Ƚ�����ElementNode�Ƿ����
		/// </summary>
		/// <param name="l">1</param>
		/// <param name="r">2</param>
		/// <returns>��ȷ���true������ȷ���false</returns>
		/// <created>Fa��,2019/7/27</created>
		/// <changed>Fa��,2019/7/27</changed>
		bool EXPORT_API operator==(const Element& l, const Element& r);

		class EXPORT_API AnimationBase : protected virtual Common::BinaryFileBase
		{
			//friend void ::KleiAnim::AnimBin2XML(Common::AnimationBase& binary, std::filesystem::path xmlpath);


		public:
			AnimationBase() = default;

			AnimationBase(const std::vector<Common::Animation>& animations,
				const std::map<unsigned int, std::string>& string_table) :animations(animations), BinaryFileBase(string_table)
			{

			}

		protected:
			///<summary>�Ϲ��ļ���CC4��ANIM</summary>
			static constexpr unsigned int valid_cc4 = 0x4D494E41;
			///<summary>��ǰ�汾</summary>
			static constexpr unsigned short cur_version = 0x0004;

			/// <summary>animation</summary>
			std::vector<Common::Animation> animations;
		};

		class EXPORT_API BuildBase : protected virtual Common::BinaryFileBase
		{
			//friend void ::KleiAnim::BuildBin2XML(Common::BuildBase& binary, std::filesystem::path xmlpath);

		public:
			BuildBase() = default;

			BuildBase(unsigned int symbol_count, unsigned int frame_count,
				const std::string& build_name, const std::vector<Common::Atlas>& atlases,
				const std::vector<Common::Symbol>& symbols,
				const std::vector<Common::AlphaVertex>& vertices) :
				symbol_count(symbol_count),
				frame_count(frame_count),
				build_name(build_name),
				materials(atlases),
				symbols(symbols)
			{}

			std::wstring ToString();

		protected:
			///<summary>�Ϲ��ļ���CC4��BILD</summary>
			static constexpr unsigned int valid_cc4 = 0x444C4942;
			///<summary>��ǰ�汾</summary>
			static constexpr unsigned short cur_version = 0x0006;

			unsigned int symbol_count = 0;
			unsigned int frame_count = 0;
			// int event_count = 0;

			std::string build_name;

			std::vector<Common::Atlas> materials;
			std::vector<Common::Symbol> symbols;
		};

		constexpr uint32_t common_license_hash[3] = {
			1495471998ui32,	//SPDX-License-Identifier: CC-BY-NC-4.0
			1850363105ui32,	//SPDX-License-Identifier: MIT
			377050555ui32,	//SPDX-License-Identifier: GPL-3.0-or-later
		};
	}
}

using KleiAnimLog = KleiAnim::Common::WideCharLog;
