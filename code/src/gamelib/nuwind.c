#include "nuwind.h"

//NGC MATCH
void NuWindDraw(struct nugscn_s *scn) {
    s32 lp;
    s32 i;
    float t2;
    float t3;
    struct nuwindgrp_s *grp;
    struct numtx_s *mtx;
    struct nugobj_s *gobj;
    char pad[25];
    
    grp = &NuWindGroup[0];
    for (lp = 0; lp < NuWindGCount; lp++) {
            if (grp->inrange != '\0') {
                grp->onscreen = '\0';
                mtx = grp->mtx;
                gobj = scn->gobjs[grp->instance->objid];
                    for (i = 0; i < grp->objcount; i++) {
                        t2 = mtx->_23;
                        t3 = mtx->_33;
                        mtx->_23 = 0.0f;
                        mtx->_33 = 1.0f;
                        if (NuRndrGrassGobj(gobj,mtx,NULL) != 0) {
                            grp->onscreen = '\x01';
                        }
                        mtx->_23 = t2;
                        mtx->_33 = t3;
                        mtx++;
                    }
            }
            grp++;
    }
    return;
}