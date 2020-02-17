#ifndef K_CLIST_H
#define K_CLIST_H

#ifdef __cplusplus
extern "C" {
#endif

#include "cerror.h"
#include "cutils.h"

	typedef struct CListNode
	{
		struct CListNode* next;
		struct CListNode* prev;
		void* data;
		size_t data_size;

	} CListNode;

	typedef struct
	{
		CListNode head;
		CListNode tail;
		size_t size;

	} CRawList;


	status_t _crawlist_init(CRawList* l);

	status_t _crawlist_push_back(CRawList* l, const void* ptr, const size_t size);




#define CList(_Type) struct { CRawList base; _Type temp; _Type* ref; }

#define clist_init(_List) _crawlist_init(&(_List)->base)

#define clist_new(_Type) cnew(CList(_Type))
#define clist_delete(_List) if((_List)) cdelete((_List))

#define clist_push_back(_List, _Value) ((_List)->temp = (_Value), _crawlist_push_back(&(_List)->base, &(_List)->temp, sizeof((_List)->temp)))



	/* Default types */

#define CPtrList CList(void*)
#define CIntList CList(int)
#define CUIntList CList(unsigned int)
#define CFloatList CList(float)
#define CCharList CList(char)
#define CStringList CList(char*)


#ifdef __cplusplus
}
#endif
#endif
