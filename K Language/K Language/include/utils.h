#pragma once

#include <cstdint>
#include <string>
#include <exception>

namespace klang
{
	typedef std::int8_t Int8;
	typedef std::int16_t Int16;
	typedef std::int32_t Int32;
	typedef std::int64_t Int64;

	typedef std::uint8_t UInt8;
	typedef std::uint16_t UInt16;
	typedef std::uint32_t UInt32;
	typedef std::uint64_t UInt64;

	typedef UInt8 Byte;
	typedef UInt16 Word;
	typedef UInt32 Long;
	typedef UInt64 Quad;
}

namespace klang
{
	class KlangException : public std::exception
	{
	public:
		KlangException() noexcept;
		KlangException(const char* const message) noexcept;
		KlangException(const std::string& message) noexcept;
		KlangException(const KlangException&) = default;
		~KlangException() = default;

		KlangException& operator= (const KlangException&) = default;
	};
}

