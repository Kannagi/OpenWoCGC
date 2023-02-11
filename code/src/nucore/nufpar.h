#ifndef NUFPAR_H
#define NUFPAR_H

#include "types.h"
#include "nucore/nucoretypes.h"

/*
  800be18c 000104 800be18c  4 NuFParGetLine 	Global
  800be3cc 0000bc 800be3cc  4 NuFParInterpretWord 	Global
*/

// Old line position.
extern s32 old_line_pos;

// Read a char from a text file.
char NuGetChar(nufpar_s* fPar);

// GetLine.

// Read a word from the text buffer into the word buffer, and return its length.
s32 NuFParGetWord(FPar* fPar);

// Get an int from a text file.
s32 NuFParGetInt(FPar* fPar);

// Push a command. Returns the index of the next command.
s32 NuFParPushCom(FPar* fPar, s32 command);

// Close a text file.
void NuFParClose(FPar* fPar);

// Open a text file.
FPar* NuFParOpen(fileHandle handle);

// Destroy a text file.
void NuFParDestroy(FPar* fPar);

// Create a text file from a file.
FPar* NuFParCreate(char* filename);
#endif // !NUFPAR_H
