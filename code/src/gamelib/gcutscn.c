void NuGCutSceneSysInit(NUGCUTLOCFNDATA_s *cutlocdat)

{
  locatorfns = cutlocdat;
  active_cutscene_instances = (instNUGCUTSCENE_s *)0x0;
  return;
}

void NuGCutSceneSysUpdate(int paused)

{
  instNUGCUTSCENE_s *icutscene;
  
  icutscene = active_cutscene_instances;
  if (active_cutscene_instances != (instNUGCUTSCENE_s *)0x0) {
    do {
      if ((*(uint *)&icutscene->field_0x6c & 0x400000) == 0) {
        instNuGCutSceneUpdate(icutscene,paused);
        *(uint *)&icutscene->field_0x6c = *(uint *)&icutscene->field_0x6c | 0x400000;
      }
      icutscene = icutscene->next;
    } while (icutscene != (instNUGCUTSCENE_s *)0x0);
  }
  return;
}

void NuGCutSceneSysRender(void)

{
  instNUGCUTSCENE_s *instCut;
  
  instCut = active_cutscene_instances;
  if (active_cutscene_instances != (instNUGCUTSCENE_s *)0x0) {
    do {
      instNuGCutSceneRender(instCut);
      *(uint *)&instCut->field_0x6c = *(uint *)&instCut->field_0x6c & 0xffbfffff;
      instCut = instCut->next;
    } while (instCut != (instNUGCUTSCENE_s *)0x0);
  }
  return;
}


NUGCUTSCENE_s * NuGCutSceneLoad(char *fname,variptr_u *buff,variptr_u *endbuff)		//need correction!

{
  float CUTBOX;
  int fsize;
  NUERRORFUNC *e;
  float ntbl;
  numtx_s *cutscene;
  void *mem;
  
  mem = (void *)((uint)((int)&buff->vec4->w + 3) & 0xfffffff0);
  buff->voidptr = mem;
  fsize = NuFileLoadBuffer(fname,mem,(int)endbuff->voidptr - (int)mem);
  if (fsize == 0) {
    e = NuErrorProlog("C:/source/crashwoc/code/gamelib/gcutscn.c",0xab);
    (*e)("Could not read from cutscene <fname>, either file is not there or the buffer may be full" )
    ;
  }
  cutscene = buff->mtx44;
  buff->voidptr = (void *)((int)&cutscene->_00 + fsize);
  if (cutscene->_00 != 1.401298e-45) {
    e = NuErrorProlog("C:/source/crashwoc/code/gamelib/gcutscn.c",0xb0);
    (*e)("Out of date version of cutscene <%s>, you have version <%d>",fname,cutscene->_00);
  }
  ntbl = 0.0;
  CUTBOX = (float)((int)cutscene - (int)cutscene->_01);
  cutscene->_01 = CUTBOX;
  if (cutscene->_03 != 0.0) {
    ntbl = (float)((int)cutscene->_03 + (int)CUTBOX);
  }
  cutscene->_03 = ntbl;
  if (cutscene->_10 != 0.0) {
    NuGCutCamsSysFixPtrs((NUGCUTSCENE_s *)cutscene,(int)CUTBOX);
  }
  if (cutscene->_13 != 0.0) {
    NuGCutLocatorSysFixPtrs((NUGCUTSCENE_s *)cutscene,(int)cutscene->_01);
  }
  if (cutscene->_11 != 0.0) {
    NuGCutRigidSysFixPtrs((NUGCUTSCENE_s *)cutscene,(int)cutscene->_01);
  }
  if (cutscene->_12 != 0.0) {
    NuGCutCharSysFixPtrs((NUGCUTSCENE_s *)cutscene,(int)cutscene->_01);
  }
  if (cutscene->_21 != 0.0) {
    NuGCutTriggerSysFixPtrs((NUGCUTSCENE_s *)cutscene,(int)cutscene->_01);
  }
  if (cutscene->_20 == 0.0) {
    CUTBOX = 0.0;
  }
  else {
    CUTBOX = (float)((int)cutscene->_20 + (int)cutscene->_01);
  }
  cutscene->_20 = CUTBOX;
  return (NUGCUTSCENE_s *)cutscene;
}


void NuGCutSceneFixUp(NUGCUTSCENE_s *cutscene,nugscn_s *scene,NUTRIGGERSYS_s *triggersys)

{
  if (cutscene != (NUGCUTSCENE_s *)0x0) {
    if (cutscene->rigids != (NUGCUTRIGIDSYS_s *)0x0) {
      NuGCutRigidSysFixUp(cutscene,scene);
    }
    if (cutscene->chars != (NUGCUTCHARSYS_s *)0x0) {
      NuGCutCharSysFixUp(cutscene);
    }
    if (cutscene->locators != (NUGCUTLOCATORSYS_s *)0x0) {
      NuGCutLocatorSysFixUp(cutscene->locators);
    }
    if ((cutscene->triggersys != (NUGCUTTRIGGERSYS_S *)0x0) && (triggersys != (NUTRIGGERSYS_s *)0x 0)
       ) {
      NuGCutTriggerSysFixUp(cutscene,triggersys);
    }
  }
  return;
}


instNUGCUTSCENE_s *
instNuGCutSceneCreate
          (NUGCUTSCENE_s *cutscene,nugscn_s *gscene,instNUTRIGGERSYS_s *itriggersys,char *name,
          variptr_u *buff)

{
  instNUGCUTCAMSYS_s *camsys;
  instNUGCUTRIGIDSYS_s *rigidsys;
  instNUGCUTCHARSYS_s *charsys;
  instNUGCUTLOCATORSYS_s *locsys;
  instNUGCUTTRIGGERSYS_s *trigsys;
  instNUGCUTSCENE_s *icutscene;
  
  icutscene = (instNUGCUTSCENE_s *)0x0;
  if (cutscene != (NUGCUTSCENE_s *)0x0) {
    icutscene = (instNUGCUTSCENE_s *)(buff->intaddr + 0xf & 0xfffffff0);
    buff->voidptr = icutscene + 1;
    memset(icutscene,0,0x94);
    icutscene->next = active_cutscene_instances;
    if (active_cutscene_instances != (instNUGCUTSCENE_s *)0x0) {
      active_cutscene_instances->prev = icutscene;
    }
    active_cutscene_instances = icutscene;
    icutscene->cutscene = cutscene;
    if (name != (char *)0x0) {
      sprintf(icutscene->name,name);
    }
    if (cutscene->cameras != (NUGCUTCAMSYS_s *)0x0) {
      camsys = instNuCGutCamSysCreate(cutscene->cameras,buff);
      icutscene->icameras = camsys;
    }
    if (cutscene->rigids != (NUGCUTRIGIDSYS_s *)0x0) {
      rigidsys = instNuCGutRigidSysCreate(cutscene,gscene,buff);
      icutscene->irigids = rigidsys;
    }
    if (cutscene->chars != (NUGCUTCHARSYS_s *)0x0) {
      charsys = instNuCGutCharSysCreate(cutscene,buff);
      icutscene->ichars = charsys;
    }
    if (cutscene->locators != (NUGCUTLOCATORSYS_s *)0x0) {
      locsys = NuCGutLocatorSysCreateInst(cutscene->locators,buff);
      icutscene->ilocators = locsys;
    }
    if ((cutscene->triggersys != (NUGCUTTRIGGERSYS_S *)0x0) &&
       (itriggersys != (instNUTRIGGERSYS_s *)0x0)) {
      trigsys = instNuCGutTriggerSysCreate(cutscene,itriggersys,buff);
      icutscene->itriggersys = trigsys;
    }
    instNuGCutSceneCalculateCentre(icutscene,(numtx_s *)0x0);
    icutscene->rate = 1.0;
  }
  return icutscene;
}


void instNuGCutSceneDestroy(instNUGCUTSCENE_s *icutscene)

{
  if ((*(uint *)&icutscene->field_0x6c & 0x40000000) != 0) //param_1[0x1b] & 0x40000000U
  {
    instNuGCutSceneEnd(icutscene);
  }
  if (icutscene->next != (instNUGCUTSCENE_s *)0x0) {
    icutscene->next->prev = icutscene->prev;
  }
  if (icutscene->prev == (instNUGCUTSCENE_s *)0x0) {
    active_cutscene_instances = icutscene->next;
  }
  else {
    icutscene->prev->next = icutscene->next;
  }
  return;
}

void instNuGCutSceneStart(instNUGCUTSCENE_s *icutscene)

{
  icutscene->cframe = 1.0;
  *(uint *)&icutscene->field_0x6c = *(uint *)&icutscene->field_0x6c & 0x7fffffff | 0x40000000;
  if (icutscene->icameras != (instNUGCUTCAMSYS_s *)0x0) {
    instNuGCutCamSysStart(icutscene->icameras,icutscene->cutscene->cameras);
  }
  if (icutscene->irigids != (instNUGCUTRIGIDSYS_s *)0x0) {
    instNuGCutRigidSysStart(icutscene->irigids,icutscene->cutscene->rigids);
  }
  if (icutscene->ilocators != (instNUGCUTLOCATORSYS_s *)0x0) {
    instNuGCutLocatorSysStart(icutscene->ilocators,icutscene->cutscene->locators);
  }
  if (icutscene->itriggersys != (instNUGCUTTRIGGERSYS_s *)0x0) {
    instNuGCutTriggerSysStart(icutscene);
  }
  return;
}

void instNuGCutSceneEnd(instNUGCUTSCENE_s *icutscene)

{
  float nframes;
  
  nframes = icutscene->cutscene->nframes;
  *(uint *)&icutscene->field_0x6c = *(uint *)&icutscene->field_0x6c & 0xbfffffff;
  icutscene->cframe = nframes + 1.0;
  return;
}


void instNuGCutSceneSetEndCallback(instNUGCUTSCENE_s *icutscene,void *fn)

{
  icutscene->endfn = fn;
  return;
}


void instNuGCutSceneClipTest(instNUGCUTSCENE_s *icutscene)

{
  uint uVar1;
  NUGCUTBBOX_s *bbox;
  int iVar2;
  numtx_s *wm;
  NUGCUTSCENE_s *cutscene;
  float fVar3;
  
  uVar1 = *(uint *)&icutscene->field_0x6c;
  cutscene = icutscene->cutscene;
  *(uint *)&icutscene->field_0x6c = uVar1 | 0x1000000;
  if (cutscene->bbox == (NUGCUTBBOX_s *)0x0) {
    return;
  }
  if (((uVar1 & 0x2000000) == 0) ||
     (fVar3 = NuCameraDistSqr(&icutscene->centre), fVar3 <= icutscene->maxdistsqr)) {
    if ((*(uint *)&icutscene->field_0x6c & 0x4000000) == 0) {
      return;
    }
    if ((*(uint *)&icutscene->field_0x6c & 0x8000000) == 0) {
      bbox = cutscene->bbox;
      wm = &numtx_identity;
    }
    else {
      bbox = cutscene->bbox;
      wm = &icutscene->mtx;
    }
    iVar2 = NuCameraClipTestExtents(&bbox->min,&bbox->max,wm);
    if (iVar2 != 0) {
      return;
    }
  }
  *(uint *)&icutscene->field_0x6c = *(uint *)&icutscene->field_0x6c & 0xfeffffff;
  return;
}

void instNuGCutSceneUpdate(instNUGCUTSCENE_s *icutscene,int paused)

{
  uint uVar1;
  code *endfn;
  NUGCUTSCENE_s *cutscene;
  double dVar2;
  float dt;
  
  cutscene = icutscene->cutscene;
  if ((*(uint *)&icutscene->field_0x6c & 0x800000) != 0) {
    return;
  }
  if ((*(uint *)&icutscene->field_0x6c & 0x90000000) == 0x10000000) {
    instNuGCutSceneStart(icutscene);
  }
  if ((*(uint *)&icutscene->field_0x6c & 0x40000000) == 0) {
    return;
  }
  instNuGCutSceneClipTest(icutscene);
  uVar1 = *(uint *)&icutscene->field_0x6c;
  if ((uVar1 & 0x1000000) == 0) {
    return;
  }
  if ((int)uVar1 < 0) {
    if (paused != 0) goto LAB_80089dbc;
    dVar2 = 1.0;
    dt = icutscene->cframe + icutscene->rate;
    icutscene->cframe = dt;
    if (1.0 < dt) {
      icutscene->cframe = 1.0;
      goto LAB_80089dbc;
    }
    if (dt < (cutscene->nframes + 1.0) - 1.0) goto LAB_80089dbc;
    endfn = (code *)icutscene->endfn;
    if (endfn != (code *)0x0) {
      icutscene->endfn = (void *)0x0;
      (*endfn)(icutscene);
    }
    if (icutscene->next_to_play != (instNUGCUTSCENE_s *)0x0) {
      dVar2 = (double)(icutscene->cframe -
                      (float)((double)(float)((double)cutscene->nframes + dVar2) - dVar2));
      if ((double)cutscene->nframes < dVar2) {
        dVar2 = 0.0;
      }
      instNuGCutSceneEnd(icutscene);
      instNuGCutSceneStart(icutscene->next_to_play);
      icutscene->next_to_play->cframe = (float)((double)icutscene->next_to_play->cframe + dVar2);
      *(uint *)&icutscene->next_to_play->field_0x6c =
           *(uint *)&icutscene->next_to_play->field_0x6c | 0x80000000;
      instNuGCutSceneClipTest(icutscene->next_to_play);
      *(uint *)&icutscene->next_to_play->field_0x6c =
           *(uint *)&icutscene->next_to_play->field_0x6c | 0x400000;
      icutscene->next_to_play = (instNUGCUTSCENE_s *)0x0;
      return;
    }
    if ((*(uint *)&icutscene->field_0x6c & 0x20000000) == 0) {
      instNuGCutSceneEnd(icutscene);
      goto LAB_80089dbc;
    }
    dVar2 = (double)(icutscene->cframe -
                    (float)((double)(float)((double)cutscene->nframes + dVar2) - dVar2));
    if ((double)cutscene->nframes < dVar2) {
      dVar2 = 0.0;
    }
    instNuGCutSceneStart(icutscene);
    uVar1 = *(uint *)&icutscene->field_0x6c;
    icutscene->cframe = (float)((double)icutscene->cframe + dVar2);
  }
  *(uint *)&icutscene->field_0x6c = uVar1 | 0x80000000;
LAB_80089dbc:
  if ((do_cameras != 0) && (icutscene->icameras != (instNUGCUTCAMSYS_s *)0x0)) {
    instNuGCutCamSysUpdate(icutscene,icutscene->cframe);
  }
  if (icutscene->itriggersys != (instNUGCUTTRIGGERSYS_s *)0x0) {
    instNuGCutTriggerSysUpdate(icutscene,icutscene->cframe);
  }
  if ((do_rigids != 0) && (icutscene->irigids != (instNUGCUTRIGIDSYS_s *)0x0)) {
    instNuGCutRigidSysUpdate(icutscene,icutscene->cframe);
  }
  return;
}


void instNuGCutSceneRender(instNUGCUTSCENE_s *icutscene)

{
  uint uVar1;
  
  uVar1 = *(uint *)&icutscene->field_0x6c;
  if ((uVar1 & 0x800000) == 0) {
    if (((uVar1 & 0x40000000) != 0) && ((uVar1 & 0x1000000) != 0)) {
      if ((do_rigids != 0) && (icutscene->irigids != (instNUGCUTRIGIDSYS_s *)0x0)) {
        instNuGCutRigidSysRender(icutscene,icutscene->cframe);
      }
      if ((do_chars != 0) && (icutscene->ichars != (instNUGCUTCHARSYS_s *)0x0)) {
        instNuGCutCharSysRender((double)icutscene->cframe,icutscene);
      }
      if ((do_locator != 0) && (icutscene->ilocators != (instNUGCUTLOCATORSYS_s *)0x0)) {
        instNuGCutLocatorSysUpdate(icutscene,icutscene->cframe);
      }
    }
  }
  return;
}


void NuGCutCamsSysFixPtrs(NUGCUTSCENE_s *cutscene,int address_offset)

{
  nuanimdata2_s *camanim;
  NUSTATEANIM_s *camchanges;
  NUGCUTCAM_s *cutcam;
  NUGCUTCAMSYS_s *camsys;
  
  if (cutscene->cameras != (NUGCUTCAMSYS_s *)0x0) {
    camsys = (NUGCUTCAMSYS_s *)(cutscene->cameras->remap_table + address_offset + -0x11);
    cutcam = (NUGCUTCAM_s *)0x0;
    cutscene->cameras = camsys;
    if (camsys->cutcams != (NUGCUTCAM_s *)0x0) {
      cutcam = (NUGCUTCAM_s *)(camsys->cutcams->pad + address_offset + -0x43);
    }
    camsys->cutcams = cutcam;
    if (camsys->camanim != (nuanimdata2_s *)0x0) {
      camanim = NuAnimData2FixPtrs(camsys->camanim,address_offset);
      camsys->camanim = camanim;
    }
    camchanges = StateAnimFixPtrs(camsys->camchanges,address_offset);
    camsys->camchanges = camchanges;
  }
  return;
}

void instNuGCutSceneCalculateCentre(instNUGCUTSCENE_s *icutscene,numtx_s *mtx)

{
  NUGCUTSCENE_s *cut;
  NUGCUTBBOX_s *bbox;
  
  cut = icutscene->cutscene;
  bbox = cut->bbox;
  if (bbox == (NUGCUTBBOX_s *)0x0) {
    (icutscene->centre).z = 0.0;
    (icutscene->centre).x = 0.0;
    (icutscene->centre).y = 0.0;
  }
  else {
    (icutscene->centre).x = ((bbox->max).x + (bbox->min).x) * 0.5;
    (icutscene->centre).y = ((cut->bbox->max).y + (cut->bbox->min).y) * 0.5;
    (icutscene->centre).z = ((cut->bbox->max).z + (cut->bbox->min).z) * 0.5;
  }
  if (mtx != (numtx_s *)0x0) {
    NuVecMtxTransform(&icutscene->centre,&icutscene->centre,mtx);
  }
  return;
}

instNUGCUTCAMSYS_s * instNuCGutCamSysCreate(NUGCUTCAMSYS_s *cameras,variptr_u *buff)  //need correction!
{
  instNUGCUTCAMSYS_s *scam;
  instNUGCUTCAM_s *icam;
  
  scam = (instNUGCUTCAMSYS_s *)0x0;
  if (((cameras != (NUGCUTCAMSYS_s *)0x0) && (cameras->ncutcams != 0)) && (buff != (variptr_u *)0x 0)
     ) {
    scam = (instNUGCUTCAMSYS_s *)((uint)((int)&buff->vec4->w + 3) & 0xfffffff0);
    buff->voidptr = scam + 1;
    memset(scam,0,0x10);
    icam = (instNUGCUTCAM_s *)((uint)((int)&buff->vec4->w + 3) & 0xfffffff0);
    buff->intaddr = (uint)icam;
    scam->icutcams = icam;
    buff->voidptr = (void *)((int)buff->voidptr + cameras->ncutcams * 4);
    memset(scam->icutcams,0,cameras->ncutcams << 2);
  }
  return scam;
}



void instNuGCutCamSysUpdate(instNUGCUTSCENE_s *icutscene,float current_frame)

{
  int state;
  NUERRORFUNC *er;
  NUGCUTCAMTGT_s *cutcamtgt;
  numtx_s *ptr3;
  nuanimdata2_s *camanim;
  numtx_s *ptrCcamMtx;
  numtx_s *ptrptr;
  instNUGCUTCAM_s *icutcam;
  NUGCUTCAM_s *cutcam2;
  byte nexttgt;
  NUGCUTCAMSYS_s *camsys;
  instNUGCUTCAMSYS_s *icamSys;
  NUGCUTCAM_s *cutcam;
  bool c;
  double dVar1;
  double dVar2;
  nuanimtime_s atime;
  numtx_s aim_mtx;
  Quat q_to;
  Quat q_result;
  Quat q_from;
  float local_58;
  float local_54;
  float local_50;
  uchar local_48 [28];
  numtx_s *cCamMtx;
  float frame;
  char remap;
  
  dVar2 = (double)current_frame;
  icamSys = icutscene->icameras;
  camsys = icutscene->cutscene->cameras;
  if ((camsys->camchanges != (NUSTATEANIM_s *)0x0) &&
     (state = StateAnimEvaluate(camsys->camchanges,&icamSys->next_switch,local_48,current_frame),
     state != 0)) {
    icamSys->current_camera = local_48[0];
  }
  nexttgt = icamSys->next_tgt_ix;
  if ((uint)nexttgt < (uint)icamSys->ntgts) {
    cutcamtgt = icamSys->tgts;
    frame = cutcamtgt[nexttgt].frame;
    while ((double)frame <= dVar2) {
      icutcam = icamSys->icutcams;
      remap = camsys->remap_table[cutcamtgt[nexttgt].camid];
      icutcam[remap].flags = icutcam[remap].flags | 2;
      icutcam[remap].tgt_ix = icamSys->next_tgt_ix;
      nexttgt = icamSys->next_tgt_ix + 1;
      icamSys->next_tgt_ix = nexttgt;
      if ((uint)icamSys->ntgts <= (uint)nexttgt) break;
      cutcamtgt = icamSys->tgts;
      frame = cutcamtgt[nexttgt].frame;
    }
  }
  nexttgt = icamSys->next_tgt_ix;
  if (nexttgt != 0) {
    cutcamtgt = icamSys->tgts;
    frame = cutcamtgt[nexttgt - 1].frame;
    while (dVar2 < (double)frame) {
      icutcam = icamSys->icutcams;
      icamSys->next_tgt_ix = (uchar)(nexttgt - 1);
      remap = camsys->remap_table[cutcamtgt->pad[((nexttgt - 1) * 0x10 & 0xff0) - 1]];
      icutcam[remap].flags = icutcam[remap].flags | 2;
      icutcam[remap].tgt_ix = icamSys->next_tgt_ix;
      nexttgt = icamSys->next_tgt_ix;
      if (nexttgt == 0) break;
      cutcamtgt = icamSys->tgts;
      frame = cutcamtgt[nexttgt - 1].frame;
    }
  }
  if (-1 < icamSys->current_camera) {
    if ((int)camsys->ncutcams <= (int)icamSys->current_camera) {
      er = NuErrorProlog("C:/source/crashwoc/code/gamelib/gcutscn.c",0x2e3);
      (*er)("instNuGCutCamSysUpdate: internal error");
    }
    icutcam = icamSys->icutcams + icamSys->current_camera;
    cutcam = camsys->cutcams + icamSys->current_camera;
    if (((cutcam->flags & 1) == 0) || (camsys->camanim->nnodes <= (short)(ushort)cutcam->anim_ix))  {
      state = 0x30;
      cCamMtx = &cutscenecammtx;
      do {
        cutcam2 = cutcam;
        ptrCcamMtx = cCamMtx;
        state = state + -0x18;
        ptrCcamMtx->_00 = (cutcam2->mtx)._00;
        ptrCcamMtx->_01 = (cutcam2->mtx)._01;
        ptrCcamMtx->_02 = (cutcam2->mtx)._02;
        ptrCcamMtx->_03 = (cutcam2->mtx)._03;
        ptrCcamMtx->_10 = (cutcam2->mtx)._10;
        cutcam = (NUGCUTCAM_s *)&(cutcam2->mtx)._12;
        ptrCcamMtx->_11 = (cutcam2->mtx)._11;
        cCamMtx = (numtx_s *)&ptrCcamMtx->_12;
      } while (state != 0);
      ptrCcamMtx->_12 = *(float *)cutcam;
      ptrCcamMtx->_13 = (cutcam2->mtx)._13;
      ptrCcamMtx->_20 = (cutcam2->mtx)._20;
      ptrCcamMtx->_21 = (cutcam2->mtx)._21;
    }
    else {
      NuAnimData2CalcTime(camsys->camanim,(float)dVar2,&atime);
      camanim = camsys->camanim;
      state = (int)camanim->ncurves * (uint)cutcam->anim_ix;
      NuAnimCurve2SetApplyToMatrix
                (camanim->curves + state,camanim->curveflags + state,
                 camanim->curvesetflags[cutcam->anim_ix],&atime,&cutscenecammtx);
    }
    if ((*(uint *)&icutscene->field_0x6c & 0x8000000) != 0) {
      NuMtxMul(&cutscenecammtx,&cutscenecammtx,&icutscene->mtx);
    }
    set_cutscenecammtx = 1;
    if ((icutcam->flags & 2) != 0) {
      cutcamtgt = icamSys->tgts + icutcam->tgt_ix;
      dVar1 = (double)cutcamtgt->nframes;
      NuFabs(cutcamtgt->nframes);
      frame = NuFsign(cutcamtgt->nframes);
      c = frame < 0.0;
      if ((!c) || (dVar2 < (double)(float)((double)cutcamtgt->frame + dVar1))) {
        local_58 = cutscenecammtx._30;
        local_54 = cutscenecammtx._31;
        local_50 = cutscenecammtx._32;
        state = 0x30;
        cCamMtx = &aim_mtx;
        ptrCcamMtx = &cutscenecammtx;
        do {
          ptrptr = ptrCcamMtx;
          ptr3 = cCamMtx;
          state = state + -0x18;
          ptr3->_00 = ptrptr->_00;
          ptr3->_01 = ptrptr->_01;
          ptr3->_02 = ptrptr->_02;
          ptr3->_03 = ptrptr->_03;
          ptr3->_10 = ptrptr->_10;
          ptr3->_11 = ptrptr->_11;
          cCamMtx = (numtx_s *)&ptr3->_12;
          ptrCcamMtx = (numtx_s *)&ptrptr->_12;
        } while (state != 0);
        ptr3->_12 = ptrptr->_12;
        ptr3->_13 = ptrptr->_13;
        ptr3->_20 = ptrptr->_20;
        ptr3->_21 = ptrptr->_21;
        NuMtxLookAtZ(&aim_mtx,cutcamtgt->tgt);
        if (dVar1 <= 0.009999999776482582) {
          dVar2 = 1.0;
        }
        else {
          dVar2 = (double)(float)((double)(float)(dVar2 - (double)cutcamtgt->frame) / dVar1);
          if (dVar2 <= 1.0) {
            if (dVar2 < 0.0) {
              c = false;
            }
          }
          else {
            c = true;
          }
        }
        if (c) {
          dVar2 = (double)(float)(1.0 - dVar2);
        }
        NuMtxToQuat(&aim_mtx,&q_result);
        NuMtxToQuat(&cutscenecammtx,&q_to);
        NuQuatSlerp((float)dVar2,&q_from,&q_to,&q_result);
        NuQuatToMtx(&q_from,&cutscenecammtx);
        cutscenecammtx._30 = local_58;
        cutscenecammtx._32 = local_50;
        cutscenecammtx._31 = local_54;
      }
      else {
        icutcam->flags = icutcam->flags & 0xfd;
      }
    }
  }
  return;
}


void instNuGCutCamSysStart(instNUGCUTCAMSYS_s *icamsys,NUGCUTCAMSYS_s *camsys)

{
  uint i;
  instNUGCUTCAM_s *icutcam;
  int n;
  char camid;
  
  i = 0;
  icamsys->next_switch = '\0';
  camid = camsys->initial_camid;
  icamsys->next_tgt_ix = '\0';
  icamsys->current_camera = camid;
  if (camsys->ncutcams != 0) {
    do {
      icutcam = icamsys->icutcams;
      n = i + 1;
      icutcam[i].flags = icutcam[i].flags & 0xfd;
      icutcam[i].tgt_ix = '\0';
      i = n;
    } while (n < camsys->ncutcams);
    return;
  }
  return;
}


void NuGCutLocatorSysFixPtrs(NUGCUTSCENE_s *cutscene,int address_offset)		//TODO

{
  NUGCUTLOCATOR_s *locator;
  NUGCUTLOCATORTYPE_s *loctype;
  nuanimdata2_s *adat;
  NUGCUTLOCATORSYS_s *locatorsys;
  int iVar1;
  NUGCUTLOCATORSYS_s *locsys2;
  int i;
  
  locatorsys = cutscene->locators;
  if (locatorsys != (NUGCUTLOCATORSYS_s *)0x0) {
    locsys2 = (NUGCUTLOCATORSYS_s *)(locatorsys->pad + address_offset + -10);
    i = 0;
    cutscene->locators = locsys2;
    if (*(int *)(locatorsys->pad + address_offset + -10) != 0) {
      i = *(int *)(locatorsys->pad + address_offset + -10) + address_offset;
    }
    *(int *)(locatorsys->pad + address_offset + -10) = i;
    if ((i != 0) && (i = 0, locsys2->nlocators != '\0')) {
      iVar1 = 0;
      do {
        locator = locsys2->locators;
        adat = *(nuanimdata2_s **)((int)&locator->anim + iVar1);
        if (adat != (nuanimdata2_s *)0x0) {
          adat = NuAnimData2FixPtrs(adat,address_offset);
          *(nuanimdata2_s **)((int)&locator->anim + iVar1) = adat;
        }
        i = i + 1;
        iVar1 = iVar1 + 0x64;
      } while (i < (int)(uint)locsys2->nlocators);
    }
    loctype = (NUGCUTLOCATORTYPE_s *)0x0;
    if (locsys2->locator_types != (NUGCUTLOCATORTYPE_s *)0x0) {
      loctype = (NUGCUTLOCATORTYPE_s *)(locsys2->locator_types->flags + address_offset + -4);
    }
    locsys2->locator_types = loctype;
    if ((loctype != (NUGCUTLOCATORTYPE_s *)0x0) && (i = 0, locsys2->ntypes != '\0')) {
      iVar1 = 0;
      do {
        i = i + 1;
        *(char **)(locsys2->locator_types->flags + iVar1 + -4) =
             cutscene->name_table + *(int *)(locsys2->locator_types->flags + iVar1 + -4) + -1;
        iVar1 = iVar1 + 0xc;
      } while (i < (int)(uint)locsys2->ntypes);
    }
  }
  return;
}

void NuGCutLocatorCalcMtx
               (NUGCUTLOCATOR_s *locator,float current_frame,numtx_s *mtx,nuanimtime_s *atime)

{
  NUGCUTLOCATOR_s *cutLoc;
  numtx_s *m;
  int i;
  nuanimdata2_s *adat;
  char *curveflags;
  nuanimcurve2_s *curves;
  nuvec_s t;
  nuvec_s r;
  nuangvec_s rf;
  
  adat = locator->anim;
  if (adat == (nuanimdata2_s *)0x0) {
    i = 0x30;
    do {
      m = mtx;
      cutLoc = locator;
      i = i + -0x18;
      m->_00 = (cutLoc->mtx)._00;
      m->_01 = (cutLoc->mtx)._01;
      m->_02 = (cutLoc->mtx)._02;
      m->_03 = (cutLoc->mtx)._03;
      m->_10 = (cutLoc->mtx)._10;
      locator = (NUGCUTLOCATOR_s *)&(cutLoc->mtx)._12;
      m->_11 = (cutLoc->mtx)._11;
      mtx = (numtx_s *)&m->_12;
    } while (i != 0);
    m->_12 = *(float *)locator;
    m->_13 = (cutLoc->mtx)._13;
    m->_20 = (cutLoc->mtx)._20;
    m->_21 = (cutLoc->mtx)._21;
  }
  else {
    curves = adat->curves;
    curveflags = adat->curveflags;
    if ((*adat->curvesetflags & 1U) == 0) {
      NuMtxSetIdentity(mtx);
    }
    else {
      r.x = NuAnimCurve2CalcVal(curves + 3,atime,(int)curveflags[3]);
      r.y = NuAnimCurve2CalcVal(curves + 4,atime,(int)curveflags[4]);
      r.z = NuAnimCurve2CalcVal(curves + 5,atime,(int)curveflags[5]);
      rf.x = (int)(r.x * 10430.38);
      rf.y = (int)(r.y * 10430.38);
      rf.z = (int)(r.z * 10430.38);
      NuMtxSetRotateXYZ(mtx,&rf);
    }
    t.x = NuAnimCurve2CalcVal(curves,atime,(int)*curveflags);
    t.y = NuAnimCurve2CalcVal(curves + 1,atime,(int)curveflags[1]);
    t.z = NuAnimCurve2CalcVal(curves + 2,atime,(int)curveflags[2]);
    NuMtxTranslate(mtx,&t);
    mtx->_02 = -mtx->_02;
    mtx->_12 = -mtx->_12;
    mtx->_20 = -mtx->_20;
    mtx->_21 = -mtx->_21;
    mtx->_23 = -mtx->_23;
    mtx->_32 = -mtx->_32;
    NuMtxTranslate(mtx,(nuvec_s *)&(locator->mtx)._30);
  }
  return;
}


int NuGCutLocatorIsVisble
              (NUGCUTLOCATOR_s *locator,float current_frame,nuanimtime_s *atime,float *rate)

{
  nuanimdata2_s *adat;
  char *curveflags;
  nuanimcurve2_s *curves;
  int is_visible;
  float fVar1;
  
  adat = locator->anim;
  if (adat == (nuanimdata2_s *)0x0) {
    is_visible = locator->flags & 8;
    if (((locator->flags & 8) != 0) && (rate != (float *)0x0)) {
      *rate = locator->rate;
    }
  }
  else {
    curveflags = adat->curveflags;
    curves = adat->curves;
    fVar1 = NuAnimCurve2CalcVal(curves + 7,atime,(int)curveflags[7]);
    is_visible = (int)fVar1;
    if ((is_visible != 0) && (rate != (float *)0x0)) {
      fVar1 = NuAnimCurve2CalcVal(curves + 6,atime,(int)curveflags[6]);
      *rate = fVar1;
    }
  }
  return is_visible;
}


instNUGCUTLOCATORSYS_s * NuCGutLocatorSysCreateInst(NUGCUTLOCATORSYS_s *locators,variptr_u *buff)

{
  instNUGCUTLOCATORSYS_s *ilocsys;
  instNUGCUTLOCATOR_s *iLoc;
  
  ilocsys = (instNUGCUTLOCATORSYS_s *)0x0;
  if (((locators != (NUGCUTLOCATORSYS_s *)0x0) && (locators->nlocators != '\0')) &&
     (buff != (variptr_u *)0x0)) {
    ilocsys = (instNUGCUTLOCATORSYS_s *)(buff->intaddr + 0xf & 0xfffffff0);
    buff->voidptr = ilocsys + 1;
    ilocsys->ilocators = (instNUGCUTLOCATOR_s *)0x0;
    iLoc = (instNUGCUTLOCATOR_s *)(buff->intaddr + 0xf & 0xfffffff0);
    buff->intaddr = (uint)iLoc;
    ilocsys->ilocators = iLoc;
    buff->voidptr = (void *)((int)buff->voidptr + (uint)locators->nlocators * 8);
    memset(ilocsys->ilocators,0,(uint)locators->nlocators << 3);
  }
  return ilocsys;
}


void instNuGCutLocatorSysUpdate(instNUGCUTSCENE_s *icutscene,float current_frame)

{
	
	//TODO
	
}


void instNuGCutLocatorSysStart(instNUGCUTLOCATORSYS_s *ilocatorsys,NUGCUTLOCATORSYS_s *locatorsys )

{
  uint i;
  uint n;
  
  i = 0;
  if (locatorsys->nlocators != '\0') {
    do {
      n = i + 1;
      ilocatorsys->ilocators[i].time = 0.0;
      i = n;
    } while (n < locatorsys->nlocators);
    return;
  }
  return;
}

void NuGCutLocatorSysFixUp(NUGCUTLOCATORSYS_s *locatorsys)

{
	//TODO
}

void instNuGCutLocatorUpdate
               (instNUGCUTSCENE_s *icutscene,NUGCUTLOCATORSYS_s *locatorsys,
               instNUGCUTLOCATOR_s *ilocator,NUGCUTLOCATOR_s *locator,float current_frame,
               numtx_s *wm)

{
  uint ix;
  uchar flags;
  
  flags = locatorsys->locator_types[locator->locator_type].flags[0];
  if ((flags & 1) == 0) {
    if (((flags & 2) != 0) &&
       (ix = (uint)locatorsys->locator_types[locator->locator_type].ix, ix != 0xffff)) {
      (*(code *)locatorfns[ix].void_fn)();
    }
  }
  else {
    instNuGCutDebrisLocatorUpdate(locatorsys,ilocator,locator,current_frame,wm);
  }
  return;
}


void instNuGCutDebrisLocatorUpdate (NUGCUTLOCATORSYS_s *locatorsys,instNUGCUTLOCATOR_s *ilocator, NUGCUTLOCATOR_s *locator,float current_frame,numtx_s *wm)

{
	//TODO
}

void NuGCutRigidSysFixPtrs(NUGCUTSCENE_s *cutscene,int address_offset)

{
  NUGCUTRIGID_s *rigid;
  int iVar1;
  nuanimdata2_s *adat;
  NUSTATEANIM_s *stateanm;
  NUGCUTRIGIDSYS_s *rigidsys;
  int n;
  NUGCUTRIGIDSYS_s *rsys2;
  int i;
  
  rigidsys = cutscene->rigids;
  if (rigidsys != (NUGCUTRIGIDSYS_s *)0x0) {
    rsys2 = (NUGCUTRIGIDSYS_s *)(rigidsys->pad + address_offset + -6);
    i = 0;
    cutscene->rigids = rsys2;
    if (*(int *)(rigidsys->pad + address_offset + -6) != 0) {
      i = *(int *)(rigidsys->pad + address_offset + -6) + address_offset;
    }
    *(int *)(rigidsys->pad + address_offset + -6) = i;
    if ((i != 0) && (i = 0, rsys2->nrigids != 0)) {
      n = 0;
      do {
        rigid = rsys2->rigids;
        iVar1 = *(int *)((int)&rigid->name + n);
        if (iVar1 != 0) {
          *(char **)((int)&rigid->name + n) = cutscene->name_table + iVar1 + -1;
        }
        adat = *(nuanimdata2_s **)((int)&rigid->anim + n);
        if (adat != (nuanimdata2_s *)0x0) {
          adat = NuAnimData2FixPtrs(adat,address_offset);
          *(nuanimdata2_s **)((int)&rigid->anim + n) = adat;
        }
        i = i + 1;
        stateanm = StateAnimFixPtrs(*(NUSTATEANIM_s **)((int)&rigid->visibiltyanim + n),
                                    address_offset);
        *(NUSTATEANIM_s **)((int)&rigid->visibiltyanim + n) = stateanm;
        n = n + 0x5c;
      } while (i < (int)(uint)rsys2->nrigids);
    }
  }
  return;
}

void NuGCutRigidSysFixUp(NUGCUTSCENE_s *cutscene,nugscn_s *scene)

{
  NUGCUTRIGID_s *rigid;
  int find;
  NUGCUTLOCATORSYS_s *loc;
  int j;
  NUGCUTRIGIDSYS_s *rigidsys;
  int i;
  
  loc = cutscene->locators;
  rigidsys = cutscene->rigids;
  if ((scene != (nugscn_s *)0x0) && (i = 0, rigidsys->nrigids != 0)) {
    j = 0;
    do {
      rigid = rigidsys->rigids;
      find = NuSpecialFind(scene,(nuhspecial_s *)((int)&(rigid->special).scene + j),
                           *(char **)((int)&rigid->name + j));
      if (find != 0) {
        (&rigid->flags)[j] = (&rigid->flags)[j] | 4;
      }
      if (((&rigid->nlocators)[j] == '\0') ||
         (find = *(int *)((int)&rigid->locators + j), 0xfe < find)) {
        (&rigid->first_locator)[j] = 0xff;
      }
      else {
        (&rigid->first_locator)[j] = *(uchar *)((int)&rigid->locators + j + 3);
        *(NUGCUTLOCATOR_s **)((int)&rigid->locators + j) = loc->locators + find;
      }
      i = i + 1;
      j = j + 0x5c;
    } while (i < (int)(uint)rigidsys->nrigids);
  }
  return;
}

static instNUGCUTRIGIDSYS_s* instNuCGutRigidSysCreate(NUGCUTSCENE_s* cutscene, nugscn_s* gscene, variptr_u* buff)
{
	//TODO
}




void NuGCutRigidCalcMtx(NUGCUTRIGID_s *rigid,float current_frame,numtx_s *mtx)

{
  NUGCUTRIGID_s *cutrigid;
  numtx_s *m;
  nuanimdata2_s *animdata;
  int i;
  nuanimcurve2_s *curves;
  char *curveflags;
  nuanimtime_s atime;
  nuvec_s t;
  nuvec_s r;
  nuangvec_s rf;
  nuvec_s scale;
  uchar curvesetflags;
  
  animdata = rigid->anim;
  if (animdata == (nuanimdata2_s *)0x0) {
    i = 0x30;
    do {
      m = mtx;
      cutrigid = rigid;
      i = i + -0x18;
      m->_00 = (cutrigid->mtx)._00;
      m->_01 = (cutrigid->mtx)._01;
      m->_02 = (cutrigid->mtx)._02;
      m->_03 = (cutrigid->mtx)._03;
      m->_10 = (cutrigid->mtx)._10;
      rigid = (NUGCUTRIGID_s *)&(cutrigid->mtx)._12;
      m->_11 = (cutrigid->mtx)._11;
      mtx = (numtx_s *)&m->_12;
    } while (i != 0);
    m->_12 = *(float *)rigid;
    m->_13 = (cutrigid->mtx)._13;
    m->_20 = (cutrigid->mtx)._20;
    m->_21 = (cutrigid->mtx)._21;
  }
  else {
    curves = animdata->curves;
    curvesetflags = *animdata->curvesetflags;
    curveflags = animdata->curveflags;
    NuAnimData2CalcTime(animdata,current_frame,&atime);
    if ((curvesetflags & 1) == 0) {
      NuMtxSetIdentity(mtx);
    }
    else {
      r.x = NuAnimCurve2CalcVal(curves + 3,&atime,(int)curveflags[3]);
      r.y = NuAnimCurve2CalcVal(curves + 4,&atime,(int)curveflags[4]);
      r.z = NuAnimCurve2CalcVal(curves + 5,&atime,(int)curveflags[5]);
      rf.x = (int)(r.x * 10430.38);
      rf.y = (int)(r.y * 10430.38);
      rf.z = (int)(r.z * 10430.38);
      NuMtxSetRotateXYZ(mtx,&rf);
    }
    if ((curvesetflags & 8) != 0) {
      scale.x = NuAnimCurve2CalcVal(curves + 6,&atime,(int)curveflags[6]);
      scale.y = NuAnimCurve2CalcVal(curves + 7,&atime,(int)curveflags[7]);
      scale.z = NuAnimCurve2CalcVal(curves + 8,&atime,(int)curveflags[8]);
      NuMtxPreScale(mtx,&scale);
    }
    t.x = NuAnimCurve2CalcVal(curves,&atime,(int)*curveflags);
    t.y = NuAnimCurve2CalcVal(curves + 1,&atime,(int)curveflags[1]);
    t.z = NuAnimCurve2CalcVal(curves + 2,&atime,(int)curveflags[2]);
    NuMtxTranslate(mtx,&t);
    mtx->_02 = -mtx->_02;
    mtx->_12 = -mtx->_12;
    mtx->_20 = -mtx->_20;
    mtx->_21 = -mtx->_21;
    mtx->_23 = -mtx->_23;
    mtx->_32 = -mtx->_32;
    NuMtxTranslate(mtx,(nuvec_s *)&(rigid->mtx)._30);
  }
  return;
}


void instNuGCutRigidSysStart(instNUGCUTRIGIDSYS_s *irigidsys,NUGCUTRIGIDSYS_s *rigidsys)

{
  int iVar1;
  uint i;
  
  i = 0;
  if (rigidsys->nrigids == 0) {
    return;
  }
  iVar1 = 0;
  do {
    i = i + 1;
    irigidsys->irigids->pad[iVar1 + -1] = '\0';
    iVar1 = iVar1 + 0xc;
  } while (i < rigidsys->nrigids);
  return;
}

void instNuGCutRigidSysUpdate(instNUGCUTSCENE_s *icutscene,float current_frame)

{
  nuinstflags_s flags;
  int iVar1;
  nuinstance_s *tinst;
  instNUGCUTRIGID_s *irigid;
  numtx_s *mtmp;
  instNUGCUTRIGIDSYS_s *irigidsys;
  NUGCUTSCENE_s *cutscene;
  NUGCUTRIGIDSYS_s *rigidsys;
  instNUGCUTLOCATORSYS_s *ilocatorsys;
  NUGCUTRIGID_s *rigid;
  int i;
  NUGCUTLOCATORSYS_s *locatorsys;
  int j;
  double dVar2;
  numtx_s mtx;
  uchar local_40 [20];
  nuinstance_s *inst;
  numtx_s *m;
  
  dVar2 = (double)current_frame;
  i = 0;
  cutscene = icutscene->cutscene;
  irigidsys = icutscene->irigids;
  rigidsys = cutscene->rigids;
  if (rigidsys->nrigids != 0) {
    do {
      irigid = irigidsys->irigids;
      rigid = rigidsys->rigids + i;
      if (((rigid->flags & 2) == 0) && ((rigid->flags & 4) != 0)) {
        if ((rigid->visibiltyanim != (NUSTATEANIM_s *)0x0) &&
           (j = StateAnimEvaluate(rigid->visibiltyanim,(uchar *)&irigid[i].visibleframeix,local_40 ,
                                  (float)dVar2), j != 0)) {
          if (local_40[0] == '\0') {
            inst = (irigid[i].special.special)->instance;
            flags = (nuinstflags_s)((uint)inst->flags & 0x7fffffff);
          }
          else {
            inst = (irigid[i].special.special)->instance;
            flags = (nuinstflags_s)((uint)inst->flags | 0x80000000);
          }
          inst->flags = flags;
        }
        if ((int)(irigid[i].special.special)->instance->flags < 0) {
          NuGCutRigidCalcMtx(rigid,(float)dVar2,&mtx);
          if ((*(uint *)&icutscene->field_0x6c & 0x8000000) != 0) {
            NuMtxMul(&mtx,&mtx,&icutscene->mtx);
          }
          j = 0x30;
          inst = (irigid[i].special.special)->instance;
          m = &mtx;
          do {
            mtmp = m;
            tinst = inst;
            j = j + -0x18;
            (tinst->matrix)._00 = mtmp->_00;
            (tinst->matrix)._01 = mtmp->_01;
            (tinst->matrix)._02 = mtmp->_02;
            (tinst->matrix)._03 = mtmp->_03;
            (tinst->matrix)._10 = mtmp->_10;
            (tinst->matrix)._11 = mtmp->_11;
            inst = (nuinstance_s *)&(tinst->matrix)._12;
            m = (numtx_s *)&mtmp->_12;
          } while (j != 0);
          *(float *)inst = mtmp->_12;
          (tinst->matrix)._13 = mtmp->_13;
          (tinst->matrix)._20 = mtmp->_20;
          (tinst->matrix)._21 = mtmp->_21;
          if (((int)(irigid[i].special.special)->instance->flags < 0) &&
             (rigid->first_locator != 0xff)) {
            j = 0;
            locatorsys = cutscene->locators;
            ilocatorsys = icutscene->ilocators;
            if (rigid->nlocators != '\0') {
              do {
                iVar1 = (uint)rigid->first_locator + j;
                j = j + 1;
                instNuGCutLocatorUpdate
                          (icutscene,locatorsys,ilocatorsys->ilocators + iVar1,
                           locatorsys->locators + iVar1,(float)dVar2,&mtx);
              } while (j < (int)(uint)rigid->nlocators);
            }
          }
        }
      }
      i = i + 1;
    } while (i < (int)(uint)rigidsys->nrigids);
  }
  return;
}


void instNuGCutRigidSysRender(instNUGCUTSCENE_s *icutscene,float current_frame)

{
  nuinstflags_s flags;
  int iVar1;
  instNUGCUTRIGID_s *irigid;
  nuinstance_s *inst;
  instNUGCUTRIGIDSYS_s *irigidsys;
  NUGCUTSCENE_s *cutscene;
  NUGCUTRIGIDSYS_s *rigidsys;
  instNUGCUTLOCATORSYS_s *ilocatorsys;
  NUGCUTRIGID_s *rigid;
  int i;
  NUGCUTLOCATORSYS_s *locatorsys;
  int j;
  double dVar2;
  numtx_s mtx;
  uchar newvisibilitystate [20];
  
  dVar2 = (double)current_frame;
  i = 0;
  cutscene = icutscene->cutscene;
  irigidsys = icutscene->irigids;
  rigidsys = cutscene->rigids;
  if (rigidsys->nrigids != 0) {
    do {
      irigid = irigidsys->irigids;
      rigid = rigidsys->rigids + i;
      if (((rigid->flags & 2) != 0) && ((rigid->flags & 4) != 0)) {
        if ((rigid->visibiltyanim != (NUSTATEANIM_s *)0x0) &&
           (j = StateAnimEvaluate(rigid->visibiltyanim,(uchar *)&irigid[i].visibleframeix,
                                  newvisibilitystate,(float)dVar2), j != 0)) {
          if (newvisibilitystate[0] == '\0') {
            inst = (irigid[i].special.special)->instance;
            flags = (nuinstflags_s)((uint)inst->flags & 0x7fffffff);
          }
          else {
            inst = (irigid[i].special.special)->instance;
            flags = (nuinstflags_s)((uint)inst->flags | 0x80000000);
          }
          inst->flags = flags;
        }
        if ((int)(irigid[i].special.special)->instance->flags < 0) {
          NuGCutRigidCalcMtx(rigid,(float)dVar2,&mtx);
          if ((*(uint *)&icutscene->field_0x6c & 0x8000000) != 0) {
            NuMtxMul(&mtx,&mtx,&icutscene->mtx);
          }
          if (NuCutSceneRigidCollisionCheck != (NuCutSceneRigidCollisionCheck *)0x0) {
            (*NuCutSceneRigidCollisionCheck)(rigid,&mtx);
          }
          if (rigid->first_locator != 0xff) {
            j = 0;
            locatorsys = cutscene->locators;
            ilocatorsys = icutscene->ilocators;
            if (rigid->nlocators != '\0') {
              do {
                iVar1 = (uint)rigid->first_locator + j;
                j = j + 1;
                instNuGCutLocatorUpdate
                          (icutscene,locatorsys,ilocatorsys->ilocators + iVar1,
                           locatorsys->locators + iVar1,(float)dVar2,&mtx);
              } while (j < (int)(uint)rigid->nlocators);
            }
          }
        }
      }
      i = i + 1;
    } while (i < (int)(uint)rigidsys->nrigids);
  }
  return;
}


void NuGCutCharSysFixPtrs(NUGCUTSCENE_s *cutscene,int address_offset)

{
	//TODO
}


void NuGCutCharSysFixUp(NUGCUTSCENE_s *cutscene)

{
  if (NuCutSceneFindCharacters != (NuCutSceneFindCharacters *)0x0) {
    (*NuCutSceneFindCharacters)(cutscene);
  }
  return;
}


instNUGCUTCHARSYS_s * instNuCGutCharSysCreate(NUGCUTSCENE_s *cutscene,variptr_u *buff)		//TODO
{
  NUGCUTCHAR_s *cutchar;
  char *pcVar1;
  int iVar2;
  instNUGCUTCHAR_s *icutchar;
  instNUGCUTCHARSYS_s *ichars;
  int i;
  int iVar3;
  NUGCUTCHARSYS_s *chars;
  int iVar4;
  
  chars = cutscene->chars;
  ichars = (instNUGCUTCHARSYS_s *)0x0;
  if (((chars != (NUGCUTCHARSYS_s *)0x0) && (chars->nchars != 0)) && (buff != (variptr_u *)0x0)) {
    i = 0;
    ichars = (instNUGCUTCHARSYS_s *)((uint)((int)&buff->vec4->w + 3) & 0xfffffff0);
    buff->voidptr = ichars + 1;
    ichars->ichars = (instNUGCUTCHAR_s *)0x0;
    icutchar = (instNUGCUTCHAR_s *)((uint)((int)&buff->vec4->w + 3) & 0xfffffff0);
    buff->intaddr = (uint)icutchar;
    ichars->ichars = icutchar;
    buff->voidptr = (void *)((int)buff->voidptr + (uint)chars->nchars * 0x18);
    memset(ichars->ichars,0,(uint)chars->nchars * 0x18);
    if (chars->nchars != 0) {
      iVar4 = 0;
      iVar3 = 0;
      do {
        icutchar = ichars->ichars;
        cutchar = chars->chars;
        iVar2 = (int)&icutchar->character + iVar4;
        *(undefined *)(iVar2 + 0x16) = 0xff;
        pcVar1 = cutchar->pad + iVar3 + -0x62;
        *(undefined *)(iVar2 + 0x15) = 0xff;
        if ((pcVar1[0x5c] & 2U) == 0) {
          *(undefined4 *)((int)&icutchar->character + iVar4) = *(undefined4 *)(pcVar1 + 0x50);
        }
        else if (NuCutSceneCharacterCreateData != (NuCutSceneCharacterCreateData *)0x0) {
          (*(code *)NuCutSceneCharacterCreateData)(pcVar1,iVar2,buff);
        }
        i = i + 1;
        iVar4 = iVar4 + 0x18;
        iVar3 = iVar3 + 100;
      } while (i < (int)(uint)chars->nchars);
    }
  }
  return ichars;
}


void instNuGCutCharSysRender(instNUGCUTSCENE_s *icutscene,float current_frame)

{
	//TODO
}


void NuGCutCharAnimProcess (NUGCUTCHAR_s *cutchar,float current_frame,numtx_s *mtx,int *is_visible,uint *animi x, float *animrate,float *blendframes)	//TODO
{
  uint anmIX;
  numtx_s *m;
  nuanimdata2_s *curves;
  NUGCUTCHAR_s *cutchar1;
  NUGCUTCHAR_s *cutchar2;
  int iVar1;
  nuanimcurve2_s *curves2;
  char *curveflags;
  float fVar2;
  nuanimtime_s atime;
  nuvec_s t;
  nuvec_s r;
  nuangvec_s rf;
  double local_30;
  char curvesetflags;
  
  if (cutchar->char_anim == (nuanimdata2_s *)0x0) {
    *is_visible = cutchar->flags & 1;
    if (animix != (uint *)0x0) {
      *animix = (uint)cutchar->animix;
    }
    if (is_visible == (int *)0x0) {
      return;
    }
    iVar1 = 0x30;
    cutchar2 = cutchar;
    do {
      cutchar1 = cutchar2;
      m = mtx;
      iVar1 = iVar1 + -0x18;
      m->_00 = (cutchar1->mtx)._00;
      m->_01 = (cutchar1->mtx)._01;
      m->_02 = (cutchar1->mtx)._02;
      m->_03 = (cutchar1->mtx)._03;
      m->_10 = (cutchar1->mtx)._10;
      cutchar2 = (NUGCUTCHAR_s *)&(cutchar1->mtx)._12;
      m->_11 = (cutchar1->mtx)._11;
      mtx = (numtx_s *)&m->_12;
    } while (iVar1 != 0);
    m->_12 = *(float *)cutchar2;
    m->_13 = (cutchar1->mtx)._13;
    m->_20 = (cutchar1->mtx)._20;
    m->_21 = (cutchar1->mtx)._21;
    if (animrate != (float *)0x0) {
      *animrate = cutchar->animrate;
    }
    if (blendframes == (float *)0x0) {
      return;
    }
  }
  else {
    NuAnimData2CalcTime(cutchar->char_anim,current_frame,&atime);
    curves = cutchar->char_anim;
    curvesetflags = *curves->curvesetflags;
    curves2 = curves->curves;
    curveflags = curves->curveflags;
    if (curves->ncurves < 7) {
      *is_visible = cutchar->flags & 1;
    }
    else {
      fVar2 = NuAnimCurve2CalcVal(curves2 + 6,&atime,(int)curveflags[6]);
      *is_visible = (int)fVar2;
    }
    if (animix != (uint *)0x0) {
      if (cutchar->char_anim->ncurves < 8) {
        anmIX = (uint)cutchar->animix;
      }
      else {
        fVar2 = NuAnimCurve2CalcVal(curves2 + 7,&atime,(int)curveflags[7]);
        if (0.0 <= fVar2) {
          if (2.147484e+09 <= fVar2) {
            anmIX = (int)(fVar2 - 2.147484e+09) ^ 0x80000000;
          }
          else {
            anmIX = (uint)fVar2;
          }
        }
        else {
          anmIX = 0xff;
        }
      }
      *animix = anmIX;
    }
    if (*is_visible == 0) {
      return;
    }
    if ((curvesetflags & 1U) == 0) {
      NuMtxSetIdentity(mtx);
    }
    else {
      r.x = NuAnimCurve2CalcVal(curves2 + 3,&atime,(int)curveflags[3]);
      r.y = NuAnimCurve2CalcVal(curves2 + 4,&atime,(int)curveflags[4]);
      r.z = NuAnimCurve2CalcVal(curves2 + 5,&atime,(int)curveflags[5]);
      rf.x = (int)(r.x * 10430.38);
      rf.y = (int)(r.y * 10430.38);
      rf.z = (int)(r.z * 10430.38);
      NuMtxSetRotateXYZ(mtx,&rf);
    }
    t.x = NuAnimCurve2CalcVal(curves2,&atime,(int)*curveflags);
    t.y = NuAnimCurve2CalcVal(curves2 + 1,&atime,(int)curveflags[1]);
    t.z = NuAnimCurve2CalcVal(curves2 + 2,&atime,(int)curveflags[2]);
    NuMtxTranslate(mtx,&t);
    mtx->_02 = -mtx->_02;
    mtx->_12 = -mtx->_12;
    mtx->_32 = -mtx->_32;
    mtx->_20 = -mtx->_20;
    mtx->_21 = -mtx->_21;
    mtx->_23 = -mtx->_23;
    if (animrate != (float *)0x0) {
      if (cutchar->char_anim->ncurves < 10) {
        *animrate = cutchar->animrate;
      }
      else {
        fVar2 = NuAnimCurve2CalcVal(curves2 + 9,&atime,(int)curveflags[9]);
        *animrate = fVar2;
      }
    }
    if (blendframes == (float *)0x0) {
      return;
    }
    if (8 < cutchar->char_anim->ncurves) {
      fVar2 = NuAnimCurve2CalcVal(curves2 + 8,&atime,(int)curveflags[8]);
      *blendframes = fVar2;
      return;
    }
  }
  local_30 = (double)CONCAT44(0x43300000,(uint)cutchar->blendframes);
  *blendframes = (float)(local_30 - 4503599627370496.0);
  return;
}

void NuGCutTriggerSysFixPtrs(NUGCUTSCENE_s *cutscene,int address_offset)	//TODO

{
  int iVar1;
  NUSTATEANIM_s *stateanm;
  NUGCUTTRIGGERSYS_S *triggersys;
  int iVar2;
  NUGCUTTRIGGER_s *trigger;
  int iVar3;
  int i;
  NUGCUTTRIGGERSYS_S *tsys2;
  
  triggersys = cutscene->triggersys;
  if (triggersys != (NUGCUTTRIGGERSYS_S *)0x0) {
    tsys2 = (NUGCUTTRIGGERSYS_S *)((int)&triggersys->ntriggers + address_offset);
    trigger = (NUGCUTTRIGGER_s *)0x0;
    cutscene->triggersys = tsys2;
    if (tsys2->triggers != (NUGCUTTRIGGER_s *)0x0) {
      trigger = (NUGCUTTRIGGER_s *)((int)&tsys2->triggers->ix + address_offset);
    }
    tsys2->triggers = trigger;
    if ((trigger != (NUGCUTTRIGGER_s *)0x0) &&
       (i = 0, 0 < *(int *)((int)&triggersys->ntriggers + address_offset))) {
      iVar3 = 0;
      do {
        trigger = tsys2->triggers;
        iVar2 = 0;
        iVar1 = *(int *)((int)&trigger->triggername + iVar3);
        if (iVar1 != 0) {
          iVar2 = iVar1 + address_offset;
        }
        *(int *)((int)&trigger->triggername + iVar3) = iVar2;
        i = i + 1;
        stateanm = StateAnimFixPtrs(*(NUSTATEANIM_s **)((int)&trigger->enableflag1anim + iVar3),
                                    address_offset);
        *(NUSTATEANIM_s **)((int)&trigger->enableflag1anim + iVar3) = stateanm;
        iVar3 = iVar3 + 0xc;
      } while (i < tsys2->ntriggers);
    }
  }
  return;
}

void NuGCutTriggerSysFixUp(NUGCUTSCENE_s *cutscene,NUTRIGGERSYS_s *triggersys)

{
  int cmp;
  int i;
  NUGCUTTRIGGERSYS_S *cuttriggersys;
  NUGCUTTRIGGER_s *cuttrigger;
  int j;
  
  cuttriggersys = cutscene->triggersys;
  i = 0;
  if (0 < cuttriggersys->ntriggers) {
    do {
      cuttrigger = cuttriggersys->triggers + i;
      i = i + 1;
      for (j = 0; j < (int)(triggersys->triggers).triggername; j = j + 1) {
        cuttrigger->ix = -1;
        cmp = strcasecmp(cuttrigger->triggername,
                         *(char **)(j * 0x34 + (int)(triggersys->triggers).enableflag1anim));
        if (cmp == 0) {
          cuttrigger->ix = (short)j;
          break;
        }
      }
      if (cuttrigger->ix == -1) {
        //e = NuErrorProlog("C:/source/crashwoc/code/gamelib/gcutscn.c",0x815);
        //(*e)("NuGCutTriggerSysFixUp: Unable to find trigger <%s>",cuttrigger->triggername);
      }
    } while (i < cuttriggersys->ntriggers);
  }
  return;
}

instNUGCUTTRIGGERSYS_s *
instNuCGutTriggerSysCreate(NUGCUTSCENE_s *cutscene,instNUTRIGGERSYS_s *itriggersys,variptr_u *buf f)
{
  int ncuttrigs;
  int n;
  instNUGCUTTRIGGERSYS_s *icuttrigsys;
  
  ncuttrigs = cutscene->triggersys->ntriggers;
  icuttrigsys = (instNUGCUTTRIGGERSYS_s *)(buff->intaddr + 0xf & 0xfffffff0);
  buff->voidptr = icuttrigsys + 1;
  memset(icuttrigsys,0,8);
  icuttrigsys->itriggersys = itriggersys;
  n = ncuttrigs * 4;
  icuttrigsys->itriggers = (instNUGCUTTRIGGER_s *)*buff;
  buff->voidptr = (void *)((int)buff->voidptr + n);
  memset(icuttrigsys->itriggers,0,n);
  return icuttrigsys;
}

void instNuGCutTriggerSysUpdate(instNUGCUTSCENE_s *icutscene,float current_frame)

{
  uchar flags;
  NUSTATEANIM_s *stateanim;
  int state;
  instNUTRIGGER_s *itrig;
  int iVar1;
  int i;
  NUGCUTTRIGGERSYS_S *cuttrigsys;
  instNUGCUTTRIGGERSYS_s *icuttrigsys;
  int iVar2;
  NUGCUTTRIGGER_s *cuttrig;
  double dVar3;
  uchar value [16];
  
  dVar3 = (double)current_frame;
  i = 0;
  icuttrigsys = icutscene->itriggersys;
  cuttrigsys = icutscene->cutscene->triggersys;
  if (0 < cuttrigsys->ntriggers) {
    iVar1 = 0;
    iVar2 = 0;
    do {
      cuttrig = cuttrigsys->triggers;
      stateanim = *(NUSTATEANIM_s **)((int)&cuttrig->enableflag1anim + iVar2);
      if ((stateanim != (NUSTATEANIM_s *)0x0) &&
         (state = StateAnimEvaluate(stateanim,(uchar *)(icuttrigsys->itriggers->pad + iVar1 + -1),
                                    value,(float)dVar3), state != 0)) {
        if (value[0] == '\0') {
          itrig = icuttrigsys->itriggersys->itriggers + *(short *)((int)&cuttrig->ix + iVar2);
          flags = itrig->enableflags & 0xfe;
        }
        else {
          itrig = icuttrigsys->itriggersys->itriggers + *(short *)((int)&cuttrig->ix + iVar2);
          flags = itrig->enableflags | 1;
        }
        itrig->enableflags = flags;
      }
      i = i + 1;
      iVar1 = iVar1 + 4;
      iVar2 = iVar2 + 0xc;
    } while (i < cuttrigsys->ntriggers);
  }
  return;
}

void instNuGCutTriggerSysStart(instNUGCUTSCENE_s *icutscene)

{
  instNUGCUTTRIGGERSYS_s *icuttrigsys;
  NUGCUTTRIGGERSYS_S *cuttrigsys;
  int i;
  int j;
  
  i = 0;
  icuttrigsys = icutscene->itriggersys;
  cuttrigsys = icutscene->cutscene->triggersys;
  if (0 < cuttrigsys->ntriggers) {
    do {
      j = i + 1;
      icuttrigsys->itriggers[i].next_ix = '\0';
      i = j;
    } while (j < cuttrigsys->ntriggers);
    return;
  }
  return;
}

int LookupLocatorFn(char *name)

{
  int *piVar1;
  int ix;
  int iVar2;
  int iVar3;
  int iVar4;
  
  iVar2 = 0;
  if ((locatorfns != (NUGCUTLOCFNDATA_s *)0x0) && (locatorfns->name != (char *)0x0)) {
    iVar4 = 0;
    iVar3 = 0;
    do {
      ix = strcasecmp(name,*(char **)((int)&locatorfns->name + iVar4));
      if (ix == 0) {
        return iVar2;
      }
      iVar4 = iVar4 + 8;
      iVar2 = iVar2 + 1;
      piVar1 = (int *)((int)&locatorfns[1].name + iVar3);
      iVar3 = iVar3 + 8;
    } while (*piVar1 != 0);
  }
  return -1;
}

void CalculateLocatorDirection(locdir ldir,numtx_s *mtx,nuvec_s *dir)

{
  bool c;
  float calc_rots;
  
  c = false;
  if (ldir == LOCATOR_NEGX) {
    calc_rots = 1.0;
LAB_8008ad5c:
    c = true;
    dir->z = 0.0;
    dir->x = calc_rots;
    dir->y = 0.0;
  }
  else {
    if (LOCATOR_NEGX < ldir) {
      if (ldir == LOCATOR_NEGY) {
        dir->z = 0.0;
        dir->y = 1.0;
      }
      else if (ldir < LOCATOR_NEGY) {
        dir->z = 0.0;
        dir->y = -1.0;
      }
      else {
        if (ldir == LOCATOR_Z) {
          calc_rots = 1.0;
        }
        else {
          if (ldir != LOCATOR_NEGZ) goto LAB_8008ade4;
          calc_rots = -1.0;
        }
        dir->y = 0.0;
        dir->z = calc_rots;
      }
      c = true;
      dir->x = 0.0;
      goto LAB_8008ade4;
    }
    if (ldir == LOCATOR_DOWN) {
      calc_rots = -1.0;
    }
    else {
      if (LOCATOR_DOWN < ldir) {
        calc_rots = -1.0;
        goto LAB_8008ad5c;
      }
      calc_rots = 1.0;
    }
    dir->x = 0.0;
    dir->y = calc_rots;
    dir->z = 0.0;
  }
LAB_8008ade4:
  if (c) {
    NuVecMtxRotate(dir,dir,mtx);
  }
  return;
}

int StateAnimEvaluate(NUSTATEANIM_s *stateanim,uchar *lastix,uchar *newstate,float frame)

{
  uchar uVar1;
  int statechanged;
  byte ix;
  uint uVar2;
  
  ix = *lastix;
  statechanged = 0;
  while (((int)(uint)ix < stateanim->nchanges && (stateanim->frames[*lastix] <= frame))) {
    statechanged = 1;
    *newstate = stateanim->states[*lastix];
    ix = *lastix + 1;
    *lastix = ix;
  }
  if (statechanged == 0) {
    uVar1 = *lastix;
    while ((uVar1 != '\0' && (frame < stateanim->frames[*lastix - 1]))) {
      uVar2 = *lastix - 1;
      statechanged = 1;
      *lastix = (uchar)uVar2;
      *newstate = stateanim->states[uVar2 & 0xff];
      uVar1 = *lastix;
    }
  }
  return statechanged;
}

NUSTATEANIM_s * StateAnimFixPtrs(NUSTATEANIM_s *sanim,int address_offset)

{
  NUSTATEANIM_s *Sanim;
  uchar *states;
  float *frames;
  
  Sanim = (NUSTATEANIM_s *)0x0;
  if (sanim != (NUSTATEANIM_s *)0x0) {
    Sanim = (NUSTATEANIM_s *)((int)&sanim->nchanges + address_offset);
  }
  if (Sanim == (NUSTATEANIM_s *)0x0) {
    return (NUSTATEANIM_s *)0x0;
  }
  frames = (float *)0x0;
  if (Sanim->frames != (float *)0x0) {
    frames = (float *)((int)Sanim->frames + address_offset);
  }
  states = (uchar *)0x0;
  Sanim->frames = frames;
  if (Sanim->states != (uchar *)0x0) {
    states = Sanim->states + address_offset;
  }
  Sanim->states = states;
  return Sanim;
}

void NuSetCutSceneCharacterRenderFn(NuCutSceneCharacterRender *param_1)

{
  NuCutSceneCharacterRender = param_1;
  return;
}

void NuSetCutSceneFindCharactersFn(NuCutSceneFindCharacters *param_1)

{
  NuCutSceneFindCharacters = param_1;
  return;
}