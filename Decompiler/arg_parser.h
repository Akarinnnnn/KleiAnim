#pragma once
#include <string>
#include <vector>

/// <summary>
/// 参数起始符：L'/'和L'-'
/// </summary>
class ArgumentParser
{
public:
	template<typename StringIterator> struct str_range
	{
		str_range(StringIterator begin,StringIterator end):_beg(begin),_end(end)
		{
			static_assert(std::is_same<StringIterator::value_type, std::wstring>::value);
		}
		
		StringIterator begin() { return _beg; }
		StringIterator end() { return _end; }
		StringIterator _beg, _end;
	};
	template<> struct str_range<std::wstring*>
	{
		str_range(std::wstring* begin, std::wstring* end) :_beg(begin), _end(end) {}
		std::wstring* begin() { return _beg; }
		std::wstring* end() { return _end; }
		std::wstring *_beg, *_end;
	};
	template <typename Iter,typename ValueType> struct range
	{
		range(Iter begin,Iter end):_beg(begin),_end(end) 
		{
			static_assert(std::is_same<Iter::value_type, ValueType>::value);
		}

		Iter begin() { return _beg; }
		Iter end() { return _end; }

		Iter _beg, _end;
	};
	template <typename T> struct kv
	{
		std::wstring k;
		T v;
	};
	ArgumentParser();
	/*template <typename EnumIt,typename Enum>
	void AddEnum(std::wstring& name, range<EnumIt,kv<Enum>> options)
	{
		for (auto& option : options)
		{
			switches.push_back(kv<kv<size_t>>{name, { option.k, (size_t)option.v }});
		}
	}*/

	void SetHelpMessage(const wchar_t* message);

	void AddInteger(std::wstring name);
	void AddString(std::wstring name);

	size_t GetInteger(std::wstring name);
	const std::wstring& GetString(std::wstring name);

	void Parse(int argc, wchar_t** argv);
	/*template<typename T> T GetEnum(std::wstring& name)
	{
		for (auto& kv : switches)
		{

		}
	}*/
private:
	//std::vector<kv<kv<size_t>>> switches;
	const wchar_t * helpmsg;
	std::vector<kv<size_t>> integers;
	std::vector<kv<std::wstring>> strings;
};

