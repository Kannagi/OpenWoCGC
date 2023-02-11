#include "nuanim.h"
#include "../system.h"


void buildBitCountTable(void) //check asm
{
    s32 k;
    s32 j;
    s32 i;

    i = 0;
    do {
        BitCountTable[i] = 0;
        k = 8;
        j = 0;
loop_2:
        if ((i >> (j + 1)) & 1) {
            BitCountTable[i] += 1;
        }
        j += 1;
        k -= 1;
        if (k != 0) {
            goto loop_2;
        }
        i += 1;
    } while (i <= 0xFF);    // i < 0x100
    isBitCountTable = 1;
}

struct nuanimdata_s* NuAnimDataLoadBuff(s8* file, struct variptr_u* buff, struct variptr_u* endbuff) {
    s32 temp_r0;
    s32 temp_r3_2;
    u32 mem;
    struct nuanimdata_s* data;
    void* temp_r30;

    mem = (buff->voidptr + 0xF) & 0xFFFFFFF0;
    buff->intaddr = mem;
    temp_r3_2 = NuFileLoadBuffer(file, (void* ) mem, endbuff->voidptr - mem);
    if (temp_r3_2 == 0) {
        //NuErrorProlog("C:/source/crashwoc/code/nu3dx/nuanim.c", 0xB6E)
        //(""Could not read from <%s>, either file is not there or the buffer may be full"", file);
    }
    temp_r30 = buff->voidptr;
    buff->voidptr = temp_r30 + temp_r3_2;
    temp_r0 = temp_r30 - temp_r30->unk4;
    temp_r30->unk4 = temp_r0;
    data = NuAnimDataFixPtrs(temp_r30->unk8, temp_r0);
    temp_r30->unk8 = data;
    return data;
}

struct nuanimdata_s* NuAnimDataFixPtrs(struct nuanimdata_s* animdata, s32 address_offset) {
    s32 temp_r0_2;
    s32 temp_r0_3;
    s32 temp_r0_4;
    s32 temp_r0_5;
    s32 temp_r0_6;
    s32 temp_r0_7;
    s32 temp_r0_8;
    s32 temp_r11;
    s32 temp_r11_2;
    s32 temp_r9_3;
    s32 temp_r9_4;
    s32 var_r10;
    s32 var_r10_2;
    s32 var_r11_2;
    s32 var_r11_3;
    s32 var_r11_4;
    s32 var_r7;
    s32 var_r8;
    s32 var_r8_2;
    s32 var_r8_3;
    s32 var_r9;
    s32 var_r9_2;
    s8* temp_r0;
    s8* var_r11;
    struct nuanimdata_s* var_r3;
    struct nuanimdatachunk_s** temp_r9;
    struct nuanimdatachunk_s** temp_r9_2;
    struct nuanimdatachunk_s** var_r0;
    void* temp_r10;
    void* temp_r6;
    void* temp_r9_5;

    if ((s32) isBitCountTable == 0) {
        buildBitCountTable();
    }
    var_r3 = NULL;
    if (animdata != NULL) {
        var_r3 = animdata + address_offset;
    }
    temp_r0 = var_r3->node_name;
    var_r11 = NULL;
    if (temp_r0 != NULL) {
        var_r11 = temp_r0 + address_offset;
    }
    temp_r9 = var_r3->chunks;
    var_r0 = NULL;
    var_r3->node_name = var_r11;
    if (temp_r9 != NULL) {
        var_r0 = temp_r9 + address_offset;
    }
    var_r3->chunks = var_r0;
    if (var_r0 != NULL) {
        var_r10 = 0;
        if ((s32) var_r3->nchunks > 0) {
            do {
                temp_r9_2 = var_r3->chunks;
                temp_r11 = var_r10 * 4;
                var_r8 = 0;
                temp_r0_2 = *(temp_r11 + temp_r9_2);
                if (temp_r0_2 != 0) {
                    var_r8 = temp_r0_2 + address_offset;
                }
                *(temp_r11 + temp_r9_2) = var_r8;
                temp_r6 = *(temp_r11 + var_r3->chunks);
                if (temp_r6 != NULL) {
                    temp_r0_3 = temp_r6->unk8;
                    var_r9 = 0;
                    if (temp_r0_3 != 0) {
                        var_r9 = temp_r0_3 + address_offset;
                    }
                    temp_r6->unk8 = var_r9;
                    if (var_r9 != 0) {
                        var_r8_2 = 0;
                        if ((s32) temp_r6->unk0 > 0) {
                            do {
                                temp_r9_3 = temp_r6->unk8;
                                temp_r11_2 = var_r8_2 * 4;
                                var_r10_2 = 0;
                                temp_r0_4 = *(temp_r11_2 + temp_r9_3);
                                if (temp_r0_4 != 0) {
                                    var_r10_2 = temp_r0_4 + address_offset;
                                }
                                *(temp_r11_2 + temp_r9_3) = var_r10_2;
                                temp_r10 = *(temp_r11_2 + temp_r6->unk8);
                                if (temp_r10 != NULL) {
                                    temp_r0_5 = temp_r10->unk4;
                                    var_r11_2 = 0;
                                    if (temp_r0_5 != 0) {
                                        var_r11_2 = temp_r0_5 + address_offset;
                                    }
                                    temp_r0_6 = temp_r10->unk8;
                                    var_r9_2 = 0;
                                    temp_r10->unk4 = var_r11_2;
                                    if (temp_r0_6 != 0) {
                                        var_r9_2 = temp_r0_6 + address_offset;
                                    }
                                    temp_r10->unk8 = var_r9_2;
                                    if (var_r9_2 != 0) {
                                        var_r7 = 0;
                                        if ((s8) temp_r10->unkC > 0) {
                                            var_r8_3 = 0;
                                            do {
                                                temp_r9_4 = temp_r10->unk8;
                                                var_r11_3 = 0;
                                                temp_r0_7 = *(var_r8_3 + temp_r9_4);
                                                if (temp_r0_7 != 0) {
                                                    var_r11_3 = temp_r0_7 + address_offset;
                                                }
                                                *(var_r8_3 + temp_r9_4) = var_r11_3;
                                                temp_r9_5 = *(var_r8_3 + temp_r10->unk8);
                                                if (temp_r9_5 != NULL) {
                                                    temp_r0_8 = temp_r9_5->unk4;
                                                    var_r11_4 = 0;
                                                    if (temp_r0_8 != 0) {
                                                        var_r11_4 = temp_r0_8 + address_offset;
                                                    }
                                                    temp_r9_5->unk4 = var_r11_4;
                                                }
                                                var_r7 += 1;
                                                var_r8_3 += 4;
                                            } while (var_r7 < (s8) temp_r10->unkC);
                                        }
                                    }
                                }
                                var_r8_2 += 1;
                            } while (var_r8_2 < (s32) temp_r6->unk0);
                        }
                    }
                }
                var_r10 += 1;
            } while (var_r10 < (s32) var_r3->nchunks);
        }
    }
    return var_r3;
}



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
              if (*(float *)(flags + (int)ptrCset[nChunks]->constants) == 3.402823e+38) {
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