#ifndef SKINNED_H
#define SKINNED_H

#include "gs.h"
#include "system/gs.h"

struct _GSMATRIX* CV_SKINMTX;
struct _GS_VERTEXSKIN* GS_SkinVertexSource;
struct _GS_VECTOR3* GS_BlendSource;
int MaxSkinVerts;
int SkinLights;
float c_one;

#endif // !SKINNED_H
