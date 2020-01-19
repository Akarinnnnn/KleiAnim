#pragma once
#include <filesystem>
namespace KleiAnim::Tool
{
	__declspec(dllexport) void decompile_scml(std::filesystem::path animation, std::filesystem::path build);
}