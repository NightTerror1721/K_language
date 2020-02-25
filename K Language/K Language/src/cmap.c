#include "utils/cmap.h"

#include <string.h>
#include "utils/cutils.h"


/* Private functions */

#define DEFAULT_SIZE 32
#define LOAD_FACTOR 0.7f

typedef CMapNode Node;

static hash_t do_hash(const char* str)
{
	hash_t hash = 5381;
	while (*str)
		hash = ((hash << 5) + hash) ^ *(str++);

	return hash;
}


static Node* node_new(CRawMap* owner, const hash_t hash, const char* key, const size_t size)
{
	Node* n = cnew(Node);
	if (!n)
		return NULL;

	n->key = cnew_strcpy(key);
	if (!n->key)
	{
		cdelete(n);
		return NULL;
	}

	n->data_size = size;
	n->data = malloc(size);
	if (!n->data)
	{
		cdelete(n->key);
		cdelete(n);
		return NULL;
	}

	n->owner = owner;
	n->hash = hash;
	n->next = NULL;
	
	return n;
}

static void node_delete(Node* n)
{
	if (!n)
		return;

	if (n->data)
		cdelete(n->data);

	if (n->key)
		cdelete(n->key);

	cdelete(n);
}

static size_t _get_bucket(CRawMap* m, const hash_t hash)
{
	return hash % m->n_buckets;
}

static void _move_node(CRawMap* m, Node* n, const hash_t hash, const size_t size)
{
	n->hash = hash;
	n->next = NULL;

	size_t idx = _get_bucket(m, hash);
	Node* next = m->buckets[idx];

	if (next)
		n->next = next;
	m->buckets[idx] = n;
}

static status_t _new_or_same_node_on_bucket(Node** dst, int* increased, CRawMap* m, const char* key, const size_t size)
{
	*dst = NULL;
	*increased = 0;

	hash_t hash = do_hash(key);
	size_t idx = _get_bucket(m, hash);

	Node* next = m->buckets[idx], *node = NULL;
	if (next)
	{
		for (; next; next = next->next)
		{
			if (strcmp(next->key, key) == 0)
			{
				node = next;
				break;
			}
		}
	}
	if (!node)
	{
		node = node_new(m, hash, key, size);
		if (!node)
			return S_ALLOC_MEM_ERROR;

		node->next = m->buckets[idx];
		m->buckets[idx] = node;
		++m->size;
		*increased = 1;
	}

	*dst = node;
	return S_OK;
}

static void _reinsert(CRawMap* m, Node* node)
{
	_move_node(m, node, node->hash, node->data_size);
}

static void _erase_node(CRawMap* m, Node* node)
{
	hash_t hash = node->hash;
	size_t idx = _get_bucket(m, hash);

	if (node == m->buckets[idx])
	{
		m->buckets[idx] = node->next;
		node_delete(node);
		--m->size;
	}
	else
	{
		Node *prev = m->buckets[idx], *curr = prev->next;
		for (; curr; prev = curr, curr = prev->next)
		{
			if (curr == node)
			{
				prev->next = curr->next;
				node_delete(node);
				--m->size;
				break;
			}
		}
	}
}

static Node* _find_node(CRawMap* m, const char* key)
{
	if (!m)
		return NULL;

	hash_t hash = do_hash(key);
	Node* node = m->buckets[_get_bucket(m, hash)];

	for (; node; node = node->next)
		if (strcmp(node->key, key) == 0)
			return node;

	return NULL;
}







/* Public functions */

status_t crawmap_init(CRawMap* m)
{
	m->buckets = cnewarray(Node*, DEFAULT_SIZE);
	if (!m->n_buckets)
		return S_ALLOC_MEM_ERROR;

	m->n_buckets = DEFAULT_SIZE;
	m->size = 0;

	carray_fill(Node*, m->buckets, 0, m->n_buckets);
	return S_OK;
}

void crawmap_deinit(CRawMap* m)
{
	if (!m)
		return;

	crawmap_clear(m);

	cdelete(m->buckets);
	m->buckets = NULL;
	m->n_buckets = 0;
}

CRawMap* crawmap_new()
{
	CRawMap* m = cnew(CRawMap);
	crawmap_init(m);
	return m;
}

void crawmap_delete(CRawMap* m)
{
	if (m)
	{
		crawmap_deinit(m);
		cdelete(m);
	}
}

status_t crawmap_resize(CRawMap* m)
{
	if (!m)
		return S_UNEXPECTED_NULL;

	Node** old_buckets = m->buckets;
	size_t old_n_buckets = m->n_buckets;

	m->n_buckets = m->n_buckets * 2;
	m->buckets = cnewarray(Node*, m->n_buckets);
	if (!m->buckets)
	{
		cdelete(m->buckets);
		m->buckets = old_buckets;
		m->n_buckets = old_n_buckets;
		return S_ALLOC_MEM_ERROR;
	}

	carray_fill(Node*, m->buckets, 0, m->n_buckets);
	for (size_t i = 0; i < old_n_buckets; ++i)
	{
		Node* node = old_buckets[i];
		if (node)
			_reinsert(m, node);
	}

	cdelete(old_buckets);
	return S_OK;
}


status_t crawmap_put(CRawMap* m, const char* key, const void* value, const size_t size)
{
	if (!m)
		return S_UNEXPECTED_NULL;

	Node* node;
	status_t status;
	int increased;
	if ((status = _new_or_same_node_on_bucket(&node, &increased, m, key, size)) != S_OK)
		return status;

	if (node->data_size == size)
	{
		if (memcpy_s(node->data, size, value, size) != 0)
		{
			if (increased)
				_erase_node(m, node);
			return S_MEMCPY_ERROR;
		}
	}
	else
	{
		void* newdata = realloc(node->data, size);
		if (!newdata)
		{
			if (increased)
				_erase_node(m, node);
			return S_ALLOC_MEM_ERROR;
		}
		if (memcpy_s(node->data, size, value, size) != 0)
		{
			if (increased)
				_erase_node(m, node);
			return S_MEMCPY_ERROR;
		}

		node->data = newdata;
		node->data_size = size;
	}

	if (increased && ((float)m->size / (float)m->n_buckets) > LOAD_FACTOR)
		return crawmap_resize(m);
	return S_OK;
}


status_t crawmap_get(CRawMap* m, const char* key, void** ptr)
{
	*ptr = NULL;

	if (!m)
		return S_UNEXPECTED_NULL;

	Node* node = _find_node(m, key);
	*ptr = node ? node->data : NULL;

	return S_OK;
}


CMapIterator crawmap_find(CRawMap* m, const char* key)
{
	Node* n = _find_node(m, key);
	if(!n)
		return crawmap_iter_end(m);
	
	CMapIterator it;
	it.node = n;
	it.bucket = _get_bucket(m, n->hash);
	return it;
}



int crawmap_erase(CRawMap* m, const char* key)
{
	if (!m)
		return 0;

	Node* n = _find_node(m, key);
	if (n)
	{
		_erase_node(m, n);
		return 1;
	}

	return 0;
}

int crawmap_erase_it(CRawMap* m, CMapIterator it)
{
	if (it.node && it.node->owner == m)
	{
		_erase_node(m, it.node);
		return 1;
	}
	return 0;
}

void crawmap_clear(CRawMap* m)
{
	if (!m)
		return;

	for (size_t i = 0; i < m->n_buckets; ++i)
	{
		Node* n = m->buckets[i];
		if (n)
		{
			Node* next;
			while (n)
			{
				next = n->next;
				node_delete(n);
				n = next;
			}
			m->buckets[i] = NULL;
		}
	}

	m->size = 0;
}






CMapIterator crawmap_iter_begin(CRawMap* m)
{
	CMapIterator it;
	if (m->size < 1)
	{
		it.node = NULL;
		it.bucket = m->n_buckets;
		return it;
	}

	for (size_t i = 0; i < m->n_buckets; ++i)
	{
		Node* n = m->buckets[i];
		while (n)
		{
			if (n != NULL)
			{
				it.node = n;
				it.bucket = i;
				return it;
			}
			n = n->next;
		}
	}
	
	it.node = NULL;
	it.bucket = m->n_buckets;
	return it;
}
CMapIterator crawmap_iter_end(CRawMap* m)
{
	CMapIterator it = { m->n_buckets, NULL };
	return it;
}
int crawmap_iter_equals(const CMapIterator it0, const CMapIterator it1)
{
	if (&it0 == &it1)
		return 1;

	if (!it0.node)
		return !it1.node;
	if (!it1.node)
		return !it0.node;

	return it0.bucket == it1.bucket && it0.node == it1.node;
}
int crawmap_iter_next(CMapIterator* it)
{
	if (!it->node)
		return 0;

	Node* n = it->node;
	if (n->next)
	{
		it->node = n->next;
		return 1;
	}

	CRawMap* m = n->owner;
	for (size_t i = it->bucket + 1; i < m->n_buckets; ++i)
	{
		if (m->buckets[i] != NULL)
		{
			it->node = m->buckets[i];
			it->bucket = i;
			return 1;
		}
	}

	it->node = NULL;
	it->bucket = m->n_buckets;
	return 0;
}
int crawmap_iter_get(CMapIterator it, void** ptr)
{
	if (!it.node)
		return 0;

	*ptr = it.node->data;
	return 1;
}
const char* crawmap_iter_get_key(CMapIterator it)
{
	if (!it.node)
		return NULL;

	return it.node->key;
}






void* _cmap_new(const size_t type_size)
{
	size_t size = sizeof(CRawMap) + type_size + sizeof(void*);
	void* p = malloc(size);
	if (!p)
		return NULL;

	memset(p, 0, size);
	crawmap_init((CRawMap*)(p));
	return p;
}
void _cmap_delete(void* pm)
{
	if (pm)
	{
		crawmap_deinit((CRawMap*)pm);
		cdelete(pm);
	}
}
