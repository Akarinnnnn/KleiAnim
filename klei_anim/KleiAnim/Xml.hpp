/***************************
* Encoding: GB2312
* 对XML形式的动画进行读写
***************************/
#pragma once
#include "Binary.hpp"
namespace KleiAnim
{

	void EXPORT_API AnimBin2XML(std::filesystem::path binary, std::filesystem::path xmlpath);
	void EXPORT_API AnimBin2XML(Binary::AnimationReader& binary, std::filesystem::path xmlpath);
	void EXPORT_API AnimBin2XML(Common::AnimationBase& binary, std::filesystem::path xmlpath);

	void EXPORT_API BuildBin2XML(std::filesystem::path binary, std::filesystem::path xmlpath);
	void EXPORT_API BuildBin2XML(Common::BuildBase& binary, std::filesystem::path xmlpath);
	void EXPORT_API BuildBin2XML(Binary::BuildReader& binary, std::filesystem::path xmlpath);

	void 检查许可(KleiAnim::Binary::BuildReader& binary);

	void EXPORT_API XML2Bin(std::filesystem::path xmlpath, std::filesystem::path outdir);


	extern "C" EXPORT_API void AnimBin2XML(const char8_t* in_path, const char8_t* out_path);
	extern "C" EXPORT_API void BuildBin2XML(const char8_t* in_path, const char8_t* out_path);
	extern "C" EXPORT_API void XML2Bin(const char8_t* in_path, const char8_t* out_path);
}