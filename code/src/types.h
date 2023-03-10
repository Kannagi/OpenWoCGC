#ifndef TYPES_H
#define TYPES_H

#include "elf_systypes.h"
#include<stdbool.h>

#define NULL 0
#define NuError(msg) \
    error_func e = NuErrorProlog(__FILE__, __LINE__); \
    e(msg);

// Unsigned byte.
typedef unsigned char u8;

// Signed byte.
typedef signed char s8;

// Unsigned short.
typedef unsigned short u16;

// Signed short.
typedef signed short s16;

// Unsigned word.
typedef unsigned int u32;

// Signed word.
typedef signed int s32;

// Unsigned  double word.
typedef unsigned long long u64;

// Signed double word.
typedef signed long long s64;

// 32-bit float.
typedef float f32;

// 64-bit float.
typedef double f64;

// Angle from [0x0, 0x10000). 0x0 represents 0 where 0x10000 is 2pi, so an increment is 2pi/0x10000.
typedef unsigned short angle;

// Fixed point value.
typedef signed int fxi;

// Error function.
typedef void(*error_func)(char[0x60], ...);

// File handle.
typedef s32 fileHandle;

union variptr_u superbuffer_ptr;
union variptr_u superbuffer_base;
union variptr_u superbuffer_end;

#include "numath/numathtypes.h"
#endif // !TYPES_H
