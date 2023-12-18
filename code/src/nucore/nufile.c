
#define FIRST 0

#include "nufile.h"
#include "nuerror.h"

unsigned char* filebuffer = NULL;
s32 blkcnt = 1;
static struct BlockInfo blkinfo[0x400];
unsigned char* bpointer;
s32 badGameDisk = 0;
fileHandle currentpointer = -1;
s32 bytesleft = 0;
s32 thisbytesread = 0;
FILE* fpointers[MAX_FILES] = {
	NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL
};
const char* fmode[] = {"rb", "wb"};
struct numemfile_s memfiles[MAX_MEM_FILES];
struct nudatfile_s datfiles[MAX_MEM_FILES];
s32 loadscreen = -1;
s32 loadscreenfadedir = 0;
s32 datacounter = 0;
s32 datafull;
s32 totalbytesread = 0;

s32 NuFileGetBadGameDisc()
{
	return badGameDisk;
}

void NuFileInitEx(int deviceid, int rebootiop)
{
	if (filebuffer == NULL)
	{
		filebuffer = malloc_x(0x10000);
	}
	memset(memfiles, 0 , MAX_MEM_FILES * sizeof(struct numemfile_s));
	memset(datfiles, 0, MAX_MEM_FILES  * sizeof(struct nudatfile_s));

	printf("memfiles after memset(): %d\n", memfiles);
	printf("datfiles after memset(): %d\n", datfiles);
}

//NGC MATCH
s32 NuFileExists(char* filename) {
    FILE* filep; //__sFILE*
    char name[128] = "";

    //strcpy(name, "/");
	strcat(name, filename);
	filep = fopen(name, "r");
	printf("check F.E. \n");
	if (filep != NULL) {
		fclose(filep);
        printf("file exist! %s \n", filename);
        return 1;
	}
	printf("file not exist %s \n", filename);
	return 0;
}

s32 checkmemfile(char* name)
{
	return -1;
}

s32 checkdiscfile(char* name)
{
	return -1;
}

//NGC 94%
s32 NuFileOpen(char* file, enum nufilemode_e mode) {
    int s; // r31
    int t; // r3
    int f;
    FILE* fp; // r9	//__sFILE*
    FILE** p; //__sFILE**
    static char name[128] = ""; // 0x80229388
    void* m;

    if (NuFileGetBadGameDisc() == 0) {
        thisbytesread = 0;
        checkmemfile(file);
        if (checkdiscfile(file) == -1) {
            //strcpy(name, "/");
            printf("checkdiscfile = -1 \n");
        } else {
            strcpy(name, "z:\\");
        }
        strcat(name, file);
        printf("F.O. strcat name %s \n", name);
        if (mode > NUFILE_APPEND) {
            NuErrorProlog("OpenCrashWOC/code/nucore/nufile.c", 0x39e,"assert");
        }
        for (p = fpointers, s = 0; s < MAX_FILES; s++) { //MAX_FILES = 10
            m = *p;
            printf("F.O. check loop \n");
            if (p == NULL) {
                    printf("F.O. fopen \n");
                fp = fopen(name, fmode[mode]);
                if (fp == NULL) {
                    printf("file null \n");
                    return NULL;
                }
                printf("\n bytesleft %d",bytesleft);
                *p = fp;
                bytesleft = m;

                return s + 1;
            }
            p++;
        }
    }
    return NULL;
}

//NGC MATCH
void NuFileClose(s32 handle) {
	if (handle > 0x3ff) {
		NuMemFileClose(handle);
	}
	else {
		if (handle == currentpointer)
		{
			currentpointer = -1;
			bytesleft = 0;
		}
        handle--;
		fclose(fpointers[handle]);
		fpointers[handle] = NULL;
	}
}

//NGC MATCH
s32 NuMemFileOpen(void* start, s32 buffsize, enum nufilemode_e mode) {
    s32 i;

	if (buffsize > 0 && mode == NUFILE_READ) {
		for (i = 0; i < 20; i++) {
			if (!memfiles[i].used) {
				memfiles[i].start = (char*)start;
				memfiles[i].end = (char*)(((s32)start + buffsize) - 1);
				memfiles[i].curr = (char*)start;
				memfiles[i].mode = mode;
				memfiles[i].used = 1;
				return i + 0x400;
			}
		}
	}
	return 0;
}

s32 NuDatFilePos(s32 handle) {
	return 0;
}

s32 NuDatFileRead(s32 fh, void* data, s32 size)
{
	return 0;
}

s32 NuDatFileSeek(s32 fh, s32 offset, s32 origin)
{
	return 0;
}

void NuDatFileClose(s32 handle) {
	return 0;
}

//NGC MATCH
void NuMemFileClose(s32 handle) {
	if (handle > 0x7ff) {
		NuDatFileClose(handle);
	}
	else {
        handle -= 0x400;
		memfiles[handle].used = 0;
	}
}

s32 NuMemFilePos(fileHandle handle)
{
	if (handle < 0x800)
	{
		return ((s32)memfiles[handle - 0x400].curr - (s32)memfiles[handle - 0x400].start);
	}
	else
	{
		return NuDatFilePos(handle);
	}
}

s32 NuMemFileRead(fileHandle handle, void* data, s32 size)
{
	if (handle < 0x800)
	{
		s32 f = handle - 0x400;
		s32 left = (s32)memfiles[f].end + (1 - (s32)memfiles[f].curr);
		if (size > left)
		{
			size = left;
		}
		if (size != 0)
		{
			memcpy(data, memfiles[f].curr, size);
			memfiles[f].curr = memfiles[f].curr + size;
		}
		return size;
	}
	else
	{
		return NuDatFileRead(handle, data, size);
	}
}

s32 NuMemFileSeek(fileHandle handle, s32 offset, s32 origin)
{
	if (handle < 0x800)
	{
		fileHandle f = handle - 0x400;
		switch (origin)
		{
		case NUFILE_SEEK_START:
			memfiles[f].curr = (void*)((s32)memfiles[f].start + offset);
			break;
		case NUFILE_SEEK_CURRENT:
			memfiles[f].curr = (void*)((s32)memfiles[f].curr + offset);
			break;
		case NUFILE_SEEK_END:
			memfiles[f].curr = (void*)((s32)memfiles[f].end + (1 - offset));
			break;
		}
		return (s32)memfiles[f].curr - (s32)memfiles[f].start;
	}
	else
	{
		return NuDatFileSeek(handle, offset, origin);
	}
}

// I completely redid a part of this function, I hope it's right.
s32 NuFilePos(fileHandle handle)
{
	if (handle < 0x400)
	{
		s32 ret = thisbytesread;
		if (fpointers[handle - 1] != NULL)
		{
			ret = ftell(fpointers[handle - 1]);
		}
		return ret;
	}
	else
	{
		return NuMemFilePos(handle);
	}
}

s32 NuFileSeek(fileHandle handle, s32 offset, s32 origin)
{
	if (handle < 0x400)
	{
		if (origin == NUFILE_SEEK_CURRENT)
		{
		    printf("seek curr...\n");
			offset -= bytesleft;
		}
		bytesleft = 0;
		return fseek(fpointers[handle - 1], offset, origin);
	}
	else
	{
		return NuMemFileSeek(handle, offset, origin);
	}
}


//NGC MATCH
s32 NuFileSize(char* fileName) {
	s32 rv = -1;
	s32 handle;

    printf("checking file size \n");
	if (fileName != NULL && *fileName != 0) {
	    if (NuFileExists(fileName) != 0) {
	    printf("file size exist... \n");
            handle = NuFileOpen(fileName, NUFILE_READ);
            if (handle != NULL)
			{
				rv = GCFileSize(handle);
				NuFileClose(handle);
			}
        }
	}
	return rv;
}

void* NuFileLoad(char* fileName)
{
	void* dest = NULL;
	s32 size = NuFileSize(fileName);
	if (size > 0)
	{
		fileHandle handle = NuFileOpen(fileName, NUFILE_READ);
		if (handle != NULL)
		{
			dest = NuMemAlloc(size);
			if (dest != NULL)
			{
				NuFileRead(handle, &dest, size);
			}
			NuFileClose(handle);
		}
	}
	return dest;
}

//98% NGC
s32 NuFileLoadBuffer(char* fileName, void* mem, s32 buffsize) {
    s32 handle;
	s32 size = NuFileSize(fileName);
	if (size == 0) {
		printf("file size: %d\n", size);
		NuErrorProlog("OpenCrashWOC/code/nucore/nufile.c", 0x58d) ("File %s does not exist!", fileName);
	}
	if (size > buffsize) {
		printf("file size: %d\n", size);
		NuErrorProlog("OpenCrashWOC/code/nucore/nufile.c", 0x590) ("Super Buffer out of space!");
        return;
	}
	if (size != 0) {
        handle = NuFileOpen(fileName, NUFILE_READ);
		if (handle != NULL) {
			printf("filebuffer reading...\n");
			NuFileRead(handle, mem, size);
			NuFileClose(handle);
			return size;
		}
	}
    return 0;
}

//93% NGC
s32 NuFileRead(s32 handle, void* data, s32 size) {
    s32 bytesread;
    u8* pt;
    s32 tbytesread;

	if (handle > 0x3ff) {
		size = NuMemFileRead(handle, data, size);
	}
	else {
		if (currentpointer != handle || handle == -1)
		{
			bytesleft = 0;
			totalbytesread = 0;
			currentpointer = handle;
		}
		handle--;
        // There's some kind of gamecube reset call here then an error check, I don't think leaving it out will matter on PC.
		//Reseter(1);
        //GC_DiskErrorPoll();
		if (bytesleft == 0) {
			bytesleft = fread(filebuffer, 1, 0x10000, fpointers[handle]);
			totalbytesread += bytesleft;
			bpointer = filebuffer;
		}
		datacounter += size;
		if (size <= bytesleft) {
			memcpy(data, bpointer, size);
			bpointer = (u8*)((u32)bpointer + size);
			bytesleft -= size;
			thisbytesread += size;
		}
		else {
			memcpy(data, bpointer, bytesleft);
			pt = (u8*)((s32)data + bytesleft);
			thisbytesread += size;
			bytesread = size - bytesleft;
			size = bytesleft;
			while (bytesread > 0x10000) {
				bytesread -= 0x10000;
				tbytesread = fread(filebuffer, 1, 0x10000, fpointers[handle]);
				size += tbytesread;
				memcpy(pt, filebuffer, 0x10000);
				pt = (u8*)((u32)pt + 0x10000);
			}
			bytesleft = 0;
			if (bytesread > 0) {
				tbytesread = fread(filebuffer, 1, 0x10000, fpointers[handle]);
				memcpy(pt, filebuffer, bytesread);
				size += tbytesread;
				bytesleft = tbytesread - bytesread;
				bpointer = (u8*)((u32)filebuffer + bytesread);
			}
		}
	}
	return size;
}

void NuStartLoadScreen(s32 screen)
{
	if (screen > 0)
	{
		loadscreen = screen;
		datacounter = 0;
		datafull = screen;
		return;
	}
	else
	{
		loadscreen = screen;
		datacounter = 0;
		datafull = 0;
	}

	return;
}

void NuStopLoadScreen()
{
	loadscreen = -1;
	loadscreenfadedir = 0;

	return;
}

f32 NuFileReadFloat(fileHandle handle)
{
	f32 tmp = 0.0;
	NuFileRead(handle, &tmp, 4);
	return tmp;
}

s32 NuFileReadInt(fileHandle handle)
{
	s32 tmp;
	NuFileRead(handle, &tmp, 4);
	return tmp;
}

s16 NuFileReadShort(fileHandle handle)
{
	s16 tmp;
	NuFileRead(handle, &tmp, 2);
	return tmp;
}

char NuFileReadChar(fileHandle handle)
{
	char tmp;
	NuFileRead(handle, &tmp, 1);
	return tmp;
}

s32 NuFileBeginBlkRead(fileHandle handle, s32 blkType)
{
	s32 currBlock = blkcnt;
	blkcnt++;
	s32 pos = NuFilePos(handle);
	if (currBlock < 2 || pos < blkinfo[currBlock - 1].pos + blkinfo[currBlock - 1].hdr.size)
	{
		blkinfo[currBlock].pos = pos; //old: blkinfo[currBlock - 1].pos = pos;
		NuFileRead(handle, blkinfo + currBlock, 8);
		if (blkinfo[currBlock].hdr.size < 0)
		{
			blkinfo[currBlock].hdr.size = -blkinfo[currBlock].hdr.size;
		}
		if (blkType != 0 && blkType != blkinfo[currBlock].hdr.blk)
		{
			NuErrorProlog("OpenCrashWOC/code/nucore/nufile.c", 471, "NuFileBeginBlkRead : Block header mismatch!");
		}
		return blkinfo[currBlock].hdr.blk;
	}
	else
	{
		currBlock = 0;
		blkcnt--;
		return 0;
	}
}

s32 NuFileEndBlkRead(fileHandle handle)
{
	blkcnt--;
	s32 pos = NuFilePos(handle);
	s32 blkEnd = blkinfo[blkcnt].pos + blkinfo[blkcnt].hdr.size;
	if (pos != blkEnd)
	{
		pos = NuFileSeek(handle, blkEnd, NUFILE_SEEK_START);
	}

	return pos;
}

void NuDatClose(struct nudathdr_s* ndh)
{
	NuFileClose(ndh->fh);
	if (ndh->finfo->foffset) //fileid
	{
		datfiles[ndh->finfo->foffset - 0x800].used = 0;
	}
	if (ndh->intalloc) //managedmem
	{
		if (ndh->memdatptr != NULL)
		{
			NuMemFree(ndh->memdatptr);
		}
		NuMemFree(ndh);
	}
	return;
}
