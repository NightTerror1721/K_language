#pragma once

#include <vcruntime.h>
#include <type_traits>

namespace klang::heap
{
	void* malloc(const size_t size);
	void free(void* const ptr);
	void gc();

	void incref(void* const ptr);
	void decref(void* const ptr);

	void* s_malloc(const size_t size);



	template<class _Ty>
	inline _Ty* create()
	{
		_Ty* ptr = reinterpret_cast<_Ty*>(klang::heap::malloc(sizeof(_Ty)));
		if (ptr)
			::new(ptr) _Ty();
		return ptr;
	}

	template<class _Ty, typename _Arg0>
	inline _Ty* create(const _Arg0& arg0)
	{
		_Ty* ptr = reinterpret_cast<_Ty*>(klang::heap::malloc(sizeof(_Ty)));
		if (ptr)
			::new(ptr) _Ty(arg0);
		return ptr;
	}

	template<class _Ty>
	inline void destroy(_Ty* value)
	{
		value->~_Ty();
		klang::heap::free(value);
	}
}
