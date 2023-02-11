#ifndef NUCORETYPES_H
#define NUCORETYPES_H

#include "types.h"

enum nufilemode_e
{
	NUFILE_MODE_CNT = 3,
	NUFILE_APPEND = 2,
	NUFILE_WRITE = 1,
	NUFILE_READ = 0
};

enum nufileseek_e
{
	NUFILE_SEEK_CNT = 3,
	NUFILE_SEEK_END = 2,
	NUFILE_SEEK_CURRENT = 1,
	NUFILE_SEEK_START = 0
};

// Size: 0x8
struct memexternal_s
{
	union variptr_u* ptr;
	union variptr_u* end;
};

// Size: 0x10
struct nudatinfo_s
{
	int foffset;
	int flen;
	int uplen;
	int ppack : 1; // Offset: 0xC, Bit Offset: 0, Bit Size: 1
};

// Size: 0x8
struct nudfnode_s
{
	short childix;
	short sibix;
	char* txt;
};

// Data file, but this doesn't even need to exist.
// Size: 0x30
struct nudathdr_s
{
	int ver;
	int nfiles;
	struct nudatinfo_s* finfo;
	int treecnt;
	struct nudfnode_s* filetree;
	int leafnamesize;
	char* leafnames;
	int dfhandle;
	int fh;
	short intalloc;
	short openmode;
	int start_lsn;
	void* memdatptr;
};

// Size: 0x14
struct nudatfile_s
{
	struct nudathdr_s* ndh;
	int start;
	int len;
	int fix;
	int used;
};

// Memory file. // Size: 0x14
typedef struct
{
	char* start;
	char* end;
	char* currposition;
	enum nufilemode_e open;
	s32 unused;
} numemfile_s;

struct fileinfo_s
{
	int fh;
	int read_pos;
	int file_pos;
	int file_length;
	int buff_start;
	int buff_length;
	int use_buff;
	struct filebuff_s* buff;
};

struct filebuff_s
{
	void* unk;
};


// Size: 0x14
typedef struct
{
	struct nudathdr_s* ndh;
	int start;
	int len;
	int fix;
	int used;
} nudatfile_s;

// Size: 0x8
typedef struct nuiffhdr_s
{
	int blk;
	int size;
};

// Size: 0xC
typedef struct
{
	struct nuiffhdr_s hdr;
	int pos;
} BlockInfo;

/*OLD
typedef struct
{
	fileHandle handle;
	u8 buffer[0x1000];
	char textBuffer[0x100];
	char wordBuffer[0x104];
	s32 line_num;
	u32 linePos;
	u32 pos;
	s32 bufferPos;
	s32 bufferEndPos;
	u32 f7;
	s32 commandStack[7]; // The last entry is the index of the last entry, where -1 is null.
	struct FParCommand* fpCmd;
	u32 fileLength;
} FPar;

typedef struct
{
	char* str;
	void (*cb)(FPar*);
} FParCommand;*/

typedef struct nufpar_s nufpar_s, *Pnufpar_s;

typedef struct nufpcomjmp_s nufpcomjmp_s, *Pnufpcomjmp_s;

struct nufpcomjmp_s {
    char * fname;
    void (*func)(nufpar_s*);
};

struct nufpar_s {
    fileHandle handle; /* fH */
    u8 buffer[4096]; /* fbuff */
    char textBuffer[256]; /* lbuff */
    char wordBuffer[260];
    int line_num;
    int linepos;
    int cpos;
    int buffstart;
    int buffend;
    struct nufpcomjmp_s * comstack[8];
    int compos;
    int size;
};

typedef struct nufparpos_s nufparpos_s, *Pnufparpos_s;

struct nufparpos_s {
    int line_num;
    int line_pos;
    int cpos;
    int buffstart;
    int buffend;
};



#endif // !NUCORETYPES_H
