#ifndef NUGOBJ_H
#define NUGOBJ_H

#include "../types.h"
#include "numath/numathtypes.h"
#include "nu3dxtypes.h"
#include <nu3dx/nugobj.h>

/**********************************************************/
// Prototypes
/**********************************************************/
//void NuMtlUVAnimation(struct nugobj_s* gobj);
void NuGobjInit(void);
void NuGobjClose(void);
struct nugobj_s* NuGobjCreate(void);
void NuGobjDestroy(struct nugobj_s* obj);
void NuGobjAddGeom(struct nugobj_s* gobj, struct nugeom_s* geom);
void NuGobjAddFaceOnGeom(struct nugobj_s* gobj, struct nufaceongeom_s* Fgeom);
struct nugeom_s* NuGeomCreate(void);
struct nufaceongeom_s* NuFaceOnGeomCreate(void);
void NuGeomDestroy(struct nugeom_s* geom);
void NuGeomCreateVB(struct nugeom_s* geom, u32 vtxCount, enum nuvtxtype_e vtxType, s32 dynamic);
void NuGeomDestroyVB(struct nugeom_s* geom);
void NuGeomAddPrim(struct nugeom_s* geom, struct nuprim_s* prim);
void NuGeomAddSkin(struct nugeom_s* geom, struct nuskin_s* skin);
struct nuprim_s* NuPrimCreate(int amount, enum nuprimtype_e type);
void NuPrimDestroy(struct nuprim_s* prim);
void* GS_CreateBuffer(u32 bufsize, s32 bufferType);
void GS_DeleteBuffer(void* ptr);
int NuVtxStride(enum nuvtxtype_e type);
void NuAnimUV(void);
/**********************************************************/
// Variables
/**********************************************************/
static s32 sysinit;
extern int _timer;
extern s32 Paused;
extern u32 GS_BufferSize;
extern u32 BufferTypes[4];
static struct nugobj_s* sysgobj;
/**************************************************************/
typedef struct GS_Buffer {
    u32 size;
    u32 type;
}; // 0x8

// Size: 0x1C, defined as 0x30.. why?
typedef struct nufaceongeom_s
{
    struct nufaceongeom_s* next;
    struct numtl_s* mtl;
    int mtl_id;
    enum nufaceontype_s faceon_type;
    struct nufaceon_s* faceons;
    int nfaceons;
    float faceon_radius;
};

// Size: 0x24
struct nugeomitem_s
{
    struct nurndritem_s hdr;
    struct numtx_s* mtx;
    struct nugeom_s* geom;
    f32** blendvals;
    u16 instancelights_index[3];
    u16 hShader;
};

// Size: 0x18
struct nufaceon_s
{
    struct nuvec_s point;
    float width;
    float height;
    int colour;
};

// Size: 0x40
struct NUBLENDGEOM_s
{
    int nblends;
    struct nuvec_s** blend_offsets;
    int* ix;
    struct nuvec_s* offsets;
    struct nuvec_s* ooffsets;
    int hVB; //VertexBuffer, GS_Buffer * ?
    int blendindex[10];
};

// Size: 0x18
struct nuskin_s
{
    struct nuskin_s* next;
    int vtxoffset;
    int vtxcnt;
    int mtxcnt;
    int* mtxid;
    float* weights;
};


// Size: 0x10
struct NUVTXSKININFO_s
{
    float wts[3];
    unsigned char joint_ixs[3];
    unsigned char pad;
};



// Size: 0x30
struct nugeom_s
{
    struct nugeom_s* next;
    struct numtl_s* mtl;
    s32 mtl_id;
    enum nuvtxtype_e vtxtype; //Size: 0x4
    s32 vtxcnt;
    s32 vtxmax;
    s32 hVB;  //GS_Buffer*
    void* basisvbptr;
    struct nuprim_s* prim;
    struct nuskin_s* skin;
    struct NUVTXSKININFO_s* vtxskininfo;
    struct NUBLENDGEOM_s* blendgeom;
};

// Size: 0x64
struct nugobj_s
{
    struct nugobj_s* sysnext;
    struct nugobj_s* syslast;
    enum gobjtype_s type; //Size: 0x4
    struct nugeom_s* geom;
    struct nufaceongeom_s* faceon_geom;
    float bounding_radius_from_origin;
    float bounding_rsq_from_origin;
    struct nuvec_s bounding_box_min;
    struct nuvec_s bounding_box_max;
    struct nuvec_s bounding_box_center;
    float bounding_radius_from_center;
    float bounding_rsq_from_center;
    int ngobjs;
    struct nugobj_s* next_gobj;
    struct nuvec_s origin;
    int ignore;
    int culltype;
};


// Size: 0x3C
struct nuprim_s
{
    struct nuprim_s* next;
    enum nuprimtype_e type; //Size: 0x4
    unsigned short vertexCount;
    unsigned short max;
    unsigned short* vid;
    struct nuplane_s* pln;
    int idxbuff;  //GS_Buffer
    int cached;
    short skinmtxlookup[16];
};

// Size: 0x10
struct nuplane_s
{
    float a;
    float b;
    float c;
    float d;
};


enum nuvtxtype_e //s32
{
    NUVT_PS = 0x11,
    NUVT_LC1 = 0x51,
    NUVT_TLTC1 = 0x53,
    NUVT_SK3TC1 = 0x5d,
    NUVT_TC1 = 0x59
};

enum nufaceontype_s
{
    NUFACEON_FACEY = 0x1,
    NUFACEON_FACEON = 0x0
};

enum gobjtype_s
{
    NUGOBJ_FACEON = 0x1,
    NUGOBJ_MESH = 0x0
};


enum nuprimtype_e
{
    NUPT_QUADLIST = 0xa,
    NUPT_FACEON = 0x9,
    NUPT_BEZTRI = 0x8,
    NUPT_BEZPATCH = 0x7,
    NUPT_NDXTRISTRIP = 0x6,
    NUPT_NDXTRI = 0x5,
    NUPT_NDXLINE = 0x4,
    NUPT_TRISTRIP = 0x3,
    NUPT_TRI = 0x2,
    NUPT_LINE = 0x1,
    NUPT_POINT = 0x0
};

#endif // !NUGOBJ_H