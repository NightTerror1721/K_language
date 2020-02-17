#include "utils/cbuffer.h"

#include <string.h>
#include <stdio.h>
#include "utils/cutils.h"

#define DEFAULT_CAPACITY 16
#define STR_END '\0'
#define BUFFER_SIZE sizeof(CBuffer)

#define IS_EMPTY(_Buffer) ((_Buffer)->size == 0)

#define REMAINING_CAPACITY(_Buffer) ((_Buffer)->capacity - (_Buffer)->size - 1)

#define EXPAND_CAPACITY(_Buffer) cbuf_ensure((_Buffer), (_Buffer)->capacity * 2)

#define TRY_ENSURE(_Buffer, _Amount) if((_Buffer)->size + (_Amount) >= (_Buffer)->capacity - 1) \
	if((_Amount) >= (_Buffer)->capacity) { \
		if(cbuf_ensure((_Buffer), (_Amount) + DEFAULT_CAPACITY)) \
			return S_ALLOC_MEM_ERROR; \
	} else if(EXPAND_CAPACITY((_Buffer)) != S_OK) \
		return S_ALLOC_MEM_ERROR


status_t cbuf_init(CBuffer* b)
{
	b->capacity = DEFAULT_CAPACITY;
	b->size = 0;

	b->data = cnewarray(CBuffer, DEFAULT_CAPACITY);
	if (!b->data)
		return S_ALLOC_MEM_ERROR;

	b->data[0] = STR_END;
	return S_OK;
}

CBuffer* cbuf_new()
{
	CBuffer* b = cnew(CBuffer);
	if (b)
		if (cbuf_init(b) != S_OK)
			return NULL;
	return b;
}

void cbuf_delete(CBuffer* b)
{
	if (b)
		cdelete(b);
}

status_t cbuf_ensure(CBuffer* b, const size_t new_capacity)
{
	if (!b)
		return S_UNEXPECTED_NULL;

	if (new_capacity < 1)
		return S_INVALID_PARAMETER;

	if (new_capacity == b->capacity)
		return S_OK;

	char* newdata = cnewarray(CBuffer, new_capacity);
	if (!newdata)
		return S_ALLOC_MEM_ERROR;

	memcpy_s(newdata, new_capacity, b->data, b->capacity);
	cdelete(b->data);
	b->capacity = new_capacity;
	b->data = newdata;

	return S_OK;
}

status_t cbuf_skip(CBuffer* b, const size_t count)
{
	if (count >= b->size)
	{
		cbuf_flush(b);
		return S_OK;
	}

	if (memmove_s(b->data, count, b->data + count, count) != 0)
		return S_UNKNOWN_ERROR;
	b->size -= count;

	return S_OK;
}

void cbuf_flush(CBuffer* b)
{
	b->data[0] = STR_END;
	b->size = 0;
}

status_t cbuf_putc(CBuffer* b, int c)
{
	TRY_ENSURE(b, 1);
	b->data[b->size++] = (char)(c & 0xff);
	b->data[b->size] = STR_END;
	return S_OK;
}

status_t cbuf_puts_s(CBuffer* b, const char* str, const size_t str_size)
{
	if (str_size < 1)
		return S_OK;

	TRY_ENSURE(b, str_size);
	memcpy_s(b->data + b->size, REMAINING_CAPACITY(b), str, str_size);
	b->size += str_size;
	b->data[b->size] = STR_END;
	return S_OK;
}

status_t cbuf_puts(CBuffer* b, const char* str)
{
	return cbuf_puts_s(b, str, strlen(str));
}

status_t cbuf_vprintf(CBuffer* b, const char* fmt, va_list vp)
{
	va_list args;
	size_t remaining;
	int printed = 0;

	for(;;)
	{
		va_copy(args, vp);
		remaining = REMAINING_CAPACITY(b);
		printed = vsnprintf(b->data + b->size, remaining, fmt, args);
		va_end(args);
		if (printed < 0)
			return S_UNKNOWN_ERROR;
		else if (printed == 0)
			return S_OK;

		if ((size_t)printed > remaining)
		{
			TRY_ENSURE(b, (size_t)(printed));
			continue;
		}
		break;
	}
	b->size += printed;
	b->data[b->size] = STR_END;

	return S_OK;
}

status_t cbuf_printf(CBuffer* b, const char* fmt, ...)
{
	status_t status;
	va_list args;

	va_start(args, fmt);
	status = cbuf_vprintf(b, fmt, args);
	va_end(args);

	return status;
}


status_t cbuf_popc(CBuffer* b, int* c)
{
	if (IS_EMPTY(b))
		return S_EMPTY_MEM;
	
	*c = *b->data & 0xff;
	return cbuf_skip(b, 1);
}

status_t cbuf_pops_s(CBuffer* b, char* str, const size_t str_size)
{
	if (str_size < 1)
		return S_OK;

	if (IS_EMPTY(b))
	{
		*str = STR_END;
		return S_EMPTY_MEM;
	}

	if (str_size >= b->size)
	{
		memcpy_s(str, str_size, b->data, b->size);
		cbuf_flush(b);
		return S_OK;
	}

	memcpy_s(str, str_size, b->data, str_size);
	memmove_s(b->data, str_size, b->data + str_size, str_size);
	return S_OK;
}

status_t cbuf_vscanf(CBuffer* b, const char* fmt, va_list vp)
{
	if (IS_EMPTY(b))
		return S_EMPTY_MEM;

	va_list args;
	int scanned;

	va_copy(args, vp);
	scanned = vsscanf(b->data, fmt, args);
	va_end(args);

	if (scanned > 0)
		cbuf_flush(b);
	return S_OK;
}

status_t cbuf_scanf(CBuffer* b, const char* fmt, ...)
{
	status_t status;
	va_list args;

	va_start(args, fmt);
	status = cbuf_vscanf(b, fmt, args);
	va_end(args);

	return status;
}



status_t cbuf_write(CBuffer* b, const void* ptr, const size_t size)
{
	return cbuf_puts_s(b, (const char*)ptr, size);
}

status_t cbuf_read(CBuffer* b, void* ptr, const size_t ptr_buffer_size)
{
	return cbuf_pops_s(b, (char*)ptr, ptr_buffer_size);
}




static int _cbuf_iterator_has_next(void* container, uintmax_t* data)
{
	CBuffer* b = (CBuffer*)container;
	return *data == (uintmax_t)(-1) || *data + 1 < b->size;
}

static void* _cbuf_iterator_next(void* container, uintmax_t* data)
{
	CBuffer* b = (CBuffer*)container;
	if (*data != (uintmax_t)(-1) && *data + 1 >= b->size)
		return NULL;

	return &b->data[++(*data)];
}

CIterator cbuf_iterator(CBuffer* b)
{
	return citer_make(b, (uintmax_t)(-1), &_cbuf_iterator_has_next, &_cbuf_iterator_next);
}
