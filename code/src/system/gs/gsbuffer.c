#include "gsbuffer.h"

//MATCH GCN
void* GS_CreateBuffer (u32 length, s32 btype){
    struct GS_Buffer *bufptr;

    bufptr = (struct GS_Buffer*)malloc(length + 8);
    GS_BufferSize += length;
    bufptr->size = length;
    bufptr->type = btype;
    buftypes[btype] = buftypes[btype] + length;
    
	return bufptr + 1;
}

//MATCH GCN
void GS_DeleteBuffer(void* ptr)
{
    struct GS_Buffer* bufptr = (struct GS_Buffer*)((int)ptr - 8);
    GS_BufferSize -= bufptr->size;
    BufferTypes[bufptr->type] -= bufptr->size;
    free(bufptr);
}
