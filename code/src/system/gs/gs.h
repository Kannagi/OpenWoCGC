#pragma once
#include "stdio.h"
#include "nuraster/nurastertypes.h"

/*
  800c8b14 00002c 800c8b14  4 GS_SetFBCopyTexturePause 	Global
  800c8b40 000040 800c8b40  4 GS_SetAlphaCompareForce 	Global
  800c8b80 000058 800c8b80  4 GS_SetAlphaCompare 	Global
  800c8bd8 000024 800c8bd8  4 GS_ARGBTORGBA 	Global
  800c8bfc 0000e0 800c8bfc  4 GS_BeginScene 	Global
  800c8cdc 000020 800c8cdc  4 GS_EndScene 	Global
  800c8cfc 0000fc 800c8cfc  4 GS_Init 	Global
  800c8df8 000088 800c8df8  4 GS_RenderClear 	Global
  800c8e80 000034 800c8e80  4 GS_RenderClearBlack 	Global
  800c8eb4 000030 800c8eb4  4 GS_SetOrthMatrix 	Global
  800c8ee4 000178 800c8ee4  4 GS_DrawFade 	Global
  800c905c 000254 800c905c  4 GS_FlipScreen 	Global
  800c92b0 00003c 800c92b0  4 GS_SetViewport 	Global
  800c92ec 00003c 800c92ec  4 GS_GetViewport 	Global
  800c9328 000090 800c9328  4 GS_SetupFog 	Global
  800c93b8 000004 800c93b8  4 GS_SetRenderState 	Global
  800c93bc 0000b4 800c93bc  4 GS_SetZCompare 	Global
  800c9470 0000bc 800c9470  4 GS_SetBlendSrc 	Global
  800c952c 000030 800c952c  4 GS_SetBlendSrc2 	Global
  800c955c 0000c0 800c955c  4 GS_SetProjectionMatrix 	Global
  800c961c 000118 800c961c  4 GS_SetLightingMatrix 	Global
  800c9734 000094 800c9734  4 GS_SetLightingMatrix2 	Global
  800c97c8 000078 800c97c8  4 GS_LoadMatrix 	Global
  800c9840 0000c4 800c9840  4 GS_SetViewMatrix 	Global
  800c9904 00003c 800c9904  4 GS_LoadWorldMatrixIdentity 	Global
  800c9940 00012c 800c9940  4 GS_SetWorldMatrix 	Global
  800c9a6c 000004 800c9a6c  4 GS_CopyTextureFromFB 	Global
  800c9a70 00008c 800c9a70  4 GS_CopyFBToPause 	Global
*/

void GS_SetRenderState(int state, int data);
void GS_SetBlendSrc(int enable,int src,int dest);


struct nuviewport_s {
    u32 x;
    u32 y;
    u32 width;
    u32 height;
    float zmin;
    float zmax;
    float centre_x;
    float centre_y;
    float clipmin_x;
    float clipmin_y;
    float clipmax_x;
    float clipmax_y;
    float clip_w;
    float clip_h;
    float scis_w;
    float scis_h;
};


enum _GXCompare
{
    GX_ALWAYS = 7,
    GX_GEQUAL = 6,
    GX_NEQUAL = 5,
    GX_GREATER = 4,
    GX_LEQUAL = 3,
    GX_EQUAL = 2,
    GX_LESS = 1,
    GX_NEVER = 0
};


enum gs_texturetype_e
{
    GS_TEX_NUMTEXTURETYPES = 26,
    GS_TEX_RZT16 = 25,
    GS_TEX_RZT24 = 24,
    GS_TEX_RZT32 = 23,
    GS_TEX_RTT16 = 22,
    GS_TEX_RTT24 = 21,
    GS_TEX_RTT32 = 20,
    GS_TEX_RT16 = 19,
    GS_TEX_RT24 = 18,
    GS_TEX_RT32 = 17,
    GS_TEX_RT8H = 16,
    GS_TEX_RTT8 = 15,
    GS_TEX_RT8 = 14,
    GS_TEX_BB = 13,
    GS_TEX_DXT5 = 12,
    GS_TEX_DXT4 = 11,
    GS_TEX_DXT3 = 10,
    GS_TEX_DXT2 = 9,
    GS_TEX_DXT1 = 8,
    GS_TEX_PAL8_S = 7,
    GS_TEX_PAL4_S = 6,
    GS_TEX_PAL8 = 5,
    GS_TEX_PAL4 = 4,
    GS_TEX_RGBA32 = 3,
    GS_TEX_RGB24 = 2,
    GS_TEX_RGBA16 = 1,
    GS_TEX_RGB16 = 0
};

enum _GXTexFmt
{
    GX_TF_A8 = 39,
    GX_CTF_Z16L = 60,
    GX_CTF_Z8L = 58,
    GX_CTF_Z8M = 57,
    GX_CTF_Z4 = 48,
    GX_TF_Z24X8 = 22,
    GX_TF_Z16 = 19,
    GX_TF_Z8 = 17,
    GX_CTF_GB8 = 44,
    GX_CTF_RG8 = 43,
    GX_CTF_B8 = 42,
    GX_CTF_G8 = 41,
    GX_CTF_R8 = 40,
    GX_CTF_A8 = 39,
    GX_CTF_YUVA8 = 38,
    GX_CTF_RA8 = 35,
    GX_CTF_RA4 = 34,
    GX_CTF_R4 = 32,
    GX_TF_CMPR = 14,
    GX_TF_RGBA8 = 6,
    GX_TF_RGB5A3 = 5,
    GX_TF_RGB565 = 4,
    GX_TF_IA8 = 3,
    GX_TF_IA4 = 2,
    GX_TF_I8 = 1,
    GX_TF_I4 = 0
};

enum _GXTexWrapMode
{
    GX_MAX_TEXWRAPMODE = 3,
    GX_MIRROR = 2,
    GX_REPEAT = 1,
    GX_CLAMP = 0
};

// Size: 0x20
struct _GXTexObj
{
    u32 dummy[8]; // Offset: 0x0
};

enum _GXColorSrc
{
    GX_SRC_VTX = 1,
    GX_SRC_REG = 0
};

enum _GXDiffuseFn
{
    GX_DF_CLAMP = 2,
    GX_DF_SIGN = 1,
    GX_DF_NONE = 0
};

enum _GXAttnFn
{
    GX_AF_NONE = 2,
    GX_AF_SPOT = 1,
    GX_AF_SPEC = 0
};

// Size: 0x40, DWARF: 0x8E9296
struct _GSMATRIX
{
    float _11; // Offset: 0x0, DWARF: 0x8E92B4
    float _12; // Offset: 0x4, DWARF: 0x8E92DA
    float _13; // Offset: 0x8, DWARF: 0x8E9300
    float _14; // Offset: 0xC, DWARF: 0x8E9326
    float _21; // Offset: 0x10, DWARF: 0x8E934C
    float _22; // Offset: 0x14, DWARF: 0x8E9372
    float _23; // Offset: 0x18, DWARF: 0x8E9398
    float _24; // Offset: 0x1C, DWARF: 0x8E93BE
    float _31; // Offset: 0x20, DWARF: 0x8E93E4
    float _32; // Offset: 0x24, DWARF: 0x8E940A
    float _33; // Offset: 0x28, DWARF: 0x8E9430
    float _34; // Offset: 0x2C, DWARF: 0x8E9456
    float _41; // Offset: 0x30, DWARF: 0x8E947C
    float _42; // Offset: 0x34, DWARF: 0x8E94A2
    float _43; // Offset: 0x38, DWARF: 0x8E94C8
    float _44; // Offset: 0x3C, DWARF: 0x8E94EE
};

// Size: 0xC, DWARF: 0x8E9114
struct _GS_VECTOR3
{
    float x; // Offset: 0x0, DWARF: 0x8E9134
    float y; // Offset: 0x4, DWARF: 0x8E9158
    float z; // Offset: 0x8, DWARF: 0x8E917C
};

// Size: 0x10, DWARF: 0x8E91C3
struct _GS_VECTOR4
{
    float x; // Offset: 0x0, DWARF: 0x8E91E3
    float y; // Offset: 0x4, DWARF: 0x8E9207
    float z; // Offset: 0x8, DWARF: 0x8E922B
    float w; // Offset: 0xC, DWARF: 0x8E924F
};

// Size: 0x10, DWARF: 0x1B76
struct _D3DCOLORVALUE
{
    float r; // Offset: 0x0, DWARF: 0x1B99
    float g; // Offset: 0x4, DWARF: 0x1BBD
    float b; // Offset: 0x8, DWARF: 0x1BE1
    float a; // Offset: 0xC, DWARF: 0x1C05
};

// Size: 0x44, DWARF: 0x1A76
struct _D3DMATERIAL8
{
    struct _D3DCOLORVALUE Diffuse; // Offset: 0x0, DWARF: 0x1A98
    struct _D3DCOLORVALUE Ambient; // Offset: 0x10, DWARF: 0x1AC4
    struct _D3DCOLORVALUE Specular; // Offset: 0x20, DWARF: 0x1AF0
    struct _D3DCOLORVALUE Emissive; // Offset: 0x30, DWARF: 0x1B1D
    float Power; // Offset: 0x40, DWARF: 0x1B4A
};


enum _GXChannelID
{
    GX_COLOR_NULL = 255,
    GX_ALPHA_BUMPN = 8,
    GX_ALPHA_BUMP = 7,
    GX_COLOR_ZERO = 6,
    GX_COLOR1A1 = 5,
    GX_COLOR0A0 = 4,
    GX_ALPHA1 = 3,
    GX_ALPHA0 = 2,
    GX_COLOR1 = 1,
    GX_COLOR0 = 0
};

// Size: 0x20
/*struct _GXTexObj
{
	UNKWORD mFlags; // at 0x0
	char UNK_0x4[0x4];
	UNKWORD mDimensions; // at 0x8 (Height/width are split between these bits)
	char UNK_0xC[0x4];
	UNKWORD mUserData; // at 0x10
	enum _GXTexFmt mFormat; // at 0x14
	UNKWORD mTLUT; // at 0x18
} GXTexObj;*/

enum _GXLightID
{
    GX_LIGHT_NULL,
    GX_MAX_LIGHT = 256,
    GX_LIGHT7 = 128,
    GX_LIGHT6 = 64,
    GX_LIGHT5 = 32,
    GX_LIGHT4 = 16,
    GX_LIGHT3 = 8,
    GX_LIGHT2 = 4,
    GX_LIGHT1 = 2,
    GX_LIGHT0 = 1
};

// Size: 0xC
struct _GXTlutObj
{
    unsigned int dummy[3]; // Offset: 0x0
};

// Size: 0x40, DWARF: 0xC20D10
struct _GXLightObj
{
    unsigned int dummy[16]; // Offset: 0x0, DWARF: 0xC20D30
};

// Size: 0x4, DWARF: 0xC20A7C
struct _GXColor
{
    unsigned char r; // Offset: 0x0, DWARF: 0xC20A99
    unsigned char g; // Offset: 0x1, DWARF: 0xC20ABD
    unsigned char b; // Offset: 0x2, DWARF: 0xC20AE1
    unsigned char a; // Offset: 0x3, DWARF: 0xC20B05
};

// Size: 0x4C
struct _GS_TEXTURE
{
    u32 Flags; // Offset: 0x0
    enum gs_texturetype_e Format; // Offset: 0x4
    u32 Width; // Offset: 0x8
    u32 Height; // Offset: 0xC
    u32 TexBits; // Offset: 0x10
    u32 PalBits; // Offset: 0x14
    u32 NUID; // Offset: 0x18
    u32 Pad; // Offset: 0x1C
    struct _GXTexObj Tex; // Offset: 0x20
    struct _GXTlutObj Tlut; // Offset: 0x40
};

struct _GS_TEXTURE *GS_TexList; //static
