
#include<stdlib.h>
#include<stdio.h>
#include<types.h>
#include<nu3dx/nu3dxtypes.h>
#include<stdbool.h>
#include<SDL/SDL.h>
#define GL_BGR 0x80E0

#include<elf_systypes.h>

#include <GL/gl.h>
#ifdef __MINGW32__
#undef main
#endif

struct nutex_s tex;
struct numtl_s *mtl;
struct numtlattrib_s attr;
s32 iss3cmp;
s32 texinfo;
void* pixel_dat;
union variptr_u superbuffer_base;
union variptr_u superbuffer_end;
int test_SDL_openGL();

int main()
{
    //DEMOInit(); hdw init
    // GS_Init(); gamecube video init , no PS2 I think
    // SS_Init(); sound init

    printf("\nStarting WoC Engine\n");
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
    //printf("NuMtl init...\n");
    //NuMtlInit();
    //NuRndrInitEx();
    //printf("NuLight init...\n");
    //NuLightInit();

    //from firstscreens function
    pixel_dat = malloc_x(0x4000c);
    NuFileLoadBuffer("licnin.s3",pixel_dat,0x2000c);
    tex.height = 0x200;
    tex.decal = 0;
    tex.pal = NULL;
    tex.width = 0x200;
    tex.mmcnt = 1;
    tex.type = NUTEX_RGB24;
    iss3cmp = 0x20000;
    tex.bits = pixel_dat;
    texinfo = NuTexCreate(&tex);
    iss3cmp = 0;
    //mtl = NuMtlCreate(1);
    //attr = mtl->attrib;
    /*mtl->tid = texinfo;
    (mtl->diffuse).b = 1.0;
    mtl->alpha = 0.999;
    (mtl->diffuse).r = 1.0;
    (mtl->diffuse).g = 1.0;*/
    //mtl->attrib = (numtlattrib_s)((uint)attr & 0xcc0cffff | 0x16e8000);   need fix
    //firstscreenfade(lic,1);
    //nuvideo_global_vbcnt = 0;


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
