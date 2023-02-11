#include "nuscene.h"
#include"nu3dxtypes.h"
#include"nugobj.h"
#include"types.h"

// Since this project isn't trying to match the original code,
// I figured it would be nice to try and have the errors show the correct file/line,
// rather than manually entering it
#define NuError(msg) \
    error_func e = NuErrorProlog(__FILE__, __LINE__); \
    e(msg);


void ReadNuIFFTextureSet(filehandle handle,nuscene_s *scene)

{
  int count;
  short *tids;
  int imsize;
  int numtids;
  nutex_s nuTex;
  
  NuFileBeginBlkRead(handle,0HST);
  count = NuFileReadInt(handle);
  NuFileEndBlkRead(handle);
  scene->numtids = 0;
  if (count == 0) {
    scene->tids = (short *)0x0;
  }
  else {
    tids = (short *)NuMemAlloc(count << 1);
    scene->tids = tids;
    *tids = 0;
  }
  if (0 < count) {
    do {
      NuFileBeginBlkRead(handle,0MXT);
      nuTex.type = NuFileReadInt(handle);
      nuTex.width = NuFileReadInt(handle);
      nuTex.height = NuFileReadInt(handle);
      imsize = NuFileReadInt(handle);
      nuTex.pal = (int *)0x0;
      nuTex.bits = (void *)0x0;
      nuTex.decal = 0;
      nuTex.mmcnt = imsize;
      if ((nuTex.type & 0x80) == 0) {
        imsize = NuTexImgSize(nuTex.type,nuTex.width,nuTex.height);
      }
      nuTex.bits = malloc_x(imsize);
      NuFileRead(handle,nuTex.bits,imsize);
      imsize = NuTexPalSize(nuTex.type);
      if (imsize != 0) {
        nuTex.pal = (int *)malloc_x(imsize);
        NuFileRead(handle,nuTex.pal,imsize);
      }
      imsize = NuTexCreate(&nuTex);
      numtids = scene->numtids;
      scene->tids[numtids] = (short)imsize;
      scene->numtids = numtids + 1;
      if (nuTex.pal != (int *)0x0) {
        free_x(nuTex.pal);
        nuTex.pal = (int *)0x0;
      }
      if (nuTex.bits != (void *)0x0) {
        free_x(nuTex.bits);
        nuTex.bits = (void *)0x0;
      }
      NuFileEndBlkRead(handle);
      count = count + -1;
    } while (count != 0);
  }
  return;
}

void ReadNuIFFMaterialSet(filehandle fh,nuscene_s *sc)

{
  numtlattrib_s attr;
  code *er;
  int nmtl;
  numtl_s **mtl;
  numtl_s *mtlR;
  numtl_s *nextmtl;
  int i;
  int j;
  
  i = 0;
  //er = NuDebugMsgProlog("C:/source/crashwoc/code/nu3dx/nuscene.c",0xe6);
  //(*er)("Reading IFF Material set...");
  nmtl = NuFileReadInt(fh);
  mtl = (numtl_s **)NuMemAlloc(nmtl << 2);
  sc->nummtls = nmtl;
  sc->mtls = mtl;
  if (0 < nmtl) {
    do {
      mtlR = (numtl_s *)NuMtlRead(fh);
      j = i + 1;
      sc->mtls[i] = mtlR;
      attr = sc->mtls[i]->attrib;
      if ((uint)attr >> 0x1e != 0) {
        sc->mtls[i]->attrib = (numtlattrib_s)((uint)attr & 0xfff3ffff | 0x40000);	//sc->mtls[i]->attrib).aref = 0xffffffff
      }
      i = j;
    } while (j < nmtl);
  }
  i = 0;
  if (0 < nmtl) {
    do {
      mtlR = sc->mtls[i];
      if ((int)mtlR->next < 1) {
        mtlR->next = (numtl_s *)0x0;
      }
      else {
        nextmtl = sc->mtls[(int)((int)&mtlR->next[-1].sv + 3)];
        mtlR->next = nextmtl;
        sc->mtls[i]->fxid = nextmtl->fxid;
      }
      i = i + 1;
      nmtl = nmtl + -1;
    } while (nmtl != 0);
  }
  return;
}


void NuSceneMtlUpdate(nuscene_s *nus)

{
  uchar decal;
  numtl_s *mtl;
  int i;
  int n;
  
  i = 0;
  if (0 < nus->nummtls) {
    do {
      mtl = nus->mtls[i];
      if (mtl->tid == -1) {
        mtl->tid = 0;
        nus->mtls[i]->L = '\0';
      }
      else {
        mtl->tid = (int)nus->tids[mtl->tid];
        mtl = nus->mtls[i];
        decal = NuTexGetDecalInfo(mtl->tid);
        mtl->L = decal;
      }
      n = i + 1;
      NuMtlUpdate((char)nus->mtls[i]);
      i = n;
    } while (n < nus->nummtls);
  }
  return;
}




void ReadNuIFFAnimationLibrary(fileHandle handle, struct nugscn_s* scene)
{
    u8 i;

    u8 numAnim = NuFileReadChar(handle);
    scene->numinstanims = numAnim;

    if (numAnim > 0)
    {
        scene->instanimdata = NuMemAlloc(numAnim * sizeof(struct nuanimdata_s**));
        memset(scene->instanimdata, 0, numAnim * sizeof(struct nuanimdata_s**));

        for (i = 0; i < numAnim; i++)
        {
            if (NuFileReadChar(handle) != 0)
            {
                scene->instanimdata[i] = NuAnimDataRead(handle);
            }
        }
    }
}

void ReadNuIFFGSplineSet(filehandle fh,nugscn_s *gsc)

{
  short *psVar1;
  int index;
  void *spline;
  nugspline_s *splineDataIndex;
  int iVar2;
  
  index = NuFileReadInt(fh);
  gsc->numsplines = index;
  index = NuFileReadInt(fh);
  if (gsc->numsplines != 0) {
    spline = NuMemAlloc(index);
    gsc->splinedata = spline;
    NuFileRead(fh,spline,index);
    splineDataIndex = (nugspline_s *)NuMemAlloc(gsc->numsplines * 0xc);
    index = 0;
    gsc->splines = splineDataIndex;
    if (0 < gsc->numsplines) {
      iVar2 = 0;
      do {
        index = index + 1;
        *(undefined2 *)((int)&gsc->splines->len + iVar2) = *(undefined2 *)((int)spline + 2);
        *(char **)((int)&gsc->splines->name + iVar2) = gsc->nametable + *(short *)((int)spline + 6 );
        *(int *)((int)&gsc->splines->pts + iVar2) = (int)spline + 8;
        *(undefined2 *)((int)&gsc->splines->ptsize + iVar2) = 0xc;
        psVar1 = (short *)((int)&gsc->splines->len + iVar2);
        iVar2 = iVar2 + 0xc;
        spline = (void *)((int)spline + 8 + *psVar1 * 0xc);
      } while (index < gsc->numsplines);
    }
  }
  return;
}


void ReadNuIFFGobjSet(fileHandle handle, struct nuscene_s* nus)
{
    s32 numGobj = NuFileReadInt(handle);

    if (nus->gobjs != NULL)
    {
        NuError("ReadNuIFFGobjSet : Object already defined!");
    }

    nus->gobjs = NuMemAlloc(numGobj * sizeof(struct nugobj_s*));
    memset(nus->gobjs, 0, numGobj * sizeof(struct nugobj_s*));
    nus->numgobjs = 0;

    while (numGobj-- > 0)
    {
        ReadNuIFFGeomDef(handle, nus);
    }
}

struct nugobj_s * ReadNuIFFGeom(filehandle handle,numtl_s **mtls)

{
  int ngobjs;
  nugobj_s *gobject;
  gobjtype_s type;
  int mtlid;
  nufaceongeom_s *face;
  nugobj_s *first;
  nugobj_s *last;
  nugeom_s *geom;
  int i;
  int count;
  nuvec_s zero;
  
  memset(&zero,0,0xc);
  i = 0;
  last = NULL;
  ngobjs = NuFileReadInt(handle);
  if (0 < ngobjs) {
    do {
      gobject = NuGobjCreate();
      gobject->ignore = 0;
      if (i == 0) {
        gobject->ngobjs = ngobjs;
        first = gobject;
      }
      else {
        gobject->ngobjs = 0;
      }
      type = NuFileReadInt(handle);
      gobject->type = type;
      if (type == NUGOBJ_MESH) {
        NuFileRead(handle,&gobject->origin,0xc);
        count = NuFileReadInt(handle);
        if (0 < count) {
          do {
            geom = NuGeomCreate();
            mtlid = NuFileReadInt(handle);
            geom->mtl_id = mtlid;
            geom->mtls = mtls[mtlid];
            ReadNuIFFGeomVtx(handle,geom);
            ReadNuIFFGeomCntrl(handle);
            ReadNuIFFGeomPrim(handle,geom);
            ReadNuIFFGeomSkin(handle,geom);
            ReadNuIFFBlendShape(handle,geom);
            NuGobjAddGeom(gobject,geom);
            count = count + -1;
          } while (count != 0);
        }
      }
      else if ((type == NUGOBJ_FACEON) && (count = NuFileReadInt(handle), 0 < count)) {
        do {
          (gobject->origin).x = zero.x;
          (gobject->origin).y = zero.y;
          (gobject->origin).z = zero.z;
          face = NuFaceOnGeomCreate();
          mtlid = NuFileReadInt(handle);
          face->mtl_id = mtlid;
          face->mtl = mtls[mtlid];
          ReadNuIFFFaceOnGeom(handle,face);
          NuGobjAddFaceOnGeom(gobject,face);
          count = count + -1;
        } while (count != 0);
      }
      i = i + 1;
      NuPs2CreateSkin(gobject);
      NuGobjCalcDims(gobject);
      for (geom = gobject->geom; geom != (nugeom_s *)0x0; geom = geom->next) {
      }
      if (last != (nugobj_s *)0x0) {
        last->next_gobj = gobject;
      }
      last = gobject;
    } while (i < ngobjs);
  }
  return first;
}


void ReadNuIFFGeomDef(fileHandle handle, struct nuscene_s* nus)
{
    struct nugobj_s* gobj;
    s32 numgobjs;
    
    gobj = ReadNuIFFGeom(handle, &nus->mtls);
    numgobjs = nus->numgobjs;
    nus->gobjs[numgobjs] = gobj;
    nus->numgobjs = numgobjs + 1;
    return;
}

void ReadNuIFFGeomVtx(fileHandle handle, struct nugeom_s* geom)
{

    s32 vtxCount = NuFileReadInt(handle);
    geom->vtxcnt = vtxCount;
    
    if (vtxCount != 0)
    {
        NuGeomCreateVB(geom, vtxCount, NUVT_TC1, 0);
        
        if (geom->vb == NULL)
        {
            NuError("ReadNuIFFGeomVtx : Lock VB failed!");
        }
        
        NuFileRead(handle, geom->hVB, vtxCount * 0x24);
        GS_ARGBTORGBA(geom->hVB, vtxCount);
    }
}

void ReadNuIFFGeomCntrl(fileHandle handle, nugeom_s* geom)
{
    // Removed code???
    NuFileReadInt(handle);
    return;
}

void ReadNuIFFFaceOnGeom(filehandle handle,struct nufaceongeom_s *face)

{
  s32 nfaceons;
  struct nufaceontype_s type;
  struct nufaceon_s *faceon;
  f32 fVar1;
  
  nfaceons = NuFileReadInt(handle);
  face->nfaceons = nfaceons;
  type = NuFileReadInt(handle);
  face->faceon_type = type;
  fVar1 = NuFileReadFloat(handle);
  face->faceon_radius = fVar1;
  nfaceons = face->nfaceons * 0x18;
  faceon = (nufaceon_s *)NuMemAlloc(nfaceons);
  face->faceons = faceon;
  NuFileRead(handle,faceon,nfaceons);
  return;
}

void ReadNuIFFGeomPrim(fileHandle handle, struct nugeom_s* geom)

{
    int counter;
    enum nuprimtype_e type;

    counter = NuFileReadInt(handle);
    if ((counter != 0) && (0 < counter)) {
        do {
            type = NuFileReadInt(handle);
            if (type == NUPT_NDXTRI) {
                ReadNuIFFGeomTri(handle, geom, NUPT_NDXTRI);
            }
            else if (type == NUPT_NDXTRISTRIP) {
                ReadNuIFFGeomTri(handle, geom, NUPT_NDXTRISTRIP);
            }
            else {
                e = NuErrorProlog("C:/source/crashwoc/code/nu3dx/nuscene.c", 0x2a7);
                (*e)("Unknown primitive type");
            }
            counter = counter + -1;
        } while (counter != 0);
    }
    return;
}

void ReadNuIFFGeomTri(fileHandle handle, struct nugeom_s* geom, enum nuprimtype_e type)
{
    struct nuprim_s *prim;
    s32 itemCount;

    itemCount = NuFileReadInt(handle);
    prim = NuPrimCreate(itemCount, type);
    NuFileRead(handle, (void*)prim->idxbuff, itemCount * 2);
    NuGeomAddPrim(geom, prim);
}

void ReadNuIFFGeomSkin(filehandle handle,nugeom_s *geom)

{
  int count;
  char index;
  struct NUVTXSKININFO_s *vtxskin_info;
  struct NuSkin *skin;
  int byteIndex;
  int *mtxid;
  int iVar1;
  float *data;
  int indx_;
  int cycleIndex;
  
  count = NuFileReadInt(handle);
  if (count != 0) {
    index = NuFileReadChar(handle);
    if (index == '\0') {
      indx_ = 0;
      if (0 < count) {
        do {
          cycleIndex = 0;
          skin = (nuskin_s *)NuMemAlloc(0x18);
          indx_ = indx_ + 1;
          memset(skin,0,0x18);
          byteIndex = NuFileReadInt(handle);
          skin->vtxoffset = byteIndex;
          byteIndex = NuFileReadInt(handle);
          skin->vtxcnt = byteIndex;
          byteIndex = NuFileReadInt(handle);
          skin->mtxcnt = byteIndex;
          mtxid = (int *)NuMemAlloc(byteIndex << 2);
          skin->mtxid = mtxid;
          if (0 < skin->mtxcnt) {
            byteIndex = 0;
            do {
              cycleIndex = cycleIndex + 1;
              iVar1 = NuFileReadInt(handle);
              *(int *)(byteIndex + (int)skin->mtxid) = iVar1;
              byteIndex = byteIndex + 4;
            } while (cycleIndex < skin->mtxcnt);
          }
          byteIndex = skin->vtxcnt * skin->mtxcnt * 4;
          data = (float *)NuMemAlloc(byteIndex);
          skin->weights = data;
          NuFileRead(handle,data,byteIndex);
          NuGeomAddSkin(geom,skin);
        } while (indx_ < count);
      }
    }
    else {
      count = geom->vtxcount << 4;
      vtxskin_info = (NUVTXSKININFO_s *)NuMemAlloc(count);
      geom->vtxskininfo = vtxskin_info;
      NuFileRead(handle,vtxskin_info,count);
    }
  }
  return;
}


void ReadNuIFFBlendShape(filehandle handle,nugeom_s *geom)

{
  float fVar1;
  int vtx_count;
  int nblends;
  NUBLENDGEOM_s *blendgeom;
  nuvec_s **offsets;
  int next_ix;
  int size;
  nuvec_s *pnVar2;
  char cVar3;
  GS_Buffer *vtxBuf;
  int iVar4;
  int iVar5;
  int iVar6;
  double dVar7;
  double dVar8;
  double dVar9;
  int __n;
  
  nblends = NuFileReadInt(handle);
  if (nblends != 0) {
    __n = nblends << 2;
    blendgeom = (NUBLENDGEOM_s *)NuMemAlloc(0x40);
    geom->blendgeom = blendgeom;
    vtx_count = 0;
    memset(blendgeom,0,0x40);
    geom->blendgeom->nblends = nblends;
    offsets = (nuvec_s **)NuMemAlloc(__n);
    geom->blendgeom->blend_offsets = offsets;
    memset(geom->blendgeom->blend_offsets,0,__n);
    next_ix = (int)NuMemAlloc(__n);
    geom->blendgeom->ix = (int *)next_ix;
    NuFileRead(handle,geom->blendgeom->ix,__n);
    size = NuFileReadInt(handle);
    pnVar2 = (nuvec_s *)NuMemAlloc(size);
    geom->blendgeom->offsets = pnVar2;
    NuFileRead(handle,geom->blendgeom->offsets,size);
    if (0 < nblends) {
      iVar5 = 0;
      iVar4 = nblends;
      do {
        cVar3 = NuFileReadChar(handle);
        if (cVar3 != '\0') {
          *(nuvec_s **)(iVar5 + (int)geom->blendgeom->blend_offsets) =
               geom->blendgeom->offsets + vtx_count;
          vtx_count = vtx_count + geom->vtxcount;
        }
        iVar5 = iVar5 + 4;
        iVar4 = iVar4 + -1;
      } while (iVar4 != 0);
    }
    if (size == 0) {
      geom->blendgeom = (NUBLENDGEOM_s *)0x0;
    }
    blendgeom = geom->blendgeom;
    if (blendgeom != (NUBLENDGEOM_s *)0x0) {
      vtx_count = geom->vtxcount;
      size = 0;
      while (size < vtx_count) {
        if (0 < nblends) {
          iVar4 = 0;
          dVar8 = 0.0;
          vtx_count = nblends;
          do {
            iVar5 = *(int *)(iVar4 + (int)blendgeom->blend_offsets);
            if (iVar5 != 0) {
              iVar6 = size * 0xc;
              fVar1 = *(float *)(iVar6 + iVar5);
              dVar7 = (double)fVar1;
              NuFabs(fVar1);
              dVar9 = 9.999999747378752e-06;
              if (dVar7 < 9.999999747378752e-06) {
                *(float *)(iVar6 + *(int *)(iVar4 + (int)geom->blendgeom->blend_offsets)) =
                     (float)dVar8;
              }
              fVar1 = *(float *)(iVar6 + *(int *)(iVar4 + (int)geom->blendgeom->blend_offsets) + 4 );
              dVar7 = (double)fVar1;
              NuFabs(fVar1);
              if (dVar7 < dVar9) {
                *(float *)(iVar6 + *(int *)(iVar4 + (int)geom->blendgeom->blend_offsets) + 4) =
                     (float)dVar8;
              }
              fVar1 = *(float *)(iVar6 + *(int *)(iVar4 + (int)geom->blendgeom->blend_offsets) + 8 );
              dVar7 = (double)fVar1;
              NuFabs(fVar1);
              if (dVar7 < dVar9) {
                *(float *)(iVar6 + *(int *)(iVar4 + (int)geom->blendgeom->blend_offsets) + 8) =
                     (float)dVar8;
              }
              blendgeom = geom->blendgeom;
            }
            iVar4 = iVar4 + 4;
            vtx_count = vtx_count + -1;
          } while (vtx_count != 0);
        }
        size = size + 1;
        vtx_count = geom->vtxcount;
      }
      vtxBuf = GS_CreateBuffer(vtx_count * 0xc,3);
      geom->blendgeom->hVB = (int)vtxBuf;
    }
  }
  return;
}


void ReadNuIFFUnknown(fileHandle handle, u32 blockMagic)
{

        return;
}

int ReadNuIFFInstSet(filehandle handle,nuinstance_s *inst_data,void *param_3)

{
  undefined4 *puVar1;
  int iVar2;
  void *pvVar3;
  int iVar4;
  undefined4 *puVar5;
  undefined4 *puVar6;
  int iVar7;
  undefined4 *puVar8;
  
  iVar2 = NuFileReadInt(handle);
  pvVar3 = NuMemAlloc(iVar2 * 0x50);
  (inst_data->matrix).mtx[0] = (float)pvVar3;
  NuFileRead(handle,pvVar3,iVar2 * 0x50);
  iVar4 = NuFileReadInt(handle);
  if (iVar4 != 0) {
    pvVar3 = NuMemAlloc(iVar4 * 0x60);
    NuFileRead(handle,pvVar3,iVar4 * 0x60);
    if (param_3 != (void *)0x0) {
      *(void **)param_3 = pvVar3;
    }
    iVar4 = 0;
    if (0 < iVar2) {
      do {
        iVar7 = iVar4 * 0x50;
        iVar4 = iVar4 + 1;
        puVar5 = (undefined4 *)((int)(inst_data->matrix).mtx[0] + iVar7);
        if (puVar5[0x12] != 0) {
          puVar5[0x12] = pvVar3;
          pvVar3 = (void *)((int)pvVar3 + 0x60);
          iVar7 = 0x30;
          puVar1 = (undefined4 *)puVar5[0x12];
          do {
            puVar8 = puVar5;
            puVar6 = puVar1;
            iVar7 = iVar7 + -0x18;
            *puVar6 = *puVar8;
            puVar6[1] = puVar8[1];
            puVar6[2] = puVar8[2];
            puVar6[3] = puVar8[3];
            puVar6[4] = puVar8[4];
            puVar6[5] = puVar8[5];
            puVar1 = puVar6 + 6;
            puVar5 = puVar8 + 6;
          } while (iVar7 != 0);
          puVar6[6] = puVar8[6];
          puVar6[7] = puVar8[7];
          puVar6[8] = puVar8[8];
          puVar6[9] = puVar8[9];
        }
      } while (iVar4 < iVar2);
    }
  }
  return iVar2;
}


void ReadNuIFFSpecialObjects(int fh,nugscn_s *gsc)

{
  int numspec;
  nufspecial_s *file_specials;
  nuspecial_s *pnVar1;
  int cnt;
  int i;
  nuinstance_s *inst;
  nuinstance_s *pnVar2;
  nuspecial_s *spec;
  
  numspec = NuFileReadInt(fh);
  gsc->numspecial = numspec;
  file_specials = (nufspecial_s *)malloc_x(numspec * 0x50);
  NuFileRead(fh,file_specials,gsc->numspecial * 0x50);
  pnVar1 = (nuspecial_s *)NuMemAlloc(gsc->numspecial * 0x50);
  numspec = 0;
  gsc->specials = pnVar1;
  if (0 < gsc->numspecial) {
    do {
      cnt = numspec + 1;
      i = 0x30;
      gsc->specials[numspec].instance = gsc->instances + file_specials[numspec].instanceix;
      pnVar2 = gsc->specials[numspec].instance;
      pnVar1 = gsc->specials + numspec;
      do {
        spec = pnVar1;
        inst = pnVar2;
        i = i + -0x18;
        (spec->mtx)._00 = (inst->matrix)._00;
        (spec->mtx)._01 = (inst->matrix)._01;
        (spec->mtx)._02 = (inst->matrix)._02;
        (spec->mtx)._03 = (inst->matrix)._03;
        (spec->mtx)._10 = (inst->matrix)._10;
        pnVar2 = (nuinstance_s *)&(inst->matrix)._12;
        (spec->mtx)._11 = (inst->matrix)._11;
        pnVar1 = (nuspecial_s *)&(spec->mtx)._12;
      } while (i != 0);
      *(float *)pnVar1 = *(float *)pnVar2;
      (spec->mtx)._13 = (inst->matrix)._13;
      (spec->mtx)._20 = (inst->matrix)._20;
      (spec->mtx)._21 = (inst->matrix)._21;
      gsc->specials[numspec].name = gsc->nametable + file_specials[numspec].nameix;
      (gsc->specials[numspec].instance)->special_flag = '\x01';
      numspec = cnt;
    } while (cnt < gsc->numspecial);
  }
  free_x(file_specials);
  return;
}

void NuSceneCalcCulling(nugscn_s *scene)

{
  float *pfVar1;
  int iVar2;
  float fVar3;
  float fVar4;
  int iVar5;
  void *allc;
  nuvec_s *v;
  int numgobj;
  int ninst;
  int iVar6;
  float local_30;
  float local_2c;
  float local_28;
  
  numgobj = scene->numgobj;
  ninst = scene->numinstance;
  allc = malloc_x(numgobj << 2);
  memset(allc,0,numgobj << 2);
  if (0 < ninst) {
    iVar6 = 0;
    do {
      v = (nuvec_s *)(scene->instances->pad + iVar6 + -0x4f);
      if (*(char *)((int)&v[6].y + 2) == '\0') {
        if (v[6].x != 0.0) goto LAB_800b9214;
        NuMtxGetScale((numtx_s *)&local_30,v);
        iVar5 = *(int *)(scene->instances->pad + iVar6 + -0xf);
        iVar2 = iVar5 * 4;
        fVar3 = local_30;
        if (local_30 < *(float *)((int)allc + iVar2)) {
          fVar3 = *(float *)((int)allc + iVar2);
        }
        *(float *)((int)allc + iVar2) = fVar3;
        fVar4 = local_2c;
        if (local_2c < fVar3) {
          fVar4 = fVar3;
        }
        *(float *)(iVar2 + (int)allc) = fVar4;
        fVar3 = local_28;
        if (local_28 < fVar4) {
          fVar3 = fVar4;
        }
        *(float *)((int)allc + iVar2) = fVar3;
        scene->gobjs[iVar5]->culltype = 0;
      }
      else {
LAB_800b9214:
        scene->gobjs[(int)v[5].y]->culltype = 1;
      }
      iVar6 = iVar6 + 0x50;
      ninst = ninst + -1;
    } while (ninst != 0);
  }
  if (0 < numgobj) {
    ninst = 0;
    do {
      numgobj = numgobj + -1;
      pfVar1 = (float *)(ninst + (int)allc);
      iVar6 = *(int *)(ninst + (int)scene->gobjs);
      ninst = ninst + 4;
      *(float *)(iVar6 + 0x40) = *(float *)(iVar6 + 0x40) * *pfVar1;
    } while (numgobj != 0);
  }
  free_x(allc);
  return;
}


void ReadNuIFFGScene(filehandle handle,nugscn_s *gscene)
{
  int iVar1;
  char *NTBL_;
  int currentBlkType;
  int tasFileOffset;
  nuscene_s nus;
  
  tasFileOffset = -1;
  memset(&nus,0,0x3c);
  do {
    currentBlkType = NuFileBeginBlkRead(handle,0);
    if (currentBlkType == 0) {
      NuSceneMtlUpdate(&nus);
      gscene->mtls = nus.mtls;
      gscene->nummtl = nus.nummtls;
      gscene->numgobj = nus.numgobjs;
      gscene->gobjs = nus.gobjs;
      gscene->numtid = nus.numtids;
      gscene->tids = nus.tids;
      NuSceneCalcCulling(gscene);
      if (tasFileOffset != -1) {
        NuFileSeek(handle,tasFileOffset,0);
        ReadNuIFFTexAnimSet(handle,gscene,nus.tids);
      }
      return;
    }
    if (currentBlkType == 0TST) {
      ReadNuIFFTextureSet(handle,&nus);
    }
    else if (currentBlkType < 0TSU) {
      if (currentBlkType == 0SAT) {
        tasFileOffset = NuFilePos(handle);
      }
      else if (currentBlkType < 0SAU) {
        if (currentBlkType == 00SM) {
          ReadNuIFFMaterialSet(handle,&nus);
        }
        else {
LAB_800b9478:
          ReadNuIFFUnknown(handle,currentBlkType);
        }
      }
      else if (currentBlkType == 0TSG) {
        ReadNuIFFGobjSet(handle,&nus);
      }
      else {
        if (currentBlkType != 0TSS) goto LAB_800b9478;
        ReadNuIFFGSplineSet(handle,gscene);
      }
    }
    else if (currentBlkType == CEPS) {
      ReadNuIFFSpecialObjects(handle,gscene);
    }
    else if (currentBlkType < CEPT) {
      if (currentBlkType != BILA) goto LAB_800b9478;
      ReadNuIFFAnimationLibrary(handle,gscene);
    }
    else if (currentBlkType == LBTN) {
      NTBL_ = ReadNuIFFNameTable(handle);
      gscene->nametable = NTBL_;
    }
    else {
      if (currentBlkType != TSNI) goto LAB_800b9478;
      iVar1 = ReadNuIFFInstSet(handle,&gscene->instances,&gscene->instanimblock);
      gscene->numinstance = iVar1;
    }
    NuFileEndBlkRead(handle);
  } while( true );
}

nuscene_s * NuSceneLoad(char *filename)

{
  int len;
  filehandle handle;
  int index;
  nugscn_s *gscn;
  NUERRORFUNC *e;
  int i;
  nugeom_s *list_item;
  nugobj_s *gobj;
  nuscene_s *scene;
  char *filen;
  nuscene_s nus;
  nugeom_s **g;
  
  filen = (char *)&nus.names;
  strcpy(filen,filename);
  scene = (nuscene_s *)0x0;
  len = strlen((char *)&nus.names);
  filen[len + -1] = 'x';
  strcpy(filen,filename);
  handle = NuFileOpen(filen,NUFILE_READ);
  if (handle == 0) {
    strlwr(filename);
  }
  else {
    strlwr(filename);
    scene = (nuscene_s *)NuMemAlloc(0x3c);
    memset(scene,0,0x3c);
    len = blkcnt;
    index = NuFileBeginBlkRead(handle,0);
    if (index == 0CSG) {
      gscn = (nugscn_s *)NuMemAlloc(0x74);
      scene->gscene = gscn;
      memset(gscn,0,0x74);
      ReadNuIFFGScene(handle,scene->gscene);
      gscn = scene->gscene;
      index = 0;
      if (0 < gscn->numinstance) {
        do {
          i = index + 1;
          for (list_item = gscn->gobjs[gscn->instances[index].objid]->geom;
              list_item != (nugeom_s *)0x0; list_item = list_item->next) {
          }
          index = i;
        } while (i < gscn->numinstance);
      }
      NuSceneAverageTextureSpaceVerts(scene);
      gscn = scene->gscene;
      index = 0;
      if (0 < gscn->numgobj) {
        do {
          gobj = gscn->gobjs[index];
          index = index + 1;
          while (gobj != (nugobj_s *)0x0) {
            g = &gobj->geom;
            gobj = gobj->next_gobj;
            for (list_item = *g; list_item != (nugeom_s *)0x0; list_item = list_item->next) {
            }
          }
        } while (index < gscn->numgobj);
      }
      NuFileEndBlkRead(handle);
      NuFileClose(handle);
      if (len != blkcnt) {
        blkcnt = len;
      }
    }
    else {
      NuMemFree(scene);
      e = NuErrorProlog("C:/source/crashwoc/code/nu3dx/nuscene.c",0x718);
      (*e)("Baaaaad file <%s>",filename);
      scene = (nuscene_s *)0x0;
    }
  }
  return scene;
}


s32 ReadNuIFFNameTable(fileHandle handle)
{
    s32 size = NuFileReadInt(handle);
    return NuFileRead(handle, NuMemAlloc(size), size);
}


void NuGSceneDestroy(nugscn_s *gsc)

{
  nuAnimData_s **ianm;
  int i;
  int j;
  
  for (i = 0; i < gsc->numtid; i = i + 1) {
    NuTexDestroy((int)gsc->tids[i]);
  }
  i = 0;
  if (0 < gsc->nummtl) {
    do {
      j = i + 1;
      NuMtlDestroy(gsc->mtls[i]);
      i = j;
    } while (j < gsc->nummtl);
  }
  i = 0;
  if (0 < gsc->numgobj) {
    do {
      j = i + 1;
      NuGobjDestroy(gsc->gobjs[i]);
      i = j;
    } while (j < gsc->numgobj);
  }
  ianm = gsc->instanimdata;
  if ((ianm != (nuAnimData_s **)0x0) && (i = 0, 0 < gsc->numinstanims)) {
    while( true ) {
      NuAnimDataDestroy(ianm[i]);
      if (gsc->numinstanims <= i + 1) break;
      ianm = gsc->instanimdata;
      i = i + 1;
    }
  }
  return;
}


void NuSceneDestroy(nuscene_s *sc)

{
  int i;
  int j;
  
  if (sc != (nuscene_s *)0x0) {
    if (sc->names != (char **)0x0) {
      NuMemFree(sc->names)
    }
    if (sc->nodes != (NUNODE_s **)0x0) {
      NuMemFree(sc->nodes);
    }
    if (sc->strings != (char *)0x0) {
      NuMemFree(sc->strings);
    }
    i = 0;
    if (0 < sc->numtids) {
      do {
        j = i + 1;
        NuTexDestroy((int)sc->tids[i]);
        i = j;
      } while (j < sc->numtids);
    }
    i = 0;
    if (0 < sc->nummtls) {
      do {
        j = i + 1;
        NuMtlDestroy(sc->mtls[i]);
        i = j;
      } while (j < sc->nummtls);
    }
    i = 0;
    if (0 < sc->numgobjs) {
      do {
        j = i + 1;
        NuGobjDestroy(sc->gobjs[i]);
        i = j;
      } while (j < sc->numgobjs);
    }
    if (sc->gscene != (nugscn_s *)0x0) {
      NuGSceneDestroy(sc->gscene);
    }
  }
  return;
}