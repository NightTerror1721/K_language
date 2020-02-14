#ifndef K_CERROR_H
#define K_CERROR_H

#ifdef __cplusplus
extern "C" {
#endif

	typedef enum status_t
	{
		S_OK = 0,
		S_UNKNOWN_ERROR = 1,

		/* NULL errors */
		S_UNEXPECTED_NULL,

		/* parameter errors */
		S_INVALID_PARAMETER,

		/* mem errors */
		S_ALLOC_MEM_ERROR,
		S_EMPTY_MEM

	} status_t;

#ifdef __cplusplus
}
#endif
#endif
