void DrawMenu(Cursor *cursor,int paused)

{
  float fVar1;
  float scalex;
  undefined4 uVar2;
  nupad_s *pnVar3;
  int col;
  uint uVar4;
  undefined4 uVar5;
  char **txt;
  int iVar6;
  int unaff_r27;
  PData *pdat;
  int iVar7;
  undefined1 *puVar8;
  double dVar9;
  double dVar10;
  double dVar11;
  float x;
  float y;
  int i;
  double local_68;
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
    local_68 = (double)CONCAT44(0x43300000,0x1eU - paused ^ 0x80000000);
    x = (float)(local_68 - (double)(variptr_u  [2])0x4330000080000000) * 0.06666667;
    if (pause_dir == 1) {
      x = 0.0 - x;
    }
    else {
      x = x + 0.0;
    }
  }
  col = (int)cursor->y_min;
  uVar4 = cursor->y_max - col;
  local_68 = (double)CONCAT44(0x43300000,uVar4 ^ 0x80000000);
  dVar10 = 0.5;
  iVar6 = (int)cursor->menu;
  i = 0;
  dVar11 = 1.0;
  fVar1 = (float)(local_68 - (double)(variptr_u  [2])0x4330000080000000) * MENUDY;
  dVar9 = (double)(fVar1 * 0.5);
  if (iVar6 != 0x17) {
    if (iVar6 < 0x18) {
      if (iVar6 == 0xb) {
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
        DrawMenuEntry(cursor,c,&x,&y,&i);
        dme_sy = 0.8333333;
        if (Game.language == 'c') {
          c = "R E S E T   L E V E L ";
        }
        else {
          c = "RESET LEVEL";
        }
        DrawMenuEntry(cursor,c,&x,&y,&i);
        dme_sy = 0.8333333;
        if (Game.language == 'c') {
          c = "G O T O   L E V E L ";
        }
        else {
          c = "GOTO LEVEL";
        }
        DrawMenuEntry(cursor,c,&x,&y,&i);
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
        DrawMenuEntry(cursor,tbuf,&x,&y,&i);
        dme_sy = 0.8333333;
        if (Game.language == 'c') {
          c = "G O T O   N E X T   C H E C K P O I N T ";
        }
        else {
          c = "GOTO NEXT CHECKPOINT";
        }
        DrawMenuEntry(cursor,c,&x,&y,&i);
        dme_sy = 0.8333333;
        if (Game.language == 'c') {
          c = "G O T O   L A S T   C H E C K P O I N T ";
        }
        else {
          c = "GOTO LAST CHECKPOINT";
        }
        DrawMenuEntry(cursor,c,&x,&y,&i);
        dme_sy = 0.8333333;
        if (Game.language == 'c') {
          c = "O P E N   G A M E ";
        }
        else {
          c = "OPEN GAME";
        }
        DrawMenuEntry(cursor,c,&x,&y,&i);
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
        DrawMenuEntry(cursor,tbuf,&x,&y,&i);
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
        DrawMenuEntry(cursor,tbuf,&x,&y,&i);
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
        DrawMenuEntry(cursor,tbuf,&x,&y,&i);
        dme_sy = 0.8333333;
        if (Game.language == 'c') {
          c = "R E S E T   G A M E ";
        }
        else {
          c = "RESET GAME";
        }
        goto LAB_80036f1c;
      }
      if (0xb < iVar6) {
        if (iVar6 == 0x11) {
          DrawNameInputTable(0,(double)TT_INPUTY,cursor);
          return;
        }
        if (0x11 < iVar6) {
          if (iVar6 == 0x14) {
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
              y = (float)(0.5 - dVar9);
              x = 0.4;
              DrawMenuEntry(cursor,tCONTINUE[Game.language],&x,&y,&i);
              DrawMenuEntry(cursor,tQUIT[Game.language],&x,&y,&i);
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
            dVar11 = 1.0;
            x = fVar1;
            y = dy;
            goto LAB_80037ad8;
          }
          if (iVar6 < 0x15) {
            if (iVar6 != 0x12) {
              return;
            }
            y = (float)(-0.699999988079071 - dVar9);
            DrawMenuEntry(cursor,tRESTARTRACE[Game.language],&x,&y,&i);
            c = tWARPROOM[Game.language];
            goto LAB_80036da0;
          }
          if (iVar6 != 0x15) {
            if (iVar6 != 0x16) {
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
          XbUpdateDateStamp((int)&Game);
          DrawGameSlot(&Game,0.0,0.25,4,0.6);
          y = (float)(-0.5 - dVar9);
          if (Game.language == 2) {
            y = y - (float)(dVar9 * 0.2000000029802322);
          }
          DrawMenuEntry(cursor,tLOADGAME[Game.language],&x,&y,&i);
          if (Game.language == 2) {
            y = (float)(dVar9 * 0.119999997317791 + (double)y);
          }
          DrawMenuEntry(cursor,tSAVEGAME[Game.language],&x,&y,&i);
          if (Game.language == 2) {
            y = (float)(dVar9 * 0.119999997317791 + (double)y);
          }
          DrawMenuEntry(cursor,tDELETEGAME[Game.language],&x,&y,&i);
          if (Game.language == 2) {
            y = (float)(dVar9 * 0.119999997317791 + (double)y);
          }
          c = tEXIT[Game.language];
          goto LAB_80037638;
        }
        if (iVar6 == 0xe) {
          if (paused < 0x1e) {
            return;
          }
          local_68 = (double)CONCAT44(0x43300000,uVar4 + 2 ^ 0x80000000);
          y = (float)(local_68 - (double)(variptr_u  [2])0x4330000080000000) * 0.1 * 0.5 - 0.2;
          i = col;
          if (cursor->y_max < col) {
            return;
          }
          do {
            iVar7 = (int)cursor->x_min;
            dVar11 = 0.5;
            local_68 = (double)CONCAT44(0x43300000,cursor->x - iVar7 ^ 0x80000000);
            fVar1 = x - (float)(local_68 - (double)(variptr_u  [2])0x4330000080000000) * 0.5;
            if (iVar7 <= cursor->x_max) {
              col = iVar7 * 0xc;
              txt = (char **)(HubName + iVar7 * 0x18);
              do {
                dVar9 = (double)fVar1;
                if (i == 0) {
                  if (unaff_r27 == -1) {
                    c = "?";
                  }
                  else {
                    c = *txt;
                  }
                  strcpy(tbuf,c);
                  Text3D(tbuf,(float)dVar9,y + 0.2,1.0,(float)(dVar10 * dVar11),(float)dVar10,
                         (float)dVar10,1,4);
                }
                unaff_r27 = (int)(char)(&HData[0].flags)[i + col];
                if (unaff_r27 == -1) {
                  c = "?";
                }
                else {
                  c = *(char **)(LevelName + unaff_r27 * 0x18);
                }
                strcpy(tbuf,c);
                if ((i == cursor->y) && (iVar7 == cursor->x)) {
                  iVar6 = 3;
                  if (GlobalTimer.frame % 0xc < 6) {
                    iVar6 = 4;
                  }
                }
                else {
                  iVar6 = 0;
                }
                iVar7 = iVar7 + 1;
                col = col + 0xc;
                Text3D(tbuf,(float)dVar9,y,1.0,(float)(dVar10 * dVar11),(float)dVar10,(float)dVar1 0,
                       1,iVar6);
                txt = txt + 6;
                fVar1 = (float)(dVar9 + dVar11);
              } while (iVar7 <= cursor->x_max);
            }
            i = i + 1;
            y = y - 0.1;
          } while (i <= cursor->y_max);
          return;
        }
        if (iVar6 < 0xf) {
          if (iVar6 != 0xc) {
            if (iVar6 != 0xd) {
              return;
            }
            if (paused < 0x1e) {
              return;
            }
            dVar9 = -0.1000000014901161;
            local_68 = (double)CONCAT44(0x43300000,
                                        (int)superbuffer_ptr.voidptr - (int)superbuffer_base);
            dVar10 = 9.5367431640625e-07;
            dVar11 = 0.5;
            y = 0.05;
            sprintf(tbuf,"SUPERBUFFER USED: %.2fMB/%.2fMB",
                    (double)((float)(local_68 - (double)(variptr_u  [2])0x4330000000000000) *
                            9.536743e-07),0x401c000000000000);
            if (Game.language == 'c') {
              AddSpacesIntoText(tbuf,1);
            }
            Text3D(tbuf,x,y,1.0,(float)dVar11,(float)dVar11,(float)dVar11,1,0);
            y = (float)((double)y + dVar9);
            local_68 = (double)CONCAT44(0x43300000,highallocaddr ^ 0x80000000);
            sprintf(tbuf,"HIGHALLOCADDR: %.2fMB/32.00MB",
                    (double)(float)((double)(float)(local_68 -
                                                   (double)(variptr_u  [2])0x4330000080000000) *
                                   dVar10));
            if (Game.language == 'c') {
              AddSpacesIntoText(tbuf,1);
            }
            Text3D(tbuf,x,y,1.0,(float)dVar11,(float)dVar11,(float)dVar11,1,0);
            return;
          }
          if (paused < 0x1e) {
            return;
          }
          y = (float)(0.0 - dVar9);
          sprintf(tbuf,"DRAW DISTANCE: %i/%i",(uint)LDATA->farclip,1000);
          if (Game.language == 'c') {
            AddSpacesIntoText(tbuf,1);
          }
          DrawMenuEntry(cursor,tbuf,&x,&y,&i);
          local_68 = (double)(longlong)(int)LDATA->fognear;
          sprintf(tbuf,"FOG NEAR: %i/%i",(int)LDATA->fognear,(uint)LDATA->farclip);
          if (Game.language == 'c') {
            AddSpacesIntoText(tbuf,1);
          }
          DrawMenuEntry(cursor,tbuf,&x,&y,&i);
          local_68 = (double)(longlong)(int)LDATA->fogfar;
          sprintf(tbuf,"FOG FAR: %i/%i",(int)LDATA->fogfar,(uint)LDATA->farclip);
          if (Game.language == 'c') {
            AddSpacesIntoText(tbuf,1);
          }
          DrawMenuEntry(cursor,tbuf,&x,&y,&i);
          sprintf(tbuf,"FOG RED: %i/255",(uint)LDATA->fogr);
          if (Game.language == 'c') {
            AddSpacesIntoText(tbuf,1);
          }
          DrawMenuEntry(cursor,tbuf,&x,&y,&i);
          sprintf(tbuf,"FOG GREEN: %i/255",(uint)LDATA->fogg);
          if (Game.language == 'c') {
            AddSpacesIntoText(tbuf,1);
          }
          DrawMenuEntry(cursor,tbuf,&x,&y,&i);
          sprintf(tbuf,"FOG BLUE: %i/255",(uint)LDATA->fogb);
          if (Game.language == 'c') {
            AddSpacesIntoText(tbuf,1);
          }
          DrawMenuEntry(cursor,tbuf,&x,&y,&i);
          sprintf(tbuf,"FOG OPACITY: %i/127",(uint)LDATA->foga);
          if (Game.language == 'c') {
            AddSpacesIntoText(tbuf,1);
          }
          DrawMenuEntry(cursor,tbuf,&x,&y,&i);
          sprintf(tbuf,"HAZE RED: %i/255",(uint)LDATA->hazer);
          if (Game.language == 'c') {
            AddSpacesIntoText(tbuf,1);
          }
          DrawMenuEntry(cursor,tbuf,&x,&y,&i);
          sprintf(tbuf,"HAZE GREEN: %i/255",(uint)LDATA->hazeg);
          if (Game.language == 'c') {
            AddSpacesIntoText(tbuf,1);
          }
          DrawMenuEntry(cursor,tbuf,&x,&y,&i);
          sprintf(tbuf,"HAZE BLUE: %i/255",(uint)LDATA->hazeb);
          if (Game.language == 'c') {
            AddSpacesIntoText(tbuf,1);
          }
          DrawMenuEntry(cursor,tbuf,&x,&y,&i);
          sprintf(tbuf,"HAZE WOBBLE: %i/255",(uint)LDATA->hazea);
          if (Game.language == 'c') {
            AddSpacesIntoText(tbuf,1);
          }
          c = tbuf;
          goto LAB_80036f1c;
        }
        if (iVar6 == 0xf) {
          y = (float)(0.0 - dVar9);
          if (Game.language == 'c') {
            c = "L O G O S ";
          }
          else {
            c = "LOGOS";
          }
          DrawMenuEntry(cursor,c,&x,&y,&i);
          if (Game.language == 'c') {
            c = "I N T R O 1 ";
          }
          else {
            c = "INTRO1";
          }
          DrawMenuEntry(cursor,c,&x,&y,&i);
          if (Game.language == 'c') {
            c = "I N T R O 2 ";
          }
          else {
            c = "INTRO2";
          }
          DrawMenuEntry(cursor,c,&x,&y,&i);
          if (Game.language == 'c') {
            c = "O U T R O ";
          }
          else {
            c = "OUTRO";
          }
          DrawMenuEntry(cursor,c,&x,&y,&i);
          if (Game.language == 'c') {
            c = "O U T R O 2 ";
          }
          else {
            c = "OUTRO2";
          }
          goto LAB_80036f1c;
        }
        if (iVar6 != 0x10) {
          return;
        }
        y = (float)(-0.699999988079071 - dVar9);
        DrawMenuEntry(cursor,tRESTARTTRIAL[Game.language],&x,&y,&i);
        c = tWARPROOM[Game.language];
        goto LAB_80036da0;
      }
      if (iVar6 == 5) {
        if (paused < 0x1e) {
          return;
        }
        y = (float)(-0.5 - dVar9);
        x = PANELMENUX;
        DrawMenuEntry(cursor,tRESUME[Game.language],&x,&y,&i);
        DrawMenuEntry(cursor,tOPTIONS[Game.language],&x,&y,&i);
        if (Level == 0x25) {
          txt = tQUIT;
        }
        else {
          txt = tWARPROOM;
        }
        DrawMenuEntry(cursor,txt[Game.language],&x,&y,&i);
        if (TimeTrial == 0) {
          return;
        }
        c = tRESTARTTRIAL[Game.language];
        goto LAB_80036f1c;
      }
      if (iVar6 < 6) {
        if (2 < iVar6) {
          if (iVar6 == 3) {
            return;
          }
          if (iVar6 != 4) {
            return;
          }
          goto LAB_80034ee8;
        }
        if (iVar6 < 1) {
          if (iVar6 != 0) {
            return;
          }
          y = (float)(-0.6899999976158142 - dVar9);
          VersionDisplayFlag = 1;
          SaveMenu = iVar6;
          BackMenu = iVar6;
          DrawMenuEntry(cursor,tNEWGAME[Game.language],&x,&y,&i);
          DrawMenuEntry(cursor,tLOADGAME[Game.language],&x,&y,&i);
          DrawMenuEntry(cursor,tPLAYCRASHBLAST[Game.language],&x,&y,&i);
          sprintf(tbuf,"%s: %s",tLANGUAGE[Game.language],
                  *(undefined4 *)(LanguageName + (uint)Game.language * 4));
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
        y = (float)(-0.6899999976158142 - dVar9);
        DrawMenuEntry(cursor,tMEMCARDRETRY[Game.language],&x,&y,&i);
        DrawMenuEntry(cursor,tMEMCARDCONTINUE[Game.language],&x,&y,&i);
        iVar7 = ParseNintendoErrorCode();
        if (iVar7 == 6) {
          uVar4 = (uint)Game.language;
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
          uVar4 = (uint)Game.language;
          txt = tFORMAT;
          goto LAB_80037c10;
        }
        goto LAB_80037be8;
      }
      if (iVar6 == 8) {
        return;
      }
      if (iVar6 < 9) {
        if (iVar6 != 6) {
          if (iVar6 != 7) {
            return;
          }
          if (paused < 0x1e) {
            return;
          }
          uVar4 = (uint)Game.sfx_volume;
          y = (float)(-0.4600000083446503 - dVar9) - (float)(dVar9 * 0.5);
          dme_sx = 1.0;
          x = PANELMENUX;
          uVar2 = 0x20;
          if (uVar4 != 0) {
            uVar2 = 0x3c;
          }
          uVar5 = 0x20;
          if (uVar4 < 100) {
            uVar5 = 0x3e;
          }
          sprintf(tbuf,"%c %i%% %c",uVar2,uVar4,uVar5);
          if (Game.language == 'c') {
            AddSpacesIntoText(tbuf,1);
          }
          dme_sx = (float)dVar11;
          DrawMenuEntry2(cursor,tSFXVOLUME[Game.language],tbuf,&x,&y,&i);
          uVar4 = (uint)Game.music_volume;
          uVar2 = 0x20;
          if (uVar4 != 0) {
            uVar2 = 0x3c;
          }
          uVar5 = 0x20;
          if (uVar4 < 100) {
            uVar5 = 0x3e;
          }
          sprintf(tbuf,"%c %i%% %c",uVar2,uVar4,uVar5);
          if (Game.language == 'c') {
            AddSpacesIntoText(tbuf,1);
          }
          dme_sx = (float)dVar11;
          DrawMenuEntry2(cursor,tMUSICVOLUME[Game.language],tbuf,&x,&y,&i);
          dme_sx = (float)dVar11;
          DrawMenuEntry(cursor,tDONE[Game.language],&x,&y,&i);
          return;
        }
        if (paused < 0x1e) {
          return;
        }
        uVar4 = (uint)Game.language;
        y = (float)(-0.5 - dVar9);
        x = PANELMENUX;
        if (uVar4 == 99) {
          c = tRELICS[3];
          if (Game.vibration != '\0') {
            c = tGEMS[3];
          }
          sprintf(tbuf,"%s:   %s",PTR_ARRAY_80143e48[3],c);
        }
        else {
          if (Game.vibration == '\0') {
            txt = tOFF;
          }
          else {
            txt = tON;
          }
          sprintf(tbuf,"%s: %s",tVIBRATION[uVar4],txt[uVar4]);
        }
        dme_sx = (float)dVar11;
        DrawMenuEntry(cursor,tbuf,&x,&y,&i);
        dme_sx = (float)dVar11;
        DrawMenuEntry(cursor,tSOUNDOPTIONS[Game.language],&x,&y,&i);
        if (LANGUAGEOPTION != 0) {
          dme_sx = (float)dVar11;
          DrawMenuEntry(cursor,tLANGUAGE[Game.language],&x,&y,&i);
        }
        dme_sx = (float)dVar11;
        c = tDONE[Game.language];
        goto LAB_80036f1c;
      }
      if (iVar6 == 9) {
        if (paused < 0x1e) {
          return;
        }
        puVar8 = LanguageName;
        x = PANELMENUX;
        y = (float)(-0.5 - dVar9);
        do {
          c = *(char **)puVar8;
          puVar8 = (undefined1 *)((int)puVar8 + 4);
          DrawMenuEntry(cursor,c,&x,&y,&i);
        } while ((int)puVar8 < -0x7febcbdb);
        return;
      }
      if (iVar6 != 10) {
        return;
      }
      if (paused < 0x1e) {
        return;
      }
      y = (float)(-0.5 - dVar9);
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
      if (iVar6 == 0x23) {
        DrawCredits();
        return;
      }
      if (iVar6 < 0x24) {
        if (iVar6 == 0x1d) {
          iVar7 = ParseNintendoErrorCode();
          if (iVar7 != 0) {
LAB_80036b44:
            BackMenu = 0x15;
            SaveMenu = iVar6;
            NewMenu(cursor,0x2f,0,-1);
            return;
          }
          iVar7 = 1;
          Text3D(tDELETEGAME[Game.language],0.0,0.81,(float)dVar11,0.6,0.6,0.6,1,0);
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
        if (iVar6 < 0x1e) {
          if (iVar6 == 0x1a) {
LAB_80034ee8:
            VersionDisplayFlag = 1;
            DrawNameInputTable(0,0xbfe0000000000000,cursor);
            c = MakeEditText(Game.name);
            Text3D(c,0.0,GAMENAMEY,1.0,1.0,1.0,1.0,1,4);
            return;
          }
          if (iVar6 < 0x1b) {
            if (iVar6 != 0x18) {
              if (iVar6 != 0x19) {
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
            Text3D(tMEMCARDACCESS[(uint)Game.language * 2],0.0,-0.3,(float)dVar11,0.6,0.6,0.6,1,0 );
            scalex = 0.6;
            fVar1 = 0.0;
            c = tMEMCARDACCESS[(uint)Game.language * 2 + 1];
            dy = -0.45;
          }
          else {
            if (iVar6 == 0x1b) {
              Text3D(tSAVEGAME[Game.language],0.0,0.75,1.0,0.6,0.6,0.6,1,0);
              DrawGameSlot(&Game,0.0,0.25,4,0.6);
              if (game->empty == '\0') {
                Text3D(tCONFIRMOVERWRITE[(uint)Game.language * 2],0.0,-0.3,(float)dVar11,0.6,0.6,0 .6
                       ,1,0);
                Text3D(tCONFIRMOVERWRITE[(uint)Game.language * 2 + 1],0.0,-0.45,(float)dVar11,0.6,
                       0.6,0.6,1,0);
              }
              else {
                Text3D(tCONFIRMSAVE[Game.language],0.0,-0.3,(float)dVar11,0.6,0.6,0.6,1,0);
              }
              y = (float)(-0.699999988079071 - dVar9);
              DrawMenuEntry(cursor,tYES[Game.language],&x,&y,&i);
              c = tNO[Game.language];
              goto LAB_80036da0;
            }
            if (iVar6 != 0x1c) {
              return;
            }
            Text3D(tSAVEGAME[Game.language],0.0,0.75,1.0,0.6,0.6,0.6,1,0);
            Text3D(tMEMCARDACCESS[(uint)Game.language * 2],0.0,-0.3,(float)dVar11,0.6,0.6,0.6,1,0 );
            scalex = 0.6;
            fVar1 = 0.0;
            c = tMEMCARDACCESS[(uint)Game.language * 2 + 1];
            dy = -0.45;
          }
          goto LAB_80037ad8;
        }
        if (iVar6 == 0x20) {
          Text3D(tFORMAT[Game.language],0.0,0.75,1.0,0.6,0.6,0.6,1,0);
          Text3D(tCONFIRMFORMAT[Game.language],0.0,-0.3,(float)dVar11,0.6,0.6,0.6,1,0);
          y = (float)(-0.699999988079071 - dVar9);
          DrawMenuEntry(cursor,tYES[Game.language],&x,&y,&i);
          txt = tNO;
LAB_80037624:
          c = txt[Game.language];
LAB_80037638:
          DrawMenuEntry(cursor,c,&x,&y,&i);
          return;
        }
        if (0x20 < iVar6) {
          if (iVar6 != 0x21) {
            if (iVar6 != 0x22) {
              return;
            }
            pdat = PData + new_power;
            if ((new_power == 0) && (Game.language == '\x01')) {
              fVar1 = 0.55;
            }
            else {
              fVar1 = 0.6;
            }
            dVar11 = 0.6000000238418579;
            Text3D(PData[new_power].name,0.0,POWERTEXTY + 0.6,1.0,fVar1,0.6,0.6,1,1);
            if (cursor->button_lock == '\0') {
              y = (float)((double)(float)((double)POWERTEXTY - dVar11) - dVar9);
              DrawMenuEntry(cursor,tPRESSxTOCONTINUE[Game.language],&x,&y,&i);
            }
            uVar4 = PData[iVar7].font3d_letter - 0x61 & 0xff;
            fVar1 = Font3DObjTab[uVar4].scale * 2.5;
            col = 0;
            DrawPanel3DCharacter
                      ((uint)pdat->character,0.0,-0.8,1.0,fVar1,fVar1,fVar1,0,0,0,
                       (int)Font3DObjTab[uVar4].action,Font3DObjTab[uVar4].anim_time,0);
            while ((col < 7 &&
                   (iVar6 = strcmp((char *)PData[iVar7].description[col],""), iVar6 != 0))) {
              col = col + 1;
            }
            local_68 = (double)CONCAT44(0x43300000,col - 1U ^ 0x80000000);
            dVar11 = 0.5;
            dVar9 = (double)(MENUDY * 1.2);
            i = 0;
            y = POWERTEXTY -
                (float)((double)(float)(local_68 - (double)(variptr_u  [2])0x4330000080000000) *
                       dVar9) * 0.5;
            if (col < 1) {
              return;
            }
            do {
              Text3D((char *)PData[iVar7].description[i],0.0,y,1.0,(float)dVar11,(float)dVar11,0.6 ,1
                     ,0);
              i = i + 1;
              y = (float)((double)y + dVar9);
            } while (i < col);
            return;
          }
          Text3D(tFORMAT[Game.language],0.0,0.75,1.0,0.6,0.6,0.6,1,0);
          Text3D(tMEMCARDACCESS[(uint)Game.language * 2],0.0,-0.3,(float)dVar11,0.6,0.6,0.6,1,0);
          scalex = 0.6;
          fVar1 = 0.0;
          c = tMEMCARDACCESS[(uint)Game.language * 2 + 1];
          dy = -0.45;
          goto LAB_80037ad8;
        }
        if (iVar6 != 0x1e) {
          if (iVar6 != 0x1f) {
            return;
          }
          Text3D(tDELETEGAME[Game.language],0.0,0.75,1.0,0.6,0.6,0.6,1,0);
          Text3D(tMEMCARDACCESS[(uint)Game.language * 2],0.0,-0.3,1.0,0.6,0.6,0.6,1,0);
          scalex = 0.6;
          fVar1 = 0.0;
          c = tMEMCARDACCESS[(uint)Game.language * 2 + 1];
          dy = -0.45;
          dVar11 = 1.0;
          goto LAB_80037ad8;
        }
        Text3D(tDELETEGAME[Game.language],0.0,0.75,1.0,0.6,0.6,0.6,1,0);
        DrawGameSlot(game,0.0,0.25,4,0.6);
        txt = tCONFIRMDELETE;
        goto LAB_80036d1c;
      }
      if (iVar6 == 0x2b) {
        NuRndrRectUV2di(0,0,0x280,0x1e0,0.0,0.0,1.0,1.0,-1,GBABG_mtl);
        return;
      }
      if (iVar6 < 0x2c) {
        if (iVar6 == 0x28) {
          NuRndrRectUV2di(0,0,0x280,0x1e0,0.0,0.0,1.0,1.0,-1,GBABG_mtl);
          Text3D(tCONNECTGBA[(uint)Game.language * 3],0.0,-0.35,(float)dVar11,0.4,0.4,0.4,1,0);
          Text3D(tCONNECTGBA[(uint)Game.language * 3 + 1],0.0,-0.49,(float)dVar11,0.4,0.4,0.4,1,0) ;
          Text3D(tCONNECTGBA[(uint)Game.language * 3 + 2],0.0,-0.63,(float)dVar11,0.4,0.4,0.4,1,0) ;
          c = tPRESSxTOCONTINUE[Game.language];
          fVar1 = -0.77;
LAB_800378c4:
          Text3D(c,0.0,fVar1,(float)dVar11,0.4,0.4,0.4,1,0);
          GBA_Draw();
          return;
        }
        if (0x28 < iVar6) {
          if (iVar6 == 0x29) {
            NuRndrRectUV2di(0,0,0x280,0x1e0,0.0,0.0,1.0,1.0,-1,GBABG_mtl);
            Text3D(tCONNECTING[Game.language],0.0,-0.35,(float)dVar11,0.4,0.4,0.4,1,0);
            c = tPLEASEWAIT[Game.language];
            fVar1 = -0.49;
          }
          else {
            if (iVar6 != 0x2a) {
              return;
            }
            NuRndrRectUV2di(0,0,0x280,0x1e0,0.0,0.0,1.0,1.0,-1,GBABG_mtl);
            Text3D(tDOWNLOADING[Game.language],0.0,-0.35,1.0,0.4,0.4,0.4,1,0);
            c = tPLEASEWAIT[Game.language];
            fVar1 = -0.49;
            dVar11 = 1.0;
          }
          goto LAB_800378c4;
        }
        if (iVar6 != 0x26) {
          if (iVar6 != 0x27) {
            return;
          }
          NuRndrRectUV2di(0,0,0x280,0x1e0,0.0,0.0,1.0,1.0,-1,GBABG_mtl);
          y = -0.55;
          DrawMenuEntry(cursor,tLOADCRASHBLAST[Game.language],&x,&y,&i);
          txt = tEXIT;
          goto LAB_80037624;
        }
        pnVar3 = (nupad_s *)XbGetNumControllers();
        if (pnVar3 == (nupad_s *)0x0) {
          Pad[0] = pnVar3;
          return;
        }
        if (lost_controller != 0) {
          return;
        }
        if (((saveload_error & 0x20) == 0) && (saveload_cardchanged == 0)) {
          if (saveload_error == 4) {
LAB_800371cc:
            txt = tFAILEDTOSAVE;
          }
          else {
            if ((int)saveload_error < 5) {
LAB_80037218:
              Text3D(tTCR_CONTENTPROTECTIONFAULT[Game.language],0.0,0.0,1.0,0.5,0.5,0.5,1,0);
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
        }
        else {
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
        }
LAB_80037260:
        scalex = 0.5;
        c = tPRESSxTOCONTINUE[Game.language];
        fVar1 = 0.0;
        dy = -0.15;
        dVar11 = 1.0;
LAB_80037ad8:
        Text3D(c,fVar1,dy,(float)dVar11,scalex,scalex,scalex,1,0);
        return;
      }
      if (iVar6 == 0x2f) {
        y = 0.2;
        for (iVar7 = 0; iVar7 < 4; iVar7 = iVar7 + 1) {
          uVar2 = ParseNintendoErrorCode();
          c = (char *)GetStringIdx(uVar2,iVar7);
          if (c == (char *)0x0) break;
          Text3D(c,0.0,y,1.0,0.4,0.4,0.4,1,0);
          y = y - 0.1;
        }
        y = (float)(-0.6899999976158142 - dVar9);
        DrawMenuEntry(cursor,tMEMCARDRETRY[Game.language],&x,&y,&i);
        DrawMenuEntry(cursor,tMEMCARDCONTINUE[Game.language],&x,&y,&i);
        iVar7 = ParseNintendoErrorCode();
        if (iVar7 < 2) {
          return;
        }
        if (iVar7 < 4) {
          uVar4 = (uint)Game.language;
          txt = tFORMAT;
          goto LAB_80037c10;
        }
LAB_80037be8:
        if (iVar7 != 8) {
          return;
        }
        uVar4 = (uint)Game.language;
        txt = tDELETE;
LAB_80037c10:
        DrawMenuEntry(cursor,txt[uVar4],&x,&y,&i);
        cursor->y_max = '\x02';
        return;
      }
      if (iVar6 < 0x30) {
        if (iVar6 == 0x2c) {
          NuRndrRectUV2di(0,0,0x280,0x1e0,0.0,0.0,1.0,1.0,-1,GBABG_mtl);
          Text3D(tGAMEINST[(uint)Game.language * 3],0.0,-0.35,(float)dVar11,0.4,0.4,0.4,1,0);
          Text3D(tGAMEINST[(uint)Game.language * 3 + 1],0.0,-0.49,(float)dVar11,0.4,0.4,0.4,1,0);
          Text3D(tGAMEINST[(uint)Game.language * 3 + 2],0.0,-0.63,(float)dVar11,0.4,0.4,0.4,1,0);
          scalex = 0.4;
          c = tPRESSxTOCONTINUE[Game.language];
          fVar1 = 0.0;
          dy = -0.77;
        }
        else {
          if (iVar6 != 0x2d) {
            return;
          }
          NuRndrRectUV2di(0,0,0x280,0x1e0,0.0,0.0,1.0,1.0,-1,GBABG_mtl);
          Text3D(tCANTDOWNLOAD[Game.language],0.0,-0.35,1.0,0.4,0.4,0.4,1,0);
          scalex = 0.4;
          c = tPRESSxTOCONTINUE[Game.language];
          fVar1 = 0.0;
          dy = -0.77;
          dVar11 = 1.0;
        }
        goto LAB_80037ad8;
      }
      if (iVar6 != 0x30) {
        if (iVar6 != 0x31) {
          return;
        }
        Text3D(tDELETEGAME[Game.language],0.0,0.75,1.0,0.6,0.6,0.6,1,0);
        Text3D(tMEMCARDACCESS[(uint)Game.language * 2],0.0,-0.3,1.0,0.6,0.6,0.6,1,0);
        scalex = 0.6;
        fVar1 = 0.0;
        c = tMEMCARDACCESS[(uint)Game.language * 2 + 1];
        dy = -0.45;
        dVar11 = 1.0;
        goto LAB_80037ad8;
      }
      Text3D(tCONFIRMDELETE[Game.language],0.0,-0.3,1.0,0.6,0.6,0.6,1,0);
      c = tYES[Game.language];
      y = (float)(-0.699999988079071 - dVar9);
    }
    DrawMenuEntry(cursor,c,&x,&y,&i);
    c = tNO[Game.language];
LAB_80036f1c:
    DrawMenuEntry(cursor,c,&x,&y,&i);
    return;
  }
  Text3D(tLOADGAME[Game.language],0.0,0.75,1.0,0.6,0.6,0.6,1,0);
  DrawGameSlot(game,0.0,0.25,4,0.6);
  txt = tCONFIRMLOAD;
LAB_80036d1c:
  Text3D(txt[Game.language],0.0,-0.3,(float)dVar11,0.6,0.6,0.6,1,0);
  y = (float)(-0.699999988079071 - dVar9);
  DrawMenuEntry(cursor,tYES[Game.language],&x,&y,&i);
  c = tNO[Game.language];
LAB_80036da0:
  DrawMenuEntry(cursor,c,&x,&y,&i);
  return;
}








void ProcMenu(Cursor *cursor,nupad_s *pad)

{
  bool bVar1;
  char cVar2;
  char cVar3;
  bool bVar4;
  uint uVar5;
  uint uVar6;
  uint uVar7;
  creature_s *pcVar8;
  nuvec_s *pnVar9;
  Game_s *pGVar10;
  Game_s *pGVar11;
  uchar uVar13;
  char cVar14;
  char cVar15;
  ushort uVar12;
  uint uVar16;
  uint uVar17;
  int iVar18;
  char *pcVar19;
  uint uVar20;
  int uVar21;
  float fVar21;
  int iVar22;
  int iVar23;
  float fVar24;
  Game_s *pGVar25;
  uchar *puVar26;
  mask *pmVar27;
  Game_s *pGVar28;
  int iVar29;
  int test_sfx [4];
  double local_38;
  
  uVar20 = menu_pulsate_ang & 0xffff;
  menu_pulsate_ang = menu_pulsate_ang + menu_pulsate_rate;
  menu_pulsate = NuTrigTable[uVar20] * 0.1 + 1.0;
  test_sfx[0] = 0x25;
  test_sfx[1] = 0x19;
  test_sfx[2] = 0x13;
  test_sfx[3] = 0x2c;
  if (0x10000 < menu_pulsate_ang) {
    menu_pulsate_ang = menu_pulsate_ang + -0x10000;
  }
  if ((cursor->wait != '\0') &&
     (uVar13 = cursor->wait + 0xff, cursor->wait = uVar13, uVar13 == '\0')) {
    cursor->wait_hack = '\x01';
    NewMenu(cursor,(int)cursor->new_menu,-1,-1);
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
    if (Paused - 1U < 0x1d) {
      return;
    }
    if (cutmovie == -1) {
      if (new_mode != -1) {
        return;
      }
      if (new_level != -1) {
        return;
      }
    }
    else if (cut_on == 0) {
      return;
    }
    if ((cursor->menu == '\x14') && (gameover_hack != 1)) {
      return;
    }
  }
  iVar29 = -1;
  if (cursor->button_lock != '\0') {
    cursor->button_lock = cursor->button_lock + -1;
  }
  if ((pad == (nupad_s *)0x0) || (cursor->button_lock != '\0')) {
    uVar20 = 0;
    fVar21 = 0.0;
  }
  else {
    fVar21 = pad->r_dy;
    if (((uint)fVar21 & 0xf000) == 0) {
      fVar21 = (float)((uint)fVar21 | stick_bits);
    }
    uVar20 = pad->old_paddata;
    if ((uVar20 & 0xf000) == 0) {
      uVar20 = uVar20 | stick_bits_db;
    }
  }
  uVar16 = uVar20 & 0x840;
  uVar5 = uVar20 & 0x180;
  uVar6 = (uint)fVar21 & 0x8000;
  uVar7 = (uint)fVar21 & 0x2000;
  if (cursor->item_frame == 0) {
    uVar16 = 0;
  }
  uVar17 = uVar16;
  if (ForceRestart != 0) {
    ForceRestart = 0;
    uVar17 = 1;
    cursor->menu = '\v';
  }
  cVar14 = cursor->y_max;
  cVar15 = cursor->y_min;
  cVar2 = cursor->y;
  cVar3 = (cVar14 - cVar15) + '\x01';
  if (cVar2 < cVar15) {
    cursor->y = cVar15;
  }
  else if (cVar14 < cVar2) {
    cursor->y = cVar14;
  }
  if ((uVar20 & 0x4000) == 0) {
    if ((uVar20 & 0x1000) != 0) {
      cVar14 = cursor->y + -1;
      cursor->y = cVar14;
      if (cVar14 < cursor->y_min) {
        cVar14 = cVar14 + cVar3;
        goto LAB_80032034;
      }
    }
  }
  else {
    cVar14 = cursor->y + '\x01';
    cursor->y = cVar14;
    if (cursor->y_max < cVar14) {
      cVar14 = cVar14 - cVar3;
LAB_80032034:
      cursor->y = cVar14;
    }
  }
  cursor->remember[cursor->menu].y = cursor->y;
  if (cursor->y != cVar2) {
    iVar29 = 0x18;
    cursor->item_frame = 0;
    uVar16 = 0;
    uVar17 = 0;
  }
  cVar14 = cursor->x_max;
  cVar15 = cursor->x_min;
  cVar2 = cursor->x;
  cVar3 = (cVar14 - cVar15) + '\x01';
  if (cVar2 < cVar15) {
    cursor->x = cVar15;
  }
  else if (cVar14 < cVar2) {
    cursor->x = cVar14;
  }
  if (cursor->y < cursor->y_max) {
    if ((uVar20 & 0x2000) == 0) {
      if ((uVar20 & 0x8000) != 0) {
        cVar14 = cursor->menu;
        cVar15 = cursor->x + -1;
        cursor->x = cVar15;
        if (((cVar14 == '\x16') || (cVar14 == '\x19')) || (cVar14 == '\x1d')) {
          if (cursor->x < cursor->x_min) {
            cursor->x = cursor->x_min;
          }
        }
        else if (cVar15 < cursor->x_min) {
          cVar15 = cVar15 + cVar3;
          goto LAB_800321ac;
        }
      }
    }
    else {
      cVar14 = cursor->menu;
      cVar15 = cursor->x + '\x01';
      cursor->x = cVar15;
      if (((cVar14 == '\x16') || (cVar14 == '\x19')) || (cVar14 == '\x1d')) {
        if (cursor->x_max < cursor->x) {
          cursor->x = cursor->x_max;
        }
      }
      else if (cursor->x_max < cVar15) {
        cVar15 = cVar15 - cVar3;
LAB_800321ac:
        cursor->x = cVar15;
      }
    }
  }
  cursor->remember[cursor->menu].x = cursor->x;
  if (cursor->x != cVar2) {
    iVar29 = 0x18;
    cursor->item_frame = 0;
    uVar16 = 0;
    uVar17 = 0;
  }
  bVar1 = uVar17 == 0;
  if ((!bVar1) || (uVar5 != 0)) {
    cursor->item_frame = 0;
  }
  iVar23 = Level;
  uVar17 = cursor->item_frame + 1;
  cursor->menu_frame = cursor->menu_frame + 1;
  cursor->item_frame = uVar17;
  iVar22 = cortex_quit_i;
  iVar18 = cortex_continue_i;
  if ((iVar23 == 0x23) || (cVar14 = cursor->menu, cutmovie == 0)) {
    cVar14 = cursor->menu;
    iVar23 = (int)cVar14;
    if ((iVar23 == 0) && (0x707 < uVar17)) {
      new_level = DemoLevel[i_demolevel];
      i_demolevel = i_demolevel + 1;
      if (i_demolevel == 3) {
        i_demolevel = iVar23;
      }
      if (cutmovie != 0) {
        Demo = 1;
        InvincibilityCHEAT = iVar23;
        return;
      }
      Demo = 1;
      Level = new_level;
      InvincibilityCHEAT = iVar23;
      fade_rate = 8;
      return;
    }
  }
  cVar15 = cursor->y;
  iVar23 = (int)cVar15;
  if (cVar14 == '\x17') {
    loadsaveCallEachFrame();
    if ((saveload_cardchanged != 0) || (saveload_error != 0)) {
      NewMenu(cursor,0x26,-1,-1);
    }
    if (bVar1) {
      if (uVar5 == 0) goto LAB_800346ac;
      iVar18 = 0x16;
      iVar29 = -1;
    }
    else {
      if (cursor->y != '\x01') {
        iVar29 = 0x408;
        pGVar10 = &Game;
        pGVar11 = game;
        do {
          pGVar28 = pGVar11;
          pGVar25 = pGVar10;
          iVar29 = iVar29 + -0x18;
          *(undefined4 *)pGVar25->name = *(undefined4 *)pGVar28->name;
          *(undefined4 *)(pGVar25->name + 4) = *(undefined4 *)(pGVar28->name + 4);
          *(undefined4 *)(pGVar25->name + 8) = *(undefined4 *)(pGVar28->name + 8);
          *(undefined4 *)&pGVar25->music_volume = *(undefined4 *)&pGVar28->music_volume;
          pGVar25->hub[0] = pGVar28->hub[0];
          pGVar25->hub[1] = pGVar28->hub[1];
          pGVar10 = (Game_s *)(pGVar25->hub + 2);
          pGVar11 = (Game_s *)(pGVar28->hub + 2);
        } while (iVar29 != 0);
        iVar29 = 0x36;
        *(undefined4 *)((Game_s *)(pGVar25->hub + 2))->name =
             *(undefined4 *)((Game_s *)(pGVar28->hub + 2))->name;
        pGVar25->hub[3] = pGVar28->hub[3];
        pGVar25->hub[4] = pGVar28->hub[4];
        NewLanguage((uint)Game.language);
        NewMenu(cursor,0x18,0,-1);
        memcard_loadresult_delay = 0x28;
        plr_lives.count = (short)Game.lives;
        plr_lives.draw = plr_lives.count;
        goto LAB_800346ac;
      }
      iVar18 = 0x16;
      iVar29 = -1;
    }
    goto LAB_80034620;
  }
  if (cVar14 < '\x18') {
    if (cVar14 == '\v') {
      if (bVar1) {
        if (uVar5 == 0) goto LAB_800346ac;
        iVar22 = -1;
        iVar18 = 5;
      }
      else {
        if (iVar23 == 5) {
          uVar21 = 1;
LAB_80032950:
          iVar18 = GotoCheckpoint(&(player->obj).pos,uVar21);
          if (iVar18 != 0) {
            new_mode = GameMode;
          }
          goto LAB_800346ac;
        }
        if (iVar23 < 6) {
          if (iVar23 != 2) {
            if (iVar23 < 3) {
              if (iVar23 == 0) {
                new_mode = GameMode;
                ResetCheckpoint(-1,-1,0.0,(nuvec_s *)0x0);
                LivesLost = iVar23;
                LostLife = iVar23;
                ResetBonus();
                ResetDeath();
                ResetGemPath();
                ResetItems();
              }
              else if (iVar23 == 1) {
                ResetCheckpoint(-1,-1,0.0,(nuvec_s *)0x0);
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
              }
              goto LAB_800346ac;
            }
            if (iVar23 == 3) {
              InvincibilityCHEAT = 1 - InvincibilityCHEAT;
              ResetAI();
              goto LAB_800346ac;
            }
            if (iVar23 != 4) goto LAB_800346ac;
            uVar21 = 0;
            goto LAB_80032950;
          }
          iVar22 = -1;
          iVar18 = 0xe;
        }
        else {
          if (iVar23 == 8) {
            ShowPlayerCoordinate = 1 - ShowPlayerCoordinate;
            goto LAB_800346ac;
          }
          if (iVar23 < 9) {
            if (iVar23 != 6) {
              if (iVar23 == 7) {
                LIFTPLAYER = 1 - LIFTPLAYER;
              }
              goto LAB_800346ac;
            }
            OpenGame();
            iVar22 = -1;
            iVar18 = 5;
          }
          else {
            if (iVar23 == 9) {
              ExtraMoves = 1 - ExtraMoves;
              goto LAB_800346ac;
            }
            if (iVar23 != 10) goto LAB_800346ac;
            NewGame();
            ResetBonus();
            ResetDeath();
            ResetGemPath();
            CalculateGamePercentage(&Game);
            Hub = HubFromLevel(Level);
            iVar22 = -1;
            iVar18 = 5;
          }
        }
      }
LAB_80034684:
      NewMenu(cursor,iVar18,iVar22,-1);
      goto LAB_800346ac;
    }
    if (cVar14 < '\f') {
      if (cVar14 == '\x05') {
        if (bVar1) goto LAB_800346ac;
        iVar29 = 0x36;
        if (iVar23 == 1) {
          NewMenu(cursor,6,0,-1);
          storemenu1();
          goto LAB_800346ac;
        }
        if (iVar23 < 2) {
          if (iVar23 == 0) {
            pause_dir = 2;
          }
          goto LAB_800346ac;
        }
        if (iVar23 == 2) {
          iVar22 = 1;
          iVar18 = 10;
          goto LAB_80034684;
        }
        if ((iVar23 != 3) || (TimeTrial == 0)) goto LAB_800346ac;
        new_mode = GameMode;
        ResetCheckpoint(-1,-1,0.0,(nuvec_s *)0x0);
        LivesLost = 0;
LAB_8003388c:
        iVar29 = 0x36;
        ResetBonus();
        ResetDeath();
        ResetGemPath();
        goto LAB_800346ac;
      }
      if ('\x05' < cVar14) {
        if (cVar14 == '\b') goto LAB_800346ac;
        if ('\b' < cVar14) {
          if (cVar14 == '\t') {
            if (!bVar1) {
              iVar29 = 0x36;
              NewLanguage(iVar23);
              iVar22 = -1;
              iVar18 = 6;
              goto LAB_80034684;
            }
            if (uVar5 == 0) goto LAB_800346ac;
            iVar29 = -1;
            iVar18 = 6;
          }
          else {
            if (cVar14 != '\n') goto LAB_800346ac;
            if (bVar1) {
              if (uVar5 == 0) goto LAB_800346ac;
              iVar29 = -1;
              iVar18 = 5;
            }
            else {
              if (iVar23 == 0) {
                new_level = 0x25;
                if (Level == 0x25) {
                  new_level = 0x23;
                }
                goto LAB_800346a8;
              }
              iVar29 = -1;
              iVar18 = 5;
            }
          }
          goto LAB_80034620;
        }
        if (cVar14 == '\x06') {
          if (!bVar1) {
            if (iVar23 == 0) {
              iVar29 = 0x36;
              Game.vibration = '\x01' - Game.vibration;
              goto LAB_800346ac;
            }
            if (iVar23 == 1) {
              storemenu2();
              iVar29 = 0x36;
              iVar18 = 7;
              goto LAB_80034680;
            }
            if ((LANGUAGEOPTION != 0) && (iVar23 == 2)) {
              iVar29 = 0x36;
              NewMenu(cursor,9,(uint)Game.language,-1);
              goto LAB_800346ac;
            }
            if (iVar23 != cursor->y_max) goto LAB_800346ac;
            iVar29 = 5;
            iVar18 = -1;
            goto LAB_800346a0;
          }
          if (uVar5 == 0) goto LAB_800346ac;
          restoremenu1();
          iVar29 = 0x3c;
          iVar22 = -1;
          iVar18 = 5;
        }
        else {
          if (cVar14 != '\a') goto LAB_800346ac;
          if (!bVar1) {
            if (iVar23 != 2) goto LAB_800346ac;
            iVar29 = 6;
            iVar18 = -1;
            goto LAB_800346a0;
          }
          if (uVar5 == 0) {
            if (iVar23 == 0) {
              iVar18 = qrand();
              if (iVar18 < 0x1000) {
                gamesfx_volume = (int)Game.sfx_volume;
                iVar18 = qrand();
                if (iVar18 < 0) {
                  iVar18 = iVar18 + 0x3fff;
                }
                GameSfx(test_sfx[iVar18 >> 0xe],(nuvec_s *)0x0);
              }
              if ((uVar6 == 0) || (Game.sfx_volume == '\0')) {
                if ((uVar7 != 0) && (Game.sfx_volume < 100)) {
                  Game.sfx_volume = Game.sfx_volume + '\x01';
                }
              }
              else {
                Game.sfx_volume = Game.sfx_volume + 0xff;
              }
            }
            else if (iVar23 == 1) {
              if ((uVar6 == 0) || (Game.music_volume == '\0')) {
                if ((uVar7 != 0) && (Game.music_volume < 100)) {
                  Game.music_volume = Game.music_volume + '\x01';
                }
              }
              else {
                Game.music_volume = Game.music_volume + 0xff;
              }
            }
            goto LAB_800346ac;
          }
          restoremenu2();
          iVar29 = 0x3c;
          iVar22 = -1;
          iVar18 = 6;
        }
        goto LAB_80034684;
      }
      if (cVar14 == '\x02') {
        if (memtest_done < 3) {
          if (memtest_done < 1) {
            if ((memtest_done == 0) && (loadsaveCallEachFrame(), saveload_status == 1)) {
              if (saveload_cardtype == 2) {
                memtest_done = 3;
              }
              else {
                memtest_done = saveload_status;
              }
            }
          }
          else {
            loadsaveCallEachFrame();
            if (!bVar1) {
              iVar29 = 0x36;
              memtest_done = 3;
            }
          }
        }
        else if (memtest_done == 3) {
          NewMenu(cursor,-1,-1,-1);
          if (CutAudio[cutworldix] != -1) {
            gamesfx_channel = 4;
            GameSfx(CutAudio[cutworldix],(nuvec_s *)0x0);
          }
        }
        goto LAB_800346ac;
      }
      if ('\x02' < cVar14) {
        if (cVar14 == '\x03') {
          if (bVar1) {
            if (uVar5 != 0) {
              NuSoundKillAllAudio();
              XbWaitForAllBuffersToStop();
              XbLaunchDashboardToFreeSpace();
            }
            goto LAB_800346ac;
          }
          if (NotEnoughSpace_NewGame != 0) {
            iVar29 = 4;
            goto LAB_8003469c;
          }
          new_level = 0x25;
          force_menu = 0x15;
          if (cutmovie == 0) {
            fade_rate = 8;
          }
          goto LAB_800346a8;
        }
        if (cVar14 != '\x04') goto LAB_800346ac;
        if (bVar1) {
          if (uVar5 == 0) goto LAB_800346ac;
          iVar29 = 0;
          iVar18 = -1;
          goto LAB_800346a0;
        }
        iVar29 = 0x36;
        iVar18 = (int)cursor->y_max;
        if (iVar23 == iVar18 + -1) {
          CleanLetters(Game.name);
          iVar18 = strcmp(Game.name,"        ");
          if (iVar18 == 0) {
            Game.name[8] = '\0';
            Game.name._0_4_ = 0x43524153;
            Game.name._4_4_ = 0x48202020;
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
          pcVar19 = strstr(Game.name,"WOMBAT");
          if (pcVar19 != (char *)0x0) {
            Game.name._0_4_ = 0x43524153;
            Game.name[8] = '\0';
            Game.name._4_4_ = 0x48202020;
            cheating = 1;
            OpenGame();
          }
          goto LAB_800346ac;
        }
        if (iVar23 == iVar18) {
          iVar22 = -1;
          iVar18 = 0;
          goto LAB_80034684;
        }
        if (Game.language == 'c') {
          iVar22 = 2;
          if ((iVar23 != iVar18 + -2) && (iVar22 = 1, iVar23 != iVar18 + -3)) {
            if (iVar23 != iVar18 + -4) {
              if ((input_alphabet == 2) && ((cVar15 == '\0' || (cVar15 == '\x04'))))
              goto LAB_800346ac;
              goto LAB_80033630;
            }
            iVar22 = 0;
          }
          input_alphabet = iVar22;
          GetMenuInfo(cursor);
        }
        else {
LAB_80033630:
          InputNewLetter(cursor,Game.name,&i_nameinput,8);
        }
        goto LAB_800346ac;
      }
      if (cVar14 != '\0') {
        if (cVar14 != '\x01') goto LAB_800346ac;
        loadsaveCallEachFrame();
        if (bVar1) {
          if (uVar5 == 0) goto LAB_800346ac;
          iVar29 = -1;
          iVar18 = 0;
          goto LAB_80034620;
        }
        iVar18 = ParseNintendoErrorCode();
        iVar29 = 0x36;
        if (iVar18 == 0) {
          if (wasloadgame != 0) {
            new_level = 0x25;
            force_menu = 0x15;
            if (cutmovie == 0) {
              fade_rate = 8;
            }
            goto LAB_800346ac;
          }
          iVar18 = 4;
          goto LAB_80034680;
        }
        if (iVar23 == 0) {
          memtest_done = iVar23;
          MemCardRetry();
          iVar29 = ParseNintendoErrorCode();
          if (iVar29 == 0) {
            if (wasloadgame == 0) {
              iVar29 = 4;
            }
            else {
              iVar29 = 0;
            }
LAB_8003469c:
            iVar18 = 0;
            goto LAB_800346a0;
          }
        }
        else {
          if (iVar23 == 1) {
            if (wasloadgame == 0) {
              iVar29 = 4;
            }
            else {
              iVar29 = 0;
            }
            goto LAB_8003469c;
          }
          if (iVar23 == 2) {
            iVar29 = ParseNintendoErrorCode();
            if (iVar29 == 6) {
              Managememcard();
            }
            else if (iVar29 < 7) {
              if ((iVar29 < 4) && (1 < iVar29)) {
                iVar29 = 0x20;
                iVar18 = 1;
                goto LAB_800346a0;
              }
            }
            else if (iVar29 == 8) {
              SaveMenu = wasloadgame;
              if (wasloadgame != 0) {
                SaveMenu = 0;
              }
              NewMenu(cursor,0x30,1,-1);
            }
          }
        }
        goto LAB_800346a8;
      }
      wasloadgame = 0;
      loadsaveCallEachFrame();
      if (bVar1) {
        if (iVar23 == 3) {
          old_lang.356 = (int)Game.language;
          if (((uVar20 & 0x2000) == 0) || (4 < (uint)old_lang.356)) {
            if (((uVar20 & 0x8000) != 0) && (Game.language != '\0')) {
              Game.language = Game.language + 0xff;
            }
          }
          else {
            Game.language = Game.language + '\x01';
          }
        }
        goto LAB_80033140;
      }
      if (iVar23 == 1) {
        wasloadgame = iVar23;
        MemCardRetry();
        iVar29 = ParseNintendoErrorCode();
        if (iVar29 != 0) {
          DestMenu = 0;
          SaveMenu = 0;
          iVar29 = 1;
          goto LAB_800330a4;
        }
        Game.name._0_4_ = 0x43524153;
        Game.name[8] = '\0';
        Game.name._4_4_ = 0x48202020;
        new_level = 0x25;
        force_menu = 0x15;
        if (cutmovie == 0) {
          fade_rate = 8;
        }
LAB_800330b0:
        iVar29 = 0x36;
      }
      else if (iVar23 < 2) {
        if (iVar23 == 0) {
          MemCardRetry();
          iVar29 = ParseNintendoErrorCode();
          if (iVar29 == 0) {
            NewMenu(cursor,4,0,-1);
            goto LAB_800330b0;
          }
          DestMenu = 4;
          iVar29 = 1;
          SaveMenu = iVar23;
LAB_800330a4:
          NewMenu(cursor,iVar29,0,-1);
          goto LAB_800330b0;
        }
      }
      else {
        if (iVar23 == 2) {
          iVar29 = 0x27;
          goto LAB_800330a4;
        }
        if (iVar23 == 3) {
          old_lang.356 = (int)Game.language;
          Game.language = (uchar)(old_lang.356 + 1U);
          if (5 < (old_lang.356 + 1U & 0xff)) {
            Game.language = '\0';
          }
        }
      }
LAB_80033140:
      if ((uint)Game.language != old_lang.356) {
        NewLanguage((uint)Game.language);
        iVar29 = 0x36;
        old_lang.356 = (int)Game.language;
      }
      goto LAB_800346ac;
    }
    if (cVar14 == '\x11') {
      if (bVar1) goto LAB_800346ac;
      iVar29 = (int)cursor->y_max;
      if (iVar23 == iVar29) {
        iVar29 = newleveltime_slot * 8;
        iVar22 = Level * 0x1c + -0x7fcfdbf0;
        pcVar19 = (char *)(iVar22 + iVar29);
        CleanLetters(pcVar19);
        iVar18 = strcmp(pcVar19,"   ");
        if (iVar18 == 0) {
          *(char (*) [4])(iVar22 + iVar29) = (char  [4])0x20203f00;
        }
        if ((new_lev_flags & 7) == 0) {
          iVar29 = 0x10;
          goto LAB_8003469c;
        }
        new_level = 0x25;
        goto LAB_800346a8;
      }
      if (Game.language == 'c') {
        iVar18 = 2;
        if ((iVar23 != iVar29 + -1) && (iVar18 = 1, iVar23 != iVar29 + -2)) {
          if (iVar23 != iVar29 + -3) {
            if ((input_alphabet == 2) && ((iVar23 == 0 || (iVar23 == 4)))) goto LAB_800346a8;
            goto LAB_80033998;
          }
          iVar18 = 0;
        }
        input_alphabet = iVar18;
        GetMenuInfo(cursor);
      }
      else {
LAB_80033998:
        InputNewLetter(cursor,(char *)(Level * 0x1c + -0x7fcfdbf0 + newleveltime_slot * 8),
                       &i_nameinput,3);
      }
      goto LAB_800346a8;
    }
    if (cVar14 < '\x12') {
      if (cVar14 == '\x0e') {
        if (!bVar1) {
          iVar18 = (int)(char)(&HData[cursor->x * 3].flags)[iVar23];
          if (iVar18 != -1) {
            new_level = iVar18;
            Hub = HubFromLevel(iVar18);
          }
          goto LAB_800346ac;
        }
        if (uVar5 == 0) goto LAB_800346ac;
        iVar22 = -1;
        iVar18 = 0xb;
        goto LAB_80034684;
      }
      if ('\x0e' < cVar14) {
        if (cVar14 != '\x0f') {
          if (cVar14 != '\x10') goto LAB_800346ac;
LAB_80033850:
          if (bVar1) goto LAB_800346ac;
          iVar29 = 0x36;
          if (iVar23 != 0) {
            if (iVar23 != 1) goto LAB_800346ac;
            goto LAB_8003389c;
          }
          new_mode = GameMode;
          ResetCheckpoint(-1,-1,0.0,(nuvec_s *)0x0);
          goto LAB_8003388c;
        }
        if (bVar1) {
          if (uVar5 == 0) goto LAB_800346ac;
          iVar22 = -1;
          iVar18 = 0xb;
          goto LAB_80034684;
        }
        if (iVar23 == 2) {
LAB_80032a44:
          iVar18 = logos_played;
          iVar22 = iVar23;
        }
        else if (iVar23 < 3) {
          iVar18 = iVar23;
          iVar22 = iVar23;
          if ((iVar23 != 0) && (iVar18 = logos_played, iVar22 = cutmovie, iVar23 == 1))
          goto LAB_80032a44;
        }
        else if ((iVar23 == 3) || (iVar18 = logos_played, iVar22 = cutmovie, iVar23 == 4))
        goto LAB_80032a44;
        cutmovie = iVar22;
        logos_played = iVar18;
        NewMenu(cursor,-1,-1,-1);
        goto LAB_8003389c;
      }
      if (cVar14 != '\f') {
        if ((cVar14 != '\r') || (uVar5 == 0)) goto LAB_800346ac;
        iVar22 = -1;
        iVar18 = 0xb;
        goto LAB_80034684;
      }
      if (!bVar1) goto LAB_800346ac;
      if (uVar5 != 0) {
        iVar22 = -1;
        iVar18 = 0xb;
        goto LAB_80034684;
      }
      if (iVar23 == 5) {
        if ((uVar6 == 0) || (LDATA->fogb == '\0')) {
          if ((uVar7 != 0) && (LDATA->fogb != 0xff)) {
            LDATA->fogb = LDATA->fogb + '\x01';
          }
        }
        else {
          LDATA->fogb = LDATA->fogb + 0xff;
        }
        goto LAB_800346ac;
      }
      if (5 < iVar23) {
        if (iVar23 == 8) {
          if ((uVar6 == 0) || (LDATA->hazeg == '\0')) {
            if ((uVar7 != 0) && (LDATA->hazeg != 0xff)) {
              LDATA->hazeg = LDATA->hazeg + '\x01';
            }
          }
          else {
            LDATA->hazeg = LDATA->hazeg + 0xff;
          }
        }
        else if (iVar23 < 9) {
          if (iVar23 == 6) {
            if ((uVar6 == 0) || (LDATA->foga == '\0')) {
              if ((uVar7 != 0) && (LDATA->foga < 0x7f)) {
                LDATA->foga = LDATA->foga + 1;
              }
            }
            else {
              LDATA->foga = LDATA->foga + 0xff;
            }
          }
          else if (iVar23 == 7) {
            if ((uVar6 == 0) || (LDATA->hazer == '\0')) {
              if ((uVar7 != 0) && (LDATA->hazer != 0xff)) {
                LDATA->hazer = LDATA->hazer + '\x01';
              }
            }
            else {
              LDATA->hazer = LDATA->hazer + 0xff;
            }
          }
        }
        else if (iVar23 == 9) {
          if ((uVar6 == 0) || (LDATA->hazeb == '\0')) {
            if ((uVar7 != 0) && (LDATA->hazeb != 0xff)) {
              LDATA->hazeb = LDATA->hazeb + '\x01';
            }
          }
          else {
            LDATA->hazeb = LDATA->hazeb + 0xff;
          }
        }
        else if (iVar23 == 10) {
          if ((uVar6 == 0) || (LDATA->hazea == '\0')) {
            if ((uVar7 != 0) && (LDATA->hazea != 0xff)) {
              LDATA->hazea = LDATA->hazea + '\x01';
            }
          }
          else {
            LDATA->hazea = LDATA->hazea + 0xff;
          }
        }
        goto LAB_800346ac;
      }
      if (iVar23 == 2) {
        if (((uVar6 == 0) || (fVar21 = LDATA->fogfar, fVar21 <= 1.0)) || (fVar21 <= LDATA->fognear ))
        {
          if ((uVar7 != 0) && ((int)LDATA->fogfar < (int)(uint)LDATA->farclip)) {
            LDATA->fogfar = LDATA->fogfar + 1.0;
          }
        }
        else {
          LDATA->fogfar = fVar21 - 1.0;
        }
        goto LAB_800346ac;
      }
      if (2 < iVar23) {
        if (iVar23 == 3) {
          if ((uVar6 == 0) || (LDATA->fogr == '\0')) {
            if ((uVar7 != 0) && (LDATA->fogr != 0xff)) {
              LDATA->fogr = LDATA->fogr + '\x01';
            }
          }
          else {
            LDATA->fogr = LDATA->fogr + 0xff;
          }
        }
        else if (iVar23 == 4) {
          if ((uVar6 == 0) || (LDATA->fogg == '\0')) {
            if ((uVar7 != 0) && (LDATA->fogg != 0xff)) {
              LDATA->fogg = LDATA->fogg + '\x01';
            }
          }
          else {
            LDATA->fogg = LDATA->fogg + 0xff;
          }
        }
        goto LAB_800346ac;
      }
      if (iVar23 != 0) {
        if (iVar23 == 1) {
          if ((uVar6 == 0) || (LDATA->fognear <= 1.0)) {
            if ((uVar7 != 0) &&
               ((fVar21 = LDATA->fognear, (int)fVar21 < (int)(uint)LDATA->farclip &&
                (fVar21 < LDATA->fogfar)))) {
              LDATA->fognear = fVar21 + 1.0;
            }
          }
          else {
            LDATA->fognear = LDATA->fognear - 1.0;
          }
        }
        goto LAB_800346ac;
      }
      if ((uVar6 == 0) || (LDATA->farclip < 0xb)) {
        if ((uVar7 != 0) && (LDATA->farclip < 1000)) {
          uVar12 = LDATA->farclip + 1;
          goto LAB_80032b44;
        }
      }
      else {
        uVar12 = LDATA->farclip - 1;
LAB_80032b44:
        LDATA->farclip = uVar12;
      }
      if ((int)(uint)LDATA->farclip < (int)LDATA->fogfar) {
        local_38 = (double)CONCAT44(0x43300000,(uint)LDATA->farclip);
        LDATA->fogfar = (float)(local_38 - 4503599627370496.0);
      }
      local_38 = (double)CONCAT44(0x43300000,(int)LDATA->fognear ^ 0x80000000);
      if (LDATA->fogfar < (float)(local_38 - 4503601774854144.0)) {
        LDATA->fognear = LDATA->fogfar;
      }
      if (pNuCam != (nucamera_s *)0x0) {
        local_38 = (double)CONCAT44(0x43300000,(uint)LDATA->farclip);
        pNuCam->farclip = (float)(local_38 - 4503599627370496.0);
      }
      goto LAB_800346ac;
    }
    if (cVar14 == '\x14') {
      if ((TempAnim.newaction == 0x22) && (!bVar1)) {
        if (iVar23 == 0) {
          iVar29 = cortex_continue_i + 1;
          cortex_gameover_i = cortex_continue_i;
          cortex_continue_i = iVar29;
          if (iVar29 == 2) {
            cortex_continue_i = iVar23;
          }
          tempanim_nextaction = (int)*(short *)(cortex_gameover_tab + iVar18 * 4);
          if ((CRemap[2] == -1) ||
             (CModel[CRemap[2]].anmdata[tempanim_nextaction] == (nuAnimData_s *)0x0)) {
            Game.lives = '\x04';
            new_level = 0x25;
            just_continued = 1;
          }
          else {
            gamesfx_channel = 4;
            GameSfx((int)*(short *)(cortex_gameover_tab + iVar18 * 4 + 2),(nuvec_s *)0x0);
            tempanim_waitaudio = 1;
          }
          iVar29 = 0x36;
          gameover_hack = 2;
        }
        else if (iVar23 == 1) {
          iVar29 = cortex_quit_i + 1;
          cortex_gameover_i = cortex_quit_i;
          cortex_quit_i = iVar29;
          if (iVar29 == 7) {
            cortex_quit_i = 2;
          }
          tempanim_nextaction = (int)*(short *)(cortex_gameover_tab + iVar22 * 4);
          if ((CRemap[2] == -1) ||
             (CModel[CRemap[2]].anmdata[tempanim_nextaction] == (nuAnimData_s *)0x0)) {
            new_level = 0x23;
          }
          else {
            gamesfx_channel = 4;
            GameSfx((int)*(short *)(cortex_gameover_tab + iVar22 * 4 + 2),(nuvec_s *)0x0);
            tempanim_waitaudio = iVar23;
          }
          iVar29 = 0x3c;
          gameover_hack = 2;
        }
      }
      goto LAB_800346ac;
    }
    if ('\x14' < cVar14) {
      if (cVar14 != '\x15') {
        if (cVar14 != '\x16') goto LAB_800346ac;
        loadsaveCallEachFrame();
        if ((saveload_cardchanged != 0) || (saveload_error != 0)) {
          NewMenu(cursor,0x26,-1,-1);
        }
        UpdateSaveSlots(cursor);
        if (bVar1) {
          if (uVar5 == 0) goto LAB_800346ac;
          iVar29 = -1;
          iVar18 = 0x15;
        }
        else {
          if ((int)cursor->y < (int)cursor->y_max) {
            iVar29 = cursor->x * 2 + (int)cursor->y;
            game = SaveSlot + iVar29;
            if (SaveSlot[iVar29].empty != '\0') goto LAB_800342c4;
            iVar29 = 0x17;
            iVar18 = 1;
            goto LAB_800346a0;
          }
          iVar29 = -1;
          iVar18 = 0x15;
        }
        goto LAB_80034620;
      }
      bVar4 = false;
      loadsaveCallEachFrame();
      if (bVar1) {
        if (uVar5 != 0) {
LAB_80033a6c:
          bVar4 = true;
        }
        goto LAB_80033a70;
      }
      if (iVar23 == 1) {
        memcard_loadattempted = 0;
        UpdateSaveSlots(cursor);
        iVar18 = 0x19;
LAB_80033a54:
        iVar29 = 0x36;
        NewMenu(cursor,iVar18,-1,-1);
      }
      else if (iVar23 < 2) {
        if (iVar23 == 0) {
          memcard_loadattempted = iVar23;
          UpdateSaveSlots(cursor);
          iVar18 = 0x16;
          goto LAB_80033a54;
        }
      }
      else {
        if (iVar23 == 2) {
          memcard_loadattempted = 0;
          UpdateSaveSlots(cursor);
          iVar18 = 0x1d;
          goto LAB_80033a54;
        }
        if (iVar23 == 3) goto LAB_80033a6c;
      }
LAB_80033a70:
      if (bVar4) {
        NewMenu(cursor,-1,-1,-1);
        iVar29 = 0x3c;
        ResetLoadSaveCharacter();
      }
      goto LAB_800346ac;
    }
    if (cVar14 == '\x12') goto LAB_80033850;
    if ((cVar14 != '\x13') || ((((uint)fVar21 & 0x840) == 0 && (cut_on != 0)))) goto LAB_800346ac;
    iVar29 = (int)cursor->new_level;
    if (cursor->new_level == -1) {
      NewMenu(cursor,-1,-1,-1);
      iVar29 = new_level;
    }
  }
  else {
    if (cVar14 == '#') {
      if ((((uint)fVar21 & 0x840) == 0) && (GameTimer.ftime < credit_time)) goto LAB_800346ac;
LAB_8003389c:
      new_level = 0x25;
      goto LAB_800346ac;
    }
    iVar18 = SaveMenu;
    if ('#' < cVar14) {
      if (cVar14 == '*') {
        iVar18 = GBA_Download();
        if (iVar18 == 0) goto LAB_800346ac;
        iVar18 = 0x2b;
LAB_80034680:
        iVar22 = 0;
        goto LAB_80034684;
      }
      if ('*' < cVar14) {
        if (cVar14 != '-') {
          if ('-' < cVar14) {
            if (cVar14 == '0') {
              if (bVar1) {
                if (uVar5 == 0) goto LAB_800346ac;
                iVar29 = -1;
              }
              else {
                if (iVar23 != 1) {
                  iVar29 = 0x36;
                  memcard_formatmessage_delay = 0x28;
                  MemCardDelete();
                  memtest_done = 0;
                  iVar18 = 0x31;
                  goto LAB_80034680;
                }
                iVar29 = -1;
              }
            }
            else {
              if ('0' < cVar14) {
                if (cVar14 != '1') goto LAB_800346ac;
                loadsaveCallEachFrame();
                UpdateSaveSlots(cursor);
                if (memcard_formatmessage_delay != 0) goto LAB_800346ac;
                iVar22 = 3;
                iVar18 = SaveMenu;
                goto LAB_80034684;
              }
              if (cVar14 != '/') goto LAB_800346ac;
              if (!bVar1) {
                iVar22 = ParseNintendoErrorCode();
                iVar18 = SaveMenu;
                if (iVar22 == 0) goto LAB_80034680;
                if (iVar23 == 0) {
                  memtest_done = iVar23;
                  MemCardRetry();
                  iVar29 = SaveMenu;
                  goto LAB_8003469c;
                }
                if (iVar23 != 1) {
                  if (iVar23 == 2) {
                    iVar29 = ParseNintendoErrorCode();
                    if (iVar29 == 6) {
                      Managememcard();
                    }
                    else if (iVar29 < 7) {
                      if ((iVar29 < 4) && (1 < iVar29)) {
                        iVar29 = 0x20;
                        iVar18 = 1;
                        goto LAB_800346a0;
                      }
                    }
                    else if (iVar29 == 8) {
                      SaveMenu = 0x15;
                      NewMenu(cursor,0x30,1,-1);
                    }
                  }
                  goto LAB_800346a8;
                }
                iVar29 = 0x15;
                iVar18 = -1;
                goto LAB_800346a0;
              }
              if (uVar5 == 0) goto LAB_800346ac;
              iVar29 = -1;
              iVar18 = BackMenu;
            }
            goto LAB_80034620;
          }
          if (cVar14 == '+') {
            iVar18 = GBA_DownloadEnd();
            if (iVar18 == -1) {
              iVar18 = 0x2d;
            }
            else {
              if (iVar18 != 0) goto LAB_800346ac;
              iVar18 = 0x2c;
            }
            goto LAB_80034680;
          }
          if (cVar14 != ',') goto LAB_800346ac;
        }
        if (bVar1) goto LAB_800346ac;
        iVar29 = 0x27;
        goto LAB_8003469c;
      }
      if (cVar14 == '\'') {
        if (bVar1) {
          if (uVar5 == 0) goto LAB_800346ac;
          iVar29 = 0;
          iVar18 = -1;
        }
        else {
          if (iVar23 == 0) {
            iVar18 = 0x28;
            goto LAB_8003461c;
          }
          if (iVar23 != 1) goto LAB_800346ac;
          iVar29 = 0;
          iVar18 = -1;
        }
      }
      else {
        if ('\'' < cVar14) {
          if (cVar14 == '(') {
            if (!bVar1) {
              iVar29 = 0x29;
              goto LAB_8003469c;
            }
            if (uVar5 == 0) goto LAB_800346ac;
            iVar18 = 0x27;
LAB_8003461c:
            iVar29 = 0;
            goto LAB_80034620;
          }
          if (cVar14 != ')') goto LAB_800346ac;
          GBA_DownloadStart();
          iVar18 = 0x2a;
          goto LAB_80034680;
        }
        if (cVar14 == '$') {
          if (uVar16 == 0) {
            pmVar27 = (player->obj).mask;
            if ((pmVar27 != (mask *)0x0) &&
               ((((pmVar27->anim).flags & 1) != 0 ||
                ((((pmVar27->sfx != -1 && (pmVar27->hold == '\0')) &&
                  (iVar18 = NuSoundKeyStatus(4), iVar18 != 1)) ||
                 ((0x707 < cursor->item_frame || (2 < ((player->obj).mask)->active)))))))) {
              NewMenu(cursor,-1,-1,-1);
            }
          }
          else {
            iVar29 = 0x36;
            NuSoundStopStream(4);
            NewMenu(cursor,-1,-1,-1);
          }
          if (cursor->menu != '$') {
            advice_wait = 0x1e;
          }
          goto LAB_800346ac;
        }
        if (cVar14 != '&') goto LAB_800346ac;
        loadsaveCallEachFrame();
        UpdateSaveSlots(cursor);
        if (bVar1) goto LAB_800346ac;
        if (saveload_cardchanged != 0) {
          saveload_cardchanged = 0;
        }
        saveload_error = 0;
        iVar29 = 0x15;
        iVar18 = -1;
      }
LAB_800346a0:
      NewMenu(cursor,iVar29,iVar18,-1);
LAB_800346a8:
      iVar29 = 0x36;
      goto LAB_800346ac;
    }
    if (cVar14 == '\x1d') {
      loadsaveCallEachFrame();
      if ((saveload_cardchanged != 0) || (saveload_error != 0)) {
        if ((saveload_error == 2) || (saveload_error == 0x10)) {
          saveload_error = 8;
        }
        NewMenu(cursor,0x26,-1,-1);
      }
      UpdateSaveSlots(cursor);
      if (bVar1) {
        if (uVar5 == 0) goto LAB_800346ac;
        iVar29 = -1;
        iVar18 = 0x15;
      }
      else {
        if ((int)cursor->y < (int)cursor->y_max) {
          iVar29 = cursor->x * 2 + (int)cursor->y;
          game = SaveSlot + iVar29;
          if (SaveSlot[iVar29].empty != '\0') {
LAB_800342c4:
            iVar29 = 2;
            goto LAB_800346ac;
          }
          iVar29 = 0x1e;
          iVar18 = 1;
          goto LAB_800346a0;
        }
        iVar29 = -1;
        iVar18 = 0x15;
      }
    }
    else {
      if (cVar14 < '\x1e') {
        if (cVar14 != '\x1a') {
          if (cVar14 < '\x1b') {
            if (cVar14 != '\x18') {
              if (cVar14 != '\x19') goto LAB_800346ac;
              loadsaveCallEachFrame();
              if ((saveload_cardchanged != 0) || (saveload_error != 0)) {
                saveload_error = 4;
                NewMenu(cursor,0x26,-1,-1);
              }
              UpdateSaveSlots(cursor);
              if (bVar1) {
                if (uVar5 == 0) goto LAB_800346ac;
                iVar29 = -1;
                iVar18 = 0x15;
              }
              else {
                if (((memcard_formatting != 0) || (memcard_formatme != 0)) ||
                   (memcard_formatmessage_delay != 0)) goto LAB_800346ac;
                if ((int)cursor->y < (int)cursor->y_max) {
                  iVar29 = 0x36;
                  game = SaveSlot + cursor->x * 2 + (int)cursor->y;
                  NewMenu(cursor,0x1b,1,-1);
                  goto LAB_800346ac;
                }
                iVar29 = 1;
                iVar18 = 0x15;
              }
              goto LAB_80034620;
            }
            loadsaveCallEachFrame();
            UpdateSaveSlots(cursor);
            if ((saveload_cardchanged == 0) && (saveload_error == 0)) {
              if (memcard_loadresult_delay == 0) {
                NewMenu(cursor,-1,-1,-1);
                pnVar9 = pos_START;
                pcVar8 = player;
                Hub = -1;
                bVar1 = pos_START != (nuvec_s *)0x0;
                tumble_time = 0.0;
                tumble_action = -1;
                tumble_duration = 0.0;
                last_level = -1;
                last_hub = -1;
                (player->obj).hdg = 0x8000;
                if (bVar1) {
                  fVar21 = pnVar9->z;
                  fVar24 = pnVar9->y;
                  (pcVar8->obj).pos.x = pnVar9->x;
                  (pcVar8->obj).pos.z = fVar21;
                  (pcVar8->obj).pos.y = fVar24;
                }
              }
              goto LAB_800346ac;
            }
            iVar22 = 1;
            iVar18 = 0x26;
          }
          else {
            if (cVar14 == '\x1b') {
              loadsaveCallEachFrame();
              if ((saveload_cardchanged != 0) || (saveload_error != 0)) {
                if ((saveload_error == 2) || (saveload_error == 0x10)) {
                  saveload_error = 4;
                }
                NewMenu(cursor,0x26,-1,-1);
              }
              if (bVar1) {
                if (uVar5 == 0) goto LAB_800346ac;
                iVar29 = -1;
                iVar18 = 0x19;
              }
              else {
                if (cursor->y != '\x01') {
                  XbUpdateDateStamp((int)game);
                  if (game->empty != '\0') {
                    memcard_gamesavailable = memcard_gamesavailable + 1;
                  }
                  iVar29 = 0x408;
                  pGVar10 = &Game;
                  pGVar11 = game;
                  do {
                    pGVar28 = pGVar11;
                    pGVar25 = pGVar10;
                    iVar29 = iVar29 + -0x18;
                    *(undefined4 *)pGVar28->name = *(undefined4 *)pGVar25->name;
                    *(undefined4 *)(pGVar28->name + 4) = *(undefined4 *)(pGVar25->name + 4);
                    *(undefined4 *)(pGVar28->name + 8) = *(undefined4 *)(pGVar25->name + 8);
                    *(undefined4 *)&pGVar28->music_volume = *(undefined4 *)&pGVar25->music_volume;
                    pGVar28->hub[0] = pGVar25->hub[0];
                    pGVar28->hub[1] = pGVar25->hub[1];
                    pGVar10 = (Game_s *)(pGVar25->hub + 2);
                    pGVar11 = (Game_s *)(pGVar28->hub + 2);
                  } while (iVar29 != 0);
                  *(undefined4 *)((Game_s *)(pGVar28->hub + 2))->name =
                       *(undefined4 *)((Game_s *)(pGVar25->hub + 2))->name;
                  iVar29 = 0x36;
                  pGVar28->hub[3] = pGVar25->hub[3];
                  pGVar28->hub[4] = pGVar25->hub[4];
                  memcard_saveneeded = 1;
                  NewMenu(cursor,0x1c,0,-1);
                  goto LAB_800346ac;
                }
                iVar29 = -1;
                iVar18 = 0x19;
              }
              goto LAB_80034620;
            }
            if (cVar14 != '\x1c') goto LAB_800346ac;
LAB_800340f0:
            loadsaveCallEachFrame();
            UpdateSaveSlots(cursor);
            if ((((memcard_saveneeded != 0) || (memcard_savestarted != 0)) ||
                (memcard_deleteneeded != 0)) ||
               ((memcard_deletestarted != 0 || (memcard_savemessage_delay != 0)))) {
              memcard_saveresult_delay = 0x3c;
            }
            if ((saveload_cardchanged == 0) && (saveload_error == 0)) {
              if (memcard_saveresult_delay != 0) goto LAB_800346ac;
              iVar22 = 3;
              iVar18 = 0x15;
            }
            else {
              if (((saveload_error == 2) || (saveload_error == 0x10)) &&
                 (saveload_error = 8, cursor->menu == '\x1c')) {
                saveload_error = 4;
              }
              iVar22 = -1;
              iVar18 = 0x26;
            }
          }
          goto LAB_80034684;
        }
        loadsaveCallEachFrame();
        if (saveload_cardchanged != 0) {
          NewMenu(cursor,0x26,-1,-1);
        }
        if (bVar1) {
          if (uVar5 != 0) {
            NewMenu(cursor,0x15,-1,-1);
            iVar29 = 0x3c;
          }
        }
        else {
          iVar29 = (int)cursor->y_max;
          if (iVar23 == iVar29 + -1) {
            CleanLetters(Game.name);
            iVar29 = strcmp(Game.name,"        ");
            if (iVar29 == 0) {
              Game.name._0_4_ = 0x43524153;
              Game.name[8] = '\0';
              Game.name._4_4_ = 0x48202020;
            }
            iVar29 = 0x19;
            iVar18 = 2;
LAB_80033b40:
            NewMenu(cursor,iVar29,iVar18,-1);
          }
          else {
            if (iVar23 == iVar29) {
              iVar29 = 0x15;
              iVar18 = -1;
              goto LAB_80033b40;
            }
            if (Game.language == 'c') {
              iVar18 = 2;
              if ((iVar23 != iVar29 + -2) && (iVar18 = 1, iVar23 != iVar29 + -3)) {
                if (iVar23 != iVar29 + -4) {
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
          iVar29 = 0x36;
        }
        if (cursor->menu == '\x15') {
          Game.name._0_4_ = 0x43524153;
          Game.name[8] = '\0';
          Game.name._4_4_ = 0x48202020;
        }
        goto LAB_800346ac;
      }
      if (cVar14 == ' ') {
        if (bVar1) {
          if (uVar5 == 0) goto LAB_800346ac;
          iVar29 = -1;
        }
        else {
          if (iVar23 != 1) {
            memcard_formatme = 1;
            iVar29 = 0x21;
            goto LAB_8003469c;
          }
          iVar29 = -1;
        }
      }
      else {
        if (' ' < cVar14) {
          if (cVar14 != '!') {
            if ((cVar14 != '\"') || (uVar16 == 0)) goto LAB_800346ac;
            if ((Level == 0x16) || (Level == 0x18)) {
              boss_dead = 2;
            }
            iVar29 = -1;
            iVar18 = -1;
            goto LAB_800346a0;
          }
          loadsaveCallEachFrame();
          UpdateSaveSlots(cursor);
          if (memcard_formatme != 0) {
            memcard_formatmessage_delay = 0x28;
            goto LAB_800346ac;
          }
          if (memcard_formatmessage_delay != 0) goto LAB_800346ac;
          iVar22 = 3;
          iVar18 = SaveMenu;
          goto LAB_80034684;
        }
        if (cVar14 != '\x1e') {
          if (cVar14 != '\x1f') goto LAB_800346ac;
          goto LAB_800340f0;
        }
        loadsaveCallEachFrame();
        if ((saveload_cardchanged != 0) || (saveload_error != 0)) {
          if ((saveload_error == 2) || (saveload_error == 0x10)) {
            saveload_error = 8;
          }
          NewMenu(cursor,0x26,-1,-1);
        }
        if (bVar1) {
          if (uVar5 == 0) goto LAB_800346ac;
          iVar29 = -1;
          iVar18 = 0x1d;
        }
        else {
          if (cursor->y != '\x01') {
            bVar1 = true;
            iVar29 = 3;
            game->empty = '\x01';
            puVar26 = &SaveSlot[0].empty;
            do {
              uVar13 = *puVar26;
              puVar26 = puVar26 + 0x414;
              if (uVar13 == '\0') {
                bVar1 = false;
              }
              iVar29 = iVar29 + -1;
            } while (iVar29 != 0);
            memcard_gamesavailable = memcard_gamesavailable + -1;
            if (bVar1) {
              memcard_deleteneeded = 1;
            }
            else {
              memcard_saveneeded = 1;
            }
            iVar29 = 0x1f;
            goto LAB_8003469c;
          }
          iVar29 = -1;
          iVar18 = 0x1d;
        }
      }
    }
LAB_80034620:
    NewMenu(cursor,iVar18,iVar29,-1);
    iVar29 = new_level;
  }
  new_level = iVar29;
  iVar29 = 0x3c;
LAB_800346ac:
  if (iVar29 != -1) {
    GameSfx(iVar29,(nuvec_s *)0x0);
  }
  return;
}