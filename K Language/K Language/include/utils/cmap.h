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
		void* data;
		size_t data_size;

		struct CMapNode* next;
		struct CMapNode* prev;
	} CMapNode;

	struct CRawMap
	{
		CMapNode** buckets;
		size_t n_buckets;
		size_t size;
	};


	status_t crawmap_init(CRawMap* m);

	CRawMap* crawmap_new();
	void crawmap_delete(CRawMap* m);



#ifdef __cplusplus
}
#endif
#endif
