#ifndef K_CLIST_H
#define K_CLIST_H

#ifdef __cplusplus
extern "C" {
#endif

#include "cerror.h"
#include "cutils.h"
#include "citer.h"

	typedef struct CListNode
	{
		struct CListNode* next;
		struct CListNode* prev;
		void* data;
		size_t data_size;

	} CListNode;

	typedef struct
	{
		CListNode* head;
		CListNode* tail;
		size_t size;

	} CRawList;


	status_t crawlist_init(CRawList* l);

	CRawList* crawlist_new();
	void crawlist_delete(CRawList* l);


	/* Addition operations */

	status_t crawlist_push_back(CRawList* l, const void* ptr, const size_t size);
	status_t crawlist_push_front(CRawList* l, const void* ptr, const size_t size);
	status_t crawlist_add(CRawList* l, const size_t index, const void* ptr, const size_t size);


	/* Erase operations */

	void crawlist_clear(CRawList* l);


	/* Iterator operations */

	CIterator crawlist_iterator(CRawList* l);


#define CList(_Type) struct { CRawList base; _Type temp; _Type* ref; }

	void* _clist_new(const size_t type_size);
	void _clist_delete(void*);

#define clist_init(_List) crawlist_init(&(_List)->base)

#define clist_new(_Type) _clist_new(sizeof(_Type))
#define clist_delete(_List) _clist_delete((_List))

#define clist_push_back(_List, _Value) ((_List)->temp = (_Value), crawlist_push_back(&(_List)->base, &(_List)->temp, sizeof((_List)->temp)))
#define clist_push_front(_List, _Value) ((_List)->temp = (_Value), crawlist_push_front(&(_List)->base, &(_List)->temp, sizeof((_List)->temp)))
#define clist_add(_List, _Index, _Value) ((_List)->temp = (_Value), crawlist_add(&(_List)->base, (_Index), &(_List)->temp, sizeof((_List)->temp)))

#define clist_iterator(_List) crawlist_iterator(&(_List)->base)



	/* Default types */

	typedef CList(void*) CPtrList;
	typedef CList(int) CIntList;
	typedef CList(unsigned int) CUIntList;
	typedef CList(float) CFloatList;
	typedef CList(double) CDoubleList;
	typedef CList(char*) CStringList;


#ifdef __cplusplus
}
#endif
#endif
