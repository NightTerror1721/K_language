#include "utils.h"

namespace klang
{
	KlangException::KlangException() noexcept :
		exception{}
	{}
	KlangException::KlangException(const char* const message) noexcept :
		exception{ message }
	{}
	KlangException::KlangException(const std::string& message) noexcept :
		exception(message.c_str())
	{}
}
