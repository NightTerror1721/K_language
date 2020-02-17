#include "utils/citer.h"

#include <stdlib.h>

status_t citer_init(
	CIterator* it,
	void* container,
	uintmax_t data,
	int (*has_next_func)(void*, uintmax_t*),
	void* (*next_func)(void*, uintmax_t*)
) {
	if (!it || !has_next_func || !next_func)
		return S_UNEXPECTED_NULL;

	it->container = container;
	it->data = data;
	it->has_next = has_next_func;
	it->next = next_func;

	return S_OK;
}

CIterator citer_make(
	void* container,
	uintmax_t data,
	int (*has_next_func)(void*, uintmax_t*),
	void* (*next_func)(void*, uintmax_t*)
) {
	CIterator it = { container, data, has_next_func, next_func };
	return it;
}

int citer_has_next(CIterator* it)
{
	return it->has_next(it->container, &it->data);
}

void* citer_raw_next(CIterator* it)
{
	return it->next(it->container, &it->data);
}

void citer_foreach(CIterator* it, void (*action)(void*))
{
	while (citer_has_next(it))
		action(citer_raw_next(it));
}

