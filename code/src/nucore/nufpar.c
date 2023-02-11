#include "nufpar.h"
#include "nufile.h"

#define LF 0xA	// '\n'
#define CR 0xD	// '\r'
// 0x3b ';'


u32 old_line_pos;

char NuGetChar(nufpar_s* fPar)
{
    s32 bufferEndPos = fPar->buffend;
    char ret;
    if (bufferEndPos < 0)
    {
        bufferEndPos = 0;
    }
    if (fPar->cpos > fPar->buffend)
    {
        if (fPar->fileLength > bufferEndPos + 1)
        {
            s32 size = fPar->size - bufferEndPos;
            if (size > 0x1000)
            {
                size = 0x1000;
            }
            size = NuFileRead(fPar->handle, fPar->buffer, size);
            bufferEndPos = fPar->buffend;
            fPar->buffend = bufferEndPos + size;
            fPar->buffstart = bufferEndPos + 1;
            if (size != 0)
            {
                ret = fPar->buffer[fPar->cpos - fPar->buffstart];
                fPar->cpos++;
                return ret;
            }
        }
        ret = 0;
    }
    else
    {
        ret = fPar->buffer[fPar->cpos - fPar->buffstart];
        fPar->pos++;
    }
    return ret;
}

s32 NuFParGetWord(nufpar_s* fPar)
{
    u32 currLinePos = old_line_pos = fPar->linePos;
    u32 len = 0;
    u32 inQuotation = 0;
    while (fPar->textBuffer[currLinePos & 0xFF] != 0)
    {
        char currChar = fPar->textBuffer[fPar->linePos];	//currChar = fPar->textBuffer[old_line_pos & 0xFF];
        switch (currChar)
        {
        case ' ':
        case ',':
        case '\t':
            if (inQuotation)
            {
                (fPar->wordBuffer + 1)[len & 0xff] = currChar;
                len++;
            }
            else if (len != 0)
            {
                (fPar->wordBuffer + 1)[len & 0xFF] = 0;
                return len;
            }
            break;
        case '\"':
            inQuotation = 1 - inQuotation;
            break;
        default:
            (fPar->wordBuffer + 1)[len & 0xff] = currChar;
            len++;
            break;
        }
        currLinePos = fPar->linePos + 1;
        fPar->linePos = currLinePos;
    }
    fPar->textbuffer[(len & 0xFF) + 1] = 0;
    return len;
}


s32 NuFParGetInt(nufpar_s* fPar)
{
    NuFParGetWord(fPar);
    s32 ret = 0;
    if (fPar->wordBuffer[1] != 0)
    {
        ret = atoi((char*)(fPar->wordBuffer + 1));
    }
    return ret;
}

s32 NuFParPushCom(nufpar_s* fPar, nufpcomjmp_s jmp)
{
    s32 ind = fPar->compos;
    if (ind < 7)
    {
        fPar->compos = ind + 1;
        fPar->comstack[ind] = jmp;
        return fPar->compos;
    }
    return -1;
}

void NuFParClose(nufpar_s* fPar)
{
    NuMemFree(fPar);
}

nufpar_s* NuFParOpen(fileHandle handle)
{
    nufpar_s* fPar = NuMemAlloc(sizeof(FPar));	//size: 0x1244
    if (fPar != NULL)
    {
        memset(fPar, 0, sizeof(FPar));
        fPar->handle = handle;
        fPar->compos = -1;
        fPar->buffend = -1;
        fPar->line_num = -1;
        s32 originalPos = NuFilePos(handle);
        NuFileSeek(handle, 0, 2);
        fPar->size = NuFilePos(handle);	// = filelength (global var s32)
        NuFileSeek(handle, originalPos, 0);
    }

    return fPar;
}

void NuFParDestroy(nufpar_s* fPar)
{
    fileHandle handle = fPar->handle;
    NuFParClose(fPar);
    NuFileClose(handle);
}

nufpar_s* NuFParCreate(char* filename)
{
    fileHandle handle = NuFileOpen(filename, 0); //0= NUFILE_READ
    if (handle != NULL)
    {
        nufpar_s* fPar = NuFParOpen(handle);
        if (fPar != NULL)
        {
            return fPar;
        }
        NuFileClose(handle);
    }
    return NULL;
}

s32 NuFParGetLine(nufpar_s* fPar) {
    s32 i;
    char ch;
    char* textBuffer_ptr;

    i = 0;
    fPar->linepos = 0;

    char inc_f2_flag = 1;
    while ((ch = NuGetChar(fPar)) != 0) {
        if (inc_f2_flag) {
            fPar->line_num += 1;
            inc_f2_flag = 0;
        }

        if ((ch == CR) || (ch == LF)) {
            if (ch == CR) {
                ch = NuGetChar(fPar);
            }
            if (i == 0) {
                inc_f2_flag = 1;
            }
            else {
                break;
            }

        }
        else {
            if (ch == 0x3B) {
                if (i == 0) {
                    do {
                        ch = NuGetChar(fPar);
                    } while (!((ch == LF) || (ch == CR) || (ch == 0)));
                    if (ch == CR) {
                        ch = NuGetChar(fPar);
                    }
                    i = 0;
                    fPar->linepos = 0;
                    inc_f2_flag = 1;
                    continue;
                }
            }
            fPar->textBuffer[i] = ch;
            i += 1;
        }
    }
    fPar->textBuffer[i] = 0;
    return i;
}

// Something like this - I cannot fully confirmed this is 100% correct
s32 NuFParInterpretWord(FPar* fPar) {
    s32 i = 0;
    if (fPar->comstack[0].fname != NULL) {
        do {
            if (strcasecmp(fPar->comstack[i].fname, fPar->wordBuffer + 1) != 0) {
                fPar->comstack[i].func(fPar);
                return 1;
            }
            i += 1;
        } while (fPar->comstack[i].fname != NULL);
    }
    return 0;
}