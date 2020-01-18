/***************************
* Encoding: GB2312
* �Զ����ƵĶ������ж�д
***************************/
#pragma once
#include "common/anim_common.hpp"
#include "common/export.h"
#include <filesystem>
#include <fstream>
#include <array>

namespace KleiAnim
{
	namespace Binary
	{
		
		/// <summary>
		/// .bin�ļ�������
		/// </summary>
		enum class BinaryType : unsigned char
		{
			
			/// <summary>�ٵ�</summary>
			Neither = 0,
			/// <summary>Build</summary>
			Build = 1,
			/// <summary>Animation</summary>
			Animation = 2
		};


		/// <summary>
		/// ���bin�ļ�������
		/// </summary>
		/// <param name="filepath">·��</param>
		/// <returns></returns>
		/// <created>Fa��,2019/10/2</created>
		/// <changed>Fa��,2019/10/2</changed>
		extern "C" EXPORT_API BinaryType CheckFileType(std::filesystem::path filepath);

		/// <summary>
		/// anim.bin��ȡ��
		/// </summary>
		class EXPORT_API AnimationReader : public Common::AnimationBase
		{
		public:
			AnimationReader() = delete;
			AnimationReader(const std::filesystem::path & animpath);
			AnimationReader(const Common::AnimationBase& base):AnimationBase(base){}

			unsigned int anim_count() const;
			
			std::vector<Common::AnimationNode>::const_iterator begin() const;
			std::vector<Common::AnimationNode>::const_iterator end() const;

			/// <summary>
			/// ��ȡ���������Ϣ��hash
			/// </summary>
			/// <returns>�Ҳ���Ϊ0</returns>
			/// <created>Fa��,2020/1/18</created>
			/// <changed>Fa��,2020/1/18</changed>
			uint32_t license_hash() const;

			/// <summary>
			/// ����hash��Ӧ���ַ�����������std::moveʹ��
			/// </summary>
			/// <example><c>string name = std::move(anim.de_hash(name_hash));</c></example>
			/// <param name="hash">��ϣֵ</param>
			/// <returns></returns>
			/// <created>Fa��,2019/7/29</created>
			/// <changed>Fa��,2019/7/29</changed>
			std::string de_hash(const unsigned int hash) const;

			/// <summary>
			/// ��ȡi�Ŷ���
			/// </summary>
			/// <param name="i"></param>
			/// <returns></returns>
			/// <created>Fa��,2019/7/29</created>
			/// <changed>Fa��,2019/7/29</changed>
			const Common::AnimationNode& animation(const size_t i) const;

			/// <summary>
			/// ��ȡi�Ŷ�������animation(size_t i)��ͬ
			/// </summary>
			/// <param name="i"></param>
			/// <returns></returns>
			/// <created>Fa��,2019/7/29</created>
			/// <changed>Fa��,2019/7/29</changed>
			const Common::AnimationNode& operator[] (const size_t i) const;

			/// <summary>
			/// ��ȡ��Ӧ�����ĵ�frame֡
			/// </summary>
			/// <param name="anim">�������</param>
			/// <param name="i">֡���</param>
			/// <returns></returns>
			/// <created>Fa��,2019/7/29</created>
			/// <changed>Fa��,2019/7/29</changed>
			const Common::AnimationFrameNode& frame(const size_t anim, const size_t frame) const;
		
			/// <summary>
			/// ��ȡanim�Ŷ�������frame֡���¼���һ��Ķ���û�У�
			/// </summary>
			/// <param name="anim"></param>
			/// <param name="frame"></param>
			/// <returns></returns>
			/// <created>Fa��,2019/7/29</created>
			/// <changed>Fa��,2019/7/29</changed>
			const std::vector<Common::EventNode>& events(const size_t anim, const size_t frame) const;		

			/// <summary>
			/// ��ȡanim�Ŷ�������frame֡��Ԫ������
			/// </summary>
			/// <param name="anim"></param>
			/// <param name="frame"></param>
			/// <returns></returns>
			/// <created>Fa��,2019/7/29</created>
			/// <changed>Fa��,2019/7/29</changed>
			const std::vector<Common::ElementNode>& element_refs(const size_t anim, const size_t frame) const;
		};

		/// <summary>
		/// build.bin��ȡ��
		/// </summary>
		class EXPORT_API BuildReader :public Common::BuildBase
		{
		public:
			BuildReader() = delete;
			BuildReader(const std::filesystem::path & buildpath);
			BuildReader(const Common::BuildBase & base):BuildBase(base) {}


			std::vector<Common::SymbolNode>::const_iterator begin() const;
			std::vector<Common::SymbolNode>::const_iterator end() const;

			unsigned int symbol_count() const;
			unsigned int atlas_count() const;
			unsigned int vertex_count() const;
			
			/// <summary>
			/// ��ȡ���������Ϣ��hash
			/// </summary>
			/// <returns>�Ҳ���Ϊ0</returns>
			/// <created>Fa��,2020/1/18</created>
			/// <changed>Fa��,2020/1/18</changed>
			uint32_t license_hash() const;
			
			/// <summary>
			/// ����
			/// </summary>
			/// <returns></returns>
			/// <created>Fa��,2019/8/2</created>
			/// <changed>Fa��,2019/8/2</changed>
			std::string name() const;

			/// <summary>
			/// ��ȡ i �� ����
			/// </summary>
			/// <param name="i"></param>
			/// <returns></returns>
			/// <created>Fa��,2019/7/31</created>
			/// <changed>Fa��,2019/7/31</changed>
			const Common::SymbolNode& symbol(const size_t i) const;

			/// <summary>
			/// ��ȡ i �� ����
			/// </summary>
			/// <param name="i"></param>
			/// <returns></returns>
			/// <created>Fa��,2019/8/2</created>
			/// <changed>Fa��,2019/8/2</changed>
			const Common::SymbolNode& operator[] (const size_t i) const;

			/// <summary>
			/// ��ȡ i ��atlas
			/// </summary>
			/// <param name="i"></param>
			/// <returns></returns>
			/// <created>Fa��,2019/8/2</created>
			/// <changed>Fa��,2019/8/2</changed>
			const Common::AtlasNode& atlas(const size_t i) const;

			/// <summary>
			/// ��ȡ i �Ŷ���������
			/// </summary>
			/// <param name="i"></param>
			/// <returns></returns>
			/// <created>Fa��,2019/8/2</created>
			/// <changed>Fa��,2019/8/2</changed>
			const Common::AlphaVertexNode& vertex(const size_t i) const;

			/// <summary>
			/// ��ȡ��i�鶥��������
			/// </summary>
			/// <returns>std::array��������������</returns>
			/// <example><c>for(auto&amp; vertex: bin.vertices(0))</c></example>
			/// <param name="start">��i��</param>
			/// <created>Fa��,2019/8/2</created>
			/// <changed>Fa��,2019/8/2</changed>
			std::array<Common::AlphaVertexNode, 6> vertices(const unsigned int start) const;

			/// <summary>
			/// ��ȡsym ���ŵ� i ֡
			/// </summary>
			/// <param name="sym"></param>
			/// <param name="i"></param>
			/// <returns></returns>
			/// <created>Fa��,2019/8/2</created>
			/// <changed>Fa��,2019/8/2</changed>
			const Common::BuildFrameNode& frame(const size_t sym, const size_t i) const;

			/// <summary>
			/// ����hash��Ӧ���ַ�����������std::moveʹ��
			/// </summary>
			/// <param name="hash">��ϣֵ</param>
			/// <returns>std::string</returns>
			/// <example>
			/// <c>string name = std::move(anim.de_hash(name_hash));</c>
			/// </example>
			/// <created>Fa��,2019/7/29</created>
			/// <changed>Fa��,2019/10/1</changed>
			std::string de_hash(const unsigned int hash) const;

			
		};
	
		/// <summary>
		/// ����anim.bin������д��ָ�����ļ�/��
		/// </summary>
		class EXPORT_API AnimationWriter :public Common::AnimationBase
		{
		public:

			/// <summary>
			/// ��ָ�������·������AnimationWriter
			/// </summary>
			/// <param name="out">�����·�������ļ���</param>
			/// <returns></returns>
			/// <created>Fa��,2019/8/2</created>
			/// <changed>Fa��,2019/8/2</changed>
			AnimationWriter(const std::filesystem::path& out);
			AnimationWriter(const std::filesystem::path& out, const AnimationBase & base);
			~AnimationWriter();

			/// <summary>
			/// ������д�뵽�ļ�
			/// </summary>
			/// <created>Fa��,2019/8/2</created>
			/// <changed>Fa��,2019/8/2</changed>
			void writefile();

			/// <summary>
			/// ������д�뵽��
			/// </summary>
			/// <param name="out"></param>
			/// <created>Fa��,2019/8/17</created>
			/// <changed>Fa��,2019/8/17</changed>
			void writestream(std::ostream& out);

			/// <summary>
			/// ���һ�ζ���
			/// </summary>
			/// <param name="anim">����ӵĶ���</param>
			/// <created>Fa��,2019/8/2</created>
			/// <changed>Fa��,2019/8/2</changed>
			void add(Common::AnimationNode& anim);

			void add_hashstringpair(unsigned int hash, std::string&& string);
			void add_hashstringpair(unsigned int hash, std::string& string);


			/// <summary>����ļ���·��</summary>
			std::filesystem::path out;
		};

		/// <summary>
		/// ����build.bin������д��ָ�����ļ�/��
		/// </summary>
		class EXPORT_API BuildWriter :public Common::BuildBase
		{
		public:
			/// <summary>
			/// ��ָ�������·������BuildWriter
			/// </summary>
			/// <param name="out">�����·�������ļ���</param>
			/// <returns></returns>
			/// <created>Fa��,2019/8/2</created>
			/// <changed>Fa��,2019/8/2</changed>
			BuildWriter(const std::filesystem::path& out);
			BuildWriter(const std::filesystem::path & out, const BuildBase & base);
			~BuildWriter();

			using BuildBase::build_name;

			/// <summary>
			/// ������д�뵽�ļ�
			/// </summary>
			/// <created>Fa��,2019/8/2</created>
			/// <changed>Fa��,2019/8/2</changed>
			void writefile();

			/// <summary>
			/// ������д�뵽��
			/// </summary>
			/// <param name="out"></param>
			/// <created>Fa��,2019/8/17</created>
			/// <changed>Fa��,2019/8/17</changed>
			void writestream(std::ostream& out);

			/// <summary>
			/// ���һ������
			/// </summary>
			/// <param name="frame"></param>
			/// <created>Fa��,2019/8/2</created>
			/// <changed>Fa��,2019/8/2</changed>
			void add(Common::SymbolNode& sym);

			/// <summary>
			/// ���һ������������
			/// </summary>
			/// <param name="vert"></param>
			/// <created>Fa��,2019/8/2</created>
			/// <changed>Fa��,2019/8/2</changed>
			void add(Common::AlphaVertexNode& vert);

			/// <summary>
			/// ���һ��atlas
			/// </summary>
			/// <param name="atlas"></param>
			/// <created>Fa��,2019/8/2</created>
			/// <changed>Fa��,2019/8/2</changed>
			void add(Common::AtlasNode& atlas);

			/// <summary>
			/// ���һ�飨6��������������
			/// </summary>
			/// <param name="vertices">����</param>
			/// <created>Fa��,2019/8/2</created>
			/// <changed>Fa��,2019/8/2</changed>
			void add(const std::array<Common::AlphaVertexNode,6>& vertices);

			void add_hashstringpair(unsigned int hash, std::string&& string);
			void add_hashstringpair(unsigned int hash, std::string& string);


			/// <summary>����ļ���·��</summary>
			std::filesystem::path out;
		};
	}
}
