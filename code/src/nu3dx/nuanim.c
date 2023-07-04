#include "nuanim.h"
#include "../system.h"

#define ASSIGN_IF_SET(a, b) a = (a == NULL) ? NULL : b

void buildBitCountTable(void) //check NGC asm	//PS2
{
     	for(i = 0; i < 256; i++){        
            	BitCountTable[i] = 0;
            	for(j = 0; j < 8; j++) {
                	if (((i >> j) & 1) != 0) {
                    	BitCountTable[i]++;
                	}
            	}
        	}
        	isBitCountTable = 1;
	}
}

struct nuanimdata_s * NuAnimDataLoadBuff(char *file,union variptr_u *buff,union variptr_u *endbuff)
{
  s32 addroff;
  s32 tmp;
  struct nuanimdata_s *data;
  void *mem;
  struct NUANIMDATAHDR_s *dathdr;
  
  mem = (((s32)buff->voidptr + 0xf) & 0xfffffff0);
  buff->voidptr = mem;
  addroff = NuFileLoadBuffer(file,mem,(s32)endbuff->voidptr - (s32)mem);
  if (addroff == 0) {
    NuErrorProlog("..\\nu2.ps2\\nu3d\\nuanim.c",0x2f1)("Could not read from <%s>, either file is not there or the buffer may be full",file);
  }
  dathdr = (struct NUANIMDATAHDR_s *)buff->voidptr;
  buff->voidptr = (void *)((s32)&dathdr->version + addroff);
  tmp = (s32)dathdr - dathdr->address_offset;
  dathdr->address_offset = tmp;
  data = NuAnimDataFixPtrs(dathdr->animdata,tmp);
  dathdr->animdata = data;
  return data;
}


struct nuanimdata_s * NuAnimDataFixPtrs(struct nuanimdata_s *animdata,s32 address_offset)
{
    s32 i;
    s32 j;
    s32 iNodes;
    s32 iChunks;
    s32 iCurves;
    struct nuanimcurve_s *curve;
    struct nuanimcurveset_s *curveset;
    struct nuanimdatachunk_s *datachunk;
    
    if (isBitCountTable == 0) {
	buildBitCountTable();
    }

    ASSIGN_IF_SET(animdata, (struct nuanimdata_s *)((s32)animdata + address_offset));
    ASSIGN_IF_SET(animdata->node_name, animdata->node_name + address_offset);
    ASSIGN_IF_SET(animdata->chunks, ((s32)animdata->chunks + address_offset));
    
    if (animdata->chunks != NULL) {
        for (iChunks = 0; iChunks < animdata->nchunks; iChunks++)
        {
            ASSIGN_IF_SET(animdata->chunks[iChunks], (struct nuanimdatachunk_s *)((s32)animdata->chunks[iChunks] + address_offset));
            datachunk = animdata->chunks[iChunks];
            if (datachunk == NULL) {
                continue;
            }

            ASSIGN_IF_SET(datachunk->animcurvesets, (struct nuanimcurveset_s **)((s32)datachunk->animcurvesets + address_offset));
            if (datachunk->animcurvesets == NULL) {
                continue;
            }
            
            for (iNodes = 0; iNodes < datachunk->numnodes; iNodes++)
            {
                ASSIGN_IF_SET(datachunk->animcurvesets[iNodes], (struct nuanimcurveset_s *)((s32)datachunk->animcurvesets[iNodes] + address_offset));
                curveset = datachunk->animcurvesets[iNodes];
                if (curveset == NULL) {
                    continue;
                }
                
                ASSIGN_IF_SET(curveset->constants, (float *)((s32)curveset->constants + address_offset));
                ASSIGN_IF_SET(curveset->set, (struct nuanimcurve_s **)((s32)curveset->set + address_offset));
                if (curveset->set == NULL) {
                    continue;
                }
                
                for(iCurves = 0; iCurves < curveset->ncurves; iCurves++)
                {
                    ASSIGN_IF_SET(curveset->set[iCurves], (struct nuanimcurve_s *)((s32)curveset->set[iCurves] + address_offset));
                    curve = curveset->set[iCurves];
                    if (curve != NULL) {
                        ASSIGN_IF_SET(curve->animkeys, (struct nuanimkey_s *)((s32)curve->animkeys + address_offset));
                    }
                }
            }
        }
    }
    return animdata;
}

/*
struct nuanimdatachunk_s * NuAnimDataChunkCreate(s32 numnodes)
{
  struct nuanimdatachunk_s *chunk;
  struct nuanimcurveset_s **curveset;
  
  if (numnodes < 1) {
    //NuErrorProlog("C:/source/crashwoc/code/nu3dx/nuanim.c",0x253)("assert");
  }
  chunk = (struct nuanimdatachunk_s *)NuMemAlloc(0x14);
  memset(chunk,0,0x14);
  chunk->numnodes = numnodes;
  curveset = (struct nuanimcurveset_s **)NuMemAlloc(numnodes << 2);
  chunk->animcurveset = curveset;
  memset(curveset,0,numnodes << 2);
  return chunk;
}


struct nuanimcurveset_s * NuAnimCurveSetCreate(s32 ncurves)
{
  nuanimcurve_s **curveset;
  float *constants;
  nuanimcurveset_s *data;
  s32 nbytes;
  
  data = NULL;
  if (ncurves != 0) {
    nbytes = ncurves << 2;
    data = (struct nuanimcurveset_s *)NuMemAlloc(0x10);
    memset(data,0,0x10);
    data->ncurves = (char)ncurves;
    curveset = (struct nuanimcurve_s **)NuMemAlloc(nbytes);
    data->set = curveset;
    memset(curveset,0,nbytes);
    constants = (float *)NuMemAlloc(nbytes);
    data->constants = constants;
    memset(constants,0,nbytes);
  }
  return data;
}

*/


nuAnimData_s * NuAnimDataRead(filehandle handle)		//WIP
{
  int nChunks;
  char *data;
  nuAnimData_s *ADat;
  nuanimdatachunk_s *chunk;
  nuanimkey_s *keys;
  nuanimcurve_s *curves;
  char nCurves;
  nuanimcurveset_s *crvset;
  int flags;
  nuanimcurveset_s **ptrCset;
  int j;
  int N;
  int i;
  int k;
  double time;
  float fVar1;
  uint *nKeys;
  
  if (isBitCountTable == 0) {
    buildBitCountTable();
  }
  nChunks = NuFileReadInt(handle);
  if (nChunks == 0) {
    data = (char *)0x0;
  }
  else {
    data = (char *)NuMemAlloc(nChunks);
    NuFileRead(handle,data,nChunks);
  }
  i = 0;
  fVar1 = NuFileReadFloat(handle);
  time = (double)fVar1;
  nChunks = NuFileReadInt(handle);
  ADat = NuAnimDataCreate(nChunks);
  ADat->time = (float)time;
  ADat->node_name = data;
  if (0 < ADat->nchunks) {
    do {
      nChunks = NuFileReadInt(handle);
      chunk = NuAnimDataChunkCreate(nChunks);
      ADat->chunks[i] = chunk;
      chunk->numnodes = nChunks;
      N = NuFileReadInt(handle);
      if (N == 0) {
        chunk->keys = (nuanimkey_s *)0x0;
      }
      else {
        keys = (nuanimkey_s *)NuMemAlloc(N << 4);
        chunk->keys = keys;
        NuFileRead(handle,keys,N << 4);
      }
      keys = chunk->keys;
      N = NuFileReadInt(handle);
      if (N == 0) {
        chunk->curves = (nuanimcurve_s *)0x0;
      }
      else {
        curves = (nuanimcurve_s *)NuMemAlloc(N << 4);
        chunk->curves = curves;
        NuFileRead(handle,curves,N << 4);
      }
      i = i + 1;
      N = 0;
      if (0 < nChunks) {
        do {
          j = N + 1;
          nCurves = NuFileReadChar(handle);
          if (nCurves != 0) {
            crvset = NuAnimCurveSetCreate((int)nCurves);
            k = 0;
            chunk->animcurveset[N] = crvset;
            flags = NuFileReadInt(handle);
            chunk->animcurveset[N]->flags = flags;
            if ('\0' < chunk->animcurveset[N]->ncurves) {
              flags = 0;
              do {
                k = k + 1;
                fVar1 = NuFileReadFloat(handle);
                *(float *)(flags + (int)chunk->animcurveset[N]->constants) = fVar1;
                flags = flags + 4;
              } while (k < chunk->animcurveset[N]->ncurves);
            }
          }
          N = j;
        } while (j < nChunks);
      }
      nChunks = 0;
      curves = chunk->curves;
      if (0 < chunk->numnodes) {
        do {
          ptrCset = chunk->animcurveset;
          N = nChunks + 1;
          if ((ptrCset[nChunks] != (nuanimcurveset_s *)0x0) &&
             (j = 0, '\0' < ptrCset[nChunks]->ncurves)) {
            flags = 0;
            do {
              if (*(float *)(flags + (int)ptrCset[nChunks]->constants) == 3.402823e+38) //3.402823e+38 --> ff ff 7f 7f
		{
                *(nuanimcurve_s **)(ptrCset[nChunks]->set->pad + flags + -0xd) = curves;
                chunk->num_valid_animcurvesets = chunk->num_valid_animcurvesets + 1;
                curves->animkeys = keys;
                nKeys = &curves->numkeys;
                curves = curves + 1;
                keys = keys + *nKeys;
              }
              ptrCset = chunk->animcurveset;
              j = j + 1;
              flags = flags + 4;
            } while (j < ptrCset[nChunks]->ncurves);
          }
          nChunks = N;
        } while (N < chunk->numnodes);
      }
    } while (i < ADat->nchunks);
  }
  return ADat;
}


void NuAnimCurveSetDestroy(struct nuanimcurveset_s* animcurveset, s32 destroy_curves) //CHECK
{
    s32 var_r29;
    s32 i;
    struct nuanimcurve_s* curve;

    if ((animcurveset != NULL) && ((struct nuanimcurve_s** ) animcurveset->set != NULL) && (destroy_curves != 0)) {
        i = 0;
        if ((s8) (u8) animcurveset->ncurves > 0) {
            var_r29 = 0;
            do {
                curve = *(var_r29 + animcurveset->set); //could be? curve = (animcurveset->set[var_r29]);
                if (curve != NULL) {
                    NuAnimCurveDestroy(curve);
                }
                i += 1;
                var_r29 += 4;
            } while (i < (s8) (u8) animcurveset->ncurves);
        }
    }


void NuAnimDataDestroy(nuAnimData_s *animdata)

{
  nuanimdatachunk_s **chunk;
  int i;
  
  if (((animdata != (nuAnimData_s *)0x0) &&
      (chunk = animdata->chunks, chunk != (nuanimdatachunk_s **)0x0)) &&
     (i = 0, 0 < animdata->nchunks)) {
    while( true ) {
      NuAnimDataChunkDestroy(chunk[i]);
      if (animdata->nchunks <= i + 1) break;
      chunk = animdata->chunks;
      i = i + 1;
    }
  }
  return;
}

//PS2 96%
void NuAnimDataCalcTime(struct nuanimdata_s *animdata,float time,struct nuanimtime_s *atime)
{
    s32 iVar1;
    s32 dVar2;
    s32 iVar3;
    
    if (animdata->time <= time) {
        if (animdata->time == 1.0f) {
          atime->time = 1.0f;
          atime->time_byte = 0;
          atime->time_mask = 1;
          atime->chunk = 0;
          return;
        }
        atime->time = animdata->time - 0.01f;
    }
    else if (time < 1.0f) {
        atime->time = 1.0f;
    }
    else {
        atime->time = time;
    }

    atime->chunk = floor((atime->time - 1.0) / 32.0);
    if (atime->chunk < animdata->nchunks) {
        // ??????
    }
    else {
        atime->chunk = animdata->nchunks - 1;
    }
    
    atime->time_offset = atime->time - atime->chunk * 32;
    iVar1 = floor(atime->time_offset);
    iVar1--;
    
    atime->time_byte = (u8)(iVar1 / 8);
    atime->time_mask = (u8)((1 << (iVar1 + (iVar1 / 8) * -8 + 1)) - 1);
return;
}


void NuAnimCurve2SetApplyToJoint
               (nuanimcurve2_s *animcurveset,char *curveflags,char curvesetflags,nuanimtime_s *ati me
               ,NUJOINTDATA_s *jointdata,nuvec_s *scale,nuvec_s *parentscale,numtx_s *T,
               NUJOINTANIM_s *offset)

{
  uint max;
  float tmp;
  float z;
  char procanim_flags;
  nuvec_s t;
  nuvec_s r;
  nuvec_s lo;
  nuangvec_s rf;
  nuvec_s inv_scale;
  
  if (offset == (NUJOINTANIM_s *)0x0) {
    procanim_flags = '\0';
  }
  else {
    procanim_flags = offset->flags;
  }
  if ((curvesetflags & 1U) == 0) {
    if ((procanim_flags & 1U) != 0) {
      r.x = 0.0;
      r.z = 0.0;
      r.y = r.x;
      goto LAB_800aac1c;
    }
    NuMtxSetIdentity(T);
  }
  else {
    r.x = NuAnimCurve2CalcVal(animcurveset + 3,atime,(int)curveflags[3]);
    r.y = NuAnimCurve2CalcVal(animcurveset + 4,atime,(int)curveflags[4]);
    r.z = NuAnimCurve2CalcVal(animcurveset + 5,atime,(int)curveflags[5]);
LAB_800aac1c:
    if ((procanim_flags & 1U) == 0) {
      rf.x = (int)(r.x * 10430.38);
      rf.y = (int)(r.y * 10430.38);
      rf.z = (int)(r.z * 10430.38);
    }
    else {
      r.x = r.x + offset->rx;
      r.y = r.y + offset->ry;
      r.z = r.z + offset->rz;
      rf.x = (int)(r.x * 10430.38);
      if ((procanim_flags & 8U) != 0) {
        rf.x = (int)(r.x * 10430.38) & 0xffff;
        if (0x7fff < (uint)rf.x) {
          rf.x = rf.x - 0x10000;
        }
        max = (uint)offset->max_rx;
        if (((int)max < rf.x) || (max = (uint)offset->min_rx, rf.x < (int)max)) {
          rf.x = max;
        }
      }
      rf.y = (int)(r.y * 10430.38);
      if ((procanim_flags & 0x10U) != 0) {
        rf.y = (int)(r.y * 10430.38) & 0xffff;
        if (0x7fff < (uint)rf.y) {
          rf.y = rf.y - 0x10000;
        }
        max = (uint)offset->max_ry;
        if (((int)max < rf.y) || (max = (uint)offset->min_ry, rf.y < (int)max)) {
          rf.y = max;
        }
      }
      rf.z = (int)(r.z * 10430.38);
      if ((procanim_flags & 0x20U) != 0) {
        rf.z = (int)(r.z * 10430.38) & 0xffff;
        if (0x7fff < (uint)rf.z) {
          rf.z = rf.z - 0x10000;
        }
        max = (uint)offset->max_rz;
        if (((int)max < rf.z) || (max = (uint)offset->min_rz, rf.z < (int)max)) {
          rf.z = max;
        }
      }
    }
    NuMtxSetRotateXYZ(T,&rf);
  }
  if ((curvesetflags & 0x20U) != 0) {
    NuMtxMulR(T,T,jointdata);
  }
  if ((curvesetflags & 8U) == 0) {
    if ((procanim_flags & 4U) == 0) {
      tmp = parentscale->y;
      z = parentscale->z;
      scale->x = parentscale->x;
      scale->y = tmp;
      scale->z = z;
      goto LAB_800aae24;
    }
    scale->x = 0.0;
    scale->z = 0.0;
    scale->y = 0.0;
  }
  else {
    tmp = NuAnimCurve2CalcVal(animcurveset + 6,atime,(int)curveflags[6]);
    scale->x = tmp;
    tmp = NuAnimCurve2CalcVal(animcurveset + 7,atime,(int)curveflags[7]);
    scale->y = tmp;
    tmp = NuAnimCurve2CalcVal(animcurveset + 8,atime,(int)curveflags[8]);
    scale->z = tmp;
  }
  if ((procanim_flags & 4U) != 0) {
    scale->x = scale->x + offset->sx;
    scale->y = scale->y + offset->sy;
    scale->z = scale->z + offset->sz;
  }
  NuMtxPreScale(T,scale);
  scale->x = scale->x * parentscale->x;
  scale->y = scale->y * parentscale->y;
  scale->z = scale->z * parentscale->z;
LAB_800aae24:
  if ((curvesetflags & 0x10U) != 0) {
    inv_scale.x = NuFdiv(1.0,parentscale->x);
    inv_scale.y = NuFdiv(1.0,parentscale->y);
    inv_scale.z = NuFdiv(1.0,parentscale->z);
    NuMtxScale(T,&inv_scale);
    scale->z = scale->z * inv_scale.z;
    scale->x = scale->x * inv_scale.x;
    scale->y = scale->y * inv_scale.y;
  }
  t.x = NuAnimCurve2CalcVal(animcurveset,atime,(int)*curveflags);
  t.y = NuAnimCurve2CalcVal(animcurveset + 1,atime,(int)curveflags[1]);
  t.z = NuAnimCurve2CalcVal(animcurveset + 2,atime,(int)curveflags[2]);
  if ((procanim_flags & 2U) != 0) {
    t.x = t.x + offset->tx;
    t.z = t.z + offset->tz;
    t.y = t.y + offset->ty;
  }
  NuMtxTranslate(T,&t);
  if ((jointdata->flags & 8U) != 0) {
    NuMtxPreTranslate(T,&jointdata->locator_offset);
    lo.y = -(jointdata->locator_offset).y;
    lo.z = -(jointdata->locator_offset).z;
    lo.x = -(jointdata->locator_offset).x;
    NuMtxTranslate(T,&lo);
  }
  T->_02 = -T->_02;
  T->_12 = -T->_12;
  T->_32 = -T->_32;
  T->_20 = -T->_20;
  T->_21 = -T->_21;
  T->_23 = -T->_23;
  return;
}


//PS2 MATCH 87,26%
float NuAnimCurveCalcVal2(struct nuanimcurve_s *animcurve,struct nuanimtime_s *atime)
{
    struct nuanimkey_s *key;
    float time;
    float fVar2;
    float fVar3;
    float fVar4;
    float fVar5;
    s32 tbyte;
    s32 tbyte2;
    u8* test;

    test = (u8*)&animcurve->mask; // ???
    switch (atime->time_byte) {
        case 0:
            tbyte =  BitCountTable[test[0] & atime->time_mask];
            break;
        case 1:
            tbyte =  BitCountTable[test[0]];
            tbyte += BitCountTable[test[1] & atime->time_mask];
            break;
        case 2:
            tbyte =  BitCountTable[test[0]];
            tbyte += BitCountTable[test[1]];
            tbyte += BitCountTable[test[2] & atime->time_mask];
            break;
        case 3:
            tbyte =  BitCountTable[test[0]];
            tbyte += BitCountTable[test[1]];
            tbyte += BitCountTable[test[2]];
            tbyte += BitCountTable[test[3] & atime->time_mask];
            break;
    }   
    key = animcurve->animkeys;
    time = key[tbyte-1].time;
    if ((animcurve->flags & 1) == 0) {
        time -= key[tbyte].time;
        
        fVar2 = key[tbyte - 1].d - key[tbyte].d;
        fVar3 = key[tbyte - 1].c * time;
        time = key[tbyte].c * time;
        fVar4 = (atime->time - key[tbyte - 1].time) * key[tbyte - 1].dtime;
        return fVar4 * (fVar4 * (((fVar4 * (fVar2 + fVar2 + fVar3 + time) + fVar2 * -3.0f) -
                               (fVar3 + fVar3)) - time) + fVar3) + key[tbyte - 1].d;
    } else {
        return key[tbyte-1].d;
    }
}



//PS2 Version
void NuAnimCurveSetApplyToJointBasic (struct nuanimcurveset_s *animcurveset, struct nuanimtime_s *atime,  
struct NUJOINTDATA_s *jointdata, struct NuVec *scale,struct NuVec *parentscale, 
struct numtx_s *T, struct NUJOINTANIM_s *offset)
{
  //UNKTYPE *puVar1;
  //unsigned long *puVar2;
  //unsigned long uVar3;
  char procanim_flags;
  //float fVar4;
  //u32 uVar5;
  //float fVar6;
  //float fVar7;
  //u32 uvar;
  //u32 uVar1;
  struct NuVec r;
  struct nuangvec_s rf;
  //float fVar2;
  
  if (offset != NULL) {
    procanim_flags = offset->flags;
  }
  else {
    procanim_flags = '\0';
  }
  if (animcurveset->set[3] != NULL) {
      r.x = NuAnimCurveCalcVal2(animcurveset->set[3],atime);
  }
  else {
    r.x = animcurveset->constants[3];
  }
  if (animcurveset->set[4] != NULL) {
    r.y = NuAnimCurveCalcVal2(animcurveset->set[4],atime);
  }
  else {
      r.y = animcurveset->constants[4];
  }
  if (animcurveset->set[5] != NULL) {
    r.z = NuAnimCurveCalcVal2(animcurveset->set[5],atime);
  }
  else {
    r.z = animcurveset->constants[5];
  }
  if ((procanim_flags & 1) != 0)  {
    r.x += offset->rx;
    r.y += offset->ry;
    r.z += offset->rz;
      rf.x = (int)(r.x * 10430.378f);
      rf.y = (int)(r.y * 10430.378f);
      rf.z = (int)(r.z * 10430.378f);
      
    if ((procanim_flags & 8) != 0) {
        rf.x &= 0xFFFF;
      if (0x7fff < rf.x) {
        rf.x -= 0x10000;
      }
      if (offset->max_rx < rf.x) {
        rf.x = offset->max_rx;
      }
      else {
        //uvar = rf.x;
        if (rf.x < offset->min_rx) {
          rf.x = offset->min_rx;
        }
      }
    }
    //rf.x = uvar;
    //rf.y._0_2_ = (u16)uVar5;
    if ((procanim_flags & 0x10) != 0) {
      rf.y &= 0xffff;
      if (0x7fff < rf.y) {
        rf.y -= 0x10000;
      }
      if (offset->max_ry < rf.y) {
        rf.y = offset->max_ry;
      }
      else {
        //rf.y = rf.y;
        if (rf.y < offset->min_ry) {
          rf.y = offset->min_ry;
        }
      }
    }
    //rf.y = uVar5;
    if ((procanim_flags & 0x20) != 0) {
    //uvar = rf.z & 0xffff;
      rf.z &= 0xFFFF;
      if (0x7fff < rf.z) {
        rf.z -= 0x10000;
      }
      if ((long)offset->max_rz < (long)rf.z) {
        rf.z = (int)offset->max_rz;
      }
      else if ((long)rf.z < (long)offset->min_rz) {
        rf.z = (int)offset->min_rz;
      }
    }
  }
  else {                                        //if ((procanim_flags & 1) == 0)
    rf.x = (int)(r.x * 10430.378f);
    rf.y = (int)(r.y * 10430.378f);
    rf.z = (int)(r.z * 10430.378f);
  }
  NuMtxSetRotateXYZ(T,&rf);
  if ((animcurveset->flags & 0x20U) != 0) {
    NuMtxMulR(T,T,&jointdata->orient);
  }
  T->_30 = animcurveset->constants[0];
  T->_31 = animcurveset->constants[1];
  T->_32 = animcurveset->constants[2];
  if ((procanim_flags & 2) != 0) {
    T->_30 += offset->tx;
    T->_31 += offset->ty;
    T->_32 += offset->tz;
  }
    //memcpy
  *scale = *parentscale;    //or -->  memcpy ( parentscale, scale, sizeof(struct NuVec) );
  T->_02 = -T->_02;
  T->_12 = -T->_12;
  T->_20 = -T->_20;
  T->_21 = -T->_21;
  T->_23 = -T->_23;
  T->_32 = -T->_32;
  return;
}

//PS2 99,45% match
void NuAnimCurveSetApplyToJoint2
               (struct nuanimcurveset_s *animcurveset,struct nuanimtime_s *atime, 
                struct NUJOINTDATA_s *jointdata,struct NuVec *scale ,
               struct NuVec *parentscale,struct numtx_s *T, struct NUJOINTANIM_s *offset)

{
    struct nuanimcurve_s **ppNVar3;
    char procanim_flags;
    struct NuVec local_e0;
    struct NuVec r;
    struct NuVec lo;
    struct nuangvec_s rf;
    struct NuVec local_a0;
    
    if (offset != NULL) {
        procanim_flags = offset->flags;
    }
    else {
        procanim_flags = 0x00;
    }

    if ((animcurveset->flags & 1) == 0)
    {
        if((procanim_flags & 1) == 0) {
            goto procanim_alt1;
        }
        goto procanim_zero1;
    } else {
        if (animcurveset->set[3] != NULL) {
            r.x = NuAnimCurveCalcVal2(animcurveset->set[3], atime);
        }
        else {
            r.x = animcurveset->constants[3];
        }
        
        if (animcurveset->set[4] != NULL) {
            r.y = NuAnimCurveCalcVal2(animcurveset->set[4], atime);
        }
        else {
            r.y = animcurveset->constants[4];
        }
        
        if (animcurveset->set[5] != NULL) {
            r.z = NuAnimCurveCalcVal2(animcurveset->set[5], atime);
        }
        else {
            r.z = animcurveset->constants[5];
        }
        goto procanim_body1;
    }
    
    if(procanim_flags & 1) {
        procanim_zero1:
        r.x = r.y = r.z = 0.0f;
        procanim_body1:
        if(procanim_flags & 1) {
            r.x += offset->rx;
            r.y += offset->ry;
            r.z += offset->rz;
            rf.x = (int)(r.x * 10430.378f);
            rf.y = (int)(r.y * 10430.378f);
            rf.z = (int)(r.z * 10430.378f);
            
            if ((procanim_flags & 8U) != 0) {
                rf.x &= 0xFFFF;
                if (0x7fff < rf.x) {
                    rf.x -= 0x10000;
                }
                if (offset->max_rx < rf.x) {
                    rf.x = (int)offset->max_rx;
                }
                else if (rf.x < offset->min_rx) {
                    rf.x = (int)offset->min_rx;
                }
            }
            
            if ((procanim_flags & 0x10) != 0) {
                rf.y &= 0xFFFF;
                if (0x7fff < rf.y) {
                    rf.y -= 0x10000;
                }
                if (offset->max_ry < rf.y) {
                    rf.y = (int)offset->max_ry;
                }
                else if (rf.y < offset->min_ry) {
                    rf.y = (int)offset->min_ry;
                }
            }
            
            if ((procanim_flags & 0x20) != 0) {
                rf.z &= 0xFFFF;
                if (0x7fff < rf.z) {
                    rf.z -= 0x10000;
                }
                if (offset->max_rz < rf.z) {
                    rf.z = (int)offset->max_rz;
                }
                else if (rf.z < offset->min_rz) {
                    rf.z = (int)offset->min_rz;
                }
            }
        } else {
            rf.x = (int)(r.x * 10430.378f);
            rf.y = (int)(r.y * 10430.378f);
            rf.z = (int)(r.z * 10430.378f);
        }
        NuMtxSetRotateXYZVU0(T, &rf);
    } else {
        procanim_alt1:
        NuMtxSetIdentity(T);
    }

    if ((animcurveset->flags & 0x20) != 0) {
        NuMtxMulRVU0(T, T, &jointdata->orient);
    }
    
    if ((animcurveset->flags & 8U) == 0) {
        if((procanim_flags & 4) == 0) {
            goto procanim_alt2;
        }
        goto procanim_zero2;
    } else {
        if (animcurveset->set[6] != NULL) {
            scale->x = NuAnimCurveCalcVal2(animcurveset->set[6], atime);
        }
        else {
            scale->x = animcurveset->constants[6];
        }
        if (animcurveset->set[7] != NULL) {
            scale->y = NuAnimCurveCalcVal2(animcurveset->set[7], atime);
        }
        else {
            scale->y = animcurveset->constants[7];
        }
        if (animcurveset->set[8] != NULL) {
            scale->z = NuAnimCurveCalcVal2(animcurveset->set[8], atime);
        }
        else {
            scale->z = animcurveset->constants[8];
        }
        goto procanim_body2;
    }


    if ((procanim_flags & 4)) {
        procanim_zero2:
        scale->x = scale->y = scale->z = 0.0f;
        procanim_body2:
        if ((procanim_flags & 4)) {
            scale->x += offset->sx;
            scale->y += offset->sy;
            scale->z += offset->sz;
        }
        NuMtxPreScaleVU0(T, scale);
        scale->x *= parentscale->x;
        scale->y *= parentscale->y;
        scale->z *= parentscale->z;
    }
    else {
        procanim_alt2:
        *scale = *parentscale;
    }
    
    if ((animcurveset->flags & 0x10U)  && (parentscale != 0)) {
        local_a0.x = 1.0f / parentscale->x;
        local_a0.y = 1.0f / parentscale->y;
        local_a0.z = 1.0f / parentscale->z;
        NuMtxScaleVU0(T, &local_a0);
        scale->x *= local_a0.x;
        scale->y *= local_a0.y;
        scale->z *= local_a0.z;
    }

    if (animcurveset->set[0] != NULL) {
        local_e0.x = NuAnimCurveCalcVal2(animcurveset->set[0], atime);
    }
    else {
        local_e0.x = animcurveset->constants[0];
    }
    if (animcurveset->set[1] != NULL) {
        local_e0.y = NuAnimCurveCalcVal2(animcurveset->set[1], atime);
    }
    else {
        local_e0.y = animcurveset->constants[1];
    }
    if (animcurveset->set[2] != NULL) {
        local_e0.z = NuAnimCurveCalcVal2(animcurveset->set[2], atime);
    }
    else {
        local_e0.z = animcurveset->constants[2];
    }
    
    if ((procanim_flags & 2) != 0) {
        local_e0.x = local_e0.x + offset->tx;
        local_e0.y = local_e0.y + offset->ty;
        local_e0.z = local_e0.z + offset->tz;
    }
    
    NuMtxTranslate(T, &local_e0);
    if ((jointdata->flags & 8) != 0) {
        NuMtxPreTranslate(T, &jointdata->locator_offset);
        lo.x = -(jointdata->locator_offset).x;
        lo.y = -(jointdata->locator_offset).y;
        lo.z = -(jointdata->locator_offset).z;
        NuMtxTranslate(T,&lo);
    }
    T->_02 = -T->_02;
    T->_12 = -T->_12;
    T->_20 = -T->_20;
    T->_21 = -T->_21;
    T->_23 = -T->_23;
    T->_32 = -T->_32;
    return;
}


//PS2 Match
void NuAnimCurveSetApplyToMatrix (struct nuanimcurveset_s *animcurveset,struct nuanimtime_s *atime,struct numtx_s *T)
{
  s32 uVar1;
  struct NuVec local_80;
  struct NuVec local_70;
  struct nuangvec_s rf;
  struct NuVec local_50;
  

  if ((animcurveset->flags & 1U) != 0)  {
    if (animcurveset->set[3] != NULL) {
      local_70.x = NuAnimCurveCalcVal2(animcurveset->set[3],atime);
    }
    else {
        local_70.x = animcurveset->constants[3];
    }
    if (animcurveset->set[4] != NULL) {
      local_70.y = NuAnimCurveCalcVal2(animcurveset->set[4],atime);
    }
    else {
      local_70.y = animcurveset->constants[4];
    }
    if (animcurveset->set[5] != NULL) {
      local_70.z = NuAnimCurveCalcVal2(animcurveset->set[5],atime);
    }
    else {
      local_70.z = animcurveset->constants[5];
    }
    rf.x = (int)(local_70.x * 10430.378f);
    rf.y = (int)(local_70.y * 10430.378f);
    rf.z = (int)(local_70.z * 10430.378f);
    NuMtxSetRotateXYZ(T,&rf);
    procanim_alt1:
  }
  else{
    NuMtxSetIdentity(T);
  }

  if ((animcurveset->flags & 8) != 0) {
    if (animcurveset->set[6] != NULL) {
      local_50.x = NuAnimCurveCalcVal2(animcurveset->set[6],atime);
    }
    else {
      local_50.x = animcurveset->constants[6];
    }
    if (animcurveset->set[7] != NULL) {
      local_50.y = NuAnimCurveCalcVal2(animcurveset->set[7],atime);
    }
    else {
      local_50.y = animcurveset->constants[7];
    }
    if (animcurveset->set[8] != NULL) {
      local_50.z = NuAnimCurveCalcVal2(animcurveset->set[8],atime);
    }
    else {
      local_50.z = animcurveset->constants[8];
    }
    NuMtxPreScale(T,&local_50);
  }
  if (*animcurveset->set != NULL) {
    local_80.x = NuAnimCurveCalcVal2(*animcurveset->set,atime);
  }
  else {
    local_80.x = *animcurveset->constants;
  }
  if (animcurveset->set[1] != NULL) {
    local_80.y = NuAnimCurveCalcVal2(animcurveset->set[1],atime);
  }
  else {
    local_80.y = animcurveset->constants[1];
  }
  if (animcurveset->set[2] != NULL) {
    local_80.z = NuAnimCurveCalcVal2(animcurveset->set[2],atime);
  }
  else {
    local_80.z = animcurveset->constants[2];
  }
  NuMtxTranslate(T,&local_80);
  T->_02 = -T->_02;
  T->_12 = -T->_12;
  T->_20 = -T->_20;
  T->_21 = -T->_21;
  T->_23 = -T->_23;
  T->_32 = -T->_32;
  return;
}

//PS2 Match
void NuAnimCurve2SetApplyToMatrix (struct nuanimcurve2_s *animcurveset,char *curveflags,char curvesetflags,
struct nuanimtime_s *atime, struct numtx_s *T)
{
  struct NuVec local_a0;
  struct NuVec local_90;
  struct nuangvec_s rf;
  struct NuVec local_70;
    
  if ((curvesetflags & 1U) != 0) {
    local_90.x = NuAnimCurve2CalcVal(animcurveset + 3,atime,(int)curveflags[3]);
    local_90.y = NuAnimCurve2CalcVal(animcurveset + 4,atime,(int)curveflags[4]);
    local_90.z = NuAnimCurve2CalcVal(animcurveset + 5,atime,(int)curveflags[5]);
    rf.z = (s32)(local_90.z * 10430.378f);
    rf.x = (s32)(local_90.x * 10430.378f);
    rf.y = (s32)(local_90.y * 10430.378f);
    NuMtxSetRotateXYZ(T,&rf);
  }
  else {
      NuMtxSetIdentity(T);
  }
  if ((curvesetflags & 8U) != 0) {
    local_70.x = NuAnimCurve2CalcVal(animcurveset + 6,atime,(int)curveflags[6]);
    local_70.y = NuAnimCurve2CalcVal(animcurveset + 7,atime,(int)curveflags[7]);
    local_70.z = NuAnimCurve2CalcVal(animcurveset + 8,atime,(int)curveflags[8]);
    NuMtxPreScale(T,&local_70);
  }
  local_a0.x = NuAnimCurve2CalcVal(animcurveset,atime,(int)*curveflags);
  local_a0.y = NuAnimCurve2CalcVal(animcurveset + 1,atime,(int)curveflags[1]);
  local_a0.z = NuAnimCurve2CalcVal(animcurveset + 2,atime,(int)curveflags[2]);
  NuMtxTranslate(T,&local_a0);
  T->_02 = -T->_02;
  T->_12 = -T->_12;
  T->_20 = -T->_20;
  T->_21 = -T->_21;
  T->_23 = -T->_23;
  T->_32 = -T->_32;
  return;
}