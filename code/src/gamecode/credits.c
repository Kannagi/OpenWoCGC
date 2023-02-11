void InitCredits(void)

{
  float fVar1;
  uint uVar2;
  Credit_s *Cr;
  short size;
  float tot;
  
  fVar1 = 0.0;
  CREDITCOUNT = 0;
  if (0 < Credit[0].size) {
    CREDITCOUNT = 0;
    Cr = Credit;
    size = Credit[0].size;
    do {
      uVar2 = (uint)size;
      CREDITCOUNT = CREDITCOUNT + 1;
      size = Cr[1].size;
      tot = (float)((double)CONCAT44(0x43300000,uVar2 ^ 0x80000000) - 4503601774854144.0) / 1000.0 ;
      fVar1 = fVar1 + tot + tot;
      Cr = Cr + 1;
    } while (0 < size);
  }
  credit_time = (fVar1 + 2.0) / credit_speed;
  return;
}


void DrawCredits(void)

{
  int cmp;
  int i;
  Credit_s *Cptr;
  double dVar1;
  double dVar2;
  double dVar3;
  double dVar4;
  double dVar5;
  double dVar6;
  double dVar7;
  double dVar8;
  float time;
  
  i = 0;
  Cptr = Credit;
  time = GameTimer.ftime * credit_speed + -1.0;
  if (0 < CREDITCOUNT) {
    dVar1 = 100.0;
    dVar5 = 4503601774854144.0;
    dVar2 = -1.5;
    dVar3 = 1.700000047683716;
    dVar4 = 0.1000000014901161;
    do {
      dVar7 = (double)time;
      dVar8 = (double)(float)((double)(float)((double)CONCAT44(0x43300000,
                                                               (int)Cptr->size ^ 0x80000000) - dVar 5
                                             ) / dVar1);
      if (((Cptr->txt != (char *)0x0) && (dVar2 < dVar7)) && (dVar7 < dVar3)) {
        strcpy(tbuf,Cptr->txt);
        cmp = strcmp(tbuf,"SR. MANAGER OF PUBLIC RELATIONS");
        time = 0.85;
        if (cmp != 0) {
          time = 0.9;
        }
        dVar6 = (double)time;
        if (Game.language == 'c') {
          AddSpacesIntoText(tbuf,1);
        }
        Text3D(tbuf,0.0,(float)dVar7,1.0,(float)(dVar8 * dVar6),(float)dVar8,(float)(dVar8 + dVar8 ),
               2,(int)Cptr->colour);
      }
      i = i + 1;
      time = (float)(dVar7 - (double)((float)(dVar8 * dVar4) + (float)(dVar8 * dVar4)));
      Cptr = Cptr + 1;
    } while (i < CREDITCOUNT);
  }
  return;
}