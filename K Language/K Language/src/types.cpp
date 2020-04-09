#include "types.h"

namespace klang::type
{
	static Value* FIRST_VALUE = nullptr;

	void Value::attachValue(Value* value)
	{
		if (FIRST_VALUE)
		{
			value->_next = FIRST_VALUE;
			FIRST_VALUE->_prev = value;
			FIRST_VALUE = value;
		}
		else
		{
			value->_next = value->_prev = nullptr;
			FIRST_VALUE = value;
		}
	}

	void Value::detachValue(Value* value)
	{
		if (value == FIRST_VALUE)
		{
			FIRST_VALUE = value->_next;
			if(FIRST_VALUE)
				FIRST_VALUE->_prev = nullptr;
		}
		else
		{
			if (value->_next)
				value->_next->_prev = value->_prev;
			if (value->_prev)
				value->_prev->_next = value->_next;
			value->_next = value->_prev = nullptr;
		}
	}
}

namespace klang::type
{
	Value::Value() :
		_refs{ 0 },
		_next{ nullptr }
	{
		attachValue(this);
	}
	Value::~Value()
	{
		detachValue(this);
	}

	void Value::incref() { _refs++; }
	void Value::decref() { _refs--; }

	unsigned int Value::narg() const { return 1; }
	Value* Value::arg0() { return this; }
	Value* Value::arg(unsigned int index) { return index == 0 ? this : nullptr; }
}



// Undefined //
namespace klang::type
{
	Undefined::Undefined() :
		Value()
	{}

	Undefined::~Undefined() {}

	Undefined::Type Undefined::klangType() { return Type::Undefined; }

//To c++ conversions
	Undefined::operator Int32() const ;
	Undefined::operator Int64() const ;
	Undefined::operator float() const ;
	Undefined::operator double() const ;
	Undefined::operator bool() const ;
	Undefined::operator std::string() const ;
	Undefined::operator std::vector<Value*>() const ;
	Undefined::operator std::unordered_map<std::string, Value*>() const ;

//Common operators
	Value* Undefined::klang_operatorEquals(Value* value) ;
	Value* Undefined::klang_operatorNotEquals(Value* value) ;
	Value* Undefined::klang_operatorGreater(Value* value) ;
	Value* Undefined::klang_operatorLess(Value* value) ;
	Value* Undefined::klang_operatorGreaterEquals(Value* value) ;
	Value* Undefined::klang_operatorLessEquals(Value* value) ;
	Value* Undefined::klang_operatorNot() ;

//Math operators
	Value* Undefined::klang_operatorPlus(Value* value) ;
	Value* Undefined::klang_operatorMinus(Value* value) ;
	Value* Undefined::klang_operatorMultiply(Value* value) ;
	Value* Undefined::klang_operatorDivide(Value* value) ;
	Value* Undefined::klang_operatorModule(Value* value) ;
	Value* Undefined::klang_operatorIncrease() ;
	Value* Undefined::klang_operatorDecrease() ;
	Value* Undefined::klang_operatorNegative() ;

//bit operators
	Value* Undefined::klang_operatorBitwiseLeft(Value* value) ;
	Value* Undefined::klang_operatorBitwiseRight(Value* value) ;
	Value* Undefined::klang_operatorBitwiseAnd(Value* value) ;
	Value* Undefined::klang_operatorBitwiseOr(Value* value) ;
	Value* Undefined::klang_operatorBitwiseXor(Value* value) ;
	Value* Undefined::klang_operatorBitwiseNot() ;

//Array/List operators
	Value* Undefined::klang_operatorArrayGet(Value* index) ;
	void Undefined::klang_operatorArraySet(Value* index, Value* value) ;

//Object operators
	Value* Undefined::klang_operatorGetProperty(const std::string& name) ;
	void Undefined::klang_operatorGetProperty(const std::string& name, Value* value) ;
	Value* Undefined::klang_operatorCall(Value* self) ;
	Value* Undefined::klang_operatorCall(Value* self, Value* arg0) ;
	Value* Undefined::klang_operatorCall(Value* self, Value* arg0, Value* arg1) ;
	Value* Undefined::klang_operatorCall(Value* self, Value* arg0, Value* arg1, Value* arg2) ;
	Value* Undefined::klang_operatorCall(Value* self, Variadic&& args) ;

//Reference operators
	Value* Undefined::klang_operatorReferenceGet() ;
	void Undefined::klang_operatorReferenceSet(Value* value) ;

//Iterator operators
	Value* Undefined::klang_operatorIterator() ;
	Value* Undefined::klang_operatorHasNext() ;
	Value* Undefined::klang_operatorNext() ;
}
