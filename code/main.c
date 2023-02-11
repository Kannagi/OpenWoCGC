
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
    return 0;
}
