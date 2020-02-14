#ifndef K_CUTILS_H
#define K_CUTILS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>

#define cnew(_Type) malloc(sizeof(_Type))
#define cnewarray(_Type, _Elements) malloc(sizeof(_Type) * (_Elements))
#define cdelete(_Ptr) free(_Ptr)

#ifdef __cplusplus
}
#endif
#endif
