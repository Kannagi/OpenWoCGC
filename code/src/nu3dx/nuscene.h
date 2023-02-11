#ifndef NUSCENE_H
#define NUSCENE_H

#include "../types.h"
#include <stdlib.h>
#include "nu3dxtypes.h"


/**********************************************************/

/****** I don't know what headers these should go in ******/
\\void GS_ARGBTORGBA(u32* colors, u32 count);
\\NuAnimData* NuAnimDataRead(fileHandle handle);
/**********************************************************/

void ReadNuIFFTextureSet(fileHandle handle, NuSceneMtl* mtl);
void ReadNuIFFMaterialSet(fileHandle handle, NuSceneMtl* mtl);
void NuSceneMtlUpdate(NuSceneMtl* mtl);
void ReadNuIFFAnimationLibrary(fileHandle handle, NuScene* scene);
void ReadNuIFFGSplineSet(fileHandle handle, NuScene* scene);
void ReadNuIFFGobjSet(fileHandle handle, NuSceneMtl* mtl);
NuGobj* ReadNuIFFGeom(fileHandle handle, UNKWORD param_2);
void ReadNuIFFGeomDef(fileHandle handle, NuSceneMtl* mtl);
void ReadNuIFFGeomVtx(fileHandle handle, NuGeom* geom);
void ReadNuIFFGeomCntrl(fileHandle handle, NuGeom* geom);
void ReadNuIFFFaceOnGeom(fileHandle handle, NuFaceOnGeom* face);
void ReadNuIFFGeomPrim(fileHandle handle, NuGeom* geom);
void ReadNuIFFGeomTri(fileHandle handle, NuGeom* geom, s32 param_3);
void ReadNuIFFGeomSkin(fileHandle handle, NuGeom* geom);
void ReadNuIFFBlendShape(fileHandle handle, NuGeom* geom);
void ReadNuIFFUnknown(fileHandle handle, u32 blockMagic);
int ReadNuIFFInstSet(fileHandle handle, UNKTYPE* param_2, UNKTYPE* param_3);
void ReadNuIFFSpecialObjects(fileHandle handle, NuScene* scene);
void NuSceneCalcCulling(NuScene* scene);
void ReadNuIFFGScene(fileHandle handle, NuScene* scene);
NuScene* NuSceneLoad(const char *name);
size_t ReadNuIFFNameTable(fileHandle handle);

#endif // !NUSCENE_H