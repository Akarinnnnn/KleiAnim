#include "arg_parser.h"
#include <utility>
#include <iostream>
using std::wstring;

ArgumentParser::ArgumentParser()
{
}

void ArgumentParser::SetHelpMessage(const wchar_t* message)
{
	helpmsg = message;
}

void ArgumentParser::AddInteger(std::wstring name)
{
	integers.push_back(kv<size_t>{name});
}

void ArgumentParser::AddString(std::wstring name)
{
	strings.push_back(kv<std::wstring>{name, wstring(20,L'\0')});
}

size_t ArgumentParser::GetInteger(std::wstring name)
{
	for (auto& pair : integers)
		if (pair.k == name)
			return pair.v;
	return 0;
}

const std::wstring& ArgumentParser::GetString(std::wstring name)
{
	static std::wstring empty = L"";
	for (auto& pair : strings)
		if (pair.k == name)
			return pair.v;
	return empty;
}

void ArgumentParser::Parse(int argc, wchar_t** argv)
{
	using namespace std::string_literals;
	if (argv[0] == L"/h"s || argv[0] == L"-h"s || argv[0] == L"/help"s || argv[0] == L"--help"s || argc == 1)
	{
		std::wcout << helpmsg << std::endl;
		system("pause");
		exit(0);
	}


	for (wchar_t **it =argv + 1;it != argv + argc;it++)
	{
#pragma warning(push)
#pragma warning (disable:6289)
		if (*it[0] == L'/' || *it[0] == L'-')
		{
			wstring name = (*it) + 1;
			it++;
			wstring value = *it;
			for (auto& integer : integers)
			{
				if (integer.k == name) integer.v = std::stoull(value);
				continue;
			}
			for (auto& string : strings)
			{
				if (string.k == name) string.v = value;
				continue;
			}
		} 
		else continue;
#pragma warning(pop)
	}
}
