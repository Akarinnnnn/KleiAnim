/***************************
* Encoding: GB2312
* 对二进制的动画进行读写
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
		/// .bin文件的类型
		/// </summary>
		enum class BinaryType : unsigned char
		{
			
			/// <summary>假的</summary>
			Neither = 0,
			/// <summary>Build</summary>
			Build = 1,
			/// <summary>Animation</summary>
			Animation = 2
		};


		/// <summary>
		/// 检查bin文件的类型
		/// </summary>
		/// <param name="filepath">路径</param>
		/// <returns></returns>
		/// <created>Fa鸽,2019/10/2</created>
		/// <changed>Fa鸽,2019/10/2</changed>
		extern "C" EXPORT_API BinaryType CheckFileType(std::filesystem::path filepath);

		/// <summary>
		/// anim.bin读取器
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
			/// 获取常见许可信息的hash
			/// </summary>
			/// <returns>找不到为0</returns>
			/// <created>Fa鸽,2020/1/18</created>
			/// <changed>Fa鸽,2020/1/18</changed>
			uint32_t license_hash() const;

			/// <summary>
			/// 返回hash对应的字符串，建议结合std::move使用
			/// </summary>
			/// <example><c>string name = std::move(anim.de_hash(name_hash));</c></example>
			/// <param name="hash">哈希值</param>
			/// <returns></returns>
			/// <created>Fa鸽,2019/7/29</created>
			/// <changed>Fa鸽,2019/7/29</changed>
			std::string de_hash(const unsigned int hash) const;

			/// <summary>
			/// 获取i号动画
			/// </summary>
			/// <param name="i"></param>
			/// <returns></returns>
			/// <created>Fa鸽,2019/7/29</created>
			/// <changed>Fa鸽,2019/7/29</changed>
			const Common::AnimationNode& animation(const size_t i) const;

			/// <summary>
			/// 获取i号动画，与animation(size_t i)相同
			/// </summary>
			/// <param name="i"></param>
			/// <returns></returns>
			/// <created>Fa鸽,2019/7/29</created>
			/// <changed>Fa鸽,2019/7/29</changed>
			const Common::AnimationNode& operator[] (const size_t i) const;

			/// <summary>
			/// 获取相应动画的第frame帧
			/// </summary>
			/// <param name="anim">动画序号</param>
			/// <param name="i">帧序号</param>
			/// <returns></returns>
			/// <created>Fa鸽,2019/7/29</created>
			/// <changed>Fa鸽,2019/7/29</changed>
			const Common::AnimationFrameNode& frame(const size_t anim, const size_t frame) const;
		
			/// <summary>
			/// 获取anim号动画，第frame帧的事件（一般的动画没有）
			/// </summary>
			/// <param name="anim"></param>
			/// <param name="frame"></param>
			/// <returns></returns>
			/// <created>Fa鸽,2019/7/29</created>
			/// <changed>Fa鸽,2019/7/29</changed>
			const std::vector<Common::EventNode>& events(const size_t anim, const size_t frame) const;		

			/// <summary>
			/// 获取anim号动画，第frame帧的元素引用
			/// </summary>
			/// <param name="anim"></param>
			/// <param name="frame"></param>
			/// <returns></returns>
			/// <created>Fa鸽,2019/7/29</created>
			/// <changed>Fa鸽,2019/7/29</changed>
			const std::vector<Common::ElementNode>& element_refs(const size_t anim, const size_t frame) const;
		};

		/// <summary>
		/// build.bin读取器
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
			/// 获取常见许可信息的hash
			/// </summary>
			/// <returns>找不到为0</returns>
			/// <created>Fa鸽,2020/1/18</created>
			/// <changed>Fa鸽,2020/1/18</changed>
			uint32_t license_hash() const;
			
			/// <summary>
			/// 名称
			/// </summary>
			/// <returns></returns>
			/// <created>Fa鸽,2019/8/2</created>
			/// <changed>Fa鸽,2019/8/2</changed>
			std::string name() const;

			/// <summary>
			/// 获取 i 号 符号
			/// </summary>
			/// <param name="i"></param>
			/// <returns></returns>
			/// <created>Fa鸽,2019/7/31</created>
			/// <changed>Fa鸽,2019/7/31</changed>
			const Common::SymbolNode& symbol(const size_t i) const;

			/// <summary>
			/// 获取 i 号 符号
			/// </summary>
			/// <param name="i"></param>
			/// <returns></returns>
			/// <created>Fa鸽,2019/8/2</created>
			/// <changed>Fa鸽,2019/8/2</changed>
			const Common::SymbolNode& operator[] (const size_t i) const;

			/// <summary>
			/// 获取 i 号atlas
			/// </summary>
			/// <param name="i"></param>
			/// <returns></returns>
			/// <created>Fa鸽,2019/8/2</created>
			/// <changed>Fa鸽,2019/8/2</changed>
			const Common::AtlasNode& atlas(const size_t i) const;

			/// <summary>
			/// 获取 i 号顶点三角形
			/// </summary>
			/// <param name="i"></param>
			/// <returns></returns>
			/// <created>Fa鸽,2019/8/2</created>
			/// <changed>Fa鸽,2019/8/2</changed>
			const Common::AlphaVertexNode& vertex(const size_t i) const;

			/// <summary>
			/// 获取第i组顶点三角形
			/// </summary>
			/// <returns>std::array，不是内置数组</returns>
			/// <example><c>for(auto&amp; vertex: bin.vertices(0))</c></example>
			/// <param name="start">第i组</param>
			/// <created>Fa鸽,2019/8/2</created>
			/// <changed>Fa鸽,2019/8/2</changed>
			std::array<Common::AlphaVertexNode, 6> vertices(const unsigned int start) const;

			/// <summary>
			/// 获取sym 符号的 i 帧
			/// </summary>
			/// <param name="sym"></param>
			/// <param name="i"></param>
			/// <returns></returns>
			/// <created>Fa鸽,2019/8/2</created>
			/// <changed>Fa鸽,2019/8/2</changed>
			const Common::BuildFrameNode& frame(const size_t sym, const size_t i) const;

			/// <summary>
			/// 返回hash对应的字符串，建议结合std::move使用
			/// </summary>
			/// <param name="hash">哈希值</param>
			/// <returns>std::string</returns>
			/// <example>
			/// <c>string name = std::move(anim.de_hash(name_hash));</c>
			/// </example>
			/// <created>Fa鸽,2019/7/29</created>
			/// <changed>Fa鸽,2019/10/1</changed>
			std::string de_hash(const unsigned int hash) const;

			
		};
	
		/// <summary>
		/// 创建anim.bin，用于写入指定的文件/流
		/// </summary>
		class EXPORT_API AnimationWriter :public Common::AnimationBase
		{
		public:

			/// <summary>
			/// 以指定的输出路径构造AnimationWriter
			/// </summary>
			/// <param name="out">输出的路径，含文件名</param>
			/// <returns></returns>
			/// <created>Fa鸽,2019/8/2</created>
			/// <changed>Fa鸽,2019/8/2</changed>
			AnimationWriter(const std::filesystem::path& out);
			AnimationWriter(const std::filesystem::path& out, const AnimationBase & base);
			~AnimationWriter();

			/// <summary>
			/// 将内容写入到文件
			/// </summary>
			/// <created>Fa鸽,2019/8/2</created>
			/// <changed>Fa鸽,2019/8/2</changed>
			void writefile();

			/// <summary>
			/// 将内容写入到流
			/// </summary>
			/// <param name="out"></param>
			/// <created>Fa鸽,2019/8/17</created>
			/// <changed>Fa鸽,2019/8/17</changed>
			void writestream(std::ostream& out);

			/// <summary>
			/// 添加一段动画
			/// </summary>
			/// <param name="anim">待添加的动画</param>
			/// <created>Fa鸽,2019/8/2</created>
			/// <changed>Fa鸽,2019/8/2</changed>
			void add(Common::AnimationNode& anim);

			void add_hashstringpair(unsigned int hash, std::string&& string);
			void add_hashstringpair(unsigned int hash, std::string& string);


			/// <summary>输出文件的路径</summary>
			std::filesystem::path out;
		};

		/// <summary>
		/// 创建build.bin，用于写入指定的文件/流
		/// </summary>
		class EXPORT_API BuildWriter :public Common::BuildBase
		{
		public:
			/// <summary>
			/// 以指定的输出路径构造BuildWriter
			/// </summary>
			/// <param name="out">输出的路径，含文件名</param>
			/// <returns></returns>
			/// <created>Fa鸽,2019/8/2</created>
			/// <changed>Fa鸽,2019/8/2</changed>
			BuildWriter(const std::filesystem::path& out);
			BuildWriter(const std::filesystem::path & out, const BuildBase & base);
			~BuildWriter();

			using BuildBase::build_name;

			/// <summary>
			/// 将内容写入到文件
			/// </summary>
			/// <created>Fa鸽,2019/8/2</created>
			/// <changed>Fa鸽,2019/8/2</changed>
			void writefile();

			/// <summary>
			/// 将内容写入到流
			/// </summary>
			/// <param name="out"></param>
			/// <created>Fa鸽,2019/8/17</created>
			/// <changed>Fa鸽,2019/8/17</changed>
			void writestream(std::ostream& out);

			/// <summary>
			/// 添加一个符号
			/// </summary>
			/// <param name="frame"></param>
			/// <created>Fa鸽,2019/8/2</created>
			/// <changed>Fa鸽,2019/8/2</changed>
			void add(Common::SymbolNode& sym);

			/// <summary>
			/// 添加一个顶点三角形
			/// </summary>
			/// <param name="vert"></param>
			/// <created>Fa鸽,2019/8/2</created>
			/// <changed>Fa鸽,2019/8/2</changed>
			void add(Common::AlphaVertexNode& vert);

			/// <summary>
			/// 添加一个atlas
			/// </summary>
			/// <param name="atlas"></param>
			/// <created>Fa鸽,2019/8/2</created>
			/// <changed>Fa鸽,2019/8/2</changed>
			void add(Common::AtlasNode& atlas);

			/// <summary>
			/// 添加一组（6个）顶点三角形
			/// </summary>
			/// <param name="vertices">数组</param>
			/// <created>Fa鸽,2019/8/2</created>
			/// <changed>Fa鸽,2019/8/2</changed>
			void add(const std::array<Common::AlphaVertexNode,6>& vertices);

			void add_hashstringpair(unsigned int hash, std::string&& string);
			void add_hashstringpair(unsigned int hash, std::string& string);


			/// <summary>输出文件的路径</summary>
			std::filesystem::path out;
		};
	}
}
