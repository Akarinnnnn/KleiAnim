/***************************
* Encoding: GB2312
* KleiAnim的共用部分，包含结构体定义及共用函数
***************************/
#pragma once

#include <string>
#include <vector>
//#include <typeinfo>
#include <fstream>
#include <map>
#include "export.h"
#include <sstream>

#define KleiAnimLog KleiAnim::Common::WideCharLog



//#include <type_traits>
//assert size
static_assert(sizeof(unsigned int) == 4,"unsigned int must be 4 bytes long");
static_assert(sizeof(unsigned short) == 2,"unsigned short must be 2 bytes long");
static_assert(sizeof(float) == 4,"float must be 4 bytes long");
//static_assert(sizeof(KleiAnim::Common::ElementNode) == 40);

//assert align
static_assert(alignof(unsigned short) == 2,"unsigned short must be 2-byte aligned");
static_assert(alignof(float) == 4,"float must be 4-byte aligned");
static_assert(alignof(unsigned int) == 4,"unsigned int must be 4-byte aligned");


//assert LE(C++ 20)
//确保目标机器是小端机
//static_assert(std::endian::native == std::endian::little)


namespace KleiAnim
{
	namespace Common
	{
		/// <summary>
		/// 若要改变KleiAnim的日志输出，只需在调用KleiAnim之前构造一个实例
		/// </summary>
		class CharLog
		{
		public:
			EXPORT_API CharLog(std::ostream& output);
			static std::ostream& write();
		private:
			inline static CharLog* in_use = nullptr;
			std::ostream* stream;
		};

		/// <summary>
		/// 若要改变KleiAnim的日志输出，只需在调用KleiAnim之前构造一个实例
		/// </summary>
		class WideCharLog
		{
		public:
			EXPORT_API WideCharLog(std::wostream& output);
			static std::wostream& write();
		private:
			inline static WideCharLog* in_use = nullptr;
			std::wostream* stream;
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
		struct AlphaVertexNode
		{
			float x = 0, y = 0, z = 0, u = 0, v = 0, w = 0;
		};

		/// <summary>
		/// Build中的帧节点
		/// </summary>
		struct BuildFrameNode
		{
			/// <summary>帧号</summary>
			unsigned int frame_number = 0;

			/// <summary>持续时间</summary>
			unsigned int duration = 1;

			/// <summary>bbox xywh参数</summary>
			float x = 0, y = 0, w = 0, h = 0;


			unsigned int alpha_index = 0;
			unsigned int alpha_count = 0;
		};

		/// <summary>
		/// 符号节点
		/// </summary>
		struct SymbolNode
		{
			unsigned int name_hash = 0;
			std::vector<BuildFrameNode> frames;
		};

		/// <summary>
		/// 事件节点
		/// </summary>
		struct EXPORT_API EventNode
		{
			/// <summary>名称的哈希</summary>
			unsigned int name_hash;

			operator unsigned int()
			{
				return name_hash;
			}
			
			EventNode() : name_hash(0) {}
			EventNode(unsigned int h):name_hash(h) {}
		};

		/// <summary>
		/// 元素节点
		/// </summary>
		struct ElementNode
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
		struct AnimationFrameNode
		{
			float x = 0, y = 0, w = 0, h = 0;

			std::vector<EventNode> events;//event count + event
			std::vector<ElementNode> elements;//elem count + elems
		};

		/// <summary>
		/// 动画节点
		/// </summary>
		struct AnimationNode
		{
			std::string name;

			/// <summary>朝向</summary>
			Facing facing = Facing::All;

			/// <summary>根符号哈希</summary>
			unsigned int rootsym_hash = 0;

			/// <summary>帧率</summary>
			float frame_rate = 10.0f;

			std::vector<AnimationFrameNode> frames;
		};

		/// <summary>
		/// Atlas
		/// </summary>
		struct AtlasNode
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
		/// <returns>返回<see cref="std::wstring"/>，建议配合std::move使用以避免构造开销</returns>
		/// <example><c>wstring elem_str = std::move(ToString(elem));</c></example>
		/// <created>Fa鸽,2019/7/27</created>
		/// <changed>Fa鸽,2019/7/27</changed>
		std::wstring EXPORT_API ToString(const ElementNode& elem);

		inline std::wstring ToString(const BuildFrameNode& elem)
		{
			std::wostringstream o;
			o << L"Frame number = " << elem.frame_number << L'\n';
			o << L"Duration = " << elem.duration << L'\n';
			o << L"x,y,w,h = " << elem.x << L',' << elem.y << L',' << elem.w << L',' << elem.h << L'\n';
			o << L"alpha_index = " << elem.alpha_index << L'\n' << L"alpha_count = " << elem.alpha_count << L'\n';
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

		}

		

		/// <summary>
		/// 比较两个ElementNode是否相等
		/// </summary>
		/// <param name="l">1</param>
		/// <param name="r">2</param>
		/// <returns>相等返回true，不相等返回false</returns>
		/// <created>Fa鸽,2019/7/27</created>
		/// <changed>Fa鸽,2019/7/27</changed>
		bool EXPORT_API operator==(const ElementNode& l, const ElementNode& r);

		class EXPORT_API AnimationBase : protected virtual Common::BinaryFileBase
		{
			//friend void ::KleiAnim::AnimBin2XML(Common::AnimationBase& binary, std::filesystem::path xmlpath);


		public:
			AnimationBase() = default;

			AnimationBase(const std::vector<Common::AnimationNode>& animations,
				const std::map<unsigned int, std::string>& string_table) :animations(animations), BinaryFileBase(string_table)
			{

			}

		protected:
			///<summary>合规文件的CC4，ANIM</summary>
			static constexpr unsigned int valid_cc4 = 0x4D494E41;
			///<summary>当前版本</summary>
			static constexpr unsigned short cur_version = 0x0004;

			/// <summary>animation</summary>
			std::vector<Common::AnimationNode> animations;
		};

		class EXPORT_API BuildBase : protected virtual Common::BinaryFileBase
		{
			//friend void ::KleiAnim::BuildBin2XML(Common::BuildBase& binary, std::filesystem::path xmlpath);

		public:
			BuildBase() = default;

			BuildBase(unsigned int symbol_count, unsigned int frame_count,
				const std::string& build_name, const std::vector<Common::AtlasNode>& atlases,
				const std::vector<Common::SymbolNode>& symbols,
				const std::vector<Common::AlphaVertexNode>& vertices) :
				symbol_count(symbol_count),
				frame_count(frame_count),
				build_name(build_name),
				atlases(atlases),
				symbols(symbols),
				vertices(vertices)
			{}

			std::wstring ToString();

		protected:
			///<summary>合规文件的CC4，BILD</summary>
			static constexpr unsigned int valid_cc4 = 0x444C4942;
			///<summary>当前版本</summary>
			static constexpr unsigned short cur_version = 0x0006;

			unsigned int symbol_count = 0;
			unsigned int frame_count = 0;
			std::string build_name;

			std::vector<Common::AtlasNode> atlases;
			std::vector<Common::SymbolNode> symbols;
			std::vector<Common::AlphaVertexNode> vertices;
		};

		constexpr uint32_t common_license_hash[] = {
			1495471998ui32,	//SPDX-License-Identifier: CC-BY-NC-4.0
			1850363105ui32,	//SPDX-License-Identifier: MIT
			377050555ui32,	//SPDX-License-Identifier: GPL-3.0-or-later
		};
	}
}