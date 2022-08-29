// c:/bsd/rigel/sort/C2LRB.c
// Date: Thu Jul 09 08:11:40 2020
// (C) OntoOO/ Dennis de Champeaux

#define iswap(p, q, A) { void *t3t = A[p]; A[p] = A[q]; A[q] = t3t; }

static const int bufSizeB = 200;

static void cut2lrbc();
// cut2left is a support function to call up the workhorse cut2leftc
void cut2lrb(void **A, int N, int M, int (*compare)()) { 
  // printf("cut2lrb %d %d %d\n", N, M, M-N);
  int L = M - N;
  if ( L < 10 * 1024 ) {
    cut2lr(A, N, M, compare);
    return;
  }
  int depthLimit = 1 + 2.9 * floor(log(L));
  cut2lrbc(A, N, M, depthLimit, compare);
} // end cut2


// cut2lrbc does 2-partitioning with one pivot.
// cut2lrbc invokes dflgm when trouble is encountered.
void cut2lrbc(void **A, int N, int M, 
	       int depthLimit, int (*compareXY)()) {  
  int bufl[bufSizeB];
  int bufr[bufSizeB];
  int L;
  // printf("\n");

 Start:
  // printf("cut2lrbc N %d M %d %d\n", N, M, M-N);

  L = M - N + 1;

  if ( L <= 4 * 1024 ) {
    cut2lrc(A, N, M, depthLimit, compareXY);
    // quicksort0c(A, N, M, depthLimit, compareXY);
    return;
  }

  if ( depthLimit <= 0 ) {
    heapc(A, N, M, compareXY);
    return;
  }
  depthLimit--;

  register void *T; // pivot
  register int I = N, J = M; // indices
  int middlex = N + (L>>1); // N + L/2
  void *middle;
  // const int small = 3200;

  { // get pivot

    int k, N1, M1; // for sampling
    // int middlex = N + (L>>1); // N + L/2

    int probeLng = sqrt(L/9);
    int halfSegmentLng = probeLng >> 1; // probeLng/2;
    //    int quartSegmentLng = probeLng >> 2; // probeLng/4;
    N1 = middlex - halfSegmentLng; //  N + (L>>1) - halfSegmentLng;
    M1 = N1 + probeLng - 1;
    int offset = L/probeLng;  

    // assemble the mini array [N1, M1]
    for (k = 0; k < probeLng; k++) // iswap(N1 + k, N + k * offset, A);
      { int xx = N1 + k, yy = N + k * offset; iswap(xx, yy, A); }
    // sort this mini array to obtain good pivots
    if ( probeLng < 120 ) quicksort0c(A, N1, M1, depthLimit, compareXY); else {
      // protect against constant arrays
      int p0 = N1 + (probeLng>>1);
      int pn = N1, pm = M1, d = (probeLng-3)>>3;
      pn = med(A, pn, pn + d, pn + 2 * d, compareXY);
      p0 = med(A, p0 - d, p0, p0 + d, compareXY);
      pm = med(A, pm - 2 * d, pm - d, pm, compareXY);
      p0 = med(A, pn, p0, pm, compareXY);
      if ( p0 != middlex ) iswap(p0, middlex, A); 
      dflgm(A, N1, M1, middlex, quicksort0c, depthLimit, compareXY);
    }
    T = middle = A[middlex];
    if ( compareXY(A[M1], middle) <= 0 ) {
      // give up because cannot find a good pivot
      // dflgm is a dutch flag type of algorithm
      void cut2c();
      dflgm(A, N, M, middlex, cut2lrbc, depthLimit, compareXY);
      return;
    }
    for ( k = N1; k <= middlex; k++ ) {
    iswap(k, I, A); I++;
    }
    I--;
    for ( k = M1; middlex < k; k--) {
      iswap(k, J, A); J--;
    }
    J++;
  }
  
  //  register void *AI,*AJ; // array values
	// The left segment has elements <= T
	// The right segment has elements >= T 
        //    and at least one element > T
  int kl, kr, idxl, idxr; 
  int bufx = bufSizeB-1;

  int chunk = 3*1024, range = (J-I)>>1;
  chunk = ( chunk < range ? chunk : range );
  int M2 = I + chunk;
  /*
       |------]--------]------------[------|
       N      I        M2           J      M
   */
  int J1 = M2;
  // J <= M
  if ( compareXY(A[M2], T) <= 0 ) {
    iswap(M2, J, A); J++;
  }
  // J could be M+1

 Right:
  kl = kr = -1;
  while ( kr < bufx ) {
    while ( compareXY(T, A[--J1]) < 0 ); 
    if ( J1 <= I ) { J1 = I+1; goto MopUpR; }
    bufr[++kr] = J1;
  }

  /*
       |------]-----[-*-*-]------------[------|
       N      I     J1    M2           J      M
   */
    while ( kl < bufx ) {
      while ( compareXY(A[++I], T) <= 0 );
      if ( J1 <= I ) { I = J1-1; goto MopUpL; }
      bufl[++kl] = I;
    }
    /*
      |------*-*]---[-*-*-]------------[------|
      N         I   J1    M2           J      M
    */
    // swap them
    // printf("DD kl %i kr %i \n", kl, kr);
    while ( 0 <= kl ) {
      idxl = bufl[kl--];
      idxr = bufr[kr--];
      iswap(idxl, idxr, A); 
    }
    goto Right;

 MopUpL:

    /*
      |------*-][-*-*-]------------[------|
      N        IJ1    M2           J      M
    */
    while ( 0 <= kl ) {
      idxl = bufl[kl--];
      idxr = bufr[kr--];
      iswap(idxl, idxr, A); 
    }

 MopUpR:
  /*
       |------------[-*-*---]------------[------|
       N           IJ1      M2           J      M
   */

    while ( 0 <= kr ) {
      idxr = bufr[kr--];
      if ( idxr <= I ) continue;
      if ( idxr - 1 == I ) { I++;  continue; }
      { int z = I+1; iswap(z, idxr, A); I = z;  }
    }
    int Z = M2;

 Repeat:
    /*     l       r                    r
       |------][--------]------------[------|
       N      I         Z            J      M
    */

    /* 
       printf("Repeat N %i I %i Z %i J %i \n", N,I,Z,J);
    { int k;
      for ( k = N; k <= I; k++ ) 
	if ( compareXY(T, A[k]) < 0 ) {
	  printf("Err L1 k %i\n", k);
	  exit(0);
	}
      for ( k = I+1; k <= Z; k++ ) 
	if ( compareXY(A[k], T) <= 0 ) {
	  printf("Err R1 k %i\n", k);
	  exit(0);
	}
    }
    */
    M2 = Z + chunk;
    if ( J <= M2 + chunk ) M2 = J-1;
    // printf("Repeat2 N %i I %i Z %i M2 %i J %i \n", N,I,Z,M2,J);
    /*    l       r                         r
       |------][----]-------------]------[------|
       N      I     Z             M2     J      M
    */
    if ( compareXY(A[M2], T) <= 0 ) {
      int z = Z-1;
      iswap(Z, M2, A); 
      Z = z; // fixed A[M2] & A[Z+1]
    }
    else { int z = Z+1;
      if ( compareXY(T, A[z]) < 0 ) { // fix A[z]
	iswap(z, I, A); I--;
      } 
    }
    // printf("Repeat3 N %i I %i Z %i M2 %i J %i \n", N,I,Z,M2,J);
    int I1 = Z, J2 = M2;
 Left:
    // printf("Left N %i I %i Z %i M2 %i J %i M %i\n", N,I,Z,M2,J,M);
    // printf("I1 %i J2 %i\n", I1, J2);
    kl = kr = -1;
    while ( kl < bufx ) {
      while ( compareXY(A[++I1], T) <= 0 );
      if ( J2 <= I1 ) { I1 = J2-1; goto MopUpL2; }
      bufl[++kl] = I1;
    }

  /*       l       r                       r
       |------][-----]-*-*-]-----]-----[------|
       N      I      Z     I1    M2    J      M
   */
    while ( kr < bufx ) {
      while ( compareXY(T, A[--J2]) < 0 ); 
      if ( J2 <= I1 ) { J2 = I1+1; goto MopUpR2; }
      bufr[++kr] = J2;
    }

    // swap them
    while ( 0 <= kl ) {
      idxl = bufl[kl--];
      idxr = bufr[kr--];
      iswap(idxl, idxr, A); 
    }
    goto Left;

 MopUpR2:
  /*       l       r        l                  r
       |------][--------]-*-*-][-*---]----[--------|
       N      I         Z    I1J2    M2   J        M
   */
    // swap the r-elements
    while ( 0 <= kr ) {
      idxl = bufl[kl--];
      idxr = bufr[kr--];
      iswap(idxl, idxr, A); 
    }

 MopUpL2:
  /*       l       r        l     r           r
       |------][--------]-*-*-][------]----[------|
       N      I         Z    I1J2     M2   J      M
   */
    // swap the l-elements
    while ( 0 <= kl ) {
      idxl = bufl[kl--];
      if ( J2 <= idxl ) continue;
      if ( idxl + 1 == J2 ) { J2--; continue; }
      { int z = J2-1; iswap(z, idxl, A); J2 = z; }
    }

    /*      l       r      l     r           r
       |------][--------]----][-----]-----[------|
       N      I         Z     J2    M2    J      M
    */
    /*
    printf("MopUpL2: N %i I %i Z %i J2 %i M2 %i\n", N,I,Z,J2,M2);
    { int k;
      for ( k = N; k <= I; k++ ) 
	if ( compareXY(T, A[k]) < 0 ) {
	  printf("Err L1 k %i\n", k);
	  exit(0);
	}
      for ( k = I+1; k <= Z; k++ ) 
	if ( compareXY(A[k], T) <= 0 ) {
	  printf("Err R1 k %i\n", k);
	  exit(0);
	}
      for ( k = Z+1; k < J2; k++ ) 
	if ( compareXY(T, A[k]) < 0 ) {
	  printf("Err L2 k %i\n", k);
	  exit(0);
	}
      for ( k = J2; k <= M2; k++ ) 
	if ( compareXY(A[k], T) <= 0 ) {
	  printf("Err R2 k %i\n", k);
	  exit(0);
	}
    }
    printf("OKOKOKOKOKOKOKOKOK\n");
    // exit(0);
    // */
    
    /*     l       r       l     r           r
       |------][--------]----][-----]-----[------|
       N      I         Z     J2    M2    J      M
    */
    // exchange elements so that there is a l-r partition
    int P = J2 - Z + I - 1;
    { int x = Z-I, y = J2-Z-1, z = (x < y ? x : y );
      int n = I+1, n3 = J2-z, eq = z; // eq is # to be moved
      void *t;
      while ( 0 < eq ) {
	eq--; t = A[n]; A[n++] = A[n3]; A[n3++] = t;
      } 
    }

    /*   l        r
      |------][-------]------------[------]
      N      P        M2           J      M     
     */
    /*
    printf("Post swap: N %i P %i M2 %i J %i\n", N,P,M2,J);
    { int k;
      for ( k = N; k <= P; k++ ) 
	if ( compareXY(T, A[k]) < 0 ) {
	  printf("Err L3 k %i\n", k);
	  exit(0);
	}
      for ( k = P+1; k <= M2; k++ ) 
	if ( compareXY(A[k], T) <= 0 ) {
	  printf("Err R3 k %i\n", k);
	  exit(0);
	}
    }
    printf("OK OK OK OK OK OK \n");
    // exit(0);
    // */

    if ( M2+1 < J ) {
      I = P; Z = M2;
      goto Repeat; // deal with next chunk
    }
    /*
    printf("Pre recur: N %i P %i M %i\n", N,P,M);
    { int k;
      for ( k = N; k <= P; k++ ) 
	if ( compareXY(T, A[k]) < 0 ) {
	  printf("Err L4 k %i\n", k);
	  exit(0);
	}
      for ( k = P+1; k <= M; k++ ) 
	if ( compareXY(A[k], T) <= 0 ) {
	  printf("Err R4 k %i\n", k);
	  exit(0);
	}
    }
    printf("xx OK OK OK OK OK OK \n");
    // exit(0);
    */

    if ( (P - N) < (M - P) ) { // smallest one first
      cut2lrbc(A, N, P, depthLimit, compareXY);
      N = P+1; 
      goto Start;
    }
    cut2lrbc(A, P+1, M, depthLimit, compareXY);
    M = P;
    goto Start;
} // (*  OF cut2lrbc; *) the brackets remind that this was once, 1985, Pascal code

#undef iswap
