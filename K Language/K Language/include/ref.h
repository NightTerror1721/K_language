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
	};
}

std::ostream& operator<< (std::ostream& os, const klang::Ref& ref);



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
