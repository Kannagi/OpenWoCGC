/*
TODO 

HubSelect
HubLevelSelect
HubDrawItems
InitVehicleToggles
ResetVehicleControl
ToggleVehicle
DrawMenu
BonusTransporter
DeathTransporter
GemPathTransporter

*/


//MATCH NGC
void InitProbe(void) {
  probecol = 0;
  probeon = 0;
  probey = 0;
  probetime = 0;
  proberot.x = 0;
  proberot.y = 0;
  proberot.z = 0;
  return;
}

//MATCH NGC
void ResetTempCharacter(s32 character,s32 action) {
  temp_character = character;
  temp_character_action = action;
  ResetAnimPacket(&TempAnim,action);
  ResetLights(&TempLights);
  return;
}

//MATCH NGC
void ResetTempCharacter2(s32 character,s32 action) {
  temp_character2 = character;
  temp_character2_action = action;
  ResetAnimPacket(&TempAnim2,action);
  ResetLights(&TempLights2);
  return;
}

//MATCH NGC
void UpdateTempCharacter(void) {
  struct CharacterModel* model;
  s32 i;

  if (temp_character2 != -1) {
    i = CRemap[temp_character2];
    if (i == -1) {
      return;
    }
    TempAnim2.oldaction = TempAnim2.action;
    model = &CModel[i];
    if (((u16)TempAnim2.action < 0x76) && ((u16)TempAnim2.newaction < 0x76)) {
      UpdateAnimPacket(model,&TempAnim2,0.5f,0.0f);
    }
  }
  if (temp_character != -1) { 
      i = CRemap[temp_character];
    if (i != -1) {
      TempAnim.oldaction = TempAnim.action;
      if ((temp_character == 2) &&
         (((TempAnim.flags & 1) != 0 ||
          ((Pad[0] != NULL && ((Pad[0]->oldpaddata & 0x840) != 0)))))) {
        if ((Level == 0x26) && (Game.lives = '\x04', 0 <= cortex_gameover_i)) {
          if (cortex_gameover_i < 2) {
            new_level = 0x25;
            just_continued = 1;
          }
          else if (cortex_gameover_i < 7) {
            new_level = 0x23;
          }
        }
        if ((TempAnim.flags & 1) != 0) {
          if (TempAnim.action == 0x73) {
            gameover_hack = 1;
          }
          TempAnim.newaction = 0x22;
          TempAnim.flags = '\0';
        }
      }
      if ((tempanim_waitaudio != 0) && (NuSoundKeyStatus(4) == 1)) {
        TempAnim.newaction = tempanim_nextaction;
        tempanim_waitaudio = 0;
        if (Pad[0] != NULL) {
          Pad[0]->rdy = 0.0f;
        }
      }
        model = &CModel[i];
      if (((u16)TempAnim.action < 0x76) && ((u16)TempAnim.newaction < 0x76)) {
        UpdateAnimPacket(model,&TempAnim,0.5f,0.0f);
      }
    }
  }
  return;
}

//MATCH NGC
void HubStart(struct obj_s *obj,s32 hub,s32 level,struct nuvec_s* pos) {
  struct nuvec_s* p0;
  struct nuvec_s* p1;
  float y;
  float t;
  s32 i;
  s32 j;
  
  if ((level != -1) && ((new_lev_flags & 0x800) != 0)) {
    new_lev_flags = new_lev_flags ^ 0x800;
    Game.level[level].flags = Game.level[level].flags | 0x800;
    CalculateGamePercentage(&Game);
  }
  tumble_action = -1;
  temp_lev_flags = 0;
  i = HData[hub].i_spl[0];
  tumble_duration = 0.0f;
  tumble_time = 0.0f;
  if ((i == -1) || (SplTab[i].spl == NULL)) {
    *pos = v000;
  }
  else {
    HubFromLevel(level);
    p0 = (struct nuvec_s *)(SplTab[i].spl->pts + ((temp_hublevel * 2 + 2) * SplTab[i].spl->ptsize) );
    tumble_oldpos = *p0;
    j = (temp_hublevel * 2 + 3) * (s32)SplTab[i].spl->ptsize;
    p1 = (struct nuvec_s *)(SplTab[i].spl->pts + j);
    pos->x = p0->x + ((p1->x - p0->x) + (p1->x - p0->x));
    pos->y = p0->y;
    pos->z = p0->z + ((p1->z - p0->z) + (p1->z - p0->z));
    y = NewShadowMaskPlat(pos,0.0f,-1);
    if (y != 2000000.0f) {
      pos->y = y;
    }
    tumble_newpos.x = pos->x;
    tumble_newpos.y = pos->y - obj->bot * obj->SCALE;
    tumble_newpos.z = pos->z;
    *pos = tumble_newpos;
    tumble_hdg = (short)NuAtan2D(p1->x - p0->x, p1->z - p0->z);
    tumble_action = (new_lev_flags != 0) ? 0x56 : 0x55;
    tumble_time = 0.0f;
    tumble_duration = ModelAnimDuration(tumble_character,tumble_action,tumble_time,tumble_time);
    if (tumble_character == 1) {
      t = 25.0f;
      tumble_item_starttime = 46.0f;
      tumble_item_addtime = 90.0f;
    }
    else {
      if (tumble_action == 0x56) {
        t = 19.0f;
      }
      else {
        t = 25.0f;
      }
      tumble_item_starttime = 32.0f;
      tumble_item_addtime = 50.0f;
    }
    tumble_moveduration = ModelAnimDuration(tumble_character,tumble_action,0.0f,t);
    tumble_cycleduration = ModelAnimDuration(tumble_character,tumble_action,0.0f,tumble_item_starttime);
  }
  return;
}

//MATCH NGC
void HubMoveVR(void) {
    struct nuvec_s* p0;
    struct nuvec_s* p1;
    struct nuvec_s pos;
    u32 j;
    u32 i;
    float dx;
    float dy;
    float dz;
    float f;
    float xz;
    u16 xrot;
    u16 yrot;
    struct nugspline_s* spl;
    u32 k;

    if (hub_vr_level == -1) {
        hub_vr_scale = 0.0f;
        hub_vr_newscale = 0.0f;
        hub_vr_scale = 0.0f;
    } else if (hub_vr_scale > hub_vr_newscale) {
        hub_vr_scale -= 0.03333334f;
        if (hub_vr_scale < hub_vr_newscale) {
            hub_vr_scale = hub_vr_newscale;
        }
    } else if ((hub_vr_scale < hub_vr_newscale)) {
        hub_vr_scale += 0.03333334f;
        if (hub_vr_scale > hub_vr_newscale) {
            hub_vr_scale = hub_vr_newscale;
        }
    }
    
    if (((Hub != -1) && (HData[Hub].i_spl[0] != -1)) && (best_cRPos != NULL)) {
        spl = SplTab[HData[Hub].i_spl[0]].spl;
        if (spl != NULL) {
            k = 1 << 18;
            i = (s32)(best_cRPos->fACROSS * k);
            j = i / 65536;
            p0 = (struct nuvec_s*)&spl->pts[(j + 14) * spl->ptsize];
            pos.x = p0->x;
            pos.y = p0->y;
            pos.z = p0->z;
            if ((j < 4) && (i = i & 0xffff, i != 0)) {
                p1 = (struct nuvec_s*)&spl->pts[(j + 15) * spl->ptsize];
                f = (i) / 65536.0f;
                pos.x += (p1->x - p0->x) * f;
                pos.y += (p1->y - p0->y) * f;
                pos.z += (p1->z - p0->z) * f;
            }
            hub_vr_pos.x += (pos.x - hub_vr_pos.x) * 0.1f;
            hub_vr_pos.y += (pos.y - hub_vr_pos.y) * 0.1f;
            hub_vr_pos.z += (pos.z - hub_vr_pos.z) * 0.1f;
            hub_vr_newscale = ((HubLevel_available != 0) && (HubLevel >= 0)) && (HubLevel <= 4) ? 1.0f : 0.0f;
            dx = ((player->obj).pos.x - hub_vr_pos.x);
            dy = ((player->obj).top * (player->obj).SCALE + (player->obj).pos.y) - hub_vr_pos.y;
            dz = ((player->obj).pos.z - hub_vr_pos.z);
            
            xz = NuFsqrt((dx * dx + (dz * dz)));
            yrot = panel_head_yrot - NuAtan2D(dy, xz);
            xrot = panel_head_xrot + NuAtan2D(dx, dz);
            
            hub_vr_xrot = SeekRot(hub_vr_xrot, yrot, 2);
            hub_vr_yrot = SeekRot(hub_vr_yrot, xrot, 2);
        }
    }
    return;
}

//MATCH NGC
void InitWumpa(void) {
  s32 i;
  
  memset(Wumpa,0,0x5f00);
  WUMPACOUNT = 0;
  if ((LDATA->flags & 0x80) != 0) {
    LoadWumpa();
  }
  for (i = 0; i < 0x140; i++) {
    Wumpa[i].iRAIL = -1;
    Wumpa[i].iALONG = -1;
  }
  for (i = 0; i < 8; i++) {
    WInfo[i].angle = 0;
    WInfo[i].spin = 0x666;
    WInfo[i].scale = 0.75f;
  }
  ResetWumpa();
  return;
}

//NGC MATCH
void LoadWumpa(void) {
  s32 fh;
  s32 i;
  void* fbuff;
  s32 fsize;
  
  sprintf(tbuf,"%s.wmp",LevelFileName);
  if (NuFileExists(tbuf) != 0) {
    fbuff = Chase;
    fsize = NuFileLoadBuffer(tbuf,fbuff,0x7fffffff);
    fh = NuMemFileOpen(fbuff,fsize,NUFILE_READ);
    if (fh != 0) {
      WUMPACOUNT = NuFileReadInt(fh);
      if (0x100 < WUMPACOUNT) {
        WUMPACOUNT = 0x100;
      }
        for (i = 0; i < WUMPACOUNT; i++) {
          (Wumpa[i].pos0).x = NuFileReadFloat(fh);
          (Wumpa[i].pos0).y = NuFileReadFloat(fh);
          (Wumpa[i].pos0).z = NuFileReadFloat(fh);
        }
      NuFileClose(fh);
    }
  }
  return;
}

//NGC MATCH
void DrawWumpa(void) {
    struct winfo_s *info;
    struct wumpa_s *wumpa;
    float dx;
    float dz;
    float d;
    float r2;
    struct numtx_s* m;
    struct nuvec_s s;
    struct nuvec_s v;
    s32 i;
    s32 j;
    
    if (ObjTab[0].obj.special != NULL) {
        r2 = 45.0f;
        if ((LDATA->farclip) < 45.0f) {
            r2 = LDATA->farclip;
        }
        r2 *= r2;
        wumpa = Wumpa;
        for (i = 0; i < 0x140; i++, wumpa++) {
            wumpa->in_range = '\0';
            if ((wumpa->active != '\0') && (TimeTrial == 0 || (wumpa->active == '\x03' && (wumpa->destroy != '\0')))) {
                dx = (pCam->pos).x - (wumpa->pos).x; 
                dz = (pCam->pos).z - (wumpa->pos).z;
                d = dx * dx + dz * dz;
                if (!(d > r2)) {
                    wumpa->in_range = '\x01';
                    info = &WInfo[(i & 7)]; 
                    if ((((wumpa->shadow != 2000000.0f) && (VEHICLECONTROL != 2)) &&
                    ((VEHICLECONTROL != 1 || ((player->obj).vehicle != 0x20)))) &&
                    ((TerSurface[wumpa->surface_type].flags & 1) == 0)) {
                        v.x = (wumpa->pos1).x;
                        v.y = wumpa->shadow;
                        v.z = (wumpa->pos1).z;
                        if (wumpa->active == '\x03') {
                            j = 0x7f - (s32)(((wumpa->time * 5.0f) / wumpa->duration) * 127.0f);
                        }
                        else if (wumpa->active == '\x01') {
                            j = (s32)((wumpa->time / 0.75f) * 127.0f);
                        }
                        else {
                            j = 0x7f;
                        }
                        if (0 < j) {
                            NuRndrAddShadow(&v,info->scale * 0.25f,j,(u32)wumpa->surface_xrot,
                            (u32)info->angle,(u32)wumpa->surface_zrot);
                        }
                    }
                    if (wumpa->active == '\x03') {
                        s.x = 1.0f - (wumpa->time * 0.5f);
                        if ((wumpa->fired != '\0') && (wumpa->time < 0.1f)) {
                            if (wumpa->fired == '\x02') {
                                s.x = 0.0f;
                            }
                            else {
                                s.x = s.x * (wumpa->time / 0.1f);
                            }
                        }
                    }
                    else {
                        s.x = 1.0f;
                    }
                    if (s.x > 0.0f) {
                        if (s.x != 1.0f) {
                            s.z = s.x;
                            s.y = s.x;
                            NuMtxSetScale(&mTEMP,&s);
                            NuMtxMulR(&mTEMP,&mTEMP,&info->m);
                            m = &mTEMP;
                        } else{
                            m = &info->m;
                        }
                        m->_30 = (wumpa->pos).x;
                        m->_31 = (wumpa->pos).y + info->dy;
                        m->_32= (wumpa->pos).z;    
                        NuRndrGScnObj((ObjTab[0].obj.scene)->gobjs[(ObjTab[0].obj.special)->instance->objid],m);
                        if ((TerSurface[wumpa->surface_type].flags & 4) != 0) {
                            mTEMP = *m;
                            mTEMP._01 = -mTEMP._01;
                            mTEMP._11 = -mTEMP._11;
                            mTEMP._21 = -mTEMP._21;
                            mTEMP._31 = wumpa->shadow - (mTEMP._31 - wumpa->shadow);
                            NuRndrGScnObj(ObjTab[0].obj.scene->gobjs[ObjTab[0].obj.special->instance->objid],&mTEMP);
                        }
                    }
                }
            }
        }
    }
    return;
}

//NGC MATCH
void AddScreenWumpa(float x,float y,float z,s32 count) {  
  if (count < 1) {
    count = 1;
  }
  NewWumpa[i_newwumpa].world_pos.x = x;
  NewWumpa[i_newwumpa].world_pos.y = y;
  NewWumpa[i_newwumpa].world_pos.z = z;
  NewWumpa[i_newwumpa].count = (u8)count;
  NewWumpa[i_newwumpa].delay = '\0';
  NewWumpa[i_newwumpa].transformed = '\0';
  NewWumpa[i_newwumpa].bonus = ((Bonus == 2) || (sw_hack != 0)) ? 1 : 0;
  NewWumpa[i_newwumpa].active = '\x01';
  i_newwumpa++;
  if (i_newwumpa == 0x20) {
    i_newwumpa = 0;
  }
  sw_hack = 0;
  return;
}

//93.88% NGC
void ResetWumpa(void) {
    struct wumpa_s* wump;
    s32 i;
    s32 gempath_reset = 0;

    if ((Rail[7].type == 3)
        && (AheadOfCheckpoint((s32)gempath_RPos.iRAIL, (s32)gempath_RPos.iALONG, gempath_RPos.fALONG) != 0))
    {
        gempath_reset = 1;
    }
    wump = Wumpa;
    for (i = 0; i < WUMPACOUNT; i++, wump++) {
        GetALONG(&wump->pos0, NULL, -1, -1, 1);
        wump->pos1 = wump->pos0;
        wump->iRAIL = temp_iRAIL;
        wump->iALONG = temp_iALONG;
        wump->fALONG = temp_fALONG;
        wump->pos = wump->pos0;
        wump->shadow = NewShadowMask(&wump->pos, 0.0, -1);
        if (wump->shadow != 2000000.0f) {
            wump->surface_type = (char)ShadowInfo();
            FindAnglesZX(&ShadNorm);
            wump->surface_xrot = temp_xrot;
            wump->surface_zrot = temp_zrot;
        } else {
            wump->surface_type = -1;
            wump->surface_xrot = 0;
            wump->surface_zrot = 0;
        }
        if (wump->iRAIL != -1 && Rail[wump->iRAIL].type == 1 && Bonus == 4) {
            if (wump->active != 2) {
                wump->active = 0;
            }
        } else {
            //missing branch, gempath_reset condition
            if ((wump->iRAIL != -1 && Rail[wump->iRAIL].type == 3 && gempath_reset)
                || (AheadOfCheckpoint(wump->iRAIL, wump->iALONG, wump->fALONG) != 0)) {
                wump->active = 2;
            } else if (wump->active != 2) {
                wump->active = 0;
            }
        }
    }
    for (i; i < 0x140; i++) {
        wump->active = 0;
        wump++;
    }
    for (i = 0; i < 0x20; i++) {
        NewWumpa[i].active = 0;
    }
    for (i = 0; i < 0x20; i++) {
        WScr[i].timer = 0.0f;
    }
    return;
}

//MATCH NGC
s32 HitWumpa(struct obj_s* obj, s32 destroy) {
    float objtop;
    float objbot;
    float dx;
    float dz;
    float r2;
    struct wumpa_s* wumpa;
    s32 i;
    s32 key;

    if (TimeTrial == 0) {
        objtop = (obj->pos).y + obj->top * obj->SCALE;
        objbot = (obj->pos).y + obj->bot * obj->SCALE;
        wumpa = Wumpa;
        r2 = (obj->RADIUS + 0.1f);
        r2 *= r2;
        for (i = 0; i < 0x140; i++, wumpa++) {
            if ((wumpa->active == 1) || (wumpa->active == 2)) {
                if (objtop < (wumpa->pos.y - 0.1f) || (objbot > wumpa->pos.y + 0.1f)) {
                    continue;
                }
                dx = ((wumpa->pos).x - (obj->pos).x) * ((wumpa->pos).x - (obj->pos).x);
                dz = ((wumpa->pos).z - (obj->pos).z) * ((wumpa->pos).z - (obj->pos).z);
                if (dx + dz < r2) {
                    if (destroy != 0) {
                        key = -1;
                        AddFiniteShotDebrisEffect(&key, GDeb[130].i, &wumpa->pos, 1);
                        wumpa->active = 0;
                    } else {
                        FlyWumpa(wumpa);
                    }
                    return 1;
                }
            }
        }
    }
    return 0;
}

//MATCH NGC
void FlyWumpa(struct wumpa_s* wumpa) {
  (wumpa->mom).x = 0.0f;
  (wumpa->mom).y = 0.0f;
  (wumpa->mom).z = 0.1666667f;
  NuVecRotateX(&wumpa->mom,&wumpa->mom,-0x400);
  NuVecRotateY(&wumpa->mom,&wumpa->mom,qrand());
  wumpa->time = 0.0f;
  wumpa->destroy = '\0';
  wumpa->active = '\x03';
  wumpa->duration = 2.0f;
  wumpa->fired = '\0';
  GameSfx(0x2a,&wumpa->pos);
  return;
}

//MATCH NGC
s32 WipeWumpa(struct RPos_s *rpos) {
  s32 i;
  struct wumpa_s* wumpa;
  
  if (TimeTrial != 0) {
      return 0;
  } else{
    wumpa = Wumpa;
    for (i = 0; i < 0x140; i++) {
      if ((((u8)wumpa->active == 1) || ((u8)wumpa->active == 2)) &&
         (FurtherALONG(rpos->iRAIL,rpos->iALONG,rpos->fALONG,wumpa->iRAIL,wumpa->iALONG,wumpa->fALONG) != 0)) {
        FlyWumpa(wumpa);
        return 1;
      }
      wumpa++;
    }
  }
    return 0;
}

//MATCH NGC
void WumpaHitTerrain(struct wumpa_s *wmp) {
  float f;
  struct nuvec_s ray;
  
  f = wmp->duration * 60.0f;
  ray.x = (wmp->mom).x * f;
  ray.y = (wmp->mom).y * f;
  ray.z = (wmp->mom).z * f;
  f = NuVecMag(&ray);
  if (NewRayCast(&wmp->pos,&ray,0.0f) != 0) {
    wmp->duration *= (NuVecMag(&ray) / f);
  }
  return;
}

//MATCH NGC
void AddFlyingWumpa(struct nuvec_s *src,struct nuvec_s *dir,struct nuvec_s *dst,s32 destroy) {
  struct wumpa_s* wumpa;

  wumpa = &Wumpa[i_tempwumpa + 0x100];
  wumpa->pos1 = *src;
  wumpa->pos0 = wumpa->pos1;
  wumpa->pos = wumpa->pos0;
  wumpa->shadow = 2000000.0f;
  wumpa->active = '\x03';
  wumpa->time = 0.0f;
  wumpa->duration = 2.0f;
  wumpa->surface_type = -1;
  NuVecScale(&wumpa->mom,dir,0.1666667f);
  WumpaHitTerrain(wumpa);
  if (destroy == 3) {
    destroy = 2;
    wumpa->fired = '\x02';
  }
  else {
    wumpa->fired = '\x01';
  }
  wumpa->destroy = (char)destroy;
  i_tempwumpa++;
  if (i_tempwumpa == 0x40) {
    i_tempwumpa = 0;
  }
  return;
}

//MATCH NGC
void UpdateScreenWumpas(void) {
  struct nuvec_s v[2];
  struct newwumpa_s *anew;
  struct wscr_s *scr;
  s32 i;
  
  scr = WScr;
  for (i = 0; i < 0x20; i++) {
        if (0.0f < scr->timer) {
          scr->timer = scr->timer - 0.016666668f;
              if (scr->timer < 0.0f) {
                scr->timer = 0.0f;
                    if (sw_hack != 0) {
                      mg_wumpatot++;
                      GameSfx(0x19,NULL);
                    }
                    else {
                          if (scr->bonus != '\0') {
                            plr_bonus_wumpas.count++;
                          }
                          else {
                            plr_wumpas.count++;
                          }
                    }
              }
        }
    scr++;
  }
  anew = NewWumpa;
  for (i = 0; i < 0x20; i++, anew++) {
        if (anew->active != '\0') {
              if (anew->delay == '\0') {
                if (anew->transformed == '\0') {
                  anew->transformed = '\x01';
                  NuCameraTransformScreenClip(&anew->screen_pos,&anew->world_pos,1,NULL);
                  v[0].x = (anew->world_pos).x + GameCam[0].vX.x * 0.1f;
                  v[0].y =  (anew->world_pos).y + GameCam[0].vX.y * 0.1f;
                  v[0].z = (anew->world_pos).z + GameCam[0].vX.z * 0.1f;
                  NuCameraTransformScreenClip(&v[1],v,1,NULL);
                  anew->screen_scale = (NuFabs((anew->screen_pos).x - v[1].x) * 3.636363f);
                }
                scr = &WScr[i_screenwumpa];
                scr->pos = anew->screen_pos;
                scr->scale = anew->screen_scale;
                scr->timer = 0.25f;
                    if (anew->bonus != '\0') {
                      scr->xs = BONUSWUMPAOBJSX;
                      scr->ys = BONUSPANELSY;
                      scr->bonus = '\x01';
                    }
                    else {
                      scr->xs = WUMPAOBJSX;
                      scr->ys = PANELSY;
                      scr->bonus = '\0';
                      force_panel_wumpa_update = 0x3c;
                    }
                i_screenwumpa++;
                if (i_screenwumpa == 0x20) {
                  i_screenwumpa = 0;
                }
                anew->count--;
                if (anew->count == '\0') {
                  anew->active = '\0';
                  continue;
                }
                anew->delay = '\f';
              }
              else {
                anew->delay--;
              }
        }
  }
    sw_hack = 0;
    return;
}

//NGC MATCH
void AddTempWumpa(float x,float y,float z,struct cratesarray_s *crate,s32 n) {
  struct wumpa_s* wumpa; 
  u16 a;
  float d;
  s32 i;

  if ((TimeTrial == 0)) {
    for (i = 0; i < n; i++) {
      wumpa = &Wumpa[i_tempwumpa + 0x100];
      wumpa->pos0.x = x;
      wumpa->pos0.y = y;
      wumpa->pos0.z = z;
      if (n > 1) {
        a = qrand();
        d = (qrand() * 0.000015259022f) * 0.25f;
        wumpa->pos1.x = wumpa->pos0.x + NuTrigTable[a & 0xffff] * (d);
        wumpa->pos1.y = wumpa->pos0.y;
        wumpa->pos1.z = wumpa->pos0.z + NuTrigTable[((a & 0xffff) + 0x4000) & 0x3fffc / 4] * d;
      }
      else {
        wumpa->pos1 = wumpa->pos0;
      }
      wumpa->pos = wumpa->pos0;
      wumpa->shadow = NewShadowMask(&wumpa->pos1,0.0,-1);
      wumpa->iRAIL = crate->iRAIL;
      wumpa->iALONG = crate->iALONG;
      wumpa->fALONG = crate->fALONG;
      wumpa->active = '\x01';
      wumpa->time = 0.0f;
      if (wumpa->shadow != 2000000.0f) {
        wumpa->surface_type = (char)ShadowInfo();
        FindAnglesZX(&ShadNorm);
        wumpa->surface_xrot = temp_xrot;
        wumpa->surface_zrot = temp_zrot;
      }
      else {
        wumpa->surface_type = -1;
      }
      i_tempwumpa++;
      if (i_tempwumpa == 0x40) {
        i_tempwumpa = 0;
      }
    }
  }
  return;
}

//98%
s32 Draw3DObject(s32 object,struct nuvec_s *pos,u16 xrot,u16 yrot,u16 zrot,float scalex,
                float scaley,float scalez,struct nugscn_s *scn,struct nuspecial_s *obj,s32 rot) {
  struct numtx_s m;
  struct nuvec_s s;
  s32 i;
  
  if (((scn == NULL) || (obj == NULL)) || ((scalex == 0.0f && ((scaley == 0.0f && (scalez == 0.0f))) || Level != 0x25))) {
          i = 0;
  }
  else {
    s.x = scalex;
    s.y = scaley;
    s.z = scalez;
    NuMtxSetScale(&m,&s);
    switch(rot) {
                case 0:
                    if (xrot != NULL) {
                        NuMtxRotateX(&m,(s32)xrot);
                    }
                    if (yrot != NULL) {
                        NuMtxRotateY(&m,(s32)yrot);
                    }
                    if (zrot != NULL) {
                        NuMtxRotateZ(&m,(s32)zrot);
                    }
                    break;
                case 1:
                    if (yrot != NULL) {
                        NuMtxRotateY(&m,(s32)yrot);
                    }
                    if (xrot != NULL) {
                        NuMtxRotateX(&m,(s32)xrot);
                    }
                    if (zrot != NULL) {
                        NuMtxRotateZ(&m,(s32)zrot);
                    }
                    break;
    }
    NuMtxTranslate(&m,pos);
    return NuRndrGScnObj(scn->gobjs[obj->instance->objid],&m);
  }
  return i;
}

//MATCH NGC
s32 Draw3DCharacter(struct nuvec_s *pos,u16 xrot,u16 yrot,u16 zrot,struct CharacterModel *model,
                    s32 action,float scale,float anim_time, s32 rot) {
    s32 i;
    struct numtx_s m;
    struct nuvec_s s;
    char pad[7];

    if ((model != 0) && (scale != 0.0f)) {
        if (Level != 0x25) {
        s32 iVar3;
            if (model->character == 0x75) {
                iVar3 = 0x84;
            } else if (model->character == 0x77) {
                iVar3 = 0x88;
            } else if (model->character == 0x78) {
                iVar3 = 0x89;
            } else if (model->character == 0x79) {
                iVar3 = 0x8a;
            } else if (model->character == 0x7a) {
                iVar3 = 0x8b;
            } else if (model->character == 0x7b) {
                iVar3 = 0x8c;
            } else if (model->character == 0x7c) {
                iVar3 = 0x8d;
            } else if (model->character == 0x7d) {
                iVar3 = 0x8e;
            }
        }
    
        s.z = scale;
        s.y = scale;
        s.x = scale;
        NuMtxSetScale(&m,&s);
        switch (rot) {
                case 0:
                    if (xrot != NULL) {
                        NuMtxRotateX(&m,(s32)xrot);
                    }
                    if (yrot != NULL) {
                        NuMtxRotateY(&m,(s32)yrot);
                    }
                    if (zrot != NULL) {
                        NuMtxRotateZ(&m,(s32)zrot);
                    }
                    break;
                case 1:
                    if (yrot != NULL) {
                        NuMtxRotateY(&m,(s32)yrot);
                    }
                    if (xrot != NULL) {
                        NuMtxRotateX(&m,(s32)xrot);
                    }
                    if (zrot != NULL) {
                        NuMtxRotateZ(&m,(s32)zrot);
                    }
                    break;
        }
        NuMtxTranslate(&m,pos);
         if (((uint)action < 0x76) && (model->anmdata[action] != NULL)) {
            NuHGobjEvalAnim(model->hobj,model->anmdata[action],anim_time,0,NULL,tmtx);
        }
        else {
            NuHGobjEval(model->hobj,0,NULL,tmtx);
        }
        i = NuHGobjRndrMtx(model->hobj,&m,1,NULL,tmtx);
    }
    else {
        i = 0;
    }
    return i;
}

//NGC MATCH
void NewMask(struct mask_s *mask,struct nuvec_s *pos) {
  
  if ((CRemap[3] != -1) && ((LBIT & 0x3e00000) == 0)) {
    if (mask->active < 2) {
      if (mask->active == 0) {
        if (pos != NULL) {
          (mask->pos) = *pos;
        }
        mask->character = 3;
        mask->scale = 0.8f;
        ResetLights(&mask->lights);
      }
      mask->active++;
    }
    else {
      mask->active = 0x296;
      GameMusic(0xa2,0);
    }
    GameSfx(0x3d,pos);
  }
  return;
}

//NGC MATCH
void MakeMaskMatrix(struct mask_s *mask,struct numtx_s *mM,struct numtx_s *mS,struct numtx_s *mLOCATOR,float scale) {
  double dVar1;
  struct nuvec_s s;
  u16 yrot;
  
  if (mM != NULL) {
    s.x = s.y = s.z = scale * mask->scale;
    NuMtxSetScale(mM,&s);
    yrot = mask->yrot - 0x8000 & 0xffff;
    if (mask->active > 2) {
         if (mLOCATOR != NULL) {
           NuMtxMulR(mM,mM,mLOCATOR);
          (mask->pos).x = mLOCATOR->_30;
          (mask->pos).y = mLOCATOR->_31;
          (mask->pos).z = mLOCATOR->_32;       
         }
         else {
              NuMtxRotateX(mM,(int)(NuTrigTable[(GameTimer.frame & 0xf) * 0x1000] * 8192.0f) & 0xffff);
              NuMtxRotateZ(mM,(int)((NuTrigTable[((GameTimer.frame & 0xf) * 0x1000 + 0x4000) & 0x3c000 / 4]) * 8192.0f) & 0xffff);
              NuMtxRotateY(mM,yrot);
          }
    }
    else {
      NuMtxRotateX(mM,(uint)mask->xrot);
      NuMtxRotateY(mM,yrot);
    }
    NuMtxTranslate(mM,&mask->pos);
    if ((mS != NULL) && (mask->shadow != 2000000.0f)) {
      ScaleFlatShadow(&s,(mask->pos).y,mask->shadow,1.0f);
      NuMtxSetScale(mS,&s);
      NuMtxRotateY(mS,yrot);
      NuMtxRotateZ(mS,mask->surface_zrot);
      NuMtxRotateX(mS,mask->surface_xrot);
      mS->_30 = (mask->pos).x;
      mS->_31 = mask->shadow + 0.025f;
      mS->_32 = (mask->pos).z;
    }
  }
  return;
}

void ResetMaskFeathers(void) {
  memset(MaskFeathers,0,0x240);
  return;
}

//NGC MATCH
void AddMaskFeathers(struct mask_s * mask) {
  struct mfeathers_s *feathers;
  s32 i;
  
  feathers = &MaskFeathers[i_maskfeathers];
    feathers->mM = mask->mM;
    feathers->mS = mask->mS;
  i = CRemap[11];
  feathers->time = 0.0f;
  feathers->shadow = mask->shadow;
  if ((i != -1) && (CModel[i].anmdata[0xe] != NULL)) {
    feathers->duration = CModel[i].anmdata[0xe]->time - 1.0f;
  }
  else {
    feathers->duration = 30.0f;
  }
  i_maskfeathers++;
  if (i_maskfeathers != 4) {
    return;
  }
  i_maskfeathers = 0;
  return;
}

//NGC MATCH
void LoseMask(struct obj_s *obj) {
  
  GameSfx(1,&obj->pos);
  AddMaskFeathers(obj->mask);
  obj->mask->active--;
  obj->invincible = 0xb4;
  (obj->mom).z = 0.0f;
  (obj->mom).x = 0.0f;
  NewBuzz(&player->rumble,0x1e);
  NewRumble(&player->rumble,0x7f);
  return;
}

//NGC MATCH
void UpdateMaskFeathers(void) {
  struct mfeathers_s *feathers;
  s32 i;
  
  feathers = MaskFeathers;
  for (i = 0; i < 4; i++) {
    if ((feathers->time < feathers->duration) && (feathers->time += 0.5f, feathers->time > feathers->duration)) {
      feathers->time = feathers->duration;
    }
    feathers++;
  }
  return;
}

//NGC MATCH
void DrawMaskFeathers(void) {
    struct CharacterModel* model;
    struct mfeathers_s *feathers;
    int i;

    i = CRemap[11];
    if (i != -1) {
        model = &CModel[i];
        if (model->anmdata[0xe] != NULL) {
            feathers = &MaskFeathers[0];
            for (i = 0; i < 4; i++) {
                if (feathers->time < feathers->duration) {
                    NuHGobjEvalAnim(model->hobj,model->anmdata[0xe],feathers->time,0,NULL,tmtx);
                    NuHGobjRndrMtx(model->hobj,&feathers->mM,1,NULL,tmtx);
                }
                feathers++;
            }
        }
    }
    return;
}

//NGC MATCH
void Draw3DCrateCount(struct nuvec_s *pos,u16 angle) {
  struct nuvec_s v;
  u16 yrot;
  float dVar6;
  float dx;
  float dz;
  s32 i;
  s32 l;
  
  v.z = v.y = v.x = 1.5f;
  NuMtxSetScale(&mTEMP,&v);
  NuMtxRotateY(&mTEMP,angle);
  NuMtxTranslate(&mTEMP,pos);
  NuRndrGScnObj((ObjTab[16].obj.scene)->gobjs[(ObjTab[16].obj.special)->instance->objid],&mTEMP);
  if (font3d_scene != NULL) {
        yrot = GameCam->yrot;
        dz = NuTrigTable[(yrot + 0x4000) & 0xFFFF];
        v = *pos;
        v.x -= NuTrigTable[yrot] * CRATECOUNT3DSEPERATION;
        v.z -= dz * CRATECOUNT3DSEPERATION;
        dx = (dz * CRATECOUNT3DSCALE);
        dVar6 = NuTrigTable[(yrot - 0x8000) & 0xFFFF] * CRATECOUNT3DSCALE;
        sprintf(tbuf,"%i/%i",(int)plr_crates.draw,DESTRUCTIBLECRATECOUNT);
        l = strlen(tbuf);
        v.x = v.x - (dx * (l - 1)) * 0.5f;
        v.z = v.z - (dVar6 * (l - 1)) * 0.5f;
        for (i = 0; i < l; i++) {
            if (Font3DRemap[(u8)tbuf[i]] != -1) {
              Draw3DObject(-1,&v,0,yrot,0,CRATECOUNT3DSCALE,CRATECOUNT3DSCALE,CRATECOUNT3DSCALE,
                           font3d_scene,Font3DTab[Font3DRemap[(u8)tbuf[i]]].obj.special,0);
            }
            v.x = v.x + dx;
            v.z = v.z + dVar6;
        }
  }
  return;
}

//NGC MATCH
void DrawTempCharacter(s32 render) {
    struct CharacterModel* model;
    struct numtx_s m;
    static struct numtx_s mtxLOCATOR_168[16];
    float **dwa;
    struct nuvec_s v;
    s32 i;

   
    if ((((((temp_character != -1) && (temp_character != 0x62)) && (temp_character != 0xb9)) &&
         ((temp_character != 0xb8 && (temp_character != 0xba)))) &&
        ((temp_character != 0xbc)))) {
        if (temp_character != 2 || (Level != 0x19)) {
            i = (s32)CRemap[temp_character];
            if((i != -1)) {
                        model = &CModel[i];
                        if (Level == 0x25) {
                            v.x = 5.0f;
                            v.y = 5.0f;
                            v.z = 5.0f;
                            NuMtxSetScale(&m,&v);
                            v.x = 0.0f;
                            v.y = 11.0f;
                            v.z = 6.25f;
                            NuMtxTranslate(&m,&v);
                        }              
                        else {
                            NuMtxSetIdentity(&m);
                        }
                        if (Level == 0x2b) {
                            if (model->character == 0xbb) {
                                NuMtxRotateY(&m,0x2000);
                                m._30 = DISCOXOFFSET;
                            }
                            else if (model->character == 0) {
                                NuMtxRotateY(&m,-0x2000);
                                m._30 = -DISCOXOFFSET;
                            }
                        }
                        EvalModelAnim(model,&TempAnim,&m,tmtx,&dwa,mtxLOCATOR_168);
                        if (render != 0) {
                            NuHGobjRndrMtxDwa(model->hobj,&m,1,NULL,tmtx,dwa);
                        }
            }
        }
    }
    return;
}

//NGC MATCH
void DrawTempCharacter2(s32 render) {
    struct CharacterModel* model;
    struct numtx_s m;
    static struct numtx_s mtxLOCATOR_172[16];
    float **dwa;
    s32 i;
    short layertab[2] = {0, 1};
    short* layer;
    s32 nlayers;

    layer = layertab;
    if ((temp_character2 != -1)) {
    i = CRemap[temp_character2];
        if (i != -1) {
                nlayers = 1;
                model = &CModel[i];
                if (model->character == 0) {
                    nlayers = 2;
                }
                NuMtxSetIdentity(&m);
                if (Level == 0x2b) {
                    if (model->character == 0xbb) {
                        NuMtxRotateY(&m,0x2000);
                        m._30 = DISCOXOFFSET;
                    }
                    else if (model->character == 0) {
                        NuMtxRotateY(&m,-0x2000);
                        m._30 = -DISCOXOFFSET;
                    }
                }
                EvalModelAnim(model,&TempAnim2,&m,tmtx,&dwa,mtxLOCATOR_172);
                if (render != 0) {
                    NuHGobjRndrMtxDwa(model->hobj,&m,nlayers,layer,tmtx,dwa);
                }
        }
 
    }
    return;
}

//NGC MATCH
s32 HubFromLevel(s32 level) {
  s32 j;
  s32 i;
  
  if (level == -1) {
    return -1;
  }
  for (i = 0; i < 6; i++) {
    for (j = 0; j < 6; j++) {
      if (HData[i].level[j] == level) {
        temp_hublevel = j;
        temp_hub = i;
        return i;
      }
    }
  }
  return -1;
}

//NGC MATCH
void ResetAwards(void) {
  s32 i;
  
  for (i = 0; i < 3; i++) {
    Award[i].time = 1.0f;
  }
  return;
}

//NGC MATCH
s32 ActiveAwards(void) {
  struct award_s *awards;
  s32 i;
  
  awards = Award;
  for (i = 0; i < 3; i++) {
    if (awards->time < 1.0f) {
      return 1;
    }
    awards = awards + 1;
  }
  return 0;
}

//NGC 99%
s32 AddAward(s32 hub,s32 level,u16 got) {
  s32 i0;
  struct award_s* award;
  struct nugspline_s *spl;
  struct nuvec_s* p0;
  struct nuvec_s* p1;
  s32 i;
  s32 j;
  s32 ang;
  
  if (((hub != -1) && (level != -1)) && (HData[hub].i_spl[1] != -1)) {
    spl = SplTab[HData[hub].i_spl[1]].spl;
    if (spl == NULL) {
      return 0;
    }
    HubFromLevel(level);
      i0 = temp_hublevel;
    if (i0 == -1) {
        return 0;
    } else{
        if ((got & 7) != 0) {
           ang = 2;
        } else if ((got & 8) != 0) {
          ang = 1;
        } else if ((got & 0x10) != 0) {
          ang = 0;
        } else{
           ang = 3;
        }
      award = &Award[i_award];
        i = ang * 2;
      i +=  (i0 * 8); 
      j = (i + 1);
      i *= spl->ptsize;
      j *= spl->ptsize;
      p0 = (struct nuvec_s*)  &spl->pts[i];
      p1 = (struct nuvec_s*) &spl->pts[j];
      award->time = 0.0f;
      award->yrot = (u16) NuAtan2D(p1->x - p0->x,p1->z - p0->z);
      award->level = (char)level;
      award->got = got;
      award->newpos = *p0;
      award->wait = 1;
      i_award++;
      if (i_award == 3) {
        i_award = 0;
      }
    }
  } else{
      return 0;
  }
  return 1;
}

//NGC MATCH
void UpdateAwards(void) {
  float fVar5;
  struct award_s* award; 
  float old_time;
  s32 i;

    award = Award;
  for(i = 0; i < 3; i++) {
    old_time = award->time;
    if (old_time < 1.0f) {
      if (award->wait) {
        if (!tumble_character) {
          NuVecAdd(&award->oldpos0,(struct nuvec_s *)&player->mtxLOCATOR[3][1]._30,
                   (struct nuvec_s *)&player->mtxLOCATOR[4][0]._30);
          NuVecScale(&award->oldpos0,&award->oldpos0,0.5f);
        }
        else {
            award->oldpos0 = *(struct nuvec_s*)&(player->mtxLOCATOR[1][1]._30);
        }
        award->oldpos1.x = award->oldpos0.x;
        fVar5 = award->oldpos0.y;
        award->oldpos1.y = (tumble_character == 1) ? fVar5 + 1.0f : fVar5 + 0.5f;
        award->oldpos1.z = award->oldpos0.z;
        if ((player->obj).anim.anim_time >= tumble_item_addtime) {
          award->wait = 0;
          GameSfx(0x26,NULL);
          AddGameDebris(0xa1, &award->oldpos1);
        }
      }
      else {
        old_time += 0.016666668f;
        award->time = old_time;
        if (old_time >= 1.0f) {
           award->time = 1.0f;
          new_lev_flags = (new_lev_flags | award->got);
            new_lev_flags ^=  award->got;
          Game.level[award->level].flags = Game.level[award->level].flags | award->got;
          CalculateGamePercentage(&Game);
          AddGameDebris(0xa1, &award->newpos);
        }
        else {
          if (old_time < 0.666f) {
            award->oldpos0.y = ((award->oldpos1).y - award->oldpos0.y) * 0.1f + award->oldpos0.y;
          }
        }
      }
    }
      award++;
  }
  return;
}

//NGC 98% (float)
void DrawAwards(void) {
    struct award_s* award;
    struct nuvec_s pos;
    float f;
    float scale;
    float t;
    s32 uVar2;
    s32 i;
    s32 i_obj;
    s32 i_chr;

    award = Award;
    for (i = 0; i < 3; i++) {
        t = award->time;
        if (t < 1.0f) {
            if (award->wait != 0) {
                pos = award->oldpos0;
                t =
                    (((player->obj).anim.anim_time - (tumble_item_starttime + 1.0f))
                     / (tumble_item_addtime - (tumble_item_starttime + 1.0f)));
                if (t > 1.0f) {
                    t = 1.0f;
                }
                if (t < 0.333f) {
                    scale = 0.0f;
                } else {
                    scale = ((t - 0.333f) / 0.667f);
                }
            } else {
                if (t < 0.666f) {
                    scale = 1.0f;
                    pos = award->oldpos0;
                } else {
                    scale = 1.0f;
                    f = (t - 0.666f) / 0.33399999f;
                    pos.x = (award->newpos.x - award->oldpos0.x) * f + award->oldpos0.x;
                    pos.y = (award->newpos.y - award->oldpos0.y) * f + award->oldpos0.y;
                    pos.z = (award->newpos.z - award->oldpos0.z) * f + award->oldpos0.z;
                }
            }
            i_obj = -1;
            i_chr = -1;
            if ((award->got & 4) != 0) {
                i_obj = 1;
            } else if ((award->got & 2) != 0) {
                i_obj = 2;
            } else if ((award->got & 1) != 0) {
                i_obj = 3;
            } else {
                if (award->got == 8) {
                    i_obj = 0xc1;
                } else if (award->got == 0x10) {
                    i_obj = 0xc2;
                } else if (award->got == 0x20) {
                    i_obj = 0xc3;
                } else if (award->got == 0x40) {
                    i_obj = 0xc4;
                } else if (award->got == 0x80) {
                    i_obj = 0xc5;
                } else if (award->got == 0x100) {
                    i_obj = 0xc6;
                } else if (award->got == 0x200) {
                    i_obj = 200;
                } else if (award->got == 0x400) {
                    i_obj = 199;
                }
            }
            if (i_obj != -1) {
                f = (scale * 0.76999998f);
                if (ObjTab[i_obj].obj.special != NULL) {
                    Draw3DObject(
                        i_obj, &pos, 0, award->yrot, 0, f, f, f, ObjTab[i_obj].obj.scene, ObjTab[i_obj].obj.special, 0
                    );
                }
            } else if (i_chr != -1) {
                f = (scale * 0.76999998f);
                if (CRemap[i_chr] != -1) {
                    F(&pos, 0, award->yrot, 0, &CModel[CRemap[i_chr]], -1, f, 1.0f, 0);
                }
            }
        }
        award++;
    }
    return;
}

//NGC MATCH
s32 qrand(void) {
  qseed = qseed * 0x24cd + 1U & 0xffff;
  return qseed;
}

//NGC MATCH
s32 RotDiff(u16 a0,u16 a1) {
  s32 diff;
  
  diff = a1 - a0;
  if (0x8000 < diff) {
    return diff + -0x10000;
  }
  if (-0x8001 < diff) {
    return diff;
  }
  return diff + 0x10000;
}

//NGC MATCH
u16 SeekRot(u16 a0,u16 a1,s32 shift) {
  s32 d;
  
  d = a1 - a0;
  if (d > 0x8000) {
    d = d + -0x10000;
  }
  else {
    if (d < -0x8000) {
      d = d + 0x10000;
    }
  }
  return a0 + (d >> shift);
}

//91%
u16 TurnRot(u16 a0,u16 a1,s32 rate) {
  s32 d;
  
  if (a1 != a0) {
    d = RotDiff(a0,a1);
    if (d > 0) {
      if (d > rate) {
       return a0 + rate & 0xffff;
      }
    }
    if (d < -rate) {
        return a0 - rate & 0xffff;
    }
  }
  return a1;
}

//NGC MATCH
void ResetCheckpoint(s32 iRAIL,s32 iALONG,float fALONG,struct nuvec_s *pos) {
  
  if ((((pos != NULL) && (iRAIL != -1)) && (Rail[iRAIL].type == '\0')) &&
     ((cp_iRAIL == -1 || (FurtherALONG(iRAIL,iALONG,fALONG,cp_iRAIL,cp_iALONG,cp_fALONG) != 0)))) {
    cpPOS = *pos;
    cp_iRAIL = iRAIL;
    cp_iALONG = iALONG;
    cp_fALONG = fALONG;
    if (Game.language == 99) {
      nCheckLetters = JStrLen(tGAMEOVER[3]);
    }
    else {
      nCheckLetters = strlen(tCHECK[Game.language]);
    }
    check_delay = CPLDELAY + CPLTIME * ((s32)nCheckLetters);
    check_duration = CHECKWAIT + check_delay + ((s32)nCheckLetters - 1) * CPLTIME;
    check_time = 0.0f;
    if (Game.language == 99) {
      nPointLetters = JStrLen(tYES[3]);
    }
    else {
      nPointLetters = strlen(tPOINT[Game.language]);
    }
    point_delay = CPLDELAY + CPLTIME * ((s32)nPointLetters);
    point_time = point_duration = CHECKWAIT + point_delay + ((s32)nPointLetters - 1) * CPLTIME;
    LivesLost = 0;
    ClockOff();
    OpenPreviousCheckpoints(iRAIL,iALONG,fALONG);
  }
  else {
    cp_iALONG = -1;
    cp_iRAIL = -1;
  }
  return;
}

//NGC MATCH
s32 AheadOfCheckpoint(s32 iRAIL,s32 iALONG,float fALONG) {
if ((cp_iRAIL == -1) || (cp_iALONG == -1))
{
    return 1;
}
  if (((iRAIL == -1)) ||  (((iALONG == -1 || (Rail[iRAIL].type != 0)) || (iRAIL > cp_iRAIL)))) {
      return 1;
  }
    if (iRAIL < cp_iRAIL) {
      return 0;
    }
    if (iALONG > cp_iALONG) { 
        return 1; 
    } else if (iALONG >= cp_iALONG) {
        return (fALONG > cp_fALONG);
    } else if (iALONG >= cp_iALONG) {
        // This just creates the final return expression
        return (fALONG > cp_fALONG);
    } 

    return 0;
}

//NGC MATCH
void ResetLoadSaveCharacter(void) {
  tumble_action = -1;
  tumble_duration = 0.0f;
  tumble_time = 0.0f;
  last_level = -1;
  last_hub = -1;
  Hub = -1;
  (player->obj).hdg = 0x8000;
  if (pos_START != NULL) {
    (player->obj).pos = *pos_START;
    (player->obj).shadow = NewShadowMaskPlat(&(player->obj).pos,0.0f,-1);
  }
  return;
}

//NGC MATCH
void DefaultTimeTrialNames(s32 all) {
  char *txt;
  s32 i;
  s32 j;
  
  for (i = 0; i < 0x23; i++) {
        for (j = 0; j < 3; j++) {
        txt = Game.level[i].time[j].name;
          if (all != 0) {
              strcpy(txt,PlaceName3[j][Game.language]);
          }
            else if (*txt == '1') {
              strcpy(txt,PlaceName3[0][Game.language]);
            }
            else if (*txt == '2') {
              strcpy(txt,PlaceName3[1][Game.language]);
            }
            else if (*txt == '3') {
              strcpy(txt,PlaceName3[2][Game.language]);
            }
        }
  }
}

//87% NGC
void NewLevelTime(s32 t) {
    s32 i = 0;
    s32 iVar6 = 0;

    s32 time;

    while (time = Game.level[Level].time[iVar6++].itime,
            time != 0 && (t > time)) {
        if (iVar6 > 2) {
            newleveltime_slot = iVar6;
            return;
        }
    }
    for (i = 2; i > iVar6; i--) {
        Game.level[Level].time[i] = Game.level[Level].time[i - 1];
    }
    Game.level[Level].time[iVar6].itime = t;
    strcpy(Game.level[Level].time[iVar6].name, "___");
    if ((iVar6 < 2) && (Game.level[Level].time[2].itime == 0)) {
        strcpy(Game.level[Level].time[2].name, PlaceName3[2][Game.language]);
    }
    if ((iVar6 < 1) && (Game.level[Level].time[1].itime == 0)) {
        strcpy(Game.level[Level].time[1].name, PlaceName3[1][Game.language]);
    }
    newleveltime_slot = iVar6;
}

//NGC MATCH
void InputNewLetter(struct cursor_s *cursor,char *name,s32 *i,s32 count) {
  char c0;
  s32 j;
  
  c0 = NameInputTable[cursor->y][cursor->x];
  if (c0 == '<') {
    if (*i > 0) {
      (*i)--;
      name[*i] = '_';
    }
    return;
  }
  if ((*i == 0) && (name[1] == 'R')) {
    name[1] = '_';
    name[2] = '_';
    name[3] = '_';
    name[4] = '_';
  }

  j = count - 1;
  if (*i >= j) {
    cursor->y = '\x04';
  } else{
      j = *i;
  }
     name[j] = c0;
      if (*i < count) {
        *i = *i + 1;
      }
  return;
}

void ProcMenu(struct cursor_s *cursor,struct nupad_s *pad) {
  static s32 old_lang;
  s32 bVar1;
  char cVar2; //temp
  char cVar3; //operation
  s32 bVar4;
  u32 uVar5;
  u32 uVar6;
  u32 uVar7;
  //char cVar14;
  //char cVar15;
  u32 uVar16;
  u32 uVar17;
  s32 iVar18;
  char *txt;
  s32 bits_db;
  //s32 uVar21;
  //float fVar21;
  s32 bits;
  s32 iVar22;
  s32 iVar28;
  s32 iVar29;
  s32 test_sfx [4] = {0x25 , 0x19, 0x13, 0x2c};
  
  //bits_db = menu_pulsate_ang & 0xffff;
  menu_pulsate = NuTrigTable[menu_pulsate_ang & 0xffff] * 0.1f + 1.0f;
  menu_pulsate_ang += menu_pulsate_rate;
  if (0x10000 < menu_pulsate_ang) {
    menu_pulsate_ang -= 0x10000;
  }
  if ((cursor->wait != '\0') && (cursor->wait--, cursor->wait == '\0')) {
    cursor->wait_hack = 1;
    NewMenu(cursor,(s32)cursor->new_menu,-1,-1);
  }
  if (ForceRestart == 0) {
    if (cursor->menu == -1) {
      return;
    }
    if (cursor->wait != '\0') {
      return;
    }
    if (0 < fadeval) {
      return;
    }
    if (Paused - 1U < 0x1d) { //compiler opt.
      return;
    }
    if (cutmovie != -1) {
         if (cut_on == 0) {
          return;
        }
    }
     else if (new_mode != -1) {
        return;
      }
    else if (new_level != -1) {
        return;
      }
    if ((cursor->menu == '\x14') && (gameover_hack != 1)) {
      return;
    }
  }
  iVar28 = -1;
  if (cursor->button_lock != '\0') {
    cursor->button_lock--;
  }
  if ((pad != NULL) && (cursor->button_lock == '\0')) {
    //fVar21 = pad->r_dy;
      bits = pad->rdy;
    if (((s32)bits & 0xf000) == 0) {
      bits = (stick_bits | bits);
    }
    bits_db = pad->oldpaddata;
    if ((bits_db & 0xf000) == 0) {
      bits_db = bits_db | stick_bits_db;
    }
  }
  else {
    bits_db = 0;
    bits = 0;
  }
  uVar16 = bits_db & 0x840;
  uVar5 = bits_db & 0x180;
  uVar6 = bits & 0x8000;
  uVar7 = bits & 0x2000;
  if (cursor->item_frame == 0) {
    uVar16 = 0;
  }
  uVar17 = uVar16;
  if (ForceRestart != 0) {
    ForceRestart = 0;
    uVar17 = 1;
    cursor->menu = '\v';
  }
  //cVar14 = cursor->y_max;
  //cVar15 = cursor->y_min;
  cVar2 = cursor->y;
  cVar3 = (cursor->y_max - cursor->y_min) + 1;
  if (cursor->y < cursor->y_min) {
    cursor->y = cursor->y_min;
  }
  else if (cursor->y > cursor->y_max) {
    cursor->y = cursor->y_max;
  }
  if ((bits_db & 0x4000) != 0) {
    //cVar14 = cursor->y + 1;
    cursor->y++;
    if (cursor->y > cursor->y_max) {
      cursor->y -= cVar3;
//LAB_80032034:
      //cursor->y = cVar14;
    }
  }
  else {
    if ((bits_db & 0x1000) != 0) {
      //cVar14 = cursor->y + -1;
      cursor->y--;
      if (cursor->y < cursor->y_min) {
        cursor->y = cursor->y + cVar3;
        //goto LAB_80032034;
      }
    }
  }
  cursor->remember[cursor->menu].y = cursor->y;
  if (cursor->y != cVar2) {
    iVar28 = 0x18;
    cursor->item_frame = 0;
    uVar16 = 0;
    uVar17 = 0;
  }
  //cVar14 = cursor->x_max;
  //cVar15 = cursor->x_min;
  cVar2 = cursor->x;
  cVar3 = (cursor->x_max - cursor->x_min) + 1;
  if (cursor->x < cursor->x_min) {
    cursor->x = cursor->x_min;
  }
  else if (cursor->x > cursor->x_max) {
    cursor->x = cursor->x_max;
  }
  if (cursor->y < cursor->y_max) {
    if ((bits_db & 0x2000) != 0) {
      //cVar14 = cursor->menu;
      //cVar15 = cursor->x + 1;
      cursor->x++;
      if (((cursor->menu == '\x16') || (cursor->menu == '\x19')) || (cursor->menu == '\x1d')) {
        if (cursor->x > cursor->x_max) {
          cursor->x = cursor->x_max;
        }
      }
      else if (cursor->x > cursor->x_max) {
        cursor->x -= cVar3;
//LAB_800321ac:
        //cursor->x = cVar15;
      }
    }
    else if ((bits_db & 0x8000) != 0) {
        //cVar14 = cursor->menu;
        //cVar15 = cursor->x + -1;
        cursor->x--;
        if (((cursor->menu == '\x16') || (cursor->menu == '\x19')) || (cursor->menu == '\x1d')) {
          if (cursor->x < cursor->x_min) {
            cursor->x = cursor->x_min;
          }
        }
        else if (cursor->x < cursor->x_min) {
          cursor->x += cVar3;
          //goto LAB_800321ac;
        }
      }
  }
  cursor->remember[cursor->menu].x = cursor->x;
  if (cursor->x != cVar2) {
    iVar28 = 0x18;
    cursor->item_frame = 0;
    uVar16 = 0;
    uVar17 = 0;
  }
  if ((uVar17 != 0) || (uVar5 != 0)) {
    cursor->item_frame = 0;
  }
  iVar29 = Level;
  cursor->menu_frame++;
  cursor->item_frame++;
  //cursor->item_frame = uVar17;
  iVar22 = cortex_quit_i;
  iVar18 = cortex_continue_i;
  if ((((iVar29 == 0x23) || (cutmovie == 0)) && (cursor->menu == '\0')) && (0x707 < cursor->item_frame)) {
    new_level = DemoLevel[i_demolevel];
    Demo = 1;
    i_demolevel++;
    if (i_demolevel == 3) {
      i_demolevel = 0;
    }
    InvincibilityCHEAT = 0;
    if (cutmovie != 0) {
      return;
    }
    Level = new_level;
    fade_rate = 8;
    return;
  }
  //cVar15 = cursor->y;
  iVar29 = (s32)cursor->y;
switch(cursor->menu){
    case 5:
        if (uVar17 == 0) break; //goto LAB_800346ac;
        iVar28 = 0x36;
        switch(iVar29) {
            case 0:
            pause_dir = 2;
            //goto LAB_800346ac;
            break;
            case 1:
            NewMenu(cursor,6,0,-1);
            storemenu1();
            //goto LAB_800346ac;
            break;
            case 2:
              iVar22 = 1;
              iVar18 = 10;
              goto LAB_80034684;
            break;
            case 3:
            if ((iVar29 != 3) || (TimeTrial == 0)) break;//goto LAB_800346ac;
            new_mode = GameMode;
            ResetCheckpoint(-1,-1,0.0f,NULL);
            LivesLost = 0;
            break;
        }
           // goto LAB_800346ac;
    break;
    case 6:
          if (uVar17 != 0) {
            if (iVar29 == 0) {
              iVar28 = 0x36;
              Game.vibration = 1 - Game.vibration;
              break;//goto LAB_800346ac;
            }
            if (iVar29 == 1) {
              storemenu2();
              iVar28 = 0x36;
              iVar18 = 7;
              goto LAB_80034680;
            }
            if ((LANGUAGEOPTION != 0) && (iVar29 == 2)) {
              iVar28 = 0x36;
              NewMenu(cursor,9,(u32)Game.language,-1);
              break; //goto LAB_800346ac;
            }
            if (iVar29 != cursor->y_max) break; //goto LAB_800346ac;
            iVar28 = 5;
            iVar18 = -1;
            break; //goto LAB_800346a0;
          }
          if (uVar5 == 0) break; //goto LAB_800346ac;
          restoremenu1();
          iVar28 = 0x3c;
          iVar22 = -1;
          iVar18 = 5;
    break;
    case '\a':
              if (uVar17 != 0) {
            if (iVar29 != 2) break; //goto LAB_800346ac;
            iVar28 = 6;
            iVar18 = -1;
            goto LAB_800346a0;
          }
          if (uVar5 != 0) {
          restoremenu2();
          iVar28 = 0x3c;
          iVar22 = -1;
          iVar18 = 6;
        goto LAB_80034684;
          } 
          else {
              switch(iVar29) {
                case 0:
                  iVar18 = qrand();
                  if (iVar18 < 0x1000) {
                    gamesfx_volume = (s32)Game.sfx_volume;
                    iVar18 = qrand();
                    if (iVar18 < 0) {
                      iVar18 = iVar18 + 0x3fff;
                    }
                    GameSfx(test_sfx[iVar18 >> 0xe],NULL);
                  }
                  if ((uVar6 == 0) || (Game.sfx_volume == '\0')) {
                    if ((uVar7 != 0) && (Game.sfx_volume < 100)) {
                      Game.sfx_volume = Game.sfx_volume + 1;
                    }
                  }
                  else {
                    Game.sfx_volume++;
                  }
                  break;
                case 1:
                      if ((uVar6 == 0) || (Game.music_volume == '\0')) {
                        if ((uVar7 != 0) && (Game.music_volume < 100)) {
                          Game.music_volume = Game.music_volume + 1;
                        }
                      }
                      else {
                        Game.music_volume++;
                      }
                  break;
              }
              //goto LAB_800346ac;
          }

    break;
    case '\t':
            if (uVar17 != 0) {
              iVar28 = 0x36;
              NewLanguage(iVar29);
              iVar22 = -1;
              iVar18 = 6;
              goto LAB_80034684;
            }
            if (uVar5 == 0) break; //goto LAB_800346ac;
            iVar28 = -1;
            iVar18 = 6;
        //goto LAB_80034620;
    NewMenu(cursor,iVar18,iVar28,-1);
  iVar28 = 0x3c;
    break;
    case '\n':
            if (uVar17 == 0) {
              if (uVar5 == 0) break; //goto LAB_800346ac;
              iVar28 = -1;
              iVar18 = 5;
            }
            else {
              if (iVar29 == 0) {
                new_level = 0x25;
                if (Level == 0x25) {
                  new_level = 0x23;
                }
                goto LAB_800346a8;
              }
              iVar28 = -1;
              iVar18 = 5;
            }
        //goto LAB_80034620;
    NewMenu(cursor,iVar18,iVar28,-1);
  iVar28 = 0x3c;
    break;
    case '\v':
      if (cursor->item_frame == 0) {
        if (uVar5 == 0) break; //goto LAB_800346ac;
        //iVar22 = -1;
        //iVar18 = 5;
          NewMenu(cursor,5,-1,-1);
      }
      else {
        switch(iVar29){
            case 0:
                new_mode = GameMode;
                ResetCheckpoint(-1,-1,0.0f,NULL);
                LivesLost = iVar29;
                LostLife = iVar29;
                ResetBonus();
                ResetDeath();
                ResetGemPath();
                ResetItems();
                //goto LAB_800346ac;
            break;
            case 1:
                ResetCheckpoint(-1,-1,0.0f,NULL);
                LivesLost = 0;
                LostLife = 0;
                ResetBonus();
                bonus_restart = 0;
                ResetDeath();
                ResetGemPath();
                RestoreCrateTypeData();
                ResetCrates();
                ResetWumpa();
                ResetChases();
                ResetPlayerEvents();
                ResetGates();
                ResetRings();
                ResetMaskFeathers();
                ResetAI();
                ResetPlayer(0);
                ResetBug();
                ResetLevel();
                ResetProjectiles();
                NewMenu(cursor,-1,-1,-1);
                Paused = 0;
                pause_dir = 0;
                ResumeGame();
                edobjResetAnimsToZero();
                //goto LAB_800346ac;
            break;
            case 2:          
                //iVar22 = -1;
                //iVar18 = 0xe;
                NewMenu(cursor,0xe,-1,-1);
            break;
            case 3:
              InvincibilityCHEAT = 1 - InvincibilityCHEAT;
              ResetAI();
              //goto LAB_800346ac;
            break;
            case 4:
            //uVar21 = 0;
          //iVar18 = GotoCheckpoint(&(player->obj).pos,0);
          if (GotoCheckpoint(&(player->obj).pos,0) != 0) {
            new_mode = GameMode;
          }
            break;
            case 5:
          //uVar21 = 1;
          //iVar18 = GotoCheckpoint(&(player->obj).pos,1);
          if (GotoCheckpoint(&(player->obj).pos,1) != 0) {
            new_mode = GameMode;
          }
          //goto LAB_800346ac;
            break;
            case 6:
            OpenGame();
            //iVar22 = -1;
            //iVar18 = 5;
            NewMenu(cursor,5,-1,-1);
            break;
            case 7:
                LIFTPLAYER = 1 - LIFTPLAYER;
              //goto LAB_800346ac;
            break;
            case 8:
            ShowPlayerCoordinate = 1 - ShowPlayerCoordinate;
            //goto LAB_800346ac;
            break;
            case 9:
              ExtraMoves = 1 - ExtraMoves;
             // goto LAB_800346ac;
            break;
            case 10:
            NewGame();
            ResetBonus();
            ResetDeath();
            ResetGemPath();
            CalculateGamePercentage(&Game);
            Hub = HubFromLevel(Level);
            //iVar22 = -1;
            //iVar18 = 5;
            NewMenu(cursor,5,-1,-1);
            break;
        } 
      }
    break;
    case '\x0f':
        if (uVar17 == 0) {
          if (uVar5 == 0) break; //goto LAB_800346ac;
          iVar22 = -1;
          iVar18 = 0xb;
          goto LAB_80034684;
        }
        switch(iVar29){
            case 2:
                cutmovie = iVar29;
            break;
            case 0:
                logos_played = 0;
                cutmovie = iVar29;
            break;
            case 1:
                cutmovie = iVar29;
            break;
            case 3:
                cutmovie = iVar29;
            break;
            case 4:
                cutmovie = iVar29;
            break;
        }
        NewMenu(cursor,-1,-1,-1);
        goto LAB_8003389c;
    break;
    case '\f':
      if (uVar17 != 0) break; //goto LAB_800346ac;
      if (uVar5 != 0) {
        iVar22 = -1;
        iVar18 = 0xb;
        goto LAB_80034684;
      }
            switch(iVar29) {
                case 0:
                  if ((uVar6 != 0) && (LDATA->farclip > 0xa)) {
                    LDATA->farclip--;
                  }
                  else {
                    if ((uVar7 != 0) && (LDATA->farclip < 1000)) {
                      LDATA->farclip++;
                    }
                  }
                  if ((s32)LDATA->fogfar > LDATA->farclip) {
                    LDATA->fogfar = (float)LDATA->farclip;
                  }
                  if ((s32)LDATA->fogfar > LDATA->fognear) {
                    LDATA->fognear = LDATA->fogfar;
                  }
                  if (pNuCam != NULL) {
                    pNuCam->farclip = (float)LDATA->farclip;
                  }
                  //goto LAB_800346ac;  
            break;
                case 1:
                      if ((uVar6 == 0) || (LDATA->fognear > 1.0f)) {
                        LDATA->fognear -= 1.0f;
                        } else {
                            if ((uVar7 != 0) && ((s32)LDATA->fognear < LDATA->farclip) && ((s32)LDATA->fognear < (s32)LDATA->fogfar))
                            {
                              LDATA->fognear += 1.0f;
                            }
                      }
                break;
                case 2:
                    if (((uVar6 != 0) && (LDATA->fogfar > 1.0f)) && (LDATA->fogfar > LDATA->fognear )) {
                      LDATA->fogfar -= 1.0f;
                    }
                    else {
                      if ((uVar7 != 0) && ((s32)LDATA->fogfar < (s32)(u32)LDATA->farclip)) {
                        LDATA->fogfar += 1.0f;
                      }
                    }
                    //break; //goto LAB_800346ac;
                break;
                case 3:
                      if ((uVar6 != 0) && (LDATA->fogr != '\0')) {
                        LDATA->fogr--;
                      }
                      else {
                        if ((uVar7 != 0) && (LDATA->fogr < 0xff)) {
                          LDATA->fogr++;
                        }
                      }
                break;
                case 4:
                      if ((uVar6 != 0) && (LDATA->fogg != '\0')) {
                        LDATA->fogg--;
                      }
                      else {
                        if ((uVar7 != 0) && (LDATA->fogg < 0xff)) {
                          LDATA->fogg++;
                        }
                      }
                break;
                case 5:
                if ((uVar6 != 0) && (LDATA->fogb != '\0')) {
                  LDATA->fogb--;
                }
                else {
                  if ((uVar7 != 0) && (LDATA->fogb < 0xff)) {
                    LDATA->fogb++;
                  }
                }
                break; //goto LAB_800346ac;
                case 6:
                    if ((uVar6 != 0) && (LDATA->foga != '\0')) {
                      LDATA->foga--;
                    }
                    else {
                      if ((uVar7 != 0) && (LDATA->foga < 0x7f)) {
                        LDATA->foga++;
                      }
                    }
                break;
                case 7:
                    if ((uVar6 != 0) && (LDATA->hazer != '\0')) {
                      LDATA->hazer--;
                    }
                    else {
                      if ((uVar7 != 0) && (LDATA->hazer < 0xff)) {
                        LDATA->hazer++;
                      } 
                    }
                break;
                case 8:
                  if ((uVar6 != 0) && (LDATA->hazeg != '\0')) {
                    LDATA->hazeg--;
                  }
                  else {
                    if ((uVar7 != 0) && (LDATA->hazeg < 0xff)) {
                      LDATA->hazeg++;
                    }
                  }
                break;
                case 9:
                      if ((uVar6 != 0) && (LDATA->hazeb != '\0')) {
                        LDATA->hazeb--;
                      }
                      else {
                        if ((uVar7 != 0) && (LDATA->hazeb < 0xff)) {
                          LDATA->hazeb++;
                        }
                      }
                break;
                case 10:
                  if ((uVar6 != 0) && (LDATA->hazea != '\0')) {
                    LDATA->hazea--;
                  }
                  else {
                    if ((uVar7 != 0) && (LDATA->hazea < 0xff)) {
                      LDATA->hazea++;
                    } 
                  }
                break;
            }
    break;
    case '\x0e':
            if (uVar17 != 0) {
          iVar18 = (s32)HData[cursor->x].level[iVar29];
          if (iVar18 != -1) {
            new_level = iVar18;
            Hub = HubFromLevel(iVar18);
          }
          break; //goto LAB_800346ac;
        }
        if (uVar5 == 0) break; //goto LAB_800346ac;
        iVar22 = -1;
        iVar18 = 0xb;
        goto LAB_80034684;
    break;  
    case 0: // < '\f'
      wasloadgame = 0;
      loadsaveCallEachFrame();
      if (uVar17 != 0) {
        switch(iVar29){
            case 0:
                  MemCardRetry();
                  iVar28 = ParseNintendoErrorCode();
                  if (iVar28 != 0) {
                  DestMenu = 4;
                  iVar28 = 1;
                  SaveMenu = 0;
    LAB_800330a4:
                  NewMenu(cursor,iVar28,0,-1);
                  goto LAB_800330b0;
                  } else{
                        NewMenu(cursor,4,0,-1);
                        goto LAB_800330b0;
                  }
            break;
            case 1:
                wasloadgame = iVar29;
                MemCardRetry();
                iVar28 = ParseNintendoErrorCode();
                if (iVar28 != 0) {
                  SaveMenu = 0;
                  DestMenu = 0;
                  iVar28 = 1;
                  goto LAB_800330a4;
                }
                memcpy(Game.name, ",CRASH   ", 9);
                new_level = 0x25;
                force_menu = 0x15;
                if (cutmovie == 0) {
                  fade_rate = 8;
                }
        LAB_800330b0:
                iVar28 = 0x36;
            break;   
            case 2:
                  iVar28 = 0x27;
                  goto LAB_800330a4;
            break;
            case 3:
                  old_lang = (s32)Game.language;
                  Game.language = (old_lang + 1U);
                  if (5 < (old_lang + 1U & 0xff)) {
                    Game.language = '\0';
                  }
            break;
        }
      } else{
        if (iVar29 == 3) {
          old_lang = (s32)Game.language;
          if (((bits_db /*& 0x2000*/) != 0) && ((u32)old_lang < 5)) {
            Game.language++;
          }
          else {
            if (((bits_db & 0x8000) != 0) && (Game.language != '\0')) {
              Game.language--;
            }
          }
        }
        //goto LAB_80033140;
      }
//LAB_80033140:
      if ((u32)Game.language != old_lang) {
        NewLanguage((u32)Game.language);
        iVar28 = 0x36;
        old_lang = (s32)Game.language;
      }
      //goto LAB_800346ac;
    break;    
    case 3:
        if (uVar17 != 0) {
          if (NotEnoughSpace_NewGame != 0) {
            iVar28 = 4;
            goto LAB_8003469c;
          }
          new_level = 0x25;
          force_menu = 0x15;
          if (cutmovie != 0) {
              break;
          } 
            fade_rate = 8;
          goto LAB_800346a8;
        }
         else {
            if (uVar5 != 0) {
              NuSoundKillAllAudio();
              XbWaitForAllBuffersToStop();
              XbLaunchDashboardToFreeSpace();
            }
            break; //goto LAB_800346ac;
          }
    break;
    case 1:
        loadsaveCallEachFrame();
        if (uVar17 != 0) {
            iVar18 = ParseNintendoErrorCode();
            iVar28 = 0x36;
            if (iVar18 == 0) {
                  if (wasloadgame != 0) {
                    new_level = 0x25;
                    force_menu = 0x15;
                    if (cutmovie == 0) {
                      fade_rate = 8;
                    }
                    break; //goto LAB_800346ac;
                  }
                  iVar18 = 4;
                  goto LAB_80034680;
            }
            if (iVar29 == 0) {
                  memtest_done = iVar29;
                  MemCardRetry();
                  iVar28 = ParseNintendoErrorCode();
                  if (iVar28 == 0) {
                    if (wasloadgame == 0) {
                      iVar28 = 4;
                    }
                    else {
                      iVar28 = 0;
                    }
        LAB_8003469c:
                    iVar18 = 0;
                    goto LAB_800346a0;
                  }
            }
            else {
                  if (iVar29 == 1) {
                    if (wasloadgame == 0) {
                      iVar28 = 4;
                    }
                    else {
                      iVar28 = 0;
                    }
                    goto LAB_8003469c;
                  }
                  if (iVar29 == 2) {
                        iVar28 = ParseNintendoErrorCode();
                      switch(iVar28){
                          case 6:
                          Managememcard();
                          break;
                          case 8:
                          SaveMenu = wasloadgame;
                          if (wasloadgame != 0) {
                            SaveMenu = 0;
                          }
                          NewMenu(cursor,0x30,1,-1);
                          break;
                          case 2:
                          case 3:
                            iVar28 = 0x20;
                            iVar18 = 1;
                            goto LAB_800346a0;
                          break;
                      }
                  }
            }      
        }
        else{
            if (uVar5 == 0) break; //goto LAB_800346ac;
            iVar28 = -1;
            iVar18 = 0;
          //goto LAB_80034620;
            NewMenu(cursor,iVar18,iVar28,-1);
            iVar28 = 0x3c;
            break;
        }

        goto LAB_800346a8;
    break;
case 0x2f:
              if (uVar17 != 0) {
                iVar22 = ParseNintendoErrorCode();
                iVar18 = SaveMenu;
                if (iVar22 == 0) goto LAB_80034680;
                if (iVar29 == 0) {
                  memtest_done = iVar29;
                  MemCardRetry();
                  iVar28 = SaveMenu;
                  goto LAB_8003469c;
                }
                if (iVar29 != 1) {
                  if (iVar29 == 2) {
                    iVar28 = ParseNintendoErrorCode();
                    if (iVar28 == 6) {
                      Managememcard();
                    }
                    else if (iVar28 < 7) {
                      if ((iVar28 < 4) && (1 < iVar28)) {
                        iVar28 = 0x20;
                        iVar18 = 1;
                        goto LAB_800346a0;
                      }
                    }
                    else if (iVar28 == 8) {
                      SaveMenu = 0x15;
                      NewMenu(cursor,0x30,1,-1);
                    }
                  }
                  goto LAB_800346a8;
                }
                iVar28 = 0x15;
                iVar18 = -1;
                        //NewMenu(cursor,0x15,-1,-1);
                goto LAB_800346a0;
              }
              if (uVar5 == 0) break; //goto LAB_800346ac;
              //iVar28 = -1;
              //iVar18 = BackMenu;
            NewMenu(cursor,-1,BackMenu,-1);
break;
    case 2:
        switch(memtest_done){
            case 0:
                loadsaveCallEachFrame();
            if (saveload_status == 1) {
                  if (saveload_cardtype == 2) {
                    memtest_done = 3;
                  }
                  else {
                    memtest_done = saveload_status;
                  }
            }
            break;
            case 1:
            case 2:
                loadsaveCallEachFrame();
                if (uVar17 != 0) {
                  iVar28 = 0x36;
                  memtest_done = 3;
                }
            break;
            case 3:
              NewMenu(cursor,-1,-1,-1);
              if (CutAudio[cutworldix] != -1) {
                gamesfx_channel = 4;
                GameSfx(CutAudio[cutworldix],NULL);
              }
            break;
        }  
    //goto LAB_800346ac;
    break;
    case '\x04':
        if (uVar17 != 0) {
            iVar28 = 0x36;
            iVar18 = (s32)cursor->y_max;
            if (iVar29 == iVar18 - 1) {
              CleanLetters(Game.name);
              iVar18 = strcmp(Game.name,"        ");
              if (iVar18 == 0) {
                memcpy(Game.name, ",CRASH   ", 9);
              }
              new_level = 0x25;
              if (cutmovie == 0) {
                next_cut_movie = 1;
                NewMenu(cursor,-1,-1,-1);
                fade_rate = 8;
              }
              else {
                cutmovie = 1;
              }
              txt = strstr(Game.name,"WOMBAT");
              if (txt != NULL) {
                memcpy(Game.name, ",CRASH   ", 9);
                cheating = 1;
                OpenGame();
              }
              break; //goto LAB_800346ac;
            }
            if (iVar29 == iVar18) {
              iVar18 = 0;
              iVar22 = -1;
              goto LAB_80034684;
            }
            if (Game.language == 'c') {
              iVar22 = 2;
              if ((iVar29 != iVar18 - 2) && (iVar22 = 1, iVar29 != iVar18 - 3)) {
                    if (iVar29 == iVar18 - 4) {
                        iVar22 = 0;
                        input_alphabet = iVar22;
                        GetMenuInfo(cursor);
                    } else {
                          if ((input_alphabet == 2) && ((cursor->y == '\0' || (cursor->y == '\x04')))) break; //goto LAB_800346ac;
                          goto LAB_80033630;     
                    }
              }
            }
            else {
    LAB_80033630:
              InputNewLetter(cursor,Game.name,&i_nameinput,8);
            }
        } else {
              if (uVar5 == 0) break; //goto LAB_800346ac;
              iVar28 = 0;
              iVar18 = -1;
              goto LAB_800346a0;
        }
        //goto LAB_800346ac;
    break; 
    case '\x13':
    if ((cursor->menu != '\x13') || (((bits & 0x840) == 0 && (cut_on != 0)))) break; //goto LAB_800346ac;
    iVar28 = (s32)cursor->new_level;
    if (cursor->new_level == -1) {
      NewMenu(cursor,-1,-1,-1);
      iVar28 = new_level;
    }
    break;
    case '\x0d':
        if ((cursor->menu != '\r') || (uVar5 == 0)) break; //goto LAB_800346ac;
        iVar22 = -1;
        iVar18 = 0xb;
        goto LAB_80034684;
    break;
    case '#':
      if (((bits & 0x840) == 0) && !(GameTimer.ftime >= credit_time)) break; //goto LAB_800346ac;
        goto LAB_8003389c;
//LAB_8003389c:
      //new_level = 0x25;
      //goto LAB_800346ac;
    break;   
    case '\x14':
      if ((TempAnim.newaction == 0x22) && (uVar17 != 0)) {
        if (iVar29 == 0) {
          iVar28 = cortex_continue_i + 1;
          cortex_gameover_i = cortex_continue_i;
          cortex_continue_i = iVar28;
          if (iVar28 == 2) {
            cortex_continue_i = 0;
          }
          tempanim_nextaction = (s32)*(short *)(cortex_gameover_tab + iVar18 * 4);
          if ((CRemap[2] == -1) ||
             (CModel[CRemap[2]].anmdata[tempanim_nextaction] == NULL)) {
            Game.lives = '\x04';
            new_level = 0x25;
            just_continued = 1;
          }
          else {
            gamesfx_channel = 4;
            GameSfx((s32)*(short *)(cortex_gameover_tab + iVar18 * 4 + 2),NULL);
            tempanim_waitaudio = 1;
          }
          iVar28 = 0x36;
          gameover_hack = 2;
        }
        else if (iVar29 == 1) {
          iVar28 = cortex_quit_i + 1;
          cortex_gameover_i = cortex_quit_i;
          cortex_quit_i = iVar28;
          if (iVar28 == 7) {
            cortex_quit_i = 2;
          }
          tempanim_nextaction = (s32)*(short *)(cortex_gameover_tab + iVar22 * 4);
          if ((CRemap[2] == -1) ||
             (CModel[CRemap[2]].anmdata[tempanim_nextaction] == NULL)) {
            new_level = 0x23;
          }
          else {
            gamesfx_channel = 4;
            GameSfx((s32)*(short *)(cortex_gameover_tab + iVar22 * 4 + 2),NULL);
            tempanim_waitaudio = 1;
          }
          iVar28 = 0x3c;
          gameover_hack = 2;
        }
      }
      //goto LAB_800346ac;
    break;
    case '\x10':
    case '\x12':
LAB_80033850:
          if (uVar17 == 0) break; //goto LAB_800346ac;
          iVar28 = 0x36;
          if (iVar29 != 0) {
            if (iVar29 == 1) goto LAB_8003389c; //goto LAB_800346ac;
            goto LAB_8003389c;
          }
          new_mode = GameMode;
          ResetCheckpoint(-1,-1,0.0f,NULL);
          //goto LAB_8003388c;
            iVar28 = 0x36;
            ResetBonus();
            ResetDeath();
            ResetGemPath();
            //goto LAB_800346ac;
    break;  
/////////////////////////////////
LAB_8003389c:
      new_level = 0x25;
    break;
///////////////////////////////////
    case '\x11':
      if (uVar17 == 0) break; //goto LAB_800346ac;
      iVar28 = (s32)cursor->y_max;
      if (iVar29 == iVar28) {
        iVar28 = newleveltime_slot * 8;
        //iVar22 = Game.level[Level].time[0].name;
        txt = Game.level[Level].time[iVar28].name;
        CleanLetters(txt);
        iVar18 = strcmp(txt,"   ");
        if (iVar18 == 0) {
          txt = (char  )0x20203f00;
        }
        if ((new_lev_flags & 7) == 0) {
          iVar28 = 0x10;
          goto LAB_8003469c;
        }
        new_level = 0x25;
        goto LAB_800346a8;
      }
      if (Game.language == 'c') {
        iVar18 = 2;
        if ((iVar29 != iVar28 + -1) && (iVar18 = 1, iVar29 != iVar28 + -2)) {
          if (iVar29 == iVar28 - 3) {
          //iVar18 = 0;
        input_alphabet = 0;
        GetMenuInfo(cursor);
        } else {
            if ((input_alphabet == 2) && ((iVar29 == 0 || (iVar29 == 4)))) goto LAB_800346a8;
            //goto LAB_80033998;
              InputNewLetter(cursor,Game.level[Level].time[newleveltime_slot].name, &i_nameinput,3);
          }
        }
      }
      else {
//LAB_80033998: //
        //InputNewLetter(cursor,Game.level[Level].time[newleveltime_slot].name, &i_nameinput,3);
      }
      //goto LAB_800346a8;
        iVar28 = 0x36;
    break;
    case '\x1d':
      loadsaveCallEachFrame();
      if ((saveload_cardchanged != 0) || (saveload_error != 0)) {
        NewMenu(cursor,0x26,-1,-1);
        if ((saveload_error == 2) || (saveload_error == 0x10)) {
          saveload_error = 8;
        }
      }
      UpdateSaveSlots(cursor);
      if (uVar17 == 0) {
        if (uVar5 == 0) break; //goto LAB_800346ac;
        iVar28 = -1;
        iVar18 = 0x15;
      }
      else {
        if ((s32)cursor->y < (s32)cursor->y_max) {
          iVar28 = cursor->x * 2 + (s32)cursor->y;
          game = SaveSlot + iVar28;
          if (SaveSlot[iVar28].empty == '\0') {
LAB_800342c4:
            iVar28 = 2;
            break; //goto LAB_800346ac;
          }
LAB_1:
          iVar28 = 0x1e;
          iVar18 = 1;
          goto LAB_800346a0;
        }
        iVar28 = -1;
        iVar18 = 0x15;
      }
    break;
    case '\x15':
      bVar4 = 0;
      loadsaveCallEachFrame();
      if (uVar17 == 0) {
        if (uVar5 != 0) {
LAB_80033a6c:
          bVar4 = 1;
        }
        goto LAB_80033a70;
      }
      if (iVar29 == 1) {
        memcard_loadattempted = 0;
        UpdateSaveSlots(cursor);
        iVar18 = 0x19;
LAB_80033a54:
        iVar28 = 0x36;
        NewMenu(cursor,iVar18,-1,-1);
      }
      else if (iVar29 < 2) {
        if (iVar29 == 0) {
          memcard_loadattempted = iVar29;
          UpdateSaveSlots(cursor);
          iVar18 = 0x16;
          goto LAB_80033a54;
        }
      }
      else {
        if (iVar29 == 2) {
          memcard_loadattempted = 0;
          UpdateSaveSlots(cursor);
          iVar18 = 0x1d;
          goto LAB_80033a54;
        }
        if (iVar29 == 3) goto LAB_80033a6c;
      }
LAB_80033a70:
      if (bVar4) {
        NewMenu(cursor,-1,-1,-1);
        iVar28 = 0x3c;
        ResetLoadSaveCharacter();
      }
      //goto LAB_800346ac;
          memcpy(Game.name, ",CRASH   ", 9);
      break;          
    case '\x21':
          iVar28 = (s32)cursor->y_max;
          if (iVar29 == iVar28 + -1) {
            CleanLetters(Game.name);
            iVar28 = strcmp(Game.name,"        ");
            if (iVar28 == 0) {
                memcpy(Game.name, ",CRASH   ", 9);
            }
            iVar28 = 0x19;
            iVar18 = 2;
LAB_80033b40:
            NewMenu(cursor,iVar28,iVar18,-1);
          }
          else {
            if (iVar29 == iVar28) {
              iVar28 = 0x15;
              iVar18 = -1;
              goto LAB_80033b40;
            }
            if (Game.language == 'c') {
              iVar18 = 2;
              if ((iVar29 != iVar28 - 2) && (iVar18 = 1, iVar29 != iVar28 - 3)) {
                if (iVar29 != iVar28 - 4) {
                  if ((input_alphabet != 2) || ((cursor->y != '\0' && (cursor->y != '\x04'))))
                  goto LAB_80033bc0;
                  goto LAB_80033bd8;
                }
                iVar18 = 0;
              }
              input_alphabet = iVar18;
              GetMenuInfo(cursor);
            }
            else {
LAB_80033bc0:
              InputNewLetter(cursor,Game.name,&i_nameinput,8);
            }
          }
LAB_80033bd8:
          iVar28 = 0x36;
    break;               
    case '(':
            if (uVar17 != 0) {
              iVar28 = 0x29;
              goto LAB_8003469c;
            }
            if (uVar5 == 0) break; //goto LAB_800346ac;
            iVar18 = 0x27;
LAB_8003461c:
            iVar28 = 0;
            //goto LAB_80034620;
            NewMenu(cursor,iVar18,iVar28,-1);
            iVar28 = 0x3c;
    break;
    case '&':
        loadsaveCallEachFrame();
        UpdateSaveSlots(cursor);
        if (uVar17 == 0) break; //goto LAB_800346ac;
        if (saveload_cardchanged != 0) {
          saveload_cardchanged = 0;
        }
        saveload_error = 0;
        iVar28 = 0x15;
        iVar18 = -1;
    break;
    case '\x16':
        //if (cursor->menu != '\x16') goto LAB_800346ac;
        loadsaveCallEachFrame();
        if ((saveload_cardchanged != 0) || (saveload_error != 0)) {
          NewMenu(cursor,0x26,-1,-1);
        }
        UpdateSaveSlots(cursor);
        if (uVar17 == 0) {
          if (uVar5 == 0) break; //goto LAB_800346ac;
          iVar28 = -1;
          iVar18 = 0x15;
        }
        else {
          if ((s32)cursor->y < (s32)cursor->y_max) {
            iVar28 = cursor->x * 2 + (s32)cursor->y;
            game = SaveSlot + iVar28;
            if (SaveSlot[iVar28].empty == '\0') {
                iVar28 = 2;
                break;
                //goto LAB_800342c4;
            }
            //iVar28 = 0x17;
            //iVar18 = 1;
            NewMenu(cursor,0x17,1,-1);
              break;
            //goto LAB_800346a0;
          }
          //iVar28 = -1;
          //iVar18 = 0x15;
            NewMenu(cursor,0x15,-1,-1);
            break;
        }
            NewMenu(cursor,iVar18,iVar28,-1);
            iVar28 = 0x3c;
        //goto LAB_80034620;
    break;
    case '\x17':
    loadsaveCallEachFrame();
    if ((saveload_cardchanged != 0) || (saveload_error != 0)) {
      NewMenu(cursor,0x26,-1,-1);
    }
    if (cursor->item_frame != 0) {
      if (cursor->y == 1) {
        //iVar18 = 0x16;
        //iVar28 = -1;
          NewMenu(cursor,0x16,-1, -1);
          break;
      } else{
        Game = *game;
        iVar28 = 0x36;
        NewLanguage((u32)Game.language);
        NewMenu(cursor,0x18,0,-1);
        memcard_loadresult_delay = 0x28;
        plr_lives.count = (short)Game.lives;
        plr_lives.draw = plr_lives.count;
          break;
        //goto LAB_800346ac;
      }

    }
    else {
      if (uVar5 == 0) break; //goto LAB_800346ac;
      iVar18 = 0x16;
      iVar28 = -1;
    }
    //goto LAB_80034620;
    NewMenu(cursor,iVar18,iVar28,-1);
    iVar28 = 0x3c;
    break;
    case '\x18':
            loadsaveCallEachFrame();
            UpdateSaveSlots(cursor);
            if ((saveload_cardchanged != 0) || (saveload_error != 0)) {
                //iVar22 = 1;
                //iVar18 = 0x26;
                NewMenu(cursor,0x26,1,-1);
                break;
            }
            else {
              if (memcard_loadresult_delay == 0) {
                NewMenu(cursor,-1,-1,-1);
                tumble_action = -1;
                tumble_duration = 0.0f;
                tumble_time = 0.0f;
                last_level = -1;
                last_hub = -1;
                Hub = -1;
                (player->obj).hdg = 0x8000;
                if (pos_START != NULL) {
                  (player->obj).pos = *pos_START;
                }
              }
              break; //goto LAB_800346ac;
            }

    break;
    case '\x19':
              loadsaveCallEachFrame();
              if ((saveload_cardchanged != 0) || (saveload_error != 0)) {
                saveload_error = 4;
                NewMenu(cursor,0x26,-1,-1);
              }
              UpdateSaveSlots(cursor);
              if (uVar17 != 0) {
                if (((memcard_formatting != 0) || (memcard_formatme != 0)) ||
                   (memcard_formatmessage_delay != 0)) break; //goto LAB_800346ac;
                if ((s32)cursor->y < (s32)cursor->y_max) {
                  iVar28 = 0x36;
                  game = SaveSlot + cursor->x * 2 + (s32)cursor->y;
                  NewMenu(cursor,0x1b,1,-1);
                  break; //goto LAB_800346ac;
                }
                iVar28 = 1;
                iVar18 = 0x15;
              }
              else {
                if (uVar5 == 0) break; //goto LAB_800346ac;
                iVar28 = -1;
                iVar18 = 0x15;
              }
              //goto LAB_80034620;
                NewMenu(cursor,iVar18,iVar28,-1);
                iVar28 = 0x3c;  
    break;
    case '\x1a':
        loadsaveCallEachFrame();
        if (saveload_cardchanged != 0) {
          NewMenu(cursor,0x26,-1,-1);
        }
        if (uVar17 == 0) {
          if (uVar5 != 0) {
            NewMenu(cursor,0x15,-1,-1);
            iVar28 = 0x3c;
          }
        }
    break;
    case '\x1b':
              loadsaveCallEachFrame();
              if ((saveload_cardchanged != 0) || (saveload_error != 0)) {
                if ((saveload_error == 2) || (saveload_error == 0x10)) {
                  saveload_error = 4;
                }
                NewMenu(cursor,0x26,-1,-1);
              }
              if (uVar17 != 0) {
                if (cursor->y == 1) {
                //iVar28 = -1;
                //iVar18 = 0x19;
                    NewMenu(cursor,0x19,-1,-1);
                } else{
                  XbUpdateDateStamp((s32)game);
                  if (game->empty != '\0') {
                    memcard_gamesavailable = memcard_gamesavailable + 1;
                  }
                  *game = Game;
                  //iVar28 = 0x36;
                  memcard_saveneeded = 1;
                  NewMenu(cursor,0x1c,0,-1);
                  break; //goto LAB_800346ac;
                }
              }
              else {
                if (uVar5 == 0) break; //goto LAB_800346ac;
                //iVar28 = -1;
                //iVar18 = 0x19;
                  NewMenu(cursor,0x19,-1,-1);
              }
              //goto LAB_80034620;
               //NewMenu(cursor,iVar18,iVar28,-1);
               iVar28 = 0x3c;
                //goto LAB_80034684;
    break;
    case 0x1c:
    case 0x1f:
LAB_800340f0:
            loadsaveCallEachFrame();
            UpdateSaveSlots(cursor);
            if ((((memcard_saveneeded != 0) || (memcard_savestarted != 0)) ||
                (memcard_deleteneeded != 0)) ||
               ((memcard_deletestarted != 0 || (memcard_savemessage_delay != 0)))) {
              memcard_saveresult_delay = 0x3c;
            }
            if ((saveload_cardchanged == 0) && (saveload_error == 0)) {
              if (memcard_saveresult_delay != 0) break; //goto LAB_800346ac;
              iVar18 = 0x15;
              iVar22 = 3;
                //NewMenu(cursor,0x15,3,-1);
            }
            else {
              if (((saveload_error == 2) || (saveload_error == 0x10)) &&
                 (saveload_error = 8, cursor->menu == '\x1c')) {
                saveload_error = 4;
              }
              iVar18 = 0x26;
              iVar22 = -1;
                //NewMenu(cursor,0x26,-1,-1);
            }
          goto LAB_80034684;
    break;
    case 0x31:
                iVar18 = SaveMenu;
                loadsaveCallEachFrame();
                UpdateSaveSlots(cursor);
                if (memcard_formatmessage_delay != 0) break; //goto LAB_800346ac;
                iVar22 = 3;
                iVar18 = SaveMenu;
                goto LAB_80034684;  
    break;
    case '?':
          loadsaveCallEachFrame();
          UpdateSaveSlots(cursor);
          if (memcard_formatme != 0) {
            memcard_formatmessage_delay = 0x28;
            break; //goto LAB_800346ac;
          }
          if (memcard_formatmessage_delay != 0) break; //goto LAB_800346ac;
          iVar22 = 3;
          iVar18 = SaveMenu;
          goto LAB_80034684;
    break;
    case 0x1e:
        loadsaveCallEachFrame();
        if ((saveload_cardchanged != 0) || (saveload_error != 0)) {
          if ((saveload_error == 2) || (saveload_error == 0x10)) {
            saveload_error = 8;
          }
          NewMenu(cursor,0x26,-1,-1);
        }
        if (uVar17 != 0) {
              if (cursor->y == 1) {
                  //iVar28 = -1;
                 // iVar18 = 0x1d;
                  NewMenu(cursor,0x1d,-1,-1);
              } else{
                bVar1 = 1;
                game->empty = 1;
                for (iVar28 = 0; iVar28 < 3; iVar28++) {
                  if (SaveSlot[iVar28].empty == '\0') {
                    bVar1 = 0;
                  }
                }
                memcard_gamesavailable = memcard_gamesavailable + -1;
                if (bVar1) {
                  memcard_deleteneeded = 1;
                }
                else {
                  memcard_saveneeded = 1;
                }
                iVar28 = 0x1f;
                goto LAB_8003469c;
              }
        }
            else {
          if (uVar5 == 0) break; //goto LAB_800346ac;
          iVar28 = -1;
          iVar18 = 0x1d; 
            }
    //break;
    //case 0x2e:
        if (uVar17 == 0) {
        if (uVar5 == 0) break; //goto LAB_800346ac;
        iVar28 = -1;
        }
    break;
    case 0x30: //FIX
                if (iVar29 != 1) {
                  iVar28 = 0x36;
                  memcard_formatmessage_delay = 0x28;
                  MemCardDelete();
                  memtest_done = 0;
                  iVar18 = 0x31;
                  goto LAB_80034680;
                }
                iVar28 = -1;
        
    break;
    case 0x20:
        if (uVar17 == 0) {
          if (uVar5 == 0) break; //goto LAB_800346ac;
          iVar28 = -1;
        }
        else {
          if (iVar29 != 1) {
            memcard_formatme = 1;
            iVar28 = 0x21;
            goto LAB_8003469c;
          }
          iVar28 = -1;
        }
    break;
    
    default:
LAB_800346a0:
      NewMenu(cursor,iVar28,iVar18,-1);
LAB_800346a8:
      iVar28 = 0x36;
      //goto LAB_800346ac;
    break;
    case '\"':
            if ((cursor->menu != '\"') || (uVar16 == 0)) break; //goto LAB_800346ac;
            if ((Level == 0x16) || (Level == 0x18)) {
              boss_dead = 2;
            }
            iVar28 = -1;
            iVar18 = -1;
            goto LAB_800346a0;
    break;   
    case '$':
          if (uVar16 != 0) {
            iVar28 = 0x36;
            NuSoundStopStream(4);
            NewMenu(cursor,-1,-1,-1);
          }
          else {
            if (((player->obj).mask != NULL) && ((((((player->obj).mask->anim).flags & 1) != 0 ||
                 ((((player->obj).mask->sfx != -1 && ((player->obj).mask->hold == '\0')) && (NuSoundKeyStatus(4) != 1)))) ||
                ((0x707 < cursor->item_frame || (2 < ((player->obj).mask)->active)))))) {
              NewMenu(cursor,-1,-1,-1);
            }
          }
          if (cursor->menu != '$') {
            advice_wait = 0x1e;
          }
          //goto LAB_800346ac;
    break; 
    case '\'':
        if (uVar17 != 0) {
          if (iVar29 != 0) {
          if (iVar29 != 1) break; //goto LAB_800346ac;
          iVar28 = 0;
          iVar18 = -1;
          }
            iVar18 = 0x28;
            goto LAB_8003461c;
        }
        else {
          if (uVar5 == 0) break; //goto LAB_800346ac;
          iVar28 = 0;
          iVar18 = -1;
        }
    break;       
    case '*':
        iVar18 = GBA_Download();
        if (iVar18 == 0) break; //goto LAB_800346ac;
        iVar18 = 0x2b;
LAB_80034680:
        iVar22 = 0;
        goto LAB_80034684;
    break;    
    case ')':
          GBA_DownloadStart();
          iVar18 = 0x2a;
          goto LAB_80034680;
    break;
    case 0x2b:
            iVar18 = GBA_DownloadEnd();
            if (iVar18 == -1) {
              iVar18 = 0x2d;
            }
            else {
              if (iVar18 != 0) break; //goto LAB_800346ac;
              iVar18 = 0x2c;
            }
        iVar22 = 0;     //goto LAB_80034680;
LAB_80034684:
      NewMenu(cursor,iVar18,iVar22,-1);
      //goto LAB_800346ac;
    break;
    case 0x2d:
    case 0x2c:
        if (uVar17 == 0) break; //goto LAB_800346ac;
        iVar28 = 0x27;
        //goto LAB_8003469c;
        iVar18 = 0;
        NewMenu(cursor,iVar28,iVar18,-1);
        iVar28 = 0x36;   
    break;
}
/*  if (cursor->menu == '\x17') {

  }
  if (cursor->menu < '\x18') {
    if (cursor->menu == '\v') {

    }
    if (cursor->menu < '\f') {
      if (cursor->menu == '\x05') {

      }
      if ('\x05' < cursor->menu) {
        if (cursor->menu == '\b') goto LAB_800346ac;
        if ('\b' < cursor->menu) {
          if (cursor->menu == '\t') {

          }
          else {
            if (cursor->menu != '\n') goto LAB_800346ac;

          }
          goto LAB_80034620;
        }
        if (cursor->menu == '\x06') {

        }
        else {
          if (cursor->menu != '\a') goto LAB_800346ac;

        }
        goto LAB_80034684;
      }
      if (cursor->menu == '\x02') {

      }
      if ('\x02' < cursor->menu) {
        if (cursor->menu == '\x03') {

        }
        if (cursor->menu != '\x04') goto LAB_800346ac;

      }
      if (cursor->menu != '\0') {
        if (cursor->menu != 1) goto LAB_800346ac;

      }

    }
    if (cursor->menu == '\x11') {

    }
    if (cursor->menu < '\x12') {
      if (cursor->menu == '\x0e') {

      }
      if ('\x0e' < cursor->menu) {
        if (cursor->menu != '\x0f') {
          if (cursor->menu != '\x10') goto LAB_800346ac;

        }

      }
      if (cursor->menu != '\f') {

      }

    }
    if (cursor->menu == '\x14') {

    }
    if ('\x14' < cursor->menu) {
      if (cursor->menu != '\x15') {

      }

    }
    if (cursor->menu == '\x12') goto LAB_80033850;

  }
  else {
    if (cursor->menu == '#') {

    }
    
    if ('#' < cursor->menu) {
      if (cursor->menu == '*') {

      }
      if ('*' < cursor->menu) {
        if (cursor->menu != '-') {
          if ('-' < cursor->menu) {
            if (cursor->menu == '0') {

              }
              else {

              }
            }
            else {
              if ('0' < cursor->menu) {
                if (cursor->menu != '1') goto LAB_800346ac;

              }
              if (cursor->menu != '/') goto LAB_800346ac;

            }
            goto LAB_80034620;
          }
          if (cursor->menu == '+') {

          }
          if (cursor->menu != ',') goto LAB_800346ac;
        }

      }
      if (cursor->menu == '\'') {

      }
      else {
       if ('\'' < cursor->menu) {
          if (cursor->menu == '(') {

          }
          if (cursor->menu != ')') goto LAB_800346ac;

        }
       if (cursor->menu == '$') {

        }
        if (cursor->menu != '&') goto LAB_800346ac;

      }

    }
    if (cursor->menu == '\x1d') {

    }
    else {
      if (cursor->menu < '\x1e') {
        if (cursor->menu != '\x1a') {
          if (cursor->menu < '\x1b') {
            if (cursor->menu != '\x18') {
              if (cursor->menu != '\x19') goto LAB_800346ac;

            }

          }
          else {
            if (cursor->menu == '\x1b') {

            }
            if (cursor->menu != '\x1c') goto LAB_800346ac;

          }
        }

        }
        else {

        }
        if (cursor->menu == '\x15') {

        }
        goto LAB_800346ac;
      }
      if (cursor->menu == ' ') {

      }
      else {
        if (' ' < cursor->menu) {
          if (cursor->menu != '!') {
          }

        }
        if (cursor->menu != '\x1e') {
          if (cursor->menu != '\x1f') goto LAB_800346ac;
          goto LAB_800340f0;
        }

      }*/
//LAB_80034620:

    //iVar28 = new_level;
  //new_level = iVar28;
//LAB_800346ac:
  if (iVar28 != -1) {
    GameSfx(iVar28,NULL);
  }
  return;
}

//PS2 match
s32 StartHGobjAnim(struct nuhspecial_s *obj)
{
  
  if (((obj->special != NULL) && (obj->special->instance != NULL)) &&
     ((obj->special->instance->anim) != NULL)) 
  {
    (obj->special->instance->anim)->playing = (obj->special->instance->anim)->playing | 1;
    return 1;
  }
  return 0;
}

//PS2 Match
s32 ResetHGobjAnim(struct nuhspecial_s *obj)
{ 
  if (((obj->special != NULL && (obj->special->instance != NULL)) &&
     ((obj->special->instance->anim) != NULL))) {
    (obj->special->instance->anim)->playing = (obj->special->instance->anim)->playing & 0xfffffffe;
    obj->special->instance->anim->ltime = 1.0;
    return 1;
  }
  return 0;
}

//NGC MATCH
s32 FinishHGobjAnim(struct nuhspecial_s *obj) { 
  if (((obj->special != NULL) && (obj->special->instance != NULL)) &&
     (obj->special->instance->anim != NULL)) {
    (obj->special->instance->anim)->playing = (obj->special->instance->anim)->playing & 2;
    obj->special->instance->anim->ltime = world_scene[0]->instanimdata[obj->special->instance->anim->anim_ix]->time;
    return 1;
  }
  return 0;
}


//vehsupp.c
void GetLocatorMtx(struct CharacterModel *Model,struct numtx_s *LocMtx,f32 YAngOff3)
{
  /*
struct NUPOINTOFINTEREST_s *ptr;
  s32 i;
  struct NUPOINTOFINTEREST_s **pLoc;
  
  if (Model != NULL) {
    i = 0;
    pLoc = Model->pLOCATOR;
    NuMtxSetRotationY(&BaseMat,(s32)(YAngOff3 * 182.0444f));
    NuHGobjEval(Model->hobj,0,0,tmtx);
    do {
      ptr = *pLoc;
      pLoc = pLoc + 1;
      if (ptr != NULL) {
        NuHGobjPOIMtx(Model->hobj,(u8)i,&BaseMat,tmtx,LocMtx);
      }
      i = i + 1;
      LocMtx = LocMtx + 1;
    } while (i < 0x10);
  }
  return;
*/
}

//NGC MATCH
void DefaultGame(void) {
  Game.vibration = 0x1;
  Game.sfx_volume = 0x64;
  Game.music_volume = 'F';
  Game.screen_x = '\0';
  Game.screen_y = '\0';
  Game.language = '\0';
  return;
}

//NGC MATCH
void NewGame(void) {
  s32 save[7];
  
  save[0] = Game.vibration;
  save[2] = Game.sfx_volume;
  save[3] = Game.music_volume;
  save[4] = Game.screen_x;
  save[5] = Game.screen_y;
  save[6] = Game.language;
  memset(&Game,0,0x414);
  Game.vibration = save[0];
  Game.sfx_volume = save[2];
  Game.music_volume = save[3];
  Game.screen_x = save[4];
  Game.screen_y = save[5];
  Game.language = save[6];
  NewLanguage(save[6]);
  memcpy(Game.name, "MIDGET  ", 9);
  DefaultTimeTrialNames(1);
  Game.lives = '\x04';
  Game.hub[0].flags = '\x01';
  Mask.active = (u16)Game.mask;
  Hub = -1;
  return;
}

//NGC MATCH
void StartTransporter(struct obj_s *cyl,struct nuvec_s *pos) {
  cyl->oldpos = *pos;
  cyl->pos = cyl->oldpos;
  cyl->mom = v000;
  cyl->RADIUS = 0.5f;
  cyl->old_SCALE = 1.0f;
  cyl->SCALE = 1.0f;
  cyl->radius = 0.5f;
   cyl->scale = 1.0f;
   cyl->top = 0.0f;
  cyl->bot = -1.0f;
  cyl->hdg = 0;
  return;
}

//NGC MATCH
void FinishTransporter(struct obj_s *cyl,struct obj_s *obj) {
  cyl->oldpos = cyl->pos;
  cyl->mom = v000;
  cyl->dyrot = 0;
  (obj->pos).y = (obj->oldpos).y = (cyl->top * cyl->SCALE + (cyl->pos).y) - obj->bot * obj->SCALE;
  return;
}

//NGC MATCH
void TransporterGo(void) {
  GameSfx(0x82,&(player->obj).pos);
  NewBuzz(&player->rumble,6);
  return;
}

//NGC MATCH
void DrawTransporters(void) {
  s32 i;

    i = 4;
  if ((TimeTrial == 0) && (ChaseActive() == -1)) {
        if ((ObjTab[i].obj.special != NULL) && ((Rail[5].type != -1 && (VEHICLECONTROL != 1)))) {
          Draw3DObject(4,&bonus_obj[0].pos,0,bonus_obj[0].hdg,0,1.0f,1.0f,1.0f,ObjTab[i].obj.scene,ObjTab[i].obj.special,0);
          Draw3DObject(4,&bonus_obj[1].pos,0,bonus_obj[1].hdg,0,1.0f,1.0f,1.0f,ObjTab[i].obj.scene,ObjTab[i].obj.special,0);
        }
        if ((Level != 0x25) || ((Game.hub[5].flags & 1) != 0)) {
              i = 5;
              if (LostLife != 0) {
                i = 6;
              }
              if (((ObjTab[i].obj.special != NULL) && (Rail[6].type != -1)) && ((VEHICLECONTROL != 1 || ((player->obj).vehicle == 0x44)))) {
                Draw3DObject(i,&death_obj.pos,0,death_obj.hdg,0,1.0f,1.0f,1.0f,ObjTab[i].obj.scene,ObjTab[i].obj.special,0);
              }
              i = 8;
              if (gempath_open != 0) {
                i = 7;
              }
              if (((ObjTab[i].obj.special != NULL) && (Rail[7].type != -1)) && ((VEHICLECONTROL != 1 || ((player->obj).vehicle == 0x44)))) {
                Draw3DObject(i,&gempath_obj.pos,0,gempath_obj.hdg,0,1.0f,1.0f,1.0f,ObjTab[i].obj.scene,ObjTab[i].obj.special,0);
              }
        }
  }
  return;
}

//NGC MATCH
void ResetAkuAkuAdvice(void) {
    s32 i;

  for (i = 0; i < 0xd; i++) {
    akuaku_advice_played[i] = '\0';
  }
  return;
}

void ResetItems(void) {
  plr_items = 0;
  plr_bonusgem.count = 0;
  plr_crystal.count = 0;
  plr_crategem.draw = 0;
  plr_crategem.count = 0;
  plr_bonusgem.draw = 0;
  plr_crystal.draw = 0;
  return;
}

//NGC MATCH
void ResetGemPath(void) {
  u8 bits;
  s32 open;
  
  gempath_begun = 0;
  GemPath = 0;
    switch (Level) {
    case 0xc:
        bits = 1;
        break;
    case 0x21:
        bits = 2;
        break;
    case 0xe:
        bits = 4;
        break;
    case 4:
        bits = 8;
        break;
    case 0x14:
        bits = 0x10;
        break;
    default:
        bits = 0;
        break;
    }
    if ((Game.gembits & bits) == 0) {
        open = 0;
    } else{
        open = 1;    
    }
    gempath_open = open;
  if (ObjTab[101].obj.special != NULL) {
     (ObjTab[101].obj.special)->instance->flags.visible = ((1 - gempath_open));
  }
  return;
}

//NGC MATCH
void ResetDeath(void) {
  struct rail_s* rail;

  death_begun = 0;
  Death = 0;
  rail = &Rail[6];

  if ((((rail->type != -1) && (Level == 0x25)) && (Hub == 5)) &&
     ((last_level != 0x26 && (last_level != -1)))) {
    StartTransporter(&death_obj,(struct nuvec_s *)(&(rail->pINPLAT)->pts[((rail->pINPLAT)->len + -1) * (s32)(rail->pINPLAT)->ptsize]));
    Death = 2;
    death_begun = 1;
  }
  return;
}

void ResetBonus(void) {
    Bonus = 0;
}

//NGC MATCH
void ResetGame(void) {
  s32 i;
  
  memset(&Cursor,0,0x80);
  for(i = 0; i < 0x2c; i++) {
    if (0x3de < LData[i].farclip - 10U) {
      LData[i].farclip = 1000;
    }
  }
  for(i = 0; i < 0x23; i++) {
    LData[i].hub = HubFromLevel(i);
  }

  for(i = 0; i < 1; i++) {
    Mask[i].character = 3;
  }
  ResetTimer(&GlobalTimer);
  InitProbe();
  return;
}

void ResetKabooms(void) {
  memset(Kaboom,0,0xa20);
  i_kaboom = 0;
  return;
}

//NGC MATCH
void DrawNextVehicle(s32 render) {
  struct vtog_s *tog;
  struct CharacterModel* model;
  struct nuvec_s pos;
  s32 i;
  s32 vehicle;
  struct anim_s anim;
  float y;
  
  vehicle = (s32)(player->obj).vehicle;
  if (vehicle != -1) {
      i = CRemap[vehicle];
        if(i != -1) {
                model = &CModel[i];
                if ((Level == 0x16) && ((VEHICLECONTROL == 0 || (vtog_time < vtog_duration)))) {
                  pos = AshesMechInPos;
                  y = NewShadow(&pos,0.0f);
                  if (y != 2000000.0f) {
                    pos.y = y;
                    FindAnglesZX(&ShadNorm);
                  }
                  else {
                    temp_xrot = temp_zrot = 0;
                  }
                  NuMtxSetRotationY(&mTEMP,vtog_angle + 0x8000);
                  NuMtxRotateZ(&mTEMP,temp_zrot);
                  NuMtxRotateX(&mTEMP,temp_xrot);
                  NuMtxTranslate(&mTEMP,&pos);
                  ResetAnimPacket(&anim,0x62);
                  DrawCharacterModel(model,&anim,&mTEMP,NULL,render,NULL,NULL,NULL,NULL);
                }
                tog = VTog;
                for(i = 0; i < 6; i++) {
                  if (((Level != 0xf) || (i < 2)) &&
                     (((vtog_time < vtog_duration && ((vtog_blend != 0 && (tog == pVTog)))) ||
                      ((best_cRPos != NULL && (((VEHICLECONTROL != 1 && ((tog)->pTRIGGER != NULL)) &&
                        (FurtherALONG((s32)(tog)->iRAIL,(s32)(tog)->iALONG,(tog)->fALONG,(s32)best_cRPos->iRAIL,
                                              (s32)best_cRPos->iALONG,best_cRPos->fALONG) != 0)))))))) {
                    if ((tog)->type != '\0') {
                      return;
                    }
                    NuMtxSetIdentity(&mTEMP);
                    NuMtxRotateY(&mTEMP,(tog)->yrot + 0x8000);
                    NuMtxRotateZ(&mTEMP,(uint)(tog)->zrot);
                    NuMtxRotateX(&mTEMP,(uint)(tog)->xrot);
                    pos.x = ((tog)->pos).x;
                    pos.y = ((tog)->pos).y - CData[vehicle].min.y * CData[vehicle].scale;
                    pos.z = ((tog)->pos).z;
                    NuMtxTranslate(&mTEMP,&(tog)->pos);
                    ResetAnimPacket(&(tog)->anim,vtog_action);
                    DrawCharacterModel(model,&(tog)->anim,&mTEMP,NULL,render,NULL,NULL,NULL,NULL);
                    return;
                  }
                  tog++;
                }
        }
  }
  return;
}

//NGC MATCH
void AddKaboom(s32 type,struct nuvec_s *pos,float radius) {
  struct kaboom_s* kaboom;
  float speed;
  s32 sfx;

  sfx = -1;
  kaboom = &Kaboom[i_kaboom];
  kaboom->time = 0.0f;
  kaboom->type = (u16)type;
  kaboom->pos = *pos;
    switch (type) {
    case 1:
    case 2:
        kaboom->radius0 = 0.0f;
        kaboom->radius1 = 1.25f;
        speed = 3.0f;
        break;
    case 0x4:
        kaboom->radius0 = 0.0f;
        kaboom->radius1 = 1.25f;
        speed = 3.0f;
        JudderGameCamera(GameCam,0.2,NULL);
        break;
    case 0x8:
        kaboom->radius0 = 0.0f;
        kaboom->radius1 = 1.75f;
        speed = 4.1999998f;
        JudderGameCamera(GameCam,0.4,NULL);
        break;
    case 0x10:
        kaboom->radius0 = 0.0f;
        kaboom->radius1 = 3.0f;
        speed = 7.5f;
        JudderGameCamera(GameCam,0.3f,&kaboom->pos);
        break;
    case 0x80:
        sfx = 0x3b;
    case 0x100:
        kaboom->radius0 = 0.0f;
        kaboom->radius1 = 1.0f;
        speed = 3.0f;
        JudderGameCamera(GameCam,0.3f,&kaboom->pos);
        break;
    case 0x40:
    case 0x20:
        kaboom->radius0 = 0.0f;
        kaboom->radius1 = 50.0f;
        speed = 15.0f;
        kaboom->i = temp_pCrate->index;
        kaboom->yrot = temp_pGroup->angle;
        break;
    }
  if (radius > 0.0f) {
    kaboom->radius1 = radius;
  }
  i_kaboom++;
  kaboom->duration = (kaboom->radius1 - kaboom->radius0) / speed;
  if (i_kaboom == 0x48) {
    i_kaboom = 0;
  }
  if (sfx != -1) {
    GameSfx(sfx,&kaboom->pos);
  }
  return;
}


//NGC MATCH (check if)
void UpdateKabooms(void) {
  struct wumpa_s *wmp;
  struct cratesarray_s *crate;
  struct obj_s *obj;
  struct crategrp_s *group;
  struct kaboom_s *kaboom;
  float r;
  float r2;
  s32 type;
  s32 i;
  s32 j;
  s32 k;
  s32 key;
  struct nuvec_s local_88;
  struct nuvec_s local_78;
  
  kaboom = Kaboom;
  for (i = 0; i < 0x48; i++) {
    if ((kaboom->time < kaboom->duration) && ((kaboom->type & 0x40) == 0)) {
      group = CrateGroup;
      r = ((kaboom->radius1 - kaboom->radius0) * (kaboom->time / kaboom->duration) + kaboom->radius0);
      r2 = (r * r);
        for (j = 0; j < CRATEGROUPCOUNT; j++) {
          if (((!(kaboom->pos.x < (group->minclip.x - r)) && !(kaboom->pos.x > (group->maxclip.x + r))) &&
               !(kaboom->pos.z < (group->minclip.z - r)) && !(kaboom->pos.z > group->maxclip.z + r) &&
               !(kaboom->pos.y < (group->minclip.y - r))) && !(kaboom->pos.y > (group->maxclip.y + r))) {
            crate = &Crate[group->iCrate];
              for (k = 0; k < group->nCrates; k++) {
                if (crate->on != '\0') {
                  type = GetCrateType(crate,0);
                  if ((type == 0x13) && ((kaboom->type & 0xc) == 0)) {
                    NewCrateAnimation(crate,0x13,0x58,0);
                    GameSfx(0x39,&crate->pos);
                  }
                      //check
                  else if (kaboom->type != 0x20 || type != 0 || crate->type3 == -1 || kaboom->i == -1 || crate->trigger != kaboom->i) {
                   if (kaboom->type != 0x20 && type != 0) {
                      local_88.x = (crate->pos).x;
                      local_88.y = ((crate->pos).y + 0.25f);
                      local_88.z = (crate->pos).z;
                      NuVecSub(&local_78,&kaboom->pos,&local_88);
                      if ((local_78.x * local_78.x + local_78.y * local_78.y + local_78.z * local_78.z) < r2) {
                        if (kaboom->type == 0x20) {
                          crate->newtype = crate->type3;
                          if (crate->type3 == '\x03') {
                            NewCrateAnimation(crate,3,0x22,1);
                          }
                          if (crate->newtype != '\0') {
                            crate->appeared = '\x01';
                          }
                        }
                        else if (type == 0xe) {
                          if ((crate->action == -1) &&
                             (NewCrateAnimation(crate,0xe,0x35,0) == 0)) {
                            StartExclamationCrateSequence(group,crate);
                          }
                        }
                        else if (type == 0x11) {
                          if ((crate->action == -1) &&
                             (NewCrateAnimation(crate,0x11,0x35,0) == 0)) {
                            DestroyAllNitroCrates(group,crate);
                          }
                        }
                        else {
                          if (CrateOff(group,crate,(uint)kaboom->type,0) != 0) {
                            HopCratesAbove(group,crate,CRATEHOPSPEED);
                          }
                        }
                      }
                    }
                  }
                }
                crate++;
              }
          }
          group++;
        }
      if (kaboom->type != 0x20) {
        r = 0.5f;
        for (j = 0; j < 0x40; j++) {
          obj = pObj[j];
          if ((((((obj != NULL) && (obj->dead == '\0')) && (obj->invisible == '\0')) &&
               ((obj->character != 0xb1 && (((obj->flags & 1) == 0 || ((kaboom->type & 0x18c) == 0)))))) &&
              (((obj->flags & 4) == 0 ||  ((((kaboom->type & 0x10) == 0 &&
                 ((kaboom->type != 4 || ((obj->vulnerable & 4) != 0)))) &&
                ((kaboom->type != 8 || ((obj->vulnerable & 8) != 0)))))))) &&
             ((((obj->flags & 0x10) == 0 || ((kaboom->type & 0xc) != 0)) && ((obj->flags & 8) == 0)))) {
            local_88.x = (obj->pos).x;
            local_88.y = (((obj->bot + obj->top) * obj->SCALE) * r + (obj->pos).y);
            local_88.z = (obj->pos).z;
            NuVecSub(&local_78,&local_88,&kaboom->pos);
            if ((local_78.x * local_78.x + local_78.y * local_78.y + local_78.z * local_78.z) < r2) {
              if ((obj->flags & 1) != 0) {
                KillPlayer(obj,6);
              }
              else {
                if ((obj->flags & 0x10) != 0) {
                  PickupItem(obj);
                }
                else {
                  if ((obj->flags & 0x40004) == 0x40004) {
                    obj->kill_contact = '\x01';
                  }
                  else {
                    FlyGameObject(obj,NuAtan2D(local_78.x,local_78.z) & 0xffff);
                    KillGameObject(obj,1);
                  }
                }
              }
            }
          }
        }
        if ((kaboom->type != 0x20) && (TimeTrial == 0)) {
          wmp = Wumpa;
          for (j = 0; j < 0x140; j++) {
            if (((wmp->active == '\x02') || ((wmp->active == '\x01' && ((kaboom->type & 0xc) == 0)))) &&
               (NuVecSub(&local_78,&kaboom->pos,&wmp->pos),
               (local_78.x * local_78.x + local_78.y * local_78.y + local_78.z * local_78.z) < r2)) {
              if ((kaboom->type & 0xc) != 0) {
                if ((player->obj).dead == '\0') {
                  AddScreenWumpa((wmp->pos).x,(wmp->pos).y,(wmp->pos).z,1);
                }
                wmp->active = '\0';
                key = -1;
                AddFiniteShotDebrisEffect(&key,GDeb[130].i,&wmp->pos,1);
              }
              else {
                FlyWumpa(wmp);
              }
            }
            wmp++;
          }
        }
      }
      kaboom->time += 0.01666667f;
      if (kaboom->time > kaboom->duration) {
        kaboom->time = kaboom->duration;
      }
    }
    kaboom++;
  }
}

//NGC MATCH
void DrawKabooms(void) {
    struct kaboom_s* kaboom;
    u16 yrot;
    s32 i;
    s32 j;
    s32 k;
    struct nuvec_s s;
    float f;

    kaboom = Kaboom;
    for (i = 0; i <= 0x47; i++, kaboom++) {
        if (kaboom->time < kaboom->duration) {
            if ((kaboom->type & 0x181) != 0) {
                k = 0xb;
            } else if ((kaboom->type & 2) != 0) {
                k = 0xc;
            } else if ((kaboom->type & 0x1c) != 0) {
                k = 0xd;
            } else if ((kaboom->type & 0x60) != 0) {
                k = 0xf;
            } else {
                continue;
            }
            if (ObjTab[k].obj.special != NULL) {
                if (k == 0xf) {
                    s.x = s.y = s.z = (kaboom->duration - kaboom->time) / kaboom->duration;
                    yrot = kaboom->yrot;
                    f = ((kaboom->radius1 - kaboom->radius0) * (kaboom->time / kaboom->duration) + kaboom->radius0);
                    for (j = 0; j < 4; j++) {
                        NuMtxSetScale(&mTEMP, &s);
                        NuMtxRotateY(&mTEMP, yrot);
                        mTEMP._30 = NuTrigTable[yrot] * f + (kaboom->pos).x;
                        mTEMP._31 = (kaboom->pos).y;
                        mTEMP._32 = NuTrigTable[(yrot + 0x4000) & 0xffff] * f + (kaboom->pos).z;
                        NuRndrGScnObj((ObjTab[k].obj.scene)->gobjs[(ObjTab[k].obj.special)->instance->objid], &mTEMP);
                        yrot += 0x4000;
                    }
                } else {
                    s.x = s.y = s.z =
                        (kaboom->radius1 - kaboom->radius0) * (kaboom->time / kaboom->duration) + kaboom->radius0;
                    NuMtxSetScale(&mTEMP, &s);
                    NuMtxTranslate(&mTEMP, &kaboom->pos);
                    NuRndrGScnObj((ObjTab[k].obj.scene)->gobjs[(ObjTab[k].obj.special)->instance->objid], &mTEMP);
                }
            }
        }
    }
}


//NGC MATCH
void CheckFinish(struct obj_s* obj) {
    u16 old_hub_flags;
    u16 old_lev_flags;
    struct nuvec_s objmid;
    struct nuvec_s v;
    float d;
    s32 iVar1;

    if (Level == 0x25) {
        return;
    }
    if (obj->dead) {
        return;
    }
    if (Hub == -1) {
        return;
    }
    objmid.x = (obj->pos).x;
    objmid.y = obj->pos.y + (obj->bot + obj->top) * obj->SCALE * 0.5f;
    objmid.z = (obj->pos).z;
    if (obj->finished == 0) {
        new_hub_flags = 0;
        new_lev_flags = 0;
        finish_type = 0;

        old_hub_flags = Game.hub[Hub].flags;
        old_lev_flags = Game.level[Level].flags;
        switch (Level) {
            default:
                if (Level == 0x24) {
                    iVar1 = GetCurrentLevelObjectives();
                    if (iVar1 != 0) {
                        return;
                    }
                    VEHICLECONTROL = iVar1;
                    ResetPlayer(1);
                    level_part_2 = 0;
                    return;
                }
                objmid.y += 0.5f;
                d = FindNearestCreature(&objmid, 0xb1, &v);
                if (d > FINISHRADIUSLEVEL * FINISHRADIUSLEVEL) {
                    in_finish_range = 0;
                    return;
                }
                in_finish_pos = v;
                finish_newpos = v;
                in_finish_range = 0x3c;
                finish_oldpos = (obj->pos);
                finish_type = 1;
                finish_frames = 0x3c;
                break;
            case 0xd:
            case 0x12:
            case 0x1a:
                if (((TimeTrial == 0) && ((Game.level[Level].flags & 0x10) == 0))
                    && (((plr_items & 2) == 0 && (plr_crates.count >= DESTRUCTIBLECRATECOUNT)))) {
                    PickupCrateGem();
                }
                if (FlyingLevelCompleteTimer != 0.0f) {
                    return;
                }
                finish_newpos = (obj->pos);
                if ((Game.level[Level].flags & 8) == 0) {
                    PickupCrystal();
                }
                finish_frames = 0x3c;
                break;
            case 0x15:
            case 0x16:
            case 0x17:
            case 0x18:
            case 0x19:
                if ((((((Level != 0x18) && (Level != 0x16)) && (Level != 0x15)) || (ChrisBigBossDead != 0))
                     && ((Level != 0x19 && (Level != 0x17)))) && (GetCurrentLevelObjectives() == 0)) {
                    if ((Game.hub[Hub].flags & 4) == 0) {
                        if ((boss_dead < 1) && (boss_dead = 1, Level == 0x18)) {
                            PickupPower(0xa2);
                        }
                    } else {
                        boss_dead = 2;
                    }
                }
                if (boss_dead < 2) {
                    return;
                }
                if (Cursor.menu != -1) {
                    return;
                }
                in_finish_range++;
                if (in_finish_range < 0x3c) {
                    return;
                }
                finish_newpos = (obj->pos);
                finish_frames = 0x3c;
                if ((Game.hub[Hub].flags & 6) == 2) {
                    Game.hub[Hub].flags = Game.hub[Hub].flags | 4;
                    Game.hub[Hub + 1].flags = Game.hub[Hub + 1].flags | 1;
                }
                break;
            case 0x24:
                if (TimeTrial != 0) {
                    return;
                }
                if (Game.level[Level].flags & 0x10) {
                    return;
                }
                if ((plr_items & 2) != 0) {
                    return;
                }
                if (plr_crates.count < DESTRUCTIBLECRATECOUNT) {
                    return;
                }
                PickupCrateGem();
                return;
                break;
            case 3:
                if ((((TimeTrial == 0) && ((Game.level[Level].flags & 0x10) == 0)) && ((plr_items & 2) == 0))
                    && (plr_crates.count >= DESTRUCTIBLECRATECOUNT)) {
                    PickupCrateGem();
                }
                if (SmokeyFinished == 0) {
                    return;
                }
                if (in_finish_range == 0) {
                    GameSfx(0x69, &(player->obj).pos);
                }
                if (((TimeTrial == 0) && (SmokeyPosition == 1))
                    && (((Game.level[Level].flags & 8) == 0 && ((plr_items & 1) == 0)))) {
                    PickupCrystal();
                }
                in_finish_range++;
                if (in_finish_range < 0x3c) {
                    return;
                }
                finish_newpos = (obj->pos);
                finish_frames = 0x3c;
                break;
            case 0x1c:
                if ((((TimeTrial == 0) && ((Game.level[Level].flags & 0x10) == 0)) && ((plr_items & 2) == 0))
                    && (plr_crates.count >= DESTRUCTIBLECRATECOUNT)) {
                    PickupCrateGem();
                }
                if (PlrTub.laps < MAXLAPS) {
                    return;
                }
                in_finish_range++;
                if (in_finish_range < 0x3c) {
                    return;
                }
                finish_newpos = (obj->pos);
                if (TimeTrial == 0) {
                    if ((PlrTub.place == '\x01') && ((Game.level[Level].flags & 0x20) == 0)) {
                        PickupBonusGem(4);
                    }
                }
                finish_frames = 0x3c;
                break;
            case 0x1d:
                if (((TimeTrial == 0) && ((Game.level[Level].flags & 0x10) == 0))
                    && (((plr_items & 2) == 0 && (plr_crates.count >= DESTRUCTIBLECRATECOUNT)))) {
                    PickupCrateGem();
                }
                if (i_ring < RINGCOUNT) {
                    return;
                }
                in_finish_range++;
                if (in_finish_range < 0x3c) {
                    return;
                }
                finish_newpos = (obj->pos);
                if (TimeTrial == 0) {
                    if ((carpet_place == 2) && ((Game.level[Level].flags & 0x20) == 0)) {
                        PickupBonusGem(4);
                    }
                }
                finish_frames = 0x3c;
                break;
        }
        if ((((Level == 3) && (1 < SmokeyPosition)) || ((Level == 0x1d && (carpet_place == 1))))
            || ((Level == 0x1c && ('\x01' < PlrTub.place)))) {
            obj->finished = '\x02';
        } else {
            obj->finished = '\x01';
        }
        AddWarpDebris(obj, NULL);
        gamesfx_effect_volume = 0x7ffe;
        GameSfx(0x1e, &obj->pos);
        if ((plr_items & 1) != 0) {
            Game.level[Level].flags = Game.level[Level].flags | 8;
            if ((Game.hub[Hub].crystals < 5) && (Game.hub[Hub].crystals++, Game.hub[Hub].crystals == '\x05')) {
                Game.hub[Hub].flags = Game.hub[Hub].flags | 2;
            }
        }
        if ((LBIT & 0x3e00000) != 0) {
            Game.level[Level].flags |= 0x800;
        } else {
            if (TimeTrial != 0) {
                TimeTrialWait = 0;
                if (obj->finished == 1) {
                    NewLevelTime(TimeTrialTimer.itime);
                    if (TimeTrialTimer.itime < LDATA->time[2]) {
                        (Game.level[Level]).flags |= 7;
                    } else if (TimeTrialTimer.itime < LDATA->time[1]) {
                        (Game.level[Level]).flags |= 3;
                    } else if (TimeTrialTimer.itime < LDATA->time[0]) {
                        (Game.level[Level]).flags |= 1;
                    }
                    if ((u32)finish_frames < 0xb4) {
                        finish_frames = 0xb4;
                    }
                } else {
                    finish_frames = 0x3c;
                }
            } else {
                if ((plr_items & 2) != 0) {
                    Game.level[Level].flags = Game.level[Level].flags | 0x10;
                }
                if ((plr_items & 4) != 0) {
                    Game.level[Level].flags = Game.level[Level].flags | 0x20;
                }
                if ((plr_items & 8) != 0) {
                    Game.level[Level].flags = Game.level[Level].flags | 0x40;
                }
                if ((plr_items & 0x20) != 0) {
                    Game.level[Level].flags = Game.level[Level].flags | 0x80;
                }
                if ((plr_items & 0x10) != 0) {
                    Game.level[Level].flags = Game.level[Level].flags | 0x100;
                }
                if ((plr_items & 0x40) != 0) {
                    Game.level[Level].flags = Game.level[Level].flags | 0x200;
                }
                if ((plr_items & 0x80) != 0) {
                    Game.level[Level].flags = Game.level[Level].flags | 0x400;
                }
            }
        }
        new_hub_flags = Game.hub[Hub].flags ^ old_hub_flags;
        new_lev_flags = Game.level[Level].flags ^ old_lev_flags;
        Game.level[Level].flags ^= new_lev_flags;
        if (obj->finished == 0) {
            return;
        }
    }
    finish_frame++;
    if (finish_frame != finish_frames) {
        return;
    }
    if (obj->finished == 1) {
        if (((LBIT & 0x3e00000) != 0) && ((new_lev_flags & 0x800) != 0)) {
            new_lev_flags = new_lev_flags ^ 0x800;
            Game.level[Level].flags = Game.level[Level].flags | 0x800;
        }
        if (TimeTrial != 0) {
            if (newleveltime_slot < 3) {
                if ((new_lev_flags & 7) != 0) {
                    GameSfx(0x26, NULL);
                    AddPanelDebris(TT_RELICX, TT_RELICY, 6, 0.125, 0x10);
                }
                NewMenu(&Cursor, 0x11, 0, -1);
            } else {
                NewMenu(&Cursor, 0x10, 0, -1);
            }
            JudderGameCamera(GameCam, 0.3, NULL);
            GameSfx(0x3c, NULL);
            return;
        }
        if (Level == 0x19) {
            CalculateGamePercentage(&Game);
            if (Game.gems > 0x2d) {
                cutmovie = 4;
                Game.cutbits |= 0x10000000;
            } else {
                cutmovie = 3;
                Game.cutbits |= 0x8000000;
            }
            new_level = 0x2b;
            return;
        } else
            new_level = 0x25;
    } else {
        if (TimeTrial != 0) {
            NewMenu(&Cursor, 0x10, 0, -1);
        }
        else if ((Level == 0x1D || Level == 0x1C || Level == 3) && new_lev_flags == 0) {
            NewMenu(&Cursor, 0x12, 0, -1);
        } else
            new_level = 0x25;
    }
    return;
}

//NGC MATCH
void OpenGame(void) {
  s32 i;
  s32 j;
  s32 k;
  
  for (k = 0; k < 0x23; k++) {
    Game.level[k].flags = 0;
  }
  for (k = 0; k < 6; k++) {
    Game.hub[k].flags = 0xff;
    Game.hub[k].crystals = '\0';
    if (k < 5) {
      for (i = 0, j = 0; i < 6; i++, j++) {
        if (HData[k].level[i] != -1) {
          if (j < 5) {
            Game.level[HData[k].level[i]].flags = 0x1f;
            Game.hub[k].crystals = Game.hub[k].crystals + '\x01';
          }
          else {
            Game.level[HData[k].level[i]].flags = 0x800;
          }
        }
      }
    }
    else {
      for (i = 0; i < 5; i++) {
        if (HData[k].level[i] != -1) {
          Game.level[HData[k].level[i]].flags = 0x17;
        }
      }
    }
  }
  Game.level[1].flags = Game.level[1].flags | 0x40;
  Game.level[17].flags = Game.level[17].flags | 0x80;
  Game.level[7].flags = Game.level[7].flags | 0x100;
  Game.level[19].flags = Game.level[19].flags | 0x200;
  Game.level[10].flags = Game.level[10].flags | 0x400;
  Game.level[4].flags = Game.level[4].flags | 0x20;
  Game.level[12].flags = Game.level[12].flags | 0x20;
  Game.level[14].flags = Game.level[14].flags | 0x20;
  Game.level[5].flags = Game.level[5].flags | 0x20;
  Game.level[20].flags = Game.level[20].flags | 0x20;
  Game.level[33].flags = Game.level[33].flags | 0x20;
  Game.level[27].flags = Game.level[27].flags | 0x20;
  Game.level[28].flags = Game.level[28].flags | 0x20;
  Game.level[29].flags = Game.level[29].flags | 0x20;
  Game.level[32].flags = Game.level[32].flags | 0x20;
  Game.level[9].flags = Game.level[9].flags | 0x20;
  Game.powerbits = 0xff;
  Game.cutbits = (u32)-1;
  CalculateGamePercentage(&Game);
  ResetBonus();
  ResetDeath();
  ResetGemPath();
  return;
}

//NGC MATCH
void InitGameMode(s32 mode) {
  ResetTimer(&GameTimer);
  finish_frame = 0;
  ResetTimer(&PauseTimer);
  pausestats_frame = 0;
  ResetTimer(&MenuTimer);
  ResetTimeTrial();
  if (force_menu != -1) {
    NewMenu(&Cursor,force_menu,-1,-1);
    force_menu = -1;
  }
  else {
    if ((LDATA->flags & 1) != 0) {
      start_time = 3.0f;
      NewMenu(&Cursor,-1,-1,-1);
    }
    else {
      start_time = 0.0f;
    }
  }
  new_mode = -1;
  new_level = -1;
  ResetPlayer(1);
  ResetTubs();
  plr_lives.count = (short)Game.lives;
  plr_lives.draw = plr_lives.count;
  switch (Level) {                        
    case 0x25:
        if (GameMode == 1) {
            InitGameCut();
            return;
        }
        return;
    case 0x17:
        ResetDRAINDAMAGE();
        return;
    case 0x19:
        ResetCRUNCHTIME();
        return;
    case 0x28:
        InitGameCut();
        break;
  }
  return;
}

//NGC MATCH
void InitPositions(void) {
  struct nuvec_s* p0;
  struct nuvec_s* p1;
  
  pos_START = NULL;
  SAFEY = -1000.0f;
  if ((SplTab[0].spl != NULL) && (0 < (SplTab[0].spl)->len)) {
    pos_START = (struct nuvec_s *)(SplTab[0].spl)->pts;
  }
  if (((Level == 0x1c) && (SplTab[29].spl != NULL)) && (SplTab[45].spl != NULL)) {
    p0 = (struct nuvec_s*)(SplTab[29].spl)->pts;
    p1 = (struct nuvec_s*)(SplTab[45].spl)->pts;
    start_pos.x = (p0->x + p1->x) * 0.5f;
    start_pos.y = (p0->y + p1->y) * 0.5f;
    start_pos.z = (p0->z + p1->z) * 0.5f;
    pos_START = &start_pos;
    return;
  }
  if ((((LDATA->vSTART).x != 0.0f) || ((LDATA->vSTART).y != 0.0f)) || ((LDATA->vSTART).z != 0.0f)) {
     pos_START = &LDATA->vSTART;
  }
  return;
}

//NGC MATCH
void InitLevel(void) {
  s32 lp;
  s32 y;
  s32 menu;
  s32 level;
  struct nuvec_s start;
  struct nuvec_s end;
  struct nuinstance_s *ipost;
  struct nuinstance_s *instgrp[24];
  
  InitPositions();
  ResetTempCharacter(-1,-1);
  menu = -1;
  ResetTempCharacter2(-1,-1);
  y = -1;
  level = -1;
  tempanim_waitaudio = 1;
    switch(Level) {
        case 0x22:
        break;
            case 0x23:
                          NewGame();
                          y = 0;
                          Hub = -1;
                          menu = 0;
                          ResetTempCharacter(0x60,0x2b);
                          Demo = 0;
            break;
            case 0x25:
                      CalculateGamePercentage(&Game);
                      ResetTempCharacter(2,0x22);
                      hub_vr_level = 0xffffffff;
                      loadsave_frame = 0x3d;
                      warp_level = 0xffffffff;
            break;
            case 0x27:
                        menu = 0x13;
                        level = 0x25;
            break;

            case 0x29:
                        menu = 0x13;
                        level = 0x2b;
            break;
            case 0x26:
                        ResetTempCharacter(2,0x22);
                        menu = 0x14;
                        tempanim_nextaction = 0x73;
                        y = 0;
                        gamesfx_channel = 4;
                        tempanim_waitaudio = 1;
                        cortex_gameover_i = -1;      
            break;
            case 0x2b:
                        InitCredits();
                        menu = 0x23;
                        ResetTempCharacter(0xbb,0x22);
                        level = 0x2b;
                        ResetTempCharacter2(0,0x22);
            break;
            case 0xd:
                ResetTempCharacter(0x62,0x22);
            break;
            case 0x1a:
                ResetTempCharacter(0xb8,0x22);
            break;
            case 0x12:
                ResetTempCharacter(0xb9,0x22);
            break;
            case 0x1e:
                        ResetTempCharacter(0xba,0x22);
            break;
            case 0x18:
                ResetTempCharacter(0xbc,0x22);
            break;
            case 0x15:
                ResetTempCharacter(0xbc,0x22);
            break;
            case 0x16:
                ResetTempCharacter(0xbc,0x22);
            break;
            case 0x19:
                ResetTempCharacter(2,0x22);
            break;
            case 0x13:
                        if ((ObjTab[145].obj.special != NULL) && (ObjTab[146].obj.special != NULL)) {
                          for (lp = 0; lp < 0x18; lp++) {
                            if (lp & 1) {
                                instgrp[lp] = ObjTab[145].obj.special->instance;
                            } else{
                              instgrp[lp] = ObjTab[146].obj.special->instance;
                            }
                          }
                          start.x = 36.3721f;
                          start.y = -0.1f;
                          start.z = 140.7f;
                          end.x = 36.49f;
                          end.y = -0.1f;
                          end.z = 146.84f;
                          NuBridgeCreate(instgrp,NULL,&start,&end,1.2f,-0x4200,0.3f,0.075f,
                                         -0.01f,4.0f,10,1.0f,0.5f,3,-0x7f7f7f80);
                          start.x = 206.1f;
                          start.y = 0.0f;
                          start.z = 249.8f;
                          end.x = 213.25f;
                          end.y = 0.0f;
                          end.z = 249.8f;
                          NuBridgeCreate(instgrp,NULL,&start,&end,1.2f,0,0.25f,0.08f,-0.005f, 7.0f
                                         ,0xc,1.0f,0.5f,3,-0x7f7f7f80);
                        }
            break;
            case 8:
                        if (ObjTab[155].obj.special != NULL) {
                          if (ObjTab[158].obj.special != NULL) {
                            ipost = (ObjTab[158].obj.special)->instance;
                          } else{
                              ipost = NULL;
                          }
                          for (lp = 0; lp < 0x18; lp++) {
                            instgrp[lp] = (ObjTab[155].obj.special)->instance;
                          }
                          start.x = -79.875f;
                          start.y = -14.2f;
                          start.z = 22.7f;
                          end.x = -79.875f;
                          end.y = -14.2f;
                          end.z = 26.0f;
                          NuBridgeCreate(instgrp,ipost,&start,&end,1.2f,-0x4000,0.18f,0.1f,-0.01f,4.0f,7,1.08f,
                                         0.5f,3,-0x7f7f7f80);
                          start.x = 52.5f;
                          start.y = -33.7f;
                          start.z = -73.45f;
                          end.x = 63.9f;
                          end.y = -33.7f;
                          end.z = -73.14f;
                          NuBridgeCreate(instgrp,ipost,&start,&end,1.2f,0,0.4,0.05,-0.004,8.0,0x11,1.08f,0.5f
                                         ,3,-0x7f7f7f80);
                          start.x = 70.0;
                          start.y = -33.7f;
                          start.z = -73.12f;
                          end.x = 77.0;
                          end.y = -33.7f;
                          end.z = -73.3f;
                          NuBridgeCreate(instgrp,ipost,&start,&end,1.2f,0,0.25f,0.075f,-0.008f,4.0f,10,1.08f,0.5f
                                         ,3,-0x7f7f7f80);
                        }
            break;
            case 1:
                        if (((ObjTab[159].obj.special != NULL)) && (ObjTab[160].obj.special != NULL)) {
                          ipost = NULL;
                          if (ObjTab[161].obj.special != NULL) {
                            ipost = (ObjTab[161].obj.special)->instance;
                          }
                          for (lp = 0; lp < 0x18; lp++) {
                            if (lp == (lp / 3) * 3) {
                                instgrp[lp] = ObjTab[159].obj.special->instance;
                            } else{
                              instgrp[lp] = ObjTab[160].obj.special->instance;
                            }
                          }
                          start.x = -1.31f;
                          start.y = 18.6f;
                          start.z = 16.23f;
                          end.x = -7.14f;
                          end.y = 18.6f;
                          end.z = 14.8f;
                          NuBridgeCreate(instgrp,ipost,&start,&end,1.2f,31000,0.12f,0.1f,-0.01f,4.0f,7,1.12f, 0.9f
                                         ,3,-0x7fffcfa8);
                          start.x = -8.47f;
                          start.y = 18.6f;
                          start.z = 14.4f;
                          end.x = -13.17f;
                          end.y = 18.6f;
                          end.z = 10.09f;
                          NuBridgeCreate(instgrp,ipost,&start,&end,1.2f,25000,0.12f,0.1f,-0.01f,4.0f,7,1.12f, 0.9f
                                         ,3,-0x7fffcfa8);
                        }
            break;
        }
  if ((LBIT & 0x3e00000) != 0) {
    Mask->active = 0;
  }
  ai_lookpos = v000;
  Cursor.menu = -1;
  NewMenu(&Cursor,menu,y,level);
  VEHICLECONTROL = 0;
  if ((Level == 6) || (Level == 0x1d)) {
    VEHICLECONTROL = 1;
  }
  else if (Level == 0x22) {
    VEHICLECONTROL = 2;
  }
  else {
    VEHICLECONTROL = 0;
  }
  level_part_2 = 0;
  SKELETALCRASH = 0;
  if (Level != 0x25) {
    new_hub_flags = '\0';
    new_lev_flags = 0;
  }
  ResetItems();
  boss_dead = 0;
  COMPLEXPLAYERSHADOW = (s32)(Level == 0xc);
  bonusgem_ok = (u32)(Level != 5);
  ResetGemPath();
  hubleveltext_level = -1;
  hubleveltext_pos = 0.0f;
  tumble_moveduration = 1.0f;
  tumble_duration = 1.0f;
  tumble_time = 1.0f;
  HubLevelText = NULL;
  InitBugAreas();
  bonus_restart = 0;
  plr_invisibility_time = 5.0f;
  in_finish_range = 0;
  LivesLost = 0;
  if ((LBIT & 0x3e00000) != 0) {
    Mask->active = 0;
  }
  ResetAkuAkuAdvice();
  return;
}

//NGC MATCH
void UpdateLevel(void) {
  s32 sfx;
  s32 i;

  for (i = 0; i < 5; i++) {
    lev_ambpos[i] = (player->obj).pos;
  }
  sfx = -1;
    switch ((s32) Level) {
    case 5:
        sfx = 0xB5;
        break;
    case 0x17:
        UpdateDRAINDAMAGE();
    case 0x26:
        sfx = 0xB9;
        break;
    case 0x19:
        sfx = UpdateCRUNCHTIME();
        break;
    case 2:
        if ((s32) VEHICLECONTROL == 0) {
            sfx = 0xB4;
        }
        break;
    case 8:
        gamesfx_effect_volume = 0x3FFF;
        GameSfxLoop(0xB7, 0);
    case 29:
    case 33:
    case 20:
    case 16:
    case 9:
    case 12:
    case 7:
    case 4:
    case 0:
        sfx = 0xB3;
        break;
    case 0xe:
        sfx = 0xC0;
        break;
    case 0x13:
        sfx = 0xBC;
        break;
    }
  if (sfx != -1) {
    gamesfx_effect_volume = 0x1fff;
    GameSfxLoop(sfx,NULL);
  }
  return;
}

//NGC MATCH
void DrawLevel(void) {
    switch (Level) {                          
    case 0x25:
        HubDrawItems();
        return;
    case 0x19:
        DrawCRUNCHTIME();
        return;
    case 0x17:
        DrawDRAINDAMAGE();
        return;
    }
}

void ResetLevel(void) {
  ResetVehicleLevel(1);
  return;
}

//NGC MATCH
void CleanLetters(char *txt) {
    while (*txt != '\0') {
      if (*txt == '_') {
        *txt = ' ';
      }
      *txt++;
      if (Game.language == 'c') {
        *txt++;
      }
    }
  return;
}

void DrawNameInputTable(struct Cursor *cursor,float x0,float y0)
{
  s32 align;
  s32 colour;
  s32 col;
  s32 j;
  char *tmpchar;
  float y;
  float x;
  s32 i;
  
  if (Game.language == 'c') {
    tbuf[2] = '\0';
    tbuf[1] = ' ';
  }
  else {
    tbuf[1] = '\0';
  }
  y = y0 - (float)(cursor->y_max - cursor->y_min) * MENUDY * 0.5f;
  i = 0;
  do {
    j = i + 1;
    x = x0 - (float)(cursor->x_max - cursor->x_min) * 0.06f * 0.5f;
    if (cursor->x_min <= cursor->x_max) {
      tmpchar = (char *)((s32)NameInputTable + cursor->x_min + i * 7);
      do {
        tbuf[0] = *tmpchar;
        align = 1;
        tmpchar = tmpchar + 1;
        if ((i == cursor->y) && (cursor->x_min == cursor->x)) {
          align = 0x21;
          col = 0;
          if (5 < GlobalTimer.frame % 0xc) {
            col = 3;
          }
        }
        else {
          col = 2;
        }
        cursor->x_min = cursor->x_min + 1;
        Text3D(tbuf,x,y,1.0f,0.6f,0.6f,0.6f,align,col);
        x = x + 0.06000000238418579f;
      } while (cursor->x_min <= cursor->x_max);
    }
    x = y + MENUDY;
    y = x;
    i = j;
  } while (j < 4);
  if ('\x03' < cursor->y_max) {
    i = 1;
    if (cursor->y == '\x04') {
      i = 0x21;
      col = 0;
      if (5 < GlobalTimer.frame % 0xc) {
        col = 3;
      }
    }
    else {
      col = 2;
    }
    Text3D(tDONE[Game.language],x0,x,1.0f,0.6f,0.6f,0.6f,i,col);
    y = y + MENUDY;
  }
  if ('\x04' < cursor->y_max) {
    i = 1;
    if (cursor->y == '\x05') {
      i = 0x21;
      col = 0;
      if (5 < GlobalTimer.frame % 0xc) {
        col = 3;
      }
    }
    else {
      col = 2;
    }
    Text3D(tCANCEL[Game.language],x0,y,1.0f,0.6f,0.6f,0.6f,i,col);
  }
  if (((cutmovie == -1) && (cursor->menu != '\x04')) && (cursor->menu != '\x1a')) {
    DrawPanel3DObject(0x81,x0,y0,1.0f,0.11875f,0.125f,0.11875f,0xc000,0,0,ObjTab[129].obj.scene,ObjTab[129].obj.special,0);
  }
  return;
}

//NGC MATCH
void DrawMenuEntry(struct cursor_s *cursor,char *txt,float *x,float *y,s32 *i) {
  s32 align;
  s32 col;
  float size;
  
  if ((*i >= cursor->y_min) && (*i <= cursor->y_max)) {
    col = *i;
    align = 1;
    size = dme_sy * 0.6f;
    if (col == cursor->y) {
      col = 0;
      if (5 < GlobalTimer.frame % 0xc) {
        col = 3;
      }
      align = 0x21;
    }
    else {
      col = 2;
    }
    Text3D(txt,*x,*y + dme_yadj,1.0f,size * dme_sx,size * dme_symul,size,align,col);
  }
  NextMenuEntry(y,i);
  dme_sx = 1.0f;
  dme_sy = 1.0f;
  dme_symul = 1.0f;
  dme_yadj = 0.0f;
  return;
}

//NGC MATCH
void DrawMenuEntry2(struct cursor_s *cursor,char *txt0,char *txt1,float *x,float *y,s32 *i) {
  s32 align;
  s32 col;
  float sx2;
  float size;
 
  if ((*i >= cursor->y_min) && (*i <= cursor->y_max)) {
    align = 1;
    size = dme_sy * 0.6f;
    sx2 = size;
    if (*i == cursor->y) {
      col = 0;
      if (5 < GlobalTimer.frame % 0xc) {
        col = 3;
      }
      align = 0x21;
    }
    else {
      col = 2;
    }
    Text3D(txt0,*x,*y,1.0f,(sx2 * dme_sy),size,size,align,col);
    size = MENUDY * dme_sy + *y;
    *y = size;
    Text3D(txt1,*x,size,1.0f,(sx2 * dme_sx),sx2,sx2,align,col);
  }
  NextMenuEntry(y,i);
  dme_sx = 1.0f;
  dme_sy = 1.0f;
  return;
}

//NGC MATCH
void NextMenuEntry(float *y, s32 *i) {
  *y += MENUDY * dme_sy;
  *i += 1;
  return;
}

//NGC MATCH
void NewMenu(struct cursor_s* cur, s32 menu, s32 y, s32 level) {
    s32 old_menu;
    s32 lock;

    old_menu = cur->menu;
    cur->wait = 0;
    lock = 0;
    
    if (menu == old_menu) {
        menu = -1;
    }
    if ((menu != cur->new_menu) || (cur->wait_hack == 0)) {
        switch (menu) {
            case 6:
            case 7:
            case 8:
            case 0x15:
                break;
            case -1:
                if ((old_menu == 0x15) || (old_menu == 0x18)) {
                    cur->wait = 60;
                }
                break;
            case 4:
            case 0x1a:
                y = 4;
                memcpy(Game.name, "CRASH___", 9);
                cur->y = '\x04';
                i_nameinput = 0;
                break;
            case 0x11:
                i_nameinput = 0;
                break;
            case 0x16:
            case 0x1d:
            case 0x19:
                InitLoadSaveDeleteScreen(cur, menu);
                break;
            case 0x22:
                lock = 0xb4;
                break;
            case 0x14:
                gameover_hack = 0;
                break;
        }
    }
    if (cur->wait != 0) {
        cur->new_menu = menu;
        cur->wait_frames = cur->wait;
    } else {
        ResetTimer(&MenuTimer);
        cur->new_level = (char)level;
        cur->new_menu = -1;
        cur->wait = 0;
        cur->button_lock = lock;
        cur->menu = menu;
        
        if (cur->menu != -1) {
            GetMenuInfo(cur);
            
            cur->x = cur->remember[menu].x;
            
            if (cur->x < cur->x_min) {
                cur->x = cur->x_min;
            } else if (cur->x > cur->x_max) {
                cur->x = cur->x_max;
            }
            
            if ((y < cur->y_min) || (y > cur->y_max)) {
                cur->y = cur->remember[menu].y;
                if (cur->y < cur->y_min) {
                    cur->y = cur->y_min;
                } else if (cur->y > cur->y_max) {
                    cur->y = cur->y_max;
                }
            } else {
                cur->y = y;
            }
            
            if (((cur->menu == 4) || (cur->menu == 26)) || (cur->menu == 17)) {
                cur->x = 0;
            }
            cur->menu_frame = 0;
            cur->item_frame = 0;
            cur->wait_hack = 0;
        }
    }
    return;
}

//NGC MATCH
void GetMenuInfo(struct cursor_s* cur) {
    cur->x_min = 0;
    cur->y_min = 0;
    cur->x_max = 0;
    cur->y_max = 0;
    switch (cur->menu) {
        case 0:
            cur->y_max = (LANGUAGEOPTION != 0) ? 3 : 2;
            return;
        case 47:
        case 1:
            switch (ParseNintendoErrorCode()) {
                case 2:
                case 3:
                    cur->y_max = 2;
                    break;
                case 6:
                    cur->y_max = 2;
                    break;
                case 8:
                    cur->y_max = 2;
                    break;
                default:
                    cur->y_max = 1;
                    break;
            }
            return;
        case 2:
        case 3:
            return;
        case 5:
            cur->y_max = (TimeTrial != 0) ? 3 : 2;
            return;
        case 6:
            cur->y_max = 2;
            if (LANGUAGEOPTION != 0) {
                cur->y_max = 3;
            }
            return;
        case 7:
            cur->y_max = 2;
            return;
        case 8:
            cur->y_max = 2;
            return;
        case 9:
            cur->y_max = 5;
            return;
        case 11:
            cur->y_max = 0xa;
            return;
        case 15:
            cur->y_max = 4;
            return;
        case 10:
            cur->y_max = 1;
            return;
        case 12:
            cur->y_max = 0xa;
            return;
        case 14:
            cur->x_max = 5;
            cur->y_max = 5;
            return;
        case 20:
            cur->y_max = 1;
            return;
        case 16:
        case 18:
            cur->y_max = 1;
            return;
        case 4:
        case 26:
            cur->x_max = 6;
            cur->y_max = 5;
            if (Game.language == 0x63) {
                cur->y_max = 8;
            }
            return;
        case 17:
            cur->x_max = 6;
            cur->y_max = 4;
            if (Game.language == 0x63) {
                cur->y_max = 7;
            }
            return;
        case 21:
            cur->y_max = 3;
            return;
        case 22:
        case 25:
        case 29:
            cur->x_max = 1;
            cur->y_max = 2;
            return;
        case 27:
        case 48:
        case 32:
        case 30:
        case 23:
            cur->x_max = 0;
            cur->y_max = 1;
            return;
        case 24:
        case 28:
        case 31:
        case 33:
        case 38:
        case 49:
            cur->x_max = 0;
            cur->y_max = 0;
            return;
        case 39:
            cur->y_max = 1;
            return;
        case 40:
            cur->y_max = 0;
            return;
        case 41:
        case 42:
        case 43:
        case 44:
        case 45:
            cur->y_max = 0;
            return;
    }
}

//NGC MATCH
void DrawNODATAAVAILABLE(void) {
  Text3D(tNODATAAVAILABLE[Game.language][0],0.0f,0.0f,1.0f,0.6f,0.6f,0.6f,1,0);
  return;
}

//NGC MATCH
char* MakeEditText(char* txt) {
    s32 j;
    s32 k = 0;

    edit_txt[k++] = '#';
    edit_txt[k++] = 'g';

    for (j = 0; txt[j] != '\0'; j++) {
        if ((Game.language == 'c' && j == (i_nameinput + i_nameinput)) 
            || (Game.language != 'c' && (j == i_nameinput))) {
            edit_txt[k++] = '#';
            edit_txt[k++] = GlobalTimer.frame % 0xc < 6 ? 'o' : 'b';
        } else if ((Game.language == 'c' && j == (i_nameinput + 1) * 2) ||
                (Game.language != 'c' && j == (i_nameinput + 1))) {
            edit_txt[k++] = '#';
            edit_txt[k++] = 'g';
        }
        edit_txt[k++] = *(txt + j);
    }
    edit_txt[k] = '\0';
    return edit_txt;
}





//WIP

void AddSpacesIntoText(char* txt, u32 bits);                        /* extern */
void DrawMenuEntry(struct cursor_s *cursor, char* txt, float* x, float* y, int* i);/* extern */
void DrawMenuEntry2(struct cursor_s *cursor, char* txt0, char* txt1, float* x, float* y, int* i); /* extern */
s8* GetStringIdx(s32, s32);                              /* extern */
s32 ParseNintendoErrorCode();                       /* extern */
void Text3D(char* txt, float x, float y, float z, float scalex, float scaley, float scalez, int align, int colour); /* extern */
char* MakeEditText(char* txt);
extern struct nupad_s* Pad[3]; //Pad[2] dwarf
extern struct game_s Game;
extern struct game_s* game;
extern s32 GameMode;
extern struct globaltimer_s GlobalTimer;
extern struct hdata_s HData[6];
extern char* HubName[6][6];
extern s32 LANGUAGEOPTION;
extern struct LDATA_s *LDATA;
extern char* LanguageName[6];
extern s32 Level;
extern char* LevelName[6][44];
static float MENUDY;
extern float PANELMENUX;
extern s32 TimeTrial;
static float dme_sx;
extern s32 highallocaddr;
extern s32 pause_dir;
extern union variptr_u superbuffer_base;
extern union variptr_u superbuffer_ptr;
extern char* tDONE[6]; 
extern char* tLANGUAGE[6]; 
extern char* tLOADGAME[6];
extern char* tCANTDOWNLOAD[6];
extern char* tMUSICVOLUME[6];
extern char* tCONTINUE[6];
extern char* tGAMEOVER[6];
extern char* tNEWGAME[6];
extern char* tRESTARTRACE[6];
extern char* tJCANCEL2;
extern char* tCANCEL[6];
extern char* tLOAD[6];
extern char* tSAVEGAME[6];
extern char* tSAVE[6];
extern char* tDELETEGAME[6];
extern char* tDELETE[6];
extern char* tEXIT[6];
extern char* tOFF[6];
extern char* tON[6];
extern char* tOPTIONS[6];
extern char* tPLAYCRASHBLAST[6];
extern char* tQUIT[6];
extern char* tRELICS[6];
extern char* tRESTARTTRIAL[6];
extern char* tRESUME[6];
extern char* tSFXVOLUME[6];
extern char* tSOUNDOPTIONS[6];
extern char* tGAMEINST[3][6];
extern char* tSUPERBELLYFLOPTEXT[6];
extern char* tTCR_ABANDONTHISGAME[6]; 
extern char* tTCR_ABANDONTHISLEVEL[6]; 
extern char* tVIBRATION[6]; 
extern char* tWARPROOM[6]; 
extern char* tYES[6]; 
extern char* tMEMCARDCONTINUE[6]; 
extern char* tMEMCARDRETRY[6]; 
extern char* tMEMCARDMANAGE[6]; 
extern char* tFORMAT[6]; 
extern char* tGEMS[6]; 
extern char* tNO[6];
extern char* tPRESSxTOCONTINUE[6];
extern char* tCONFIRMOVERWRITE[2][6];
extern char* tMEMCARDACCESS[2][6];
extern char* tCONFIRMDELETE[6];
extern char* tCONFIRMSAVE[6];
extern char* tCONFIRMFORMAT[6];
extern char* tFAILEDTOSAVE[6];
extern char* tFAILEDTODELETE[6];
extern char* tCONNECTGBA[3][6];
extern char* tCONNECTING[6];
extern char* tPLEASEWAIT[6];
extern char* tDOWNLOADING[6];
extern char* tLOADCRASHBLAST[6];
extern char* tPLAYCRASHBLAST[6];
extern char* tTCR_CONTENTPROTECTIONFAULT[6];
extern int new_power;
extern float dme_sy;
extern s32 InvincibilityCHEAT;
extern s32 LIFTPLAYER;
extern s32 ShowPlayerCoordinate;
extern s32 ExtraMoves;
float TT_INPUTY;
int new_level;
struct anim_s TempAnim;
int gameover_hack;
int fadeval;
int memcard_gamesavailable;
int memcard_saveneeded;
int memcard_savestarted;
int memcard_savemessage_delay;
int memcard_formatme;
int memcard_formatting;
int memcard_formatmessage_delay;
int saveload_error;
int saveload_cardchanged; 
int lost_controller;
float GAMENAMEY;
float POWERTEXTY;
extern struct numtl_s * GBABG_mtl;
int VersionDisplayFlag;
int SaveMenu;
int BackMenu;



/*
    s32 i; // 0x38(r1)
    s32 j; // r29
    s32 k; // r0
    s32 col; // r5
    float f; // f29
    float size; // f29
    float x; // 0x30(r1)
    float y; // 0x34(r1)
    float dy; // f31
    float xnew; // f31
    // Size: 0xC, DWARF: 0x2739C2
    struct pdata_s* PData; // r31
    unsigned char c; // 
*/

void DrawMenu(struct cursor_s *cursor,s32 paused) {
    float fVar1;
    float scalex;
    UNKTYPE* uVar2;
    s32 col;
    //uint uVar3;
    UNKTYPE* uVar4;
    char **txt;
    s32 iVar5;
    s32 unaff_r27;
    //char *(*papcVar6) [6];
    struct pdata_s *pdat;
    s32 iVar7;
    //double dVar8;
    double dVar9;
    double dVar10;
    //float local_68;
    float x;
    float y;
    s32 i;
    char *c;
    float dy;
    
    iVar7 = new_power;
    if (cursor->wait != '\0') {
        return;
    }
    if (cursor->menu == -1) {
        return;
    }
    if (GameMode == 1) {
        return;
    }
    x = 0.0;
    if (pause_dir != 0) {
        //local_68 = 0x1eU - paused;
        x = 0x1eU - paused * 0.06666667;
        if (pause_dir == 1) {
            x = 0.0 - x;
        }
        else {
            x = x + 0.0;
        }
    }
    col = (s32)cursor->y_min;
    //uVar3 = cursor->y_max - col;
    dVar9 = 0.5;
    iVar5 = (s32)cursor->menu;
    i = 0;
    dVar10 = 1.0;
    fVar1 = (cursor->y_max - col) * MENUDY;
    //dVar8 = (double)(fVar1 * 0.5);
    if (iVar5 != 0x17) {
        if (iVar5 < 0x18) {
            if (iVar5 == 0xb) {
                if (paused < 0x1e) {
                    return;
                }
                dme_sy = 0.8333333;
                y = 0.0 - fVar1 * 0.8333333 * 0.5;
                if (Game.language == 'c') {
                    c = "R E S T A R T   L E V E L ";
                }
                else {
                    c = "RESTART LEVEL";
                }
                DrawMenuEntry(cursor,c,&x,&y,(int*)i);
                dme_sy = 0.8333333;
                if (Game.language == 'c') {
                    c = "R E S E T   L E V E L ";
                }
                else {
                    c = "RESET LEVEL";
                }
                DrawMenuEntry(cursor,c,&x,&y,(int*)i);
                dme_sy = 0.8333333;
                if (Game.language == 'c') {
                    c = "G O T O   L E V E L ";
                }
                else {
                    c = "GOTO LEVEL";
                }
                DrawMenuEntry(cursor,c,&x,&y,(int*)i);
                dme_sy = 0.8333333;
                if (Game.language == 'c') {
                    c = "I N V I N C I B I L I T Y :   ";
                }
                else {
                    c = "INVINCIBILITY: ";
                }
                if (InvincibilityCHEAT == 0) {
                    txt = tOFF;
                }
                else {
                    txt = tON;
                }
                sprintf(tbuf,"%s%s",c,txt[Game.language]);
                DrawMenuEntry(cursor,tbuf,&x,&y,(int*)i);
                dme_sy = 0.8333333;
                if (Game.language == 'c') {
                    c = "G O T O   N E X T   C H E C K P O I N T ";
                }
                else {
                    c = "GOTO NEXT CHECKPOINT";
                }
                DrawMenuEntry(cursor,c,&x,&y,(int*)i);
                dme_sy = 0.8333333;
                if (Game.language == 'c') {
                    c = "G O T O   L A S T   C H E C K P O I N T ";
                }
                else {
                    c = "GOTO LAST CHECKPOINT";
                }
                DrawMenuEntry(cursor,c,&x,&y,(int*)i);
                dme_sy = 0.8333333;
                if (Game.language == 'c') {
                    c = "O P E N   G A M E ";
                }
                else {
                    c = "OPEN GAME";
                }
                DrawMenuEntry(cursor,c,&x,&y,(int*)i);
                dme_sy = 0.8333333;
                if (Game.language == 'c') {
                    c = "L I F T   P L A Y E R :   ";
                }
                else {
                    c = "LIFT PLAYER: ";
                }
                if (LIFTPLAYER == 0) {
                    txt = tOFF;
                }
                else {
                    txt = tON;
                }
                sprintf(tbuf,"%s%s",c,txt[Game.language]);
                DrawMenuEntry(cursor,tbuf,&x,&y,(int*)i);
                dme_sy = 0.8333333;
                if (Game.language == 'c') {
                    c = "P L A Y E R   C O O R D I N A T E :   ";
                }
                else {
                    c = "PLAYER COORDINATE: ";
                }
                if (ShowPlayerCoordinate == 0) {
                    txt = tOFF;
                }
                else {
                    txt = tON;
                }
                sprintf(tbuf,"%s%s",c,txt[Game.language]);
                DrawMenuEntry(cursor,tbuf,&x,&y,(int*)i);
                dme_sy = 0.8333333;
                if (Game.language == 'c') {
                    c = "E X T R A   M O V E S :   ";
                }
                else {
                    c = "EXTRA MOVES: ";
                }
                if (ExtraMoves == 0) {
                    txt = tOFF;
                }
                else {
                    txt = tON;
                }
                sprintf(tbuf,"%s%s",c,txt[Game.language]);
                DrawMenuEntry(cursor,tbuf,&x,&y,(int*)i);
                dme_sy = 0.8333333;
                if (Game.language == 'c') {
                    c = "R E S E T   G A M E ";
                }
                else {
                    c = "RESET GAME";
                }
                goto LAB_80036f1c;
            }
            if (0xb < iVar5) {
                if (iVar5 == 0x11) {
                    DrawNameInputTable(cursor,0.0,TT_INPUTY);
                    return;
                }
                if (0x11 < iVar5) {
                    if (iVar5 == 0x14) {
                        if (TempAnim.newaction == 0x75) {
                            return;
                        }
                        if (TempAnim.newaction == 0x33) {
                            return;
                        }
                        if (new_level != -1) {
                            return;
                        }
                        if (gameover_hack != 1) {
                            return;
                        }
                        if ((TempAnim.newaction == 0x22) && (fadeval == 0)) {
                            y = (float)(0.5 - fVar1 * 0.5);
                            x = 0.4;
                            DrawMenuEntry(cursor,tCONTINUE[Game.language],&x,&y,(int*)i);
                            DrawMenuEntry(cursor,tQUIT[Game.language],&x,&y,(int*)i);
                        }
                        if (Game.language - 2 < 4) {
                            fVar1 = 0.0;
                            dy = -0.6;
                        }
                        else {
                            fVar1 = 0.4;
                            dy = -0.5;
                        }
                        scalex = 0.8;
                        c = tGAMEOVER[Game.language];
                        dVar10 = 1.0;
                        x = fVar1;
                        y = dy;
                        goto LAB_80037ad8;
                    }
                    if (iVar5 < 0x15) {
                        if (iVar5 != 0x12) {
                            return;
                        }
                        y = (float)(-0.699999988079071 - fVar1 * 0.5);
                        DrawMenuEntry(cursor,tRESTARTRACE[Game.language],&x,&y,(int*)i);
                        c = tWARPROOM[Game.language];
                        goto LAB_80036da0;
                    }
                    if (iVar5 != 0x15) {
                        if (iVar5 != 0x16) {
                            return;
                        }
                        iVar7 = ParseNintendoErrorCode();
                        if (iVar7 != 0) goto LAB_80036b44;
                        iVar7 = 1;
                        Text3D(tLOADGAME[Game.language],0.0,0.81,1.0,0.6,0.6,0.6,1,0);
                        if (memcard_gamesavailable == 0) {
                            DrawNODATAAVAILABLE();
                            cursor->y = cursor->y_max;
                        }
                        else {
                            DrawGameSlots(cursor);
                        }
                        if (cursor->y == cursor->y_max) {
                            iVar7 = 0x21;
                            col = 0;
                            if (5 < GlobalTimer.frame % 0xc) {
                                col = 3;
                            }
                        }
                        else {
                            col = 2;
                        }
                        c = tJCANCEL2;
                        if (Game.language != 99) {
                            c = tCANCEL[Game.language];
                        }
                        goto LAB_80036c8c;
                    }
                    if (Game.language == 'c') {
                        c = "%s/ %s";
                    }
                    else {
                        c = "%s/%s";
                    }
                    sprintf(tbuf,c,tLOAD[Game.language],tSAVE[Game.language]);
                    Text3D(tbuf,0.0,0.75,1.0,0.6,0.6,0.6,1,0);
                    XbUpdateDateStamp(&Game);
                    DrawGameSlot(&Game,0.0,0.25,4,0.6);
                    y = (float)(-0.5 - fVar1 * 0.5);
                    if (Game.language == 2) {
                        y = y - (float)(fVar1 * 0.5 * 0.2000000029802322);
                    }
                    DrawMenuEntry(cursor,tLOADGAME[Game.language],&x,&y,(int*)i);
                    if (Game.language == 2) {
                        y = (float)(fVar1 * 0.5 * 0.119999997317791 + y);
                    }
                    DrawMenuEntry(cursor,tSAVEGAME[Game.language],&x,&y,(int*)i);
                    if (Game.language == 2) {
                        y = (float)(fVar1 * 0.5 * 0.119999997317791 + (double)y);
                    }
                    DrawMenuEntry(cursor,tDELETEGAME[Game.language],&x,&y,(int*)i);
                    if (Game.language == 2) {
                        y = (float)(fVar1 * 0.5 * 0.119999997317791 + (double)y);
                    }
                    c = tEXIT[Game.language];
                    goto LAB_80037638;
                }
                if (iVar5 == 0xe) {
                    if (paused < 0x1e) {
                        return;
                    }
                    //local_68 = cursor->y_max - col + 2;
                    y = cursor->y_max - col + 2 * 0.1f * 0.5f - 0.2f;
                    i = col;
                    if (cursor->y_max < col) {
                        return;
                    }
                    do {
                        iVar7 = (s32)cursor->x_min;
                        dVar10 = 0.5;
                        //local_68 = cursor->x - iVar7;
                        fVar1 = x - cursor->x - iVar7 * 0.5;
                        if (iVar7 <= cursor->x_max) {
                            col = iVar7 * 0xc;
                            //papcVar6 = HubName[iVar7];
                            do {
                                //dVar8 = (double)fVar1;
                                if (i == 0) {
                                    if (unaff_r27 == -1) {
                                        c = "?";
                                    }
                                    else {
                                        c = HubName[iVar7];
                                    }
                                    strcpy(tbuf,c);
                                    Text3D(tbuf,(float)fVar1,y + 0.2f,1.0f,(float)(dVar9 * dVar10),
                                           (float)dVar9,(float)dVar9,1,4);
                                }
                                //unaff_r27 = (s32)(char)(&HData)[i + col];
                                if (unaff_r27 == -1) {
                                    c = "?";
                                }
                                else {
                                   // c = LevelName[unaff_r27 * 6];
                                }
                                strcpy(tbuf,c);
                                if ((i == cursor->y) && (iVar7 == cursor->x)) {
                                    iVar5 = 3;
                                    if (GlobalTimer.frame % 0xc < 6) {
                                        iVar5 = 4;
                                    }
                                }
                                else {
                                    iVar5 = 0;
                                }
                                iVar7 = iVar7 + 1;
                                col = col + 0xc;
                                Text3D(tbuf,(float)fVar1,y,1.0f,(float)(dVar9 * dVar10),(float)dVar9,
                                       (float)dVar9,1,iVar5);
                            //    papcVar6 = papcVar6[1];
                                fVar1 = (float)(fVar1 + dVar10);
                            } while (iVar7 <= cursor->x_max);
                        }
                        i = i + 1;
                        y = y - 0.1;
                    } while (i <= cursor->y_max);
                    return;
                }
                if (iVar5 < 0xf) {
                    if (iVar5 != 0xc) {
                        if (iVar5 != 0xd) {
                            return;
                        }
                        if (paused < 0x1e) {
                            return;
                        }
                        //dVar8 = -0.1000000014901161;
                        dVar9 = 9.5367431640625e-07; //35800000h
                        //dVar10 = 0.5;
                        y = 0.05;
                       // sprintf(tbuf,"SUPERBUFFER USED: %.2fMB/%.2fMB",
                        //        (((s32)superbuffer_ptr.voidptr -(s32)superbuffer_base * dVar9),7.0);
                        if (Game.language == 'c') {
                            AddSpacesIntoText(tbuf,1);
                        }
                        Text3D(tbuf,x,y,1.0,0.5,0.5,0.5,1,0);
                        y = (float)((double)y + -0.1000000014901161);
                        //local_68 = highallocaddr;
                        sprintf(tbuf,"HIGHALLOCADDR: %.2fMB/32.00MB",highallocaddr * dVar9);
                        if (Game.language == 'c') {
                            AddSpacesIntoText(tbuf,1);
                        }
                        Text3D(tbuf,x,y,1.0,0.5,0.5,0.5,1,0);
                        return;
                    }
                    if (paused < 0x1e) {
                        return;
                    }
                    y = (float)(0.0 - -0.1000000014901161);
                    sprintf(tbuf,"DRAW DISTANCE: %i/%i",(uint)LDATA->farclip,1000);
                    if (Game.language == 'c') {
                        AddSpacesIntoText(tbuf,1);
                    }
                    DrawMenuEntry(cursor,tbuf,&x,&y,(int*)i);
                    //local_68 = (double)(long long)(s32)LDATA->fognear;
                    sprintf(tbuf,"FOG NEAR: %i/%i",(s32)LDATA->fognear,(uint)LDATA->farclip);
                    if (Game.language == 'c') {
                        AddSpacesIntoText(tbuf,1);
                    }
                    DrawMenuEntry(cursor,tbuf,&x,&y,(int*)i);
                    //local_68 = (double)(long long)(s32)LDATA->fogfar;
                    sprintf(tbuf,"FOG FAR: %i/%i",(s32)LDATA->fogfar,(uint)LDATA->farclip);
                    if (Game.language == 'c') {
                        AddSpacesIntoText(tbuf,1);
                    }
                    DrawMenuEntry(cursor,tbuf,&x,&y,(int*)i);
                    sprintf(tbuf,"FOG RED: %i/255",(uint)LDATA->fogr);
                    if (Game.language == 'c') {
                        AddSpacesIntoText(tbuf,1);
                    }
                    DrawMenuEntry(cursor,tbuf,&x,&y,(int*)i);
                    sprintf(tbuf,"FOG GREEN: %i/255",(uint)LDATA->fogg);
                    if (Game.language == 'c') {
                        AddSpacesIntoText(tbuf,1);
                    }
                    DrawMenuEntry(cursor,tbuf,&x,&y,(int*)i);
                    sprintf(tbuf,"FOG BLUE: %i/255",(uint)LDATA->fogb);
                    if (Game.language == 'c') {
                        AddSpacesIntoText(tbuf,1);
                    }
                    DrawMenuEntry(cursor,tbuf,&x,&y,(int*)i);
                    sprintf(tbuf,"FOG OPACITY: %i/127",(uint)LDATA->foga);
                    if (Game.language == 'c') {
                        AddSpacesIntoText(tbuf,1);
                    }
                    DrawMenuEntry(cursor,tbuf,&x,&y,(int*)i);
                    sprintf(tbuf,"HAZE RED: %i/255",(uint)LDATA->hazer);
                    if (Game.language == 'c') {
                        AddSpacesIntoText(tbuf,1);
                    }
                    DrawMenuEntry(cursor,tbuf,&x,&y,(int*)i);
                    sprintf(tbuf,"HAZE GREEN: %i/255",(uint)LDATA->hazeg);
                    if (Game.language == 'c') {
                        AddSpacesIntoText(tbuf,1);
                    }
                    DrawMenuEntry(cursor,tbuf,&x,&y,(int*)i);
                    sprintf(tbuf,"HAZE BLUE: %i/255",(uint)LDATA->hazeb);
                    if (Game.language == 'c') {
                        AddSpacesIntoText(tbuf,1);
                    }
                    DrawMenuEntry(cursor,tbuf,&x,&y,(int*)i);
                    sprintf(tbuf,"HAZE WOBBLE: %i/255",(uint)LDATA->hazea);
                    if (Game.language == 'c') {
                        AddSpacesIntoText(tbuf,1);
                    }
                    c = tbuf;
                    goto LAB_80036f1c;
                }
                if (iVar5 == 0xf) {
                    y = (float)(0.0 - -0.1000000014901161);
                    if (Game.language == 'c') {
                        c = "L O G O S ";
                    }
                    else {
                        c = "LOGOS";
                    }
                    DrawMenuEntry(cursor,c,&x,&y,(int*)i);
                    if (Game.language == 'c') {
                        c = "I N T R O 1 ";
                    }
                    else {
                        c = "INTRO1";
                    }
                    DrawMenuEntry(cursor,c,&x,&y,(int*)i);
                    if (Game.language == 'c') {
                        c = "I N T R O 2 ";
                    }
                    else {
                        c = "INTRO2";
                    }
                    DrawMenuEntry(cursor,c,&x,&y,(int*)i);
                    if (Game.language == 'c') {
                        c = "O U T R O ";
                    }
                    else {
                        c = "OUTRO";
                    }
                    DrawMenuEntry(cursor,c,&x,&y,(int*)i);
                    if (Game.language == 'c') {
                        c = "O U T R O 2 ";
                    }
                    else {
                        c = "OUTRO2";
                    }
                    goto LAB_80036f1c;
                }
                if (iVar5 != 0x10) {
                    return;
                }
                y = (float)(-0.699999988079071 - -0.1000000014901161);
                DrawMenuEntry(cursor,tRESTARTTRIAL[Game.language],&x,&y,(int*)i);
                c = tWARPROOM[Game.language];
                goto LAB_80036da0;
            }
         /*   if (iVar5 == 5) {
                if (paused < 0x1e) {
                    return;
                }
                y = (float)(-0.5 - -0.1000000014901161);
                x = PANELMENUX;
                DrawMenuEntry(cursor,tRESUME[Game.language],&x,&y,(int*)i);
                DrawMenuEntry(cursor,tOPTIONS[Game.language],&x,&y,(int*)i);
                if (Level == 0x25) {
                    txt = tQUIT;
                }
                else {
                    txt = tWARPROOM;
                }
                DrawMenuEntry(cursor,txt[Game.language],&x,&y,(int*)i);
                if (TimeTrial == 0) {
                    return;
                }
                c = tRESTARTTRIAL[Game.language];
                goto LAB_80036f1c;
            }*/
        /*    if (iVar5 < 6) {
                if (2 < iVar5) {
                    if (iVar5 == 3) {
                        return;
                    }
                    if (iVar5 != 4) {
                        return;
                    }
                    goto LAB_80034ee8;
                }
                if (iVar5 < 1) {
                    if (iVar5 != 0) {
                        return;
                    }
                    y = (float)(-0.6899999976158142 - -0.1000000014901161);
                    VersionDisplayFlag = 1;
                    SaveMenu = iVar5;
                    BackMenu = iVar5;
                    DrawMenuEntry(cursor,tNEWGAME[Game.language],&x,&y,(int*)i);
                    DrawMenuEntry(cursor,tLOADGAME[Game.language],&x,&y,(int*)i);
                    DrawMenuEntry(cursor,tPLAYCRASHBLAST[Game.language],&x,&y,(int*)i);
                    sprintf(tbuf,"%s: %s",tLANGUAGE[Game.language],LanguageName[Game.language]);
                    c = tbuf;
                    goto LAB_80036f1c;
                }
                y = 0.2;
                for (iVar7 = 0; iVar7 < 4; iVar7 = iVar7 + 1) {
                    uVar2 = ParseNintendoErrorCode();
                    c = (char *)GetStringIdx(uVar2,iVar7);
                    if (c == (char *)0x0) break;
                    Text3D(c,0.0,y,1.0,0.4,0.4,0.4,1,0);
                    y = y - 0.1;
                }
                y = (float)(-0.6899999976158142 - -0.1000000014901161);
                DrawMenuEntry(cursor,tMEMCARDRETRY[Game.language],&x,&y,(int*)i);
                DrawMenuEntry(cursor,tMEMCARDCONTINUE[Game.language],&x,&y,(int*)i);
                iVar7 = ParseNintendoErrorCode();
                if (iVar7 == 6) {
                    uVar3 = (uint)Game.language;
                    txt = tMEMCARDMANAGE;
                    goto LAB_80037c10;
                }
                if (iVar7 < 7) {
                    if (3 < iVar7) {
                        return;
                    }
                    if (iVar7 < 2) {
                        return;
                    }
                    uVar3 = (uint)Game.language;
                    txt = tFORMAT;
                    goto LAB_80037c10;
                }
                goto LAB_80037be8;
            }*/
            if (iVar5 == 8) {
                return;
            }
            if (iVar5 < 9) {
                if (iVar5 != 6) {
                    if (iVar5 != 7) {
                        return;
                    }
                    if (paused < 0x1e) {
                        return;
                    }
                    //uVar3 = (uint)Game.sfx_volume;
                    y = (float)(-0.4600000083446503 - -0.1000000014901161) - (float)(-0.1000000014901161 * 0.5);
                    dme_sx = 1.0;
                    x = PANELMENUX;
                    //uVar2 = 0x20;
                    if (Game.sfx_volume != 0) {
                       // uVar2 = 0x3c;
                    }
                  //  uVar4 = 0x20;
                    if (Game.sfx_volume < 100) {
                     //   uVar4 = 0x3e;
                    }
                    sprintf(tbuf,"%c %i%% %c",uVar2,Game.sfx_volume,uVar4);
                    if (Game.language == 'c') {
                        AddSpacesIntoText(tbuf,1);
                    }
                    dme_sx = (float)dVar10;
                    DrawMenuEntry2(cursor,tSFXVOLUME[Game.language],tbuf,&x,&y,(int*)i);
                    //uVar3 = (uint)Game.music_volume;
                   // uVar2 = 0x20;
                    if (Game.music_volume != 0) {
                    //    uVar2 = 0x3c;
                    }
                   // uVar4 = 0x20;
                    if (Game.music_volume < 100) {
                    //    uVar4 = 0x3e;
                    }
                    sprintf(tbuf,"%c %i%% %c",uVar2,Game.music_volume,uVar4);
                    if (Game.language == 'c') {
                        AddSpacesIntoText(tbuf,1);
                    }
                    dme_sx = (float)dVar10;
                    DrawMenuEntry2(cursor,tMUSICVOLUME[Game.language],tbuf,&x,&y,(int*)i);
                    dme_sx = (float)dVar10;
                    DrawMenuEntry(cursor,tDONE[Game.language],&x,&y,(int*)i);
                    return;
                }
                if (paused < 0x1e) {
                    return;
                }
                //uVar3 = (uint)Game.language;
                y = (float)(-0.5 - -0.1000000014901161);
                x = PANELMENUX;
                if (Game.language == 99) {
                    c = tRELICS[3];
                    if (Game.vibration != '\0') {
                        c = tGEMS[3];
                    }
                    sprintf(tbuf,"%s:   %s",tSUPERBELLYFLOPTEXT[2][3],c);
                }
                else {
                    if (Game.vibration == '\0') {
                        txt = tOFF;
                    }
                    else {
                        txt = tON;
                    }
                    sprintf(tbuf,"%s: %s",tVIBRATION[Game.language],txt[Game.language]);
                }
                dme_sx = (float)dVar10;
                DrawMenuEntry(cursor,tbuf,&x,&y,(int*)i);
                dme_sx = (float)dVar10;
                DrawMenuEntry(cursor,tSOUNDOPTIONS[Game.language],&x,&y,(int*)i);
                if (LANGUAGEOPTION != 0) {
                    dme_sx = (float)dVar10;
                    DrawMenuEntry(cursor,tLANGUAGE[Game.language],&x,&y,(int*)i);
                }
                dme_sx = (float)dVar10;
                c = tDONE[Game.language];
                goto LAB_80036f1c;
            }
            if (iVar5 == 9) {
                if (paused < 0x1e) {
                    return;
                }
             //   txt = LanguageName;
                x = PANELMENUX;
                y = (float)(-0.5 - -0.1000000014901161);
             /*   do {
                    c = *txt;
                    txt = txt + 1;
                    DrawMenuEntry(cursor,c,&x,&y,(int*)i);
                } while ((s32)txt < *LanguageName);    */
                return;
            }
            if (iVar5 != 10) {
                return;
            }
            if (paused < 0x1e) {
                return;
            }
            y = (float)(-0.5 - -0.1000000014901161);
            x = PANELMENUX;
            if (Level == 0x25) {
                txt = tTCR_ABANDONTHISGAME;
            }
            else {
                txt = tTCR_ABANDONTHISLEVEL;
            }
            Text3D(txt[Game.language],PANELMENUX,y + 0.25,1.0,0.5,0.5,0.5,1,0);
            c = tYES[Game.language];
        }
        else {
            if (iVar5 == 0x23) {
                DrawCredits();
                return;
            }
            if (iVar5 < 0x24) {
                if (iVar5 == 0x1d) {
                    iVar7 = ParseNintendoErrorCode();
                    if (iVar7 != 0) {
LAB_80036b44:
                        BackMenu = 0x15;
                        SaveMenu = iVar5;
                        NewMenu(cursor,0x2f,0,-1);
                        return;
                    }
                    iVar7 = 1;
                    Text3D(tDELETEGAME[Game.language],0.0,0.81,(float)dVar10,0.6,0.6,0.6,1,0);
                    if (memcard_gamesavailable == 0) {
                        DrawNODATAAVAILABLE();
                        cursor->y = cursor->y_max;
                    }
                    else {
                        DrawGameSlots(cursor);
                    }
                    if (cursor->y == cursor->y_max) {
                        col = 0;
                        if (5 < GlobalTimer.frame % 0xc) {
                            col = 3;
                        }
                        iVar7 = 0x21;
                    }
                    else {
                        col = 2;
                    }
                    if (memcard_saveneeded != 0) {
                        return;
                    }
                    if (memcard_savestarted != 0) {
                        return;
                    }
                    if (memcard_savemessage_delay != 0) {
                        return;
                    }
                    c = tCANCEL[Game.language];
LAB_80036c8c:
                    Text3D(c,0.0,-0.81,1.0,0.6,0.6,0.6,iVar7,col);
                    return;
                }
                if (iVar5 < 0x1e) {
                    if (iVar5 == 0x1a) {
LAB_80034ee8:
                        VersionDisplayFlag = 1;
                        DrawNameInputTable(cursor,0.0,-0.5);
                        c = MakeEditText(Game.name);
                        Text3D(c,0.0,GAMENAMEY,1.0,1.0,1.0,1.0,1,4);
                        return;
                    }
                    if (iVar5 < 0x1b) {
                        if (iVar5 != 0x18) {
                            if (iVar5 != 0x19) {
                                return;
                            }
                            iVar7 = ParseNintendoErrorCode();
                            if (iVar7 != 0) goto LAB_80036b44;
                            iVar7 = 1;
                            Text3D(tSAVEGAME[Game.language],0.0,0.81,1.0,0.6,0.6,0.6,1,0);
                            DrawGameSlots(cursor);
                            if (cursor->y == cursor->y_max) {
                                iVar7 = 0x21;
                                col = 0;
                                if (5 < GlobalTimer.frame % 0xc) {
                                    col = 3;
                                }
                            }
                            else {
                                col = 2;
                            }
                            if (memcard_formatting != 0) {
                                return;
                            }
                            if (memcard_formatme != 0) {
                                return;
                            }
                            if (memcard_formatmessage_delay != 0) {
                                return;
                            }
                            c = tJCANCEL2;
                            if (Game.language != 99) {
                                c = tCANCEL[Game.language];
                            }
                            goto LAB_80036c8c;
                        }
                        Text3D(tLOADGAME[Game.language],0.0,0.75,1.0,0.6,0.6,0.6,1,0);
                        Text3D((char*)tMEMCARDACCESS[(uint)Game.language * 2],0.0,-0.3,(float)dVar10,0.6,
                               0.6,0.6,1,0);
                        scalex = 0.6;
                        fVar1 = 0.0;
                        c = (char*) tMEMCARDACCESS[(uint)Game.language * 2 + 1];
                        dy = -0.45;
                    }
                    else {
                        if (iVar5 == 0x1b) {
                            Text3D(tSAVEGAME[Game.language],0.0,0.75,1.0,0.6,0.6,0.6,1,0);
                            DrawGameSlot(&Game,0.0,0.25,4,0.6);
                            if (game->empty == '\0') {
                                Text3D((char*) tCONFIRMOVERWRITE[(uint)Game.language * 2],0.0,-0.3,
                                       (float)dVar10,0.6,0.6,0.6,1,0);
                                Text3D((char*) tCONFIRMOVERWRITE[(uint)Game.language * 2 + 1],0.0,-0.45,
                                       (float)dVar10,0.6,0.6,0.6,1,0);
                            }
                            else {
                                Text3D(tCONFIRMSAVE[Game.language],0.0,-0.3,(float)dVar10,0.6,0.6,
                                       0.6,1,0);
                            }
                            y = (float)(-0.699999988079071 - -0.1000000014901161);
                            DrawMenuEntry(cursor,tYES[Game.language],&x,&y,(int*)i);
                            c = tNO[Game.language];
                            goto LAB_80036da0;
                        }
                        if (iVar5 != 0x1c) {
                            return;
                        }
                        Text3D(tSAVEGAME[Game.language],0.0,0.75,1.0,0.6,0.6,0.6,1,0);
                        Text3D((char*) tMEMCARDACCESS[(uint)Game.language * 2],0.0,-0.3,(float)dVar10,0.6,
                               0.6,0.6,1,0);
                        scalex = 0.6;
                        fVar1 = 0.0;
                        c = (char*) tMEMCARDACCESS[(uint)Game.language * 2 + 1];
                        dy = -0.45;
                    }
                    goto LAB_80037ad8;
                }
                if (iVar5 == 0x20) {
                    Text3D(tFORMAT[Game.language],0.0,0.75,1.0,0.6,0.6,0.6,1,0);
                    Text3D(tCONFIRMFORMAT[Game.language],0.0,-0.3,(float)dVar10,0.6,0.6,0.6,1,0);
                    y = (float)(-0.699999988079071 - -0.1000000014901161);
                    DrawMenuEntry(cursor,tYES[Game.language],&x,&y,(int*)i);
                    txt = tNO;
LAB_80037624:
                    c = txt[Game.language];
LAB_80037638:
                    DrawMenuEntry(cursor,c,&x,&y,(int*)i);
                    return;
                }
          /*      if (0x20 < iVar5) {
                    if (iVar5 != 0x21) {
                        if (iVar5 != 0x22) {
                            return;
                        }
                        pdat = struct pdata_s + new_power;
                        if ((new_power == 0) && (Game.language == '\x01')) {
                            fVar1 = 0.55;
                        }
                        else {
                            fVar1 = 0.6;
                        }
                        dVar10 = 0.6000000238418579;
                        Text3D(struct pdata_s[new_power].name,0.0,POWERTEXTY + 0.6,1.0,fVar1,0.6,0.6,1,1);
                        if (cursor->button_lock == '\0') {
                            y = (float)((double)(float)((double)POWERTEXTY - 0.6000000238418579) - -0.1000000014901161);
                            DrawMenuEntry(cursor,tPRESSxTOCONTINUE[Game.language],&x,&y,(int*)i);
                        }
                        uVar3 = struct pdata_s[iVar7].font3d_letter - 0x61 & 0xff;
                        fVar1 = Font3DObjTab[uVar3].scale * 2.5;
                        col = 0;
                        DrawPanel3DCharacter
                                  ((uint)pdat->character,0.0,-0.8,1.0,fVar1,fVar1,fVar1,0,0,0,
                                   (s32)Font3DObjTab[uVar3].action,Font3DObjTab[uVar3].anim_time,0 );
                        while ((col < 7 &&
                               (iVar5 = strcmp((char *)struct pdata_s[iVar7].description[col],""), iVar5 !=  0
                               ))) {
                            col = col + 1;
                        }
                        //local_68 = col - 1U;
                        dVar10 = 0.5;
                        dVar8 = (double)(MENUDY * 1.2);
                        i = 0;
                        y = POWERTEXTY -
                            (float)((double)(float)(col - 1U -
                                                   (double)(variptr_u  [2])0x4330000080000000) *
                                   MENUDY * 1.2) * 0.5;
                        if (col < 1) {
                            return;
                        }
                        do {
                            Text3D((char *)struct pdata_s[iVar7].description[i],0.0,y,1.0,(float)0.5,
                                   (float)0.5,0.6,1,0);
                            i = i + 1;
                            y = (float)((double)y + MENUDY * 1.2);
                        } while (i < col);
                        return;
                    }
                    Text3D(tFORMAT[Game.language],0.0,0.75,1.0,0.6,0.6,0.6,1,0);
                    Text3D(tMEMCARDACCESS[(uint)Game.language * 2],0.0,-0.3,(float)0.5,0.6,0.6,
                           0.6,1,0);
                    scalex = 0.6;
                    fVar1 = 0.0;
                    c = tMEMCARDACCESS[(uint)Game.language * 2 + 1];
                    dy = -0.45;
                    goto LAB_80037ad8;
                }*/
        /*        if (iVar5 != 0x1e) {
                    if (iVar5 != 0x1f) {
                        return;
                    }
                    Text3D(tDELETEGAME[Game.language],0.0,0.75,1.0,0.6,0.6,0.6,1,0);
                    Text3D(tMEMCARDACCESS[(uint)Game.language * 2],0.0,-0.3,1.0,0.6,0.6,0.6,1,0);
                    scalex = 0.6;
                    fVar1 = 0.0;
                    c = tMEMCARDACCESS[(uint)Game.language * 2 + 1];
                    dy = -0.45;
                    dVar10 = 1.0;
                    goto LAB_80037ad8;
                }*/
                Text3D(tDELETEGAME[Game.language],0.0,0.75,1.0,0.6,0.6,0.6,1,0);
                DrawGameSlot(game,0.0,0.25,4,0.6);
                txt = tCONFIRMDELETE;
                goto LAB_80036d1c;
            }
          if (iVar5 == 0x2b) {
                NuRndrRectUV2di(0,0,0x280,0x1e0,0.0,0.0,1.0,1.0,-1,GBABG_mtl);
                return;
            }
            if (iVar5 < 0x2c) {
                if (iVar5 == 0x28) {
                    NuRndrRectUV2di(0,0,0x280,0x1e0,0.0,0.0,1.0,1.0,-1,GBABG_mtl);
                    Text3D((char*) tCONNECTGBA[(uint)Game.language * 3],0.0,-0.35,(float)dVar10,0.4,0.4,0.4,
                           1,0);
                    Text3D((char*) tCONNECTGBA[(uint)Game.language * 3 + 1],0.0,-0.49,(float)dVar10,0.4,0.4,
                           0.4,1,0);
                    Text3D((char*) tCONNECTGBA[(uint)Game.language * 3 + 2],0.0,-0.63,(float)dVar10,0.4,0.4,
                           0.4,1,0);
                    c = tPRESSxTOCONTINUE[Game.language];
                    fVar1 = -0.77;
LAB_800378c4:
                    Text3D(c,0.0,fVar1,(float)dVar10,0.4,0.4,0.4,1,0);
                    GBA_Draw();
                    return;
                }
                if (0x28 < iVar5) {
                  /*  if (iVar5 == 0x29) {
                        NuRndrRectUV2di(0,0,0x280,0x1e0,0.0,0.0,1.0,1.0,-1,GBABG_mtl);
                        Text3D(tCONNECTING[Game.language],0.0,-0.35,(float)dVar10,0.4,0.4,0.4,1,0) ;
                        c = tPLEASEWAIT[Game.language];
                        fVar1 = -0.49;
                    }*/
                  /*  else {
                        if (iVar5 != 0x2a) {
                            return;
                        }
                        NuRndrRectUV2di(0,0,0x280,0x1e0,0.0,0.0,1.0,1.0,-1,GBABG_mtl);
                        Text3D(tDOWNLOADING[Game.language],0.0,-0.35,1.0,0.4,0.4,0.4,1,0);
                        c = tPLEASEWAIT[Game.language];
                        fVar1 = -0.49;
                        dVar10 = 1.0;
                    }*/
                    //goto LAB_800378c4;
                }
           /*     if (iVar5 != 0x26) {
                    if (iVar5 != 0x27) {
                        return;
                    }
                    NuRndrRectUV2di(0,0,0x280,0x1e0,0.0,0.0,1.0,1.0,-1,GBABG_mtl);
                    y = -0.55;
                    DrawMenuEntry(cursor,tLOADCRASHBLAST[Game.language],&x,&y,(int*)i);
                    txt = tEXIT;
                    goto LAB_80037624;
                }*/
          /*      iVar7 = XbGetNumControllers();
                if (iVar7 == 0) {
                    Pad[0] = NULL;
                    return;
                }
                if (lost_controller != 0) {
                    return;
                }
        /*        if (((saveload_error & 0x20) == 0) && (saveload_cardchanged == 0)) {
                    if (saveload_error == 4) {
LAB_800371cc:
                        txt = tFAILEDTOSAVE;
                    }
                    else {
                        if ((s32)saveload_error < 5) {
LAB_80037218:
                            Text3D(tTCR_CONTENTPROTECTIONFAULT[Game.language],0.0,0.0,1.0,0.5,0.5,
                                   0.5,1,0);
                            goto LAB_80037260;
                        }
                        if (saveload_error == 8) {
                            txt = tFAILEDTODELETE;
                        }
                        else {
                            if (saveload_error != 0x10) goto LAB_80037218;
                            txt = tTCR_CONTENTPROTECTIONFAULT;
                        }
                    }
LAB_800371e8:
                    Text3D(txt[Game.language],0.0,0.0,1.0,0.5,0.5,0.5,1,0);
                }*/
      /*          else {
                    iVar7 = saveload_error - 0x20;
                    if (iVar7 == 4) goto LAB_800371cc;
                    if (iVar7 < 5) {
                        if (iVar7 == 2) {
LAB_80037150:
                            txt = tTCR_CONTENTPROTECTIONFAULT;
                            goto LAB_800371e8;
                        }
                    }
                    else {
                        if (iVar7 == 8) {
                            txt = tFAILEDTODELETE;
                            goto LAB_800371e8;
                        }
                        if (iVar7 == 0x10) goto LAB_80037150;
                    }
                }*/
/*LAB_80037260:
                scalex = 0.5;
                c = tPRESSxTOCONTINUE[Game.language];
                fVar1 = 0.0;
                dy = -0.15;
                dVar10 = 1.0;*/
LAB_80037ad8:
                 Text3D(c,fVar1,dy,(float)dVar10,scalex,scalex,scalex,1,0);
                return;
            }
       /*     if (iVar5 == 0x2f) {
                y = 0.2;
                for (iVar7 = 0; iVar7 < 4; iVar7 = iVar7 + 1) {
                    uVar2 = ParseNintendoErrorCode();
                    c = (char *)GetStringIdx(uVar2,iVar7);
                    if (c == (char *)0x0) break;
                    Text3D(c,0.0,y,1.0,0.4,0.4,0.4,1,0);
                    y = y - 0.1;
                }
                y = (float)(-0.6899999976158142 - MENUDY * 1.2);
                DrawMenuEntry(cursor,tMEMCARDRETRY[Game.language],&x,&y,(int*)i);
                DrawMenuEntry(cursor,tMEMCARDCONTINUE[Game.language],&x,&y,(int*)i);
                iVar7 = ParseNintendoErrorCode();
                if (iVar7 < 2) {
                    return;
                }
                if (iVar7 < 4) {
                    uVar3 = (uint)Game.language;
                    txt = tFORMAT;
                    goto LAB_80037c10;
                }
LAB_80037be8:
                if (iVar7 != 8) {
                    return;
                }
                uVar3 = (uint)Game.language;
                txt = tDELETE;
LAB_80037c10:
                DrawMenuEntry(cursor,txt[uVar3],&x,&y,(int*)i);
                cursor->y_max = '\x02';
                return;
            }*/
      /*      if (iVar5 < 0x30) {
                if (iVar5 == 0x2c) {
                    NuRndrRectUV2di(0,0,0x280,0x1e0,0.0,0.0,1.0,1.0,-1,GBABG_mtl);
                    Text3D(tGAMEINST[(uint)Game.language * 3],0.0,-0.35,(float)dVar10,0.4,0.4,0.4, 1,
                           0);
                    Text3D(tGAMEINST[(uint)Game.language * 3 + 1],0.0,-0.49,(float)dVar10,0.4,0.4,
                           0.4,1,0);
                    Text3D(tGAMEINST[(uint)Game.language * 3 + 2],0.0,-0.63,(float)dVar10,0.4,0.4,
                           0.4,1,0);
                    scalex = 0.4;
                    c = tPRESSxTOCONTINUE[Game.language];
                    fVar1 = 0.0;
                    dy = -0.77;
                }
                else {
                    if (iVar5 != 0x2d) {
                        return;
                    }
                    NuRndrRectUV2di(0,0,0x280,0x1e0,0.0,0.0,1.0,1.0,-1,GBABG_mtl);
                    Text3D(tCANTDOWNLOAD[Game.language],0.0,-0.35,1.0,0.4,0.4,0.4,1,0);
                    scalex = 0.4;
                    c = tPRESSxTOCONTINUE[Game.language];
                    fVar1 = 0.0;
                    dy = -0.77;
                    dVar10 = 1.0;
                }
                goto LAB_80037ad8;
            }*/
       /*     if (iVar5 != 0x30) {
                if (iVar5 != 0x31) {
                    return;
                }
                Text3D(tDELETEGAME[Game.language],0.0,0.75,1.0,0.6,0.6,0.6,1,0);
                Text3D(tMEMCARDACCESS[(uint)Game.language * 2],0.0,-0.3,1.0,0.6,0.6,0.6,1,0);
                scalex = 0.6;
                fVar1 = 0.0;
                c = tMEMCARDACCESS[(uint)Game.language * 2 + 1];
                dy = -0.45;
                dVar10 = 1.0;
                goto LAB_80037ad8;
            }
            Text3D(tCONFIRMDELETE[Game.language],0.0,-0.3,1.0,0.6,0.6,0.6,1,0);
            c = tYES[Game.language];
            y = (float)(-0.699999988079071 - MENUDY * 1.2);*/
        }
    //    DrawMenuEntry(cursor,c,&x,&y,(int*)i);
      //  c = tNO[Game.language];
LAB_80036f1c:
     //   DrawMenuEntry(cursor,c,&x,&y,(int*)i);
        return;
    }
/*    Text3D(tLOADGAME[Game.language],0.0,0.75,1.0,0.6,0.6,0.6,1,0);
    DrawGameSlot(game,0.0,0.25,4,0.6);
    txt = tCONFIRMLOAD;    */
LAB_80036d1c:
 /*   Text3D(txt[Game.language],0.0,-0.3,(float)dVar10,0.6,0.6,0.6,1,0);
    y = (float)(-0.699999988079071 - MENUDY * 1.2);
    DrawMenuEntry(cursor,tYES[Game.language],&x,&y,(int*)i);
    c = tNO[Game.language];    */
LAB_80036da0:
    //DrawMenuEntry(cursor,c,&x,&y,(int*)i);
  //  return;
}

//99% NGC
void GameTiming(void) {
  s32 i;
  s32 tmp;
  
  if (pause_dir == 1) {
    Paused = Paused + 1;
    if (Paused == 0x1e) {
      pause_dir = 0;
    }
  }
 else if (pause_dir == 2) {
    Paused--;
    if (Paused == 0) {
      pause_dir = Paused;
      NewMenu(&Cursor,-1,-1,-1);
      ResumeGame();
    }
}
if  (Paused != 0) {
  if (Paused == 0x1e) {
    UpdateTimer(&PauseTimer);
    pausestats_frame++;
  }
} else{
      UpdateTimer(&GameTimer);
      if (force_panel_wumpa_update != 0) {
        force_panel_wumpa_update--;
      }
      if (force_panel_crate_update != 0) {
        force_panel_crate_update--;
      }
      if (force_panel_lives_update != 0) {
        force_panel_lives_update--;
      }
      if (force_panel_items_update != 0) {
        force_panel_items_update--;
      }
      if (((Pad[0] != NULL) && ((Pad[0]->rdy & 0x10) != 0)) && (GameMode != 1)) {
        force_panel_wumpa_update = 0x3c;
        if ((TimeTrial == 0) && (LDATA->hub != -1)) {
          force_panel_crate_update = 0x3c;
        }
        force_panel_lives_update = 0x3c;
        force_panel_items_update = 0x3c;
      }
      UpdatePanelItem(&plr_wumpas,force_panel_wumpa_update,1);
      UpdatePanelItem(&plr_crates,force_panel_crate_update,0);
      UpdatePanelItem(&plr_lives,force_panel_lives_update,1);
      UpdatePanelItem(&plr_crystal,force_panel_items_update,1);
      UpdatePanelItem(&plr_crategem,force_panel_items_update,1);
      UpdatePanelItem(&plr_bonusgem,force_panel_items_update,1);
      if ((check_time < check_duration) && (check_time+= 0.01666667f, check_time >= check_duration)) {
        check_time = check_duration;
        point_time = 0.0f;
      }
      if ((point_time < point_duration) &&
         (point_time += 0.01666667f, point_time >= point_duration)) {
        point_time = point_duration;
      }
      if ((0.0f < start_time) && (start_time-= 0.01666667f, start_time < 0.0f)) {
        start_time = 0.0f;
      }
      if (TimeTrial != 0) {
        if (TimeTrialWait > 0.0f) {
          if (timetrial_frame == (timetrial_frame / 0xf) * 0xf) {
            GameSfx(0x4e,NULL);
          }
          TimeTrialWait-= 0.01666667f;
          if (TimeTrialWait < 0.0f) {
            TimeTrialWait = 0.0f;
          }
        }
        else {
          if (((player->obj).finished == '\0') && (FlyingLevelExtro == 0)) {
            UpdateTimer(&TimeTrialTimer);
          }
        }
        timetrial_frame++;
      }
      if (Bonus == 2) {
        bonus_frame++;
      }
      PANELWUMPAYROT = PANELWUMPAYROT - 0x666;
      panel_head_xrot = 0;
      PANELCRATEYROT = PANELCRATEYROT - 0x222;
      panel_head_yrot = 0;
      if ((Level != 0x1a && GameTimer.frame == 0xb4) || (Level == 0x1a && GameTimer.frame == 0x1e)) {
            i = -1;
            if ((Level == 0xd) || (Level == 0x1a)) {
              i = 0xb3;
            }
            if (i != -1) {
              gamesfx_channel = -1;
              GameSfx(i,NULL);
            }
      }
      tmp = 0;
      if ((TimeTrial != 0) && ((player->obj).finished == '\x01')) {
        tmp = 0x2c - (finish_frame % 0x3c); //(s32)(finish_frame % 0x3c < 0x2d);
      }
        tt_flash = tmp;
      if (advice_wait != 0) {
        advice_wait--;
      }
    }
  UpdateTimer(&MenuTimer);
  UpdateTimer(&GlobalTimer);
  GameAudioUpdate();
  return;
}

//NGC MATCH
s32 PlayerObjectAnimCollision(struct obj_s *obj,struct nuspecial_s *special,float radius) {
  float r;
  float dx;
  float dz;
  struct nuvec_s pos;
  
  if ((((obj != NULL) && (special != NULL)) && (special->instance != NULL)) &&
     ((special->instance->flags.visible < 0 && (obj->dead == '\0')))) {
    if (special->instance->anim != NULL) {
      pos.x = (special->instance->anim->mtx)._30;
      pos.y = (special->instance->anim->mtx)._31;
      pos.z = (special->instance->anim->mtx)._32;
    }
    else {
      pos.x = (special->instance->mtx)._30;
      pos.y = (special->instance->mtx)._31;
      pos.z = (special->instance->mtx)._32;
    }
    if (!(obj->objbot > (pos.y + radius)) && !(obj->objtop < pos.y - radius)) {
      dx = pos.x - (obj->pos).x;
      dz = pos.z -(obj->pos).z;
      r = obj->RADIUS + radius;
      if (dx * dx + dz * dz > r * r) {
        return 0;
      }
      return 1;
    }
  }
  return 0;
}

//98% NGC
void BonusTiming (struct creature_s* plr) {
    s32 dead;

    dead = 0;
    if ((Bonus == 2) && ((plr->obj).dead != 0)) {
        dead = 1;
    }
    if (((Bonus != 3) && (Bonus != 4)) && (!dead)) {
        return;
    }
    bonus_finish_frame++;
    if (bonus_wumpa_delay != 0) {
        bonus_wumpa_delay--;
        if (plr_bonus_wumpas.count == 0) {
            bonus_wumpa_wait = 0.5f;
        } else if (bonus_wumpa_wait > 0.0f) {
            bonus_wumpa_wait -= 0.01666667f;
            if (bonus_wumpa_wait < 0.0f) {
                bonus_wumpa_wait = 0.0f;
            }
        }
    } else {
        if (plr_bonus_wumpas.count != 0) {
            bonus_wumpa_delay = 6;
            if (dead == 0) {
                plr_wumpas.count++;
                AddPanelDebris(BONUSWUMPAOBJSX, BONUSPANELSY, 0, 1.0f, 1);
            } else {
                AddPanelDebris(BONUSWUMPAOBJSX, BONUSPANELSY, 1, 1.0f, 1);
                GameSfx(0x19, NULL);
            }
            plr_bonus_wumpas.count--;
        }
        if (plr_bonus_wumpas.count == 0) {
            bonus_wumpa_wait = 0.5f;
        } else if (bonus_wumpa_wait > 0.0f) { // this isn't correct?
            bonus_wumpa_wait -= 0.01666667f;
            if (bonus_wumpa_wait < 0.0f) {
                bonus_wumpa_wait = 0.0f;
            }
        }
    }
    plr_bonus_wumpas.draw = plr_bonus_wumpas.count;
    if (bonus_life_delay != 0) {
        bonus_life_delay--;
        if (bonus_lives == 0) {
            bonus_lives_wait = 0.5f;
        }
    } else if (bonus_lives != 0) {
        bonus_life_delay = 90;
        if (!dead) {
            AddPanelDebris(BONUSLIVESOBJSX, BONUSPANELSY, 4, BONUSLIFESCALE, 1);
        }
        bonus_lives--;
        if (bonus_lives == 0) {
            bonus_lives_wait = 0.5f;
        }
    } else if (bonus_lives == 0 && bonus_lives_wait > 0.0f) {
        bonus_lives_wait -= 0.01666667f;
        if (bonus_lives_wait < 0.0f) {
            bonus_lives_wait = 0.0f;
        }
    }
    return;
}

//NGC MATCH
void CalculateGamePercentage(struct game_s *game) {
  s32 hub;
  s32 i;
  
  game->percent = 0;
  game->crystals = 0;
  game->relics = 0;
  game->crate_gems = 0;
  game->bonus_gems = 0;
  game->gems = 0;
  game->gembits = 0;
  sapphire_relics = gold_relics = platinum_relics = 0;
  for(i = 0; i < 6; i++) {
    game->hub[i].crystals = 0;
  }
  for(i = 0; i < 0x23; i++) {
    hub = HubFromLevel(i);
    if ((LData[i].flags & 2) != 0) {
         if ((game->level[i].flags & 0x800) != 0) {
          game->percent++;
        }
    } else { 
            if ((game->level[i].flags & 8) != 0) {
            game->percent++;
            game->crystals = game->crystals + 1;
            game->hub[hub].crystals++;
          }
          if ((game->level[i].flags & 7) != 0) {
            game->percent++;
            game->relics = game->relics + 1;
            if ((game->level[i].flags & 4) != 0) {
              platinum_relics++;
    
            }
            else {
              if ((game->level[i].flags & 2) != 0) {
                gold_relics++;
              }
              else {
                sapphire_relics++;
              }
            }
          }
            if ((game->level[i].flags & 0x10) != 0) {
            game->percent++;
            game->crate_gems++;
            game->gems++;
          }
           if ((game->level[i].flags & 0x20) != 0) {
            game->percent++;
            game->bonus_gems++;
            game->gems++;
          }
          else if ((game->level[i].flags & 0x40) != 0) {
              game->percent++;
              game->gembits |= 1;
              game->gems++;
            }
            else if ((game->level[i].flags & 0x80) != 0) {
                game->percent++;
                game->gembits |= 2;
                game->gems++;
              }
              else if ((game->level[i].flags & 0x100) != 0) {
                    game->percent++;
                    game->gembits |= 4;
                    game->gems++;
                }
                else if ((game->level[i].flags & 0x200) != 0) {
                    game->percent++;
                    game->gembits |= 8;
                    game->gems++;
                  }
                  else if ((game->level[i].flags & 0x400) != 0){
                    game->percent++;
                    game->gembits |= 0x10;
                    game->gems++;
                  }
        }
  }
}


//NGC MATCH
void MakeTimeI(s32 time,s32 hours,char *txt) {
  s32 t;
  
  if (time < 0) {
    time = 0;
  }
  t = time / 3;
  if (hours != 0) {
    temp_hours = t / 360000;
    temp_minutes = t / 6000 - temp_hours * 0x3c;
  }
  else {
    temp_minutes = t / 6000;

  }
  temp_hundredths = t % 100;
  temp_tenths = temp_hundredths / 10;
  temp_seconds = (t / 100) % 0x3c;
  if ((txt != NULL) && (sprintf(txt,"%i:%c%c.%c%c",temp_minutes,temp_seconds / 10 + 0x30,temp_seconds % 10 + 0x30,
              temp_tenths + 0x30,temp_hundredths % 10 + 0x30), Game.language == 'c')) {
    AddSpacesIntoText(txt,1);
  }
  return;
}

//NGC MATCH
void MakeLevelTimeString(struct time_s *time,char *txt) {
  char *__format;
  char time_string [64];
  
  MakeTimeI(time->itime,0,time_string);
  if (Game.language == 'c') {
    __format = "%s  %s";
  }
  else {
    __format = "%s %s";
  }
  sprintf(txt,__format,time,time_string);
  return;
}

//NGC MATCH
void ResetTimer(struct GTimer *t) {
  t->frame = 0;
  t->itime = 0;
  t->isec = 0;
  t->ftime = 0.0f;
  t->fsec = 0.0f;
  return;
}

//NGC MATCH
void UpdateTimer(struct GTimer* t) {
  t->frame++;
  t->itime+= 5;
  t->isec+= 5;
  if (299 < t->isec) {
    t->isec-= 0x12C;
  }
  t->ftime+= 0.01666667f;
  t->fsec+= 0.01666667f;
  if (t->fsec >= 1.0f) {
    t->fsec-= 1.0f;
  }
  return;
}

//NGC MATCH
void StartTimeTrial(struct nuvec_s *pos, s32 off) {
  TimeTrial = 1;
  ResetCheckpoint(-1, -1, 0.0f, NULL);
  GameSfx(0x4c, NULL);
  if (off != 0) {
    ClockOff();
  }
  NuCameraTransformScreenClip(&TTScrPos, pos, 1, NULL);
  TTScrPos.z = (1.0f - TTScrPos.z) * (TEXTZMUL / 0.1f);
  if (ObjTab[101].obj.special != NULL) {
    (ObjTab[101].obj.special)->instance->flags.visible = 1 | 0x80000000;
  }
  if (Level == 0x1d) {
    tt_sx = -0.1f;
  }
  else if (((LDATA->flags & 0x200) != 0) || (Level == 0x1c)) {
    tt_sx = 0.0f;
  }
  else {
    tt_sx = 0.49f;
  }
  tt_sy = -0.8f;
  return;
}

//NGC MATCH
void ResetTimeTrial(void) {
s32 tmp;
    
  TimeTrial = 0;
  ResetTimer(&TimeTrialTimer);
  timetrial_frame = 0;
  tmp = 0;
  TimeTrialWait = 0;
  if (PLAYERCOUNT != 0) {
   // clock_ok = plr_items ? 0 : 1;
    tmp = !plr_items; //??
  } else{
      tmp = 0;
  }
    clock_ok = tmp;
  return;
}

//NGC MATCH
void ClockOff(void) {
  s32 i;
  
  for(i = 1; i < 9; i++) {
    if ((Character[i].on != '\0') && ((Character[i].obj).character == 0x76)) {
      KillItem(&Character[i].obj);
    }
  }
  clock_ok = 0;
  return;
}

//NGC MATCH
struct creature_s * FindClock(void) {
  s32 i;

  for(i = 1; i < 9; i++) {
      if(Character[i].on != '\0' && ((Character[i].obj).character == 0x76)){
          return &Character[i];
      }
  }
  return NULL;
}

//NGC MATCH
void InitSplineTable(void) {
  s32 i;
  
  if (world_scene[0] != NULL) {
    for (i = 0; i < 0x49; i++) {
      SplTab[i].spl = NULL;
      if (((SplTab[i].levbits >> Level) & 1) != 0) {
        SplTab[i].spl = NuSplineFind(world_scene[0],SplTab[i].name);
        if (SplTab[i].spl != NULL) {
          if ((SplTab[i].min > 0) && (SplTab[i].spl->len < SplTab[i].min)) {
            SplTab[i].spl = NULL;
          }
          else {
            if ((SplTab[i].max > 0) && ((SplTab[i].max >= SplTab[i].min  && (SplTab[i].spl->len > SplTab[i].max)))) {
              SplTab[i].spl = 0;
            }
          }
        }
      }
    }
  }
  else {
    for (i = 0; i < 0x49; i++) {
      SplTab[i].spl = NULL;
    }
  }
  return;
}

//NGC MATCH
void InitObjectTable(void) {
  //struct nugscn_s* scene;
  s32 i;
  s32 j;

  for (i = 0; i < 0xc9; i++) {
    ObjTab[i].obj.special = NULL;
    if ((((ObjTab[i].levbits >> Level) & 1) != 0) && (*ObjTab[i].scene != NULL)) {
      if (NuSpecialFind(*ObjTab[i].scene,&ObjTab[i].obj,ObjTab[i].name) != 0) {
        (ObjTab[i].obj.special)->instance->flags.visible = ObjTab[i].visible;
        for (j = 0; j < 0x1a; j++) {
          if ((((Font3DObjTab[j].flags) & 2) != 0) && ((Font3DObjTab[j].i) == i)) {
            ObjTab[i].font3d_letter = (char)j + 0x61;
            j = 0x1a;
          }
        }
      }
    }
  }
  return;
}

//NGC MATCH
float DistanceToLine(struct nuvec_s* pos, struct nuvec_s* p0, struct nuvec_s* p1) {
  s32 a;
  
  a = NuAtan2D(p1->x - p0->x,p1->z - p0->z);
  return NuFabs(((pos->x - p0->x) * (NuTrigTable[(((-a & 0xffff) + 0x4000) & 0x3fffc  / 4)]) +
                                 (pos->z - p0->z) * NuTrigTable[-a & 0xffff]));
}

//NGC MATCH
s32 LineCrossed(float xold,float zold,float xnew,float znew,float x0,float z0,float x1,float z1) {

s32 rv;

  if (!(0.0f <= (xnew - x0) * (z1 - z0) + (znew - z0) * (x0 - x1)) &&
     ((xold - x0) * (z1 - z0) + (zold - z0) * (x0 - x1) >= 0.0f)) {
      rv = 1;
    if (!((xnew - xold) * (z0 - zold) + (znew - zold) * (xold - x0) >= 0.0f && ((xnew - x1) * (zold - z1) + (znew - z1) * (x1 - xold) >= 0.0f))) {
    }else {
        rv = 2;
    }
      return rv;
  }
   else 
  return 0;
}

//NGC MATCH
float RatioBetweenEdges(struct nuvec_s *pos,struct nuvec_s *pL0,struct nuvec_s *pL1,struct nuvec_s *pR0,struct nuvec_s *pR1) {
  float dL;
  float dR;
  
  dL = DistanceToLine(pos,pL0,pL1);
  dR = DistanceToLine(pos,pR0,pR1);
  return (dL / (dL + dR));
}

//NGC MATCH
float RatioAlongLine(struct nuvec_s* pos, struct nuvec_s* p0, struct nuvec_s* p1) {
    float z;
    float z1;
    float dx;
    float dz;
    float siny;
    float cosy;
    u16 yrot;

    dx = p1->x - p0->x;
    dz = p1->z - p0->z;
    yrot = -NuAtan2D(dx, dz);
    siny = NuTrigTable[yrot];
    cosy = NuTrigTable[(yrot + 0x4000) & 0xffff];
    z = -((pos->x - p0->x) * siny) + ((pos->z - p0->z) * cosy);
    if (z <= 0.0f) {
        return 0.0f;
    }
    z1 = -(dx * siny) + (dz * cosy);
    if (z >= z1) {
        return 1.0f;
    } else {
        return z / z1;
    }
}

//NGC MATCH
u16 SplinePointAngle(struct nugspline_s *spl,int i) {
  struct nuvec_s *p0;
  struct nuvec_s *p1;
  float dx;
  float dz;
    
  p0 = (struct nuvec_s *)(spl->pts + i * spl->ptsize);
  dx = dz = 0.0f;
  if (i > 0) {
    p1 = (struct nuvec_s *)(spl->pts + (i - 1) * spl->ptsize);
    dx = (p0->x - p1->x) + 0.0f;
    dz = (p0->z - p1->z) + 0.0f;
  }
  if (i < spl->len - 1) {
    p1 = (struct nuvec_s *)(spl->pts + (i + 1) * spl->ptsize);
    dx = dx + (p1->x - p0->x);
    dz = dz + (p1->z - p0->z);
  }
  return (u16)NuAtan2D(dx,dz);
}

//NGC MATCH
u16 SplinePointTilt(struct nugspline_s *spl,s32 i) {
  struct nuvec_s *p0;
  struct nuvec_s *p1;
  float dx;
  float dz;
  float dy;
  float d;
  
    dx = 0.0f;
    dy = 0.0f;
    dz = 0.0f;
  p0 = (struct nuvec_s *)(spl->pts + (i * spl->ptsize));
  if (i > 0) {
    p1 = (struct nuvec_s *)(spl->pts + ((i - 1) * spl->ptsize));
    dx = (p0->x - p1->x) + 0.0f;
    dy = (p0->y - p1->y) + 0.0f;
    dz = (p0->z - p1->z) + 0.0f;
  }
  if (i < spl->len - 1) {
      p1 = (struct nuvec_s *)(spl->pts + ((i + 1) * spl->ptsize));
    dx +=  p1->x - p0->x;
    dy +=  p1->y - p0->y;
    dz +=  p1->z - p0->z;
  }
  d = NuFsqrt((dx * dx + (dz * dz)));
  return (u16)NuAtan2D(dy,d);
}

//NGC MATCH
float SplineDistance(struct nugspline_s *spl) {
  struct nuvec_s *p0;
  struct nuvec_s *p1;
  struct nuvec_s v;
  s32 i;
  float d;
  
  if ((spl == NULL) || (spl->len < 2)) {
    return 0.0f;
  }
  else {
    p0 = (struct nuvec_s *)spl->pts;
    d = 0.0f;
    for (i = 0; i < (spl->len - 1); i++) {
        p1 = (struct nuvec_s *)(spl->pts + ((i + 1) * spl->ptsize));
        NuVecSub(&v,p1,p0);
        d += NuVecMag(&v);
        p0 = p1;
    }
  }
  return d;
}

//NGC MATCH
s32 NearestSplinePoint(struct nuvec_s *pos,struct nugspline_s *spl) {
  s32 index;
  s32 i;
  s32 d;
  s32 d0;
  
  index = -1;
  if ((spl != NULL)) {
    for (i = 0; i < spl->len; i++) {
      d = NuVecDistSqr(pos,(spl->pts + i * spl->ptsize),NULL);
      if ((index == -1) || (d < d0)) {
        index = i;
        d0 = d;
      }
    }
  }
  return index;
}

//NGC MATCH
void PointAlongSpline(struct nugspline_s *spl,float ratio,struct nuvec_s *dst,u16 *angle,u16 *tilt) {
  struct nuvec_s *p0;
  struct nuvec_s *p1;
  u32 i;
  u32 j;
  u32 l;
  u16 a;
  float f;
  s32 d;
  
  if (angle != NULL) {
    *angle = 0;
  }
  if (tilt != NULL) {
    *tilt = 0;
  }
  if (spl != NULL) {
    if ((1.0f < ratio)) {
        ratio = 1.0f;
    } else if (ratio < 0.0f) {
        ratio = 0.0f;
    }
    l = (s32)(ratio * (float)((u32)(spl->len + -1) * 0x10000));
    i = l >> 0x10;
    iTEMP = i;
    p0 = (struct nuvec_s *)(spl->pts + i * (int)spl->ptsize);
    *dst = *p0;
    if (angle != NULL) {
      a = SplinePointAngle(spl,i);
      *angle = a;
    }
    if (tilt != NULL) {
      a = SplinePointTilt(spl,i);
      *tilt = a;
    }
    if ((i < spl->len - 1U) && (l &= 0xffff, l != 0)) {
      j = i + 1;
      p1 = (struct nuvec_s *)(spl->pts + j * spl->ptsize);
      f = (float)l * 0.000015258789f;
      dst->x = (((p1->x) - p0->x) * f + dst->x);
      dst->y = (((p1->y) - p0->y) * f + dst->y);
      dst->z = (((p1->z) - p0->z) * f + dst->z);
      if (((angle == NULL) & 1) == 0) {
        a = SplinePointAngle(spl,j);
        d = RotDiff(*angle,a);
        *angle += (u16)(f * d);
      }
      if (tilt != NULL) {
        a = SplinePointTilt(spl,j);
        d = RotDiff(*tilt,a);
        *tilt += (u16)(f * d);
      }
    }
  }
  return;
}

//NGC MATCH
void DrawParallax(void) {
  static struct numtx_s m_513;
  struct nuvec_s s;
  
  s.x = s.y = s.z = pNuCam->farclip * 0.1f * 0.5f;
  NuMtxSetScale(&m_513,&s);
  m_513._30 = (pCam->m)._30;
  m_513._31 = (pCam->m)._31;
  m_513._32 = (pCam->m)._32;
  if (ObjTab[9].obj.special != NULL) {
    NuRndrGScnObj((ObjTab[9].obj.scene)->gobjs[(ObjTab[9].obj.special)->instance->objid],&m_513);
  }
  if (ObjTab[10].obj.special != NULL) {
    NuRndrGScnObj((ObjTab[10].obj.scene)->gobjs[(ObjTab[10].obj.special)->instance->objid],&m_513);
  }
  return;
}

//NGC MATCH
void DrawTarget(void) {
  struct anim_s* anim;
  struct nuvec_s v;
  s32 i;

  if (player->target == '\0') {
      return;
  }

  if (player->target_wait != '\0') {
      return;
  }
    
  if ((player->fire_lock == '\0')
      && (player->obj).dead == '\0' && ((player->obj).finished == '\0') &&
     (boss_dead == 0 && ((player->fire == '\0' && (0 < plr_target_frame))))) {
         anim = &(player->obj).anim;
        if ((VEHICLECONTROL != 1) || (((player->obj).vehicle != 0x44 && ((player->obj).vehicle != 0xb2)))) {
          if (anim->blend != '\0') {
            if ((anim->blend_src_action != 0x4e) && (anim->blend_src_action != 0x50)) {
              return;
            }
                if ((anim->blend_dst_action != 0x4e) && (anim->blend_dst_action != 0x50)) {
                    return;
                }
          }
          else if ((anim->action != 0x4e) && (anim->action != 0x50)) {
                return;
          }
        }
        NuVecSub(&v,plr_target_pos + 1,&plr_target_sightpos);
        Draw3DObject(0x94,&plr_target_sightpos,(u16)NuAtan2D(v.y, NuFsqrt(v.x * v.x + v.z * v.z)),
                        (short)NuAtan2D(v.x,v.z) + 0x8000,0,1.0f,1.0f,
                     NuVecMag(&v),ObjTab[148].obj.scene,ObjTab[148].obj.special,0);
        i = 0x82;
        if (plr_target_found != 0) {
          i = 0x83;
        }
        Draw3DObject(i,plr_target_pos + 1,-(player->obj).target_xrot,
                 (player->obj).hdg + (player->obj).target_yrot,0,1.0f,1.0f,1.0f,ObjTab[i].obj.scene,ObjTab[i].obj.special,0);
  }
      return;
}

//95% NGC
void DrawProbeFX(struct obj_s* obj) {
    s32 ang;
    struct nuvec_s vec;
    struct nuvec_s vec1;
    struct nuvec_s vec2;
    s32 loop;

    if (Paused == 0) {
        vec = obj->pos;
        AddVariableShotDebrisEffect(GDeb[76].i, &vec, 1, -0x8000, 0);
    }
    
    ang = obj->hdg + 3000;
    for (loop = 0; loop < 3; loop++) {
        vec = obj->pos;
        vec2.x = (NuTrigTable[ang & 0xffff] * 0.48f);
        vec2.y = 0.0f;
        vec2.z = (NuTrigTable[(ang + 0x4000) & 0xffff] * 0.48f);
        
        vec.x += vec2.x;
        vec.z += vec2.z;

        
        vec1.x = vec.x + vec2.x;
        vec1.y = vec.y - 1.8f;
        vec.y -= 1.0f;
        vec1.z = vec.z + vec2.z;
        ang += 0x5555;
        
        if (Paused == 0) {
            NuLgtArcLaser(0, &vec1, &vec, &vec2, 0.1f, 0.1f, 0.01f, 0.3f, 0x80FF7F3F);
            NuLgtArcLaser(0, &vec1, &vec, &vec2, 0.4f, 0.3f, 0.001f, 0.3f, 0x80800040);
            AddVariableShotDebrisEffect(GDeb[78].i, &vec1, 1, 0, 0);
            AddVariableShotDebrisEffect(GDeb[78].i, &vec, 1, -0x8000, 0);
        }
    }
    return;
}

//NGC MATCH
s32 RayIntersectSphere(struct nuvec_s *p0,struct nuvec_s *p1,struct nuvec_s *pos,float radius) {
  struct nuvec_s ray;
  struct nuvec_s v;
  struct nuvec_s local_60;
  struct nuvec_s local_70;
  float ratio;
  float dp0;
  float dp1;

  ratio = 1.0f;
    local_60.x = (p0->x < p1->x) ? p0->x : p1->x;
  if (!((pos->x + radius) < local_60.x)) {
    local_60.y = (p0->y < p1->y) ? p0->y : p1->y;
    if (!((pos->y + radius) < local_60.y)) {
        local_60.z = (p0->z < p1->z) ? p0->z : p1->z;
      if (!((pos->z + radius) < local_60.z)) {
          local_70.x = (p0->x > p1->x) ? p0->x : p1->x;
        if (!((pos->x - radius) > local_70.x)) {
            local_70.y = (p0->y > p1->y) ? p0->y : p1->y;
          if (!((pos->y - radius) > local_70.y)) {
              local_70.z = (p0->z > p1->z) ? p0->z : p1->z;
            if (!((pos->z - radius) > local_70.z)) {
              NuVecSub(&ray,p1,p0);
              NuVecSub(&v,pos,p0);
              dp0 = NuVecDot(&ray,&v);
              dp1 = NuVecDot(&ray,&ray);
              if ((0.0f <= dp0) && (dp0 <= dp1)) {
                dp0 = dp0 / dp1;
                v.x = ((p1->x - p0->x) * dp0 + p0->x);
                v.y = ((p1->y - p0->y) * dp0 + p0->y);
                v.z = ((p1->z - p0->z) * dp0 + p0->z);
                NuVecSub(&v,pos,&v);
                if (v.x * v.x + v.y * v.y + v.z * v.z < (radius * radius)) {
                  ratio = dp0;
                }
              }
              temp_ratio = ratio;
            }
          }
        }
      }
    }
  }
  return (ratio < 1.0f);
}

//NGC MATCH
void GameRayCast(struct nuvec_s *src,struct nuvec_s *dir,float d,struct nuvec_s *dst) {
  struct nuvec_s end;
  struct nuvec_s v;
  float ratio;
  float length;
  
  ratio = 1.0f;
  end.x = dir->x * d + src->x;
  end.y = dir->y * d + src->y;
  end.z = dir->z * d + src->z;
  length = NuVecDist(&end,src,NULL);
  if ((RayIntersectSphere(src,&end,&GameCam->pos,ratio) != 0) && (temp_ratio < ratio)) {
    temp_ratio -= (ratio / length);
    if (0.0f <= temp_ratio) {
      plr_target_found = 0;
      ratio = temp_ratio;
    }
  }
  if ((CrateRayCast(src,&end) != 0) && (temp_ratio < ratio)) {
    plr_target_found = 1;
    ratio = temp_ratio;
  }
  if ((CreatureRayCast(src,&end) != 0) && (temp_ratio < ratio)) {
    plr_target_found = 1;
    ratio = temp_ratio;
  }
  if ((WumpaRayCast(src,&end,ratio) != 0) && (temp_ratio < ratio)) {
    temp_ratio = (temp_ratio - (0.1f / length));
    if (0.0f <= temp_ratio) {
      plr_target_found = 1;
      ratio = temp_ratio;
    }
  }
  if (1.0f > ratio) {
    dst->x = (end.x - src->x) * ratio + src->x;
    dst->y = (end.y - src->y) * ratio + src->y;
    dst->z = (end.z - src->z) * ratio + src->z;
  }
  else {
    *dst = end;
  }
  NuVecSub(&v,dst,src);
  if (NewRayCast(src,&v,0.0f) != 0) {
    NuVecAdd(plr_target_pos + 1,plr_target_pos,&v);
    plr_target_found = 0;
  }
  return;
}

//NGC MATCH
s32 WumpaRayCast(struct nuvec_s* p0, struct nuvec_s* p1, float ratio0) {
    struct wumpa_s* wmp;
    struct nuvec_s min;
    struct nuvec_s max;
    struct nuvec_s ray;
    struct nuvec_s v;
    float ratio;
    float dp_ray;
    float dp0;
    float dp;
    float r;
    float r2;
    s32 i;

    ratio = ratio0;
    
    min.x = (p0->x < p1->x) ? p0->x : p1->x;
    min.y = (p0->y < p1->y) ? p0->y : p1->y;
    min.z = (p0->z < p1->z) ? p0->z : p1->z;

    max.x = (p0->x > p1->x) ? p0->x : p1->x;
    max.y = (p0->y > p1->y) ? p0->y : p1->y;
    max.z = (p0->z > p1->z) ? p0->z : p1->z;

    NuVecSub(&ray, p1, p0);

    dp = NuVecDot(&ray, &ray);
    dp_ray = dp;
    
    wmp = Wumpa;
    
    r = 0.2f;
    r2 = r * r;
    
    for (i = 0; i < 0x140; i++, wmp++) {
        if ((wmp->active == 2) && (wmp->in_range != 0)) {
            if (((wmp->pos.x + r) < min.x) || ((wmp->pos.z + r) < min.z)) {
                continue;
            }
            if (((wmp->pos.x - r) > max.x) || ((wmp->pos.z - r) > max.z)) {
                continue;
            }
            if (((wmp->pos.y + r) < min.y) || ((wmp->pos.y - r) > max.y)) {
                continue;
            }
            NuVecSub(&v, &wmp->pos, p0);
            dp = NuVecDot(&ray, &v);
            if ((dp >= 0.0f) && (dp <= dp_ray)) {
                dp0 = (dp / dp_ray);
                if (dp0 < ratio) {
                    v.x = ((p1->x - p0->x) * dp0 + p0->x);
                    v.y = ((p1->y - p0->y) * dp0 + p0->y);
                    v.z = ((p1->z - p0->z) * dp0 + p0->z);
                    NuVecSub(&v, &wmp->pos, &v);
                    if ((v.x * v.x + v.y * v.y + v.z * v.z) < r2) {
                        ratio = dp0;
                    }
                }
            }
        }
    }
    temp_ratio = ratio;
    return ratio < 1.0f;
}

//NGC MATCH
s32 InLoadSaveZone(struct creature_s *plr) {
  float x;
  float z;
  
  if ((((plr->obj).ground != '\0') && ((plr->obj).idle_gametime > 0.0f)) && (NuVecDistSqr(&(plr->obj).pos,&loadsavepos,NULL) < 9.0f)) {
    x = (plr->obj).pos.x;
    z = (plr->obj).pos.z;
    if (((0.0f <= ((x - vLSZ[0].x) * (vLSZ[1].z - vLSZ[0].z) + (z - vLSZ[0].z) * (vLSZ[0].x - vLSZ[1].x))) &&
        (0.0f <= ((x - vLSZ[1].x) * (vLSZ[2].z - vLSZ[1].z) + (z - vLSZ[1].z) * (vLSZ[1].x - vLSZ[2].x)))) &&
       ((0.0f <= ((x - vLSZ[2].x) * (vLSZ[3].z - vLSZ[2].z) + (z - vLSZ[2].z) * (vLSZ[2].x - vLSZ[3].x)) &&
        (0.0f <= ((x - vLSZ[3].x) * (vLSZ[0].z - vLSZ[3].z) + (z - vLSZ[3].z) * (vLSZ[3].x - vLSZ[0].x)))))) {
      return 1;
    }
  }
  return 0;
}
