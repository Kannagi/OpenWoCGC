nuscene_s * InstSceneLoad(char *name)

{
  int cmp;
  sceneinst_s *sc;
  nuscene_s *nus;
  nulnkhdr_s *lnk;
  
  lnk = (nulnkhdr_s *)0x0;
  do {
    lnk = NuLstGetNext(sceneinst_pool,lnk);
    if (lnk == (nulnkhdr_s *)0x0) {
      sc = (sceneinst_s *)NuLstAlloc(sceneinst_pool);
      if (sc != (sceneinst_s *)0x0) {
        nus = NuSceneLoad(name);
        sc->nus = nus;
        if (nus != (nuscene_s *)0x0) {
          strcpy(sc->name,name);
          sc->inst_cnt = 1;
          return sc->nus;
        }
        NuLstFree((nulnkhdr_s *)sc);
      }
      return (nuscene_s *)0x0;
    }
    cmp = strcmp(name,(char *)&lnk->succ);
  } while (cmp != 0);
  nus = (nuscene_s *)lnk->owner;
  lnk[0x10].succ = (nulnkhdr_s *)((int)&(lnk[0x10].succ)->owner + 1);
  return nus;
}


nuAnimData_s * InstAnimDataLoad(char *name)

{
  int cmp;
  nulsthdr_s *pnVar1;
  nulnkhdr_s *lstnext;
  
  lstnext = (nulnkhdr_s *)0x0;
  do {
    lstnext = NuLstGetNext(animdatainst_pool,lstnext);
    if (lstnext == (nulnkhdr_s *)0x0) {
      lstnext = NuLstAlloc(animdatainst_pool);
      if (lstnext != (nulnkhdr_s *)0x0) {
        pnVar1 = (nulsthdr_s *)NuAnimDataLoadBuff(name,&superbuffer_ptr,&superbuffer_end);
        lstnext->owner = pnVar1;
        if (pnVar1 != (nulsthdr_s *)0x0) {
          strcpy((char *)&lstnext->succ,name);
          pnVar1 = lstnext->owner;
          lstnext[0x10].succ = (nulnkhdr_s *)0x1;
          return (nuAnimData_s *)pnVar1;
        }
        NuLstFree(lstnext);
      }
      return (nuAnimData_s *)0x0;
    }
    cmp = strcasecmp(name,(char *)&lstnext->succ);
  } while (cmp != 0);
  pnVar1 = lstnext->owner;
  lstnext[0x10].succ = (nulnkhdr_s *)((int)&(lstnext[0x10].succ)->owner + 1);
  return (nuAnimData_s *)pnVar1;
}


void InstInit(void)

{
  sceneinst_pool = NuLstCreate(0x10,0x108);
  animdatainst_pool = NuLstCreate(0xc0,0x108);
  return;
}

void InstClose(void)

{
  animdatainst_s *adi;
  sceneinst_s *sc;
  
  if (animdatainst_pool != (nulsthdr_s *)0x0) {
    adi = (animdatainst_s *)0x0;
    do {
      adi = (animdatainst_s *)NuLstGetNext(animdatainst_pool,(nulnkhdr_s *)adi);
    } while (adi != (animdatainst_s *)0x0);
    NuLstDestroy(animdatainst_pool);
    animdatainst_pool = (nulsthdr_s *)0x0;
  }
  if (sceneinst_pool != (nulsthdr_s *)0x0) {
    sc = (sceneinst_s *)0x0;
    while (sc = (sceneinst_s *)NuLstGetNext(sceneinst_pool,(nulnkhdr_s *)sc),
          sc != (sceneinst_s *)0x0) {
      NuSceneDestroy(sc->nus);
    }
    NuLstDestroy(sceneinst_pool);
    sceneinst_pool = (nulsthdr_s *)0x0;
  }
  return;
}