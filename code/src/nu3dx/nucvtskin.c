#include "nucvtskin.h"


struct primdef_s* primdefs_sorted;
struct primdef_s* primdefs;
s32 stritot;
s32 totaldupes;
s32 totalpts;
s32 tritot;
s32 stats[15];
static char mtxused[256];

void InitSkinning(s32 buffsize) //makes primitive

{
  primdefs = (struct primdef_s *)NuMemAlloc(0xc7ce0); // 0xC7CE0 bytes can fit 0x3548 NuPrims perfectly
  primdefs_sorted = (struct primdef_s *)NuMemAlloc(0xc7ce0);
  return;
}


void CloseSkinning(void)

{
  NuMemFree(primdefs);
  NuMemFree(primdefs_sorted);
  primdefs_sorted = NULL;
  primdefs = NULL;
  return;
}


void NuPs2CreateSkin(struct nugobj_s* gobj) {
    struct nugeom_s* tmp;

    tmp = gobj->geom;
    if ((tmp != NULL) && (tmp->prim->type == NUPT_NDXTRI)) {
        NuPs2CreateSkinNorm(gobj);
    }
return;
}


/********************************WIP*************************************************/

void CreateSkinGeom(struct nugeom_s* ge, struct primdef_s* pd, s32 pdcnt) {

    u32 amount_per_range_arr[300];    //primsize
    s32 uniq_baseid_ranges;
    s32 i;
    s32 j;
    s32 k;
    s32 vtx_k;
    u16 j1;
    s32 cur_baseid;
    u32* amount_per_range;
    struct nuprim_s* new_prim_list;    //nextprim
    struct nuprim_s* cur_prim;
    struct nuvtx_sk3tc1_s* vertex_buffer;
    struct primdef_s* cur_pd;
    float const* cur_vert_weights;
    s32 weight_idx;
    s32 nonzero_weight_count;
    struct nuprim_s* old_prims;



    if (ge->vtxtype != NUVT_TC1) {
        NuErrorProlog("C:/source/crashwoc/code/nu3dx/nucvtskn.c",0x69)("CreateSkinGeom : Unknown vertex type!");
    }

  memset(amount_per_range_arr, 0, sizeof(amount_per_range_arr));
  uniq_baseid_ranges = 1;
  for (i = 0, cur_baseid = pd[0].baseid; i < pdcnt; ++i) {
    if (pd[i].baseid != cur_baseid) {
      cur_baseid = pd[i].baseid;
      ++uniq_baseid_ranges;
    }
    amount_per_range_arr[uniq_baseid_ranges] += 3;
  }

  // Since the first element is never referenced, it's easier to read if working from index 1
  amount_per_range = amount_per_range_arr + 1;
  new_prim_list = NuPrimCreate(amount_per_range[0], NUPT_NDXTRI);
  cur_prim = new_prim_list; // r28
  for (i = 1; i < uniq_baseid_ranges; i++) {
    cur_prim->next = NuPrimCreate(amount_per_range[i], NUPT_NDXTRI);
    cur_prim = cur_prim->next;
  }

  vertex_buffer = (struct nuvtx_sk3tc1_s*)GS_CreateBuffer(ge->vtxcnt * sizeof(struct nuvtx_sk3tc1_s), 1);

  cur_pd = pd; // bound to r4
  cur_prim = new_prim_list; // bound to r27
  for (i = 0; i < uniq_baseid_ranges; i++) {
    for (j = 0; j < 15; j++) {
      cur_prim->skinmtxlookup[j] = cur_pd->mtxid[j];
    }

    // j ~= r10, r18
    for (j1 = 0; j1 < cur_prim->vertexCount; j1 += 3) {
      ((u16*)cur_prim->idxbuff)[j1] = cur_pd->vid[0];
      ((u16*)cur_prim->idxbuff)[j1 + 1] = cur_pd->vid[1];
      ((u16*)cur_prim->idxbuff)[j1 + 2] = cur_pd->vid[2];

      // k ~= r10, r24
      for (k = 0; k < 3; k++) {
        s32 vtx_k = cur_pd->vid[k];
        vertex_buffer[vtx_k].pnt.x = cur_pd->vrts[k].pnt.x;
        vertex_buffer[vtx_k].pnt.y = cur_pd->vrts[k].pnt.y;
        vertex_buffer[vtx_k].pnt.z = cur_pd->vrts[k].pnt.z;
        vertex_buffer[vtx_k].nrm.x = cur_pd->vrts[k].nrm.x;
        vertex_buffer[vtx_k].nrm.y = cur_pd->vrts[k].nrm.y;
        vertex_buffer[vtx_k].nrm.z = cur_pd->vrts[k].nrm.z;
        vertex_buffer[vtx_k].tc[0] = cur_pd->vrts[k].tc[0];
        vertex_buffer[vtx_k].tc[1] = cur_pd->vrts[k].tc[1];
        vertex_buffer[vtx_k].diffuse = cur_pd->vrts[k].diffuse;
        memset(vertex_buffer[vtx_k].weights, 0, sizeof(float[2]));
        memset(vertex_buffer[vtx_k].indexes, 0, sizeof(float[3]));

        // TODO: Likely need to flip weights around to be [3][15]
        cur_vert_weights = (float const*)cur_pd->weights + j * 15;
        weight_idx = 0;
        nonzero_weight_count = 0;
        while (weight_idx < 15 && nonzero_weight_count < 3) {
          if (cur_vert_weights[weight_idx] != 0.0f) {
            vertex_buffer[vtx_k].indexes[nonzero_weight_count] = (float)weight_idx;
            if (nonzero_weight_count > 0) {
              vertex_buffer[vtx_k].weights[nonzero_weight_count - 1] = cur_vert_weights[weight_idx];
            }
            ++nonzero_weight_count;
          }
          ++weight_idx;
        }
      }
    }
  }

  GS_DeleteBuffer((u8*)ge->hVB);
  old_prims = ge->prim;
  while (old_prims != NULL) {
    struct NuPrim* old_prim_next = old_prims->next;
    NuPrimDestroy(old_prims);
    old_prims = old_prim_next;
  }

  ge->hVB = vertex_buffer;
  ge->vtxtype = NUVT_SK3TC1;
  ge->prim = new_prim_list;
}


//94.78%
// This function creates a skinned geometry. It does this by creating an array of
// primitives, setting vertex skin data, sorting the primitives and then creating a
// new geometry from the sorted primitive array. A better name for this function
void NuPs2CreateSkinNorm(struct NuGobj *gobj) {
    struct NuGeom* currGeom;
    struct NuPrim *p;
    struct nuvtx_tc1_s *srcvb;
    struct primdef_s *pd;
    u16 *vid;
    s32 pdcnt;
    s32 skinix;
    s32 totmtx;
    s32 ptot; 

    s32 iVar2;
    s32 iVar3;

    memset(stats, NULL, sizeof(stats));


    totaldupes = 0;
    totalpts = 0;
    tritot = 0;
    stritot = 0;

    for (currGeom = gobj->geom; currGeom != 0; currGeom = currGeom->next)
    {
        if (currGeom->skins == NULL && !currGeom->vtxskininfo) {
            continue;
        }
        pd = primdefs;
        p = currGeom->prims;
        pdcnt = 0;
        vid = (u16*)p->idxbuff;
        srcvb = currGeom->hVB;
        // Maximum amount of faces is 2200 (0x898)
        if ((p->cnt / 3) > 2200)
        {
            NuErrorProlog("C:/source/crashwoc/code/nu3dx/nucvtskn.c", 0x35e)("NuPs2CreateSkinNorm: TOO MANY PRIMS!");
        }
        
        for (skinix = 0; skinix < p->cnt; skinix += 3)
        {
            memset(pd, 0, sizeof(struct primdef_s));
            memset(pd->mtxid, -1, 0x3c);
            
            if (currGeom->vtxskininfo)
            {
                SetVtxSkinData2(pd, 0, srcvb, (s32)vid[0], currGeom);
                SetVtxSkinData2(pd, 1, srcvb, (s32)vid[1], currGeom);
                SetVtxSkinData2(pd, 2, srcvb, (s32)vid[2], currGeom);
            }
            else
            {
                SetVtxSkinData(pd, 0, srcvb, (s32)vid[0], currGeom);
                SetVtxSkinData(pd, 1, srcvb, (s32)vid[1], currGeom);
                SetVtxSkinData(pd, 2, srcvb, (s32)vid[2], currGeom);
            }
            
            if (((pd->vid[0] == pd->vid[1]) || (pd->vid[1] == pd->vid[2])) || (pd->vid[0] == pd->vid[2]))
            {
                pd -= 1;
                pdcnt -= 1;
            }
            totmtx = pd->nummtx;
            pd++;
            pdcnt++;
            vid += 3; //vid += 6;
            stats[totmtx]++;
        }
        SortPrimdefs(primdefs,pdcnt);
        iVar3 = 0;
        iVar2 = 0;
        for(skinix = 0; skinix < 15; skinix++) {
            if ((stats[skinix] != 0) && (skinix > iVar2)) {
                iVar2 = skinix;
            }
            if (stats[skinix] >= stats[iVar3]) {
                iVar3 = skinix;
            }
        }
        CreateSkinGeom(currGeom,primdefs,pdcnt);
    }
    
	return;
}


//MATCH GCN
static s32 FillFreeMatrixSlots(struct primdef_s* pd, s32 cnt, s32 start) {
    struct primdef_s* primdef_batch; //
    int numbatchmtx; //
    int n; //
    int m; //
    int maxmtxperprim; // 

    maxmtxperprim = 0xC;
    
    primdef_batch = &pd[start];
    numbatchmtx = primdef_batch->nummtx;
    
    if (start >= cnt) {
        return numbatchmtx;
    }
    
    if (numbatchmtx >= maxmtxperprim) {
        return numbatchmtx;
    }
    
    for(n = start + 1; n < cnt; n++) {
        if (pd[n].sorted != 1) {
            for(m = 0; m < maxmtxperprim && pd[n].mtxid[m] != -1; m++) {
                if (mtxused[pd[n].mtxid[m]] == 0) {
                    primdef_batch->mtxid[numbatchmtx] = pd[n].mtxid[m];
                    mtxused[pd[n].mtxid[m]] = 1;
                    
                    if (numbatchmtx++ == 11) {
                        return maxmtxperprim;
                    }
                }
            }
        }
    }
    return numbatchmtx;
}


//67%
s32 SortPrimdefs(struct primdef_s* pd, s32 count) {
    s32 scnt; //
    s32 tot; // 
    s32 baseid; // 
    s32 totsmtx; // 
    s32 totusmtx; // 
    s32 n; // 
    s32 m; // 
    s32 o; // 
    s32 p; // 
    s32 s; // 
    s32 t; // 
    struct primdef_s tpd; //
    s32 batchmatrices[200][15]; //
    s32 batchmatrices_sorted[200][15]; // 
    s32 batchcount; // 
    s32 matrixusecount[150]; // 
    s32 matrixindex; // 
    s32 matrixcount; // 
    s32 maxmatrix; // 
    struct matchingslot_s matchingslot[200]; //
    s32 nummatches; // 
    s32 freeslotcount; // 
    s32 matrixslot; // 
    s32 batchnum; // 
    float totalweights11; // 
    float totalweights12; // 
    float totalweights13; // 
    s32 totalmtx; // 
    s32 i; // 
    s32 j;

    //temp
    s32 l;
    s32 var_r8_4;
    s32 iVar6;
    s32 iVar9;
    s32 iVar16;
    s32 iVar17;
    s32 iVar21;
    s32 iVar22;
    s32 iVar23;
    s32 bID;
    s32 k;
    //    

    bID = 0;
    // 'Tis but a bubble sort
    for (i = 0; i < count; i++) {
        for (j = i + 1; j < count; j++) {
            if (pd[j].nummtx > pd[i].nummtx) {
                tpd = pd[i];
                pd[i] = pd[j];
                pd[j] = tpd;
            }
        }
    }

    m = 0;
    for (k = 0; k < count; k++) {
        if (pd[k].sorted == 0) {
            bID++;
            memset(&mtxused, 0, sizeof(mtxused));
            
            for (n = 0; n < pd[k].nummtx; n++) {
                mtxused[pd[k].mtxid[n]] = 1;
            }
            
            pd[k].nummtx = FillFreeMatrixSlots(pd, count, k);
            
            pd[k].baseid = m;
            memcpy(&primdefs_sorted[m], &pd[k], sizeof (struct primdef_s));
            
            m++;
            
            for (j = k + 1; j < count; j++) {
                if ((pd[j].sorted == 0) && (pd[k].nummtx >= pd[j].nummtx)) {
                    iVar6 = 0;
                    for (i = 0; i < pd[j].nummtx; i++) {
                        matrixslot = (mtxused[pd[j].mtxid[i]] - 1) >> 0x1f;
                        iVar6 += ((matrixslot ^ (s32)mtxused[pd[j].mtxid[i]] - 1) - matrixslot);
                    }
                    
                    if (iVar6 == 0) {
                        pd[j].baseid = m;
                        memcpy(&primdefs_sorted[m], &pd[j], sizeof (struct primdef_s));

                        
                        pd[j].sorted = 1;
                        
                        for (n = 0; n < 0xF; n++) {
                            primdefs_sorted[m].mtxid[n] = -1;
                            primdefs_sorted[m].weights[0][n] = 0.0f;
                            primdefs_sorted[m].weights[1][n] = 0.0f;
                            primdefs_sorted[m].weights[2][n] = 0.0f;
                        }
                        
                        for (n = 0; n < pd[k].nummtx; n++) {
                            for (iVar6 = 0; iVar6 < pd[j].nummtx; iVar6++) {
                                if (pd[k].mtxid[n] == pd[j].mtxid[iVar6]) {
                                    primdefs_sorted[m].weights[0][n] = pd[j].weights[0][iVar6];
                                    primdefs_sorted[m].weights[1][n] = pd[j].weights[1][iVar6];
                                    primdefs_sorted[m].weights[2][n] = pd[j].weights[2][iVar6];
                                    primdefs_sorted[m].mtxid[n] = pd[k].mtxid[n];
                                }
                            }
                        }
                        m++;
                    }
                }
            }
            
            for (t = 0; t < 15; t++) {
                batchmatrices[bID][t] = primdefs_sorted[bID].mtxid[t];
            }
            bID++;
        }
    }
    
    memset(matrixusecount, 0, sizeof(matrixusecount));
    i = -1;
    for (iVar17 = 0; iVar17 < bID; iVar17++) {
        for (iVar21 = 0; iVar21 < 0xF; iVar21++) {
            iVar9 = batchmatrices[iVar17][iVar21];
            if (iVar9 != -1) {
                matrixusecount[iVar9]++;
                iVar9 = batchmatrices[iVar17][iVar21];
                if (iVar9 < i) {
                    iVar9 = i;
                }
                i = iVar9;
            }
        }
    }
    
    memset(batchmatrices_sorted, -1, sizeof(batchmatrices_sorted));
    while (1) {
        iVar17 = 0;
        var_r8_4 = matrixusecount[0];
        for (iVar23 = 0; iVar23 <= i; iVar23++) {
            if (matrixusecount[iVar23] < matrixusecount[iVar17]) {
                var_r8_4 = matrixusecount[iVar23];
                iVar17 = iVar23;
            }
        }

        if (var_r8_4 == 0) break;
        
        iVar23 = 0;
        iVar9 = 0;
        for (iVar21 = 0; iVar21 < bID; iVar21++) {
            for (j = 0; j < 0xF; j++) {
                if (batchmatrices[iVar21][j] == iVar17) {
                    matchingslot[iVar9].batch = iVar21;
                    matchingslot[iVar9].slot = j;
                    iVar23++;
                    iVar9++;
                }
            }
        }
        
        iVar21 = iVar9;
        if (iVar23 > 0) {
            for (iVar9 = 0; iVar9 < 0xf; iVar9++) {
                iVar16 = 0;
                iVar9 = iVar21 + 1;
                if (0 < iVar23) {
                    iVar22 = iVar23;
                    do {
                        batchcount = &matchingslot->batch;
                        if ((*batchmatrices_sorted)[batchcount * 0xf + iVar21] == -1) {
                            iVar16 = iVar16 + 1;
                        }
                        iVar22 = iVar22 + -1;
                    } while (iVar22 != 0);
                }
                if (iVar23 <= iVar16) goto LAB_800ae5b0;
                iVar21 = iVar9;
            }
            NuErrorProlog("C:/source/crashwoc/code/nu3dx/nucvtskn.c", 0x232)("SortPrimDefs: Unable to find a matching free slot in all batches!!");
        }
        else {
            LAB_800ae5b0:
            if (0 < iVar23) {
                do {
                    batchcount = &matchingslot->batch;
                    iVar23 = iVar23 + -1;
                    batchmatrices_sorted[-1][batchcount * 0xf + iVar9 + 199] = iVar17;
                    matrixusecount[iVar17] = 0;
                } while (iVar23 != 0);
            }
        }
    }
    
    bID = 0;
    iVar17 = 0;
    for (iVar23 = 0; iVar23 < count; iVar23++) {
        if (primdefs_sorted[i].baseid != bID) {
            bID = primdefs_sorted[i].baseid;
            iVar17++;
        }
        
        memcpy(&pd[iVar23], &primdefs_sorted[iVar23], sizeof (struct primdef_s));
        
        for (k = 0; k < 0xF; k++) {
            pd[iVar23].mtxid[k] = -1;
            pd[iVar23].weights[0][k] = 0.0f;
            pd[iVar23].weights[1][k] = 0.0f;
            pd[iVar23].weights[2][k] = 0.0f;
        }
        
        pd[iVar23].nummtx = primdefs_sorted[bID].nummtx;
        pd[iVar23].baseid = bID;
        
        for (k = 0; k < 0xF; k++) {
            for (l = 0; l < 0xF; l++) {
                if (batchmatrices_sorted[iVar17][iVar23] == primdefs_sorted[iVar23].mtxid[l]) {
                    pd[iVar23].weights[0][k] = primdefs_sorted[iVar23].weights[0][l];
                    pd[iVar23].weights[1][k] = primdefs_sorted[iVar23].weights[1][l];
                    pd[iVar23].weights[2][k] = primdefs_sorted[iVar23].weights[2][l];
                    pd[iVar23].mtxid[k] = primdefs_sorted[iVar23].mtxid[l];
                }
            }
        }
    }
    
    i = 0;
    bID = -1;
    for (k = 0; k < n; k++) {
        if (primdefs_sorted[k].baseid != bID) {
            i += primdefs_sorted[k].nummtx;
            bID = primdefs_sorted[k].baseid;
        }
    }
    return i;
}


//MATCH GCN
static s32 AddMtxToPrimDef(struct primdef_s *primdef,s32 mtxid)
{
  s32 n;
  s32 i;


     for(i = 0; i < primdef->nummtx; i++) {
          
          n = primdef->mtxid[i];
        if (n == mtxid) {
            return i;
        }
    }
    if (i >= 15) {
        return 0;
    }
    primdef->mtxid[i] = mtxid;
    primdef->nummtx++;
    return i;
}


//NGC MATCH
static void SetVtxSkinData (struct primdef_s * pd, s32 pdix, struct nuvtx_tc1_s * vb, s32 vid, struct NuGeom * ge)
{
  s32 n;
  s32 offset;
  float *weights;
  s32 ix;
  struct NuSkin *sk;

      //memcpy(&vb[vid], &pd->vrts[pdix], sizeof (struct nuvtx_tc1_s));
      pd->vrts[pdix] = vb[vid];
      pd->vid[pdix] = vid;
    
      if (pdix < 3) {
        for (sk = ge->skins; sk != NULL; sk = sk->next) {
          offset = vid - sk->vtxoffset;
          if ((-1 < offset) && (offset < sk->vtxcnt)) {
            weights = &sk->weights[sk->mtxcnt * offset];
            for (ix = 0, n = 0; ix < sk->mtxcnt; ix++)
            {
              n = AddMtxToPrimDef(pd,sk->mtxid[ix]);
              pd->weights[pdix * 5][n] = weights[ix];
            }
            return;
          }
        }
      }
      return;
}

//86%
static void SetVtxSkinData2(struct primdef_s *pd,int pdix, struct nuvtx_tc1_s *vertexbuf,int vid,struct NuGeom *currgeom)
{
    struct NUVTXSKININFO_s *skinfo;
    int i;
    int ix;
    
    pd->vrts[pdix] = vertexbuf[vid];
    pd->vid[pdix] = vid;
    skinfo = &currgeom->vtxskininfo[vid];
    // ix = 0;
       
    for(i = ix = 0; i != 3 && skinfo->wts[ix] != 0.0f; i++) {
        ix++;
        pd->weights[pdix][AddMtxToPrimDef(pd,skinfo->joint_ixs[i])] = skinfo->wts[ix];
    }

    return;
}
