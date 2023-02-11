#ifndef NUMATHTYPES_H
#define NUMATHTYPES_H

#include "types.h"

// Size: 0xC // 3D vector.
typedef struct
{
	f32 x;
	f32 y;
	f32 z;
} Vec;


struct nuvec_s
{
	f32 x;
	f32 y;
	f32 z;
};

// Vector with 4 slots.
typedef struct
{
	f32 x;
	f32 y;
	f32 z;
	f32 w;
} Vec4;

// Size: 0x10
struct nuivec4_s
{
	int x;
	int y;
	int z;
	int w;
};

// Size: 0xC
struct nuivec_s
{
	int x;
	int y;
	int z;
};

// 4x4 matrix.
typedef struct
{
	f32 m11, m12, m13, m14;
	f32 m21, m22, m23, m24;
	f32 m31, m32, m33, m34;
	f32 m41, m42, m43, m44;
} Mtx; // Size: 0x40

typedef struct numtx_s
{
	f32 mtx[4][4];
};

// Quaternion.
typedef struct
{
	float x;
	float y;
	float z;
	float w;
} Quat;

#endif // !NUMATHTYPES_H
