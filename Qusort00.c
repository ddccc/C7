// c:/bsd/rigel/sort/Qusort00.c
// Date: Sun May 23 15:36:59 2021
// (C) OntoOO/ Dennis de Champeaux

// This version combines isort + dflgm + ( pivot sample + (fast loops | dflgm ) )
// It is a clone of cut2 --  Fri May 28 21:20:05 2021

#include "Isort.h"

#define iswap(p, q, A) { void *t3t = A[p]; A[p] = A[q]; A[q] = t3t; }

static const int dflgmLimit3 = 250;
static const int iLimit3 = 9;
static void quicksort3c(void **, int, int, int, int (*)(const void*, const void*));

void quicksort3(void **A, int N, int M, int (*compare)(const void*, const void*)) {
  // printf("quicksort3 N %i M %i L %i\n", N, M, M-N);
  int L = M - N;
  if ( L <= 0 ) return;
  int depthLimit = 2.9 * floor(log(L));
  quicksort3c(A, N, M, depthLimit, compare);
} // end quicksort3

// Quicksort equipped with a defense against quadratic explosion;
// calling heapsort if depthlimit exhausted

// calculate the median of 3
static int medq3(void **A, int a, int b, int c,
	int (*compareXY ) (const void *, const void * ) ) {
  return
    compareXY( A[a], A[b] ) < 0 ?
    ( compareXY( A[b], A[c] ) < 0 ? b : compareXY( A[a], A[c] ) < 0 ? c : a)
    : compareXY( A[b], A[c] ) > 0 ? b : compareXY( A[a], A[c] ) > 0 ? c : a;
} // end medq3

void quicksort3c(void **A, int N, int M, int depthLimit, 
		 int (*compareXY)(const void*, const void*)) {
  // printf("Enter quicksort3c N: %d M: %d %d\n", N, M, depthLimit);
  // printf(" gap %d \n", M-N);

  while ( N < M ) {
    // printf("quicksort3c N: %d M %d  L %i\n", N, M, M-N);
    int L = M - N;
    /*
    if ( L <= iLimit3) {
      dflgm3(A, N, M, depthLimit, compareXY);
      return;
    }
    // */
    // /*
    if ( L <= iLimit3) {
      insertionsort(A, N, M, compareXY);
      return;
    }

    if ( L < dflgmLimit3 ) {
      int p0 = N + (L>>1); // N + L/2;
      if ( 7 < L ) {
	int pn = N;
	int pm = M;
	// if ( 51 < L ) {
	if ( 40 < L ) {
	  int d = (L-2)>>3; // L/8;
	  pn = medq3(A, pn, pn + d, pn + 2 * d, compareXY);
	  p0 = medq3(A, p0 - d, p0, p0 + d, compareXY);
	  pm = medq3(A, pm - 2 * d, pm - d, pm, compareXY);
	}
	p0 = medq3(A, pn, p0, pm, compareXY);
      }
      dflgm(A, N, M, p0, quicksort3c, depthLimit, compareXY);
      return;
    }
    // */
    
    if ( depthLimit <= 0 ) {
      heapc(A, N, M, compareXY);
      return;
    }
    depthLimit--;

    register void *T; // pivot
    register int I = N, J = M; // indices
    int middlex = N + (L>>1); // N + L/2
    void *middle;
    int k, N1, M1; // for sampling
    int probeLng = sqrt(L/7.0); if ( probeLng < 9 ) probeLng = 9;
    int halfSegmentLng = probeLng >> 1; // probeLng/2;
    N1 = middlex - halfSegmentLng; //  N + (L>>1) - halfSegmentLng;
    M1 = N1 + probeLng - 1;
    int offset = L/probeLng;  

    // assemble the mini array [N1, M1]
    for (k = 0; k < probeLng; k++) // iswap(N1 + k, N + k * offset, A);
      { int xx = N1 + k, yy = N + k * offset; iswap(xx, yy, A); }
    // sort this mini array to obtain good pivots
    // quicksort0c(A, N1, M1, depthLimit, compareXY);
    // cut2c(A, N1, M1, depthLimit, compareXY);
    quicksort3c(A, N1, M1, depthLimit, compareXY);
    T = middle = A[middlex];
    if ( compareXY(A[M1], middle) <= 0 ) {
      // give up because cannot find a good pivot
      // dflgm is a dutch flag type of algorithm
      dflgm(A, N, M, middlex, quicksort3c, depthLimit, compareXY);
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
    // Tail iteration
    if ( (I - N) < (M - J) ) { // smallest one first
      quicksort3c(A, N, J, depthLimit, compareXY);
      N = I; 
    } else {
      quicksort3c(A, I, M, depthLimit, compareXY);
      M = J;
    }
  } // end while
} // (*  OF cut2; *) the brackets remind that this was once, 1985, Pascal code

#undef iswap
