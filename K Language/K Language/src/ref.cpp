#include "ref.h"

#include <utility>
#include <iostream>

namespace klang
{
	using namespace type;
	namespace ct = constant;

	Ref::Ref(Value* value) noexcept :
		_value{ value ? value : ct::Undefined }
	{
		if (value)
			heap::incref(value);
	}
	Ref::Ref(const Value* value) noexcept :
		Ref{ const_cast<Value*>(value) }
	{}
	Ref::Ref(const Ref& ref) noexcept :
		_value{ ref._value }
	{
		heap::incref(ref._value);
	}
	Ref::Ref(Ref&& ref) noexcept :
		_value{ std::move(ref._value) }
	{
		ref._value = ct::Undefined;
	}
	Ref::~Ref()
	{
		heap::decref(_value);
	}

	Ref& Ref::operator= (Value* value) noexcept
	{
		heap::decref(_value);
		_value = value;
		heap::incref(_value);
		return *this;
	}
	Ref& Ref::operator= (const Ref& ref) noexcept
	{
		heap::decref(_value);
		_value = ref._value;
		heap::incref(_value);
		return *this;
	}
	Ref& Ref::operator= (Ref&& ref) noexcept
	{
		heap::decref(_value);
		_value = std::move(ref._value);
		ref._value = ct::Undefined;
		return *this;
	}

	Value::Type Ref::type() const { return _value->type; }

	Ref::operator klang::type::Value* () const { return _value; }
	Ref::operator const klang::type::Value* () const { return _value; }

	Value* Ref::operator-> () { return _value; }
	const Value* Ref::operator-> () const { return _value; }





	Ref::Ref(decltype(nullptr)) : Ref{ ct::Undefined } {}
	Ref& Ref::operator= (decltype(nullptr)) { return *this = ct::Undefined; }


	Ref::Ref(const bool value) : Ref{ value ? ct::True : ct::False } {}
	Ref& Ref::operator= (const bool value) { return *this = value ? ct::True : ct::False; }
	Ref::operator bool() const { return static_cast<bool>(*_value); }


	Ref::Ref(const Int32 value) : Ref{ newInteger(value) } {}
	Ref::Ref(const UInt32 value) : Ref{ newInteger(static_cast<Int32>(value)) } {}
	Ref& Ref::operator= (const Int32 value) { return *this = newInteger(value); }
	Ref& Ref::operator= (const UInt32 value) { return *this = newInteger(static_cast<Int32>(value)); }
	Ref::operator Int32() const { return static_cast<Int32>(*_value); }
	Ref::operator UInt32() const { return static_cast<UInt32>(static_cast<Int32>(*_value)); }


	Ref::Ref(const Int64 value) : Ref{ newLongInteger(value) } {}
	Ref::Ref(const UInt64 value) : Ref{ newLongInteger(static_cast<Int64>(value)) } {}
	Ref& Ref::operator= (const Int64 value) { return *this = newLongInteger(value); }
	Ref& Ref::operator= (const UInt64 value) { return *this = newLongInteger(static_cast<Int64>(value)); }
	Ref::operator Int64() const { return static_cast<Int64>(*_value); }
	Ref::operator UInt64() const { return static_cast<UInt64>(static_cast<Int64>(*_value)); }


	Ref::Ref(const float value) : Ref{ newFloat(value) } {}
	Ref& Ref::operator= (const float value) { return *this = newFloat(value); }
	Ref::operator float() const { return static_cast<float>(*_value); }


	Ref::Ref(const double value) : Ref{ newDouble(value) } {}
	Ref& Ref::operator= (const double value) { return *this = newDouble(value); }
	Ref::operator double() const { return static_cast<double>(*_value); }
}

std::ostream& operator<< (std::ostream& os, const klang::Ref& ref)
{
	return os << *static_cast<const klang::type::Value*>(ref);
}

