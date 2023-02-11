#include "gs.h"


void GS_SetFBCopyTexturePause(void)

{
  GXLoadTexObj(&GS_FrameBufferCopyPause.tobj,GX_TEXMAP0);
  return;
}

void GS_SetAlphaCompareForce(int param_1)

{
  GS_ForceNoAlphaCompareFlag = param_1;
  if (param_1 != 0) {
    GXSetAlphaCompare(GX_GREATER,'\0',GX_AOP_OR,GX_GREATER,'\0');
  }
  return;
}

void GS_SetAlphaCompare(int Func,int Ref)

{
  if (GS_ForceNoAlphaCompareFlag == 0) {
    GXSetAlphaCompare(Func,(uchar)Ref,GX_AOP_OR,Func,(uchar)Ref);
  }
  else {
    GXSetAlphaCompare(GX_ALWAYS,'\0',GX_AOP_OR,GX_ALWAYS,'\0');
  }
  return;
}

void GS_ARGBTORGBA(uint *colors,int count)

{
  if (count < 1) {
    return;
  }
  do {
    colors[6] = colors[6] << 8 | colors[6] >> 0x18;
    colors = colors + 9;
    count = count + -1;
  } while (count != 0);
  return;
}


void GS_BeginScene(void)

{
  ulong field;
  
  GS_WorldMatIsIdentity = 0;
  GS_SetZCompare(1,1,7);
  field = 1;
  GS_SetBlendSrc(1,1,0);
  if (GS_IsNewFrame != 0) {
    DEMOBeforeRender();
  }
  GS_IsNewFrame = 0;
  GXSetViewport(0.0,0.0,640.0,448.0,0.0,1.0,field);
  GXSetScissor(0,0,0x280,0x1c0);
  GXSetClipMode(GX_CLIP_ENABLE);
  GS_SetBlendSrc(0,1,0);
  GXSetZCompLoc('\0');
  GXSetAlphaUpdate('\0');
  GXSetCullMode(0);
  GS_SetZCompare(1,1,3);
  return;
}


void GS_EndScene(void)

{
  DEMODoneRender();
  return;
}


int GS_Init(void)

{
  int i;
  _GXRenderModeObj *rmp;
  _GXColor GXCol [3];
  
  GXInvalidateTexAll();
  GS_TexInit();
  GXCol[0] = GS_BgColour;
  GXSetCopyClear(GXCol,0xffffff);
  i = DEMOGetCurrentBuffer();
  GXCopyDisp((void *)i,'\x01');
  rmp = DEMOGetRenderModeObj();
  GS_ScreenHeight = (int)rmp->efbHeight;
  GS_ScreenWidth = (int)rmp->fbWidth;
  GS_InitVertexDescriptors();
  GS_InitXForm();
  DemoStatEnable = '\0';
  GS_FrameBufferCopyPause.width = 0x140;
  GS_FrameBufferCopyPause.height = 0xe0;
  GS_FrameBufferCopyPause.data = (uchar *)0x0;
  GS_FrameBufferCopyPause.top = 0;
  GS_FrameBufferCopyPause.left = 0;
  CARDInit();
  GBA_Init();
  GS_FrameBufferCopySize =
       GXGetTexBufferSize(GS_FrameBufferCopyPause.width._2_2_,GS_FrameBufferCopyPause.height._2_2 _,4
                          ,'\0','\0');
  GS_FrameBufferCopyPause.data = (uchar *)malloc(GS_FrameBufferCopySize);
  if (GS_FrameBufferCopyPause.data == (uchar *)0x0) {
    DisplayErrorAndLockup
              ("C:/source/crashwoc/code/system/gc/gs.c",0x182,
               "Out Of Memory - Frame Buffer Copy Pause");
  }
  GS_FrameBufferCopydataptr = GS_FrameBufferCopyPause.data;
  GS_LoadWorldMatrixIdentity();
  return 0;
}


void GS_RenderClear(ulong Flags,ulong Color,float Z,ulong Stencil)

{
  _GXColor bgcol [2];
  
  if ((Flags & 0xf0) != 0) {
    if (fadeval == 0xff) {
      GS_BgColour = (_GXColor)0x0;
    }
    else {
      GS_BgColour._2_2_ = CONCAT11((char)Color,(char)(Color >> 0x18));
      GS_BgColour = (_GXColor)((Color >> 8) << 0x10 | (uint)GS_BgColour._2_2_);
    }
    bgcol[0] = GS_BgColour;
    GXSetCopyClear(bgcol,0xffffff);
  }
  return;
}


void GS_RenderClearBlack(void)

{
  _GXColor clr_col [2];
  
  clr_col[0] = GS_BgColourBlack;
  GXSetCopyClear(clr_col,0xffffff);
  return;
}

void GS_SetOrthMatrix(void)

{
  DEMOSetupScrnSpc(GS_ScreenWidth,GS_ScreenHeight,100.0);
  return;
}


void GS_DrawFade(int fadecol)

{
  GS_SetOrthMatrix();
  GS_SetZCompare(0,0,7);
  GS_SetBlendSrc(1,0,3);
  GS_CurrentVertDesc = 0;
  GXClearVtxDesc();
  GXSetVtxDesc(GX_VA_POS,GX_DIRECT);
  GXSetVtxDesc(GX_VA_CLR0,GX_DIRECT);
  GXSetChanCtrl(GX_COLOR0A0,'\0',GX_SRC_VTX,GX_SRC_VTX,0,GX_DF_NONE,GX_AF_NONE);
  GXSetNumChans(1);
  GXSetTevOrder(GX_TEVSTAGE0,GX_TEXCOORD_NULL,GX_TEXMAP_NULL,GX_COLOR0A0);
  GXSetTevOp(GX_TEVSTAGE0,GX_PASSCLR);
  GXSetNumTexGens('\0');
  GXSetNumTevStages('\x01');
  GXBegin(GX_QUADS,GX_VTXFMT7,4);
  _DAT_cc008000 = fadecol & 0xff000000;
  return;
}

void GS_FlipScreen(void)

{
  size_t sVar1;
  int iVar2;
  undefined4 uVar3;
  char *pcVar4;
  double fps;
  
  if (GS_IsNewFrame != 0) {
    DEMOBeforeRender();
  }
  GS_IsNewFrame = 0;
  if (PrintError == 0) {
    if (Lockupbuffer1[0] == '\0') {
      TTLLights();
      GS_SetZCompare(1,1,7);
      goto LAB_800c9244;
    }
    TTLLights();
    GS_SetZCompare(1,1,7);
    GS_SetBlendSrc(1,1,0);
    DEMOInitCaption(0,0x280,0x1c0);
    DEMOPrintf(10,0xf,0,"FATAL ERROR - GAME HALTED");
    DEMOPrintf(10,0x19,0,"REPORT THE FOLLOWING MESSAGE AND HOW TO REPRODUCE");
    DEMOPrintf(10,0x2d,0,Lockupbuffer1);
    DEMOPrintf(10,0x37,0,Lockupbuffer2);
    DEMOPrintf(10,0x41,0,Lockupbuffer3);
    iVar2 = 10;
    pcVar4 = namesave;
    uVar3 = 0x4b;
  }
  else {
    TTLLights();
    GS_SetZCompare(1,1,7);
    GS_SetBlendSrc(1,1,0);
    DEMOInitCaption(0,0x280,0x1c0);
    sVar1 = strlen(Lockupbuffer1);
    DEMOPrintf((int)(short)(0x140 - (short)(sVar1 << 2)),0x91,0,Lockupbuffer1);
    sVar1 = strlen(Lockupbuffer2);
    DEMOPrintf((int)(short)(0x140 - (short)(sVar1 << 2)),0x9b,0,Lockupbuffer2);
    sVar1 = strlen(Lockupbuffer3);
    pcVar4 = Lockupbuffer3;
    uVar3 = 0xa5;
    iVar2 = (int)(short)(0x140 - (short)(sVar1 << 2));
  }
  DEMOPrintf(iVar2,uVar3,0,pcVar4);
LAB_800c9244:
  DBTimerReset();
  DEMODoneRender();
  DEMOSwapBuffers();
  fps = TimerGetFPS();
  avfps = (avfps + (float)fps) * 0.5;
  GS_IsNewFrame = 1;
  nuvideo_global_vbcnt = nuvideo_global_vbcnt + 1;
  GS_WorldMatIsIdentity = 0;
  GS_CurrentTexture = -1;
  cufps = (float)fps;
  GS_CurrentVertDesc = -1;
  return;
}

void GS_SetViewport(_GS_VIEWPORT *pViewport)

{
  GS_ViewPort.X = pViewport->X;
  GS_ViewPort.Y = pViewport->Y;
  GS_ViewPort.width = pViewport->width;
  GS_ViewPort.height = pViewport->height;
  GS_ViewPort.MaxZ = pViewport->MaxZ;
  GS_ViewPort.MinZ = pViewport->MinZ;
  return;
}

void GS_GetViewport(nuviewport_s *vp)

{
  float MinZ;
  ulong Y;
  ulong height;
  ulong width;
  
  height = GS_ViewPort.height;
  width = GS_ViewPort.width;
  Y = GS_ViewPort.Y;
  vp->x = GS_ViewPort.X;
  vp->y = Y;
  vp->width = width;
  vp->height = height;
  MinZ = GS_ViewPort.MinZ;
  vp->zmax = GS_ViewPort.MaxZ;
  vp->zmin = MinZ;
  return;
}

void GS_SetupFog(int type,float startz,float endz,uint colour)

{
  _GXColor local_8;
  
  local_8 = (_GXColor)
            ((colour & 0xff) << 8 |
            (colour >> 8 & 0xff) << 0x10 | (colour >> 0x10) << 0x18 | colour >> 0x18);
  if (type == 0) {
    GXSetFog(GX_FOG_NONE,0.0,0.0,0.0,0.0,(_GXColor)&local_8);
  }
  else {
    GXSetFog(GX_FOG_LINEAR,startz,endz,0.3,1000.0,(_GXColor)&local_8);
  }
  return;
}


void GS_SetRenderState(int state,int data)

{
  return;
}

void GS_SetZCompare(int enable,int upd,_GXCompare mode)

{
  sprintf(GS_CommandBuffer,"ZCMP En %d Up %d Mo %d\n",enable,upd,mode);
  if (GS_ZCompareMode != mode) {
    GS_ZCompareMode = mode;
  }
  if (GS_ZCmpUpdate != upd) {
    GS_ZCmpUpdate = upd;
  }
  if (GS_ZCmpEnable != enable) {
    GS_ZCmpEnable = enable;
  }
  if (GS_Parallax == 0) {
    GXSetZMode((uchar)GS_ZCmpEnable,GS_ZCompareMode,(uchar)GS_ZCmpUpdate);
  }
  else {
    GXSetZMode('\0',GX_NEVER,'\0');
  }
  return;
}

void GS_SetBlendSrc(int enable,int src,int dest)

{
  sprintf(GS_CommandBuffer,"BLEND En %d Sr %d Ds %d\n",enable,src,dest);
  if (GS_BlendEnable != enable) {
    GS_BlendEnable = enable;
  }
  if (GS_BlendSrc != src) {
    GS_BlendSrc = src;
  }
  if (GS_BlendDest != dest) {
    GS_BlendDest = dest;
  }
  if (GS_BlendEnable == 1) {
    GXSetBlendMode(GX_BM_BLEND,GS_BlendSrc,GS_BlendDest,GX_LO_COPY);
  }
  else {
    GXSetBlendMode(GX_BM_NONE,GX_BL_ONE,GX_BL_ZERO,GX_LO_COPY);
  }
  return;
}

void GS_SetBlendSrc2(int enable,int src,int dest)

{
  GXSetBlendMode(GX_BM_BLEND,GX_BL_SRCCLR,GX_BL_SRCCLR,GX_LO_COPY);
  return;
}

void GS_SetProjectionMatrix(_GSMATRIX *pMatrix)

{
  _GSMATRIX *pfVar2;
  float *pfVar1;
  float *pfVar3;
  int iVar4;
  _GSMATRIX f;
  
  iVar4 = 0x30;
  pfVar1 = (float *)&GS_MatProjection;
  do {
    pfVar3 = (float *)pMatrix;
    pfVar2 = (_GSMATRIX *)pfVar1;
    iVar4 = iVar4 + -0x18;
    pfVar2->mtx[0] = *pfVar3;
    pfVar2->mtx[1] = pfVar3[1];
    pfVar2->mtx[2] = pfVar3[2];
    pfVar2->mtx[3] = pfVar3[3];
    pfVar2->mtx[1][0] = (*(float (*) [4])(pfVar3 + 4))[0];
    pfVar2->mtx[1][1] = pfVar3[5];
    pfVar1 = pfVar2->mtx[1] + 2;
    pMatrix = (_GSMATRIX *)(pfVar3 + 6);
  } while (iVar4 != 0);
  *pfVar1 = pfVar3[6];
  pfVar2->mtx[1][3] = pfVar3[7];
  pfVar2->mtx[2][0] = pfVar3[8];
  pfVar2->mtx[2][1] = pfVar3[9];
  C_MTXPerspective((float *(*) [4])&f,40.0,1.428571);
  GXSetProjection(&f,GX_PERSPECTIVE);
  return;
}

void GS_SetLightingMatrix(_GSMATRIX *mtx)

{
  float (*pafVar1) [4];
  float *pfVar2;
  _GSMATRIX *MatrixA;
  float *pfVar3;
  float *pfVar4;
  int iVar5;
  float *pfVar6;
  undefined local_b0 [96];
  float local_50 [17];
  
  MatrixA = (_GSMATRIX *)local_50;
  iVar5 = 0x30;
  pfVar4 = (float *)local_b0;
  pfVar2 = (float *)MatrixA;
  do {
    pfVar6 = (float *)mtx;
    pfVar3 = pfVar2;
    iVar5 = iVar5 + -0x18;
    *pfVar3 = *pfVar6;
    pfVar3[1] = pfVar6[1];
    pfVar3[2] = pfVar6[2];
    pfVar3[3] = pfVar6[3];
    (*(float (*) [4])(pfVar3 + 4))[0] = (*(float (*) [4])(pfVar6 + 4))[0];
    pfVar3[5] = pfVar6[5];
    pfVar2 = pfVar3 + 6;
    mtx = (_GSMATRIX *)(pfVar6 + 6);
  } while (iVar5 != 0);
  pfVar3[6] = pfVar6[6];
  pfVar3[7] = pfVar6[7];
  pfVar3[8] = pfVar6[8];
  pfVar3[9] = pfVar6[9];
  MatReorder(MatrixA);
  iVar5 = 0x30;
  do {
    iVar5 = iVar5 + -0x18;
    *pfVar4 = MatrixA->mtx[0];
    pfVar4[1] = MatrixA->mtx[1];
    pfVar4[2] = MatrixA->mtx[2];
    pfVar4[3] = MatrixA->mtx[3];
    pfVar4[4] = MatrixA->mtx[1][0];
    pafVar1 = MatrixA->mtx;
    MatrixA = (_GSMATRIX *)(MatrixA->mtx[1] + 2);
    pfVar4[5] = pafVar1[1][1];
    pfVar4 = pfVar4 + 6;
  } while (iVar5 != 0);
  PSMTXInverse((_GSMATRIX *)local_b0,(_GSMATRIX *)(local_b0 + 0x30));
  PSMTXTranspose((float *(*) [4])(_GSMATRIX *)(local_b0 + 0x30),(float *(*) [4])local_b0);
  GXLoadNrmMtxImm(local_b0,0);
  return;
}

void GS_SetLightingMatrix2(_GSMATRIX *m)

{
  float *pfVar1;
  float *pfVar2;
  int iVar3;
  float *pfVar4;
  float local_a0 [40];
  
  iVar3 = 0x30;
  pfVar1 = local_a0;
  do {
    pfVar4 = (float *)m;
    pfVar2 = pfVar1;
    iVar3 = iVar3 + -0x18;
    *pfVar2 = *pfVar4;
    pfVar2[1] = pfVar4[1];
    pfVar2[2] = pfVar4[2];
    pfVar2[3] = pfVar4[3];
    pfVar2[4] = (*(float (*) [4])(pfVar4 + 4))[0];
    pfVar2[5] = pfVar4[5];
    pfVar1 = pfVar2 + 6;
    m = (_GSMATRIX *)(pfVar4 + 6);
  } while (iVar3 != 0);
  pfVar2[6] = pfVar4[6];
  pfVar2[7] = pfVar4[7];
  pfVar2[8] = pfVar4[8];
  pfVar2[9] = pfVar4[9];
  GXLoadNrmMtxImm(local_a0,0);
  return;
}

void GS_LoadMatrix(_GSMATRIX *Matrix)

{
  float *pfVar1;
  _GSMATRIX *pfVar2;
  int iVar2;
  undefined local_30 [48];
  
  pfVar2 = (_GSMATRIX *)local_30;
  iVar2 = 0x30;
  do {
    iVar2 = iVar2 + -0x18;
    pfVar2->mtx[0] = *(float *)Matrix;
    pfVar2->mtx[1] = *(float *)((int)Matrix + 4);
    pfVar2->mtx[2] = *(float *)((int)Matrix + 8);
    pfVar2->mtx[3] = *(float *)((int)Matrix + 0xc);
    pfVar2->mtx[1][0] = (*(float (*) [4])((int)Matrix + 0x10))[0];
    pfVar1 = (float *)((int)Matrix + 0x14);
    Matrix = (_GSMATRIX *)((int)Matrix + 0x18);
    pfVar2->mtx[1][1] = *pfVar1;
    pfVar2 = (_GSMATRIX *)(pfVar2->mtx[1] + 2);
  } while (iVar2 != 0);
  GXSetCurrentMtx((uint)local_30);
  GXSetCurrentMtx(0);
  return;
}

void GS_SetViewMatrix(_GSMATRIX *a)

{
  float *pfVar1;
  float *pfVar2;
  float *pfVar3;
  int iVar4;
  float *pfVar5;
  
  iVar4 = 0x30;
  pfVar1 = (float *)&GS_MatView;
  pfVar2 = (float *)a;
  do {
    pfVar5 = pfVar2;
    pfVar3 = pfVar1;
    iVar4 = iVar4 + -0x18;
    *pfVar3 = *pfVar5;
    pfVar3[1] = pfVar5[1];
    pfVar3[2] = pfVar5[2];
    pfVar3[3] = pfVar5[3];
    (*(float (*) [4])(pfVar3 + 4))[0] = (*(float (*) [4])(pfVar5 + 4))[0];
    pfVar3[5] = pfVar5[5];
    pfVar1 = pfVar3 + 6;
    pfVar2 = pfVar5 + 6;
  } while (iVar4 != 0);
  pfVar3[6] = pfVar5[6];
  pfVar3[7] = pfVar5[7];
  pfVar3[8] = pfVar5[8];
  pfVar3[9] = pfVar5[9];
  SetIdentity(&GS_ViewIdentity);
  MatMult(&GS_ViewIdentity,a);
  MatReorder(&GS_ViewIdentity);
  return;
}

void GS_LoadWorldMatrixIdentity(void)

{
  if (GS_WorldMatIsIdentity == 0) {
    GS_WorldMatIsIdentity = 1;
    GS_LoadMatrix(&GS_ViewIdentity);
  }
  return;
}


void GS_SetWorldMatrix(_GSMATRIX *pMatrix)

{
  float *pfVar1;
  int iVar2;
  float *pfVar3;
  _GSMATRIX *pfVar4;
  float *pfVar5;
  float *pfVar6;
  _GSMATRIX local_48;
  
  iVar2 = 0x30;
  pfVar5 = (float *)&GS_MatWorld;
  do {
    pfVar4 = (_GSMATRIX *)pfVar5;
    pfVar1 = (float *)pMatrix;
    iVar2 = iVar2 + -0x18;
    pfVar4->mtx[0] = *pfVar1;
    pfVar4->mtx[1] = pfVar1[1];
    pfVar4->mtx[2] = pfVar1[2];
    pfVar4->mtx[3] = pfVar1[3];
    pfVar4->mtx[1][0] = (*(float (*) [4])(pfVar1 + 4))[0];
    pfVar4->mtx[1][1] = pfVar1[5];
    pfVar5 = pfVar4->mtx[1] + 2;
    pMatrix = (_GSMATRIX *)(pfVar1 + 6);
  } while (iVar2 != 0);
  iVar2 = 0x30;
  *pfVar5 = pfVar1[6];
  pfVar4->mtx[1][3] = pfVar1[7];
  pfVar4->mtx[2][0] = pfVar1[8];
  pfVar4->mtx[2][1] = pfVar1[9];
  pfVar5 = (float *)&GS_MatWorld;
  pfVar1 = (float *)&local_48;
  do {
    pfVar6 = pfVar1;
    pfVar3 = pfVar5;
    iVar2 = iVar2 + -0x18;
    *pfVar6 = *pfVar3;
    pfVar6[1] = pfVar3[1];
    pfVar6[2] = pfVar3[2];
    pfVar6[3] = pfVar3[3];
    (*(float (*) [4])(pfVar6 + 4))[0] = (*(float (*) [4])(pfVar3 + 4))[0];
    pfVar6[5] = pfVar3[5];
    pfVar5 = pfVar3 + 6;
    pfVar1 = pfVar6 + 6;
  } while (iVar2 != 0);
  pfVar6[6] = pfVar3[6];
  pfVar6[7] = pfVar3[7];
  pfVar6[8] = pfVar3[8];
  pfVar6[9] = pfVar3[9];
  MatMult(&local_48,&GS_MatView);
  MatReorder(&local_48);
  GS_LoadMatrix(&local_48);
  GS_WorldMatIsIdentity = iVar2;
  return;
}


void GS_CopyTextureFromFB(void)

{
  return;
}


void GS_CopyFBToPause(void)

{
  GXSetTexCopySrc(GS_FrameBufferCopyPause.left._2_2_,GS_FrameBufferCopyPause.top._2_2_,0x280,0x1c 0);
  GXSetTexCopyDst(GS_FrameBufferCopyPause.width._2_2_,GS_FrameBufferCopyPause.height._2_2_,
                  GX_TF_RGB565,'\x01');
  GXCopyTex(GS_FrameBufferCopyPause.data,'\0');
  GXPixModeSync();
  GXDrawDone();
  GXInitTexObj(&GS_FrameBufferCopyPause.tobj,GS_FrameBufferCopyPause.data,
               (uint)GS_FrameBufferCopyPause.width._2_2_,(uint)GS_FrameBufferCopyPause.height._2_ 2_,
               GX_TF_IA8,GX_CLAMP,GX_CLAMP,'\0');
  return;
}

