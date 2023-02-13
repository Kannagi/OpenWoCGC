#pragma once
#include "types.h"
#include "nu3dx/nu3dxtypes.h"
#include "nuraster/nurastertypes.h"

/*
  800cb6d8 0000a4 800cb6d8  4 GS_TexInit 	Global
  800cb77c 0000c0 800cb77c  4 GS_TexReInit 	Global
  800cb83c 000114 800cb83c  4 GS_TexSwizzleRGB5A3 	Global
  800cb950 0003b4 800cb950  4 GS_TexCreateNU 	Global
  800cbd04 0000a4 800cbd04  4 GS_ChangeTextureStates 	Global
  800cbda8 000038 800cbda8  4 GS_TexSetWrapModes 	Global
  800cbde0 000038 800cbde0  4 GS_TexSetWrapModet 	Global
  800cbe18 00025c 800cbe18  4 GS_TexSelect 	Global
  800cc074 000058 800cc074  4 GS_SetTevBlend 	Global
  800cc0cc 000024 800cc0cc  4 GS_SetTevModulate 	Global
  800cc0f0 000004 800cc0f0  4 GS_SetTextureStageState 	Global
*/

//Gamecube SDK
enum GXTexWrapMode
{
    GX_MAX_TEXWRAPMODE = 3,
    GX_MIRROR = 2,
    GX_REPEAT = 1,
    GX_CLAMP = 0
};

//Gamecube SDK
enum _GXTevStageID
{
    GX_MAX_TEVSTAGE = 16,
    GX_TEVSTAGE15 = 15,
    GX_TEVSTAGE14 = 14,
    GX_TEVSTAGE13 = 13,
    GX_TEVSTAGE12 = 12,
    GX_TEVSTAGE11 = 11,
    GX_TEVSTAGE10 = 10,
    GX_TEVSTAGE9 = 9,
    GX_TEVSTAGE8 = 8,
    GX_TEVSTAGE7 = 7,
    GX_TEVSTAGE6 = 6,
    GX_TEVSTAGE5 = 5,
    GX_TEVSTAGE4 = 4,
    GX_TEVSTAGE3 = 3,
    GX_TEVSTAGE2 = 2,
    GX_TEVSTAGE1 = 1,
    GX_TEVSTAGE0 = 0
};
