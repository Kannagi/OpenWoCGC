int PtInsideSpline(nuvec_s *wpos,nugspline_s *sp)

{
  int iVar1;
  nuvec_s *s;
  uint n;
  int icnt;
  nuvec_s *e;
  nuvec_s ipnt;
  nuplane_s p;
  
  n = 0;
  p.d = -(wpos->x * 0.0 + wpos->y * 0.0 + wpos->z);
  p.c = 1.0;
  icnt = 0;
  p.a = 0.0;
  p.b = 0.0;
  s = (nuvec_s *)sp->pts + sp->len + -1;
  e = (nuvec_s *)sp->pts;
  if (0 < sp->len) {
    do {
      iVar1 = NuPlnLine(&p,s,e,&ipnt);
      if ((iVar1 != 0) && (wpos->x <= ipnt.x)) {
        n = n + 1;
      }
      icnt = icnt + 1;
      s = e;
      e = e + 1;
    } while (icnt < sp->len);
  }
  return n & 1;
}

//NGC MATCH
void ApplyVisiTable(struct nugscn_s *sc,struct nuvec_s *pos) {
  s32 n;
  s32 m;
  struct visidat_s *vd;
  
  if (vscnt != 0) {
    if (pos == NULL) {
        for (m = 0, n = 0; m < sc->numinstance; m++, n++) {
          sc->instances[n].flags.visitest = 0x20000000 | 1;
        }
    }
    else {
        for (m = 0, n = 0; m < sc->numinstance; m++, n++) {
              sc->instances[n].flags.visitest = 0xdfffffff * 2;
        }
        for (m = 0; m < vscnt; m++) {
          vd = visidat[m];
          if ((PtInsideSpline(pos,vd->sp) != 0)) {
            for (n = 0; n < vd->numinstances; n++) {
              vd->i[n]->flags.visitest = 0x20000000 | 1;
            }
          }
        }
    }
  }
  return;
}


int VSAddObjs(nuinstance_s **op,nugscn_s *gsc,nugspline_s *sp)

{

}


void BuildVisiTable(nugscn_s *gsc)

{

}


void ReleaseVisiTable(void)

{
  if (vsdata != NULL) {
    NuMemFree(vsdata);
  }
  vsdata = NULL;
  vscnt = 0;
  return;
}