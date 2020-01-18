#pragma once
#include <exception>
#include "export.h"
namespace KleiAnim
{
	namespace Exception
	{
		class EXPORT_API invalid_file final : public std::exception
		{
		public:
			invalid_file() noexcept;
			invalid_file(const char* message) noexcept;
			invalid_file(const char* message, unsigned int cc4, unsigned short version) noexcept;
			~invalid_file();

			unsigned int get_cc4() noexcept;
			unsigned int get_version() noexcept;

			const char* what() const noexcept override;

		private:
			unsigned int cc4;
			unsigned short version;
			char msg[128]{0};
		};
	}
}