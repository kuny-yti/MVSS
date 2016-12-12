#ifndef BS_PTR
#define BS_PTR
#include "bs_object.h"


#ifdef  bsAutoPtr_Enable
#define bsAutoPtr(type) bs_ptrref<type>
#define bsAutoPtrMalloc(type, ...) bs_ptrref<type> (bsNew (type(__VA_ARGS__)))
#define bsAutoPtrFree(ptr) ptr.reset()
#define bsAutoPtrNull(type) bs_ptrref<type>()
#else
#define bsAutoPtr(type) type *
#define bsAutoPtrMalloc(type, ...) bsNew (type(__VA_ARGS__))
#define bsAutoPtrFree(ptr) bsDelete (ptr)
#define bsAutoPtrNull(type) NULL
#endif

#define bs_ptr(type) bsAutoPtr(type)
#define bs_malloc(type, ...) bsAutoPtrMalloc(type, __VA_ARGS__)
#define bs_null(type) bsAutoPtrNull(type)
#define bs_free(ptr) bsAutoPtrFree(ptr)


#endif // BS_PTR

