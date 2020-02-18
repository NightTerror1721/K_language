#include "utils/cmap.h"

#include <string.h>
#include "utils/cutils.h"


/* Private functions */

typedef CMapNode Node;

static hash_t do_hash(const char* str)
{
	hash_t hash = 5381;
	while (*str)
		hash = ((hash << 5) + hash) ^ *(str++);

	return hash;
}


static Node* node_new(CRawMap* owner, const char* key, const void* value, const size_t size)
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
	n->hash = do_hash(key);
	n->next = NULL;
	
	return n;
}

static void node_delete(Node* n)
{
	if (!n)
		return;

	if (n->prev)
		n->prev->next = n->next;
	if (n->next)
		n->next->prev = n->prev;

	if (n->data)
		cdelete(n->data);

	cdelete(n);
}

static status_t node_set(Node* n, const void* value, const size_t size)
{
	if (memcpy_s(n->data, n->data_size, value, size) != 0)
		return S_MEMCPY_ERROR;
	return S_OK;
}

static status_t node_make(Node** dst, CRawMap* owner, const char* key, const void* value, const size_t size)
{
	Node* n = node_new(owner, key, value, size);
	if (!n)
		return S_ALLOC_MEM_ERROR;

	status_t status = node_set(n, value, size);
	if (status != S_OK)
	{
		node_delete(n);
		return status;
	}

	*dst = n;
	return S_OK;
}





/* Public functions */
