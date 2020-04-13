#pragma once

#include "types.h"

namespace klang
{
	class Ref
	{
	private:
		klang::type::Value* _value;

	public:
		constexpr Ref() noexcept : _value{ type::constant::Undefined } {}
		Ref(klang::type::Value* value) noexcept;
		Ref(const klang::type::Value* value) noexcept;
		Ref(const Ref& ref) noexcept;
		Ref(Ref&& ref) noexcept;
		~Ref();

		Ref& operator= (klang::type::Value* value) noexcept;
		Ref& operator= (const Ref& ref) noexcept;
		Ref& operator= (Ref&& ref) noexcept;

		klang::type::Value::Type type() const;

		operator klang::type::Value* () const;
		operator const klang::type::Value* () const;

		klang::type::Value* operator-> ();
		const klang::type::Value* operator-> () const;



	public:
		// Array access operator //
		class ArrayAccessor
		{
		private:
			klang::type::Value* const _value;
			klang::type::Value* const _index;

		public:
			constexpr ArrayAccessor(klang::type::Value* const value, klang::type::Value* const index) :
				_value{ value ? value : klang::type::constant::Undefined },
				_index{ index ? index : klang::type::constant::Undefined }
			{}
			constexpr ArrayAccessor(const klang::type::Value* const value, const klang::type::Value* const index) :
				ArrayAccessor{ const_cast<klang::type::Value*>(value), const_cast<klang::type::Value*>(index) }
			{}
			inline ArrayAccessor(klang::type::Value* const value, const size_t index) :
				ArrayAccessor{ value, klang::type::newLongInteger(static_cast<Int64>(index)) }
			{}
			inline ArrayAccessor(const klang::type::Value* const value, const size_t index) :
				ArrayAccessor{ const_cast<klang::type::Value*>(value), klang::type::newLongInteger(static_cast<Int64>(index)) }
			{}

			ArrayAccessor(const ArrayAccessor&) = delete;
			ArrayAccessor& operator= (const ArrayAccessor&) = delete;

			inline ArrayAccessor& operator= (klang::type::Value* const value)
			{
				return _value->klang_operatorArraySet(_index, value), *this;
			}
			inline ArrayAccessor& operator= (Ref& value)
			{
				return _value->klang_operatorArraySet(_index, value), * this;
			}
			inline ArrayAccessor& operator= (Ref&& value)
			{
				return _value->klang_operatorArraySet(_index, value), * this;
			}

			inline operator Ref() const { return const_cast<klang::type::Value*>(_value)->klang_operatorArrayGet(_index); }
		};

		Ref::ArrayAccessor operator[] (const size_t index);
		Ref::ArrayAccessor operator[] (const int index);
		Ref::ArrayAccessor operator[] (Ref& index);
		Ref::ArrayAccessor operator[] (Ref&& index);

		const Ref::ArrayAccessor operator[] (const size_t index) const;
		const Ref::ArrayAccessor operator[] (const int index) const;
		const Ref::ArrayAccessor operator[] (Ref& index) const;
		const Ref::ArrayAccessor operator[] (Ref&& index) const;



	public:
		/* NULL values */
		Ref(decltype(nullptr));
		Ref& operator= (decltype(nullptr));


		/* Bool values */
		Ref(const bool value);
		Ref& operator= (const bool value);
		operator bool() const;


		/* Integer values */
		Ref(const Int32 value);
		Ref(const UInt32 value);
		Ref& operator= (const Int32 value);
		Ref& operator= (const UInt32 value);
		operator Int32() const;
		operator UInt32() const;


		/* Long Integer values */
		Ref(const Int64 value);
		Ref(const UInt64 value);
		Ref& operator= (const Int64 value);
		Ref& operator= (const UInt64 value);
		operator Int64() const;
		operator UInt64() const;


		/* Float values */
		Ref(const float value);
		Ref& operator= (const float value);
		operator float() const;


		/* Double values */
		Ref(const double value);
		Ref& operator= (const double value);
		operator double() const;


		/* String values */
		Ref(const std::wstring& value);
		Ref& operator= (const std::wstring& value);
		operator std::wstring () const;
	};
}

std::wostream& operator<< (std::wostream& os, const klang::Ref& ref);



template<typename _Ty>
inline klang::Ref operator== (klang::Ref& ref, const _Ty& value) { return ref->klang_operatorEquals(klang::Ref{ value }); }
template<typename _Ty>
inline klang::Ref operator== (klang::Ref&& ref, const _Ty& value) { return ref->klang_operatorEquals(klang::Ref{ value }); }
template<typename _Ty>
inline klang::Ref operator== (const _Ty& value, klang::Ref& ref) { return ref->klang_operatorEquals(klang::Ref{ value }); }
template<typename _Ty>
inline klang::Ref operator== (const _Ty& value, klang::Ref&& ref) { return ref->klang_operatorEquals(klang::Ref{ value }); }
inline klang::Ref operator== (klang::Ref& ref0, klang::Ref& ref1) { return ref0->klang_operatorEquals(ref1); }

template<typename _Ty>
inline klang::Ref operator!= (klang::Ref& ref, const _Ty& value) { return ref->klang_operatorNotEquals(klang::Ref{ value }); }
template<typename _Ty>
inline klang::Ref operator!= (klang::Ref&& ref, const _Ty& value) { return ref->klang_operatorNotEquals(klang::Ref{ value }); }
template<typename _Ty>
inline klang::Ref operator!= (const _Ty& value, klang::Ref& ref) { return ref->klang_operatorNotEquals(klang::Ref{ value }); }
template<typename _Ty>
inline klang::Ref operator!= (const _Ty& value, klang::Ref&& ref) { return ref->klang_operatorNotEquals(klang::Ref{ value }); }
inline klang::Ref operator!= (klang::Ref& ref0, klang::Ref& ref1) { return ref0->klang_operatorNotEquals(ref1); }

template<typename _Ty>
inline klang::Ref operator> (klang::Ref& ref, const _Ty& value) { return ref->klang_operatorGreater(klang::Ref{ value }); }
template<typename _Ty>
inline klang::Ref operator> (klang::Ref&& ref, const _Ty& value) { return ref->klang_operatorGreater(klang::Ref{ value }); }
template<typename _Ty>
inline klang::Ref operator> (const _Ty& value, klang::Ref& ref) { return ref->klang_operatorGreater(klang::Ref{ value }); }
template<typename _Ty>
inline klang::Ref operator> (const _Ty& value, klang::Ref&& ref) { return ref->klang_operatorGreater(klang::Ref{ value }); }
inline klang::Ref operator> (klang::Ref& ref0, klang::Ref& ref1) { return ref0->klang_operatorGreater(ref1); }

template<typename _Ty>
inline klang::Ref operator< (klang::Ref& ref, const _Ty& value) { return ref->klang_operatorLess(klang::Ref{ value }); }
template<typename _Ty>
inline klang::Ref operator< (klang::Ref&& ref, const _Ty& value) { return ref->klang_operatorLess(klang::Ref{ value }); }
template<typename _Ty>
inline klang::Ref operator< (const _Ty& value, klang::Ref& ref) { return ref->klang_operatorLess(klang::Ref{ value }); }
template<typename _Ty>
inline klang::Ref operator< (const _Ty& value, klang::Ref&& ref) { return ref->klang_operatorLess(klang::Ref{ value }); }
inline klang::Ref operator< (klang::Ref& ref0, klang::Ref& ref1) { return ref0->klang_operatorLess(ref1); }

template<typename _Ty>
inline klang::Ref operator>= (klang::Ref& ref, const _Ty& value) { return ref->klang_operatorGreaterEquals(klang::Ref{ value }); }
template<typename _Ty>
inline klang::Ref operator>= (klang::Ref&& ref, const _Ty& value) { return ref->klang_operatorGreaterEquals(klang::Ref{ value }); }
template<typename _Ty>
inline klang::Ref operator>= (const _Ty& value, klang::Ref& ref) { return ref->klang_operatorGreaterEquals(klang::Ref{ value }); }
template<typename _Ty>
inline klang::Ref operator>= (const _Ty& value, klang::Ref&& ref) { return ref->klang_operatorGreaterEquals(klang::Ref{ value }); }
inline klang::Ref operator>= (klang::Ref& ref0, klang::Ref& ref1) { return ref0->klang_operatorGreaterEquals(ref1); }

template<typename _Ty>
inline klang::Ref operator<= (klang::Ref& ref, const _Ty& value) { return ref->klang_operatorLessEquals(klang::Ref{ value }); }
template<typename _Ty>
inline klang::Ref operator<= (klang::Ref&& ref, const _Ty& value) { return ref->klang_operatorLessEquals(klang::Ref{ value }); }
template<typename _Ty>
inline klang::Ref operator<= (const _Ty& value, klang::Ref& ref) { return ref->klang_operatorLessEquals(klang::Ref{ value }); }
template<typename _Ty>
inline klang::Ref operator<= (const _Ty& value, klang::Ref&& ref) { return ref->klang_operatorLessEquals(klang::Ref{ value }); }
inline klang::Ref operator<= (klang::Ref& ref0, klang::Ref& ref1) { return ref0->klang_operatorLessEquals(ref1); }

inline klang::Ref operator! (klang::Ref& ref) { return ref->klang_operatorNot(); }
inline klang::Ref operator! (klang::Ref&& ref) { return ref->klang_operatorNot(); }

template<typename _Ty>
inline klang::Ref operator+ (klang::Ref& ref, const _Ty& value) { return ref->klang_operatorPlus(klang::Ref{ value }); }
template<typename _Ty>
inline klang::Ref operator+ (klang::Ref&& ref, const _Ty& value) { return ref->klang_operatorPlus(klang::Ref{ value }); }
template<typename _Ty>
inline klang::Ref operator+ (const _Ty& value, klang::Ref& ref) { return ref->klang_operatorPlus(klang::Ref{ value }); }
template<typename _Ty>
inline klang::Ref operator+ (const _Ty& value, klang::Ref&& ref) { return ref->klang_operatorPlus(klang::Ref{ value }); }
inline klang::Ref operator+ (klang::Ref& ref0, klang::Ref& ref1) { return ref0->klang_operatorPlus(ref1); }

template<typename _Ty>
inline klang::Ref operator- (klang::Ref& ref, const _Ty& value) { return ref->klang_operatorMinus(klang::Ref{ value }); }
template<typename _Ty>
inline klang::Ref operator- (klang::Ref&& ref, const _Ty& value) { return ref->klang_operatorMinus(klang::Ref{ value }); }
template<typename _Ty>
inline klang::Ref operator- (const _Ty& value, klang::Ref& ref) { return ref->klang_operatorMinus(klang::Ref{ value }); }
template<typename _Ty>
inline klang::Ref operator- (const _Ty& value, klang::Ref&& ref) { return ref->klang_operatorMinus(klang::Ref{ value }); }
inline klang::Ref operator- (klang::Ref& ref0, klang::Ref& ref1) { return ref0->klang_operatorMinus(ref1); }

template<typename _Ty>
inline klang::Ref operator* (klang::Ref& ref, const _Ty& value) { return ref->klang_operatorMultiply(klang::Ref{ value }); }
template<typename _Ty>
inline klang::Ref operator* (klang::Ref&& ref, const _Ty& value) { return ref->klang_operatorMultiply(klang::Ref{ value }); }
template<typename _Ty>
inline klang::Ref operator* (const _Ty& value, klang::Ref& ref) { return ref->klang_operatorMultiply(klang::Ref{ value }); }
template<typename _Ty>
inline klang::Ref operator* (const _Ty& value, klang::Ref&& ref) { return ref->klang_operatorMultiply(klang::Ref{ value }); }
inline klang::Ref operator* (klang::Ref& ref0, klang::Ref& ref1) { return ref0->klang_operatorMultiply(ref1); }

template<typename _Ty>
inline klang::Ref operator/ (klang::Ref& ref, const _Ty& value) { return ref->klang_operatorDivide(klang::Ref{ value }); }
template<typename _Ty>
inline klang::Ref operator/ (klang::Ref&& ref, const _Ty& value) { return ref->klang_operatorDivide(klang::Ref{ value }); }
template<typename _Ty>
inline klang::Ref operator/ (const _Ty& value, klang::Ref& ref) { return ref->klang_operatorDivide(klang::Ref{ value }); }
template<typename _Ty>
inline klang::Ref operator/ (const _Ty& value, klang::Ref&& ref) { return ref->klang_operatorDivide(klang::Ref{ value }); }
inline klang::Ref operator/ (klang::Ref& ref0, klang::Ref& ref1) { return ref0->klang_operatorDivide(ref1); }

template<typename _Ty>
inline klang::Ref operator% (klang::Ref& ref, const _Ty& value) { return ref->klang_operatorModule(klang::Ref{ value }); }
template<typename _Ty>
inline klang::Ref operator% (klang::Ref&& ref, const _Ty& value) { return ref->klang_operatorModule(klang::Ref{ value }); }
template<typename _Ty>
inline klang::Ref operator% (const _Ty& value, klang::Ref& ref) { return ref->klang_operatorModule(klang::Ref{ value }); }
template<typename _Ty>
inline klang::Ref operator% (const _Ty& value, klang::Ref&& ref) { return ref->klang_operatorModule(klang::Ref{ value }); }
inline klang::Ref operator% (klang::Ref& ref0, klang::Ref& ref1) { return ref0->klang_operatorModule(ref1); }

inline klang::Ref operator++ (klang::Ref& ref) { return ref = ref->klang_operatorIncrease(); }
inline klang::Ref operator++ (klang::Ref&& ref) { return ref = ref->klang_operatorIncrease(); }
inline klang::Ref operator++ (klang::Ref& ref, int) { klang::Ref res = ref; return ref = ref->klang_operatorIncrease(), res; }
inline klang::Ref operator++ (klang::Ref&& ref, int) { klang::Ref res = ref; return ref = ref->klang_operatorIncrease(), res; }

inline klang::Ref operator-- (klang::Ref& ref) { return ref = ref->klang_operatorDecrease(); }
inline klang::Ref operator-- (klang::Ref&& ref) { return ref = ref->klang_operatorDecrease(); }
inline klang::Ref operator-- (klang::Ref& ref, int) { klang::Ref res = ref; return ref = ref->klang_operatorDecrease(), res; }
inline klang::Ref operator-- (klang::Ref&& ref, int) { klang::Ref res = ref; return ref = ref->klang_operatorDecrease(), res; }

inline klang::Ref operator- (klang::Ref& ref) { return ref->klang_operatorNegative(); }
inline klang::Ref operator- (klang::Ref&& ref) { return ref->klang_operatorNegative(); }
