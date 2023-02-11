#include "gstex.h"



void GS_TexInit(void)

{
  int iVar1;
  int iVar2;
  
  if (GS_TexInitFlag == 0) {
    iVar2 = 4;
    iVar1 = 0;
    do {
      *(undefined4 *)((int)&GS_TexWrapMode_t + iVar1) = 1;
      *(undefined4 *)((int)&GS_TexWrapMode_s + iVar1) = 1;
      DAT_80214b70 = 0;
      iVar1 = iVar1 + 4;
      iVar2 = iVar2 + -1;
    } while (iVar2 != 0);
    GS_TexList = (_GS_TEXTURE *)malloc(0x13000);
    memset(GS_TexList,0,0x13000);
    GS_NumTextures = 0;
    GS_TexInitFlag = 1;
  }
  return;
}


// Re-initialize textures.
void GS_TexReInit(void)		// TODO!!!

{
  int i;
  _GS_TEXTURE *GSTex;
  int j;
  
  GSTex = GS_TexList;
  if (GS_TexInitFlag != 0) {
    j = 4;
    i = 0;
    do {
      *(undefined4 *)((int)&GS_TexWrapMode_t + i) = 1;
      *(undefined4 *)((int)&GS_TexWrapMode_s + i) = 1;
      DAT_80214b70 = 0;
      i = i + 4;
      j = j + -1;
    } while (j != 0);
    i = 0x400;
    do {
      if (GSTex->Flags == -1) {
        free((void *)GSTex->TexBits);
      }
      GSTex = GSTex + 1;
      i = i + -1;
    } while (i != 0);
  }
  memset(GS_TexList,0,0x13000);
  GS_TexAllocs = 0;
  GS_NumTextures = 0;
  return;
}


void GS_TexSwizzleRGB5A3(int width,int height,void *param_3,void *param_4)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  uint *puVar5;
  int iVar6;
  int iVar7;
  ushort uVar8;
  uint uVar9;
  int iVar10;
  int iVar11;
  int iVar12;
  
  iVar2 = width;
  if (width < 0) {
    iVar2 = width + 3;
  }
  if (height < 0) {
    height = height + 3;
  }
  iVar4 = 0;
  iVar6 = 0;
  if (0 < height >> 2) {
    do {
      iVar10 = 0;
      iVar11 = iVar6 + 1;
      if (0 < iVar2 >> 2) {
        do {
          iVar1 = iVar10 * 4;
          iVar7 = 0;
          iVar10 = iVar10 + 1;
          do {
            iVar3 = iVar6 * 4 + iVar7;
            iVar7 = iVar7 + 1;
            iVar12 = 4;
            puVar5 = (uint *)((iVar1 + iVar3 * width) * 4 + (int)param_3);
            do {
              uVar9 = *puVar5;
              puVar5 = puVar5 + 1;
              uVar8 = (ushort)(uVar9 >> 0x10);
              if (uVar9 >> 0x1d == 7) {
                uVar8 = uVar8 >> 3 & 0x1f | (ushort)(uVar9 >> 6) & 0x3e0 | 0x8000 |
                        (ushort)((uVar9 & 0xf8) << 7);
              }
              else {
                uVar8 = uVar8 >> 4 & 0xf | (ushort)(uVar9 >> 8) & 0xf0 |
                        (ushort)((uVar9 & 0xf0) << 4) | uVar8 >> 1 & 0x7000;
              }
              *(char *)((int)param_4 + iVar4) = (char)(uVar8 >> 8);
              *(char *)((int)param_4 + iVar4 + 1) = (char)uVar8;
              iVar4 = iVar4 + 2;
              iVar12 = iVar12 + -1;
            } while (iVar12 != 0);
          } while (iVar7 < 4);
        } while (iVar10 < iVar2 >> 2);
      }
      iVar6 = iVar11;
    } while (iVar11 < height >> 2);
  }
  return;
}



void GS_TexCreateNU(nutextype_e type,int width,int height,void *bits,int mmcnt,int rendertargetfl ag, int tpid)

{
  void *mmalloc;
  _GXTexObj *obj;
  GXTexFmt format;
  int i;
  _GS_TEXTURE *texlist;
  size_t size;
  
  texlist = GS_TexList;
  if (iss3cmp == 0) {
    if (type == 0x80) {
      mmalloc = malloc(mmcnt);
      GS_TexAllocs = GS_TexAllocs + mmcnt;
      memcpy(mmalloc,bits,mmcnt);
      DCFlushRange(mmalloc,mmcnt);
      for (i = 0; i < 0x400; i = i + 1) {
        if (texlist->Flags == 0) {
          texlist->Pad = 0xe;
          obj = &texlist->Tex;
          texlist->Format = 0x80;
          texlist->NUID = tpid;
          texlist->Width = width;
          texlist->Height = height;
          format = GX_CTF_RA8;
          goto LAB_800cbc0c;
        }
        texlist = texlist + 1;
      }
    }
    else if (type == 0x81) {
      size = width * height * 2;
      mmalloc = malloc(size);
      GS_TexAllocs = GS_TexAllocs + size;
      DCFlushRange(bits,size);
      memcpy(mmalloc,bits,size);
      DCFlushRange(mmalloc,size);
      for (i = 0; i < 0x400; i = i + 1) {
        if (texlist->Flags == 0) {
          texlist->Pad = 5;
          obj = &texlist->Tex;
          texlist->Format = 0x81;
          texlist->NUID = tpid;
          texlist->Width = width;
          texlist->Height = height;
          format = GX_TF_RGB565;
          goto LAB_800cbc0c;
        }
        texlist = texlist + 1;
      }
    }
    else if (type == 0x82) {
      size = width * height * 4;
      mmalloc = malloc(size);
      GS_TexAllocs = GS_TexAllocs + size;
      DCFlushRange(bits,size);
      memcpy(mmalloc,bits,size);
      DCFlushRange(mmalloc,size);
      for (i = 0; i < 0x400; i = i + 1) {
        if (texlist->Flags == 0) {
          texlist->Pad = 6;
          obj = &texlist->Tex;
          texlist->Format = 0x82;
          texlist->NUID = tpid;
          texlist->Width = width;
          texlist->Height = height;
          format = GX_TF_RGB5A3;
          goto LAB_800cbc0c;
        }
        texlist = texlist + 1;
      }
    }
    else {
      size = width * height * 2;
      mmalloc = malloc(size);
      GS_TexAllocs = GS_TexAllocs + size;
      DCFlushRange(bits,width * height * 4);
      GS_TexSwizzleRGB5A3(width,height,bits,mmalloc);
      DCFlushRange(mmalloc,size);
      for (i = 0; i < 0x400; i = i + 1) {
        if (texlist->Flags == 0) {
          texlist->Pad = 5;
          texlist->Format = type;
          texlist->NUID = tpid;
          texlist->Width = width;
          texlist->Height = height;
          texlist->TexBits = (uint)mmalloc;
          texlist->Flags = -1;
          GXInitTexObj(&texlist->Tex,mmalloc,width & 0xffff,height & 0xffff,GX_TF_RGB565,GX_CLAMP,
                       GX_CLAMP,'\0');
          break;
        }
        texlist = texlist + 1;
      }
    }
  }
  else {
    mmalloc = malloc(iss3cmp);
    GS_TexAllocs = GS_TexAllocs + iss3cmp;
    memcpy(mmalloc,(void *)((int)bits + 0xc),iss3cmp);
    DCFlushRange(mmalloc,iss3cmp);
    for (i = 0; i < 0x400; i = i + 1) {
      if (texlist->Flags == 0) {
        texlist->Pad = 0xe;
        obj = &texlist->Tex;
        texlist->Format = type;
        texlist->NUID = tpid;
        texlist->Width = width;
        texlist->Height = height;
        format = GX_CTF_RA8;
LAB_800cbc0c:
        texlist->TexBits = (uint)mmalloc;
        texlist->Flags = -1;
        GXInitTexObj(obj,mmalloc,width & 0xffff,height & 0xffff,format,GX_CLAMP,GX_CLAMP,'\0');
        break;
      }
      texlist = texlist + 1;
    }
  }
  GS_NumTextures = GS_NumTextures + 1;
  return;
}


void GS_ChangeTextureStates(int id)

{
  uint i;
  _GS_TEXTURE *texlist;
  
  i = 0;
  texlist = GS_TexList;
  if (GS_NumTextures != 0) {
    do {
      if (texlist->NUID == TexStages[id] - 1U) {
        GXInitTexObjWrapMode(&texlist->Tex,(&GS_TexWrapMode_s)[id],(&GS_TexWrapMode_t)[id]);
        GXLoadTexObj(&texlist->Tex,id);
        return;
      }
      i = i + 1;
      texlist = texlist + 1;
    } while (i < GS_NumTextures);
  }
  return;
}


void GS_TexSetWrapModes(int id,GXTexWrapMode mode)

{
  if (id < 4) {
    (&GS_TexWrapMode_s)[id] = mode;
    GS_ChangeTextureStates(id);
  }
  return;
}

void GS_TexSetWrapModet(int id,GXTexWrapMode mode)

{
  if (id < 4) {
    (&GS_TexWrapMode_t)[id] = mode;
    GS_ChangeTextureStates(id);
  }
  return;
}


void GS_TexSelect(_GXTevStageID stage,int NUID)

{
  int iVar1;
  uint uVar2;
  _GS_TEXTURE *GSTex;
  bool check;
  
  check = stage == GX_TEVSTAGE0;
  if (check) {
    ShadowBodge = stage;
  }
  if (3 < (int)stage) {
    DisplayErrorAndLockup("C:/source/crashwoc/code/system/gc/gstex.c",0x21c,"GS_TexSelect1");
  }
  TexStages[stage] = NUID;
  GSTex = GS_TexList;
  if ((NUID == 0) || (NUID == 9999)) {
    GXSetNumTevStages('\x01');
    GXSetTevOrder(stage,GX_TEXCOORD_NULL,GX_TEXMAP_NULL,GX_COLOR0A0);
    GXSetTevOp(stage,GX_PASSCLR);
  }
  else {
    if (NUID == ShadowMatBodge) {
      ShadowBodge = GX_TEVSTAGE1;
    }
    if ((int)maxstage.189 < (int)stage) {
      maxstage.189 = stage;
    }
    if (check) {
      maxstage.189 = stage;
    }
    if (NUID == 0x270e) {
      GS_SetFBCopyTexturePause();
    }
    GXSetNumTexGens((char)maxstage.189 + '\x01');
    GXSetNumTevStages((char)maxstage.189 + '\x01');
    GXSetTexCoordGen2(stage,GX_TG_MTX2x4,GX_TG_TEX0,0x3c,'\0',0x7d);
    GXSetTevOrder(stage,stage,stage,GX_COLOR0A0);
    iVar1 = 0;
    if (check) {
      iVar1 = 10;
    }
    GXSetTevColorIn(stage,0xf,8,iVar1,0xf);
    GXSetTevColorOp(stage,0,0,0,1,0);
    iVar1 = 0;
    if (check) {
      iVar1 = 5;
    }
    GXSetTevColorIn(stage,7,4,iVar1,7);
    GXSetTevAlphaOp(stage,0,0,0,1,0);
    if (1 < NUID - 0x270eU) {
      uVar2 = 0;
      if (GS_NumTextures != 0) {
        do {
          if (GSTex->NUID == NUID - 1U) {
            GXInitTexObjWrapMode(&GSTex->Tex,(&GS_TexWrapMode_s)[stage],(&GS_TexWrapMode_t)[stage] );
            GXLoadTexObj(&GSTex->Tex,stage);
            return;
          }
          uVar2 = uVar2 + 1;
          GSTex = GSTex + 1;
        } while (uVar2 < GS_NumTextures);
      }
      DisplayErrorAndLockup("C:/source/crashwoc/code/system/gc/gstex.c",0x281,"GS_TexSelect2");
      GXLoadTexObj(&GS_TexList->Tex,stage);
    }
  }
  return;
}

void GS_SetTevBlend(_GXTevStageID id)

{
  GXSetTevColorIn(id,8,0xf,0xf,10);
  GXSetTevColorOp(id,0,0,0,1,0);
  return;
}

void GS_SetTevModulate(_GXTevStageID id)

{
  GXSetTevOp(id,GX_MODULATE);
  return;
}

void GS_SetTextureStageState(void)

{
  return;
}