// File: c:/bsd/rigel/sort/DdcChen.c
// Date: Sat May 23 15:18:12 2020
// (C) OntoOO/ Dennis de Champeaux

#define iswap(p, q, A) { void *t3t = A[p]; A[p] = A[q]; A[q] = t3t; }

static const int ddcchenLimit = 1024*1024; 

static void ddcchenc();

void ddcchen(void **A, int N, int M, int (*compare)()) { 
  // printf("ddcchen %d %d %d\n", N, M, M-N);
  int L = M - N;
  if ( L < ddcchenLimit ) { 
    quicksort0(A, N, M, compare);
    return;
  }
  int depthLimit = 1 + 2.5 * floor(log(L));
  ddcchenc(A, N, M, depthLimit, compare);
} // end cut2

static int partitionc();
static void chenL();
static void chenR();
void ddcchenc(void **A, int N, int M, int depthLimit, int (*compare)()) {
  // printf("\nddcchenc %d %d %d\n", N, M, M-N);
  int L;
  L = M - N + 1;

  int k, N1, M1; // for sampling
  int probeLng = sqrt(L/9);
  int halfSegmentLng = probeLng >> 1; // probeLng/2;
  int mx = N +  halfSegmentLng; 
  N1 = N; 
  M1 = N1 + probeLng - 1;
  int offset = L/probeLng;  

  // assemble the mini array [N1, M1]
  for (k = 0; k < probeLng; k++) // iswap(N1 + k, N + k * offset, A);
    { int xx = N1 + k, yy = N + k * offset; iswap(xx, yy, A); }
  // sort this mini array to obtain good pivot
  quicksort0c(A, N1, M1, depthLimit, compare);
  //  register void *T = A[mx]; // pivot

  while (1) {
    int bite = (M1 - N1) >> 1;
    int N2 = M1+1; 
    int M2 = N2 + bite;
    M2 = ( M2 <= M ? M2 : M );
    mx = N + ((M1-N) >>1);
    /*
      |------*-----|-----------|---------------------------|
      N=N1  mx   M1 N2        M2                           M
     */
    chenL(A, N, M1, M2, mx, depthLimit, compare);
    /*
    int j;
    for (j = N+1; j <= M2; j++) {
      void *a = A[j-1], *b = A[j];
      if ( compare(b, a) < 0 ) {
	printf("ERR j %d\n", j);
	printf("N %d mx %d M1 %d N2 %d M2 %d\n", N,mx,M1,N2,M2);
	exit(0);
      }
    }
    */
    M1 = M2;
    if ( M <= M1 ) break;
    //    exit(0);
  }
} // end ddcchenc

int partitionc(void **A, int N, int M, void* pv, int (*compare)()) {
  int i = N;
  while ( i <= M && compare(A[i], pv) <= 0 ) { i++; }
  if ( M < i ) // all elements are less or equal than pv
     return i;
  void* ai = A[i]; 
  // pv < ai & N <= i <= M
  int j = M;
  while ( i <= j && compare(pv, A[j]) <= 0 ) { j--; };

  // j < i or i < j & A[j] < pv
  if ( j < i ) return j; // j can be N-1
  A[i++] = A[j]; A[j--] = ai;
  if ( j < i ) return j;
  if ( i == j ) {
    if ( compare(A[i], pv) <= 0 ) return i;
    return i - 1;
  }
  while ( 1 ) {
    /*
	  |----------)-------------(-----------|
	  N   <=pv   i             j   >=pv    M   
    */
    while ( i <= j && compare(A[i], pv) <= 0 ) { i++; }
    if ( j < i ) return j;
    ai = A[i];
    while ( i <= j && compare(pv, A[j]) <= 0 ) { j--; }
    if ( j < i ) return j;
    A[i++] = A[j]; A[j--] = ai;
    if ( j < i ) return j;
    if ( i == j ) {
      if ( compare(A[i], pv) <= 0 ) return i;
      return i - 1;
    }
  }
} // end partitionc

void chenL(void **A, int N, int M, int M2, int mx,
	   int depthLimit, int (*compare)()) {
  /*
    [--------|-------]-----------|     [N-M] is sorted, mx pivot loc
    N        mx      M           M2
   */

  void *pv = A[mx];

  /* 
     {
    int j;
    for (j = N+1; j <= M; j++) {
      void *a = A[j-1], *b = A[j];
      if ( compare(b, a) < 0 ) {
	printf("ChenLin j %d  N %d M %d\n", j, N, M);
	exit(0);
      }
    }
    } 
  //*/

 if ( depthLimit <= 0 ) {
    heapc(A, N, M2, compare);
    return;
  }
  depthLimit--;
  if ( M2-N <= 10 || M-N < 3) {
    // printf("chenL calls quicksort0c\n");
    quicksort0c(A, N, M2, depthLimit, compare);
    return;
  }
  /*
      |------*-----|-----------|---------------------------|
      N      mx    M           M2                          M
  */
  int lng = M-mx, j;
  for ( j = 0; j < lng; j++ ) 
      { int xx = M-j; int yy = M2-j; iswap(xx, yy, A); }
  int N3 = M2-lng+1;
    /*
      |------*[-----------]------|---------------------------|
      N     mx             N3    M2                          M
     */
  int px = partitionc(A, mx+1, N3-1, pv, compare);

  /*
  if ( 0 == N && M2 == 3071 ) {
    printf("---L px %d\n", px);
    { int n = mx+1; int m = N3-1; int i;
      int up = (px <= m ? px : m);
      int lw = (n <= px ? px+1 : n);
      printf("up %d lw %d\n", up, lw);
      for (i = n; i <= up; i++) 
	if ( compare(pv, A[i]) < 0 ) {
	  printf("partitioncLL ** i %d\n", i);
	  exit(0);
	}
      for (i = lw; i <= m; i++) 
	if ( compare(A[i], pv) < 0 ) {
	  printf("partitioncLR ** i %d\n", i);
	  exit(0);
	}
    }
  }
  // */
  if ( N3 <= px ) { // all elements go left only
    int mx2 = N + ( (mx-N) >> 1 ); // N + (mx-N)/2
    chenL(A, N, mx, N3-1, mx2, depthLimit, compare);
    return;
  }
  if ( px <= mx ) { // all elements go right only
   int mx2 = N3 + ( (M2-N3) >> 1 ); // N3 + (M2-N3)/2
   chenR(A, N3, M2, mx+1, mx2, depthLimit, compare);
   return;
  }

    /*
      |------*[----|------]------|---------------------------|
      N     mx     px      N3    M2                          M
     */
  int mx2 = N + ( (mx-N) >> 1 ); // N + (mx-N)/2
  chenL(A, N, mx, px, mx2, depthLimit, compare);

  mx2 = N3 + ( (M2 - N3) >> 1);
  chenR(A, N3, M2, px+1, mx2, depthLimit, compare);
  /*
    for (j = N+1; j <= M2; j++) {
      void *a = A[j-1], *b = A[j];
      if ( compare(b, a) < 0 ) {
	printf("ERRL j %d  N %d M2 %d\n", j, N, M2);
	exit(0);
      }
    }
    // printf("Leaving  chenL\n");
    // */
}  // end chenL

void chenR(void **A, int N, int M, int M2, int mx, 
	   int depthLimit, int (*compare)()) {
  /*
    [--------[-----|------|     [N-M] is sorted, mx pivot loc
    M2       N     mx     M
  */

  void *pv = A[mx];

  /*{
    int j;
    for (j = N+1; j <= M; j++) {
      void *a = A[j-1], *b = A[j];
      if ( compare(b, a) < 0 ) {
	printf("ChenRin j %d  N %d M %d\n", j, N, M);
	exit(0);
      }
    }
    } */

  // printf("chenR M2 %d N %d mx %d M %d\n", M2, N, mx, M);

  if ( depthLimit <= 0 ) {
    heapc(A, M2, M, compare);
    return;
  }
  depthLimit--;
  if ( M-M2 <= 10 || M - N < 3) {
    // printf("chenR calls quicksort0c\n");
    quicksort0c(A, M2, M, depthLimit, compare);
    return;
  }
  /*
    [--------[-----|------|     [N-M] is sorted, mx pivot loc
    M2       N     mx     M
   */
  int lng = mx-N+1, j;
  for ( j = 0; j < lng; j++ ) 
      { int xx = N+j; int yy = M2+j; iswap(xx, yy, A); }
  int N3 = M2 + lng;
  /*
    [----[--------|------|     
    M2   N3       mx     M
   */

  int px = partitionc(A, N3, mx, pv, compare);
  /*
  if ( 2707 == M2 && 2786 == M )
    printf("---R px %d\n", px);
  { int n = N3; int m = mx; int i;
    int up = (px <= m ? px : m);
    int lw = (n <= px ? px+1 : n);
    for (i = n; i <= up; i++) 
      if ( compare(pv, A[i]) < 0 ) {
	printf("partitioncRL ** i %d\n", i);
	exit(0);
    }
   for (i = lw; i <= m; i++) 
     if ( compare(A[i], pv) < 0 ) {
	printf("partitioncRR ** i %d\n", i);
	exit(0);
    }
  }
  // */
  if ( mx < px ) { // all elements go left only
    int mx2 = M2 + ((N3-M2) >> 1);
    chenL(A, M2, N3-1, mx, mx2, depthLimit, compare);
    return;
  }
  if ( px < N3 ) { // all elements go right only
    int mx2 = mx + ( (M-mx) >> 1 );
    chenR(A, mx+1, M, N3, mx2, depthLimit, compare);
    return;
 }

  /*
    [----[----|-----|------|     
    M2   N3   px    mx     M
   */
  int mx2 = M2 + ((N3-M2) >> 1);
  chenL(A, M2, N3-1, px, mx2, depthLimit, compare);
  mx2 = mx + ( (M-mx) >> 1 );
  chenR(A, mx+1, M, px+1, mx2, depthLimit, compare);
  /*
  { for (j = M2+1; j <= M; j++) {
      void *a = A[j-1], *b = A[j];
      if ( compare(b, a) < 0 ) {
	printf("ERRR j %d  M2 %d M %d\n", j, M2, M);
	exit(0);
      }
    }
    // printf("Leaving  chenR\n");
  }
  //  */
} // end chenR

#undef iswap
