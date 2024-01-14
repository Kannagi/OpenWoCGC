
// Size: 0xB4C, DWARF: 0x73CED2
struct Bridge_s
{
    struct NuVec pos[24][2]; // Offset: 0x0, DWARF: 0x73CEE4
    struct NuVec vel[24][2]; // Offset: 0x240, DWARF: 0x73CF0C
    struct numtx_s mtx[24]; // Offset: 0x480, DWARF: 0x73CF34
    struct nuinstance_s* instance[24]; // Offset: 0xA80, DWARF: 0x73CF5C
    struct nuinstance_s* ipost; // Offset: 0xAE0, DWARF: 0x73CF89
    struct NuVec center; // Offset: 0xAE4, DWARF: 0x73CFB6
    float radius; // Offset: 0xAF0, DWARF: 0x73CFE1
    short plat[24]; // Offset: 0xAF4, DWARF: 0x73D00A
    char inrange; // Offset: 0xB24, DWARF: 0x73D033
    char onscreen; // Offset: 0xB25, DWARF: 0x73D05D
    char sections; // Offset: 0xB26, DWARF: 0x73D088
    char postint; // Offset: 0xB27, DWARF: 0x73D0B3
    s32 colour; // Offset: 0xB28, DWARF: 0x73D0DD
    short hit; // Offset: 0xB2C, DWARF: 0x73D106
    short yang; // Offset: 0xB2E, DWARF: 0x73D12C
    float width; // Offset: 0xB30, DWARF: 0x73D153
    float tension; // Offset: 0xB34, DWARF: 0x73D17B
    float gravity; // Offset: 0xB38, DWARF: 0x73D1A5
    float damp; // Offset: 0xB3C, DWARF: 0x73D1CF
    float plrweight; // Offset: 0xB40, DWARF: 0x73D1F6
    float postw; // Offset: 0xB44, DWARF: 0x73D222
    float posth; // Offset: 0xB48, DWARF: 0x73D24A
};


#define SQR(x) ((x)*(x))

//NGC MATCH
s32* NuBridgeCreate(struct nuinstance_s** instance,struct nuinstance_s* ipost,struct nuvec_s* start,struct nuvec_s* end,float width,
    short yang,float tension,float damp,float gravity,float plrweight,s32 sections,float postw,float posth,s32 postint,s32 colour) {
 
    struct nuvec_s dir; // 0x8(r1)
    struct nuvec_s dir90; // 0x18(r1)
    struct Bridge_s* bridge;
    s32 lp;
    float t;

    if (sections > 24) {
        printf("to many sections/n");
    }
    NuBridgeOn(1);
    bridge = NuBridgeAlloc();
    if (bridge != NULL) {
        bridge->tension = tension;
        bridge->damp = damp;
        bridge->gravity = gravity;
        bridge->ipost = ipost;
        bridge->plrweight = plrweight;
        bridge->postw = postw;
        bridge->posth = posth;
        bridge->postint = postint;
        bridge->colour = colour;
        bridge->sections = sections;
        bridge->width = width;
        
        (bridge->center).x = (end->x + start->x) * 0.5f;
        (bridge->center).y = (end->y + start->y) * 0.5f;
        (bridge->center).z = (end->z + start->z) * 0.5f;
        
        dir.x = end->x - start->x;
        dir.y = end->y - start->y;
        dir.z = end->z - start->z;

        bridge->radius = SQR(dir.x * 0.5f) + SQR(dir.y * 0.5f) + SQR(dir.z * 0.5f) + 1.0f;
        bridge->inrange = 0;

        t = 1.0f / NuFsqrt(SQR(dir.x) + SQR(dir.z));
        dir90.x = -dir.z * t;
        dir90.y = 0.0f;
        dir90.z = dir.x * t;
        bridge->yang = yang;
        
        for (lp = 0; lp < sections; lp++) {
            bridge->instance[lp] = instance[lp];
            if (bridge->instance[lp]) {
                bridge->plat[lp] = NewPlatInst(&bridge->mtx[lp], NuBridgeLookupInstanceIndex(instance[lp]));
                PlatInstRotate(bridge->plat[lp], 1);
            } else {
                bridge->plat[lp] = -1;
            }
            
            NuMtxSetIdentity(&bridge->mtx[lp]);
            NuMtxPreRotateY(&bridge->mtx[lp], bridge->yang);
            
            bridge->mtx[lp]._30 = ((dir.x * lp) / (sections - 1)) + start->x;
            bridge->mtx[lp]._31 = ((dir.y * lp) / (sections - 1)) + start->y;
            bridge->mtx[lp]._32 = ((dir.z * lp) / (sections - 1)) + start->z;
            
            bridge->pos[lp][0].x = ((dir.x * lp) / (sections - 1) + start->x) - (dir90.x * width * 0.5f);
            bridge->pos[lp][0].y = (dir.y * lp) / (sections - 1) + start->y;
            bridge->pos[lp][0].z = ((dir.z * lp) / (sections - 1) + start->z) - (dir90.z * width * 0.5f);
            
            bridge->pos[lp][1].x = ((dir.x * lp) / (sections - 1) + start->x) + (dir90.x * width * 0.5f);
            bridge->pos[lp][1].y = (dir.y * lp) / (sections - 1) + start->y;
            bridge->pos[lp][1].z = ((dir.z * lp) / (sections - 1) + start->z) + (dir90.z * width * 0.5f);
            
            bridge->vel[lp][0].x = 0.0f;
            bridge->vel[lp][0].y = 0.0f;
            bridge->vel[lp][0].z = 0.0f;
            
            bridge->vel[lp][1].x = 0.0f;
            bridge->vel[lp][1].y = 0.0f;
            bridge->vel[lp][1].z = 0.0f;
            bridge->hit = 0;
        }
    }
    return (s32*)bridge;
}

//NGC MATCH
void NuBrdigeDrawRope(struct numtl_s *mtl,struct nuvec_s *rope1,struct nuvec_s *rope2,s32 ropecnt,s32 *ropetab,s32 colour) {
    s32 *ropeiter;
    
    if (*ropetab < ropetab[1]) { 
        for (ropeiter = ropetab; *ropeiter < ropeiter[1]; ropeiter++) {
            ropesegment(mtl,rope1 + *ropeiter, ropeiter[1] - *ropeiter,colour);
            ropesegment(mtl,rope2 + *ropeiter, ropeiter[1] - *ropeiter,colour);
           
        }
    }
    return;
}

//NGC MATCH
void NuBridgeDraw(struct nugscn_s *scn,struct numtl_s *mtl) {
    s32 iVar7;
    s32 i;
    s32 lp;
    struct Bridge_s *bridge;
    struct numtx_s m;
    struct nuvec4_s v;
    struct nuvec4_s vp [2];
    struct nuvec_s rope1 [512];
    struct nuvec_s rope2 [512];
    s32 ropecnt;
    s32 ropetab [8];
    
    if (NuBridgeProc != 0) {
        iVar7 = 0;
        ropecnt = 0;
        bridge = &Bridges;
        ropetab[iVar7] = ropecnt;
        
        for (i = 0; i < BridgeFree; i++) {
            //i = i + 1;
            if (bridge->inrange) {
                bridge->onscreen = FALSE;
                NuMtxSetRotationY(&m, bridge->yang);
                
                vp[0].x = 0.0f;
                vp[0].y = 0.0f;
                vp[0].z = -bridge->postw;
                vp[0].w = 1.0f;
                
                vp[1].x = 0.0f;
                vp[1].y = 0.0f;
                vp[1].z = bridge->postw;
                vp[1].w = 1.0f;
                
                for (lp = 0; lp < bridge->sections; lp++) {
                    
                    if ((bridge->instance[lp] != 0) && (NuRndrGScnObj(scn->gobjs[bridge->instance[lp]->objid], &bridge->mtx[lp]) != 0)) {
                        bridge->onscreen = TRUE;
                    }
                    
                    if ((lp == (lp / bridge->postint) * bridge->postint) && (bridge->ipost != NULL)) {
                        NuVec4MtxTransformVU0(&v, &vp[0], &bridge->mtx[lp]);
                        if (ropecnt < 512) {
                            rope1[ropecnt].x = v.x;
                            rope1[ropecnt].y = v.y + bridge->posth;
                            rope1[ropecnt].z = v.z;
                        }
                        
                        m._30 = v.x;
                        m._31 = v.y;
                        m._32 = v.z;
                        NuRndrGScnObj(scn->gobjs[bridge->ipost->objid], &m);
                        
                        v.x = 0.0f;
                        v.y = 0.0f;
                        v.z = 1.0f;
                        v.w = 1.0f;
                        NuVec4MtxTransformVU0(&v, &vp[1], &bridge->mtx[lp]);
                        
                        if (ropecnt < 512) {
                            rope2[ropecnt].x = v.x;
                            rope2[ropecnt].y = v.y + bridge->posth;
                            rope2[ropecnt].z = v.z;
                            ropecnt++;
                        }
                        
                        m._30 = v.x;
                        m._31 = v.y;
                        m._32 = v.z;
                        NuRndrGScnObj(scn->gobjs[bridge->ipost->objid], &m);
                    }
                }
                
                if (ropetab[iVar7] != ropecnt) {
                    ropetab[iVar7 + 1] = ropecnt;
                    iVar7++;
                }
            }
            bridge++;
        }
        
        ropetab[iVar7 + 1] = ropecnt;
        if (1 < ropecnt) {
            ropemat = m;
            NuBrdigeDrawRope(mtl, rope1, rope2, ropecnt, ropetab, Bridges->colour);
        }
    }
    return;
}