#include"system/port.h"

void CloseShaders(void) {
    ResetShaders();
    RemoveWaterCausticTextures();
    return;
}

void ResetShaders(void) {
    SetupShaders(NULL);
    return;
}

//NGC MATCH
void InitialiseShaders(s32 Level) {
    NuMtxSetIdentity(&identmtx);
    currentLevel = Level;
    defaultShader = NO_SHADER;
    if (((Level == 2) || (Level == 0x22)) || (Level == 6)) {
        defaultShader = WATERCAUSTICS;
    }
    else if ((Level == 0x1b) && (IsLoadingScreen == 0)) {
        defaultShader = POINTLIGHT;
    }
    LoadWaterCausticTextures();
}

//NGC MATCH
void NuShaderSetBypassShaders(s32 flag) {
    bypassEffectShaders = flag;
    return;
}

//NGC MATCH
void NuShaderSetSkinningConstants(struct nugeomitem_s* item, struct nuprim_s* prim) {
  s32 i;
  s32 lookup;

  lookup = (s32)prim->skinmtxlookup;
  for(i = 0; i < 0x10; i++, lookup+= 2) {
    if (*(short *)lookup != -1) {
      NuMtxTranspose((struct numtx_s *)&CV_SKINMTX[i], &item->mtx[*(short *)lookup]);
    }
  }
  return;
}

//NGC MATCH
void NuShaderSetColourConstants(struct numtl_s *mtl) {
    if ((mtl != NULL) && (mtl->attrib.lighting != 2 || mtl->attrib.colour != 1)) {
        CV_MTL_COLOR = *(struct _GS_VECTOR4 *)makenuvec4((mtl->diffuse).r,(mtl->diffuse).g,(mtl->diffuse).b,mtl->alpha);
        NEW_CV_COLOR_TYPE = 1;
    }
    else {
        CV_MTL_COLOR = *(struct _GS_VECTOR4 *)makenuvec4(0.0f,0.0f,0.0f,0.0f);
        NEW_CV_COLOR_TYPE = 2;
    }
    return;
}

//NGC MATCH
void NuShaderSetLightConstants(struct numtl_s *mtl) {
    struct nulight_s *l1;
    struct nulight_s *l2;
    struct nulight_s *l3;

    if (mtl->attrib.lighting == 2) {
        CV_LIGHT1_COLOR.x = 0.0f;
        CV_LIGHT1_COLOR.y = 0.0f;
        CV_LIGHT1_COLOR.z = 0.0f;
        CV_LIGHT1_COLOR.w = 0.0f;
        CV_LIGHT2_COLOR.x = 0.0f;
        CV_LIGHT2_COLOR.y = 0.0f;
        CV_LIGHT2_COLOR.z = 0.0f;
        CV_LIGHT2_COLOR.w = 0.0f;
        CV_LIGHT3_COLOR.x = 0.0f;
        CV_LIGHT3_COLOR.y = 0.0f;
        CV_LIGHT3_COLOR.z = 0.0f;
        CV_LIGHT3_COLOR.w = 0.0f;
        CV_LIGHT1_POS.x = 0.0f;
        CV_LIGHT1_POS.y = 0.0f;
        CV_LIGHT1_POS.z = 0.0f;
        CV_LIGHT1_POS.w = 1.0f;
        CV_LIGHT2_POS.x = 0.0f;
        CV_LIGHT2_POS.y = 0.0f;
        CV_LIGHT2_POS.z = 0.0f;
        CV_LIGHT2_POS.w = 1.0f;
        CV_LIGHT3_POS.x = 0.0f;
        CV_LIGHT3_POS.y = 0.0f;
        CV_LIGHT3_POS.z = 0.0f;
        CV_LIGHT3_POS.w = 1.0f;
        CV_AMBIENT_COLOR.x = 0.0f;
        CV_AMBIENT_COLOR.y = 0.0f;
        CV_AMBIENT_COLOR.z = 0.0f;
        CV_AMBIENT_COLOR.w = 0.0f; 
        newCV_LIGHTS_OFF = 1;
    }
    else {
        l1 = NuLightGetLight(0);
        l2 = NuLightGetLight(1);
        l3 = NuLightGetLight(2);
        CV_LIGHT1_POS.x = (l1->mtx)._20;
        CV_LIGHT1_POS.y = (l1->mtx)._21;
        CV_LIGHT1_POS.z = (l1->mtx)._22;
        CV_LIGHT1_POS.w = 1.0f;
        CV_LIGHT2_POS.x = (l2->mtx)._20;
        CV_LIGHT2_POS.y = (l2->mtx)._21;
        CV_LIGHT2_POS.z = (l2->mtx)._22;
        CV_LIGHT2_POS.w = 1.0f;
        CV_LIGHT3_POS.x = (l3->mtx)._20;
        CV_LIGHT3_POS.y = (l3->mtx)._21;
        CV_LIGHT3_POS.z = (l3->mtx)._22;
        CV_LIGHT3_POS.w = 1.0f;
        CV_LIGHT1_COLOR.x = (l1->diffuse).r;
        CV_LIGHT1_COLOR.y = (l1->diffuse).g;
        CV_LIGHT1_COLOR.z = (l1->diffuse).b;
        CV_LIGHT1_COLOR.w = 0.0f;
        CV_LIGHT2_COLOR.x = (l2->diffuse).r;
        CV_LIGHT2_COLOR.y = (l2->diffuse).g;
        CV_LIGHT2_COLOR.z = (l2->diffuse).b;
        CV_LIGHT2_COLOR.w = 0.0f;
        CV_LIGHT3_COLOR.x = (l3->diffuse).r;
        CV_LIGHT3_COLOR.y = (l3->diffuse).g;
        CV_LIGHT3_COLOR.z = (l3->diffuse).b;
        CV_LIGHT3_COLOR.w = 0.0f;
        CV_AMBIENT_COLOR.x = (l1->ambient).r / 3.0f;
        CV_AMBIENT_COLOR.y = (l1->ambient).g / 3.0f;
        CV_AMBIENT_COLOR.z = (l1->ambient).b / 3.0f;
        CV_AMBIENT_COLOR.w = 1.0f;
        newCV_LIGHTS_OFF = 0;
    }

    GS_Set3Lights(&CV_LIGHT1_POS,&CV_LIGHT2_POS,&CV_LIGHT3_POS,&CV_LIGHT1_COLOR,&CV_LIGHT2_COLOR,
                  &CV_LIGHT3_COLOR,&CV_AMBIENT_COLOR);
}

void NuShaderSetGlassMix(float mix) {
    glassmix = mix;
    return;
}

//EMPTY FUNCTION
void SetPixelShader(u32 Handle) {
    return;
}

//NGC MATCH
void SetVertexShader(u32 Handle) {
    
    switch (Handle) {
    case 0x144:
    case 0x53:
        shaderselected = 1;
        xytype = 1;
        ShaderHasNormals = 0;
        return;
    case 0x152:
    case 0x59:
        shaderselected = 2;
        xytype = 0;
        ShaderHasNormals = 1;
        return;
    case 0x142:
        shaderselected = 3;
        xytype = 0;
        ShaderHasNormals = 0;
        return;
    case 0x5D:
        shaderselected = 4;
        xytype = 0;
        ShaderHasNormals = 1;
        return;
    case 0x11:
        shaderselected = 5;
        xytype = 2;
        ShaderHasNormals = 0;
        return;
    default:
            DisplayErrorAndLockup
                      ("C:/source/crashwoc/code/system/port.c",0x70,"D3DDevice_SetVertexShader");
        return;
    }
}

//NGC MATCH
struct NuVec* GetPlayerPosition(void) {
    if (player == NULL) {
        NuErrorProlog("C:/source/crashwoc/code/system/port.c",0xfc)("assert");
    }
    return &(player->obj).pos;
}

/* //ASM GCN
void LinkDmaParticalSets(int **table,int count) {
    int **ppiVar1;
    int **lp;
    int *ptr;
    
    *table[count + -1] = 1;
    if (count + -2 < 0) {
        return;
    }
    lp = table + count + -2;
    do {
        ptr = *lp;
        *ptr = 0;
        ppiVar1 = lp + 1;
        lp = lp + -1;
        ptr[1] = (int)*ppiVar1;
    } while ((int)table <= (int)lp);
    return;
}
*/
