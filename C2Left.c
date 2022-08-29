// c:/bsd/rigel/sort/C2Left.c
// Date: Wed Jun 10 15:37:30 2020 & Sun Nov 01 19:27:30 2020
// (C) OntoOO/ Dennis de Champeaux

#define minL(a, b)	((a) < (b) ? (a) : (b))
#define iswap(p, q, A) { void *t3t = A[p]; A[p] = A[q]; A[q] = t3t; }

static void cut2lrc();
const int cut2LeftLimit =  600; 

const int bufSizeL = 200;

void cut2leftc();
// cut2left is a support function to call up the workhorse cut2leftc
void cut2left(void **A, int N, int M, int (*compare)()) { 
  // printf("cut2left %d %d %d\n", N, M, M-N);
  int L = M - N;
  if ( L < cut2LeftLimit ) { 
    quicksort0(A, N, M, compare);
    return;
  }
  int depthLimit = 1 + 2.5 * floor(log(L));
  cut2leftc(A, N, M, depthLimit, compare);
} // end cut2


// cut2leftc does 2-partitioning with one pivot.
// cut2leftc invokes dflgm when trouble is encountered.
void cut2leftc(void **A, int N, int M, 
	       int depthLimit, int (*compareXY)()) {  
  int bufl[bufSizeL];
  int L;
 Start:
  // printf("cut2leftc N %d M %d %d\n", N, M, M-N);

  L = M - N + 1;

  if ( L <= 4 * 1024 ) {
    cut2lrc(A, N, M, depthLimit, compareXY);
    // quicksort0c(A, N, M, depthLimit, compareXY);
    return;
  }

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
    /*
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
    */
    quicksort0c(A, N1, M1, depthLimit, compareXY); 
    T = middle = A[middlex];
    if ( compareXY(A[M1], middle) <= 0 ) {
      // give up because cannot find a good pivot
      // dflgm is a dutch flag type of algorithm
      void cut2c();
      dflgm(A, N, M, middlex, cut2c, depthLimit, compareXY);
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
  
  //   register void *AI, *AJ; // array values
	// The left segment has elements <= T
	// The right segment has elements >= T 
        //    and at least one element > T
  // int kl, kr, idxl, idxr; 
  int kl, idxl; 
  int bufx = bufSizeL;

  //  int M2, I2, M3;
  int M2;

  /*
       |------]--------------------[------|
       N      I                    J      M
   { int z; for ( z = N; z<=I; z++ ) 
	       if ( compareXY(T, A[z]) < 0 ) {
		   printf("ERR0 %d %d %d", N, M, M-N);
		   printf(" Err0 %i %i z %i\n", N, I, z);
		   exit(0);
		 }
   }
    { int z; for ( z = J; z<=M; z++ ) 
		 if ( compareXY(A[z], T) < 0 ) {
		   printf("ERR1 %d %d %d", N, M, M-N);
		   printf(" Err1 %i %i z %i\n", J, M, z);
		   exit(0);
		 }
    }
   */
   // printf("start    N %i I %i J %i M %i\n", N,I,J,M);
   /*      l                           r
       |------]--------------------[------|
       N      I                    J      M
   */
    // find first element that belongs right
    while ( compareXY(A[++I], T) <= 0 );

    if ( I == J ) { // very weird 
      int middlex = N + (L>>1); // N + L/2;
      int p0 = middlex;
      if ( 7 < L ) {
	int pn = N;
	int pm = M;
	if ( 51 < L ) {	
	  int d = (L-2)>>3; // L/8;
	  pn = med(A, pn, pn + d, pn + 2 * d, compareXY);
	  p0 = med(A, p0 - d, p0, p0 + d, compareXY);
	  pm = med(A, pm - 2 * d, pm - d, pm, compareXY);
	}
	p0 = med(A, pn, p0, pm, compareXY);
      }
      if ( middlex != p0 ) iswap(p0, middlex, A);
      dflgm(A, N, M, middlex, cut2leftc, depthLimit, compareXY);
      return;
    }

    int K = I;
    I--;
 
 Left:
    // printf("Left N %i I %i K %i J %i M %i\n", N,I,K,J,M);
    M2 =  K + bufx;
    // I2 = K;
    if ( J <= M2 ) M2 = J-1;

    /*     l    r                       r
       |------]---]-----]------------[------|
       N      I   K     M2           J      M
    */
    // printf("XXX  N %i I %i I2 %i K %i M2 %i J %i M %i\n", N,I,I2, K,M2,J,M);
    /*
   { int z; for ( z = N; z<=I; z++ ) 
	       if ( compareXY(T, A[z]) < 0 ) {
		   printf("ERR2 %d %d %d", N, M, M-N);
		   printf(" Err2 %i %i z %i\n", N, I, z);
		   exit(0);
		 } }
    { int z; for ( z = I+1; z<=K; z++ ) 
		 if ( compareXY(A[z], T) < 0 ) {
		   printf("ERR3 %d %d %d", N, I, K);
		   printf(" Err3 %i %i z %i\n", I, K, z);
		   exit(0);
		 } }
    */

    /*     l    r                       r
       |------]---]-----]------------[------|
       N      I   K     M2           J      M
    */
    // locate elements that belong left
    kl = -1;
    int j;
    for (j = K+1; j <= M2; j++)
      if ( compareXY(A[j], T) <= 0 ) bufl[++kl] = j;
    /*     l    r                         r
       |------]---]-*--*--]------------[------|
       N      I   K       M2           J      M
    */
    // swap them to the left
    I = I+1;
    for ( j = 0; j <= kl; j++ ) {
      idxl = bufl[j];
      iswap(I, idxl, A); I++;
    }
    I--;
    /*     l      r                    r
       |------]--------]------------[------|
       N      I        M2           J      M
    printf("     N %i I %i M2 %i J %i M %i\n", N,I,M2,J,M);
    { int z; for ( z = N; z<=I; z++ ) 
	       if ( compareXY(T, A[z]) < 0 ) {
		 printf("ERR4 %d %d %d", N, M, M-N);
		 printf(" Err4 %i %i z %i\n", N, I, z);
		 exit(0);
	       }
    }
    */
    if ( M2+1 < J ) { K = M2; goto Left; }    

    //  Done:
    if ( (I - N) < (M - I) ) { // smallest one first
      cut2leftc(A, N, I, depthLimit, compareXY);
      N = I+1;
      goto Start;
    }
    cut2leftc(A, I+1, M, depthLimit, compareXY);
    M = I;
    goto Start;


} // (*  OF cut2lefc; *) the brackets remind that this was once, 1985, Pascal code

#undef iswap
#undef min
