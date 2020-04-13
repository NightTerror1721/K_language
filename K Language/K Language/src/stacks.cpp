#include "stacks.h"

#include <cstring>

#include "ref.h"

namespace klang::stack
{


	Stack::Stack(const Byte capacity) :
		regs{ new Register[capacity] },
		capacity{ capacity },
		size{}
	{
		std::memset(regs, 0, sizeof(Register) * capacity);
	}
	Stack::~Stack()
	{
		Reference* reg = regs;
		for (int i = 0; i < static_cast<int>(capacity); i++, reg++)
			if (*reg)
				heap::decref(*reg);
		delete[] regs;
	}

	void Stack::push_value(type::Value* const value)
	{
		if (size < capacity)
		{
			regs[size++] = value;
			heap::incref(value);
		}
	}

	void Stack::push_value(const klang::Ref& value)
	{
		if (size < capacity)
		{
			regs[size++] = const_cast<klang::type::Value*>(static_cast<const klang::type::Value*>(value));
			heap::incref(regs[size - 1]);
		}
	}


	klang::type::Value* Stack::pop_value()
	{
		if (size > 0)
			return regs[--size];
		return klang::type::constant::Undefined;
	}


	klang::type::Value* Stack::get(const Byte index) const
	{
		return index >= capacity
			? type::constant::Undefined
			: regs[index] ? regs[index] : type::constant::Undefined;
	}

	void Stack::set(const Byte index, klang::type::Value* value)
	{
		if (index < capacity)
			regs[index] = value ? value : type::constant::Undefined;
	}

}
