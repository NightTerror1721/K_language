#pragma once

#include <type_traits>
#include <vector>
#include <unordered_map>

#include "utils.h"

namespace klang::type
{
	class Value;



	class Variadic
	{
	protected:
		constexpr Variadic() = default;

	public:
		virtual unsigned int narg() const ;
		virtual Value* arg0() ;
		virtual Value* arg(unsigned int index) ;
	};



	class Value : public Variadic
	{
	public:
		enum class Type
		{
			Undefined,

			Integer,
			Float,
			Boolean,

			String,

			Function,

			Reference,

			Array,

			List,

			Object
		};

	private:
		unsigned int _refs;
		Value *_next, *_prev;

	protected:
		Value();
		virtual ~Value();

	public:
		void incref();
		void decref();

	public:
		virtual Type klangType() = 0;

	public: //To c++ conversions
		virtual operator Int32() const = 0;
		virtual operator Int64() const = 0;
		virtual operator float() const = 0;
		virtual operator double() const = 0;
		virtual operator bool() const = 0;
		virtual operator std::string() const = 0;
		virtual operator std::vector<Value*>() const = 0;
		virtual operator std::unordered_map<std::string, Value*>() const = 0;

	public: //Common operators
		virtual Value* klang_operatorEquals(Value* value) = 0;
		virtual Value* klang_operatorNotEquals(Value* value) = 0;
		virtual Value* klang_operatorGreater(Value* value) = 0;
		virtual Value* klang_operatorLess(Value* value) = 0;
		virtual Value* klang_operatorGreaterEquals(Value* value) = 0;
		virtual Value* klang_operatorLessEquals(Value* value) = 0;
		virtual Value* klang_operatorNot() = 0;

	public: //Math operators
		virtual Value* klang_operatorPlus(Value* value) = 0;
		virtual Value* klang_operatorMinus(Value* value) = 0;
		virtual Value* klang_operatorMultiply(Value* value) = 0;
		virtual Value* klang_operatorDivide(Value* value) = 0;
		virtual Value* klang_operatorModule(Value* value) = 0;
		virtual Value* klang_operatorIncrease() = 0;
		virtual Value* klang_operatorDecrease() = 0;
		virtual Value* klang_operatorNegative() = 0;

	public: //bit operators
		virtual Value* klang_operatorBitwiseLeft(Value* value) = 0;
		virtual Value* klang_operatorBitwiseRight(Value* value) = 0;
		virtual Value* klang_operatorBitwiseAnd(Value* value) = 0;
		virtual Value* klang_operatorBitwiseOr(Value* value) = 0;
		virtual Value* klang_operatorBitwiseXor(Value* value) = 0;
		virtual Value* klang_operatorBitwiseNot() = 0;

	public: //Array/List operators
		virtual Value* klang_operatorArrayGet(Value* index) = 0;
		virtual void klang_operatorArraySet(Value* index, Value* value) = 0;

	public: //Object operators
		virtual Value* klang_operatorGetProperty(const std::string& name) = 0;
		virtual void klang_operatorGetProperty(const std::string& name, Value* value) = 0;
		virtual Value* klang_operatorCall(Value* self) = 0;
		virtual Value* klang_operatorCall(Value* self, Value* arg0) = 0;
		virtual Value* klang_operatorCall(Value* self, Value* arg0, Value* arg1) = 0;
		virtual Value* klang_operatorCall(Value* self, Value* arg0, Value* arg1, Value* arg2) = 0;
		virtual Value* klang_operatorCall(Value* self, Variadic&& args) = 0;

	public: //Reference operators
		virtual Value* klang_operatorReferenceGet() = 0;
		virtual void klang_operatorReferenceSet(Value* value) = 0;

	public: //Iterator operators
		virtual Value* klang_operatorIterator() = 0;
		virtual Value* klang_operatorHasNext() = 0;
		virtual Value* klang_operatorNext() = 0;

	public:
		inline bool isUndefined() { return klangType() == Type::Undefined; }
		inline bool isInteger() { return klangType() == Type::Integer; }
		inline bool isFloat() { return klangType() == Type::Float; }
		inline bool isBoolean() { return klangType() == Type::Boolean; }
		inline bool isString() { return klangType() == Type::String; }
		inline bool isFunction() { return klangType() == Type::Function; }
		inline bool isReference() { return klangType() == Type::Reference; }
		inline bool isArray() { return klangType() == Type::Array; }
		inline bool isList() { return klangType() == Type::List; }
		inline bool isObject() { return klangType() == Type::Object; }


	public:
		unsigned int narg() const override;
		Value* arg0() override;
		Value* arg(unsigned int index) override;

	public:
		template<class _Ty>
		inline _Ty& as()
		{
			static_assert(std::is_base_of<Value, _Ty>::value);
			return *reinterpret_cast<_Ty*>(this);
		}

		template<class _Ty>
		inline const _Ty& as() const
		{
			static_assert(std::is_base_of<Value, _Ty>::value);
			return *reinterpret_cast<const _Ty*>(this);
		}

	private:
		static void attachValue(Value* value);
		static void detachValue(Value* value);
	};



	class Undefined : public Value
	{
	private:
		Undefined();

	public:
		~Undefined();

		virtual Type klangType();

	public: //To c++ conversions
		virtual operator Int32() const override;
		virtual operator Int64() const override;
		virtual operator float() const override;
		virtual operator double() const override;
		virtual operator bool() const override;
		virtual operator std::string() const override;
		virtual operator std::vector<Value*>() const override;
		virtual operator std::unordered_map<std::string, Value*>() const override;

	public: //Common operators
		virtual Value* klang_operatorEquals(Value* value) override;
		virtual Value* klang_operatorNotEquals(Value* value) override;
		virtual Value* klang_operatorGreater(Value* value) override;
		virtual Value* klang_operatorLess(Value* value) override;
		virtual Value* klang_operatorGreaterEquals(Value* value) override;
		virtual Value* klang_operatorLessEquals(Value* value) override;
		virtual Value* klang_operatorNot() override;

	public: //Math operators
		virtual Value* klang_operatorPlus(Value* value) override;
		virtual Value* klang_operatorMinus(Value* value) override;
		virtual Value* klang_operatorMultiply(Value* value) override;
		virtual Value* klang_operatorDivide(Value* value) override;
		virtual Value* klang_operatorModule(Value* value) override;
		virtual Value* klang_operatorIncrease() override;
		virtual Value* klang_operatorDecrease() override;
		virtual Value* klang_operatorNegative() override;

	public: //bit operators
		virtual Value* klang_operatorBitwiseLeft(Value* value) override;
		virtual Value* klang_operatorBitwiseRight(Value* value) override;
		virtual Value* klang_operatorBitwiseAnd(Value* value) override;
		virtual Value* klang_operatorBitwiseOr(Value* value) override;
		virtual Value* klang_operatorBitwiseXor(Value* value) override;
		virtual Value* klang_operatorBitwiseNot() override;

	public: //Array/List operators
		virtual Value* klang_operatorArrayGet(Value* index) override;
		virtual void klang_operatorArraySet(Value* index, Value* value) override;

	public: //Object operators
		virtual Value* klang_operatorGetProperty(const std::string& name) override;
		virtual void klang_operatorGetProperty(const std::string& name, Value* value) override;
		virtual Value* klang_operatorCall(Value* self) override;
		virtual Value* klang_operatorCall(Value* self, Value* arg0) override;
		virtual Value* klang_operatorCall(Value* self, Value* arg0, Value* arg1) override;
		virtual Value* klang_operatorCall(Value* self, Value* arg0, Value* arg1, Value* arg2) override;
		virtual Value* klang_operatorCall(Value* self, Variadic&& args) override;

	public: //Reference operators
		virtual Value* klang_operatorReferenceGet() override;
		virtual void klang_operatorReferenceSet(Value* value) override;

	public: //Iterator operators
		virtual Value* klang_operatorIterator() override;
		virtual Value* klang_operatorHasNext() override;
		virtual Value* klang_operatorNext() override;
	};
}

namespace klang::constant
{

}
