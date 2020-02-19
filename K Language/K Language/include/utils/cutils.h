#ifndef K_CUTILS_H
#define K_CUTILS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <string.h>


#define cnew(_Type) malloc(sizeof(_Type))
#define cnewarray(_Type, _Elements) malloc(sizeof(_Type) * (_Elements))
#define cdelete(_Ptr) free(_Ptr)

#define carray_fill(_Type, _Ptr, _Value, _Elements) memset((_Ptr), (_Value), sizeof(_Type) * (_Elements))

#define cnew_strcpy(_Str) malloc(strlen((_Str)))

#ifdef __cplusplus
}
#endif
#endif
