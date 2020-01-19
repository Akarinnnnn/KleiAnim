/***************************
* Encoding: GB2312
* KleiAnim�Ĺ��ò��֣������ṹ�嶨�弰���ú���
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
//ȷ��Ŀ�������С�˻�
//static_assert(std::endian::native == std::endian::little)


namespace KleiAnim
{
	namespace Common
	{
		/// <summary>
		/// ��Ҫ�ı�KleiAnim����־�����ֻ���ڵ���KleiAnim֮ǰ����һ��ʵ��
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
		/// ��Ҫ�ı�KleiAnim����־�����ֻ���ڵ���KleiAnim֮ǰ����һ��ʵ��
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
		struct AlphaVertexNode
		{
			float x = 0, y = 0, z = 0, u = 0, v = 0, w = 0;
		};

		/// <summary>
		/// Build�е�֡�ڵ�
		/// </summary>
		struct BuildFrameNode
		{
			/// <summary>֡��</summary>
			unsigned int frame_number = 0;

			/// <summary>����ʱ��</summary>
			unsigned int duration = 1;

			/// <summary>bbox xywh����</summary>
			float x = 0, y = 0, w = 0, h = 0;


			unsigned int alpha_index = 0;
			unsigned int alpha_count = 0;
		};

		/// <summary>
		/// ���Žڵ�
		/// </summary>
		struct SymbolNode
		{
			unsigned int name_hash = 0;
			std::vector<BuildFrameNode> frames;
		};

		/// <summary>
		/// �¼��ڵ�
		/// </summary>
		struct EXPORT_API EventNode
		{
			/// <summary>���ƵĹ�ϣ</summary>
			unsigned int name_hash;

			operator unsigned int()
			{
				return name_hash;
			}
			
			EventNode() : name_hash(0) {}
			EventNode(unsigned int h):name_hash(h) {}
		};

		/// <summary>
		/// Ԫ�ؽڵ�
		/// </summary>
		struct ElementNode
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
		struct AnimationFrameNode
		{
			float x = 0, y = 0, w = 0, h = 0;

			std::vector<EventNode> events;//event count + event
			std::vector<ElementNode> elements;//elem count + elems
		};

		/// <summary>
		/// �����ڵ�
		/// </summary>
		struct AnimationNode
		{
			std::string name;

			/// <summary>����</summary>
			Facing facing = Facing::All;

			/// <summary>�����Ź�ϣ</summary>
			unsigned int rootsym_hash = 0;

			/// <summary>֡��</summary>
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
		/// <returns>����<see cref="std::wstring"/>���������std::moveʹ���Ա��⹹�쿪��</returns>
		/// <example><c>wstring elem_str = std::move(ToString(elem));</c></example>
		/// <created>Fa��,2019/7/27</created>
		/// <changed>Fa��,2019/7/27</changed>
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
		/// �Ƚ�����ElementNode�Ƿ����
		/// </summary>
		/// <param name="l">1</param>
		/// <param name="r">2</param>
		/// <returns>��ȷ���true������ȷ���false</returns>
		/// <created>Fa��,2019/7/27</created>
		/// <changed>Fa��,2019/7/27</changed>
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
			///<summary>�Ϲ��ļ���CC4��ANIM</summary>
			static constexpr unsigned int valid_cc4 = 0x4D494E41;
			///<summary>��ǰ�汾</summary>
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
			///<summary>�Ϲ��ļ���CC4��BILD</summary>
			static constexpr unsigned int valid_cc4 = 0x444C4942;
			///<summary>��ǰ�汾</summary>
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