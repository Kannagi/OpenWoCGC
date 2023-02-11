nulsthdr_s * NuLstCreate(int elcnt,int elsize)

{
  nulsthdr_s *list;
  int n;
  prevLst free;
  nulnkhdr_s *start;
  prevLst curr;
  short i;
  
  list = (nulsthdr_s *)NuMemAlloc(elcnt * (elsize + 0x10) + 0x10);
  if (list != (nulsthdr_s *)0x0) {
    start = (nulnkhdr_s *)(list + 1);
    n = 1;
    list->head = (nulnkhdr_s *)0x0;
    list->free = start;
    list->elcnt = (short)elcnt;
    list->elsize = (short)elsize;
    curr.s8 = (char *)((int)start + elsize + 0x10);
    free.lhdr = start;
    if (1 < elcnt) {
      do {
        start = curr.lhdr;
        i = (short)n;
        (free.lhdr)->succ = start;
        (free.lhdr)->owner = list;
        n = n + 1;
        (free.lhdr)->id = i - 1;
        curr.s8 = (start + elsize + 0x10).s8;
        free.lhdr = start;
      } while (n < elcnt);
    }
    start->succ = (nulnkhdr_s *)0x0;
    start->id = (short)n - 1;
    start->owner = list;
  }
  return list;
}



void NuLstDestroy(nulsthdr_s *hdr)

{
  NuMemFree(hdr);
  return;
}


nulnkhdr_s * NuLstAlloc(nulsthdr_s *hdr)

{
  nulnkhdr_s *lnk;
  
  lnk = hdr->free;
  if (lnk != (nulnkhdr_s *)0x0) {
    hdr->free = lnk->succ;
    lnk->succ = hdr->head;
    if (hdr->head == (nulnkhdr_s *)0x0) {
      hdr->tail = lnk;
    }
    else {
      hdr->head->prev = lnk;
    }
    lnk->prev = (nulnkhdr_s *)0x0;
    hdr->head = lnk;
    *(uint *)&lnk->id = *(uint *)&lnk->id | 0x8000;
    return lnk + 1;
  }
  return (nulnkhdr_s *)0x0;
}


void NuLstFree(nulnkhdr_s *lnk)

{
  nulsthdr_s *owner;
  
  owner = lnk[-1].owner;
  if (lnk[-1].succ == (nulnkhdr_s *)0x0) {
    owner->tail = lnk[-1].prev;
  }
  else {
    (lnk[-1].succ)->prev = lnk[-1].prev;
  }
  if (lnk[-1].prev == (nulnkhdr_s *)0x0) {
    owner->head = lnk[-1].succ;
  }
  else {
    (lnk[-1].prev)->succ = lnk[-1].succ;
  }
  lnk[-1].succ = owner->free;
  owner->free = lnk + -1;
  *(uint *)&lnk[-1].id = *(uint *)&lnk[-1].id & 0xffff7fff;
  return;
}


nulnkhdr_s * NuLstGetNext(nulsthdr_s *hdr,nulnkhdr_s *lnk)

{
  if (lnk == (nulnkhdr_s *)0x0) {
    if (hdr->head != (nulnkhdr_s *)0x0) {
      return hdr->head + 1;
    }
  }
  else if (lnk[-1].succ != (nulnkhdr_s *)0x0) {
    return lnk[-1].succ + 1;
  }
  return (nulnkhdr_s *)0x0;
}


