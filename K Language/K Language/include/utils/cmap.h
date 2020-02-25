#ifndef K_CMAP_H
#define K_CMAP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "cerror.h"
#include <stdlib.h>

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

	typedef struct {
		size_t bucket;
		CMapNode* node;
	} CMapIterator;


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

	CMapIterator crawmap_find(CRawMap* m, const char* key);


	/* Erase operations */

	int crawmap_erase(CRawMap* m, const char* key);
	int crawmap_erase_it(CRawMap* m, CMapIterator it);
	void crawmap_clear(CRawMap* m);


	/* Iterator operations */

	CMapIterator crawmap_iter_begin(CRawMap* m);
	CMapIterator crawmap_iter_end(CRawMap* m);
	int crawmap_iter_equals(const CMapIterator it0, const CMapIterator it1);
	int crawmap_iter_next(CMapIterator* it);
	int crawmap_iter_get(CMapIterator it, void** ptr);
	const char* crawmap_iter_get_key(CMapIterator it);



#define CMap(_Type) struct { CRawMap base; _Type temp; _Type* ref; }

	void* _cmap_new(const size_t type_size);
	void _cmap_delete(void*);

#define cmap_init(_Map) crawmap_init(&(_Map)->base)
#define cmap_deinit(_Map) crawmap_deinit(&(_Map)->base)

#define cmap_new(_Type) _cmap_new(sizeof(_Type))
#define cmap_delete(_Map) _cmap_delete((_Map))

#define cmap_resize(_Map) crawmap_resize(&(_Map)->base)

#define cmap_put(_Map, _Key, _Value) ((_Map)->temp = (_Value), crawmap_put(&(_Map)->base, (_Key), &(_Map)->temp, sizeof((_Map)->temp)))

#define cmap_get(_Map, _Key) (crawmap_get(&(_Map)->base, (_Key), (void**)(&(_Map)->ref)), (_Map)->ref)

#define cmap_find(_Map, _Key) crawmap_find(&(_Map)->base, (_Key))

#define cmap_erase(_Map, _Key) crawmap_erase(&(_Map)->base, (_Key))
#define cmap_erase_it(_Map, _Iter) crawmap_erase_it(&(_Map)->base, (_Iter))
#define cmap_clear(_Map) crawmap_clear(&(_Map)->base)

#define cmap_iter_begin(_Map) crawmap_iter_begin(&(_Map)->base)
#define cmap_iter_end(_Map) crawmap_iter_end(&(_Map)->base)
#define cmap_iter_equals = crawmap_iter_equals
#define cmap_iter_next = crawmap_iter_next
#define cmap_iter_get(_Map, _Iter) ( crawmap_iter_get(_Iter, (void**)(&(_Map)->ref)), (_Map)->ref )
#define cmap_iter_get_key(_Iter) crawmap_iter_get_key(&(_Map)->base)




#ifdef __cplusplus
}
#endif
#endif
