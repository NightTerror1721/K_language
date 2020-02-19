#ifndef K_CBUFFER_H
#define K_CBUFFER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stdarg.h>

#include "cerror.h"
#include "citer.h"

	typedef struct
	{
		char* data;
		size_t size;
		size_t capacity;

	} CBuffer;

	status_t cbuf_init(CBuffer* b);
	void cbuf_deinit(CBuffer* b);

	CBuffer* cbuf_new(void);
	void cbuf_delete(CBuffer* b);

	status_t cbuf_ensure(CBuffer* b, const size_t new_capacity);

	status_t cbuf_skip(CBuffer* b, const size_t count);

	void cbuf_flush(CBuffer* b);


	/* String operations */

	status_t cbuf_putc(CBuffer* b, int c);
	status_t cbuf_puts_s(CBuffer* b, const char* str, const size_t str_size);
	status_t cbuf_puts(CBuffer* b, const char* str);

	status_t cbuf_vprintf(CBuffer* b, const char* fmt, va_list vp);
	status_t cbuf_printf(CBuffer* b, const char* fmt, ...);

	status_t cbuf_popc(CBuffer* b, int* c);
	status_t cbuf_pops_s(CBuffer* b, char* str, const size_t str_size);

	status_t cbuf_vscanf(CBuffer* b, const char* fmt, va_list vp);
	status_t cbuf_scanf(CBuffer* b, const char* fmt, ...);


	/* Raw data operations */

	status_t cbuf_write(CBuffer* b, const void* ptr, const size_t size);
	status_t cbuf_read(CBuffer* b, void* ptr, const size_t ptr_buffer_size);


	/* Iterator funcs */

	CIterator cbuf_iterator(CBuffer* b);


	/* inline functions */

	inline size_t cbuf_size(const CBuffer* b) { return b->size; }
	inline size_t cbuf_capacity(const CBuffer* b) { return b->capacity; }
	inline char* cbuf_rawdata(CBuffer* b) { return b->data; }
	
#define cbuf_pops cbuf_pops_s


#ifdef __cplusplus
}
#endif
#endif
