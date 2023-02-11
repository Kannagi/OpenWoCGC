#include "nurndr.h"
#include "../system.h"


void NuRndrInit(void)

{
  short *timer;
  int lp;
  
  lp = 0x20;
  NuRndrShadowCnt = 0;
  timer = &NuRndrWaterRipDat[0x1f].timer;
  do {
    *timer = 0;
    timer = timer + -0x12;
    lp = lp + -1;
  } while (lp != 0);
  return;
}


int NuRndrBeginScene(int hRT)

{
  uint bs;
  
  if (rndrmtx_cnt < rndrmtx_cnt_max) {
    rndrmtx_cnt_max = rndrmtx_cnt;
  }
  if (geomitem_cnt < geomitem_cnt_max) {
    geomitem_cnt_max = geomitem_cnt;
  }
  rndrmtx_cnt = 0x600;
  rndr_blend_shape_deformer_wt_cnt = 0x400;
  rndr_blend_shape_deformer_wt_ptrs_cnt = 0x800;
  geomitem_cnt = 0x800;
  NuMtlClearOt();
  bs = NudxFw_BeginScene(hRT);
  return ~bs >> 0x1f;
}

void NuRndrEndScene(void)

{
  if (GS_Parallax != 0) {
    GS_SetZCompare(0,0,GX_NEVER);
  }
  NuRndrFlush();
  GS_EndScene();
  return;
}


void NuRndrFlush(void)

{
  NuMtlRender();
  NuLightClearStoredLights();
  return;
}

void NuRndrClear(s32 flags,s32 colour,f32 depth)

{
  s32 fwf;
  
    fwf = 0;
  if ((flags & 1U) != 0) {
    fwf = 0xf0;
  }
  if ((flags & 2U) != 0) {
    fwf = fwf | 1;
  }
  if ((flags & 8U) != 0) {
    fwf = fwf | 2;
  }
  NudxFw_Clear(fwf,colour,depth);
  return;
}

void NuRndrSwapScreen(int hRT)

{
  NudxFw_SetBackBufferCopied(1);
  if (Pad[0] != NULL) {
    if (Pad[0]->old_paddata == 0) {
      padflag = 1;
    }
    if (padflag != 0) {
      if ((Pad[0]->old_paddata & 1) != 0) {
        NudxFw_FlipScreen(hRT,1);
        return;
      }
      NudxFw_FlipScreen(hRT,1);
      return;
    }
  }
  NudxFw_FlipScreen(hRT,0);
  return;
}


/*

int NuRndrGobj(nugobj_s *gobj,numtx_s *wm,float **blendvals)

{
  numtl_s **ppnVar1;
  float *pfVar2;
  float *pfVar3;
  nugobj_s *nextgobj;
  int i;
  int currlgts;
  int iVar4;
  nuvec_s *min;
  short sVar6;
  numtl_s *mtl;
  NUERRORFUNC *e;
  int iVar5;
  nuvec_s *max;
  float *pfVar7;
  uint s_id;
  float *pfVar8;
  int iVar9;
  float *m;
  nugeom_s *geom;
  byte in_cr0;
  byte in_cr1;
  byte in_cr2;
  byte in_cr3;
  byte unaff_cr4;
  byte in_cr5;
  byte in_cr6;
  byte in_cr7;
  numtx_s mtx;
  nuvec_s bb_min;
  nuvec_s bb_max;
  uint local_4c;
  
  local_4c = (uint)(in_cr0 & 0xf) << 0x1c | (uint)(in_cr1 & 0xf) << 0x18 |
             (uint)(in_cr2 & 0xf) << 0x14 | (uint)(in_cr3 & 0xf) << 0x10 |
             (uint)(unaff_cr4 & 0xf) << 0xc | (uint)(in_cr5 & 0xf) << 8 | (uint)(in_cr6 & 0xf) <<  4
             | (uint)(in_cr7 & 0xf);
  nextgobj = gobj->next_gobj;
  iVar9 = -1;
  currlgts = NuLightStoreCurrentLights();
  if (gobj != (nugobj_s *)0x0) {
    do {
      if (gobj->culltype == 0) {
        iVar4 = NuCameraClipTestBoundingSphere
                          (&gobj->bounding_box_center,&gobj->bounding_radius_from_center,wm);
      }
      else {
        if ((((gobj->origin).x == 0.0) && ((gobj->origin).y == 0.0)) && ((gobj->origin).z == 0.0))  {
          min = &gobj->bounding_box_min;
          max = &gobj->bounding_box_max;
        }
        else {
          bb_min.x = (gobj->bounding_box_min).x;
          bb_min.y = (gobj->bounding_box_min).y;
          bb_min.z = (gobj->bounding_box_min).z;
          bb_max.x = (gobj->bounding_box_max).x;
          bb_max.y = (gobj->bounding_box_max).y;
          bb_max.z = (gobj->bounding_box_max).z;
          NuVecAdd(&bb_min,&gobj->bounding_box_min,&gobj->origin);
          NuVecAdd(&bb_max,&gobj->bounding_box_max,&gobj->origin);
          min = &bb_min;
          max = &bb_max;
        }
        iVar4 = NuCameraClipTestExtents(min,max,wm);
      }
      i = iVar4;
      if (iVar9 == -1) {
LAB_800b47d0:
        iVar9 = i;
        if (iVar4 != 0) {
          rndrmtx_cnt = rndrmtx_cnt + -1;
          if (rndrmtx_cnt < 0) {
            e = NuErrorProlog("C:/source/crashwoc/code/nu3dx/nurndr.c",0x282);
            (*e)("NuRndrGobj : No free matrix slots!");
          }
          i = 0x30;
          m = rndrmtx + rndrmtx_cnt;
          pnVar2 = m;
          pnVar3 = wm;
          do {
            pnVar8 = pnVar3;
            pnVar7 = pnVar2;
            i = i + -0x18;
            pnVar7->_00 = pnVar8->_00;
            pnVar7->_01 = pnVar8->_01;
            pnVar7->_02 = pnVar8->_02;
            pnVar7->_03 = pnVar8->_03;
            pnVar7->_10 = pnVar8->_10;
            pnVar7->_11 = pnVar8->_11;
            pnVar2 = (numtx_s *)&pnVar7->_12;
            pnVar3 = (numtx_s *)&pnVar8->_12;
          } while (i != 0);
          pnVar7->_12 = pnVar8->_12;
          pnVar7->_13 = pnVar8->_13;
          pnVar7->_20 = pnVar8->_20;
          pnVar7->_21 = pnVar8->_21;
          if (nextgobj != (nugobj_s *)0x0) {
            NuMtxSetIdentity(&mtx);
            mtx._30 = (gobj->origin).x;
            mtx._31 = (gobj->origin).y;
            mtx._32 = (gobj->origin).z;
            NuMtxMul(m,&mtx,m);
          }
          geom = gobj->geom;
          if (geom != (nugeom_s *)0x0) {
            do {
              if (geomitem_cnt == 0) {
                e = NuErrorProlog("C:/source/crashwoc/code/nu3dx/nurndr.c",0x2ba);
                (*e)("NuRndrGobj : No free geom item slots!");
              }
              else {
                i = geomitem_cnt + -1;
                geomitem_cnt = i;
                geomitem[i].hdr.type = NURNDRITEM_GEOM3D;
                geomitem[i].hdr.flags = 0;
                if (iVar4 == 1) {
                  geomitem[i].hdr.flags = 1;
                }
                geomitem[i].hdr.lights_index = (short)currlgts;
                geomitem[i].mtx = m;
                geomitem[i].geom = geom;
                geomitem[i].blendvals = blendvals;
                sVar6 = NuShaderAssignShader(geom);
                ppnVar1 = nurndr_forced_mtl_table;
                geomitem[i].hShader = sVar6;
                if ((ppnVar1 == (numtl_s **)0x0) || (s_id = (uint)geom->mtls->special_id, s_id ==  0)
                   ) {
                  mtl = (nusysmtl_s *)nurndr_forced_mtl;
                  if (nurndr_forced_mtl == (numtl_s *)0x0) {
                    mtl = (nusysmtl_s *)geom->mtls;
                  }
                }
                else {
                  mtl = (nusysmtl_s *)ppnVar1[s_id];
                  if (mtl == (nusysmtl_s *)0x0) goto LAB_800b4994;
                }
                NuMtlAddRndrItem(mtl,geomitem + i);
              }
LAB_800b4994:
              geom = geom->next;
            } while (geom != (nugeom_s *)0x0);
          }
          geom = (nugeom_s *)gobj->faceon_geom;
          if (geom != (nugeom_s *)0x0) {
            if (geomitem_cnt == 0) {
              e = NuErrorProlog("C:/source/crashwoc/code/nu3dx/nurndr.c",0x2dc);
              (*e)("NuRndrGobj : No free geom item slots!");
            }
            else {
              i = geomitem_cnt + -1;
              geomitem_cnt = i;
              geomitem[i].geom = geom;
              iVar5 = NuLightStoreCurrentLights();
              geomitem[i].hdr.lights_index = (short)iVar5;
              geomitem[i].hdr.type = NURNDRITEM_GEOMFACE;
              geomitem[i].hdr.flags = 0;
              if (iVar4 == 1) {
                geomitem[i].hdr.flags = 1;
              }
              geomitem[i].mtx = m;
              geomitem[i].blendvals = blendvals;
              NuMtlAddFaceonItem(geom->mtls,&geomitem[i].hdr);
            }
          }
        }
      }
      else {
        if ((iVar9 == 1) && (iVar4 != 1)) {
LAB_800b47cc:
          i = 2;
          goto LAB_800b47d0;
        }
        i = iVar9;
        if (iVar9 != 0) goto LAB_800b47d0;
        if (iVar4 != 0) goto LAB_800b47cc;
      }
      gobj = gobj->next_gobj;
    } while (gobj != (nugobj_s *)0x0);
  }
  return iVar9;
}


int NuRndrGrassGobj(nugobj_s *gobj,numtx_s *wm,float **blendvals)

{
  float fVar1;
  float fVar2;
  float fVar3;
  nusysmtl_s **ppnVar4;
  numtx_s *pnVar5;
  numtx_s *pnVar6;
  NUERRORFUNC *err;
  int iVar7;
  short sVar8;
  nusysmtl_s *mtl;
  numtx_s *pnVar9;
  uint uVar10;
  int iVar11;
  numtx_s *pnVar12;
  int iVar13;
  numtx_s *pnVar14;
  int iVar15;
  nugeom_s *geom;
  double dVar16;
  nuvec_s local_70;
  nuvec_s local_60;
  
  iVar13 = -1;
  if (gobj != (nugobj_s *)0x0) {
    dVar16 = 100.0;
    do {
      local_70.x = (gobj->bounding_box_min).x;
      local_70.y = (gobj->bounding_box_min).y;
      local_70.z = (gobj->bounding_box_min).z;
      local_60.x = (gobj->bounding_box_max).x;
      local_60.y = (gobj->bounding_box_max).y;
      local_60.z = (gobj->bounding_box_max).z;
      NuVecAdd(&local_70,&gobj->bounding_box_min,&gobj->origin);
      NuVecAdd(&local_60,&gobj->bounding_box_max,&gobj->origin);
      fVar1 = local_70.y - wm->_31;
      fVar2 = local_70.x - wm->_30;
      fVar3 = local_70.z - wm->_32;
      if ((double)(fVar3 * fVar3 + fVar2 * fVar2 + fVar1 * fVar1) <= dVar16) {
        iVar15 = NuCameraClipTestBoundingSphere
                           (&gobj->bounding_box_center,&gobj->bounding_radius_from_center,wm);
      }
      else {
        iVar15 = 0;
      }
      iVar11 = iVar15;
      if (iVar13 == -1) {
LAB_800b4b88:
        iVar13 = iVar11;
        if (iVar15 != 0) {
          rndrmtx_cnt = rndrmtx_cnt + -1;
          if (rndrmtx_cnt < 0) {
            err = NuErrorProlog("C:/source/crashwoc/code/nu3dx/nurndr.c",0x316);
            (*err)("NuRndrGobj : No free matrix slots!");
          }
          iVar11 = 0x30;
          pnVar14 = rndrmtx + rndrmtx_cnt;
          pnVar5 = pnVar14;
          pnVar6 = wm;
          do {
            pnVar12 = pnVar6;
            pnVar9 = pnVar5;
            iVar11 = iVar11 + -0x18;
            pnVar9->_00 = pnVar12->_00;
            pnVar9->_01 = pnVar12->_01;
            pnVar9->_02 = pnVar12->_02;
            pnVar9->_03 = pnVar12->_03;
            pnVar9->_10 = pnVar12->_10;
            pnVar9->_11 = pnVar12->_11;
            pnVar5 = (numtx_s *)&pnVar9->_12;
            pnVar6 = (numtx_s *)&pnVar12->_12;
          } while (iVar11 != 0);
          pnVar9->_12 = pnVar12->_12;
          pnVar9->_13 = pnVar12->_13;
          pnVar9->_20 = pnVar12->_20;
          pnVar9->_21 = pnVar12->_21;
          geom = gobj->geom;
          if (geom != (nugeom_s *)0x0) {
            do {
              if (geomitem_cnt == 0) {
                err = NuErrorProlog("C:/source/crashwoc/code/nu3dx/nurndr.c",0x344);
                (*err)("NuRndrGobj : No free geom item slots!");
              }
              else {
                iVar11 = geomitem_cnt + -1;
                geomitem_cnt = iVar11;
                geomitem[iVar11].hdr.type = NURNDRITEM_GEOM3D;
                geomitem[iVar11].hdr.flags = 0;
                if (iVar15 == 1) {
                  geomitem[iVar11].hdr.flags = 1;
                }
                iVar7 = NuLightStoreCurrentLights();
                geomitem[iVar11].hdr.lights_index = (short)iVar7;
                geomitem[iVar11].mtx = pnVar14;
                geomitem[iVar11].geom = geom;
                geomitem[iVar11].blendvals = blendvals;
                sVar8 = NuShaderAssignShader(geom);
                ppnVar4 = nurndr_forced_mtl_table;
                geomitem[iVar11].hShader = sVar8;
                if ((ppnVar4 == (nusysmtl_s **)0x0) ||
                   (uVar10 = (uint)(geom->mtls->mtl).special_id, uVar10 == 0)) {
                  mtl = nurndr_forced_mtl;
                  if (nurndr_forced_mtl == (nusysmtl_s *)0x0) {
                    mtl = geom->mtls;
                  }
                }
                else {
                  mtl = ppnVar4[uVar10];
                  if (mtl == (nusysmtl_s *)0x0) goto LAB_800b4d14;
                }
                NuMtlAddRndrItem(mtl,geomitem + iVar11);
              }
LAB_800b4d14:
              geom = geom->next;
            } while (geom != (nugeom_s *)0x0);
          }
        }
      }
      else {
        if ((iVar13 == 1) && (iVar15 != 1)) {
LAB_800b4b84:
          iVar11 = 2;
          goto LAB_800b4b88;
        }
        iVar11 = iVar13;
        if (iVar13 != 0) goto LAB_800b4b88;
        if (iVar15 != 0) goto LAB_800b4b84;
      }
      gobj = gobj->next_gobj;
    } while (gobj != (nugobj_s *)0x0);
  }
  return iVar13;
}


int NuRndrGobjSkin2(nugobj_s *gobj,int nummtx,numtx_s *wm,float **blendvals)

{
  numtx_s *pnVar1;
  numtx_s *pnVar2;
  int i;
  int cclip;
  NUERRORFUNC *e;
  int currlgt;
  short shad;
  nusysmtl_s *sm1;
  numtx_s *pnVar3;
  uint specid;
  int iVar4;
  numtx_s *pnVar5;
  numtx_s *m;
  nugeom_s *geom;
  nusysmtl_s **sm2;
  
  if (gobj->culltype == 0) {
    cclip = NuCameraClipTestBoundingSphere
                      (&gobj->bounding_box_center,&gobj->bounding_radius_from_center,wm);
  }
  else {
    cclip = NuCameraClipTestExtents(&gobj->bounding_box_min,&gobj->bounding_box_max,wm);
  }
  if (cclip != 0) {
    rndrmtx_cnt = rndrmtx_cnt - nummtx;
    if (rndrmtx_cnt < 0) {
      e = NuErrorProlog("C:/source/crashwoc/code/nu3dx/nurndr.c",0x3c3);
      (*e)("NuRndrGobjSkin : No free matrix slots!");
    }
    m = rndrmtx + rndrmtx_cnt;
    i = 0;
    if (0 < nummtx) {
      do {
        currlgt = i + 1;
        iVar4 = 0x30;
        pnVar1 = m + i;
        pnVar2 = wm + i;
        do {
          pnVar5 = pnVar2;
          pnVar3 = pnVar1;
          iVar4 = iVar4 + -0x18;
          pnVar3->_00 = pnVar5->_00;
          pnVar3->_01 = pnVar5->_01;
          pnVar3->_02 = pnVar5->_02;
          pnVar3->_03 = pnVar5->_03;
          pnVar3->_10 = pnVar5->_10;
          pnVar3->_11 = pnVar5->_11;
          pnVar1 = (numtx_s *)&pnVar3->_12;
          pnVar2 = (numtx_s *)&pnVar5->_12;
        } while (iVar4 != 0);
        pnVar3->_12 = pnVar5->_12;
        pnVar3->_13 = pnVar5->_13;
        pnVar3->_20 = pnVar5->_20;
        pnVar3->_21 = pnVar5->_21;
        i = currlgt;
      } while (currlgt < nummtx);
    }
    geom = gobj->geom;
    if (geom != (nugeom_s *)0x0) {
      do {
        if (geomitem_cnt == 0) {
          e = NuErrorProlog("C:/source/crashwoc/code/nu3dx/nurndr.c",0x3f3);
          (*e)("NuRndrGobjSkin : No free geom item slots!");
        }
        else {
          i = geomitem_cnt + -1;
          geomitem_cnt = i;
          geomitem[i].hdr.type = NURNDRITEM_SKIN3D2;
          geomitem[i].hdr.flags = 0;
          if (cclip == 1) {
            geomitem[i].hdr.flags = 1;
          }
          currlgt = NuLightStoreCurrentLights();
          geomitem[i].hdr.lights_index = (short)currlgt;
          geomitem[i].blendvals = blendvals;
          geomitem[i].mtx = m;
          geomitem[i].geom = geom;
          shad = NuShaderAssignShader(geom);
          sm2 = nurndr_forced_mtl_table;
          geomitem[i].hShader = shad;
          if ((sm2 == (nusysmtl_s **)0x0) ||
             (specid = (uint)(geom->mtls->mtl).special_id, specid == 0)) {
            sm1 = nurndr_forced_mtl;
            if (nurndr_forced_mtl == (nusysmtl_s *)0x0) {
              sm1 = geom->mtls;
            }
          }
          else {
            sm1 = sm2[specid];
            if (sm1 == (nusysmtl_s *)0x0) goto LAB_800b4f6c;
          }
          NuMtlAddRndrItem(sm1,geomitem + i);
        }
LAB_800b4f6c:
        geom = geom->next;
      } while (geom != (nugeom_s *)0x0);
    }
  }
  return cclip;
}

*/


int NuRndrTri2d(nuvtx_tltc1_s *vtx,numtl_s *mtl)

{
  NUERRORFUNC *e;
  int stride;
  int buf;
  nuvtx_tltc1_s *vb;
  int i;
  nugeom_s *geom;
  nuprim_s *prim;
  float *tc;
  
  NuMtlGet2dBuffer((nusysmtl_s *)mtl,NUPT_NDXTRI,&geom,&prim,&superbuffer_ptr,&superbuffer_end);
  if (geom->vtxmax - geom->vtxcount < 3) {
    e = NuErrorProlog("C:/source/crashwoc/code/nu3dx/nurndr.c",0x5c0);
    (*e)("NuRndrTri2d : Vertex buffer full!");
  }
  if ((int)((uint)prim->max - (uint)prim->vertexCount) < 3) {
    e = NuErrorProlog("C:/source/crashwoc/code/nu3dx/nurndr.c",0x5c2);
    (*e)("NuRndrTri2d : Primitive buffer full!");
  }
  stride = NuVtxStride(geom->vertex_type);
  buf = geom->hVB;
  if (buf == 0) {
    e = NuErrorProlog("C:/source/crashwoc/code/nu3dx/nurndr.c",0x5c9);
    (*e)("NuRndrTri2d : Lock VB failed!");
  }
  i = 3;
  vb = (nuvtx_tltc1_s *)(buf + stride * geom->vtxcount);
  do {
    (vb->pnt).x = (vtx->pnt).x;
    (vb->pnt).y = (vtx->pnt).y;
    (vb->pnt).z = (vtx->pnt).z;
    vb->rhw = vtx->rhw;
    vb->diffuse = vtx->diffuse;
    vb->tc[0] = vtx->tc[0];
    tc = vtx->tc;
    vtx = vtx + 1;
    vb->tc[1] = tc[1];
    vb = (nuvtx_tltc1_s *)((int)vb->tc + stride + -0x14);
    geom->vtxcount = geom->vtxcount + 1;
    i = i + -1;
  } while (i != 0);
  return 1;
}

int NuRndrTri3d(nuvtx_tc1_s *vtx,numtl_s *mtl,numtx_s *wm)

{
  NUERRORFUNC *e;
  int stride;
  float fVar1;
  float fVar2;
  int iVar3;
  int Hvb;
  nuvec_s *dest;
  nuvec_s *v0;
  byte in_cr0;
  byte in_cr1;
  byte in_cr2;
  byte in_cr3;
  byte unaff_cr4;
  byte in_cr5;
  byte in_cr6;
  byte in_cr7;
  nugeom_s *geom;
  nuprim_s *prim;
  uint local_20;
  nuvec_s *v;
  
  local_20 = (uint)(in_cr0 & 0xf) << 0x1c | (uint)(in_cr1 & 0xf) << 0x18 |
             (uint)(in_cr2 & 0xf) << 0x14 | (uint)(in_cr3 & 0xf) << 0x10 |
             (uint)(unaff_cr4 & 0xf) << 0xc | (uint)(in_cr5 & 0xf) << 8 | (uint)(in_cr6 & 0xf) <<  4
             | (uint)(in_cr7 & 0xf);
  NuMtlGet3dBuffer(mtl,NUPT_TRI,&geom,&prim,&superbuffer_ptr,&superbuffer_end);
  if (geom->vtxmax - geom->vtxcount < 3) {
    e = NuErrorProlog("C:/source/crashwoc/code/nu3dx/nurndr.c",0x5f0);
    (*e)("NuRndrTri3d : Vertex buffer full!");
  }
  if ((int)((uint)prim->max - (uint)prim->vertexCount) < 3) {
    e = NuErrorProlog("C:/source/crashwoc/code/nu3dx/nurndr.c",0x5f2);
    (*e)("NuRndrTri3d : Primitive buffer full!");
  }
  stride = NuVtxStride(geom->vertex_type);
  Hvb = geom->hVB;
  if (Hvb == 0) {
    e = NuErrorProlog("C:/source/crashwoc/code/nu3dx/nurndr.c",0x5f9);
    (*e)("NuRndrTri3d : Lock VB failed!");
  }
  iVar3 = 3;
  v0 = &vtx->nrm;
  dest = (nuvec_s *)(Hvb + stride * geom->vtxcount);
  do {
    if (wm == (numtx_s *)0x0) {
      fVar1 = v0[-1].y;
      fVar2 = v0[-1].z;
      dest->x = v0[-1].x;
      dest->y = fVar1;
      dest->z = fVar2;
      fVar1 = v0->y;
      fVar2 = v0->z;
      dest[1].x = v0->x;
      dest[1].y = fVar1;
      dest[1].z = fVar2;
    }
    else {
      NuVecMtxTransform(dest,&vtx->pnt,wm);
      NuVecMtxRotate(dest + 1,v0,wm);
    }
    vtx = vtx + 1;
    iVar3 = iVar3 + -1;
    dest[2].x = v0[1].x;
    dest[2].y = v0[1].y;
    v = v0 + 1;
    v0 = v0 + 3;
    dest[2].z = v->z;
    dest = (nuvec_s *)((int)&dest->x + stride);
  } while (iVar3 != 0);
  geom->vtxcount = geom->vtxcount + 3;
  return 1;
}

float NuRndrItemDist(nugeomitem_s *item)

{
  nuvec_s pnt;
  float dist;
  
  dist = 0.0;
  if ((item->hdr).type == NURNDRITEM_GEOM3D) {
    NuCameraTransformClip(&pnt,(nuvec_s *)&item->mtx->_30,1,NULL);
    dist = pnt.z;
  }
  return dist;
}

/*undefined4 NuRndrStrip3d(nuvec_s *param_1,numtl_s *mtl,undefined4 param_3,int param_4)

{
		//TODO!

}*/

void NuRndrItem(nurndritem_s *item)

{
  nurndritemtype_s type;
  
  type = item->type;
  if (type == NURNDRITEM_GEOM2D) {
    DBTimerStart(0x1e);
    NuRndr2dItem((nugeomitem_s *)item);
    DBTimerEnd(0x1e);
  }
  else if (type == NURNDRITEM_GEOM3D) {
    DBTimerStart(0x1f);
    NuRndrGeomItem((nugeomitem_s *)item);
    DBTimerEnd(0x1f);
  }
  else if (type == NURNDRITEM_SKIN3D2) {
    DBTimerStart(0x20);
    NuRndrSkinItem2((nugeomitem_s *)item);
    DBTimerEnd(0x20);
  }
  else if (type == NURNDRITEM_GEOMFACE) {
    DBTimerStart(0x22);
    NuRndrFaceItem((nugeomitem_s *)item);
    DBTimerEnd(0x22);
  }
  else {
    e = NuErrorProlog("C:/source/crashwoc/code/nu3dx/nurndr.c",0xbff);
    //(*e)("NuRndrItem : Unknown render item type!");
  }
  return;
}


void NuRndr2dItem(nugeomitem_s *item)

{
  int stride;
  nugeom_s *geom;
  int vtxcnt;
  float *hVB;
  nuprim_s *prim;
  
  if ((_GSMATRIX *)item->mtx == (_GSMATRIX *)0x0) {
    GS_LoadWorldMatrixIdentity();
  }
  else {
    GS_SetWorldMatrix((_GSMATRIX *)item->mtx);
  }
  SetupShaders(item);
  for (prim = item->geom->prim; prim != (nuprim_s *)0x0; prim = prim->next) {
    geom = item->geom;
    vtxcnt = geom->vtxcount;
    hVB = (float *)geom->hVB;
    stride = NuVtxStride(geom->vertex_type);
    GS_DrawTriList(vtxcnt,hVB,stride);
  }
  return;
}


/*

void NuRndrFaceItem(nugeomitem_s *item)	//TODO

{
  float *pfVar1;
  float *pfVar2;
  numtx_s *pnVar3;
  int iVar4;
  float *pfVar5;
  float *pfVar6;
  nugeom_s *pnVar7;
  float fVar8;
  nuvec_s *v0;
  double dVar9;
  double dVar10;
  double dVar11;
  double dVar12;
  double dVar13;
  double dVar14;
  double dVar15;
  _GS_VERTEX GSVtx;
  nuvec_s nStack_234;
  float local_228;
  float local_224;
  float local_220;
  float local_21c;
  float local_218;
  float local_214;
  nuvec_s nStack_210;
  float local_204;
  float local_200;
  float local_1fc;
  float local_1f8;
  float local_1f4;
  float local_1f0;
  nuvec_s nStack_1ec;
  float local_1e0;
  float local_1dc;
  float local_1d8;
  float local_1d4;
  float local_1d0;
  float local_1cc;
  nuvec_s vec_a;
  nuvec_s local_1b8;
  numtx_s nStack_1a8;
  float local_168 [12];
  undefined4 local_138;
  undefined4 local_134;
  undefined4 local_130;
  nuvec_s nStack_128;
  numtx_s nStack_118;
  numtx_s mtx_dest;
  
  DBTimerStart(0x1c);
  SetupShaders(item);
  NuMtxSetIdentity(&nStack_118);
  NuMtxSetIdentity(&nStack_1a8);
  pnVar3 = item->mtx;
  nStack_1a8.mtx[3][0] = pnVar3->mtx[3][0];
  nStack_1a8.mtx[3][1] = pnVar3->mtx[3][1];
  nStack_1a8.mtx[3][2] = pnVar3->mtx[3][2];
  GS_LoadWorldMatrixIdentity();
  iVar4 = 0x30;
  pfVar1 = (float *)item->mtx;
  pfVar2 = local_168;
  do {
    pfVar6 = pfVar2;
    pfVar5 = pfVar1;
    iVar4 = iVar4 + -0x18;
    *pfVar6 = *pfVar5;
    pfVar6[1] = pfVar5[1];
    pfVar6[2] = pfVar5[2];
    pfVar6[3] = pfVar5[3];
    (*(float (*) [4])(pfVar6 + 4))[0] = (*(float (*) [4])(pfVar5 + 4))[0];
    pfVar6[5] = pfVar5[5];
    pfVar1 = pfVar5 + 6;
    pfVar2 = pfVar6 + 6;
  } while (iVar4 != 0);
  pfVar6[6] = pfVar5[6];
  pnVar7 = item->geom;
  pfVar6[7] = pfVar5[7];
  pfVar6[8] = pfVar5[8];
  pfVar6[9] = pfVar5[9];
  local_130 = 0;
  local_138 = 0;
  local_134 = 0;
  for (; pnVar7 != (nugeom_s *)0x0; pnVar7 = pnVar7->next) {
    iVar4 = pnVar7->vtxmax;
    v0 = (nuvec_s *)pnVar7->vtxcount;
    if (0 < iVar4) {
      dVar9 = 0.5;
      dVar11 = 0.0;
      dVar10 = 1.0;
      do {
        fVar8 = v0[1].z;
        dVar14 = (double)(float)((double)v0[1].x * dVar9);
        dVar15 = (double)(float)((double)v0[1].y * dVar9);
        NuVecMtxRotate(&nStack_128,v0,(numtx_s *)local_168);
        dVar12 = -dVar14;
        dVar13 = -dVar15;
        v0 = v0 + 2;
        NuVecAdd(&vec_a,&nStack_128,(nuvec_s *)item->mtx->mtx[3]);
        NuMtxCalcCheapFaceOn(&mtx_dest,&vec_a);
        local_1b8.x = (float)dVar12;
        local_1b8.y = (float)dVar15;
        local_1b8.z = (float)dVar11;
        NuVecMtxTransform((nuvec_s *)&GSVtx,&local_1b8,&mtx_dest);
        GSVtx.u = (float)dVar11;
        GSVtx.v = (float)dVar11;
        GSVtx.nx = (float)dVar10;
        GSVtx.ny = (float)dVar11;
        GSVtx.nz = (float)dVar11;
        local_1b8.x = (float)dVar14;
        local_1b8.y = (float)dVar15;
        local_1b8.z = (float)dVar11;
        GSVtx.diffuse = (uint)fVar8;
        NuVecMtxTransform(&nStack_234,&local_1b8,&mtx_dest);
        local_1b8.x = (float)dVar14;
        local_218 = (float)dVar10;
        local_214 = (float)dVar11;
        local_228 = (float)dVar10;
        local_224 = (float)dVar11;
        local_220 = (float)dVar11;
        local_1b8.y = (float)dVar13;
        local_1b8.z = (float)dVar11;
        local_21c = fVar8;
        NuVecMtxTransform(&nStack_210,&local_1b8,&mtx_dest);
        local_1b8.x = (float)dVar12;
        local_1b8.y = (float)dVar13;
        local_1f4 = (float)dVar10;
        local_1f0 = (float)dVar10;
        local_204 = (float)dVar10;
        local_200 = (float)dVar11;
        local_1fc = (float)dVar11;
        local_1b8.z = (float)dVar11;
        local_1f8 = fVar8;
        NuVecMtxTransform(&nStack_1ec,&local_1b8,&mtx_dest);
        local_1d0 = (float)dVar11;
        local_1cc = (float)dVar10;
        local_1e0 = (float)dVar10;
        local_1dc = (float)dVar11;
        local_1d8 = (float)dVar11;
        local_1d4 = fVar8;
        GS_DrawPrimitiveQuad(&GSVtx);
        iVar4 = iVar4 + -1;
      } while (iVar4 != 0);
    }
  }
  DBTimerStart(0x1c);
  return;
}



void NuRndrGeomItem(nugeomitem_s *item)

{
  ushort uVar1;
  nuprimtype_e ptype;
  int stride;
  nugeom_s *geom;
  int vtxcnt;
  uint nverts;
  float *vertlist;
  short *buff;
  nuprim_s *prim;
  
  DBTimerStart(6);
  if ((_GSMATRIX *)item->mtx == (_GSMATRIX *)0x0) {
    GS_LoadWorldMatrixIdentity();
  }
  else {
    GS_SetWorldMatrix((_GSMATRIX *)item->mtx);
  }
  SetupShaders(item);
  for (prim = item->geom->prim; prim != (nuprim_s *)0x0; prim = prim->next) {
    ptype = prim->type;
    if (ptype == NUPT_TRISTRIP) {
      geom = item->geom;
      vtxcnt = geom->vtxcount;
      vertlist = (float *)geom->hVB;
      stride = NuVtxStride(geom->vertex_type);
      GS_DrawTriStrip(vtxcnt,vertlist,stride);
    }
    else if (ptype < NUPT_NDXLINE) {
      if (ptype != NUPT_LINE) {
        if (ptype < NUPT_TRI) {
          NuVtxStride(item->geom->vertex_type);
          GS_DrawPointList();
        }
        else {
          geom = item->geom;
          vtxcnt = geom->vtxcount;
          vertlist = (float *)geom->hVB;
          stride = NuVtxStride(geom->vertex_type);
          GS_DrawTriList(vtxcnt,vertlist,stride);
        }
      }
    }
    else if (ptype == NUPT_NDXTRISTRIP) {
      uVar1 = prim->vertexCount;
      buff = (short *)prim->idxbuff;
      vertlist = (float *)item->geom->hVB;
      stride = NuVtxStride(item->geom->vertex_type);
      GS_DrawIndexedTriStrip((uint)uVar1,buff,vertlist,stride);
    }
    else if (ptype < NUPT_BEZPATCH) {
      if (ptype == NUPT_NDXTRI) {
        uVar1 = prim->vertexCount;
        buff = (short *)prim->idxbuff;
        vertlist = (float *)item->geom->hVB;
        stride = NuVtxStride(item->geom->vertex_type);
        GS_DrawIndexedTriList((uint)uVar1,buff,vertlist,stride);
      }
    }
    else if (ptype == NUPT_QUADLIST) {
      geom = item->geom;
      nverts = geom->vtxcount;
      vertlist = (float *)geom->hVB;
      stride = NuVtxStride(geom->vertex_type);
      GS_DrawQuadList(nverts,vertlist,stride);
    }
  }
  DBTimerEnd(6);
  return;
}


void NuRndrSkinItem2(nugeomitem_s *item)

{
  NUERRORFUNC *e;
  nuprim_s *prim;
  
  if ((item->blendvals == (float **)0x0) || (item->geom->blendgeom == (NUBLENDGEOM_s *)0x0)) {
    DBTimerStart(5);
    SetupShaders(item);
    GS_LoadWorldMatrixIdentity();
    GS_SetVertexSource((float *)item->geom->hVB);
    for (prim = item->geom->prim; prim != (nuprim_s *)0x0; prim = prim->next) {
      NuShaderSetSkinningConstants(item,prim);
      if (prim->type == NUPT_NDXTRI) {
        SkinnedShader((uint)prim->vertexCount,(short *)prim->idxbuff);
      }
      else if (prim->type != NUPT_NDXTRISTRIP) {
        e = NuErrorProlog("C:/source/crashwoc/code/nu3dx/nurndr.c",0xf4e);
        (*e)("NuRndrGeomItem : Unknown primitive type!");
      }
    }
    DBTimerEnd(5);
  }
  else {
    NuRndrBlendedSkinItem(item);
  }
  return;
}

/*

void NuRndrBlendedSkinItem(nugeomitem_s *item)

{
  float fVar1;
  int i;
  NUERRORFUNC *e;
  int iVar2;
  float *hVB2;
  int iVar3;
  int iVar4;
  nugeom_s *geom;
  float *hVB;
  int cnt;
  NUBLENDGEOM_s *blendgeom;
  nuprim_s *prim;
  
  blendgeom = item->geom->blendgeom;
  DBTimerStart(4);
  DBTimerStart(0xd);
  PPCMtmmcr0(0x8b);
  PPCMtmmcr1(0x78400000);
  geom = item->geom;
  iVar4 = 0;
  hVB = (float *)geom->hVB;
  hVB2 = (float *)geom->blendgeom->hVB;
  if (0 < geom->vtxcount) {
    do {
      i = 0;
      *hVB2 = *hVB;
      cnt = iVar4 + 1;
      hVB2[1] = hVB[1];
      hVB2[2] = hVB[2];
      if (0 < blendgeom->nblends) {
        iVar4 = iVar4 * 0xc;
        iVar2 = 0;
        do {
          iVar3 = *(int *)(iVar2 + (int)blendgeom->blend_offsets);
          if ((iVar3 != 0) &&
             (fVar1 = (*item->blendvals)[*(int *)(iVar2 + (int)blendgeom->ix)], fVar1 != 0.0)) {
            *hVB2 = *(float *)(iVar4 + iVar3) * fVar1 + *hVB2;
            hVB2[1] = *(float *)(iVar4 + *(int *)(iVar2 + (int)blendgeom->blend_offsets) + 4) *
                      fVar1 + hVB2[1];
            hVB2[2] = *(float *)(iVar4 + *(int *)(iVar2 + (int)blendgeom->blend_offsets) + 8) *
                      fVar1 + hVB2[2];
          }
          i = i + 1;
          iVar2 = iVar2 + 4;
        } while (i < blendgeom->nblends);
      }
      hVB2 = hVB2 + 3;
      iVar4 = cnt;
      hVB = hVB + 0xe;
    } while (cnt < item->geom->vtxcount);
  }
  PPCMtmmcr1(0);
  PPCMtmmcr0(0);
  DBTimerEnd(0xd);
  SetupShaders(item);
  GS_LoadWorldMatrixIdentity();
  GS_SetVertexSource((float *)item->geom->hVB);
  GS_SetBlendSource((_GS_VECTOR3 *)item->geom->blendgeom->hVB);
  for (prim = item->geom->prim; prim != (nuprim_s *)0x0; prim = prim->next) {
    NuShaderSetSkinningConstants(item,prim);
    if (prim->type == NUPT_NDXTRI) {
      SkinnedShaderBlend(prim->vertexCount,prim->idxbuff);
    }
    else {
      e = NuErrorProlog("C:/source/crashwoc/code/nu3dx/nurndr.c",0x1087);
      (*e)("NuRndrGeomItem : Unknown primitive type!");
    }
  }
  DBTimerEnd(4);
  return;
}
*/

/*
void NuRndrAnglesZX(nuvec_s *src,nuvec_s *rot)

{
  uint A2D;
  int iVar1;
  double dVar2;
  nuvec_s local_38;
  double local_20;
  
  A2D = NuAtan2D(src->z,src->y);
  dVar2 = 4503601774854144.0;
  local_20 = (double)CONCAT44(0x43300000,A2D ^ 0x80000000);
  rot->x = (float)(local_20 - 4503601774854144.0);
  NuVecRotateX(&local_38,src,(int)-(float)(local_20 - 4503601774854144.0));
  iVar1 = NuAtan2D(local_38.x,local_38.y);
  local_20 = (double)CONCAT44(0x43300000,-iVar1 ^ 0x80000000);
  rot->z = (float)(local_20 - dVar2);
  return;
}


void NuRndrAddFootPrint(int rot,float sizex,float sizez,int brightness,nuvec_s *pos,nuvec_s *norm ,
                       int gfx,int unknown)

{
  float fVar1;
  float fVar2;
  float fVar3;
  float fVar4;
  float fVar5;
  float fVar6;
  int iVar7;
  uint i2;
  numtx_s m2;
  nuvec_s terrrot;
  numtx_s m;
  longlong local_30;
  uint free;
  float trigtab;
  
  trigtab = NuTrigTable[rot & 0xffff];
  fVar1 = *(float *)((int)NuTrigTable + ((rot + 0x4000) * 4 & 0x3fffcU));
  fVar5 = fVar1 * -sizez;
  fVar4 = trigtab * -sizez;
  fVar3 = -trigtab * -sizex;
  m2._03 = 0.0;
  fVar6 = fVar1 * -sizex;
  m2._30 = fVar6 + fVar4;
  m2._31 = 0.0;
  m2._32 = fVar3 + fVar5;
  m2._21 = 0.0;
  fVar2 = -trigtab * sizex;
  m2._11 = 0.0;
  m2._22 = fVar2 + fVar5;
  m2._01 = 0.0;
  m2._20 = fVar1 * sizex + fVar4;
  m2._10 = fVar6 + trigtab * sizez;
  m2._12 = fVar3 + fVar1 * sizez;
  m2._00 = fVar1 * sizex + trigtab * sizez;
  m2._02 = fVar2 + fVar1 * sizez;
  m2._33 = 0.0;
  m2._23 = 0.0;
  m2._13 = 0.0;
  NuRndrAnglesZX(norm,&terrrot);
  NuMtxSetIdentity(&m);
  local_30 = (longlong)(int)terrrot.z;
  NuMtxRotateZ(&m,(int)terrrot.z);
  local_30 = (longlong)(int)terrrot.x;
  NuMtxRotateX(&m,(int)terrrot.x);
  NuVec4MtxTransformVU0((nuvec4_s *)&m2,(nuvec4_s *)&m2,&m);
  NuVec4MtxTransformVU0((nuvec4_s *)&m2._10,(nuvec4_s *)&m2._10,&m);
  NuVec4MtxTransformVU0((nuvec4_s *)&m2._20,(nuvec4_s *)&m2._20,&m);
  NuVec4MtxTransformVU0((nuvec4_s *)&m2._30,(nuvec4_s *)&m2._30,&m);
  free = NuRndrFootFree & 0x3f;
  NuRndrFootData[free].pnts[0].x = m2._00 + pos->x;
  NuRndrFootData[free].pnts[0].y = m2._01 + pos->y;
  i2 = NuRndrFootFree + 1;
  NuRndrFootData[free].pnts[0].z = m2._02 + pos->z;
  iVar7 = NuRndrFootFree + 9;
  NuRndrFootData[free].pnts[1].x = m2._10 + pos->x;
  NuRndrFootData[free].pnts[1].y = m2._11 + pos->y;
  NuRndrFootData[free].pnts[1].z = m2._12 + pos->z;
  NuRndrFootData[free].pnts[2].x = m2._20 + pos->x;
  NuRndrFootData[free].pnts[2].y = m2._21 + pos->y;
  NuRndrFootData[free].pnts[2].z = m2._22 + pos->z;
  NuRndrFootData[free].pnts[3].x = m2._30 + pos->x;
  NuRndrFootData[free].pnts[3].y = m2._31 + pos->y;
  NuRndrFootFree = i2;
  NuRndrFootData[free].pnts[3].z = m2._32 + pos->z;
  NuRndrFootData[free].brightness = (short)brightness;
  NuRndrFootData[free].gfx = (char)(gfx << 2);
  NuRndrFootData[free].timer = '\x10';
  for (; (int)i2 < iVar7; i2 = i2 + 1) {
    if ('\x0f' < NuRndrFootData[i2 & 0x3f].timer) {
      NuRndrFootData[i2 & 0x3f].timer = '\x0f';
    }
  }
  return;
}


void NuRndrFootPrints(numtl_s *mtl,float *u,float *v)

{
  char cVar1;
  short sVar2;
  int iVar3;
  nuvec_s *pnVar4;
  float *pfVar5;
  float fVar6;
  nuvtx_tc1_s *pnVar7;
  float fVar8;
  float fVar9;
  float fVar10;
  int iVar11;
  undefined4 *puVar12;
  undefined4 *puVar13;
  undefined4 *puVar14;
  int iVar15;
  nuvtx_tc1_s *local_30;
  undefined4 *local_2c;
  undefined4 *local_28;
  undefined4 *local_24;
  
  iVar15 = 0x40;
  pfVar5 = (float *)&DAT_801bd184;
  pnVar4 = NuRndrFootData[0].pnts + 3;
  iVar11 = 0;
  do {
    cVar1 = *(char *)((int)(pnVar4 + 1) + 2);
    if ('\0' < cVar1) {
      if (cVar1 < '\x10') {
        *(char *)((int)(pnVar4 + 1) + 2) = cVar1 + -1;
      }
      iVar11 = iVar11 + 4;
      fVar6 = *(float *)((FootData *)(pnVar4 + -3))->pnts;
      fVar8 = *(float *)((FootData *)(pnVar4 + -3))->pnts;
      cVar1 = *(char *)((int)(pnVar4 + 1) + 3);
      pfVar5[-0x1b] = ((FootData *)(pnVar4 + -3))->pnts[0].x;
      pfVar5[-0x1a] = fVar6;
      iVar3 = (int)cVar1;
      pfVar5[-0x19] = fVar8;
      fVar9 = pnVar4[-2].x;
      fVar10 = pnVar4[-2].y;
      pfVar5[-0x14] = u[iVar3];
      fVar6 = v[iVar3];
      fVar8 = pnVar4[-2].z;
      pfVar5[-0x12] = fVar9;
      pfVar5[-0x13] = fVar6;
      pfVar5[-0x11] = fVar10;
      pfVar5[-0x10] = fVar8;
      fVar8 = pnVar4[-1].y;
      fVar9 = pnVar4[-1].x;
      pfVar5[-0xb] = u[iVar3 + 1];
      fVar6 = v[iVar3 + 1];
      pfVar5[-7] = pnVar4[-1].z;
      pfVar5[-9] = fVar9;
      pfVar5[-10] = fVar6;
      pfVar5[-8] = fVar8;
      fVar6 = pnVar4->x;
      fVar9 = pnVar4->y;
      pfVar5[-2] = u[iVar3 + 2];
      fVar8 = pnVar4->z;
      pfVar5[-1] = v[iVar3 + 2];
      *pfVar5 = fVar6;
      cVar1 = *(char *)((int)(pnVar4 + 1) + 2);
      sVar2 = *(short *)(pnVar4 + 1);
      pfVar5[1] = fVar9;
      pfVar5[2] = fVar8;
      pfVar5[7] = u[iVar3 + 3];
      fVar6 = v[iVar3 + 3];
      fVar8 = (float)(((int)sVar2 * (int)cVar1 * 0x100000 & 0xff000000U) + 0x808080);
      pfVar5[-0x15] = fVar8;
      pfVar5[8] = fVar6;
      pfVar5[6] = fVar8;
      pfVar5[-3] = fVar8;
      pfVar5[-0xc] = fVar8;
      pfVar5 = pfVar5 + 0x24;
    }
    pnVar4 = (nuvec_s *)((int)(pnVar4 + 4) + 4);
    iVar15 = iVar15 + -1;
  } while (iVar15 != 0);
  if ((iVar11 != 0) && (iVar15 = 0, 0 < iVar11)) {
    pnVar7 = (nuvtx_tc1_s *)&vtx.247;
    puVar12 = &DAT_801bd184;
    puVar13 = &DAT_801bd160;
    puVar14 = &DAT_801bd13c;
    do {
      local_30 = pnVar7;
      local_2c = puVar14;
      local_28 = puVar13;
      local_24 = puVar12;
      NuRndrStrip3d((nuvtx_tc1_s *)&local_30,mtl,0,4);
      iVar15 = iVar15 + 4;
      puVar12 = puVar12 + 0x24;
      puVar13 = puVar13 + 0x24;
      puVar14 = puVar14 + 0x24;
      pnVar7 = pnVar7 + 4;
    } while (iVar15 < iVar11);
  }
  return;
}


*/


float * NuRndrCreateBlendShapeDeformerWeightsArray(int nweights)

{
  float *ptr;
  
  rndr_blend_shape_deformer_wt_cnt = rndr_blend_shape_deformer_wt_cnt - nweights;
  if (rndr_blend_shape_deformer_wt_cnt < 0) {
    e = NuErrorProlog("C:/source/crashwoc/code/nu3dx/nurndr.c",0x11f4);
    //(*e)("No free blend shape deformer weights");
    ptr = (float *)0x0;
  }
  else {
    ptr = (float *)(&rndr_blend_shape_deformer_wts + rndr_blend_shape_deformer_wt_cnt * 4);
  }
  return ptr;
}


float ** NuRndrCreateBlendShapeDWAPointers(int size)

{
  float **ptr;
  
  rndr_blend_shape_deformer_wt_ptrs_cnt = rndr_blend_shape_deformer_wt_ptrs_cnt - size;
  if (rndr_blend_shape_deformer_wt_cnt < 0) {
    e = NuErrorProlog("C:/source/crashwoc/code/nu3dx/nurndr.c",0x1206);
    (*e)("No free blend shape deformer weights");
    ptr = (float **)0x0;
  }
  else {
    ptr = (float **)(&rndr_blend_shape_deformer_wt_ptrs + rndr_blend_shape_deformer_wt_ptrs_cnt *  4)
    ;
  }
  return ptr;
}

void NuRndrInitWorld(void)

{
  char *timer;
  int lp;
  
  lp = 0x40;
  NuRndrShadMaskCount = 0;
  timer = &NuRndrFootData[0x3f].timer;
  do {
    *timer = '\0';
    timer = timer + -0x34;
    lp = lp + -1;
  } while (lp != 0);
  return;
}

void NuRndrFx(int paused,nuvec_s *playerpos)

{
  NuLgtArcLaserDraw(paused);
  return;
}


void NuRndrTrail(int trailpt,trail *trail,int TRAILCOUNT)

{
  float fVar1;
  uint uVar2;
  undefined4 *puVar3;
  float fVar4;
  uint in_r12;
  uint uVar5;
  int iVar6;
  int unaff_r15;
  uint uVar7;
  int iVar8;
  uint uVar9;
  int iVar10;
  int iVar11;
  int iVar12;
  int iVar13;
  undefined4 *puVar14;
  undefined4 *puVar15;
  float *pfVar16;
  undefined4 *puVar17;
  nuvec_s local_58;
  undefined4 *local_4c;
  
  iVar6 = (trailpt + 1) - TRAILCOUNT;
  iVar8 = 0;
  if (iVar6 < trailpt) {
    uVar7 = TRAILCOUNT - 1;
    iVar12 = 0x6c;
    iVar13 = 0x48;
    iVar10 = 0x24;
    iVar11 = 0;
    pfVar16 = (float *)&DAT_801cf13c;
    uVar2 = 0xffffffff;
    do {
      uVar9 = trailpt & uVar7;
      if (trail[uVar9].pos1.x == -10000.0) {
        uVar9 = 0xffffffff;
      }
      else if (uVar2 == 0xffffffff) {
        in_r12 = 0;
        unaff_r15 = 1;
      }
      else {
        uVar5 = trail[uVar9].intensity;
        if ((trail[trailpt - 1U & uVar7].pos1.x == -10000.0) || ((int)(trailpt - 1U) <= iVar6)) {
          uVar5 = 0;
        }
        else if ((trail[trailpt - 2U & uVar7].pos1.x == -10000.0) || (unaff_r15 != 0)) {
          uVar5 = (((int)uVar5 >> 0x18) - ((int)uVar5 >> 0x1f)) * 0x800000 & 0xff000000;
        }
        fVar4 = trail[uVar2].pos1.y;
        fVar1 = trail[uVar2].pos1.z;
        *(float *)((int)&vtx.263 + iVar11) = trail[uVar2].pos1.x;
        *(float *)((int)&DAT_801cf120 + iVar11) = fVar1;
        *(float *)((int)&DAT_801cf11c + iVar11) = fVar4;
        unaff_r15 = 0;
        fVar1 = trail[uVar9].pos1.y;
        fVar4 = trail[uVar9].pos1.z;
        *pfVar16 = trail[uVar9].pos1.x;
        pfVar16[1] = fVar1;
        pfVar16[2] = fVar4;
        fVar1 = trail[uVar2].pos1.x;
        *(float *)((int)&vtx.263 + iVar13) = (trail[uVar2].pos2.x - fVar1) * 0.25 + fVar1;
        fVar1 = trail[uVar2].pos1.y;
        *(float *)((int)&DAT_801cf11c + iVar13) = (trail[uVar2].pos2.y - fVar1) * 0.25 + fVar1;
        fVar1 = trail[uVar2].pos1.z;
        *(float *)((int)&DAT_801cf120 + iVar13) = (trail[uVar2].pos2.z - fVar1) * 0.25 + fVar1;
        fVar1 = trail[uVar9].pos1.x;
        *(float *)((int)&vtx.263 + iVar12) = (trail[uVar9].pos2.x - fVar1) * 0.25 + fVar1;
        fVar1 = trail[uVar9].pos1.y;
        *(float *)((int)&DAT_801cf11c + iVar12) = (trail[uVar9].pos2.y - fVar1) * 0.25 + fVar1;
        fVar1 = trail[uVar9].pos1.z;
        *(float *)((int)&DAT_801cf120 + iVar12) = (trail[uVar9].pos2.z - fVar1) * 0.25 + fVar1;
        if (-1 < (int)in_r12) {
          *(uint *)((int)&DAT_801cf130 + iVar11) = in_r12 + 0xffffff;
          *(undefined4 *)((int)&DAT_801cf130 + iVar13) = 0xffffff;
        }
        else {
          *(uint *)((int)&DAT_801cf130 + iVar11) = -in_r12;
          *(undefined4 *)((int)&DAT_801cf130 + iVar13) = 0;
        }
        if (-1 < (int)uVar5) {
          *(uint *)((int)&DAT_801cf130 + iVar10) = uVar5 + 0xffffff;
          *(undefined4 *)((int)&DAT_801cf130 + iVar12) = 0xffffff;
        }
        else {
          *(uint *)((int)&DAT_801cf130 + iVar10) = -uVar5;
          *(undefined4 *)((int)&DAT_801cf130 + iVar12) = 0;
        }
        fVar4 = trail[uVar2].pos2.z;
        fVar1 = trail[uVar2].pos2.y;
        *(float *)((int)&DAT_801cf1a8 + iVar11) = trail[uVar2].pos2.x;
        *(float *)((int)&DAT_801cf1b0 + iVar11) = fVar4;
        *(float *)((int)&DAT_801cf1ac + iVar11) = fVar1;
        fVar4 = trail[uVar9].pos2.z;
        fVar1 = trail[uVar9].pos2.y;
        pfVar16[0x24] = trail[uVar9].pos2.x;
        pfVar16[0x25] = fVar1;
        pfVar16[0x26] = fVar4;
        fVar1 = trail[uVar2].pos2.x;
        *(float *)((int)&DAT_801cf1a8 + iVar13) = (trail[uVar2].pos1.x - fVar1) * 0.25 + fVar1;
        fVar1 = trail[uVar2].pos2.y;
        *(float *)((int)&DAT_801cf1ac + iVar13) = (trail[uVar2].pos1.y - fVar1) * 0.25 + fVar1;
        fVar1 = trail[uVar2].pos2.z;
        *(float *)((int)&DAT_801cf1b0 + iVar13) = (trail[uVar2].pos1.z - fVar1) * 0.25 + fVar1;
        fVar1 = trail[uVar9].pos2.x;
        *(float *)((int)&DAT_801cf1a8 + iVar12) = (trail[uVar9].pos1.x - fVar1) * 0.25 + fVar1;
        fVar1 = trail[uVar9].pos2.y;
        *(float *)((int)&DAT_801cf1ac + iVar12) = (trail[uVar9].pos1.y - fVar1) * 0.25 + fVar1;
        fVar1 = trail[uVar9].pos2.z;
        *(float *)((int)&DAT_801cf1b0 + iVar12) = (trail[uVar9].pos1.z - fVar1) * 0.25 + fVar1;
        if (-1 < (int)in_r12) {
          *(uint *)((int)&DAT_801cf1c0 + iVar11) = in_r12;
          *(undefined4 *)((int)&DAT_801cf1c0 + iVar13) = 0;
        }
        else {
          *(uint *)((int)&DAT_801cf1c0 + iVar11) = 0xfffff - in_r12;
          *(undefined4 *)((int)&DAT_801cf1c0 + iVar13) = 0xffffff;
        }
        if (-1 < (int)uVar5) {
          *(uint *)((int)&DAT_801cf1c0 + iVar10) = uVar5;
          *(undefined4 *)((int)&DAT_801cf1c0 + iVar12) = 0;
        }
        else {
          *(uint *)((int)&DAT_801cf1c0 + iVar10) = 0xffffff - uVar5;
          *(undefined4 *)((int)&DAT_801cf1c0 + iVar12) = 0xffffff;
        }
        iVar12 = iVar12 + 0x120;
        iVar13 = iVar13 + 0x120;
        pfVar16 = pfVar16 + 0x48;
        iVar10 = iVar10 + 0x120;
        iVar11 = iVar11 + 0x120;
        iVar8 = iVar8 + 8;
        in_r12 = uVar5;
      }
      trailpt = trailpt - 1;
      uVar2 = uVar9;
    } while (iVar6 < trailpt);
  }
  if (iVar8 != 0) {
    iVar6 = 0;
    if (0 < iVar8 + -4) {
      puVar3 = &vtx.263;
      puVar14 = &DAT_801cf184;
      puVar17 = &DAT_801cf13c;
      local_58.z = (float)&DAT_801cf160;
      do {
        iVar6 = iVar6 + 4;
        puVar15 = (undefined4 *)((int)local_58.z + 0x90);
        local_58.x = (float)puVar3;
        local_58.y = (float)puVar17;
        local_4c = puVar14;
        NuRndrStrip3d(&local_58,NuLightAddMat,0,4);
        puVar14 = puVar14 + 0x24;
        puVar17 = puVar17 + 0x24;
        puVar3 = puVar3 + 0x24;
        local_58.z = (float)puVar15;
      } while (iVar6 < iVar8 + -4);
    }
  }
  return;
}

void NuRndrWaterRip(numtl_s *mtl)	//TODO
{
  float fVar1;
  short sVar2;
  int iVar3;
  float fVar4;
  float *pfVar5;
  float fVar6;
  short *psVar7;
  int iVar8;
  WaterDat *dat;
  double dVar9;
  double dVar10;
  double dVar11;
  double dVar12;
  double dVar13;
  
  iVar8 = 0;
  NuCameraGetMtx();
  NuRndrWaterRipCnt = 0;
  dat = NuRndrWaterRipDat;
  do {
    if ((dat->timer != 0) &&
       (iVar3 = NuCameraClipTestPoints(&dat->pos,1,(numtx_s *)0x0), iVar3 == 0)) {
      indexlist.272[NuRndrWaterRipCnt] = (short)iVar8;
      NuRndrWaterRipCnt = NuRndrWaterRipCnt + 1;
    }
    iVar8 = iVar8 + 1;
    dat = dat + 1;
  } while (iVar8 < 0x20);
  if (NuRndrWaterRipCnt != 0) {
    iVar8 = 4;
    pfVar5 = (float *)&DAT_801e112c;
    do {
      pfVar5[-2] = 1.0;
      pfVar5[-1] = 0.0;
      *pfVar5 = 0.0;
      pfVar5 = pfVar5 + 9;
      iVar8 = iVar8 + -1;
    } while (iVar8 != 0);
    DAT_801e11b4 = &DAT_801e1184;
    vtx2.271 = &vtx.270;
    iVar8 = 0;
    DAT_801e11ac = &DAT_801e113c;
    DAT_801e11b0 = &DAT_801e1160;
    if (0 < NuRndrWaterRipCnt) {
      dVar9 = 0.1;
      dVar10 = 0.75;
      dVar11 = 0.25;
      dVar13 = 0.5;
      dVar12 = 1.0;
      psVar7 = indexlist.272;
      do {
        sVar2 = *psVar7;
        DAT_801e1134 = (float)dVar10;
        DAT_801e1138 = (float)dVar11;
        DAT_801e1158 = (float)dVar12;
        DAT_801e115c = (float)dVar11;
        DAT_801e117c = (float)dVar10;
        iVar8 = iVar8 + 1;
        DAT_801e1180 = (float)dVar13;
        psVar7 = psVar7 + 1;
        fVar6 = NuRndrWaterRipDat[sVar2].pos.x;
        fVar4 = NuRndrWaterRipDat[sVar2].pos.z;
        fVar1 = NuRndrWaterRipDat[sVar2].cursize;
        DAT_801e113c = fVar6 + fVar1;
        DAT_801e1168 = fVar4 + fVar1;
        DAT_801e111c = (float)((double)NuRndrWaterRipDat[sVar2].pos.y - dVar9);
        DAT_801e1130 = NuRndrWaterRipDat[sVar2].curshade + 0x80808080;
        vtx.270 = fVar6 - fVar1;
        DAT_801e1120 = fVar4 - fVar1;
        DAT_801e11a0 = (float)dVar12;
        DAT_801e11a4 = (float)dVar13;
        DAT_801e1140 = DAT_801e111c;
        DAT_801e1144 = DAT_801e1120;
        DAT_801e1154 = DAT_801e1130;
        DAT_801e1160 = vtx.270;
        DAT_801e1164 = DAT_801e111c;
        DAT_801e1178 = DAT_801e1130;
        DAT_801e1184 = DAT_801e113c;
        DAT_801e1188 = DAT_801e111c;
        DAT_801e118c = DAT_801e1168;
        DAT_801e119c = DAT_801e1130;
        NuRndrStrip3d((nuvtx_tc1_s *)&vtx2.271,mtl,0,4);
      } while (iVar8 < NuRndrWaterRipCnt);
    }
  }
  return;
}

void NuRndrRectUV2di(int x,int y,int w,int h,float tx,float ty,float tw,float th,int col,
                    numtl_s *mtl)

{
  double dVar1;
  double dVar2;
  double dVar3;
  double dVar4;
  float local_118;
  float local_114;
  undefined4 local_110;
  undefined4 local_10c;
  int local_108;
  float local_104;
  float local_100;
  float local_fc;
  float local_f8;
  undefined4 local_f4;
  undefined4 local_f0;
  int local_ec;
  float local_e8;
  float local_e4;
  float local_e0;
  float local_dc;
  undefined4 local_d8;
  undefined4 local_d4;
  int local_d0;
  float local_cc;
  float local_c8;
  float local_c4;
  float local_c0;
  undefined4 local_bc;
  undefined4 local_b8;
  int local_b4;
  float local_b0;
  float local_ac;
  float local_a8;
  float local_a4;
  undefined4 local_a0;
  undefined4 local_9c;
  int local_98;
  float local_94;
  float local_90;
  float local_8c;
  float local_88;
  undefined4 local_84;
  undefined4 local_80;
  int local_7c;
  float local_78;
  float local_74;
  undefined4 local_48;
  uint uStack_44;
  
  dVar4 = (double)th;
  dVar3 = (double)tw;
  dVar2 = (double)ty;
  dVar1 = (double)tx;
  GS_SetOrthMatrix();
  uStack_44 = (y + h) - 1U ^ 0x80000000;
  local_118 = (float)((double)CONCAT44(0x43300000,x ^ 0x80000000) - 4503601774854144.0);
  local_114 = (float)((double)CONCAT44(0x43300000,y ^ 0x80000000) - 4503601774854144.0);
  local_e8 = (float)(dVar1 + dVar3);
  local_c8 = (float)(dVar2 + dVar4);
  local_48 = 0x43300000;
  local_fc = (float)((double)CONCAT44(0x43300000,(x + w) - 1U ^ 0x80000000) - 4503601774854144.0);
  local_ac = (float)dVar2;
  local_dc = (float)((double)CONCAT44(0x43300000,uStack_44) - 4503601774854144.0);
  local_110 = 0x358637bd;
  local_10c = 0x3f800000;
  local_f4 = 0x358637bd;
  local_f0 = 0x3f800000;
  local_d8 = 0x358637bd;
  local_d4 = 0x3f800000;
  local_bc = 0x358637bd;
  local_b8 = 0x3f800000;
  local_104 = (float)dVar1;
  local_100 = (float)dVar2;
  local_e4 = (float)dVar2;
  local_cc = (float)dVar1;
  local_a0 = 0x358637bd;
  local_84 = 0x358637bd;
  local_80 = 0x3f800000;
  local_78 = (float)dVar1;
  local_9c = 0x3f800000;
  local_108 = col;
  local_f8 = local_114;
  local_ec = col;
  local_e0 = local_118;
  local_d0 = col;
  local_c4 = local_fc;
  local_c0 = local_114;
  local_b4 = col;
  local_b0 = local_e8;
  local_a8 = local_fc;
  local_a4 = local_dc;
  local_98 = col;
  local_94 = local_e8;
  local_90 = local_c8;
  local_8c = local_118;
  local_88 = local_dc;
  local_7c = col;
  local_74 = local_c8;
  ResetShaders();
  NuTexSetTexture(0,0);
  NuMtlSetRenderStates(mtl);
  NuTexSetTextureStates(mtl);
  SetVertexShader(0x144);
  NuTexSetTexture(0,mtl->tid);
  GS_DrawTriListTTL((int)&local_118,6);
  return;
}


void NuRndrShadPolys(numtl_s *mtl)

{
  float fVar1;
  float fVar2;
  float *pfVar3;
  float fVar4;
  int i;
  ShadPolDat *dat;
  double dVar5;
  double dVar6;
  double dVar7;
  nuvec_s *v;
  
  if (NuRndrShadowCnt != 0) {
    i = 4;
    pfVar3 = (float *)&DAT_801e120c;
    do {
      pfVar3[-2] = 0.0;
      pfVar3[-1] = 0.0;
      *pfVar3 = 0.0;
      pfVar3 = pfVar3 + 9;
      i = i + -1;
    } while (i != 0);
    vtx2.283[3] = (nuvec_s *)&DAT_801e1264;
    vtx2.283[0] = (nuvec_s *)&vtx.282;
    i = 0;
    vtx2.283[1] = (nuvec_s *)&DAT_801e121c;
    vtx2.283[2] = (nuvec_s *)&DAT_801e1240;
    if (0 < NuRndrShadowCnt) {
      dVar5 = 0.009999999776482582;
      dVar6 = 0.0;
      dVar7 = 1.0;
      dat = NuRndrShadPolDat;
      do {
        fVar2 = (dat->pos).x;
        v = &dat->pos;
        fVar4 = (dat->pos).z;
        i = i + 1;
        fVar1 = dat->size;
        dat = dat + 1;
        DAT_801e121c = fVar2 + fVar1;
        DAT_801e1248 = fVar4 + fVar1;
        DAT_801e127c = 0xff000000;
        DAT_801e11fc = (float)((double)v->y + dVar5);
        vtx.282 = fVar2 - fVar1;
        DAT_801e1200 = fVar4 - fVar1;
        DAT_801e1210 = 0xff000000;
        DAT_801e1234 = 0xff000000;
        DAT_801e1258 = 0xff000000;
        DAT_801e1214 = (float)dVar6;
        DAT_801e1218 = (float)dVar6;
        DAT_801e1238 = (float)dVar7;
        DAT_801e123c = (float)dVar6;
        DAT_801e125c = (float)dVar6;
        DAT_801e1260 = (float)dVar7;
        DAT_801e1280 = (float)dVar7;
        DAT_801e1284 = (float)dVar7;
        DAT_801e1220 = DAT_801e11fc;
        DAT_801e1224 = DAT_801e1200;
        DAT_801e1240 = vtx.282;
        DAT_801e1244 = DAT_801e11fc;
        DAT_801e1264 = DAT_801e121c;
        DAT_801e1268 = DAT_801e11fc;
        DAT_801e126c = DAT_801e1248;
        NuRndrStrip3d((nuvtx_tc1_s *)vtx2.283,mtl,0,4);
      } while (i < NuRndrShadowCnt);
    }
    NuRndrShadowCnt = 0;
  }
  return;
}



void NuRndrWaterRippleUpdate(int count)

{
  float fVar1;
  short sVar2;
  WaterDat *pWVar3;
  int iVar4;
  double dVar5;
  double local_8;
  
  iVar4 = 0x20;
  pWVar3 = NuRndrWaterRipDat;
  do {
    if (pWVar3->timer != 0) {
      sVar2 = pWVar3->timer - (short)count;
      pWVar3->timer = sVar2;
      if (sVar2 < 1) {
        pWVar3->timer = 0;
      }
      else {
        local_8 = (double)CONCAT44(0x43300000,(int)sVar2 ^ 0x80000000);
        dVar5 = local_8 - 4503601774854144.0;
        local_8 = (double)CONCAT44(0x43300000,(int)pWVar3->otimer ^ 0x80000000);
        fVar1 = (float)dVar5 / (float)(local_8 - 4503601774854144.0);
        pWVar3->cursize = (pWVar3->size - pWVar3->endsize) * fVar1 + pWVar3->endsize;
        if (fVar1 <= 0.75) {
          fVar1 = fVar1 * 1.333333;
        }
        else {
          fVar1 = (1.0 - fVar1) * 4.0;
        }
        local_8 = (double)CONCAT44(0x43300000,pWVar3->shade >> 0x18);
        pWVar3->curshade =
             (int)((float)(local_8 - 4503599627370496.0) * fVar1) * 0x1000000 +
             (pWVar3->shade & 0xffffff);
      }
    }
    pWVar3 = pWVar3 + 1;
    iVar4 = iVar4 + -1;
  } while (iVar4 != 0);
  return;
}


void NuRndrAddWaterRipple(nuvec_s *pos,float size,float endsize,int duration,int shade)

{
  float y;
  float x;
  WaterDat *ripDat;
  float z;
  int i;
  
  i = 0x20;
  ripDat = NuRndrWaterRipDat;
  do {
    if (ripDat->timer == 0) {
      z = pos->z;
      x = pos->x;
      pos->y = pos->y + 0.01;
      y = pos->y;
      ripDat->otimer = (short)duration;
      (ripDat->pos).x = x;
      (ripDat->pos).y = y;
      (ripDat->pos).z = z;
      ripDat->cursize = size;
      ripDat->endsize = endsize;
      ripDat->shade = shade;
      ripDat->curshade = 0;
      ripDat->size = size;
      ripDat->timer = (short)duration;
      return;
    }
    ripDat = ripDat + 1;
    i = i + -1;
  } while (i != 0);
  return;
}

void NuRndrAddShadow(struct NuVec* v, s16 shade, s16 xrot, s16 yrot, s16 zrot, f32 arg8) {

    if ((NuCameraClipTestPoints(v, 1, NULL) == 0) && ((s32) NuRndrShadowCnt <= 0x7F)) {
        v->y += 0.01f;
        NuRndrShadPolDat[NuRndrShadowCnt].pos.x = v->x;
        NuRndrShadPolDat[NuRndrShadowCnt].pos.y = v->y;
        NuRndrShadPolDat[NuRndrShadowCnt].pos.z = v->z;
        *(&NuRndrShadPolDat->size + (NuRndrShadowCnt * 0x18)) = arg8;         //NuRndrShadPolDat[NuRndrShadowCnt].size = arg8;
        NuRndrShadPolDat[NuRndrShadowCnt].zrot = zrot;
        NuRndrShadPolDat[NuRndrShadowCnt].shade = shade;
        NuRndrShadPolDat[NuRndrShadowCnt].xrot = xrot;
        NuRndrShadPolDat[NuRndrShadowCnt].yrot = yrot;
        NuRndrShadowCnt += 1;
    }
	return;
}



void NuRndrInitEx(int streambuffersize)

{
  NuRndrInit();
  return;
}

int NuRndrGScnObj(nugobj_s *gobj,numtx_s *wm)

{
  int ret;
  
  ret = NuRndrGobj(gobj,wm,NULL);
  return ret;
}

/*void NuRndrRect2di(int a,int b,int SWIDTH,int SHEIGHT,int fadecol,numtl_s *mtl)

{
  GS_DrawFade(fadecol);
  return;
}


void NuRndrParticleGroup(rdata_s *data,setup_s *setup,numtl_s *mtl,float time,numtx_s *wm)

{
  int i;
  _sceDmaTag *data2;
  double dVar1;
  bool check;
  
  dVar1 = (double)time;
  setup->DmaHeader[3] = (int)time;
  check = false;
  i = 0;
  do {
    if (check) {
      return;
    }
    data2 = (_sceDmaTag *)data->dmadata[1];
    if (data->dmadata[0] == 0) {
      if (data2 != (_sceDmaTag *)0x0) {
        renderpsdma(0x20,data,setup,mtl,(float)dVar1,wm);
        data = (rdata_s *)data2;
      }
    }
    else {
      if (data->dmadata[0] == 1) {
        renderpsdma(0x20,data,setup,mtl,(float)dVar1,wm);
      }
      check = true;
    }
    i = i + 1;
  } while (i < 0x101);
  return;
}
*/


int NuHGobjRndr(NUHGOBJ_s *hgobj,numtx_s *wm,int nlayers,short *layers)

{
  uint uVar1;
  NULAYERDATA_s *pNVar2;
  NUERRORFUNC *e;
  nugobj_s *pnVar3;
  int iVar4;
  numtx_s *pnVar5;
  int iVar6;
  int iVar7;
  numtx_s *pnVar8;
  int iVar9;
  int iVar10;
  
  iVar9 = nlayers + -1;
  if ((int)(uint)hgobj->num_layers <= iVar9) {
    e = NuErrorProlog("C:/source/crashwoc/code/nu3dx/nurndr.c",0x8a8);
    (*e)("assert");
  }
  uVar1 = (uint)hgobj->num_joints;
  iVar10 = 0;
  if (uVar1 != 0) {
    iVar6 = 0;
    pnVar5 = mtx_arrayHGobj;
    iVar7 = 0;
    do {
      iVar4 = *(int *)(hgobj->joints->pad + iVar6 + -5);
      pnVar8 = (numtx_s *)((int)&hgobj->T->_00 + iVar7);
      if (iVar4 == -1) {
        NuMtxMul(pnVar5,pnVar8,wm);
      }
      else {
        NuMtxMul(pnVar5,pnVar8,mtx_arrayHGobj + iVar4);
      }
      uVar1 = (uint)hgobj->num_joints;
      iVar10 = iVar10 + 1;
      iVar6 = iVar6 + 0x60;
      pnVar5 = pnVar5 + 1;
      iVar7 = iVar7 + 0x40;
    } while (iVar10 < (int)uVar1);
  }
  pNVar2 = hgobj->layers;
  if ((pNVar2[iVar9].gobjs != (nugobj_s **)0x0) && (iVar10 = 0, uVar1 != 0)) {
    iVar6 = 0;
    pnVar5 = mtx_arrayHGobj;
    do {
      pnVar3 = *(nugobj_s **)(iVar6 + (int)hgobj->layers[iVar9].gobjs);
      if (pnVar3 != (nugobj_s *)0x0) {
        NuRndrGobj(pnVar3,pnVar5,(float **)0x0);
      }
      uVar1 = (uint)hgobj->num_joints;
      iVar10 = iVar10 + 1;
      pnVar5 = pnVar5 + 1;
      iVar6 = iVar6 + 4;
      pNVar2 = hgobj->layers;
    } while (iVar10 < (int)uVar1);
  }
  if ((pNVar2[iVar9].blend_gobjs != (nugobj_s **)0x0) && (iVar10 = 0, uVar1 != 0)) {
    iVar6 = 0;
    pnVar5 = mtx_arrayHGobj;
    do {
      pnVar3 = *(nugobj_s **)(iVar6 + (int)hgobj->layers[iVar9].blend_gobjs);
      if (pnVar3 != (nugobj_s *)0x0) {
        NuRndrGobj(pnVar3,pnVar5,(float **)0x0);
      }
      iVar10 = iVar10 + 1;
      pnVar5 = pnVar5 + 1;
      iVar6 = iVar6 + 4;
    } while (iVar10 < (int)(uint)hgobj->num_joints);
  }
  iVar10 = 0;
  if (hgobj->num_joints != '\0') {
    pnVar8 = mtx_array2HGobj;
    iVar6 = 0;
    pnVar5 = mtx_arrayHGobj;
    do {
      iVar10 = iVar10 + 1;
      NuMtxMul(pnVar8,(numtx_s *)((int)&hgobj->INV_WT->_00 + iVar6),pnVar5);
      pnVar8 = pnVar8 + 1;
      iVar6 = iVar6 + 0x40;
      pnVar5 = pnVar5 + 1;
    } while (iVar10 < (int)(uint)hgobj->num_joints);
  }
  pnVar3 = hgobj->layers[iVar9].skin_gobj;
  if (pnVar3 != (nugobj_s *)0x0) {
    NuRndrGobjSkin2(pnVar3,(uint)hgobj->num_joints,mtx_array2HGobj,(float **)0x0);
  }
  pnVar3 = hgobj->layers[iVar9].blend_skin_gobj;
  if (pnVar3 != (nugobj_s *)0x0) {
    NuRndrGobjSkin2(pnVar3,(uint)hgobj->num_joints,mtx_array2HGobj,(float **)0x0);
  }
  return 1;
}


int NuHGobjRndrMtxDwa(NUHGOBJ_s *hgobj,numtx_s *wm,int nlayers,short *layers,numtx_s *mtx_array,
                     float **dwa)

{
  uint uVar1;
  nugobj_s *gobj;
  int i;
  int j;
  NULAYERDATA_s *layerdat;
  uint uVar2;
  int iVar3;
  int iVar4;
  numtx_s *mtx;
  numtx_s *m_00;
  numtx_s m;
  
  uVar2 = 0;
  if (hgobj_enabled.231 == 0) {
    uVar2 = 1;
  }
  else {
    j = 0;
    if (0 < nlayers) {
      do {
        if (layers == (short *)0x0) {
          nlayers = 1;
          i = 0;
LAB_800b6930:
          if ((-1 < i) && (i <= (int)(uint)hgobj->num_layers)) {
            layerdat = hgobj->layers;
            if ((layerdat[i].gobjs != (nugobj_s **)0x0) && (iVar3 = 0, hgobj->num_joints != '\0'))  {
              iVar4 = 0;
              mtx = mtx_array;
              do {
                if (*(int *)(iVar4 + (int)hgobj->layers[i].gobjs) != 0) {
                  NuMtxMul(&m,mtx,wm);
                  uVar1 = NuRndrGobj(*(nugobj_s **)(iVar4 + (int)hgobj->layers[i].gobjs),&m,
                                     (float **)0x0);
                  uVar2 = uVar2 | uVar1;
                }
                iVar3 = iVar3 + 1;
                mtx = mtx + 1;
                iVar4 = iVar4 + 4;
                layerdat = hgobj->layers;
              } while (iVar3 < (int)(uint)hgobj->num_joints);
            }
            if ((layerdat[i].blend_gobjs != (nugobj_s **)0x0) &&
               (iVar3 = 0, hgobj->num_joints != '\0')) {
              iVar4 = 0;
              mtx = mtx_array;
              do {
                if (*(int *)(iVar4 + (int)hgobj->layers[i].blend_gobjs) != 0) {
                  NuMtxMul(&m,mtx,wm);
                  uVar1 = NuRndrGobj(*(nugobj_s **)(iVar4 + (int)hgobj->layers[i].blend_gobjs),&m,
                                     (float **)0x0);
                  uVar2 = uVar2 | uVar1;
                }
                iVar3 = iVar3 + 1;
                mtx = mtx + 1;
                iVar4 = iVar4 + 4;
                layerdat = hgobj->layers;
              } while (iVar3 < (int)(uint)hgobj->num_joints);
            }
            if ((layerdat[i].skin_gobj != (nugobj_s *)0x0) ||
               (layerdat[i].blend_skin_gobj != (nugobj_s *)0x0)) {
              iVar3 = 0;
              if (hgobj->num_joints != '\0') {
                m_00 = mtx_array2HGobjRndrDwa;
                iVar4 = 0;
                mtx = mtx_array;
                do {
                  iVar3 = iVar3 + 1;
                  NuMtxMul(m_00,(numtx_s *)((int)&hgobj->INV_WT->_00 + iVar4),mtx);
                  m_00 = m_00 + 1;
                  iVar4 = iVar4 + 0x40;
                  mtx = mtx + 1;
                } while (iVar3 < (int)(uint)hgobj->num_joints);
              }
              iVar3 = 0;
              if (hgobj->num_joints != '\0') {
                mtx = mtx_array2HGobjRndrDwa;
                do {
                  iVar3 = iVar3 + 1;
                  NuMtxMul(mtx,mtx,wm);
                  mtx = mtx + 1;
                } while (iVar3 < (int)(uint)hgobj->num_joints);
              }
              gobj = hgobj->layers[i].skin_gobj;
              if (gobj != (nugobj_s *)0x0) {
                uVar1 = NuRndrGobjSkin2(gobj,(uint)hgobj->num_joints,mtx_array2HGobjRndrDwa,
                                        (float **)0x0);
                uVar2 = uVar2 | uVar1;
              }
              gobj = hgobj->layers[i].blend_skin_gobj;
              if (gobj != (nugobj_s *)0x0) {
                uVar1 = NuRndrGobjSkin2(gobj,(uint)hgobj->num_joints,mtx_array2HGobjRndrDwa,dwa);
                uVar2 = uVar2 | uVar1;
              }
            }
          }
        }
        else {
          i = (int)layers[j];
          if ((-1 < i) && (i < (int)(uint)hgobj->num_layers)) goto LAB_800b6930;
        }
        j = j + 1;
      } while (j < nlayers);
    }
  }
  return uVar2;
}

int NuHGobjRndrMtx(NUHGOBJ_s *hgobj,numtx_s *wm,int nlayers,short *layers,numtx_s *mtx_array)

{
  int iVar1;
  
  iVar1 = NuHGobjRndrMtxDwa(hgobj,wm,nlayers,layers,mtx_array,(float **)0x0);
  return iVar1;
}


void NuHGobjEvalAnim(NUHGOBJ_s *hgobj,nuAnimData_s *animdata,float time,int njanims,
                    NUJOINTANIM_s *janim,numtx_s *mtx_array)

{
  numtx_s *pnVar1;
  nuanimcurveset_s *animcurveset;
  numtx_s *WT;
  numtx_s *LOCAL_T;
  numtx_s *parent_T;
  NUJOINTDATA_s *jointdata;
  int joint_ix;
  NUJOINTANIM_s *offset;
  uint cindex;
  nuanimdatachunk_s *chunk;
  numtx_s T;
  nuanimtime_s atime;
  uint i;
  
  VtxBuffer[255].z = 1.0;
  VtxBuffer[255].x = 1.0;
  VtxBuffer[255].y = 1.0;
  NuAnimDataCalcTime(animdata,time,&atime);
  chunk = animdata->chunks[atime.chunk];
  if ((njanims != 0) && (i = 0, 0 < njanims)) {
    do {
      cindex = (uint)janim[i].joint_id;
      if ((cindex < hgobj->num_joint_ixs) && (hgobj->joint_ixs[cindex] != 0xff)) {
        janim_array.198[hgobj->joint_ixs[cindex]] = janim + i;
      }
      i = i + 1 & 0xff;
    } while ((int)i < njanims);
  }
  i = 0;
  if (hgobj->num_joints != '\0') {
    do {
      if ((njanims == 0) || (hgobj->joint_ixs[janim->joint_id] != i)) {
        offset = (NUJOINTANIM_s *)0x0;
      }
      else {
        njanims = njanims + -1;
        offset = janim;
        janim = janim + 1;
      }
      animcurveset = chunk->animcurveset[i];
      if (animcurveset == (nuanimcurveset_s *)0x0) {
        LOCAL_T = hgobj->T + i;
      }
      else {
        if (((animcurveset->flags & 0x1aU) == 0) &&
           (jointdata = hgobj->joints + i, (jointdata->flags & 8U) == 0)) {
          NuAnimCurveSetApplyToJointBasic
                    (animcurveset,&atime,jointdata,VtxBuffer + i,
                     VtxBuffer + *(byte *)((int)&jointdata->parent_ix + 3),&T,offset);
        }
        else {
          NuAnimCurveSetApplyToJoint2
                    (animcurveset,&atime,hgobj->joints + i,VtxBuffer + i,
                     VtxBuffer + *(byte *)((int)&hgobj->joints[i].parent_ix + 3),&T,offset);
        }
        LOCAL_T = &T;
      }
      joint_ix = hgobj->joints[i].parent_ix;
      if (joint_ix == -1) {
        joint_ix = 0x30;
        pnVar1 = mtx_array + i;
        do {
          parent_T = LOCAL_T;
          WT = pnVar1;
          joint_ix = joint_ix + -0x18;
          WT->_00 = parent_T->_00;
          WT->_01 = parent_T->_01;
          WT->_02 = parent_T->_02;
          WT->_03 = parent_T->_03;
          WT->_10 = parent_T->_10;
          WT->_11 = parent_T->_11;
          pnVar1 = (numtx_s *)&WT->_12;
          LOCAL_T = (numtx_s *)&parent_T->_12;
        } while (joint_ix != 0);
        WT->_12 = parent_T->_12;
        WT->_13 = parent_T->_13;
        WT->_20 = parent_T->_20;
        WT->_21 = parent_T->_21;
      }
      else {
        NuMtxMul(mtx_array + i,LOCAL_T,mtx_array + joint_ix);
      }
      i = i + 1 & 0xff;
    } while (i < hgobj->num_joints);
  }
  return;
}




//NUJOINTANIM_s* janim_arrayHGobjEval2[256]; global var

void NuHGobjEvalAnim2(NUHGOBJ_s *hgobj,nuanimdata2_s *animdata,float time,int njanims,
                     NUJOINTANIM_s *janim,numtx_s *mtx_array)

{
  numtx_s *pnVar1;
  numtx_s *pnVar2;
  numtx_s *pnVar3;
  numtx_s *pnVar4;
  int cindex;
  NUJOINTDATA_s *jointdata;
  uint jID;
  uint i;
  numtx_s LOCAL_T;
  nuanimtime_s atime;
  uchar curvesetflags;
  
  scale_arrayHGobjEval2[255].z = 1.0;			//nuvec_s scale_arrayHGobjEval2[256];
  scale_arrayHGobjEval2[255].x = 1.0;
  scale_arrayHGobjEval2[255].y = 1.0;
  NuAnimData2CalcTime(animdata,time,&atime);
  if ((njanims != 0) && (i = 0, 0 < njanims)) {
    do {
      jID = (uint)janim[i].joint_id;
      if ((jID < hgobj->num_joint_ixs) && (hgobj->joint_ixs[jID] != 0xff)) {
        janim_arrayHGobjEval2[hgobj->joint_ixs[jID]] = janim + i;
      }
      i = i + 1 & 0xff;
    } while ((int)i < njanims);
  }
  i = 0;
  if (hgobj->num_joints != '\0') {
    do {
      cindex = (int)animdata->ncurves * i;
      curvesetflags = animdata->curvesetflags[i];
      if ((curvesetflags & 0x1a) == 0) {
        jointdata = hgobj->joints + i;
        if ((jointdata->flags & 8U) != 0) goto LAB_800b58ec;
        NuAnimCurve2SetApplyToJointBasic
                  (animdata->curves + cindex,animdata->curveflags + cindex,curvesetflags,&atime,
                   jointdata,scale_arrayHGobjEval2 + i,
                   scale_arrayHGobjEval2 + *(byte *)((int)&jointdata->parent_ix + 3),&LOCAL_T,
                   (NUJOINTANIM_s *)((byte)jointdata->flags & 8));
      }
      else {
LAB_800b58ec:
        NuAnimCurve2SetApplyToJoint
                  (animdata->curves + cindex,animdata->curveflags + cindex,curvesetflags,&atime,
                   hgobj->joints + i,scale_arrayHGobjEval2 + i,
                   scale_arrayHGobjEval2 + *(byte *)((int)&hgobj->joints[i].parent_ix + 3),&LOCAL_ T,
                   (NUJOINTANIM_s *)0x0);
      }
      cindex = hgobj->joints[i].parent_ix;
      if (cindex == -1) {
        cindex = 0x30;
        pnVar1 = mtx_array + i;
        pnVar2 = &LOCAL_T;
        do {
          pnVar4 = pnVar2;
          pnVar3 = pnVar1;
          cindex = cindex + -0x18;
          pnVar3->_00 = pnVar4->_00;
          pnVar3->_01 = pnVar4->_01;
          pnVar3->_02 = pnVar4->_02;
          pnVar3->_03 = pnVar4->_03;
          pnVar3->_10 = pnVar4->_10;
          pnVar3->_11 = pnVar4->_11;
          pnVar1 = (numtx_s *)&pnVar3->_12;
          pnVar2 = (numtx_s *)&pnVar4->_12;
        } while (cindex != 0);
        pnVar3->_12 = pnVar4->_12;
        pnVar3->_13 = pnVar4->_13;
        pnVar3->_20 = pnVar4->_20;
        pnVar3->_21 = pnVar4->_21;
      }
      else {
        NuMtxMul(mtx_array + i,&LOCAL_T,mtx_array + cindex);
      }
      i = i + 1 & 0xff;
    } while (i < hgobj->num_joints);
  }
  return;
}



void NuHGobjEvalAnimBlend
               (NUHGOBJ_s *hgobj,nuAnimData_s *animdata1,float time1,nuAnimData_s *animdata2,
               float time2,float blend,int njanims,NUJOINTANIM_s *janim,numtx_s *mtx_array)

{
  numtx_s *pnVar1;
  nuanimcurveset_s *animcurveset1;
  numtx_s *parent_T;
  numtx_s *T;
  numtx_s *WT;
  nuanimcurveset_s *animcurveset2;
  uint joint_ix;
  int iVar2;
  NUJOINTANIM_s *offset;
  nuanimdatachunk_s *chunk1;
  nuanimdatachunk_s *chunk2;
  uint i;
  double dVar3;
  double dVar4;
  numtx_s LOCAL_T;
  nuanimtime_s atime1;
  nuanimtime_s atime2;
  
  dVar4 = (double)blend;
  dVar3 = (double)time2;
  scale_array0.202[255].z = 1.0;
  scale_array0.202[255].x = 1.0;
  scale_array0.202[255].y = 1.0;
  NuAnimDataCalcTime(animdata1,time1,&atime1);
  chunk1 = animdata1->chunks[atime1.chunk];
  NuAnimDataCalcTime(animdata2,(float)dVar3,&atime2);
  chunk2 = animdata2->chunks[atime2.chunk];
  if ((njanims != 0) && (i = 0, 0 < njanims)) {
    do {
      joint_ix = (uint)janim[i].joint_id;
      if ((joint_ix < hgobj->num_joint_ixs) && (hgobj->joint_ixs[joint_ix] != 0xff)) {
        janim_array.203[hgobj->joint_ixs[joint_ix]] = janim + i;
      }
      i = i + 1 & 0xff;
    } while ((int)i < njanims);
  }
  i = 0;
  if (hgobj->num_joints != '\0') {
    do {
      if ((njanims == 0) || (hgobj->joint_ixs[janim->joint_id] != i)) {
        offset = (NUJOINTANIM_s *)0x0;
      }
      else {
        njanims = njanims + -1;
        offset = janim;
        janim = janim + 1;
      }
      animcurveset1 = chunk1->animcurveset[i];
      if ((animcurveset1 == (nuanimcurveset_s *)0x0) ||
         (animcurveset2 = chunk2->animcurveset[i], animcurveset2 == (nuanimcurveset_s *)0x0)) {
        T = hgobj->T + i;
      }
      else {
        NuAnimCurveSetApplyBlendToJoint2
                  (animcurveset1,&atime1,animcurveset2,&atime2,(float)dVar4,hgobj->joints + i,
                   scale_array0.202 + i,
                   scale_array0.202 + *(byte *)((int)&hgobj->joints[i].parent_ix + 3),&LOCAL_T,
                   offset);
        T = &LOCAL_T;
      }
      joint_ix = hgobj->joints[i].parent_ix;
      if ((joint_ix & 0xff) == 0xff) {
        iVar2 = 0x30;
        pnVar1 = mtx_array + i;
        do {
          WT = T;
          parent_T = pnVar1;
          iVar2 = iVar2 + -0x18;
          parent_T->_00 = WT->_00;
          parent_T->_01 = WT->_01;
          parent_T->_02 = WT->_02;
          parent_T->_03 = WT->_03;
          parent_T->_10 = WT->_10;
          parent_T->_11 = WT->_11;
          pnVar1 = (numtx_s *)&parent_T->_12;
          T = (numtx_s *)&WT->_12;
        } while (iVar2 != 0);
        parent_T->_12 = WT->_12;
        parent_T->_13 = WT->_13;
        parent_T->_20 = WT->_20;
        parent_T->_21 = WT->_21;
      }
      else {
        NuMtxMul(mtx_array + i,T,mtx_array + joint_ix);
      }
      i = i + 1 & 0xff;
    } while (i < hgobj->num_joints);
  }
  return;
}


void NuHGobjEval(NUHGOBJ_s *hgobj,int njanims,NUJOINTANIM_s *janim,numtx_s *mtx_array)

{
  numtx_s *pnVar1;
  numtx_s *pfVar2;
  NUJOINTANIM_s *offset;
  numtx_s *pfVar3;
  numtx_s *pfVar4;
  int iVar5;
  numtx_s *pnVar2;
  uint uVar6;
  numtx_s *dest;
  double dVar7;
  nuangvec_s local_d0;
  numtx_s mtx;
  numtx_s nStack_80;
  nuangvec_s local_38;
  
  uVar6 = 0;
  if (hgobj->num_joints != '\0') {
    dVar7 = 10430.3779296875;
    do {
      if ((njanims == 0) || (hgobj->joint_ixs[janim->joint_id] != uVar6)) {
        offset = (NUJOINTANIM_s *)0x0;
      }
      else {
        njanims = njanims + -1;
        offset = janim;
        janim = janim + 1;
      }
      if (offset == (NUJOINTANIM_s *)0x0) {
        dest = hgobj->T + uVar6;
      }
      else {
        dest = &mtx;
        iVar5 = 0x30;
        pnVar1 = dest;
        pnVar2 = hgobj->T + uVar6;
        do {
          pfVar4 = pnVar2;
          pfVar3 = pnVar1;
          iVar5 = iVar5 + -0x18;
          pfVar3->_00 = pfVar4->_00;
          pfVar3->_01 = pfVar4->_01;
          pfVar3->_02 = pfVar4->_02;
          pfVar3->_03 = pfVar4->_03;
          pfVar3->_10 = pfVar4->_10;
          pfVar3->_11 = pfVar4->_11;
          pnVar1 = (numtx_s *)&pfVar3->_12;
          pnVar2 = (numtx_s *)&pfVar4->_12;
        } while (iVar5 != 0);
        pfVar3->_12 = pfVar4->_12;
        pfVar3->_13 = pfVar4->_13;
        pfVar3->_20 = pfVar4->_20;
        pfVar3->_21 = pfVar4->_21;
        local_d0.x = (int)((double)offset->rx * dVar7);
        local_d0.y = (int)((double)offset->ry * dVar7);
        local_d0.z = (int)((double)offset->rz * dVar7);
        local_38._0_8_ = (longlong)local_d0.z;
        NuMtxSetRotateXYZ(&nStack_80,&local_d0);
        NuMtxTranslate(&nStack_80,(nuvec_s *)&offset->tx);
        NuMtxMul(dest,&nStack_80,dest);
      }
      iVar5 = hgobj->joints[uVar6].parent_ix;
      if (iVar5 == -1) {
        iVar5 = 0x30;
        pnVar1 = mtx_array + uVar6;
        do {
          pnVar2 = pnVar1;
          pfVar2 = dest;
          iVar5 = iVar5 + -0x18;
          pnVar2->_00 = pfVar2->_00;
          pnVar2->_01 = pfVar2->_01;
          pnVar2->_02 = pfVar2->_02;
          pnVar2->_03 = pfVar2->_03;
          pnVar2->_10 = pfVar2->_10;
          pnVar2->_11 = pfVar2->_11;
          dest = (numtx_s *)&pfVar2->_12;
          pnVar1 = (numtx_s *)&pnVar2->_12;
        } while (iVar5 != 0);
        pnVar2->_12 = pfVar2->_12;
        pnVar2->_13 = pfVar2->_13;
        pnVar2->_20 = pfVar2->_20;
        pnVar2->_21 = pfVar2->_21;
      }
      else {
        NuMtxMul(mtx_array + uVar6,dest,mtx_array + iVar5);
      }
      uVar6 = uVar6 + 1 & 0xff;
    } while (uVar6 < hgobj->num_joints);
  }
  return;
}

float ** NuHGobjEvalDwa(int layer,void *bollox,nuAnimData_s *vtxanim,float vtxtime)

{
  nufaceongeom_s *iVar4;
  float *dwa;
  nuanimcurve_s *animcurve;
  nuanimcurveset_s *pnVar1;
  float **DWAptr;
  int iVar2;
  int iVar3;
  int iVar5;
  int iVar6;
  int iVar7;
  double dVar8;
  double dVar9;
  float fVar10;
  nuanimtime_s atimeblendanim;
  
  iVar6 = layer + -1;
  DWAptr = (float **)0x0;
  iVar3 = 0;
  if (vtxanim != (nuAnimData_s *)0x0) {
    NuAnimDataCalcTime(vtxanim,vtxtime,&atimeblendanim);
    DWAptr = NuRndrCreateBlendShapeDWAPointers(1);
    if (((atimeblendanim.chunk != -1) && (DWAptr != (float **)0x0)) &&
       (vtxanim->chunks[atimeblendanim.chunk] != (nuanimdatachunk_s *)0x0)) {
      iVar4 = (nufaceongeom_s *)vtxanim->chunks[atimeblendanim.chunk]->animcurveset[iVar6];
      if (iVar4 != (nufaceongeom_s *)0x0) {
        dwa = NuRndrCreateBlendShapeDeformerWeightsArray((int)*(char *)&iVar4->faceon_type);
        *DWAptr = dwa;
        if (dwa != (float *)0x0) {
          iVar2 = (int)vtxanim->chunks[atimeblendanim.chunk]->animcurveset[iVar6]->ncurves;
          if (0 < iVar2) {
            dVar9 = 1e-08;
            iVar7 = 0;
            dVar8 = 0.0;
            iVar5 = iVar2;
            do {
              *(float *)(iVar7 + (int)dwa) = (float)dVar8;
              pnVar1 = vtxanim->chunks[atimeblendanim.chunk]->animcurveset[iVar6];
              animcurve = *(nuanimcurve_s **)(pnVar1->set->pad + iVar7 + -0xd);
              if (animcurve == (nuanimcurve_s *)0x0) {
                fVar10 = *(float *)(iVar7 + (int)pnVar1->constants);
                *(float *)(iVar7 + (int)dwa) = fVar10;
                if (dVar9 < (double)fVar10) {
                  iVar3 = iVar3 + 1;
                }
              }
              else {
                iVar3 = iVar3 + 1;
                fVar10 = NuAnimCurveCalcVal2(animcurve,&atimeblendanim);
                *(float *)(iVar7 + (int)dwa) = fVar10;
              }
              if (*(float *)(iVar7 + (int)dwa) < 0.0) {
                *(undefined4 *)(iVar7 + (int)dwa) = 0;
              }
              iVar7 = iVar7 + 4;
              iVar5 = iVar5 + -1;
            } while (iVar5 != 0);
          }
          if (maxblend_cnt <= iVar3) {
            maxblend_cnt = iVar3;
            maxblend_cntcnt = iVar2;
          }
        }
      }
    }
  }
  return DWAptr;
}


float ** NuHGobjEvalDwaBlend(int nlayers,short *layers,nuAnimData_s *vtxanim1,float vtxtime1,
                            nuAnimData_s *vtxanim2,float vtxtime2,float blend)

{
  float *pfVar1;
  nuanimcurve_s *pnVar2;
  nuanimcurveset_s *pnVar3;
  float **ppfVar4;
  int iVar5;
  int iVar6;
  int nweights;
  int iVar7;
  int iVar8;
  int iVar9;
  int iVar10;
  byte in_cr0;
  byte in_cr1;
  byte in_cr2;
  byte in_cr3;
  byte bVar11;
  byte unaff_cr4;
  byte in_cr5;
  byte in_cr6;
  byte in_cr7;
  double dVar12;
  double dVar13;
  double dVar14;
  double dVar15;
  float fVar16;
  nuanimtime_s nStack_a0;
  nuanimtime_s anStack_88 [2];
  uint local_58;
  
  dVar13 = (double)blend;
  dVar14 = (double)vtxtime2;
  local_58 = (uint)(in_cr0 & 0xf) << 0x1c | (uint)(in_cr1 & 0xf) << 0x18 |
             (uint)(in_cr2 & 0xf) << 0x14 | (uint)(in_cr3 & 0xf) << 0x10 |
             (uint)(unaff_cr4 & 0xf) << 0xc | (uint)(in_cr5 & 0xf) << 8 | (uint)(in_cr6 & 0xf) <<  4
             | (uint)(in_cr7 & 0xf);
  ppfVar4 = (float **)0x0;
  if (((vtxanim1 != (nuAnimData_s *)0x0) || (vtxanim2 != (nuAnimData_s *)0x0)) && (nlayers != 0))  {
    if (vtxanim1 != (nuAnimData_s *)0x0) {
      NuAnimDataCalcTime(vtxanim1,vtxtime1,&nStack_a0);
    }
    bVar11 = (vtxanim2 == (nuAnimData_s *)0x0) << 1;
    if (vtxanim2 != (nuAnimData_s *)0x0) {
      NuAnimDataCalcTime(vtxanim2,(float)dVar14,anStack_88);
    }
    if (layers == (short *)0x0) {
      nlayers = 1;
    }
    ppfVar4 = NuRndrCreateBlendShapeDWAPointers(nlayers);
    if ((ppfVar4 != (float **)0x0) && (iVar8 = 0, 0 < nlayers)) {
      do {
        if (layers == (short *)0x0) {
          iVar10 = 0;
LAB_800c4018:
          if (((vtxanim1 == (nuAnimData_s *)0x0) ||
              (vtxanim1->chunks[nStack_a0.chunk]->numnodes <= iVar10)) ||
             (pnVar3 = vtxanim1->chunks[nStack_a0.chunk]->animcurveset[iVar10],
             pnVar3 == (nuanimcurveset_s *)0x0)) {
            iVar5 = 0;
          }
          else {
            iVar5 = (int)pnVar3->ncurves;
          }
          if ((((bool)(bVar11 >> 1 & 1)) ||
              (vtxanim2->chunks[anStack_88[0].chunk]->numnodes <= iVar10)) ||
             (pnVar3 = vtxanim2->chunks[anStack_88[0].chunk]->animcurveset[iVar10],
             pnVar3 == (nuanimcurveset_s *)0x0)) {
            iVar6 = 0;
          }
          else {
            iVar6 = (int)pnVar3->ncurves;
          }
          nweights = iVar6;
          if (iVar6 < iVar5) {
            nweights = iVar5;
          }
          pfVar1 = NuRndrCreateBlendShapeDeformerWeightsArray(nweights);
          ppfVar4[iVar8] = pfVar1;
          if ((pfVar1 != (float *)0x0) && (iVar9 = 0, 0 < nweights)) {
            dVar14 = 1.0;
            iVar7 = 0;
            do {
              if (iVar9 < iVar5) {
                pnVar3 = vtxanim1->chunks[nStack_a0.chunk]->animcurveset[iVar10];
                pnVar2 = *(nuanimcurve_s **)(pnVar3->set->pad + iVar7 + -0xd);
                if (pnVar2 == (nuanimcurve_s *)0x0) {
                  dVar15 = (double)*(float *)(iVar7 + (int)pnVar3->constants);
                }
                else {
                  fVar16 = NuAnimCurveCalcVal2(pnVar2,&nStack_a0);
                  dVar15 = (double)fVar16;
                }
              }
              else {
                dVar15 = 0.0;
              }
              if (iVar9 < iVar6) {
                pnVar3 = vtxanim2->chunks[anStack_88[0].chunk]->animcurveset[iVar10];
                pnVar2 = *(nuanimcurve_s **)(pnVar3->set->pad + iVar7 + -0xd);
                if (pnVar2 == (nuanimcurve_s *)0x0) {
                  dVar12 = (double)pnVar3->constants[iVar9];
                }
                else {
                  fVar16 = NuAnimCurveCalcVal2(pnVar2,anStack_88);
                  dVar12 = (double)fVar16;
                }
              }
              else {
                dVar12 = 0.0;
              }
              iVar9 = iVar9 + 1;
              *(float *)(iVar7 + (int)pfVar1) =
                   (float)(dVar12 * dVar13 +
                          (double)(float)(dVar15 * (double)(float)(dVar14 - dVar13)));
              iVar7 = iVar7 + 4;
            } while (iVar9 < nweights);
          }
        }
        else {
          iVar10 = (int)layers[iVar8];
          if (-1 < iVar10) goto LAB_800c4018;
        }
        iVar8 = iVar8 + 1;
      } while (iVar8 < nlayers);
    }
  }
  return ppfVar4;
}



float ** NuHGobjEvalDwa2(int nlayers,short *layers,nuanimdata2_s *vtxanim,float vtxtime)

{
  char *pcVar1;
  float *pfVar2;
  float **ppfVar3;
  int nweights;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  nuanimcurve2_s *animcurve;
  byte in_cr0;
  byte in_cr1;
  byte in_cr2;
  byte in_cr3;
  byte unaff_cr4;
  byte in_cr5;
  byte in_cr6;
  byte in_cr7;
  double dVar8;
  float fVar9;
  nuanimtime_s nStack_60;
  uint local_3c;
  
  local_3c = (uint)(in_cr0 & 0xf) << 0x1c | (uint)(in_cr1 & 0xf) << 0x18 |
             (uint)(in_cr2 & 0xf) << 0x14 | (uint)(in_cr3 & 0xf) << 0x10 |
             (uint)(unaff_cr4 & 0xf) << 0xc | (uint)(in_cr5 & 0xf) << 8 | (uint)(in_cr6 & 0xf) <<  4
             | (uint)(in_cr7 & 0xf);
  ppfVar3 = (float **)0x0;
  if ((vtxanim != (nuanimdata2_s *)0x0) && (nlayers != 0)) {
    NuAnimData2CalcTime(vtxanim,vtxtime,&nStack_60);
    if (layers == (short *)0x0) {
      nlayers = 1;
    }
    ppfVar3 = NuRndrCreateBlendShapeDWAPointers(nlayers);
    if ((ppfVar3 != (float **)0x0) && (iVar5 = 0, 0 < nlayers)) {
      do {
        if (layers == (short *)0x0) {
          iVar7 = 0;
LAB_800c429c:
          nweights = (int)vtxanim->ncurves;
          pfVar2 = NuRndrCreateBlendShapeDeformerWeightsArray(nweights);
          ppfVar3[iVar5] = pfVar2;
          pcVar1 = vtxanim->curveflags;
          iVar7 = vtxanim->ncurves * iVar7;
          animcurve = vtxanim->curves + iVar7;
          if ((pfVar2 != (float *)0x0) && (iVar6 = 0, 0 < nweights)) {
            dVar8 = 0.0;
            iVar4 = 0;
            do {
              fVar9 = NuAnimCurve2CalcVal(animcurve,&nStack_60,(int)pcVar1[iVar6 + iVar7]);
              *(float *)(iVar4 + (int)pfVar2) = fVar9;
              if ((double)fVar9 < dVar8) {
                *(float *)(iVar4 + (int)pfVar2) = (float)dVar8;
              }
              iVar6 = iVar6 + 1;
              animcurve = animcurve + 1;
              iVar4 = iVar4 + 4;
            } while (iVar6 < nweights);
          }
        }
        else {
          iVar7 = (int)layers[iVar5];
          if (-1 < iVar7) {
            if (iVar7 < vtxanim->nnodes) goto LAB_800c429c;
            pfVar2 = NuRndrCreateBlendShapeDeformerWeightsArray(0);
            ppfVar3[iVar5] = pfVar2;
          }
        }
        iVar5 = iVar5 + 1;
      } while (iVar5 < nlayers);
    }
  }
  return ppfVar3;
}
