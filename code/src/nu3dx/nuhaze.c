#include "nuhaze.h"
#include "../system.h"

struct numtl_s* NuLightHazePolymtl;
float NuLightHazePolyus[2];
float NuLightHazePolyvs[2];

//MATCH NGC
void NuLigthSetPolyHazeMat(struct numtl_s *mtl,float *arg1,float *arg2) {
  NuLightHazePolymtl = mtl;
  NuLightHazePolyus[0] = *arg1;
  NuLightHazePolyus[1] = arg1[1];
  NuLightHazePolyvs[0] = *arg2;
  NuLightHazePolyvs[1] = arg2[1];
  return;
}