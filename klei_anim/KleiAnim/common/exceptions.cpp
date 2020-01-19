#include "../../pch.h"
#include "exceptions.hpp"
#include <cstdio>

using namespace KleiAnim::Exception;
invalid_file::invalid_file() noexcept : std::exception("文件格式不正确，具体原因未知")
{
	cc4 = 0, version = 0;
}

KleiAnim::Exception::invalid_file::invalid_file(const char* message) noexcept :exception("")
{
	cc4 = 0, version = 0;
	sprintf_s(msg, "文件格式不正确：%s", message);
}

invalid_file::invalid_file(const char* message, unsigned int cc4, unsigned short version) noexcept :cc4(cc4), version(version), exception("")
{
	sprintf_s(msg, "文件格式不正确：%s，cc4 = 0x%08X,版本为%d", message, cc4, version);
}

invalid_file::~invalid_file()
{

}

unsigned int KleiAnim::Exception::invalid_file::get_cc4() noexcept
{
	return this->cc4;
}

unsigned int KleiAnim::Exception::invalid_file::get_version() noexcept
{
	return this->version;
}

const char* KleiAnim::Exception::invalid_file::what() const noexcept
{
	if (msg[0] != '\0')
	{
		return msg;
	}
	else
	{
		return exception::what();
	}
}
