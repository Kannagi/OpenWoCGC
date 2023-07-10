#include "nutxanm.h"
#include "../system.h"


s32 nta_labels[64];
static struct nutexanimprog_s* parprog; 


void NuTexAnimProgSysInit(void)

{
  struct nutexanimlist_s **succ;
  struct nutexanimlist_s *nta;
  s32 n;
  
  sys_progs = NULL;
  nta = ntalsysbuff[63]; //0x803c85f0
  n = 0x3f;
  succ = &ntalsysbuff[0x3e].succ;
  do {
    *succ = nta;
    succ = succ + -3;
    nta = nta + -1;
    n = n + -1;
  } while (n != 0);
  ntal_free = ntalsysbuff;
  ntalsysbuff[63].succ = NULL;
  nta_sig_on = 0;
  ntal_first = NULL;
  xdeflabtabcnt = 0;
  nta_sig_old = 0;
  nta_sig_off = 0;
  return;
}


struct nutexanimprog_s * NuTexAnimProgFind(char *name)

{
  s32 cmp;
  nutexanimprog_s *rv;
  
  rv = sys_progs;
  while ((rv != NULL && (cmp = strcasecmp(name,rv->name), cmp != 0))) {
    rv = rv->succ;
  }
  return rv;
}


//PS2
void NuTexAnimProgInit(nutexanimprog_s *rv)
{

if (rv != 0) {
        s32 i;
        for(i = 0; i < 32; i++) {
            rv->off_sig[i] = -1;
            rv->on_sig[i] = -1;
        }
      rv->eop = 0;	//*(uint *)&rv->eop = *(uint *)&rv->eop & 0xffff7fff;
      rv->on_mask = 0;
      rv->off_mask = 0;
      rv->name[0] = '\0';
      rv->dynalloc = 0;
      rv->xdef_cnt = 0;
    }
  return;
}


/*void NuTexAnimProgAssembleEnd(nutexanimprog_s *p)

{
  short ix;
  int i;
  int j;
  short code;
  
  i = 0;
  if (p->eop < 1) {
    return;
  }
  do {
    code = p->code[i];
    ix = (short)i;
    if (code == 8) {
LAB_800ba2fc:
      j = i + 1;
      ix = ix + 2;
LAB_800ba304:
      i = (int)ix;
      p->code[j] = *(short *)((int)nta_labels + p->code[j] * 4 + 2);
      goto LAB_800ba354;
    }
    if (code < 9) {
      if (code < 5) {
        if (code < 3) {
          if (code == 1) {
            ix = ix + 4;
          }
          else if (code < 2) {
            if (code != 0) goto LAB_800ba354;
LAB_800ba34c:
            ix = ix + 2;
          }
          else {
            ix = ix + 3;
          }
          goto LAB_800ba350;
        }
      }
      else {
        if (code == 5) {
          ix = ix + 3;
          goto LAB_800ba350;
        }
        if (code == 7) goto LAB_800ba2fc;
      }
    }
    else {
      if (code == 0xc) {
        ix = ix + 1;
      }
      else if (code < 0xd) {
        if (code == 10) {
          ix = ix + 1;
        }
        else {
          if (code < 0xb) {
            j = i + 3;
            ix = ix + 4;
            goto LAB_800ba304;
          }
          ix = ix + 3;
        }
      }
      else if (code == 0xe) {
        ix = ix + 1;
      }
      else {
        if (0xd < code) {
          if (code == 0xf) goto LAB_800ba34c;
          goto LAB_800ba354;
        }
        ix = ix + 3;
      }
LAB_800ba350:
      i = (int)ix;
    }
LAB_800ba354:
    if (p->eop <= i) {
      return;
    }
  } while( true );
}


//DECOMP2


void NuTexAnimProgAssembleEnd(struct nutexanimprog_s* p) {
    s16 temp_r0;
    s16 var_r0;
    s16 var_r0_2;
    s16 var_r10;
    s16* temp_r8;
    s32 var_r11;

    var_r10 = 0;
    if ((s16) p->eop > 0) {
        temp_r8 = p->code;
        do {
            temp_r0 = temp_r8[var_r10];
            switch (temp_r0) {                      /* irregular */
            case 1:
                var_r0 = var_r10 + 4;
block_34:
                var_r10 = (s16) var_r0;
                break;
            case 2:
                var_r0 = var_r10 + 3;
                goto block_34;
            case 5:
                var_r0 = var_r10 + 3;
                goto block_34;
            case 9:
                var_r11 = var_r10 + 3;
                var_r0_2 = var_r10 + 4;
block_27:
                var_r10 = (s16) var_r0_2;
                temp_r8[var_r11] = (s16) nta_labels[temp_r8[var_r11]].unk2;
                break;
            case 8:
            case 7:
                var_r11 = var_r10 + 1;
                var_r0_2 = var_r10 + 2;
                goto block_27;
            case 10:
                var_r0 = var_r10 + 1;
                goto block_34;
            case 11:
                var_r0 = var_r10 + 3;
                goto block_34;
            case 12:
                var_r0 = var_r10 + 1;
                goto block_34;
            case 13:
                var_r0 = var_r10 + 3;
                goto block_34;
            case 14:
                var_r0 = var_r10 + 1;
                goto block_34;
            case 15:
            case 0:
                var_r0 = var_r10 + 2;
                goto block_34;
            }
        } while (var_r10 < (s16) p->eop);
    }
}



*/

//PS2
struct nutexanimenv_s * NuTexAnimEnvCreate(union variptr_u *buff,struct numtl_s *mtl,s16 *tids, struct nutexanimprog_s *p)
{
  struct nutexanimenv_s *rv;
  
  if (buff != NULL) {
    rv = (struct nutexanimenv_s *)(((s32)buff->voidptr + 3) & 0xfffffffc);
    buff->voidptr = rv + 1;
  }
  else {
      rv= (struct nutexanimenv_s *)NuMemAlloc(0xEC); //rv = (struct nutexanimenv_s *)NuMemAllocFn(0xec,"..\\nu2.ps2\\nu3d\\nutexanm.c",0x17a);
  }
  if (rv != NULL) {
    rv->prog = p;
    rv->mtl = mtl;
    rv->tids = tids;
    rv->pc = 0;
    rv->rep_ix = 0;
    rv->ra_ix = 0;
    rv->pause = 0;
    rv->pause_r = 0;
    rv->pause_cnt = 0;
    rv->tex_ix = 0;
    if (buff != NULL) {
      rv->dynalloc = rv->dynalloc & 0xfffffffe;	//0x7fffffff
    }
    else {
     rv->dynalloc = rv->dynalloc | 1;	// 0x80000000
    }
  }
  return rv;
}


//PS2
struct nutexanimprog_s * NuTexAnimProgReadScript(union variptr_u *buff,char *fname)
{
  struct nutexanimprog_s *rv;
  struct nufpar_s *fp;
  union variptr_u *ptr;
  
  if (buff != NULL) {
    ptr = (union variptr_u *)(s32)((buff->intaddr + 3) & ~3);
  } else {
    ptr = NuMemAllocFn(0x400,"..\\nu2.ps2\\nu3d\\nutexanm.c",0x2f9);
  }
  memset(labtab,0,0x540);
  labtabcnt = 0;
  fp = NuFParCreate(fname);
    rv = NULL;
  if (fp != NULL) {
    NuFParPushCom(fp,nutexanimcomtab);
    rv = (struct nutexanimprog_s*)ptr;
    NuTexAnimProgInit(rv);
      parprog = rv;
    while (NuFParGetLine(fp)) {
        int n = NuFParGetWord(fp);
      if (n && !NuFParInterpretWord(fp) && fp->wbuff[0]) {
          if (fp->wbuff[n - 1] == ':') {
              fp->wbuff[n - 1] = '\0';
              nta_labels[LabTabFind(fp->wbuff)] = parprog->eop;
          }
      }
    }
    if (buff != NULL) {
      buff->voidptr = rv->code + rv->eop;
    }
    NuFParDestroy(fp);
    NuTexAnimProgAssembleEnd(rv);
    rv->succ = sys_progs;
    if (sys_progs != NULL) {
      sys_progs->prev = rv;
    }
    rv->prev = NULL;
    sys_progs = rv;
  }
  return rv;
}



void NuTexAnimXCall(int lid,nutexanimenv_s *ignore)

{
  short *psVar1;
  s32 iVar2;	//xdef_cnt?
  nutexanim_s *nta;
  s32 n;
  short *xdef_addrs;
  short *addrs;
  nutexanimprog_s *p;
  nutexanimenv_s *e;
  nutexanimlist_s *rv;
  short pc;
  
  rv = ntal_first;
  if (ntal_first == NULL) {
    return;
  }
  do {
    for (nta = rv->nta; nta != NULL; nta = nta->succ) {
      e = nta->env;
      if ((((e != NULL) && (e != ignore)) &&
          (p = e->prog, p != NULL)) && (iVar2 = p->xdef_cnt, iVar2 != 0)) {
        n = 0;
        if (0 < iVar2) {
          xdef_addrs = p->xdef_addrs;
          if (p->xdef_ids[0] == lid) {
            pc = p->xdef_addrs[0];
          }
          else {
            do {
              n = n + 1;
              addrs = xdef_addrs + 1;
              if (iVar2 <= n) goto LAB_800bb11c;
              psVar1 = xdef_addrs + -0x1f;
              xdef_addrs = addrs;
            } while (*psVar1 != lid);
            pc = *addrs;
          }
          e->rep_ix = 0;
          e->pc = (int)pc;
          e->pause_cnt = 0;
          e->ra_ix = 0;
        }
      }
LAB_800bb11c:
    }
    rv = rv->succ;
    if (rv == NULL) {
      return;
    }
  } while( true );
}


/*		//DECOMP 2


void NuTexAnimXCall(s32 lid, struct nutexanimenv_s* ignore) {
    s16 var_r0;
    s16* var_r9;
    s32 temp_r0;
    s32 var_r6;
    struct nutexanim_s* var_r5;
    struct nutexanimenv_s* temp_r11;
    struct nutexanimlist_s* var_r12;
    struct nutexanimprog_s* temp_r10;

    var_r12 = ntal_first;
    if (var_r12 != NULL) {
        do {
            var_r5 = var_r12->nta;
            if (var_r5 != NULL) {
                do {
                    temp_r11 = var_r5->env;
                    if ((temp_r11 != NULL) && (temp_r11 != ignore)) {
                        temp_r10 = temp_r11->prog;
                        if (temp_r10 != NULL) {
                            temp_r0 = temp_r10->xdef_cnt;
                            if (temp_r0 != 0) {
                                var_r6 = 0;
                                var_r9 = temp_r10->xdef_addrs;
                                if (temp_r0 > 0) {
                                    if ((s16) var_r9->unk-40 == lid) {
                                        var_r0 = temp_r10->xdef_addrs[0];
                                        goto block_12;
                                    }
loop_9:
                                    var_r6 += 1;
                                    var_r9 += 2;
                                    if (var_r6 < temp_r0) {
                                        if ((s16) var_r9->unk-40 == lid) {
                                            var_r0 = var_r9->unk0;
block_12:
                                            temp_r11->rep_ix = 0;
                                            temp_r11->pc = (s32) var_r0;
                                            temp_r11->pause_cnt = 0;
                                            temp_r11->ra_ix = 0;
                                        } else {
                                            goto loop_9;
                                        }
                                    }
                                }
                            }
                        }
                    }
                    var_r5 = var_r5->succ;
                } while (var_r5 != NULL);
            }
            var_r12 = var_r12->succ;
        } while (var_r12 != NULL);
    }
}

*/


void NuTexAnimEnvProc(struct nutexanimenv_s *e)	//CHECK

{
  short sVar1;
  int iVar2;
  long rand;
  nutexanimprog_s *p;
  uint off;
  int n;
  int *sig;
  int pause_r;
  bool check;
  
  check = false;
  p = e->prog;
  if (p != NULL) {
    off = p->off_mask;
    if ((nta_sig_off & off) != 0) {
      n = 0;
      sig = p->off_sig;
      if ((nta_sig_off & 1 & off) == 0) {
        do {
          n = n + 1;
          sig = sig + 1;
          if (0x1f < n) goto LAB_800bb1dc;
        } while ((nta_sig_off & 1 << n & off) == 0);
        n = *sig;
        e->rep_ix = 0;
        e->pc = n;
        e->pause_cnt = 0;
        e->ra_ix = 0;
      }
      else {
        n = p->off_sig[0];
        e->pause_cnt = 0;
        e->pc = n;
        e->ra_ix = 0;
        e->rep_ix = 0;
      }
    }
LAB_800bb1dc:
    off = p->on_mask;
    if ((nta_sig_on & off) != 0) {
      n = 0;
      sig = p->on_sig;
      if ((nta_sig_on & 1 & off) == 0) {
        do {
          n = n + 1;
          sig = sig + 1;
          if (0x1f < n) goto LAB_800bb24c;
        } while ((nta_sig_on & 1 << n & off) == 0);
        n = *sig;
      }
      else {
        n = p->on_sig[0];
      }
      e->rep_ix = 0;
      e->pc = n;
      e->pause_cnt = 0;
      e->ra_ix = 0;
    }
LAB_800bb24c:
    if (e->pause_cnt == 0) {
      do {
        n = e->pc;
        sVar1 = p->code[n];
        if (sVar1 == 9) {
          n = EvalVars((int)p->code[n + 1],e->tex_ix,(int)*(short *)(p[1].name + n * 2 + -8));
          if (n != 0) {
            n = e->pc + 3;
LAB_800bb510:
            e->pc = (int)p->code[n];
            goto LAB_800bb770;
          }
          n = e->pc + 4;
LAB_800bb76c:
          e->pc = n;
        }
        else if (sVar1 < 10) {
          if (sVar1 == 2) {
            e->pause_cnt = (int)p->code[n + 1];
            if (*(short *)(p[1].name + n * 2 + -8) != 0) {
              rand = NuRand(0);
              e->pause_cnt = e->pause_cnt +
                             (rand - (rand / (int)*(short *)(p[1].name + e->pc * 2 + -8)) *
                                     (int)*(short *)(p[1].name + e->pc * 2 + -8));
            }
            check = true;
            n = e->pc + 3;
            goto LAB_800bb76c;
          }
          if (sVar1 < 3) {
            if (sVar1 == 0) {
              sVar1 = p->code[n + 1];
              e->tex_ix = (int)sVar1;
              e->mtl->tid = (int)e->tids[sVar1];
              n = e->pause;
              pause_r = e->pause_r;
              iVar2 = e->pc + 2;
            }
            else {
              if (sVar1 != 1) goto LAB_800bb770;
              iVar2 = e->tex_ix + (int)p->code[n + 1];
              if (iVar2 < *(short *)(p[1].name + n * 2 + -8)) {
                iVar2 = (int)*(short *)(p[1].name + n * 2 + -8);
              }
              if (*(short *)(p[1].name + n * 2 + -6) < iVar2) {
                iVar2 = (int)*(short *)(p[1].name + n * 2 + -6);
              }
              e->tex_ix = iVar2;
              e->mtl->tid = (int)e->tids[iVar2];
              n = e->pause;
              pause_r = e->pause_r;
              iVar2 = e->pc + 4;
            }
            e->pc = iVar2;
            e->pause_cnt = n;
            if (pause_r != 0) {
              rand = NuRand((nunrand_s *)0x0);
              e->pause_cnt = e->pause_cnt + (rand - (rand / e->pause_r) * e->pause_r);
            }
LAB_800bb748:
            check = true;
          }
          else {
            if (sVar1 == 7) {
              n = (int)p->code[n + 1];
              goto LAB_800bb76c;
            }
            if (7 < sVar1) {
              if (0xf < e->ra_ix) {
                err = NuErrorProlog("C:/source/crashwoc/code/nu3dx/nutexanm.c",0x388);
                //(*error)("TexAnim Processor Alert: Call Stack Overflow at (%d)",e->pc);
              }
              n = e->ra_ix;
              e->ra[n] = e->pc + 2;
              e->ra_ix = n + 1;
              n = e->pc + 1;
              goto LAB_800bb510;
            }
            if (sVar1 == 5) {
              e->pause = (int)p->code[n + 1];
              sVar1 = *(short *)(p[1].name + n * 2 + -8);
              e->pc = n + 3;
              e->pause_r = (int)sVar1;
            }
          }
        }
        else if (sVar1 == 0xc) {
          if (e->rep_ix == 0) {
            err = NuErrorProlog("C:/source/crashwoc/code/nu3dx/nutexanm.c",0x3a2);
            //(*error)("TexAnim Processor Alert: REPEND without REPEAT at (%d)",e->pc);
          }
          n = e->rep_ix;
          if (e->rep_count[n + -1] == 0) {
            e->rep_ix = n + -1;
            n = e->pc + 1;
            goto LAB_800bb76c;
          }
LAB_800bb730:
          e->pc = e->rep_start[n + -1];
          e->rep_count[n + -1] = e->rep_count[n + -1] + -1;
        }
        else if (sVar1 < 0xd) {
          if (sVar1 == 10) {
            if (e->ra_ix == 0) {
              err = NuErrorProlog("C:/source/crashwoc/code/nu3dx/nutexanm.c",0x390);
              //(*error)("TexAnim Processor Alert: Call Stack Underflow at (%d)",e->pc);
            }
            n = e->ra_ix;
            e->ra_ix = n + -1;
            e->pc = e->ra[n + -1];
          }
          else if (sVar1 == 0xb) {
            if (0xf < e->rep_ix) {
              err = NuErrorProlog("C:/source/crashwoc/code/nu3dx/nutexanm.c",0x398);
              //(*error)("TexAnim Processor Alert: Too Many Nested Repeat Loops at (%d)",e->pc);
            }
            e->rep_count[e->rep_ix] = (int)p->code[e->pc + 1];
            if (*(short *)(p[1].name + e->pc * 2 + -8) != 0) {
              rand = NuRand(0);
              e->rep_count[e->rep_ix] =
                   e->rep_count[e->rep_ix] +
                   (rand - (rand / (int)*(short *)(p[1].name + e->pc * 2 + -8)) *
                           (int)*(short *)(p[1].name + e->pc * 2 + -8));
            }
            iVar2 = e->rep_ix;
            n = e->pc + 3;
            e->pc = n;
            e->rep_start[iVar2] = n;
            e->rep_ix = iVar2 + 1;
          }
        }
        else {
          if (sVar1 == 0xe) goto LAB_800bb748;
          if (sVar1 < 0xe) {
            if (e->rep_ix == 0) {
              err = NuErrorProlog("C:/source/crashwoc/code/nu3dx/nutexanm.c",0x3b0);
              //(*error)("TexAnim Processor Alert: UNTILTEX without REPEAT at (%d)",e->pc);
            }
            n = EvalVars((int)p->code[e->pc + 1],e->tex_ix,
                         (int)*(short *)(p[1].name + e->pc * 2 + -8));
            if ((n == 0) && (n = e->rep_ix, e->rep_count[n + -1] != 0)) goto LAB_800bb730;
            e->pc = e->pc + 3;
            e->rep_ix = e->rep_ix + -1;
          }
          else if (sVar1 == 0xf) {
            NuTexAnimXCall((int)p->code[n + 1],e);
            n = e->pc + 2;
            goto LAB_800bb76c;
          }
        }
LAB_800bb770:
      } while (!check);
    }
    else {
      e->pause_cnt = e->pause_cnt + -1;
    }
  }
  return;
}

/*

//DECOMP 2


void NuTexAnimEnvProc(struct nutexanimenv_s* e) {
    s16 temp_r0;
    s16 temp_r8;
    s16 temp_r9_4;
    s16 temp_r9_5;
    s16 var_r0_2;
    s16 var_r9_4;
    s16* temp_r29;
    s32 temp_r10;
    s32 temp_r11;
    s32 temp_r11_2;
    s32 temp_r8_2;
    s32 temp_r8_3;
    s32 temp_r9_3;
    s32 temp_r9_6;
    s32 temp_r9_7;
    s32 temp_r9_8;
    s32 temp_r9_9;
    s32 var_r0;
    s32 var_r11_3;
    s32 var_r28;
    s32 var_r9;
    s32 var_r9_2;
    s32 var_r9_3;
    s32 var_r9_5;
    s32* temp_r25;
    s32* temp_r26;
    s32* temp_r30;
    s32* var_r11;
    s32* var_r11_2;
    struct nutexanimprog_s* temp_r6;
    u32 temp_r9;
    u32 temp_r9_2;

    var_r28 = 0;
    temp_r6 = e->prog;
    if (temp_r6 != NULL) {
        temp_r9 = temp_r6->off_mask;
        if (nta_sig_off & temp_r9) {
            var_r9 = 0;
            var_r11 = temp_r6->off_sig;
            if (nta_sig_off & 1 & temp_r9) {
                e->pause_cnt = 0;
                e->pc = temp_r6->off_sig[0];
                e->ra_ix = 0;
                e->rep_ix = 0;
            } else {
loop_4:
                var_r9 += 1;
                var_r11 += 4;
                if (var_r9 <= 0x1F) {
                    if (nta_sig_off & (1 << var_r9) & temp_r9) {
                        e->rep_ix = 0;
                        e->pc = *var_r11;
                        e->pause_cnt = 0;
                        e->ra_ix = 0;
                    } else {
                        goto loop_4;
                    }
                }
            }
        }
        temp_r9_2 = temp_r6->on_mask;
        if (nta_sig_on & temp_r9_2) {
            var_r9_2 = 0;
            var_r11_2 = temp_r6->on_sig;
            if (nta_sig_on & 1 & temp_r9_2) {
                var_r0 = temp_r6->on_sig[0];
                goto block_13;
            }
loop_10:
            var_r9_2 += 1;
            var_r11_2 += 4;
            if (var_r9_2 <= 0x1F) {
                if (nta_sig_on & (1 << var_r9_2) & temp_r9_2) {
                    var_r0 = *var_r11_2;
block_13:
                    e->rep_ix = 0;
                    e->pc = var_r0;
                    e->pause_cnt = 0;
                    e->ra_ix = 0;
                } else {
                    goto loop_10;
                }
            }
        }
        temp_r9_3 = e->pause_cnt;
        if (temp_r9_3 != 0) {
            e->pause_cnt = temp_r9_3 - 1;
            return;
        }
        temp_r29 = temp_r6->code;
        temp_r25 = e->ra;
        temp_r26 = e->rep_start;
        do {
            temp_r10 = e->pc;
            temp_r0 = temp_r29[temp_r10];
            switch (temp_r0) {                      /* irregular */
            case 0:
                temp_r9_4 = temp_r29[temp_r10 + 1];
                e->tex_ix = (s32) temp_r9_4;
                e->mtl->tid = (s32) e->tids[temp_r9_4];
                var_r9_3 = e->pc + 2;
block_43:
                e->pc = var_r9_3;
                e->pause_cnt = e->pause;
                if (e->pause_r != 0) {
                    e->pause_cnt += NuRand(NULL) % e->pause_r;
                }
            case 14:
                var_r28 = 1;
                break;
            case 1:
                temp_r8 = temp_r29[temp_r10 + 2];
                temp_r9_5 = temp_r29[temp_r10 + 3];
                var_r0_2 = e->tex_ix + temp_r29[temp_r10 + 1];
                if (var_r0_2 < temp_r8) {
                    var_r0_2 = temp_r8;
                }
                if (var_r0_2 > temp_r9_5) {
                    var_r0_2 = temp_r9_5;
                }
                e->tex_ix = (s32) var_r0_2;
                e->mtl->tid = (s32) e->tids[var_r0_2];
                var_r9_3 = e->pc + 4;
                goto block_43;
            case 2:
                e->pause_cnt = (s32) temp_r29[temp_r10 + 1];
                if ((s16) temp_r29[temp_r10 + 2] != 0) {
                    e->pause_cnt += NuRand(NULL) % temp_r29[e->pc + 2];
                }
                var_r28 = 1;
                var_r9_4 = e->pc + 3;
block_77:
                e->pc = (s32) var_r9_4;
                break;
            case 5:
                e->pause = (s32) temp_r29[temp_r10 + 1];
                e->pc = temp_r10 + 3;
                e->pause_r = (s32) temp_r29[temp_r10 + 2];
                break;
            case 7:
                var_r9_4 = temp_r29[temp_r10 + 1];
                goto block_77;
            case 9:
                if (EvalVars((s32) temp_r29[temp_r10 + 1], e->tex_ix, (s32) temp_r29[temp_r10 + 2]) != 0) {
                    var_r9_5 = e->pc + 3;
block_56:
                    e->pc = (s32) temp_r29[var_r9_5];
                } else {
                    var_r9_4 = e->pc + 4;
                    goto block_77;
                }
                break;
            case 8:
                if ((s32) e->ra_ix > 0xF) {
                    //NuErrorProlog(0x8011E754, 0x388)(&lbl_8011E8AC, e->pc);
                }
                temp_r11 = e->ra_ix;
                temp_r25[temp_r11] = e->pc + 2;
                e->ra_ix = temp_r11 + 1;
                var_r9_5 = e->pc + 1;
                goto block_56;
            case 10:
                if ((s32) e->ra_ix == 0) {
                    //NuErrorProlog(0x8011E754, 0x390)(&lbl_8011E8E4, e->pc);
                }
                temp_r9_6 = e->ra_ix - 1;
                e->ra_ix = temp_r9_6;
                e->pc = temp_r25[temp_r9_6];
                break;
            case 11:
                if ((s32) e->rep_ix > 0xF) {
                    //NuErrorProlog(0x8011E754, 0x398)(&lbl_8011E91C, e->pc);
                }
                temp_r30 = e->rep_count;
                temp_r30[e->rep_ix] = (s32) temp_r29[e->pc + 1];
                if ((s16) temp_r29[e->pc + 2] != 0) {
                    temp_r8_2 = e->rep_ix;
                    temp_r30[temp_r8_2] += NuRand(NULL) % temp_r29[e->pc + 2];
                }
                temp_r11_2 = e->rep_ix;
                temp_r9_7 = e->pc + 3;
                e->pc = temp_r9_7;
                temp_r26[temp_r11_2] = temp_r9_7;
                e->rep_ix = temp_r11_2 + 1;
                break;
            case 12:
                if ((s32) e->rep_ix == 0) {
                    //NuErrorProlog(0x8011E754, 0x3A2)(&lbl_8011E95C, e->pc);
                }
                temp_r8_3 = e->rep_ix - 1;
                var_r11_3 = temp_r8_3 * 4;
                if ((s32) e->rep_count[temp_r8_3] == 0) {
                    e->rep_ix = temp_r8_3;
                    var_r9_4 = e->pc + 1;
                    goto block_77;
                }
block_74:
                e->pc = *(temp_r26 + var_r11_3);
                *(e->rep_count + var_r11_3) = *(e->rep_count + var_r11_3) - 1;
                break;
            case 13:
                if ((s32) e->rep_ix == 0) {
                    //NuErrorProlog(0x8011E754, 0x3B0)(&lbl_8011E994, e->pc);
                }
                temp_r9_8 = e->pc;
                if ((EvalVars((s32) temp_r29[temp_r9_8 + 1], e->tex_ix, (s32) temp_r29[temp_r9_8 + 2]) != 0) || (temp_r9_9 = e->rep_ix - 1, var_r11_3 = temp_r9_9 * 4, (((s32) e->rep_count[temp_r9_9] == 0) != 0))) {
                    e->pc += 3;
                    e->rep_ix -= 1;
                } else {
                    goto block_74;
                }
                break;
            case 15:
                NuTexAnimXCall((s32) temp_r29[temp_r10 + 1], e);
                var_r9_4 = e->pc + 2;
                goto block_77;
            }
        } while (var_r28 == 0);
    }
}





*/


void NuTexAnimSetSignals(u32 sig)
{
  nta_sig_old = sig;
  nta_sig_off = (sig ^ nta_sig_old) & ~sig;
  nta_sig_on = (sig ^ nta_sig_old) & sig;
  return;
}


void NuTexAnimProcessList(struct nutexanim_s *nta)

{
  if (nta != NULL) {
    do {
      if (nta->env != NULL) {
        NuTexAnimEnvProc(nta->env);
      }
      nta = nta->succ;
    } while (nta != NULL);
  }
  return;
}


void NuTexAnimAddList(struct nutexanim_s *nta)

{
  struct nutexanimlist_s *lst;
  struct nutexanimlist_s **prev;
  
  lst = ntal_free;
  if (ntal_free == NULL) {
    return;
  }
  prev = &ntal_free->prev;
  ntal_free = ntal_free->succ;
  *prev = NULL;
  lst->nta = nta;
  lst->succ = ntal_first;
  if (ntal_first != NULL) {
    ntal_first->prev = lst;
  }
  ntal_first = lst;
  return;
}


void NuTexAnimRemoveList(struct nutexanim_s *nta)

{
  struct nutexanimlist_s *rv;
  struct nutexanimlist_s **succ;
  
  rv = ntal_first;
  if (ntal_first == NULL) {
    return;
  }
  do {
    if (rv->nta == nta) {
      if (rv->succ != NULL) {
        rv->succ->prev = rv->prev;
      }
      if (rv->prev == NULL) {
        ntal_first = rv->succ;
      }
      else {
        rv->prev->succ = rv->succ;
      }
      rv->succ = ntal_free;
      ntal_free = rv;
      return;
    }
    succ = &rv->succ;
    rv = *succ;
  } while (*succ != NULL);
  return;
}



void NuTexAnimProcess(void)

{
  struct nutexanimlist_s *rv;
  
  rv = ntal_first;
  if (ntal_first != NULL) {
    do {
      NuTexAnimProcessList(rv->nta);
      rv = rv->succ;
    } while (rv != NULL);
  }
  return;
}


int EvalVars(int cc,int v0,int v1)

{
  
  if (cc == 2) {
    if (v0 <= v1) {
      return 0;
    }
    return 1;
  }
  if (cc < 3) {
    if (cc == 0) {
      if (v0 != v1) {
        return 0;
      }
      return 1;
    }
    if (cc == 1) {
      if (v1 <= v0) {
        return 0;
      }
      return 1;
    }
  }
  else {
    if (cc == 4) {
      if (v0 < v1) {
        return 0;
      }
      return 1;
    }
    if (cc < 4) {
      if (v1 < v0) {
        return 0;
      }
      return 1;
    }
    if (cc == 5) {
      if (v0 == v1) {
        return 0;
      }
      return 1;
    }
  }
  NuErrorProlog("C:/source/crashwoc/code/nu3dx/nutexanm.c",0x17e)("unknown condition code %d",cc);
  return 0;
}


s32 ParGetCC(struct nufpar_s *pf)
{
  s32 cc;
  char Wbuf;
  
  NuFParGetWord(pf);
  Wbuf = pf->wordBuffer[1];
  if (Wbuf == '<') {
    if (pf->wordBuffer[2] == '=') {
      return 3;
    }
    if (pf->wordBuffer[2] != '>') {
      return 1;
    }
LAB_800ba56c:
    cc = 5;
  }
  else {
    if (Wbuf < '=') {
      if (Wbuf == '!') goto LAB_800ba56c;
    }
    else {
      if (Wbuf == '=') {
        return 0;
      }
      if (Wbuf == '>') {
        if (pf->wordBuffer[2] != '=') {
          return 2;
        }
        return 4;
      }
    }
    NuErrorProlog("C:/source/crashwoc/code/nu3dx/nutexanm.c",0x1ac)("unknown condition \'%s\' at line %d",pf->wordBuffer + 1,pf->line_num);
  }
  return cc;
}

//PS2
s32 LabTabFind(char* buf) \\static inline on ps2
{
    s32 i;

    if (strlen(buf) > 0x14) {
        buf[0x14] = '\0';
      }
    for(i = 0; i < labtabcnt; i++) {
      if (strcasecmp(&labtab[i],buf) == 0) {
          return i;
      }
    }
    if (0x3f < labtabcnt) {
        NuErrorProlog("..\\nu2.ps2\\nu3d\\nutexanm.c",0x206)("Tex Anim Assembler Fatal Error: too many labels");
      }
      strcpy(&labtab[labtabcnt++],buf);
    return labtabcnt - 1;
}


s32 XDefLabTabFind(char* buf)
{
   s32 i;
    if (strlen(buf) > 0x14) {
        buf[0x14] = '\0';
      }
    for(i = 0; i < xdeflabtabcnt; i++) {
      if (strcasecmp(&xdeflabtab[i],buf) == 0) {
          return i;
      }
    }
  if (0xff < xdeflabtabcnt) {
    NuErrorProlog("C:/source/crashwoc/code/nu3dx/nutexanm.c",0x1de)("Tex Anim Assembler Fatal Error: too many global labels");
  }
  strcpy(&xdeflabtab[xdeflabtabcnt++],buf);
  return xdeflabtabcnt + -1;
}



void pftaTex(nufpar_s *fp)

{
  int tid;
  short eop;
  nutexanimprog_s *prog;
  
  tid = NuFParGetInt(fp);
  prog = parprog;
  eop = parprog->eop;
  parprog->eop = eop + 1;
  prog->code[eop] = 0;
  eop = prog->eop;
  prog->eop = eop + 1;
  prog->code[eop] = (short)tid;
  return;
}

void pftaTexAdj(nufpar_s *fpar)

{
  int tid;
  int mi;
  int ma;
  short eop;
  nutexanimprog_s *prog;
  
  tid = NuFParGetInt(fpar);
  mi = NuFParGetInt(fpar);
  ma = NuFParGetInt(fpar);
  prog = parprog;
  eop = parprog->eop;
  parprog->eop = eop + 1;
  prog->code[eop] = 1;
  eop = prog->eop;
  prog->eop = eop + 1;
  prog->code[eop] = (short)tid;
  eop = prog->eop;
  prog->eop = eop + 1;
  prog->code[eop] = (short)mi;
  eop = prog->eop;
  prog->eop = eop + 1;
  prog->code[eop] = (short)ma;
  return;
}

void pftaWait(nufpar_s *fpar)

{
  int fcnt;
  int rcnt;
  short eop;
  nutexanimprog_s *prog;
  
  fcnt = NuFParGetInt(fpar);
  rcnt = NuFParGetInt(fpar);
  prog = parprog;
  eop = parprog->eop;
  parprog->eop = eop + 1;
  prog->code[eop] = 2;
  eop = prog->eop;
  prog->eop = eop + 1;
  prog->code[eop] = (short)fcnt;
  eop = prog->eop;
  prog->eop = eop + 1;
  prog->code[eop] = (short)rcnt;
  return;
}

void pftaRate(nufpar_s *fpar)

{
  int fcnt;
  int rcnt;
  short eop;
  nutexanimprog_s *prog;
  
  fcnt = NuFParGetInt(fpar);
  rcnt = NuFParGetInt(fpar);
  prog = parprog;
  eop = parprog->eop;
  parprog->eop = eop + 1;
  prog->code[eop] = 5;
  eop = prog->eop;
  prog->eop = eop + 1;
  prog->code[eop] = (short)fcnt;
  eop = prog->eop;
  prog->eop = eop + 1;
  prog->code[eop] = (short)rcnt;
  return;
}

static void pftaOn(struct nufpar_s *fp)
{
  s32 sig;
  
  sig = NuFParGetInt(fp);
  parprog->on_sig[sig] = (int)parprog->eop;
  parprog->on_mask = parprog->on_mask | 1 << sig;
  return;
}

static void pftaOff(struct nufpar_s *fp)
{
  s32 sig;
  
  sig = NuFParGetInt(fp);
  parprog->off_sig[sig] = (int)parprog->eop;
  parprog->off_mask = parprog->off_mask | 1 << sig;
  return;
}

static void pftaLabel(struct nufpar_s *fp) {
    NuFParGetWord(fp);
    nta_labels[LabTabFind(fp->wbuff)] = parprog->eop;
}

static void pftaXDef(struct nufpar_s *fpar)
{
  s32 lab;
  
  NuFParGetWord(fpar);
  lab = XDefLabTabFind(fpar->wordBuffer + 1);
  parprog->xdef_ids[parprog->xdef_cnt] = (short)lab;
  parprog->xdef_addrs[prog->xdef_cnt] = parprog->eop;
  parprog->xdef_cnt = parprog->xdef_cnt + 1;
  return;
}

static void pftaGoto(struct nufpar_s *fpar)
{
  s32 lab;
  
  NuFParGetWord(fpar);
  lab = LabTabFind(fpar->wordBuffer + 1);
  parprog->eop = parprog->eop; + 1;
  parprog->code[parprog->eop] = 7;
  parprog->eop = parprog->eop + 1;
  parprog->code[parprog->eop] = (short)lab;
  return;
}


void pftaXRef(nufpar_s *fpar)

{
  int lab;
  short eop;
  nutexanimprog_s *prog;
  
  NuFParGetWord(fpar);
  lab = XDefLabTabFind(fpar->wordBuffer + 1);
  prog = parprog;
  eop = parprog->eop;
  parprog->eop = eop + 1;
  prog->code[eop] = 0xf;
  eop = prog->eop;
  prog->eop = eop + 1;
  prog->code[eop] = (short)lab;
  return;
}

void pftaBtex(nufpar_s *fpar)

{
  int cc;
  int tid;
  int lab;
  short eop;
  nutexanimprog_s *prog;
  
  cc = ParGetCC(fpar);
  tid = NuFParGetInt(fpar);
  NuFParGetWord(fpar);
  lab = LabTabFind(fpar->wordBuffer + 1);
  prog = parprog;
  eop = parprog->eop;
  parprog->eop = eop + 1;
  prog->code[eop] = 9;
  eop = prog->eop;
  prog->eop = eop + 1;
  prog->code[eop] = (short)cc;
  eop = prog->eop;
  prog->eop = eop + 1;
  prog->code[eop] = (short)tid;
  eop = prog->eop;
  prog->eop = eop + 1;
  prog->code[eop] = (short)lab;
  return;
}

void pftaGosub(nufpar_s *fpar)

{
  int lab;
  short eop;
  nutexanimprog_s *prog;
  
  NuFParGetWord(fpar);
  lab = LabTabFind(fpar->wordBuffer + 1);
  prog = parprog;
  eop = parprog->eop;
  parprog->eop = eop + 1;
  prog->code[eop] = 8;
  eop = prog->eop;
  prog->eop = eop + 1;
  prog->code[eop] = (short)lab;
  return;
}

void pftaRet(nufpar_s *fp)

{
  short eop;
  nutexanimprog_s *prog;
  
  prog = parprog;
  eop = parprog->eop;
  parprog->eop = eop + 1;
  prog->code[eop] = 10;
  return;
}


void pftaRepeat(nufpar_s *fpar)

{
  int randval;
  int cnt;
  short eop;
  nutexanimprog_s *prog;
  
  randval = NuFParGetInt(fpar);
  if (randval == 0) {
    randval = 0xffff; //=-1
  }
  cnt = NuFParGetInt(fpar);
  prog = parprog;
  eop = parprog->eop;
  parprog->eop = eop + 1;
  prog->code[eop] = 0xb;
  eop = prog->eop;
  prog->eop = eop + 1;
  prog->code[eop] = (short)randval;
  eop = prog->eop;
  prog->eop = eop + 1;
  prog->code[eop] = (short)cnt;
  return;
}

void pftaRepend(nufpar_s *fp)

{
  short eop;
  nutexanimprog_s *prog;
  
  prog = parprog;
  eop = parprog->eop;
  parprog->eop = eop + 1;
  prog->code[eop] = 0xc;
  return;
}

void pftaUntiltex(nufpar_s *fpar)

{
  int cc;
  int tid;
  short eop;
  nutexanimprog_s *prog;
  
  cc = ParGetCC(fpar);
  tid = NuFParGetInt(fpar);
  prog = parprog;
  eop = parprog->eop;
  parprog->eop = eop + 1;
  prog->code[eop] = 0xd;
  eop = prog->eop;
  prog->eop = eop + 1;
  prog->code[eop] = (short)cc;
  eop = prog->eop;
  prog->eop = eop + 1;
  prog->code[eop] = (short)tid;
  return;
}

static void pftaEnd(struct nufpar_s *fp)
{
  parprog->eop = parprog->eop + 1;
  parprog->code[parprog->eop] = 0xe;
  return;
}

static void pftaScriptname(struct nufpar_s *fpar)
{
  NuFParGetWord(fpar);
  fpar->wordBuffer[0x21] = '\0';
  strcpy(parprog->name,fpar->wordBuffer + 1);
  return;
}