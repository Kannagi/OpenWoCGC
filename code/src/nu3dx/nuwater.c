#include "nuwater.h"
#include "../system.h"

//NGC MATCH
static s32 Powr2(s32 v) {
    s32 p2;

    p2 = 1;
    while (p2 < v) {
        p2 = p2 << 1;
    }
    return p2;
}


//NGC MATCH
static void fixMaterials(void) {
    s32 nummtls;
    struct numtl_s *mtl;
    s32 i;
    
    nummtls = NuMtlNum();
        for (i = 0; i < nummtls; i++) {
            mtl = NuMtlGet(i);
            if ((mtl != NULL) && (mtl->fxid == '\x04')) {
                mtl->tid = water_copy_mtl->tid;
                //mtl->attrib & 0x3c30ffff | 0x1420000;
                mtl->attrib.alpha = 0;
                mtl->attrib.utc = 1;
                mtl->attrib.vtc = 1;
                mtl->attrib.lighting = 2;
                mtl->alpha_sort = 100;
                mtl->attrib.zmode = 0;
                NuMtlUpdate(mtl);
            }
        }
    return;
}


//NGC MATCH
s32 dynamicWaterGetNormalMap(void) {
    return dynamicWaterTextureIds[dynamicWaterForceStepOneTex];
}


//NGC MATCH
static void NuDynamicWaterClose(void) {
    s32 i;
    
    if (dynamicWaterInitialised != 0) {
        dynamicWaterInitialised = 0;
        for (i = 0; i < 6; i++) {
            NuTexDestroy(dynamicWaterTextureIds[i]);
        }
    }
    return;
}

//NGC MATCH
static s32 isWaterInstance(struct nugscn_s *gsc,struct nuinstance_s *inst) {
    struct nugeom_s *geom;
    
    geom = gsc->gobjs[inst->objid]->geom;
    while( geom != NULL ) {
        if ((NuShaderAssignShader(geom) == 1) || (NuShaderAssignShader(geom) == 0x1b)) return 1;
        geom = geom->next;
    }
    return 0;
}

//NGC MATCH
static void initWater(struct nugscn_s* gsc) {
    s32 i;
    s32 num_water_insts;
    struct nugeom_s *geom;
    
    num_water_insts = 0;
    for (i = 0; i < gsc->numinstance; i++) {
            if (isWaterInstance(gsc,&gsc->instances[i]) != 0) {
                num_water_insts++;
            }
    }
    if (0xff < nwinst + num_water_insts) {
        NuErrorProlog("C:/source/crashwoc/code/nu3dx/nuwater.c",0x35b)("assert");
    }
    for (i = 0; i < gsc->numinstance; i++) {
            if (isWaterInstance(gsc,&gsc->instances[i]) != 0) {
                wgsc[nwinst] = gsc;
                winst[nwinst++] = &gsc->instances[i];
                gsc->instances[i].flags.visible = 0;
                for (geom = gsc->gobjs[gsc->instances[i].objid]->geom; geom != NULL;
                    geom = geom->next) {
                    initWaterGeom(geom);
                }
            }
    }
    return;
}

//NGC MATCH
void NuWaterLoad(struct nugscn_s *gsc) {
    if (gsc != NULL) {
        initWater(gsc);
        fixMaterials();
    }
    return;
}

//NGC MATCH
void NuWaterInit(void) {
    
    float p2w = Powr2(SWIDTH);
    float p2h = Powr2(SHEIGHT);
    NuRandSeed(0x11);
    water_copy_mtl = NuMtlCreate(1);

    //(attrib & 0xcc0cffff | 0x16e8000);
    water_copy_mtl->attrib.cull = 2;
    water_copy_mtl->attrib.zmode = 3;
    water_copy_mtl->attrib.filter = 0;
    water_copy_mtl->attrib.lighting = 2;
    water_copy_mtl->attrib.colour = 1;
    (water_copy_mtl->diffuse).r = 1.0f;
    (water_copy_mtl->diffuse).g = 1.0f;
    (water_copy_mtl->diffuse).b = 1.0f;
    water_copy_mtl->alpha = 0.999f;
    water_copy_mtl->attrib.utc = 1;
    water_copy_mtl->attrib.vtc = 1;
    water_copy_mtl->tid = NudxFw_GetBackBufferCopyTID();
    NuMtlUpdate(water_copy_mtl);
    if (dynamicWaterEnabled != 0) {
        NuDynamicWaterInit();
    }
    nwinst = 0;
    nwater_mtl = 0;
    return;
}

//NGC MATCH
void NuWaterClose(void) {
    nwinst = 0;
    nwater_mtl = 0;
    if (dynamicWaterEnabled != 0) {
        NuDynamicWaterClose();
    }
    return;
}



