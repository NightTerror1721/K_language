#include "types.h"

#include <sstream>


namespace klang::type
{
	static inline std::string UnsupportedException_GenMessage(const Value& value, const std::string& operationName)
	{
		return "Klang " + GetTypeName(value.klangType()) + " value not support " + operationName + " operation.";
	}

	UnsupportedException::UnsupportedException(const Value& value, const std::string& operationName) noexcept :
		KlangException{ UnsupportedException_GenMessage(value, operationName) }
	{}
}

namespace klang::type
{
	unsigned int Value::narg() const { return 1; }
	Value* Value::arg0() { return this; }
	Value* Value::arg(unsigned int index) { return index == 0 ? this : nullptr; }

	std::string Value::getKlangTypeName() const { return GetTypeName(type); }

	Value::operator std::string() const
	{
		std::stringstream ss;
		ss << GetTypeName(klangType()) << "::" << this;
		return ss.str();
	}
	Value::operator ValueVector() const { return { const_cast<Value*>(this) }; };
	Value::operator ValueMap() const { return { { "scalar", const_cast<Value*>(this) } }; }
	 
	Value* Value::klang_operatorEquals(Value* value) { return this == value ? constant::True : constant::False; }
	Value* Value::klang_operatorNotEquals(Value* value) { return this != value ? constant::True : constant::False; }
	Value* Value::klang_operatorGreater(Value* value) { throw UnsupportedException{ *this, "klang_operatorGreater" }; }
	Value* Value::klang_operatorLess(Value* value) { throw UnsupportedException{ *this, "klang_operatorLess" }; }
	Value* Value::klang_operatorGreaterEquals(Value* value) { throw UnsupportedException{ *this, "klang_operatorGreaterEquals" }; }
	Value* Value::klang_operatorLessEquals(Value* value) { throw UnsupportedException{ *this, "klang_operatorLessEquals" }; }
	Value* Value::klang_operatorNot() { return static_cast<bool>(*this) ? constant::False : constant::True; }

//Math operators
	Value* Value::klang_operatorPlus(Value* value) { throw UnsupportedException{ *this, "klang_operatorPlus" }; }
	Value* Value::klang_operatorMinus(Value* value) { throw UnsupportedException{ *this, "klang_operatorMinus" }; }
	Value* Value::klang_operatorMultiply(Value* value) { throw UnsupportedException{ *this, "klang_operatorMultiply" }; }
	Value* Value::klang_operatorDivide(Value* value) { throw UnsupportedException{ *this, "klang_operatorDivide" }; }
	Value* Value::klang_operatorModule(Value* value) { throw UnsupportedException{ *this, "klang_operatorModule" }; }
	Value* Value::klang_operatorIncrease() { throw UnsupportedException{ *this, "klang_operatorIncrease" }; }
	Value* Value::klang_operatorDecrease() { throw UnsupportedException{ *this, "klang_operatorDecrease" }; }
	Value* Value::klang_operatorNegative() { throw UnsupportedException{ *this, "klang_operatorNegative" }; }

//bit operators
	Value* Value::klang_operatorBitwiseLeft(Value* value) { throw UnsupportedException{ *this, "klang_operatorBitwiseLeft" }; }
	Value* Value::klang_operatorBitwiseRight(Value* value) { throw UnsupportedException{ *this, "klang_operatorBitwiseRight" }; }
	Value* Value::klang_operatorBitwiseAnd(Value* value) { throw UnsupportedException{ *this, "klang_operatorBitwiseAnd" }; }
	Value* Value::klang_operatorBitwiseOr(Value* value) { throw UnsupportedException{ *this, "klang_operatorBitwiseOr" }; }
	Value* Value::klang_operatorBitwiseXor(Value* value) { throw UnsupportedException{ *this, "klang_operatorBitwiseXor" }; }
	Value* Value::klang_operatorBitwiseNot() { throw UnsupportedException{ *this, "klang_operatorBitwiseNot" }; }

//Array/List operators
	Value* Value::klang_operatorArrayGet(Value* index) { throw UnsupportedException{ *this, "klang_operatorArrayGet" }; }
	void Value::klang_operatorArraySet(Value* index, Value* value) { throw UnsupportedException{ *this, "klang_operatorArraySet" }; }

//Object operators
	Value* Value::klang_operatorGetProperty(const std::string& name) { throw UnsupportedException{ *this, "klang_operatorGetProperty" }; }
	void Value::klang_operatorGetProperty(const std::string& name, Value* value) { throw UnsupportedException{ *this, "klang_operatorGetProperty" }; }
	/*virtual Value* klang_operatorCall(Value* self);
	virtual Value* klang_operatorCall(Value* self, Value* arg0);
	virtual Value* klang_operatorCall(Value* self, Value* arg0, Value* arg1);
	virtual Value* klang_operatorCall(Value* self, Value* arg0, Value* arg1, Value* arg2);
	virtual Value* klang_operatorCall(Value* self, Variadic&& args);*/

//Reference operators
	Value* Value::klang_operatorReferenceGet() { throw UnsupportedException{ *this, "klang_operatorReferenceGet" }; }
	void Value::klang_operatorReferenceSet(Value* value) { throw UnsupportedException{ *this, "klang_operatorReferenceSet" }; }

//Iterator operators
	Value* Value::klang_operatorIterator() { throw UnsupportedException{ *this, "klang_operatorIterator" }; }
	Value* Value::klang_operatorHasNext() { throw UnsupportedException{ *this, "klang_operatorHasNext" }; }
	Value* Value::klang_operatorNext() { throw UnsupportedException{ *this, "klang_operatorNext" }; }

	std::string GetTypeName(Value::Type type)
	{
		using Type = Value::Type;
		switch (type)
		{
			case Type::Undefined: return "undefined";
			case Type::Integer: return "integer";
			case Type::Float: return "float";
			case Type::Boolean: return "boolean";
			case Type::String: return "string";
			case Type::Function: return "function";
			case Type::Reference: return "reference";
			case Type::Array: return "array";
			case Type::List: return "list";
			case Type::Object: return "object";
		}
		return "";
	}
}




namespace klang::type
{
	template<class _Ty>
	_Ty* static_create()
	{
		_Ty* ptr = reinterpret_cast<_Ty*>(klang::heap::s_malloc(sizeof(_Ty)));
		if (ptr)
			::new(ptr) _Ty();
		return ptr;
	}

	template<class _Ty, typename _Arg0>
	_Ty* static_create(const _Arg0& arg0)
	{
		_Ty* ptr = reinterpret_cast<_Ty*>(klang::heap::s_malloc(sizeof(_Ty)));
		if (ptr)
			::new(ptr) _Ty(arg0);
		return ptr;
	}
}

#define BOOL(_Expr) static_cast<bool>(_Expr)
#define INT32(_Expr) static_cast<klang::Int32>(_Expr)
#define INT64(_Expr) static_cast<klang::Int64>(_Expr)
#define FLOAT(_Expr) static_cast<float>(_Expr)
#define DOUBLE(_Expr) static_cast<double>(_Expr)
#define STRING(_Expr) static_cast<std::string>(_Expr)

#define TRUE klang::type::constant::True
#define FALSE klang::type::constant::False
#define UNDEFINED klang::type::constant::Undefined

#define BOOL_TEST(_Expr) (BOOL((_Expr)) ? TRUE : FALSE)
#define BOOL_NOT_TEST(_Expr) (BOOL((_Expr)) ? FALSE : TRUE)





// Undefined //
namespace klang::type
{
	Undefined::Undefined() :
		Value( Type::Undefined )
	{}

	Undefined::~Undefined() {}

	Undefined::operator Int32() const { return 0; }
	Undefined::operator Int64() const { return 0; }
	Undefined::operator float() const { return 0; }
	Undefined::operator double() const { return 0; }
	Undefined::operator bool() const { return false; }
	Undefined::operator std::string() const { return "undefined"; }

	Value* Undefined::klang_operatorNot() { return constant::True; }

	void* Undefined::operator new(size_t size) { return Instance; }
	void Undefined::operator delete(void* p) {}

	Undefined* const Undefined::Instance = static_create<Undefined>();
}





// Boolean //
namespace klang::type
{
	Boolean::Boolean(const bool value) :
		Value{ Type::Boolean },
		_value{ value }
	{}
	Boolean::~Boolean() {}

	Boolean::operator Int32() const { return _value; }
	Boolean::operator Int64() const { return _value; }
	Boolean::operator float() const { return _value; }
	Boolean::operator double() const { return _value; }
	Boolean::operator bool() const { return _value; }
	Boolean::operator std::string() const { return _value ? "true" : "false"; }

	Value* Boolean::klang_operatorEquals(Value* value) { return BOOL_TEST(_value == BOOL(*value)); }
	Value* Boolean::klang_operatorNotEquals(Value* value) { return BOOL_TEST(_value != BOOL(*value)); }
	Value* Boolean::klang_operatorGreater(Value* value) { return BOOL_TEST(INT32(_value) > INT32(BOOL(*value))); }
	Value* Boolean::klang_operatorLess(Value* value) { return BOOL_TEST(INT32(_value) < INT32(BOOL(*value))); }
	Value* Boolean::klang_operatorGreaterEquals(Value* value) { return BOOL_TEST(INT32(_value) >= INT32(BOOL(*value))); }
	Value* Boolean::klang_operatorLessEquals(Value* value) { return BOOL_TEST(INT32(_value) <= INT32(BOOL(*value))); }
	Value* Boolean::klang_operatorNot() { return BOOL_NOT_TEST(_value); }

	Value* Boolean::klang_operatorPlus(Value* value) { return newDouble(DOUBLE(_value) + DOUBLE(*value)); }
	Value* Boolean::klang_operatorMinus(Value* value) { return newDouble(DOUBLE(_value) - DOUBLE(*value)); }
	Value* Boolean::klang_operatorMultiply(Value* value) { return newDouble(DOUBLE(_value) * DOUBLE(*value)); }
	Value* Boolean::klang_operatorDivide(Value* value) { return newDouble(DOUBLE(_value) / DOUBLE(*value)); }
	Value* Boolean::klang_operatorModule(Value* value) { return newLongInteger(INT64(_value) % INT64(*value)); }
	Value* Boolean::klang_operatorIncrease() { return newInteger(INT32(_value) + 1); }
	Value* Boolean::klang_operatorDecrease() { return newInteger(INT32(_value) - 1); }
	Value* Boolean::klang_operatorNegative() { return newInteger(-INT32(_value)); }

	Value* Boolean::klang_operatorBitwiseLeft(Value* value) { return newLongInteger(INT64(_value) << INT64(*value)); }
	Value* Boolean::klang_operatorBitwiseRight(Value* value) { return newLongInteger(INT64(_value) >> INT64(*value)); }
	Value* Boolean::klang_operatorBitwiseAnd(Value* value) { return newLongInteger(INT64(_value) & INT64(*value)); }
	Value* Boolean::klang_operatorBitwiseOr(Value* value) { return newLongInteger(INT64(_value) | INT64(*value)); }
	Value* Boolean::klang_operatorBitwiseXor(Value* value) { return newLongInteger(INT64(_value) ^ INT64(*value)); }
	Value* Boolean::klang_operatorBitwiseNot() { return newInteger(~INT32(_value)); }

	void* Boolean::operator new(size_t size, const bool value) { return value ? True : False; }
	void Boolean::operator delete(void* p) {}

	Boolean* const Boolean::True = static_create<Boolean>(true);
	Boolean* const Boolean::False = static_create<Boolean>(false);
}







namespace klang::type::constant
{
	extern Value* const Undefined = Undefined::Instance;
	extern Value* const True = Boolean::True;
	extern Value* const False = Boolean::False;
}

std::ostream& operator<< (std::ostream& os, const klang::type::Value& value)
{
	return os << static_cast<std::string>(value);
}
