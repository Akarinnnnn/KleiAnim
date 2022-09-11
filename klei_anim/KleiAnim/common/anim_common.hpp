/***************************
* Encoding: GB2312
* KleiAnim的共用部分，包含结构体定义及共用函数
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
//确保目标机器是小端机
//static_assert(std::endian::native == std::endian::little);


namespace KleiAnim
{
	namespace Common
	{

		/// <summary>
		/// 默认输出到std::wcout
		/// </summary>
		class WideCharLog
		{
		public:
			WideCharLog();
			void EXPORT_API AddFile(std::filesystem::path path);			
#pragma region 内部函数
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
		///图片朝向，Flag
		///</summary>
		enum class Facing : unsigned char
		{
			/// <summary>右</summary>
			Right = 1,

			/// <summary>上</summary>
			Up = 1 << 1,

			/// <summary>左</summary>
			Left = 1 << 2,

			/// <summary>下</summary>
			Down = 1 << 3,

			/// <summary>右上</summary>
			UpRight = 1 << 4,

			/// <summary>左上</summary>
			UpLeft = 1 << 5,

			/// <summary>右下</summary>
			DownRight = 1 << 6,

			/// <summary>左下</summary>
			DownLeft = 1 << 7,

			/// <summary>上下左右</summary>
			All90 = Up | Left | Right | Down,

			/// <summary>四个角</summary>
			All45 = UpRight | UpLeft | DownLeft | DownRight,

			/// <summary>所有朝向</summary>
			All = 0xFFUi8,

			/// <summary>这不可能！！</summary>
			Invalid = 0
		};

		/// <summary>
		/// 动画用bin文件的共用部分
		/// </summary>
		struct EXPORT_API BinaryFileBase
		{
			unsigned int cc4 = 0;
			signed int version = 0;

			BinaryFileBase() = default;

			BinaryFileBase(const std::map<unsigned int, std::string> & string_table) :str_table(string_table)
			{

			}

			/// <summary>哈希化字符串表</summary>
			std::map<unsigned int, std::string> str_table;
		};

		/// <summary>
		/// OpenGL顶点
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
		/// Build中的帧节点
		/// </summary>
		struct BuildFrame
		{
			/// <summary>帧号</summary>
			unsigned int frame_number = 0;

			/// <summary>持续时间</summary>
			unsigned int duration = 1;

			/// <summary>碰撞检测 xywh参数</summary>
			float x = 0, y = 0, w = 0, h = 0;


			unsigned int vertbox_startindex = 0;
			std::vector<Common::AlphaVertex> vertices;
		};

		/// <summary>
		/// 符号节点
		/// </summary>
		struct Symbol
		{
			unsigned int name_hash = 0;
			std::vector<BuildFrame> frames;
		};

		/// <summary>
		/// 事件节点
		/// </summary>
		struct EXPORT_API Event
		{
			/// <summary>名称的哈希</summary>
			unsigned int name_hash;

			operator unsigned int()
			{
				return name_hash;
			}
			
			Event() : name_hash(0) {}
			Event(unsigned int h):name_hash(h) {}
		};

		/// <summary>
		/// 元素节点
		/// </summary>
		struct Element
		{
			/// <summary>哈希</summary>
			unsigned int name_hash = 0;
			unsigned int frame = 0;
			unsigned int layer_hash = 0;

			float a = 0, b = 0, c = 0, d = 0;
			float tx = 0, ty = 0;
			float z = 1.0f;
		};

		/// <summary>
		/// Animation中的帧节点
		/// </summary>
		struct AnimationFrame
		{
			float x = 0, y = 0, w = 0, h = 0;

			std::vector<Event> events;//event count + event
			std::vector<Element> elements;//elem count + elems
		};

		/// <summary>
		/// 动画节点
		/// </summary>
		struct Animation
		{
			std::string name;

			/// <summary>朝向</summary>
			Facing facing = Facing::All;

			/// <summary>根符号哈希</summary>
			unsigned int rootsym_hash = 0;

			/// <summary>帧率</summary>
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
		/// 计算字符串的哈希值
		/// </summary>
		/// <param name="s">将计算哈希的字符串</param>
		/// <returns>哈希值</returns>
		/// <created>Fa鸽,2019/7/22</created>
		/// <changed>Fa鸽,2019/7/22</changed>
		unsigned int EXPORT_API hash(std::string&& s);

		/// <summary>
		/// 计算字符串的哈希值
		/// </summary>
		/// <param name="s">将计算哈希的字符串</param>
		/// <returns>哈希值</returns>
		/// <created>Fa鸽,2019/7/22</created>
		/// <changed>Fa鸽,2019/7/22</changed>
		unsigned int EXPORT_API hash(std::string& s);


		/// <summary>
		/// 读取bin文件中的一条字符串（非哈希化字符串）
		/// </summary>
		/// <param name="f">流，必须行进到字符串所在的位置</param>
		/// <returns>读取出的字符串</returns>
		/// <created>Fa鸽,2019/7/24</created>
		/// <changed>Fa鸽,2019/7/24</changed>
		std::string read_str(std::istream& f);

		/// <summary>
		/// 读取哈希化的字符串表
		/// </summary>
		/// <param name="f">文件流，必须行进到字符串表所在位置</param>
		/// <returns>字符串表</returns>
		/// <created>Fa鸽,2019/7/24</created>
		/// <changed>Fa鸽,2019/7/24</changed>
		std::map<unsigned int, std::string> read_strhashtable(std::ifstream& f);

		/// <summary>
		/// 获取元素节点的字符串(<see cref="std::wstring"/>)表示，仅支持wstring
		/// </summary>
		/// <param name="elem">元素节点</param>
		/// <example><c>wstring elem_str = std::move(ToString(elem));</c></example>
		/// <created>Fa鸽,2019/7/27</created>
		/// <changed>Fa鸽,2019/7/27</changed>
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
		/// 比较两个ElementNode是否相等
		/// </summary>
		/// <param name="l">1</param>
		/// <param name="r">2</param>
		/// <returns>相等返回true，不相等返回false</returns>
		/// <created>Fa鸽,2019/7/27</created>
		/// <changed>Fa鸽,2019/7/27</changed>
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
			///<summary>合规文件的CC4，ANIM</summary>
			static constexpr unsigned int valid_cc4 = 0x4D494E41;
			///<summary>当前版本</summary>
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
			///<summary>合规文件的CC4，BILD</summary>
			static constexpr unsigned int valid_cc4 = 0x444C4942;
			///<summary>当前版本</summary>
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
