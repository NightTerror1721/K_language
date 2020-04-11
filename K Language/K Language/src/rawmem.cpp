#include "rawmem.h"

#include "heap.h"

#define DEFAULT_HEAP_SIZE (64 * 1024 * 1024)
#define DEFAULT_STATIC_HEAP_SIZE (8192)

namespace klang::heap
{
	class Heap
	{
	public:
		__private_heap mem;

		Heap(bool isStatic) :
			mem{}
		{
			klangh_CreateHeap(&mem, (isStatic ? DEFAULT_STATIC_HEAP_SIZE : DEFAULT_HEAP_SIZE), isStatic);
		}
		~Heap()
		{
			klangh_DestroyHeap(&mem);
		}

		Heap(const Heap&) = delete;
		Heap& operator= (const Heap&) = delete;

		static Heap Default;
		static Heap Static;
	};

	Heap Heap::Default{ false };
	Heap Heap::Static{ true };
}



namespace klang::heap
{
	void* malloc(const size_t size)
	{
		void* ptr;
		if (klangh_Malloc(&Heap::Default.mem, size, &ptr) != HS_OK)
			return nullptr;
		return ptr;
	}
	void free(void* const ptr) { klangh_Free(&Heap::Default.mem, ptr); }
	void gc() { klangh_RunGarbageCollector(&Heap::Default.mem); }

	void incref(void* const ptr) { klangh_IncreaseReferenceCounter(ptr); }
	void decref(void* const ptr) { klangh_DecreaseReferenceCounter(ptr); }

	void* s_malloc(const size_t size)
	{
		void* ptr;
		if (klangh_Malloc(&Heap::Static.mem, size, &ptr) != HS_OK)
			return nullptr;
		return ptr;
	}
}
