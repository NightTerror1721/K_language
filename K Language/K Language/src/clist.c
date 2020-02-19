#include "utils/clist.h"

#include <string.h>

typedef CListNode Node;


/* Private functions */

static Node* node_new(CRawList* owner, const size_t size)
{
	Node* n = cnew(Node);
	if (!n)
		return NULL;

	n->data_size = size;
	n->data = malloc(size);
	if (!n->data)
	{
		cdelete(n);
		return NULL;
	}
	n->owner = owner;
	n->next = n->prev = NULL;
	return n;
}

static void node_delete(Node* n)
{
	if (n)
	{
		cdelete(n->data);
		cdelete(n);
	}
}

static status_t node_set(Node* n, const void* value, const size_t size)
{
	if (memcpy_s(n->data, n->data_size, value, size) != 0)
		return S_MEMCPY_ERROR;
	return S_OK;
}

static status_t node_make(Node** dst, CRawList* owner, const void* ptr, const size_t size)
{
	Node* n = node_new(owner, size);
	if (!n)
		return S_ALLOC_MEM_ERROR;

	status_t status = node_set(n, ptr, size);
	if (status != S_OK)
	{
		node_delete(n);
		return status;
	}

	*dst = n;
	return S_OK;
}

static status_t node_rawfind(Node** dst, Node* head, Node* tail, size_t index, int reverse)
{
	if (reverse)
	{
		Node* n = tail;
		while (index > 0 && n)
		{
			n = n->prev;
			--index;
		}
		if (!n)
			return S_OUT_OF_RANGE;
		*dst = n;
	}
	else
	{
		Node* n = head;
		while (index > 0 && n)
		{
			n = n->next;
			--index;
		}
		if (!n)
			return S_OUT_OF_RANGE;
		*dst = n;
	}

	return S_OK;
}
#define node_find(_Dst, _List, _Index) \
	node_rawfind((_Dst), (_List)->head, (_List)->tail, (_Index), (_Index) > ((_List)->size / 2))

status_t _erase_node(CRawList* l, Node* n)
{
	if (n->owner != l)
		return S_INVALID_PARAMETER;

	if (n == l->head)
	{
		if (n == l->tail)
		{
			l->head = l->tail = NULL;
		}
		else
		{
			l->head = n->next;
			l->head->prev = NULL;
		}
	}
	else if (n == l->tail)
	{
		l->tail = n->prev;
		l->tail->next = NULL;
	}
	else
	{
		n->next->prev = n->prev;
		n->prev->next = n->next;
	}

	--l->size;
	node_delete(n);
	return S_OK;
}

/* Public functions */

status_t crawlist_init(CRawList* l)
{
	if (!l)
		return S_UNEXPECTED_NULL;

	l->head = l->tail = NULL;
	l->size = 0;
	return S_OK;
}

void crawlist_deinit(CRawList* l)
{
	if (!l)
		return;

	crawlist_clear(l);
}

CRawList* crawlist_new()
{
	CRawList* l = cnew(CRawList);
	if (!l)
		return NULL;

	crawlist_init(l);
	return l;
}

void crawlist_delete(CRawList* l)
{
	if (l)
	{
		crawlist_clear(l);
		cdelete(l);
	}
}

status_t crawlist_push_back(CRawList* l, const void* ptr, const size_t size)
{
	if(!l)
		return S_UNEXPECTED_NULL;

	Node* n;
	status_t status = node_make(&n, l, ptr, size);
	if (status != S_OK)
		return status;

	if (!l->head)
	{
		l->head = l->tail = n;
	}
	else
	{
		n->prev = l->tail;
		l->tail->next = n;
		l->tail = n;
	}

	++l->size;
	return S_OK;
}

status_t crawlist_push_front(CRawList* l, const void* ptr, const size_t size)
{
	if (!l)
		return S_UNEXPECTED_NULL;

	Node* n;
	status_t status = node_make(&n, l, ptr, size);
	if (status != S_OK)
		return status;

	if (!l->head)
	{
		l->head = l->tail = n;
	}
	else
	{
		n->next = l->head;
		l->head->prev = n;
		l->head = n;
	}

	++l->size;
	return S_OK;
}

status_t crawlist_add(CRawList* l, const size_t index, const void* ptr, const size_t size)
{
	if (index == 0)
		return crawlist_push_front(l, ptr, size);

	if (index == l->size)
		return crawlist_push_back(l, ptr, size);

	if (!l)
		return S_UNEXPECTED_NULL;

	Node* prev;
	status_t status = node_find(&prev, l, index);
	if (status != S_OK)
		return status;

	Node* n;
	status = node_make(&n, l, ptr, size);
	if (status != S_OK)
		return status;

	n->next = prev->next;
	n->prev = prev;

	prev->next->prev = n;
	prev->next = n;

	++l->size;

	return S_OK;
}

status_t crawlist_set(CRawList* l, const size_t index, const void* ptr, const size_t size)
{
	if (!l)
		return S_UNEXPECTED_NULL;

	if (index >= l->size)
		return S_OUT_OF_RANGE;

	Node* n;
	status_t status;
	if (index == 0)
		n = l->head;
	else if (index == l->size - 1)
		n = l->tail;
	else
	{
		status = node_find(&n, l, index);
		if (status != S_OK)
			return status;
	}

	return node_set(n, ptr, size);
}


status_t crawlist_back(CRawList* l, void** ptr)
{
	*ptr = NULL;
	if (!l)
		return S_UNEXPECTED_NULL;

	if (!l->tail)
		return S_OUT_OF_RANGE;

	*ptr = l->tail->data;
	return S_OK;
}

status_t crawlist_front(CRawList* l, void** ptr)
{
	*ptr = NULL;
	if (!l)
		return S_UNEXPECTED_NULL;

	if (!l->head)
		return S_OUT_OF_RANGE;

	*ptr = l->head->data;
	return S_OK;
}

status_t crawlist_get(CRawList* l, const size_t index, void** ptr)
{
	*ptr = NULL;
	if (!l)
		return S_UNEXPECTED_NULL;

	if (index >= l->size)
		return S_OUT_OF_RANGE;

	Node* n;
	status_t status;
	if (index == 0)
		n = l->head;
	else if (index == l->size - 1)
		n = l->tail;
	else
	{
		status = node_find(&n, l, index);
		if (status != S_OK)
			return status;
	}

	*ptr = n->data;
	return S_OK;
}

CListNode* crawlist_find(CRawList* l, const void* ptr, const size_t size)
{
	if (!l)
		return NULL;

	for (Node* n = l->head; n; n = n->next)
		if (memcmp(n->data, ptr, min(n->data_size, size)) == 0)
			return n;
	return NULL;
}

CListNode* crawlist_find_if(CRawList* l, int (*criteria)(const void*, const size_t))
{
	if (!l)
		return NULL;

	for (Node* n = l->head; n; n = n->next)
		if (criteria(n->data, n->data_size))
			return n;
	return NULL;
}

CListNode* crawlist_find_ifd(CRawList* l, void* extern_data, int (*criteria)(void*, const void*, const size_t))
{
	if (!l)
		return NULL;

	for (Node* n = l->head; n; n = n->next)
		if (criteria(extern_data, n->data, n->data_size))
			return n;
	return NULL;
}





status_t crawlist_erase_back(CRawList* l)
{
	if (!l)
		return S_UNEXPECTED_NULL;

	if (!l->head)
		return S_OUT_OF_RANGE;

	return _erase_node(l, l->tail);
}

status_t crawlist_erase_front(CRawList* l)
{
	if (!l)
		return S_UNEXPECTED_NULL;

	if (!l->head)
		return S_OUT_OF_RANGE;

	return _erase_node(l, l->head);
}

status_t crawlist_erase(CRawList* l, const size_t index)
{
	if (!l)
		return S_UNEXPECTED_NULL;

	if (index >= l->size)
		return S_OUT_OF_RANGE;

	Node* n;
	status_t status;
	if (index == 0)
		n = l->head;
	else if (index == l->size - 1)
		n = l->tail;
	else
	{
		status = node_find(&n, l, index);
		if (status != S_OK)
			return status;
	}

	return _erase_node(l, n);
}

status_t crawlist_erase_node(CRawList* l, CListNode* node)
{
	if (!l)
		return S_UNEXPECTED_NULL;

	return _erase_node(l, node);
}

void crawlist_clear(CRawList* l)
{
	if (!l)
		return;

	Node *node = l->head, *next;
	while (node)
	{
		next = node->next;
		node_delete(node);
		node = next;
	}

	l->head = l->tail = NULL;
	l->size = 0;
}




#define __NOINIT_IT (uintmax_t)(-1)

static int _crawlist_iterator_has_next(void* c, uintmax_t* d)
{
	return *d == __NOINIT_IT || ((Node*)(*d)) != NULL;
}

static void* _crawlist_iterator_next(void* c, uintmax_t* d)
{
	CRawList* l = (CRawList*)c;
	if (*d == __NOINIT_IT)
	{
		*d = (uintmax_t)(l->head);
		return l->head ? l->head->data : NULL;
	}

	Node* n = (Node*)(*d);
	if (!n)
		return NULL;

	*d = (uintmax_t)n->next;
	return n->next ? n->next->data : NULL;
}

CIterator crawlist_iterator(CRawList* l)
{
	return citer_make(l, __NOINIT_IT, &_crawlist_iterator_has_next, &_crawlist_iterator_next);
}



void* _clist_new(const size_t type_size)
{
	size_t size = sizeof(CRawList) + type_size + sizeof(void*);
	void* p = malloc(size);
	if (!p)
		return NULL;

	memset(p, 0, size);
	crawlist_init((CRawList*)(p));
	return p;
}

void _clist_delete(void* pl)
{
	if (pl)
	{
		crawlist_clear((CRawList*)pl);
		cdelete(pl);
	}
}
