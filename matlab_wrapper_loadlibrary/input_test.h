#ifndef _INPUT_TEST_H
#define _INPUT_TEST_H

/*=========================================================================
| TYPEDEFS
 ========================================================================*/
#ifndef TOTALPHASE_DATA_TYPES
#define TOTALPHASE_DATA_TYPES
#ifndef _MSC_VER
/* C99-compliant compilers (GCC) */
#include <stdint.h>
typedef uint8_t   u08;
typedef uint16_t  u16;
typedef uint32_t  u32;
typedef uint64_t  u64;
typedef int8_t    s08;
typedef int16_t   s16;
typedef int32_t   s32;
typedef int64_t   s64;
#else
/* Microsoft compilers (Visual C++) */
typedef unsigned __int8   u08;
typedef unsigned __int16  u16;
typedef unsigned __int32  u32;
typedef unsigned __int64  u64;
typedef signed   __int8   s08;
typedef signed   __int16  s16;
typedef signed   __int32  s32;
typedef signed   __int64  s64;
#endif /* __MSC_VER */
typedef float   f32;
typedef double  f64;
#endif /* TOTALPHASE_DATA_TYPES */


#ifdef __cplusplus
extern "C" {
#endif
int input_test(int64_t n, int64_t m, int64_t *ei, int64_t *ej, double *w);
#ifdef __cplusplus
}
#endif

#endif

