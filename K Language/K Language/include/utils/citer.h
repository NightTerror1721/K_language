#ifndef K_CITER_H
#define K_CITER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "cerror.h"

#include <stdint.h>

	typedef struct {
		void* container;
		uintmax_t data;
		int (*has_next)(void*, uintmax_t*);
		void* (*next)(void*, uintmax_t*);
	} CIterator;

	status_t citer_init(
		CIterator* it,
		void* container,
		uintmax_t init_data,
		int (*has_next_func)(void*, uintmax_t*),
		void* (*next_func)(void*, uintmax_t*)
	);

	CIterator citer_make(
		void* container,
		uintmax_t init_data,
		int (*has_next_func)(void*, uintmax_t*),
		void* (*next_func)(void*, uintmax_t*)
	);

	int citer_has_next(CIterator* it);

	void* citer_raw_next(CIterator* it);

	status_t citer_foreach(CIterator* it, void (*action)(void*));


#define citer_container(_Iter, _ContainerType) ((_ContainerType*)(_Iter)->container)

#define citer_next(_Iter, _Type) ((_Type*) citer_raw_next((_Iter)))

#define cforeach(_Type, _Var, _Iter) for( \
		_Type* _Var = citer_next((_Iter), _Type); \
		citer_has_next(_Iter); \
		_Var = citer_next((_Iter), _Type) \
	)

#ifdef __cplusplus
}
#endif
#endif
