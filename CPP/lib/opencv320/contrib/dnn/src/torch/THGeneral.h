#ifndef TH_GENERAL_INC
#define TH_GENERAL_INC

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#include <limits.h>
#include <float.h>
#include <time.h>
#include <string.h>

#ifdef __cplusplus
# define TH_EXTERNC extern "C"
#else
# define TH_EXTERNC extern
#endif

#define TH_API TH_EXTERNC

#define THInf DBL_MAX

//#define TH_INLINE @TH_INLINE@

#ifndef __cplusplus
//#define inline @TH_INLINE@
#endif

#ifndef M_PI
# define M_PI 3.14159265358979323846
#endif

TH_API double THLog1p(const double x);
TH_API void _THError(const char *file, const int line, const char *fmt, ...);
TH_API void _THAssertionFailed(const char *file, const int line, const char *exp, const char *fmt, ...);
TH_API void THSetErrorHandler( void (*torchErrorHandlerFunction)(const char *msg, void *data), void *data );
TH_API void _THArgCheck(const char *file, int line, int condition, int argNumber, const char *fmt, ...);
TH_API void THSetArgErrorHandler( void (*torchArgErrorHandlerFunction)(int argNumber, const char *msg, void *data), void *data );
TH_API void* THAlloc(long size);
TH_API void* THRealloc(void *ptr, long size);
TH_API void THFree(void *ptr);
TH_API void THSetGCHandler( void (*torchGCHandlerFunction)(void *data), void *data );
// this hook should only be called by custom allocator functions
TH_API void THHeapUpdate(long size);

#define THError(...) _THError(__FILE__, __LINE__, __VA_ARGS__)
#define THArgCheck(...) _THArgCheck(__FILE__, __LINE__, __VA_ARGS__)
#define THAssert(exp)                                                 \
do {                                                                  \
  if (!(exp)) {                                                       \
    _THAssertionFailed(__FILE__, __LINE__, #exp, "");                 \
  }                                                                   \
} while(0)
#define THAssertMsg(exp, ...)                                         \
do {                                                                  \
  if (!(exp)) {                                                       \
    _THAssertionFailed(__FILE__, __LINE__, #exp, __VA_ARGS__);        \
  }                                                                   \
} while(0)

#define TH_CONCAT_STRING_2(x,y) TH_CONCAT_STRING_2_EXPAND(x,y)
#define TH_CONCAT_STRING_2_EXPAND(x,y) #x #y

#define TH_CONCAT_STRING_3(x,y,z) TH_CONCAT_STRING_3_EXPAND(x,y,z)
#define TH_CONCAT_STRING_3_EXPAND(x,y,z) #x #y #z

#define TH_CONCAT_STRING_4(x,y,z,w) TH_CONCAT_STRING_4_EXPAND(x,y,z,w)
#define TH_CONCAT_STRING_4_EXPAND(x,y,z,w) #x #y #z #w

#define TH_CONCAT_2(x,y) TH_CONCAT_2_EXPAND(x,y)
#define TH_CONCAT_2_EXPAND(x,y) x ## y

#define TH_CONCAT_3(x,y,z) TH_CONCAT_3_EXPAND(x,y,z)
#define TH_CONCAT_3_EXPAND(x,y,z) x ## y ## z

#define TH_CONCAT_4_EXPAND(x,y,z,w) x ## y ## z ## w
#define TH_CONCAT_4(x,y,z,w) TH_CONCAT_4_EXPAND(x,y,z,w)

#define THMin(X, Y)  ((X) < (Y) ? (X) : (Y))
#define THMax(X, Y)  ((X) > (Y) ? (X) : (Y))

#if (defined(_MSC_VER) || defined(__MINGW32__))
# define log1p(x) THLog1p(x)
#define snprintf _snprintf
#define popen _popen
#define pclose _pclose
#endif

#endif
