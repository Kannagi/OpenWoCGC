#include "../nu.h"

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

