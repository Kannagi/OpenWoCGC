#include "../nu.h"

//NGC MATCH
void DebrisMalloc(void) {
  if (debbuffer == NULL) {
    debbuffer = (char *)malloc_x(0x93400);
  }
  return;
}

//NGC MATCH
s32 DebAlloc(void) {
  s32 key;
  
  if (freedebkeyptr > 0xff) {
   return -1;
  }
    key = (int)freedebkeys[freedebkeyptr];
    freedebkeyptr++;
    debkeydata[key].count = 0;
    debkeydata[key].debcount = 0;
    debkeydata[key].reqcount = 0;
    debkeydata[key].reqdebcount = 0;
    debkeydata[key].chunks[0] = NULL;
    return key;
}

//NGC MATCH
void DebReAlloc(struct debkeydatatype_s *debkey,s32 newdebcount) {
  s32 newchunksneeded;
  
  debkey->reqdebcount = (short)newdebcount;
  newchunksneeded = newdebcount + 0x1f;
  if (newchunksneeded < 0) {
    newchunksneeded = newdebcount + 0x3e;
  }
  debkey->reqcount = (newchunksneeded >> 5);
  if (debkey->reqcount == debkey->count) {
    debkey->debcount = (short)newdebcount;
  }
  else {
    DebReAlloc2(debkey);
  }
  return;
}

//NGC MATCH
void DebrisOrientation(s32 key,short rotz,short roty) {
  if (key != -1) {
    debkeydata[key].rotmtx = numtx_identity;
    NuMtxRotateZ(&debkeydata[key].rotmtx,rotz);
    NuMtxRotateY(&debkeydata[key].rotmtx,roty);
  }
  return;
}


//NGC MATCH
void AddVariableShotDebrisEffectMtx(s32 type,struct nuvec_s *pos,s32 numdeb,short emitrotz,short emitroty,struct numtx_s *rotmtx) {
  struct numtx_s tmtx;

  tmtx = numtx_identity;
  NuMtxRotateZ(&tmtx,emitrotz);
  NuMtxRotateY(&tmtx,emitroty);
  AddVariableShotDebrisEffectMtx2(type,pos,numdeb,&tmtx,rotmtx);
  return;
}

//NGC MATCH
s32 LookupDebrisEffect(char *name) {
  s32 i;
  
  for (i = 1; i < 128; i++) {
    if ((debtab[i] != NULL) && (strcmp((debtab[i])->id,name) == 0)) return i;
  }
    return -1;
}

struct nuvec_s* CutoffCameraVec;

//NGC MATCH
void DebrisRegisterCutoffCameraVec(struct nuvec_s *cutoff) {
  CutoffCameraVec = cutoff;
  return;
}

//NGC MATCH
float CameraEmitterDistance(struct nuvec_s *vec) {
  if (CutoffCameraVec == NULL) {
    return 0.0f;
  }
  else {
    return NuVecDist(vec,CutoffCameraVec,NULL);
  }
}

