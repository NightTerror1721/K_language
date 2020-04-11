#ifndef KLANG__PRIVATE_HEAP_H_
#define KLANG__PRIVATE_HEAP_H_

#ifdef __cplusplus
extern "C" {
#endif

	#include <stdlib.h>

	typedef struct __private_heap_header {

		struct __private_heap_header* next;
		struct __private_heap_header* prev;
		size_t size;
		unsigned int refs;

	} __private_heap_header;

	typedef struct {

		int is_static;
		size_t capacity;
		size_t used;
		__private_heap_header* last;
		void* data;

	} __private_heap;


	int klangh_CreateHeap(__private_heap* const heap, const size_t size, const int is_static);
	int klangh_DestroyHeap(__private_heap* const heap);

	int klangh_Malloc(__private_heap* const heap, const size_t size, void** const ptr);
	int klangh_Free(__private_heap* const heap, void* const ptr);

	int klangh_GetHeader(const void* const ptr, __private_heap_header** const header);
	int klangh_IncreaseReferenceCounter(void* const ptr);
	int klangh_DecreaseReferenceCounter(void* const ptr);

	int klangh_RunGarbageCollector(__private_heap* const heap);


	enum heap_status
	{
		HS_OK = 0,
		HS_CANNOT_CREATE = -1,
		HS_HEAP_OVERFLOW = -2
	};

#ifdef __cplusplus
}
#endif
#endif
