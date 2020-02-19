#ifndef K_CMAP_H
#define K_CMAP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "cerror.h"
#include "citer.h"

	typedef unsigned int hash_t;

	typedef struct CRawMap CRawMap;

	typedef struct CMapNode {
		CRawMap* owner;

		hash_t hash;
		char* key;

		void* data;
		size_t data_size;

		struct CMapNode* next;
	} CMapNode;

	struct CRawMap
	{
		CMapNode** buckets;
		size_t n_buckets;
		size_t size;
	};


	status_t crawmap_init(CRawMap* m);
	void crawmap_deinit(CRawMap* m);

	CRawMap* crawmap_new();
	void crawmap_delete(CRawMap* m);

	status_t crawmap_resize(CRawMap* m);


	/* Put operations */

	status_t crawmap_put(CRawMap* m, const char* key, const void* value, const size_t size);


	/* Get operations */

	status_t crawmap_get(CRawMap* m, const char* key, void** ptr);


	/* Find operations */

	CMapNode* crawmap_find(CRawMap* m, const char* key);


	/* Erase operations */

	int crawmap_erase(CRawMap* m, const char* key);
	int crawmap_erase_node(CRawMap* m, CMapNode* node);
	void crawmap_clear(CRawMap* m);


	/* Iterator operations */

	CIterator crawmap_iterator(CRawMap* m);


#ifdef __cplusplus
}
#endif
#endif
