#pragma once

#include <type_traits>
#include <vector>
#include <unordered_map>
#include <iostream>

#include "utils.h"
#include "rawmem.h"

namespace klang
{
	namespace type { class Value; }

	typedef std::vector<type::Value*> ValueVector;
	typedef std::unordered_map<std::string, type::Value*> ValueMap;

}

namespace klang::stack { struct Stack; }

namespace klang::type
{
	



	class UnsupportedException : public KlangException
	{
	public:
		UnsupportedException(const Value& value, const std::string& operationName) noexcept;
	};



	class Variadic
	{
	protected:
		constexpr Variadic() = default;

	public:
		virtual unsigned int narg() const = 0;
		virtual Value* arg0() = 0;
		virtual Value* arg(unsigned int index) = 0;
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

	public:
		const Type type;
		
	protected:
		constexpr Value(const Type type) noexcept : type{ type } {};

	public:
		virtual ~Value() = default;

		inline Type klangType() const { return type; };
		std::string getKlangTypeName() const;

	public: //To c++ conversions
		virtual operator Int32() const = 0;
		virtual operator Int64() const = 0;
		virtual operator float() const = 0;
		virtual operator double() const = 0;
		virtual operator bool() const = 0;
		virtual operator std::wstring() const;
		virtual operator ValueVector() const;
		virtual operator ValueMap() const;

	public: //Common operators
		virtual Value* klang_operatorEquals(Value* value);
		virtual Value* klang_operatorNotEquals(Value* value);
		virtual Value* klang_operatorGreater(Value* value);
		virtual Value* klang_operatorLess(Value* value);
		virtual Value* klang_operatorGreaterEquals(Value* value);
		virtual Value* klang_operatorLessEquals(Value* value);
		virtual Value* klang_operatorNot();

	public: //Math operators
		virtual Value* klang_operatorPlus(Value* value);
		virtual Value* klang_operatorMinus(Value* value);
		virtual Value* klang_operatorMultiply(Value* value);
		virtual Value* klang_operatorDivide(Value* value);
		virtual Value* klang_operatorModule(Value* value);
		virtual Value* klang_operatorIncrease();
		virtual Value* klang_operatorDecrease();
		virtual Value* klang_operatorNegative();

	public: //bit operators
		virtual Value* klang_operatorBitwiseLeft(Value* value);
		virtual Value* klang_operatorBitwiseRight(Value* value);
		virtual Value* klang_operatorBitwiseAnd(Value* value);
		virtual Value* klang_operatorBitwiseOr(Value* value);
		virtual Value* klang_operatorBitwiseXor(Value* value);
		virtual Value* klang_operatorBitwiseNot();

	public: //Array/List operators
		virtual Value* klang_operatorArrayGet(Value* index);
		virtual void klang_operatorArraySet(Value* index, Value* value);

	public: //Object operators
		virtual Value* klang_operatorGetProperty(const std::string& name);
		virtual void klang_operatorGetProperty(const std::string& name, Value* value);
		//virtual Value* klang_operatorCall(klang::stack::Stack& stack);

	public: //Reference operators
		virtual Value* klang_operatorReferenceGet();
		virtual void klang_operatorReferenceSet(Value* value);

	public: //Iterator operators
		virtual Value* klang_operatorIterator();
		virtual Value* klang_operatorHasNext();
		virtual Value* klang_operatorNext();

	public:
		inline bool isUndefined() { return type == Type::Undefined; }
		inline bool isInteger() { return type == Type::Integer; }
		inline bool isFloat() { return type == Type::Float; }
		inline bool isBoolean() { return type == Type::Boolean; }
		inline bool isString() { return type == Type::String; }
		inline bool isFunction() { return type == Type::Function; }
		inline bool isReference() { return type == Type::Reference; }
		inline bool isArray() { return type == Type::Array; }
		inline bool isList() { return type == Type::List; }
		inline bool isObject() { return type == Type::Object; }


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
	};

	std::string GetTypeName(Value::Type type);
	std::wstring GetTypeWName(Value::Type type);



	class Undefined : public Value
	{
	public:
		Undefined();
		~Undefined();

	public: //To c++ conversions
		operator Int32() const override;
		operator Int64() const override;
		operator float() const override;
		operator double() const override;
		operator bool() const override;
		operator std::wstring() const override;

		Value* klang_operatorNot() override;

	public:
		static void* operator new(size_t size);
		static void operator delete(void* p);

	public:
		static Undefined* const Instance;
	};



	class Boolean : public Value
	{
	private:
		const bool _value;

	public:
		Boolean(const bool value);
		~Boolean();

	public: //To c++ conversions
		operator Int32() const override;
		operator Int64() const override;
		operator float() const override;
		operator double() const override;
		operator bool() const override;
		operator std::wstring() const override;

	public: //Common operators
		Value* klang_operatorEquals(Value* value) override;
		Value* klang_operatorNotEquals(Value* value) override;
		Value* klang_operatorGreater(Value* value) override;
		Value* klang_operatorLess(Value* value) override;
		Value* klang_operatorGreaterEquals(Value* value) override;
		Value* klang_operatorLessEquals(Value* value) override;
		Value* klang_operatorNot() override;

	public: //Math operators
		Value* klang_operatorPlus(Value* value) override;
		Value* klang_operatorMinus(Value* value) override;
		Value* klang_operatorMultiply(Value* value) override;
		Value* klang_operatorDivide(Value* value) override;
		Value* klang_operatorModule(Value* value) override;
		Value* klang_operatorIncrease() override;
		Value* klang_operatorDecrease() override;
		Value* klang_operatorNegative() override;

	public: //bit operators
		Value* klang_operatorBitwiseLeft(Value* value) override;
		Value* klang_operatorBitwiseRight(Value* value) override;
		Value* klang_operatorBitwiseAnd(Value* value) override;
		Value* klang_operatorBitwiseOr(Value* value) override;
		Value* klang_operatorBitwiseXor(Value* value) override;
		Value* klang_operatorBitwiseNot() override;

	public:
		static void* operator new(size_t size) = delete;
		static void* operator new(size_t size, const bool value);
		static void operator delete(void* p);

	public:
		static Boolean* const True;
		static Boolean* const False;
	};
	inline Boolean* newBoolean(const bool value) { return value ? Boolean::True : Boolean::False; }



	// GENERIC NUMBER //
	namespace
	{
		template<Value::Type _ValueType, typename _NativeType>
		class __Number;

		template<Value::Type _ValueType, typename _NativeType>
		__Number<_ValueType, _NativeType>* newnum(const _NativeType value)
		{
			return heap::create<__Number<_ValueType, _NativeType>>(value);
		}

		template<Value::Type _ValueType, typename _NativeType>
		__Number<_ValueType, _NativeType>* newnum(const Int32 value)
		{
			return heap::create<__Number<_ValueType, _NativeType>>(static_cast<_NativeType>(value));
		}

		template<Value::Type _ValueType, typename _NativeType>
		__Number<_ValueType, _NativeType>* newnum(const Int64 value)
		{
			return heap::create<__Number<_ValueType, _NativeType>>(static_cast<_NativeType>(value));
		}

		template<Value::Type _ValueType, typename _NativeType>
		__Number<_ValueType, _NativeType>* newnum(const float value)
		{
			return heap::create<__Number<_ValueType, _NativeType>>(static_cast<_NativeType>(value));
		}

		template<Value::Type _ValueType, typename _NativeType>
		__Number<_ValueType, _NativeType>* newnum(const double value)
		{
			return heap::create<__Number<_ValueType, _NativeType>>(static_cast<_NativeType>(value));
		}

		template<Value::Type _ValueType, typename _NativeType>
		class __Number : public Value
		{
	#define CREATE newnum<_ValueType, _NativeType>

		private:
			const _NativeType _value;

		public:
			__Number(const _NativeType value) :
				Value{ _ValueType },
				_value{ value }
			{}
			~__Number() {}

		public: //To c++ conversions
			operator Int32() const override { return static_cast<Int32>(_value); }
			operator Int64() const override { return static_cast<Int64>(_value); }
			operator float() const override { return static_cast<float>(_value); }
			operator double() const override { return static_cast<double>(_value); }
			operator bool() const override { return static_cast<bool>(_value); }
			operator std::wstring() const override { return std::to_wstring(_value); }

		public: //Common operators
			Value* klang_operatorEquals(Value* value) override
			{
				if constexpr (_ValueType == Type::Integer)
				{
					switch (value->type)
					{
						default:
						case Type::Float:
							return static_cast<double>(_value) == static_cast<double>(*value) ? Boolean::True : Boolean::False;
						case Type::Integer:
							return static_cast<Int64>(_value) == static_cast<Int64>(*value) ? Boolean::True : Boolean::False;
					}
				}
				else return static_cast<double>(_value) == static_cast<double>(*value) ? Boolean::True : Boolean::False;
			}
			Value* klang_operatorNotEquals(Value* value) override
			{
				if constexpr (_ValueType == Type::Integer)
				{
					switch (value->type)
					{
						default:
						case Type::Float:
							return static_cast<double>(_value) != static_cast<double>(*value) ? Boolean::True : Boolean::False;
						case Type::Integer:
							return static_cast<Int64>(_value) != static_cast<Int64>(*value) ? Boolean::True : Boolean::False;
					}
				}
				else return static_cast<double>(_value) != static_cast<double>(*value) ? Boolean::True : Boolean::False;
			}
			Value* klang_operatorGreater(Value* value) override
			{
				if constexpr (_ValueType == Type::Integer)
				{
					switch (value->type)
					{
						default:
						case Type::Float:
							return static_cast<double>(_value) > static_cast<double>(*value) ? Boolean::True : Boolean::False;
						case Type::Integer:
							return static_cast<Int64>(_value) > static_cast<Int64>(*value) ? Boolean::True : Boolean::False;
					}
				}
				else return static_cast<double>(_value) > static_cast<double>(*value) ? Boolean::True : Boolean::False;
			}
			Value* klang_operatorLess(Value* value) override
			{
				if constexpr (_ValueType == Type::Integer)
				{
					switch (value->type)
					{
						default:
						case Type::Float:
							return static_cast<double>(_value) < static_cast<double>(*value) ? Boolean::True : Boolean::False;
						case Type::Integer:
							return static_cast<Int64>(_value) < static_cast<Int64>(*value) ? Boolean::True : Boolean::False;
					}
				}
				else return static_cast<double>(_value) < static_cast<double>(*value) ? Boolean::True : Boolean::False;
			}
			Value* klang_operatorGreaterEquals(Value* value) override
			{
				if constexpr (_ValueType == Type::Integer)
				{
					switch (value->type)
					{
					default:
						case Type::Float:
							return static_cast<double>(_value) >= static_cast<double>(*value) ? Boolean::True : Boolean::False;
						case Type::Integer:
							return static_cast<Int64>(_value) >= static_cast<Int64>(*value) ? Boolean::True : Boolean::False;
					}
				}
				else return static_cast<double>(_value) >= static_cast<double>(*value) ? Boolean::True : Boolean::False;
			}
			Value* klang_operatorLessEquals(Value* value) override
			{
				if constexpr (_ValueType == Type::Integer)
				{
					switch (value->type)
					{
						default:
						case Type::Float:
							return static_cast<double>(_value) <= static_cast<double>(*value) ? Boolean::True : Boolean::False;
						case Type::Integer:
							return static_cast<Int64>(_value) <= static_cast<Int64>(*value) ? Boolean::True : Boolean::False;
					}
				}
				else return static_cast<double>(_value) <= static_cast<double>(*value) ? Boolean::True : Boolean::False;
			}
			Value* klang_operatorNot() override { return CREATE(!_value); }

		public: //Math operators
			Value* klang_operatorPlus(Value* value) override
			{
				if constexpr (_ValueType == Type::Integer)
				{
					switch (value->type)
					{
						default:
						case Type::Float:
							return CREATE(static_cast<double>(_value) + static_cast<double>(*value));
						case Type::Integer:
							return CREATE(static_cast<Int64>(_value) + static_cast<Int64>(*value));
					}
				}
				else return CREATE(static_cast<double>(_value) + static_cast<double>(*value));
			}
			Value* klang_operatorMinus(Value* value) override
			{
				if constexpr (_ValueType == Type::Integer)
				{
					switch (value->type)
					{
					default:
					case Type::Float:
						return CREATE(static_cast<double>(_value) - static_cast<double>(*value));
					case Type::Integer:
						return CREATE(static_cast<Int64>(_value) - static_cast<Int64>(*value));
					}
				}
				else return CREATE(static_cast<double>(_value) - static_cast<double>(*value));
			}
			Value* klang_operatorMultiply(Value* value) override
			{
				if constexpr (_ValueType == Type::Integer)
				{
					switch (value->type)
					{
					default:
					case Type::Float:
						return CREATE(static_cast<double>(_value) * static_cast<double>(*value));
					case Type::Integer:
						return CREATE(static_cast<Int64>(_value) * static_cast<Int64>(*value));
					}
				}
				else return CREATE(static_cast<double>(_value) * static_cast<double>(*value));
			}
			Value* klang_operatorDivide(Value* value) override
			{
				if constexpr (_ValueType == Type::Integer)
				{
					switch (value->type)
					{
					default:
					case Type::Float:
						return CREATE(static_cast<double>(_value) / static_cast<double>(*value));
					case Type::Integer:
						return CREATE(static_cast<Int64>(_value) / static_cast<Int64>(*value));
					}
				}
				else return CREATE(static_cast<double>(_value) / static_cast<double>(*value));
			}
			Value* klang_operatorModule(Value* value) override
			{
				return CREATE(static_cast<Int64>(_value) % static_cast<Int64>(*value));
			}
			Value* klang_operatorIncrease() override
			{
				return CREATE(_value + 1);
			}
			Value* klang_operatorDecrease() override
			{
				return CREATE(_value - 1);
			}
			Value* klang_operatorNegative() override
			{
				return CREATE(-_value);
			}

		public: //bit operators
			Value* klang_operatorBitwiseLeft(Value* value) override
			{
				return CREATE(static_cast<Int64>(_value) << static_cast<Int64>(*value));
			}
			Value* klang_operatorBitwiseRight(Value* value) override
			{
				return CREATE(static_cast<Int64>(_value) >> static_cast<Int64>(*value));
			}
			Value* klang_operatorBitwiseAnd(Value* value) override
			{
				return CREATE(static_cast<Int64>(_value) & static_cast<Int64>(*value));
			}
			Value* klang_operatorBitwiseOr(Value* value) override
			{
				return CREATE(static_cast<Int64>(_value) | static_cast<Int64>(*value));
			}
			Value* klang_operatorBitwiseXor(Value* value) override
			{
				return CREATE(static_cast<Int64>(_value) ^ static_cast<Int64>(*value));
			}
			Value* klang_operatorBitwiseNot() override
			{
				if constexpr (_ValueType == Type::Float)
					return CREATE(~static_cast<Int64>(_value));
				else return CREATE(~_value);
			}

		public:
			static void* operator new(size_t size) = delete;
			inline static void* operator new(size_t size, const _NativeType value) { return heap::create<_NativeType>(value); }
			static void operator delete(void* p) { heap::destroy(reinterpret_cast<__Number*>(p)); }

			#undef CREATE
		};
	}

	typedef __Number<Value::Type::Integer, Int32> Integer;
	typedef __Number<Value::Type::Integer, Int64> LongInteger;
	typedef __Number<Value::Type::Float, float> Float;
	typedef __Number<Value::Type::Float, double> Double;

	inline Integer* newInteger(const Int32 value) { return heap::create<Integer>(value); }
	inline LongInteger* newLongInteger(const Int64 value) { return heap::create<LongInteger>(value); }
	inline Float* newFloat(const float value) { return heap::create<Float>(value); }
	inline Double* newDouble(const double value) { return heap::create<Double>(value); }



	class String : public Value
	{
	private:
		wchar_t* const _value;
		const size_t _size;

	public:
		String(const std::wstring& value);
		~String();

	public: //To c++ conversions
		operator Int32() const override;
		operator Int64() const override;
		operator float() const override;
		operator double() const override;
		operator bool() const override;
		operator std::wstring() const override;

	public: //Common operators
		Value* klang_operatorEquals(Value* value) override;
		Value* klang_operatorNotEquals(Value* value) override;
		Value* klang_operatorGreater(Value* value) override;
		Value* klang_operatorLess(Value* value) override;
		Value* klang_operatorGreaterEquals(Value* value) override;
		Value* klang_operatorLessEquals(Value* value) override;
		Value* klang_operatorNot() override;

	public: //Math operators
		Value* klang_operatorPlus(Value* value) override;

	public: //Array/List operators
		virtual Value* klang_operatorArrayGet(Value* index) override;

	public:
		static void* operator new(size_t size) = delete;
		static void* operator new(size_t size, const std::wstring& str);
		static void operator delete(void* p);
	};

	inline String* newString(const std::wstring& value) { return heap::create<String>(value); }




	/*class Array : public Value
	{

	};*/
}



namespace klang::type::constant
{
	extern Value* const Undefined;
	extern Value* const True;
	extern Value* const False;
}

std::wostream& operator<< (std::wostream& os, const klang::type::Value& value);
