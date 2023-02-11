#ifndef NUCAMERA_H
#define NUCAMERA_H

#include "../types.h"
#include "nu3dxtypes.h"
#include "numath.h"

/*
  800ac788 0003b4 800ac788  4 NuCameraCalcFrustrumPlanes 	Global          See https://github.com/microsoft/DirectX-Graphics-Samples/blob/master/MiniEngine/Core/Math/Frustum.h
  800acb3c 0003a4 800acb3c  4 NuCameraSet 	Global
  800acf10 0000e4 800acf10  4 NuCameraTransformView 	Global
  800acff4 0000e4 800acff4  4 NuCameraTransformClip 	Global
  800ad0d8 000274 800ad0d8  4 NuCameraClipTestExtents 	Global
  800ad34c 0000d0 800ad34c  4 NuCameraClipTestBoundingSphere 	Global
  800ad41c 00016c 800ad41c  4 NuCameraClipTestPoints 	Global
  800ad680 00017c 800ad680  4 SetProjectionMatrix 	Global
  800ad804 0000e4 800ad804  4 NuCameraTransformScreenClip 	Global
*/

// Global camera.
extern nucamera_s global_camera;

// View matrix.
extern Mtx* vmtx;

// Projection matrix.
extern Mtx pmtx;

static Mtx vpmtx;

// VPCS matrix.
extern Mtx vpcsmtx;

static Mtx vpcmtx;

static Mtx ivpcsmtx;

// Camera clipping.
extern u32 clip_enable;

// Camera axes.
extern Vec cam_axes;

static struct Vec4 frustrumplanes[6];

static Mtx vpc_vport_mtx;

static Mtx cmtx;

static Mtx smtx;

static Mtx csmtx;

static Mtx icsmtx;

static Mtx* glassMtx[256]; 

static Mtx smtx;

static s32 clip_enable; 

static Vec4 frustrumplanes[6];


// Create a new camera.
nucamera_s* NuCameraCreate();

// NuCameraCalcFrustrumPlanes

// NuCameraSet

// Get the camera matrix.
Mtx* NuCameraGetMtx();

// Get the view matrix.
Mtx* NuCameraGetViewMtx();

// Get the projection matrix.
Mtx* NuCameraGetProjectionMtx();

// Get the VPCS matrix.
Mtx* NuCameraGetVPCSMtx();

// NuCameraTransformView

// NuCameraTransformClip

// NuCameraClipTestExtents

// NuCameraClipTestBoundingSphere

// NuCameraClipTestPoints

// Get the squared distance from the camera to the point.
f32 NuCameraDistSqr(Vec* point);

// Fix the axes of a matrix.
void FixAxes(Mtx* m);

// SetProjectionMatrix

// Enable or disable camera clipping.
void NuCameraEnableClipping(u32 enable);

// NuCameraTransformScreenClip

#endif // !NUCAMERA_H