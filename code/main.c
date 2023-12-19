
#include<stdlib.h>
#include<stdio.h>
#include<types.h>
#include<nu3dx/nu3dxtypes.h>
#include "nu3dx.h"
#include<stdbool.h>
#include<SDL/SDL.h>
#define GL_BGR 0x80E0

#include<elf_systypes.h>

#include <GL/gl.h>
#ifdef __MINGW32__
#undef main
#endif

s32 PHYSICAL_SCREEN_X = 640;
s32 PHYSICAL_SCREEN_Y = 480;
s32 IsLoadingScreen;
struct nutex_s tex;
s32 firstscreen1_tid;
struct numtl_s *firstscreen1_mtl;
s32 iss3cmp;
struct nugscn_s* font3d_scene;
struct nuscene_s* font3d_scene2;
char *gerbils;
int test_SDL_openGL();
struct nucamera_s* pNuCam;

void MAHLoadingMessage(void)
{
  IsLoadingScreen = 1;
  NuRndrClear(0xb,0,1.0);
  if (NuRndrBeginScene(1) != 0) {
    memcpy(&pNuCam->mtx, &numtx_identity, sizeof (struct numtx_s));
    NuCameraSet(pNuCam);
    if ((font3d_scene != NULL) && (font3d_initialised != 0)) {
      NuShaderSetBypassShaders(1);
      //DrawGameMessage(tLOADING[Game.language],0,0.0);
      NuShaderSetBypassShaders(0);
    }
    NuRndrEndScene();
  }
  NuRndrSwapScreen(1);
  IsLoadingScreen = 0;
  return;
}

s32 CopyFilesThreadProc() {
    s32 iVar1;
    char texBuf [128];
    u32 iStack_c;

    iVar1 = GetTickCount();
    //InitLevelSfxTables();
    //InitGlobalSfx();
    iStack_c = GetTickCount() - iVar1;
    printf("CopyFilesThreadProc...\n");
    sprintf(texBuf,"Filecopy took %.2f seconds", iStack_c / 1000.0f);
    return 0;
}

int exists_test(const char *fname)
{
    FILE *file;
    s32 d;
    char name[128] = "";

    //strcpy(name, "/");
	strcat(name, fname);
    if ((file = fopen(name, "r")))
    {
        printf("fclose F.E. %s \n", name);
        d = 1;
        fclose(file);
    }
    else{
        d = 0;
    }
    printf("%s exist? %d \n", name, d);
    return d;
}


int main()
{

    printf("\nStarting WoC Engine\n");
        //DEMOInit(NULL); hdw init
        GS_Init(); //gamecube video init , no PS2 I think
        printf("\GS initialized..\n");
        // SS_Init(); sound init
    NuFileInitEx(0,0);
    printf("\nNuFile initialized..\n");


    /* PadData.dwPacketNumber = (uint)GS_FrameBufferCopydataptr;   //useful?
    DefaultGame();  //gamevars set
    ResetGame();    //other gameinit
    NuPs2Init();    //d3dinit
    DebrisMalloc();

    Level = -1;
    Hub = -1;
    MAXVPSIZEX = SWIDTH;
    MAXVPSIZEY = SHEIGHT;
    MINVPSIZEX = (SWIDTH * 390.0) / 640.0);
    MINVPSIZEY = (SHEIGHT * 244.0) / 480.0);

    InitCutScenes();
    */

    printf("NuTrigTable init...\n");
    NuTrigInit();
    printf("NuTex init...\n");
    NuTexInit();
    printf("NuGobj init...\n");
    NuGobjInit();
    printf("NuMtl init...\n");
    NuMtlInit();
    printf("NuRndr init...\n");
    NuRndrInitEx();
  /*  printf("NuLight init...\n");
    NuLightInit();*/
    pNuCam = NuCameraCreate();

    //from firstscreens function
    CopyFilesThreadProc(0);
    gerbils = malloc_x(0x4000c);
    printf("bits allocated \n");
   if (NuFileLoadBuffer("licnin.s3",gerbils,0x2000c) != 0)
   {
       printf("licnin.s3 loaded \n");
   }
   else {
       printf("licnin.s3 not loaded \n");
   }


    tex.width = 0x200;
    tex.height = 0x200;
    tex.decal = 0;
    tex.pal = NULL;
    tex.bits = gerbils;
    tex.mmcnt = 1;
    tex.type = NUTEX_RGB24;
   iss3cmp = 0x20000;
    firstscreen1_tid = NuTexCreate(&tex);
    iss3cmp = 0;
    firstscreen1_mtl = NuMtlCreate(1);

    firstscreen1_mtl->tid = firstscreen1_tid;
    (firstscreen1_mtl->diffuse).r = 1.0f;
    (firstscreen1_mtl->diffuse).g = 1.0f;
    (firstscreen1_mtl->diffuse).b = 1.0f;
    //firstscreen1_mtl->attrib = (struct numtlattrib_s *)((uint)attr & 0xcc0cffff | 0x16e8000);
    firstscreen1_mtl->attrib.cull = 2;
    firstscreen1_mtl->attrib.zmode = 3;
    firstscreen1_mtl->attrib.filter = 4;
    firstscreen1_mtl->attrib.lighting = 2;
    firstscreen1_mtl->attrib.colour = 1;
    firstscreen1_mtl->alpha = 0.999f;
    firstscreen1_mtl->attrib.utc = 1;
    firstscreen1_mtl->attrib.vtc = 1;
    NuMtlUpdate(firstscreen1_mtl);
    firstscreenfade(firstscreen1_mtl,1);
    nuvideo_global_vbcnt = 0;
   do {
        if (NuRndrBeginScene(1) != 0) {
            NuRndrClear(0xb,0,1.0f);
            NuRndrRectUV2di(0,0,PHYSICAL_SCREEN_X,PHYSICAL_SCREEN_Y,0.0f,0.0f,1.0f,1.0f,-1,firstscreen1_mtl);
            NuRndrEndScene();
            NuRndrSwapScreen(1);
        }
        //Reseter(0);
        //GC_DiskErrorPoll();
    } while (nuvideo_global_vbcnt < 0x78);

/*
    nuvideo_global_vbcnt = 0;
    firstscreenfade(firstscreen1_mtl,-1);
    NuRndrClear(0xb,0,1.0f);
    NuRndrSwapScreen(1);
    if (firstscreen1_mtl->tid != 0) {
        NuTexDestroy(firstscreen1_mtl->tid);
    }
*/

    test_SDL_openGL();
    return 0;
}


int test_SDL_openGL()
{

    // Slightly different SDL initialization
    if ( SDL_Init(SDL_INIT_VIDEO) != 0 ) {
        printf("Unable to initialize SDL: %s\n", SDL_GetError());
        return 1;
    }
    else
    {
        printf("\nSDL initialized \n");
    }


    SDL_WM_SetCaption("demo", NULL);
	SDL_Surface *screen = SDL_SetVideoMode(640,480, 16,SDL_OPENGL);
	if ( !screen ) {
		printf("Unable to set video mode: %s\n", SDL_GetError());
	}
    // Set the OpenGL state after creating the context with SDL_SetVideoMode

    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 ); // *new*

	glClearColor( 0, 0, 0, 0 );

	glEnable( GL_TEXTURE_2D ); // Need this to display a texture

    glViewport( 0, 0, 640, 480 );

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();

    glOrtho( 0, 640, 480, 0, -1, 1 );

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    // Load the OpenGL texture

    GLuint texture; // Texture object handle
    SDL_Surface *surface; // Gives us the information to make the texture

    if ( (surface = SDL_LoadBMP("image.bmp")) ) {

        // Check that the image's width is a power of 2
        if ( (surface->w & (surface->w - 1)) != 0 ) {
            printf("warning: image.bmp's width is not a power of 2\n");
        }

        // Also check if the height is a power of 2
        if ( (surface->h & (surface->h - 1)) != 0 ) {
            printf("warning: image.bmp's height is not a power of 2\n");
        }

        // Have OpenGL generate a texture object handle for us
        glGenTextures( 1, &texture );

        // Bind the texture object
        glBindTexture( GL_TEXTURE_2D, texture );

        // Set the texture's stretching properties
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

        // Edit the texture object's image data using the information SDL_Surface gives us
        glTexImage2D( GL_TEXTURE_2D, 0, 3, surface->w, surface->h, 0,
                      GL_BGR, GL_UNSIGNED_BYTE, surface->pixels );
    }
    else {
        printf("SDL could not load image.bmp: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Free the SDL_Surface only if it was successfully created
    if ( surface ) {
        SDL_FreeSurface( surface );
    }

    // Clear the screen before drawing
	glClear( GL_COLOR_BUFFER_BIT );

    // Bind the texture to which subsequent calls refer to
    glBindTexture( GL_TEXTURE_2D, texture );

    float va[8];
    float vt[8];

    float px = 100;
    float py = 200;
    float w = 256;
    float h = 128;


    vt[0] = 0;
	vt[1] = 1;

	vt[2] = 0;
	vt[3] = 0;

	vt[4] = 1;
	vt[5] = 0;

	vt[6] = 1;
	vt[7] = 1;


	va[0] = px;
	va[1] = py;

	va[2] = px;
	va[3] = py-h;

	va[4] = px+w;
	va[5] = py-h;

	va[6] = px+w;
	va[7] = py;


    glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );

	glVertexPointer(2,GL_FLOAT,0,va);
	glTexCoordPointer(2,GL_FLOAT,0,vt);

	glDrawArrays( GL_QUADS, 0,4);

	glDisableClientState( GL_TEXTURE_COORD_ARRAY );
	glDisableClientState( GL_VERTEX_ARRAY );




    SDL_GL_SwapBuffers();

    // Wait for 3 seconds to give us a chance to see the image
    //SDL_Delay(3000);
    bool running = true;
    while(running)
    {
        SDL_Event event;
        //Start event loop
        while(SDL_PollEvent(&event))
        {
            //Handle each specific event
            if(event.type == SDL_QUIT)
            {
                running = false;
            }
        }

    }

    // Now we can delete the OpenGL texture and close down SDL
    glDeleteTextures( 1, &texture );

    SDL_Quit();

	return 0;
}

//NGC MATCH
void firstscreenfade(struct numtl_s *mat,s32 dir) {
    s32 s;
    s32 t;
    u32 colour;
    s32 col;

    if (dir > 0) {
        colour = 0;
        s = 0x10;
    }
    else {
        colour = 0xff;
        s = -0x10;
    }
    for (t = 0; t < 0xe; t++) {
        col = (0xFF << 24) | (colour << 16) | (colour << 8) | colour;
        colour = colour + s;
        if (NuRndrBeginScene(1) != 0) {
            NuRndrClear(0xb,0,1.0f);
            NuRndrRectUV2di(0,0,PHYSICAL_SCREEN_X,PHYSICAL_SCREEN_Y,0.0f,0.0f,1.0f,1.0f,col,mat);
            NuRndrEndScene();
            NuRndrSwapScreen(1);
        }
    }
    return;
}

/*
void firstscreens(void) {
    char *gerbils;
    s32 firstscreen1_tid;
    struct numtl_s *firstscreen1_mtl;
    struct nutex_s tex;

    CopyFilesThreadProc(0);
    gerbils = malloc_x(0x4000c);
    NuFileLoadBuffer("gfx\\licnin.s3",gerbils,0x2000c);
    tex.width = 0x200;
    tex.height = 0x200;
    tex.decal = 0;
    tex.pal = NULL;
    tex.bits = gerbils;
    tex.mmcnt = 1;
    tex.type = NUTEX_RGB24;

    iss3cmp = 0x20000;
    firstscreen1_tid = NuTexCreate(&tex);
    iss3cmp = 0;
    firstscreen1_mtl = NuMtlCreate(1);

    firstscreen1_mtl->tid = firstscreen1_tid;
    (firstscreen1_mtl->diffuse).r = 1.0f;
    (firstscreen1_mtl->diffuse).g = 1.0f;
    (firstscreen1_mtl->diffuse).b = 1.0f;
    //firstscreen1_mtl->attrib = (struct numtlattrib_s *)((uint)attr & 0xcc0cffff | 0x16e8000);
    firstscreen1_mtl->attrib.cull = 2;
    firstscreen1_mtl->attrib.zmode = 3;
    firstscreen1_mtl->attrib.filter = 4;
    firstscreen1_mtl->attrib.lighting = 2;
    firstscreen1_mtl->attrib.colour = 1;
    firstscreen1_mtl->alpha = 0.999f;
    firstscreen1_mtl->attrib.utc = 1;
    firstscreen1_mtl->attrib.vtc = 1;
    NuMtlUpdate(firstscreen1_mtl);
    firstscreenfade(firstscreen1_mtl,1);
    nuvideo_global_vbcnt = 0;
    do {
        if (NuRndrBeginScene(1) != 0) {
            NuRndrClear(0xb,0,1.0f);
            NuRndrRectUV2di(0,0,PHYSICAL_SCREEN_X,PHYSICAL_SCREEN_Y,0.0f,0.0f,1.0f,1.0f,-1,firstscreen1_mtl);
            NuRndrEndScene();
            NuRndrSwapScreen(1);
        }
        Reseter(0);
        GC_DiskErrorPoll();
    } while (nuvideo_global_vbcnt < 0x78);
    nuvideo_global_vbcnt = 0;
    firstscreenfade(firstscreen1_mtl,-1);
    NuRndrClear(0xb,0,1.0f);
    NuRndrSwapScreen(1);
    if (firstscreen1_mtl->tid != 0) {
        NuTexDestroy(firstscreen1_mtl->tid);
    }
    NuFileLoadBuffer("gfx\\copyr1.s3",gerbils,0x4000c);
    tex.width = 0x200;
    tex.height = 0x200;
    tex.decal = 0;
    tex.bits = gerbils;
    tex.mmcnt = 1;
    tex.pal = NULL;
    tex.type = NUTEX_RGB24;
    iss3cmp = 0x40000;
    firstscreen1_tid = NuTexCreate(&tex);
    iss3cmp = 0;
    firstscreen1_mtl = NuMtlCreate(1);

    firstscreen1_mtl->tid = firstscreen1_tid;
    (firstscreen1_mtl->diffuse).r = 1.0f;
    (firstscreen1_mtl->diffuse).g = 1.0f;
    (firstscreen1_mtl->diffuse).b = 1.0f;
    firstscreen1_mtl->attrib.cull = 2;
    firstscreen1_mtl->attrib.zmode = 3;
    firstscreen1_mtl->attrib.filter = 0;
    firstscreen1_mtl->attrib.lighting = 2;
    firstscreen1_mtl->attrib.colour = 1;
    firstscreen1_mtl->alpha = 0.999f;
    firstscreen1_mtl->attrib.utc = 1;
    firstscreen1_mtl->attrib.vtc = 1;
    NuMtlUpdate(firstscreen1_mtl);
    firstscreenfade(firstscreen1_mtl,1);
    nuvideo_global_vbcnt = 0;
    do {
        if (NuRndrBeginScene(1) != 0) {
            NuRndrClear(0xb,0,1.0f);
            NuRndrRectUV2di(0,0,PHYSICAL_SCREEN_X,PHYSICAL_SCREEN_Y,0.0f,0.0f,1.0f,1.0f,-1,firstscreen1_mtl);
            NuRndrEndScene();
            NuRndrSwapScreen(1);
        }
        Reseter(0);
        GC_DiskErrorPoll();
    } while (nuvideo_global_vbcnt < 0x78);
    nuvideo_global_vbcnt = 0;
    firstscreenfade(firstscreen1_mtl,-1);
    NuRndrClear(0xb,0,1.0f);
    NuRndrSwapScreen(1);
    if (firstscreen1_mtl->tid != 0) {
        NuTexDestroy(firstscreen1_mtl->tid);
    }
    NuFileLoadBuffer("gfx\\euro.s3",gerbils,0x2000c);
    tex.width = 0x200;
    tex.height = 0x200;
    tex.decal = 0;
    tex.mmcnt = 1;
    tex.pal = NULL;
    tex.type = NUTEX_RGB24;
    iss3cmp = 0x20000;
    firstscreen1_tid = NuTexCreate(&tex);
    iss3cmp = 0;
    firstscreen1_mtl = NuMtlCreate(1);

    firstscreen1_mtl->tid = firstscreen1_tid;
    (firstscreen1_mtl->diffuse).r = 1.0f;
    (firstscreen1_mtl->diffuse).g = 1.0f;
    (firstscreen1_mtl->diffuse).b = 1.0f;
    //firstscreen1_mtl->attrib = (numtlattrib_s *)((uint)attr & 0xcc0cffff | 0x16e8000);
    firstscreen1_mtl->attrib.cull = 2;
    firstscreen1_mtl->attrib.zmode = 3;
    firstscreen1_mtl->attrib.filter = 0;
    firstscreen1_mtl->attrib.lighting = 2;
    firstscreen1_mtl->attrib.colour = 1;
    firstscreen1_mtl->alpha = 0.999f;
    firstscreen1_mtl->attrib.utc = 1;
    firstscreen1_mtl->attrib.vtc = 1;
    NuMtlUpdate(firstscreen1_mtl);
    firstscreenfade(firstscreen1_mtl,1);
    nuvideo_global_vbcnt = 0;
    do {
        if (NuRndrBeginScene(1) != 0) {
            NuRndrClear(0xb,0,1.0f);
            NuRndrRectUV2di(0,0,PHYSICAL_SCREEN_X,PHYSICAL_SCREEN_Y,0.0f,0.0f,1.0f,1.0f,-1,firstscreen1_mtl);
            NuRndrEndScene();
            NuRndrSwapScreen(1);
        }
        Reseter(0);
        GC_DiskErrorPoll();
    } while (nuvideo_global_vbcnt < 0x78);
    nuvideo_global_vbcnt = 0;
    firstscreenfade(firstscreen1_mtl,-1);
    NuRndrClear(0xb,0,1.0f);
    NuRndrSwapScreen(1);
    free_x(gerbils);
    if (firstscreen1_mtl != NULL) {
        if (firstscreen1_mtl->tid != 0) {
            NuTexDestroy(firstscreen1_mtl->tid);
        }
        NuMtlDestroy(firstscreen1_mtl);
    }
    return;
}
*/


/* TODO firstscreens
GS_RenderClear

GS_SetOrthMatrix
GS_DrawTriListTTL
GS_BeginScene

GS_SetZCompare
GS_EndScene

GS_FlipScreen
*/
