// This file is original in C++, but I'll try and make it in C for consistency.
#include "dxframe.h"

// If the backbuffer has been grabbed this frame.
s32 backbuffer_grabbed_this_frame = 0;
int iss3cmp;
char DebugText[256];

// Back buffer TID.
s32 backbuffer_tid = 0x207F;

void ResetFwGlobals()
{
	g_pddsZBuffer = NULL;
	g_dwRenderWidth = 0;
	g_dwRenderHeight = 0;
	g_pd3dDevice = NULL;
	g_pddsBackBuffer = NULL;
	InitRenderTargets();
  	return;
}


void InitRenderTargets(void)

{
  int last;
  int next;
  rendertargetlist_s *tglist;
  int i;

  memset(g_pRTArray,0,0x1c0);
  g_pRTArray[0].last = -1;
  g_pRTArray[0].next = 1;
  i = 0xe;
  next = 1;
  tglist = g_pRTArray;
  do {
    last = next;
    next = last + 1;
    tglist[1].last = last + -1;
    tglist[1].next = next;
    i = i + -1;
    tglist = tglist + 1;
  } while (i != 0);
  g_pRTArray[next].last = last;
  g_pRTArray[next].next = -1;
  g_dwNumRT = 0;
  g_dwCurrentRT = -1;
  g_dwFreeRT = 0;
  g_dwAllocRT = -1;
  return;
}


void initd3d(void)

{
  NudxFw_Init(NULL,NULL,0);
  return;
}

void NudxFw_DestroyBackBufferCopy(void)

{
  return;
}

void NudxFw_MakeBackBufferCopy(s32 force)
{
	if ((backbuffer_grabbed_this_frame == 0) || (force != 0))
	{
		//GS_CopyTextureFromFB(); This is a stub function. Is it supposed to do something?
		NudxFw_SetBackBufferCopied(1);
	}
	return;
}



s32 NudxFw_GetBackBufferCopyTID()
{
	return backbuffer_tid;
}


void NudxFw_SetBackBufferCopied(s32 copied)
{
	backbuffer_grabbed_this_frame = copied;
}


s32 NudxFw_Init()
{
	ResetFwGlobals();
	s32 code = CreateEnvironment(NULL);
	if (code > -1)
	{
		code = 0;
	}
	return code;
}


s32 NudxFw_SetRenderTargetSurface(struct D3DSurface *RenderTarget,struct D3DSurface *Zbuffer)

{
  return 0;
}


s32 NudxFw_BeginScene(s32 hRT)

{
  if ((hRT + -1 != g_dwCurrentRT) || (hRT != -1)) {
    SetRenderTarget(hRT);
  }
  //GS_BeginScene();
  return 0;
}

s32 SetRenderTarget(u32 hRT)

{
  return 0;
}

s32 NudxFw_Clear(s32 flags,s32 colour,float depth)

{
  //GS_RenderClear(flags,colour,depth,0);
  return 0;
}

int NudxFw_FlipScreen(int hRT,int ss)

{
  NudxFw_MakeBackBufferCopy(0);
  GS_FlipScreen();
  GS_RenderClear(3,0,1.0,0);
  if (hLoadScreenThread == NULL) {
    NuAnimUV();
  }
  return 0;
}


s32 NudxFw_SetRenderState(enum _D3DRENDERSTATETYPE state, u32 data)

{
  //GS_SetRenderState(state,data);	//Empty function
  return 0;
}

s32 NudxFw_SetTextureState(u32 stage, enum _D3DTEXTURESTAGESTATETYPE state, u32 data)
{
  //GS_SetTextureStageState(); //Empty function
  return 0;
}

struct D3DSurface * NudxFw_GetBackBuffer(void)

{
  return g_pddsBackBuffer;
}


struct D3DSurface * NudxFw_GetZBuffer(void)

{
  return g_pddsZBuffer;
}



s32 CreateEnvironment(struct HWND__ *hwnd)
{
  s32 RT;
  s32 next;

  next = CreateDirect3D(hwnd);
  if (-1 < next) {
    GetFullscreenBuffers(g_pd3dDevice);
    RT = g_dwNumRT;
    next = g_pRTArray[0].next;
    if (g_dwNumRT == 0) {
      g_pRTArray[0].pddsStencilBuffer = g_pddsStencilBuffer;
      g_pRTArray[0].width = g_dwRenderWidth;
      g_pRTArray[0].height = g_dwRenderHeight;
      g_pRTArray[0].pddsRenderTarget = g_pddsBackBuffer;
      g_pRTArray[0].pddsZBuffer = g_pddsZBuffer;
      g_pRTArray[g_pRTArray[0].next].last = -1;
      g_dwNumRT = 1;
      g_pRTArray[0].next = g_dwAllocRT;
      g_dwFreeRT = next;
      if (g_dwAllocRT != -1) {
        g_pRTArray[g_dwAllocRT].last = 0;
      }
      g_dwAllocRT = RT;
      next = 0;
    }
    else {
      next = -0x7e000000;
    }
  }
  return next;
}

s32 GetFullscreenBuffers(struct D3DDevice *lpdev)

{
  g_rcScreenRect.left = 0;
  g_rcScreenRect.right = 0x280;
  g_dwRenderWidth = 0x280;
  g_dwRenderHeight = 0x1e0;
  g_rcScreenRect.top = 0;
  g_rcScreenRect.bottom = 0x1e0;
  return 0;
}

s32 CreateDirect3D(struct HWND__ *hwnd)

{
  struct _GS_VIEWPORT gsvp;
  struct _D3DVIEWPORT8 vp;

  //GS_RenderClear(1,0,1.0,0);
  //GS_RenderClear(2,0,1.0,0);
  //GS_RenderClear(0xf0,0,0.0,0);
  GS_FlipScreen();
  //GS_RenderClear(0xf0,0,0.0,0);
  GS_FlipScreen();
  memset(&vp,0,0x18);
  vp.Height = 0x1c0;
  vp.Width = PHYSICAL_SCREEN_X;
  vp.MaxZ = 1.0;
  gsvp.X = vp.X;
  gsvp.Y = vp.Y;
  gsvp.width = PHYSICAL_SCREEN_X;
  gsvp.height = 0x1c0;
  gsvp.MaxZ = 1.0;
  gsvp.MinZ = vp.MinZ;
  //GS_SetViewport(&gsvp);
  return 0;
}



//84% WIP
struct D3DTexture* NudxTx_Create(struct nutex_s* texture, s32 rendertargetflag) {
    long data;
    long data2;
    unsigned char* inbits24;
    unsigned short* inbits16;
    long* inbits32;
    long* outbits;
    long* outline;
    unsigned short* outline16;
    unsigned char* inbits8;
    int s;
    int t;
    unsigned char* b1;
    int imagesize; 
    int mapix;
    int ix;
    int format;
    int solid;
    int blended;
    int transparent;
    enum nutextype_e type;
    int width;
    int height;
    u8* bits; // void
    int* pal;
    float alphatest;

    int iss3;
    
    u8 *pbVar1;
    u8 *pbVar2;
    char cVar3;
    char bVar4;
    float fVar5; //alphatest ?
    u32 uVar6;
    //short sVar7;
    s32 unkcnt;
    // long* outbits;
    long* px_buffer;
    u32 uVar8;
    char *puVar9;
    long *puVar10; //register 10?
    long *puVar11;
    long *piVar9;
    u32 uVar10;
    u32 uVar11;
    s32 unkcnt2;
    
    //tempcmp =  iss3cmp;
    type = texture->type;
    width = texture->width;
    height = texture->height;
    bits = texture->bits;
    pal = texture->pal;
    iss3 = iss3cmp;
    
    if (iss3 != 0) {
        GS_TexCreateNU(type, width, height, bits, texture->mmcnt, rendertargetflag, GetTPID());
        return NULL;
    }
    
    if (type == 0x82) {
        GS_TexCreateNU(0x82, width, height, bits, texture->mmcnt, rendertargetflag, GetTPID());
        return NULL;
    }
    
    if (type == 0x81) {
        GS_TexCreateNU(0x81, width, height, bits, texture->mmcnt, rendertargetflag, GetTPID());
        return NULL;
    }
    
    if (type == 0x80) {
        GS_TexCreateNU(0x80, width, height, bits, texture->mmcnt, rendertargetflag, GetTPID());
        return NULL;
    }
    
    unkcnt = width * height;
    inbits16 = (u16*)bits;
    inbits24 = (s8*)bits;
    inbits32 = (long*)bits;
    inbits8 = (s8*)bits;
    //uVar11 = 0;
    //uVar10 = 0;
    unkcnt2 = 0;
    bits = (void* )malloc_x(unkcnt * 4);
    texture->decal = iss3;
    texture->linear = iss3;
    px_buffer = bits;
    
    switch (type) {
        case NUTEX_RGB16:
        case NUTEX_RGBA16:
            sprintf(DebugText, "RGB16/RGBA16");
            
            for(s = 0; s < height; s++)
            {
                u8* puVar10 = px_buffer;
                for(t = 0; t < width; t++) {
                     u8 tmp1;
                    u8 tmp3;
                    data = *inbits16;
                    
                    //bitwise transformation on uVar8, rearranging and multiplying specific bits from different positions to create a new value
                    //uVar8 & 0x1f, the mask gets you the lowest 5 bits of uVar8
                    //with uVar8 & 0x3e0 you get the next 5 bits above that; uVar8 & 0x7c00 you get the next 5 after that
                    uVar8 = ((data & 0x1f) << 3) | ((data & 0x3e0) << 6) | ((data & 0x7c00) << 9);
                    if ((data & 1) != 0) {
                        // set the highest 8 bits (bits 24 to 31) of uVar8 to 1 while keeping the other bits unchanged
                        uVar8 |= 0xff000000;
                    }
                    
                    *(u32*)puVar10 = uVar8;
                    tmp1 = puVar10[1];
                    tmp3 = puVar10[3];
                    puVar10[3] = tmp1;
                    puVar10[1] = tmp3;
                    puVar10 += 4;
                    inbits16++;
                }
                px_buffer = px_buffer + width;
            }
            break;
        case NUTEX_RGB24:
            sprintf(DebugText, "RGB24"); // print texture type we're decoding
    
            for(unkcnt = 0; unkcnt < height; unkcnt++)
            {
                //puVar9 = (char *)inbits24; // puVar9 is output buffer for this row of pixels
                u8* puVar10 = px_buffer; // Pushing px_buffer into local puVar10 (What makes puVar10?)
                for(t = 0; t < width; t++) {
                    u8 tmp1;
                    u8 tmp3;

                    // NUTEXes seems to be little-endian based on this code
                    uVar8  = (*inbits24++ << 0x00); // the blue component
                    uVar8 += (*inbits24++ << 0x08); // the green component
                    uVar8 += (*inbits24++ << 0x10); // the red component
                    uVar8 += 0xFF000000; // set the upper two bytes to FF which means solid alpha
                    
                    *(u32*)puVar10 = uVar8;

                    // swaps blue and red components
                    tmp1 = puVar10[1];
                    tmp3 = puVar10[3];
                    
                    // finishes swap, resulting in a big-endian ARBG color
                    puVar10[3] = tmp1;
                    puVar10[1] = tmp3;
                    puVar10 += 4;
                }
                px_buffer = px_buffer + width;
            }
            break;
        case NUTEX_RGBA32:
            sprintf(DebugText,"RGB32");
            
            px_buffer = bits;
            
            for(unkcnt = 0; unkcnt < height; unkcnt++)
            {
                s8* puVar10 = px_buffer;
                for(t = 0; t < width; t++) {
                    *(u32*)puVar10 = *inbits32;
                    fVar5 = puVar10[3] / 255.0f;
                    
                    if (fVar5 < 0.9f) {
                        uVar10 = uVar10 + 1;
                    }
                    else if (0.063f < fVar5) {
                        uVar11 = uVar11 + 1;
                    }
                    else {
                        unkcnt2 = unkcnt2 + 1;
                    }
                    
                    puVar10 += 4;
                    inbits32++;
                }
                px_buffer = px_buffer + width;
            }
            break;
        case NUTEX_PAL4:
        case NUTEX_PAL8:
            if (type == NUTEX_PAL4) {
                sprintf(DebugText, "PAL4");
            } else if (type == NUTEX_PAL8) {
                sprintf(DebugText, "PAL8");
            }
            
            // piVar9 = bits;
            {
        
                s8* puVar10 = px_buffer;
                for(s = 0; s < unkcnt;s++) {
                    // uVar6 = (u32)bits[s];
                    if (type == NUTEX_PAL4) {
                        if ((s & 1) != 0) {
                            uVar6 = bits[s / 2] >> 4;
                        }
                        else {
                            uVar6 = bits[s / 2] & 0xf;
                        }
                    } else if (type == NUTEX_PAL8) {
                        uVar6 = bits[s];
                    }
                    
                    *(u32*)puVar10 = pal[uVar6];
                    
                    fVar5 = puVar10[0] / 255.0f;
                    
                    if (fVar5 < 0.9f) {
                        uVar10 = uVar10 + 1;
                    }
                    else if (0.063f < fVar5) {
                        uVar11 = uVar11 + 1;
                    }
                    else {
                        unkcnt2 = unkcnt2 + 1;
                    }
                    
                    puVar10 += 4;
                }
            }
            break;
        default:
            NuErrorProlog("C:/source/crashwoc/code/system/crashlib.c", 0x187)("NudxTx_Create:\tUnknown texture type!");
            break;
    }
    
    if (((uVar11 == 0) || (uVar10 ==  0)) || (unkcnt2 != 0)) {
        texture->decal = 0;
    }
    else if (0.6f > (float)uVar10/(float)uVar11)
    {
        texture->decal = 1;
    }
    
    GS_TexCreateNU(type, texture->width, texture->height, &outbits, texture->mmcnt, rendertargetflag, GetTPID());
    free_x(&outbits);
    DebugText[0] = '\0';
    return NULL;
}