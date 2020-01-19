#include "../pch.h"
#include "Binary.hpp"
#include "common/exceptions.hpp"
#include <memory>
#include <thread>
#include <combaseapi.h>
#include <mutex>
#include <chrono>
#include <iostream>

using namespace KleiAnim::Binary;
using namespace KleiAnim;
using std::unique_ptr;
using std::string;
using HashedStringTable = std::map<unsigned int, string>;
using std::filesystem::path;
using std::ios;

#define TO_PCHAR(x) reinterpret_cast<char*>(&(x))
#define TO_CONST_PCHAR(x) reinterpret_cast<const char*>(&(x))

#define LOG(str) L##str

#ifdef ENABLE_TIME_RECORD
#define PRINT_TIME std::cout<< "T = " << std::chrono::high_resolution_clock::now().time_since_epoch().count() << "ns" << std::endl
#endif // DEBUG

#pragma warning(disable:4267)//在MSVC上忽略C4267警告，初始化均经过检查

//还不如st_read_elem，不信看看测试
//留着以后看看能不能改进
//定义MT_READ_ELEM以使用这个sb函数
void mt_read_elem(const unsigned int count,
	std::vector<::KleiAnim::Common::ElementNode>& out,
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
				::KleiAnim::Common::ElementNode read_out{ 0,0,0,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f };

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
		std::this_thread::sleep_for(100ms);
	}
}

std::vector<KleiAnim::Common::ElementNode> st_read_elem(std::ifstream& f, unsigned int count)
{
	using KleiAnim::Common::ElementNode;
	std::vector<ElementNode> ret;
	ret.resize(count);
	f.read((char*)ret.data(), static_cast<size_t>(count) * 40Ui64);
	return ret;
}

void write_str(const std::string& s, std::ostream& o)
{
	size_t size_ui64 = s.size();
	if (size_ui64 > UINT32_MAX)
		throw std::invalid_argument("KleiAnim/Binary.cpp " __FUNCTION__ " s:字符串过长");
	unsigned int size = size_ui64;
	
	o.write(TO_PCHAR(size), 4);
	o.write(s.data(), size);
}

AnimationReader::AnimationReader(const std::filesystem::path & animpath)
{
	using std::cout;
	using std::endl;
	std::ifstream file(animpath, std::ios::binary | std::ios::in);

	{
		std::error_code ec;
		if (!std::filesystem::exists(animpath, ec))
			throw std::system_error(ec, "文件不存在");
	}

	if (!file.is_open())
	{
		throw std::invalid_argument("打开失败");
	}

	file.read(TO_PCHAR(cc4), 8);

	if (cc4 != valid_cc4 || version != cur_version)
	{
		throw Exception::invalid_file("假的anim.bin", cc4, version);
	}

	alignas(16) struct { unsigned int elem = 0, frame = 0, event = 0, anim = 0; } info;

	file.read(TO_PCHAR(info), 16);

	unsigned int elem_total = 0, frame_total = 0, event_total = 0;

	animations.reserve(info.anim);
	for (size_t i_anim = 0; i_anim < info.anim; i_anim++)
	{
		Common::AnimationNode anim;
		//第一段
#ifdef ENABLE_TIME_RECORD
		cout << "i_anim = " << i_anim << ',';
		cout << "read build_name,facing,hashes" << endl;
		PRINT_TIME;
#endif
		{
			anim.name = Common::read_str(file);
			file.read(TO_PCHAR(anim.facing), 1);

			if (anim.facing != Common::Facing::All)
			{
				KleiAnimLog::write() << animpath << LOG(" 不是一段全朝向的动画。");
			}

			file.read(TO_PCHAR(anim.rootsym_hash), 4);
			file.read(TO_PCHAR(anim.frame_rate), sizeof(float));
		}
#ifdef ENABLE_TIME_RECORD
		cout << "finished,";
		PRINT_TIME;
		cout << "read frames" << endl;
#endif 

		//frame
		{
			unsigned int frame_count = 0;
			file.read(TO_PCHAR(frame_count), 4);
			anim.frames.reserve(frame_count);

			for (size_t i = 0; i < frame_count; i++)
			{
#ifdef ENABLE_TIME_RECORD
				cout << "frame i = " << i;
				PRINT_TIME;
#endif 
				Common::AnimationFrameNode frame;
				unsigned int event_count = 0, elem_count = 0;

				file.read(TO_PCHAR(frame.x), 4 * sizeof(float));

				file.read(TO_PCHAR(event_count), 4);

				frame.events.resize(event_count);
				file.read((char*)(frame.events.data()), (size_t)4 * (size_t)event_count);

				file.read(TO_PCHAR(elem_count), 4);
				frame.elements.reserve(elem_count);

				#ifndef MT_READ_ELEM
				frame.elements = std::move(st_read_elem(file, elem_count));
				#else
				mt_read_elem(elem_count, frame.elements, animpath, file.tellg());
				#endif

				anim.frames.push_back(std::move(frame));
				elem_total += elem_count;
				event_total += event_count;
#ifdef ENABLE_TIME_RECORD
				cout << "finished.";
				PRINT_TIME;
#endif 
			}
			frame_total += frame_count;

		}
#ifdef ENABLE_TIME_RECORD
		cout << "read anim finished.";
		PRINT_TIME;
#endif 
		animations.push_back(std::move(anim));
#ifdef ENABLE_TIME_RECORD
		cout << "push_back";
		PRINT_TIME;
#endif 
	}
	str_table = Common::read_strhashtable(file);

	if (elem_total != info.elem || frame_total != info.frame || event_total != info.event)
	{
		KleiAnimLog::write() << LOG("警告：元素数量/帧数量/事件数量与实际不符。\n") <<
			LOG("类型 预期数量 实际数量\n") <<
			LOG("元素 ") << info.elem << ' ' << elem_total << endl <<
			LOG("帧   ") << info.frame << ' ' << frame_total << endl <<
			LOG("事件 ") << info.event << ' ' << event_total << endl;
	}
}

unsigned int KleiAnim::Binary::AnimationReader::anim_count() const
{
	return animations.size();
}

std::vector<Common::AnimationNode>::const_iterator KleiAnim::Binary::AnimationReader::begin() const
{
	return this->animations.begin();
}

std::vector<Common::AnimationNode>::const_iterator KleiAnim::Binary::AnimationReader::end() const
{
	return this->animations.end();
}

uint32_t KleiAnim::Binary::AnimationReader::license_hash() const
{
	for (auto& hash : Common::common_license_hash)
	{
		auto it = this->str_table.find(hash);
		if (it != str_table.end()) return it->first;
	}
	return 0;
}

std::string KleiAnim::Binary::AnimationReader::de_hash(const unsigned int hash) const
{
	return str_table.at(hash);
}

const Common::AnimationNode& KleiAnim::Binary::AnimationReader::animation(const size_t i) const
{
	return animations[i];
}

const Common::AnimationNode& KleiAnim::Binary::AnimationReader::operator[](const size_t i) const
{
	return animations[i];
}

const Common::AnimationFrameNode& KleiAnim::Binary::AnimationReader::frame(const size_t anim, const size_t frame) const
{
	return animations[anim].frames[frame];
}

const std::vector<Common::EventNode>& KleiAnim::Binary::AnimationReader::events(const size_t anim, const size_t _frame) const
{
	return animations[anim].frames[_frame].events;
}

const std::vector<Common::ElementNode>& KleiAnim::Binary::AnimationReader::element_refs(const size_t anim, const size_t frame) const
{
	return animations[anim].frames[frame].elements;
}

BuildReader::BuildReader(const std::filesystem::path & buildpath)
	
{
	using std::cout;
	using std::endl;
	std::ifstream file(buildpath, std::ios::binary | std::ios::in);

	{
		std::error_code ec;
		if (!std::filesystem::exists(buildpath, ec))
			throw std::system_error(ec, "文件不存在");
	}

	if (!file.is_open())
	{
		throw std::invalid_argument("打开失败");
	}

	file.read(TO_PCHAR(cc4), 8);
	if (cc4 != valid_cc4 || version != cur_version)
	{
		throw Exception::invalid_file("应提供正确的build.bin", cc4, version);
	}
	
	file.read(TO_PCHAR(BuildBase::symbol_count), 8);
	build_name = std::move(Common::read_str(file));
	symbols.reserve(BuildBase::symbol_count);
	//atlas
	{
		unsigned int atlas_count = 0;
		file.read(TO_PCHAR(atlas_count), 4);
		atlases.reserve(atlas_count);
		for (unsigned int i = 0; i < atlas_count; i++)
		{
			atlases.push_back({ Common::read_str(file) });
		}
	}

	//symbol
	unsigned int frame_total = 0;
	{
		for (unsigned int i = 0; i < BuildBase::symbol_count; i++)
		{
			Common::SymbolNode symbol;
			Common::BuildFrameNode curframe;
			unsigned int cur_frame_count = 0;
			file.read(TO_PCHAR(symbol.name_hash), 4);
			file.read(TO_PCHAR(cur_frame_count), 4);

			for (unsigned int i = 0; i < cur_frame_count; i++)
			{
				file.read(TO_PCHAR(curframe), 32);
				symbol.frames.push_back(std::move(curframe));
			}
			frame_total += cur_frame_count;
			symbols.push_back(std::move(symbol));
		}
	}

	//alpha vertex
	{
		unsigned int vertex_count = 0;
		Common::AlphaVertexNode avn;
		file.read(TO_PCHAR(vertex_count), 4);
		BuildBase::vertices.reserve(vertex_count);
		for (unsigned int i = 0; i < vertex_count; i++)
		{
			file.read(TO_PCHAR(avn), 24);
			BuildBase::vertices.push_back(avn);
		}
	}

	str_table = Common::read_strhashtable(file);

	if (frame_total != frame_count)
	{
		KleiAnimLog::write() << LOG("警告：帧数量与实际不符。\n") <<
			LOG("预期数量 实际数量\n") <<
			frame_count << ' ' << frame_total << endl;
	}
}

std::vector<Common::SymbolNode>::const_iterator KleiAnim::Binary::BuildReader::begin() const
{
	return symbols.begin();
}

std::vector<Common::SymbolNode>::const_iterator KleiAnim::Binary::BuildReader::end() const
{
	return symbols.end();
}

unsigned int KleiAnim::Binary::BuildReader::symbol_count() const
{
	return symbols.size();
}

unsigned int KleiAnim::Binary::BuildReader::atlas_count() const
{
	return atlases.size();
}

unsigned int KleiAnim::Binary::BuildReader::vertex_count() const
{
	return BuildBase::vertices.size();
}

std::string KleiAnim::Binary::BuildReader::name() const
{
	return build_name;
}

const Common::SymbolNode& KleiAnim::Binary::BuildReader::symbol(const size_t i) const
{
	return symbols.at(i);
}

const Common::SymbolNode& KleiAnim::Binary::BuildReader::operator[](const size_t i) const
{
	return symbols[i];
}

const Common::AtlasNode& KleiAnim::Binary::BuildReader::atlas(const size_t i) const
{
	return atlases.at(i);
}

const Common::AlphaVertexNode& KleiAnim::Binary::BuildReader::vertex(const size_t i) const
{
	return BuildBase::vertices.at(i);
}

std::array<Common::AlphaVertexNode, 6> KleiAnim::Binary::BuildReader::vertices(const unsigned int start) const
{
	if (BuildBase::vertices.size() < (size_t(start) * size_t(6) + size_t(6)))
		throw std::out_of_range("并没有这么多组");

	std::array<Common::AlphaVertexNode, 6> ret;
	auto beg = BuildBase::vertices.data() + size_t(6) * size_t(start);
	auto d = ret.data();
	for (unsigned int i = 0; i < 6; i++)
		d[i] = beg[i];
	return ret;
}

const Common::BuildFrameNode& KleiAnim::Binary::BuildReader::frame(const size_t sym, const size_t i) const
{
	return symbols.at(sym).frames.at(i);
}

std::string KleiAnim::Binary::BuildReader::de_hash(const unsigned int hash) const
{
	return str_table.at(hash);
}

uint32_t KleiAnim::Binary::BuildReader::license_hash() const
{
	for (auto& hash : Common::common_license_hash)
	{
		auto it = this->str_table.find(hash);
		if (it != str_table.end()) return it->first;
	}
	return 0;
}

KleiAnim::Binary::AnimationWriter::AnimationWriter(const std::filesystem::path& out) :out(out)
{
	cc4 = valid_cc4;
	version = cur_version;
}

KleiAnim::Binary::AnimationWriter::AnimationWriter(const std::filesystem::path& out, const AnimationBase& base) :
	AnimationBase(base), out(out)
{}

KleiAnim::Binary::AnimationWriter::~AnimationWriter()
{
}

void KleiAnim::Binary::AnimationWriter::writefile()
{
	std::ofstream file(out, ios::binary | ios::trunc | ios::out);
	if (!file.is_open())
	{
		path out = ".\\默认输出\\anim-" + animations[0].name + ".bin";
		KleiAnimLog::write() << LOG("打开失败，指定的路径为:") << this->out << LOG("\n输出到默认路径:") << out;
		file.open(out, ios::binary | ios::trunc | ios::out);
		if (!file.is_open())
			throw std::system_error(std::make_error_code(std::errc(errno)));
	}
	writestream(file);
	file.close();
}

void KleiAnim::Binary::AnimationWriter::writestream(std::ostream& file)
{

	file.write("ANIM", 4);
	file.write(TO_CONST_PCHAR(cur_version), 4);

	unsigned int elem_total = 0, frame_total = 0, event_total = 0, anim_total;
	animations.size() > UINT32_MAX ? throw std::overflow_error("animations.size > UINT32_MAX") : anim_total = animations.size();
	//elem_total,frame_total,event_total字段写完文件之后再写入
	//写入12字节占位
	file.write("0123456789a", 12);

	//animations
	file.write(TO_PCHAR(anim_total), 4);
	for (auto& anim : animations)
	{
		write_str(anim.name, file);
		file.write(TO_PCHAR(anim.facing), 1);
		file.write(TO_PCHAR(anim.rootsym_hash), 4);
		file.write(TO_PCHAR(anim.frame_rate), 4);

		unsigned int frame_count;
		anim.frames.size() > UINT32_MAX ? throw std::overflow_error("anim.frames.size() > UINT32_MAX") : frame_count = anim.frames.size();
		frame_total += frame_count;

		//frame
		file.write(TO_PCHAR(frame_count), 4);
		for (auto& frame : anim.frames)
		{
			unsigned int event_count, elem_count;
			frame.events.size() > UINT32_MAX ? throw std::overflow_error("frame.events.size() > UINT32_MAX") : event_count = frame.events.size();
			frame.elements.size() > UINT32_MAX ? throw std::overflow_error("frame.elements.size() > UINT32_MAX") : elem_count = frame.elements.size();

			event_total += event_count;
			elem_total += elem_count;

			//xywh
			file.write(TO_CONST_PCHAR(frame.x), sizeof(float) * 4);

			//event
			file.write(TO_PCHAR(event_count), 4);
			if (event_total != 0)
				file.write(reinterpret_cast<char*>(frame.events.data()), size_t(4) * frame.events.size());

			//elmement
			file.write(TO_PCHAR(elem_count), 4);
			file.write(reinterpret_cast<char*>(frame.elements.data()), size_t(40) * frame.elements.size());
		}
	}

	//str table
	{
		unsigned int table_count;
		str_table.size() > UINT32_MAX ? throw std::overflow_error("str_table.size() > UINT32_MAX") : table_count = str_table.size();
		file.write(TO_PCHAR(table_count), 4);
		for (auto& pair : str_table)
		{
#undef max
			unsigned int strsize;
			pair.second.size() > std::numeric_limits<unsigned int>::max() ? throw std::overflow_error("写入字符串表出错，字符串过长") : strsize = pair.second.size();

			file.write(TO_CONST_PCHAR(pair.first), 4);

			file.write(TO_CONST_PCHAR(strsize), 4);
			file.write(pair.second.c_str(), pair.second.size());
		}
	}

	//插入之前跳过的字段
	file.seekp(8);
	file.write(TO_PCHAR(elem_total), 4);
	file.write(TO_PCHAR(frame_total), 4);
	file.write(TO_PCHAR(event_total), 4);

	file.flush();
}

void KleiAnim::Binary::AnimationWriter::add(Common::AnimationNode& anim)
{
	animations.push_back(anim);
}

KleiAnim::Binary::BuildWriter::BuildWriter(const std::filesystem::path& out) :out(out)
{
	cc4 = valid_cc4;
	version = cur_version;
}

KleiAnim::Binary::BuildWriter::BuildWriter(const std::filesystem::path& out, const BuildBase& base) :
	BuildBase(base), out(out)
{}

KleiAnim::Binary::BuildWriter::~BuildWriter()
{
}

void KleiAnim::Binary::BuildWriter::writefile()
{
	using std::ofstream;
	ofstream file(out, ofstream::binary | ofstream::trunc);
	if (!file.is_open())
	{
		path out = "./默认输出/build-" + build_name + ".bin";
		KleiAnimLog::write() << LOG("打开失败，指定的路径为:") << this->out << LOG("\n输出到默认路径:") << out;
		file.open(out, ofstream::binary | ofstream::trunc);
	}
	writestream(file);

	file.flush();
	file.close();
}

void KleiAnim::Binary::BuildWriter::writestream(std::ostream& file)
{
	this->symbol_count = 0;
	this->frame_count = 0;

	file.write("BILD", 4);
	file.write(TO_CONST_PCHAR(cur_version), 4);
	this->symbol_count = symbols.size();
	file.write(TO_CONST_PCHAR(this->symbol_count), 4);
	file.write(TO_CONST_PCHAR(this->frame_count), 4);
	write_str(build_name, file);

	//atlas
	{
		unsigned int atlas_count;
		atlases.size() > UINT32_MAX ? throw std::overflow_error("atlases.size() > UINT32_MAX") : atlas_count = atlases.size();
		file.write(TO_PCHAR(atlas_count), 4);
		for (auto& atlas : atlases)
		{
			write_str(atlas.name, file);
		}
	}

	//symbol
	for (auto& symbol : symbols)
	{
		unsigned frame_count;
		symbol.frames.size() > UINT32_MAX ? throw std::overflow_error("symbol.frames.size() > UINT32_MAX") : frame_count = symbol.frames.size();

		this->frame_count += frame_count;

		file.write(TO_PCHAR(symbol.name_hash), 4);
		file.write(TO_PCHAR(frame_count), 4);

		//frame
		for (auto& frame : symbol.frames)
		{
			file.write(TO_PCHAR(frame), 32);
		}
	}

	//alpha vertex
	{
		unsigned int vertex_count;
		vertices.size() > UINT32_MAX ? throw std::overflow_error("vertices.size() > UINT32_MAX") : vertex_count = vertices.size();

		file.write(TO_PCHAR(vertex_count), 4);
		for (auto& avn : vertices)
		{
			file.write(TO_PCHAR(avn), 24);
		}
	}

	//str table
	{
		unsigned int table_count;
		str_table.size() > UINT32_MAX ? throw std::overflow_error("str_table.size() > UINT32_MAX") : table_count = str_table.size();

		file.write(TO_PCHAR(table_count), 4);
		for (auto& pair : str_table)
		{
			unsigned int strsize;
			pair.second.size() > UINT32_MAX ? throw std::overflow_error("写入字符串表出错，字符串过长") : strsize = pair.second.size();

			file.write(TO_CONST_PCHAR(pair.first), 4);

			file.write(TO_CONST_PCHAR(strsize), 4);
			file.write(pair.second.c_str(), pair.second.size());
		}
	}

	file.seekp(12);
	file.write(TO_PCHAR(this->frame_count), 4);
}

void KleiAnim::Binary::BuildWriter::add(Common::SymbolNode& sym)
{
	BuildBase::symbols.push_back(sym);
	if (sym.frames.size() >= UINT32_MAX - frame_count)
		throw std::out_of_range("too much frames");
	frame_count += sym.frames.size();
}

void KleiAnim::Binary::BuildWriter::add(Common::AlphaVertexNode& vert)
{
	BuildBase::vertices.push_back(vert);
}

void KleiAnim::Binary::BuildWriter::add(Common::AtlasNode& atlas)
{
	BuildBase::atlases.push_back(atlas);
}

void KleiAnim::Binary::BuildWriter::add(const std::array<Common::AlphaVertexNode, 6> & vertices)
{
	for (auto& vert : vertices)
	{
		BuildBase::vertices.push_back(vert);
	}
}

void KleiAnim::Binary::BuildWriter::add_hashstringpair(unsigned int hash, std::string&& string)
{
	this->str_table.insert(std::make_pair(hash, string));
}

void KleiAnim::Binary::BuildWriter::add_hashstringpair(unsigned int hash, std::string& string)
{
	this->str_table.insert(std::make_pair(hash, string));
}

void KleiAnim::Binary::AnimationWriter::add_hashstringpair(unsigned int hash, std::string&& string)
{
	this->str_table.insert(std::make_pair(hash, string));
}

void KleiAnim::Binary::AnimationWriter::add_hashstringpair(unsigned int hash, std::string& string)
{
	this->str_table.insert(std::make_pair(hash, string));
}

std::wstring KleiAnim::Common::BuildBase::ToString()
{
	using std::wostringstream;
	wostringstream o;

	o << L"Build名：" << build_name.c_str() << L'\n'
		<< L"符号数量：" << symbol_count << L'\n'
		<< L"帧数量：" << frame_count << L'\n'
		<< L"atlas：" << L"\n";

	for (auto& atlas : atlases)
	{
		o << L"  " << atlas.name.c_str() << L'\n';
	}

	return o.str();
}

BinaryType KleiAnim::Binary::CheckFileType(std::filesystem::path filepath)
{
	using std::ifstream;
	ifstream ifs;
	ifs.setf(ifstream::binary | ifstream::in);
	ifs.exceptions(ifstream::badbit | ifstream::failbit | ifstream::eofbit);
	ifs.open(filepath);
	using std::cerr;

	if (!ifs.is_open())
		cerr<<"打开失败";

	constexpr unsigned int bild = 0x444C4942;
	constexpr unsigned int anim = 0x4D494E41;

	unsigned int readout = 0;


	try
	{
		ifs.read(TO_PCHAR(readout), 4);
	}
	catch (const ifstream::failure& fail)
	{
		ifs.close();
		cerr << fail.what();
		return BinaryType::Neither;
	}
	catch (std::exception& e)
	{
		ifs.close();
		cerr << e.what();
	}

	ifs.close();

	if (readout == bild)
		return BinaryType::Build;
	if (readout == anim)
		return BinaryType::Animation;
	
	return BinaryType::Neither;
}
