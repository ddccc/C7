// c:/bsd/rigel/sort/C2m.c
// Date: Sat Nov 13 18:40:13 2021
// (C) OntoOO/ Dennis de Champeaux

#define iswap(p, q, A) { void *t3t = A[p]; A[p] = A[q]; A[q] = t3t; }
// This version combines isort + dflgm + ( pivot sample + (fast loops | dflgm ) )

static const int dflgmLimit4 = 250;
static const int iLimit4 = 9;

void cut2mc(); // is called also
// cut2 is used as a best in class quicksort implementation 
// with a defense against quadratic behavior due to duplicates
// cut2 is a support function to call up the workhorse cut2c
void cut2m(void **A, int lo, int hi, int (*compare)()) { 
  // printf("cut2m %d %d %d\n", lo, hi, hi-lo);
  int L = hi - lo;
  int depthLimit = 2.9 * floor(log(L));
  cut2mc(A, lo, hi, depthLimit, compare);
} // end cut2
// calculate the median of 3
static int medq4(void **A, int a, int b, int c,
	int (*compareXY ) (const void *, const void * ) ) {
  return
    compareXY( A[a], A[b] ) < 0 ?
    ( compareXY( A[b], A[c] ) < 0 ? b : compareXY( A[a], A[c] ) < 0 ? c : a)
    : compareXY( A[b], A[c] ) > 0 ? b : compareXY( A[a], A[c] ) > 0 ? c : a;
} // end medq2

void cut2mc(void **A, int lo, int hi, int depthLimit, 
		 int (*compareXY)(const void*, const void*)) {
  // printf("Enter cut2mc lo: %d hi: %d %d\n", lo, hi, depthLimit);
  // printf(" gap %d \n", hi-lo);

  while ( lo < hi ) {
    // printf("cut2mc lo: %d hi %d  L %i\n", lo, hi, hi-lo);
    int L = hi - lo;
    /*
    if ( L <= iLimit2) {
      dflgm3(A, lo, hi, depthLimit, compareXY);
      return;
    }
    */
    if ( L <= iLimit4) {
      insertionsort(A, lo, hi, compareXY);
      return;
    }
  
    if ( L < dflgmLimit4 ) {
      int p0 = lo + (L>>1); // lo + L/2;
      if ( 7 < L ) {
	int pn = lo;
	int pm = hi;
	// if ( 51 < L ) {
	if ( 40 < L ) {
	  int d = (L-2)>>3; // L/8;
	  pn = medq4(A, pn, pn + d, pn + 2 * d, compareXY);
	  p0 = medq4(A, p0 - d, p0, p0 + d, compareXY);
	  pm = medq4(A, pm - 2 * d, pm - d, pm, compareXY);
	}
	p0 = medq4(A, pn, p0, pm, compareXY);
      }
      dflgm(A, lo, hi, p0, cut2c, depthLimit, compareXY);
      return;
    }
    if ( depthLimit <= 0 ) {
      heapc(A, lo, hi, compareXY);
      return;
    }
    depthLimit--;
  
    register void *T; // pivot
    register int I = lo, J = hi; // indices
    // int middlex = lo + (L>>1); // lo + L/2
    int k, lo1, hi1; // for sampling
    int probeLng = sqrt(L/7.0); // 2/3 ?
    // int probeLng = sqrt(L/6.5); // - - 
    // int probeLng = sqrt(L/6.0); // 0.99 no good
    if ( probeLng < 9 ) probeLng = 9;

    // int probeLng = L/10000;     
    // if ( probeLng < 9 ) probeLng = 9;

    // int halfSegmentLng = probeLng >> 1; // probeLng/2;
    // lo1 = middlex - halfSegmentLng; //  lo + (L>>1) - halfSegmentLng;
    lo1 = lo;
    hi1 = lo1 + probeLng - 1;
    int middlex = lo1 + (probeLng >> 1); // probeLng/2
    int offset = L/probeLng;  
  
    // assemble the mini array [lo1, hi1]
    for (k = 0; k < probeLng; k++) // iswap(lo1 + k, lo + k * offset, A);
      { int xx = lo1 + k, yy = lo + k * offset; iswap(xx, yy, A); }
    // sort this mini array to obtain good pivots
    // quicksort0c(A, lo1, hi1, depthLimit, compareXY);
    cut2mc(A, lo1, hi1, depthLimit, compareXY);
    T = A[middlex];
    if ( compareXY(A[hi1], T) <= 0 ) {
      // give up because cannot find a good pivot
      // dflgm is a dutch flag type of algorithm
      int center = lo + (L>>1); // lo + L/2
      iswap(middlex, center, A);
      dflgm(A, lo, hi, center, cut2mc, depthLimit, compareXY);
      return;
    }

    /*      L            R     
      |------------]----------]----------------------|
      lo=lo1   middlex     hi1=k                    hi=J
    */

    I = middlex;

    for ( k = hi1; middlex < k; k--) {
      iswap(k, J, A); J--;
    }
    J++;

    // get the ball rolling::
    register void *AI, *AJ; // array values
    // The left segment has elements <= T
    // The right segment has elements >= T
  Left:
    while ( compareXY(A[++I], T) <= 0 ); 
    AI = A[I];
    while ( compareXY(T, A[--J]) < 0 ); 
    AJ = A[J];
    if ( I < J ) { // swap
      A[I] = AJ; A[J] = AI;
      goto Left;
    }
    // Done:
    // Tail iteration
    if ( (I - lo) < (hi - J) ) { // smallest one first
      cut2c(A, lo, J, depthLimit, compareXY);
      lo = I; 
    } else {
      cut2c(A, I, hi, depthLimit, compareXY);
      hi = J;
    }
  } // end while
} // (*  OF cut2; *) the brackets remind that this was once, 1985, Pascal code

#undef iswap
