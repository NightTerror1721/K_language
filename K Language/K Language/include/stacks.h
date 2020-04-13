#pragma once

#include "utils.h"
#include "types.h"

namespace klang { class Ref; }


namespace klang::stack
{
	typedef UInt8		 Byte;
	typedef UInt16		 Word;
	typedef UInt32		 Long;
	typedef UInt64		 Quad;
	typedef type::Value* Reference;
	typedef Reference    Register;



	struct CallInfo
	{
		
	};

	struct CallStack
	{

	};

	struct Stack
	{
		Register* const regs;
		const Byte capacity;
		Byte size;

		Stack(const Byte capacity);
		~Stack();

		void push_value(type::Value* const value);
		void push_value(const klang::Ref& value);

		klang::type::Value* pop_value();

		template<typename _Ty>
		void push(const _Ty& value) {}


		void set(const Byte index, klang::type::Value* value);

		klang::type::Value* get(const Byte index) const;


	public:
		inline void push(type::Value* const value) { push_value(value); }
		inline void push(const klang::Ref& value) { push_value(value); }

		template<> void push<Int32>(const Int32& value) { push_value(type::newInteger(value)); }
		template<> void push<UInt32>(const UInt32& value) { push_value(type::newInteger(static_cast<Int32>(value))); }

	public:
		template<Byte _Index>
		klang::type::Value* get() const
		{
			return _Index >= capacity
				? type::constant::Undefined
				: regs[_Index] ? regs[_Index] : type::constant::Undefined;
		}
	};
}
