
#include<stdlib.h>
#include<stdio.h>
#include<elf_systypes.h>

union variptr_u superbuffer_base;
union variptr_u superbuffer_end;

int main()
{
    //DEMOInit(); hdw init
    // GS_Init(); gamecube video init
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
    printf("\nNuTrigTable init...\n");
    NuTrigInit();
    return 0;
}
