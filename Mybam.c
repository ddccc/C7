// c:/bsd/rigel/sort/Mybam.c
// Date: Thu Dec 17 09:22:26 2020
// (C) OntoOO/ Dennis de Champeaux

/*
#include "Hsort.c"
#include "Dsort.c"
*/

#include "Isort.h"

#define iswap(p, q, A) { void *t3t = A[p]; A[p] = A[q]; A[q] = t3t; }

static void mybamc(void **, int, int, int, int (*)(const void*, const void*));

// calculate the median of 3
static int med2(void **A, int a, int b, int c,
	int (*compareXY ) (const void *, const void * ) ) {
  return
    compareXY( A[a], A[b] ) < 0 ?
    ( compareXY( A[b], A[c] ) < 0 ? b : compareXY( A[a], A[c] ) < 0 ? c : a)
    : compareXY( A[b], A[c] ) > 0 ? b : compareXY( A[a], A[c] ) > 0 ? c : a;
} // end med2

static void vswap2(void **A, int N, int N3, int eq) {
  void *t;
  while ( 0 < eq ) { eq--; t = A[N]; A[N++] = A[N3]; A[N3++] = t; }
}

static const int small2 = 400;

void mybam(void **A, int N, int M, int (*compare)(const void*, const void*)) {
  // printf("mybam N %i M %i L %i\n", N, M, M-N);
  int L = M - N;
  if ( L <= 0 ) return;
  if ( L < 9 ) { 
    insertionsort(A, N, M, compare);
    return;
  }
  int depthLimit = 2.5 * floor(log(L));
  mybamc(A, N, M, depthLimit, compare);
} // end mybam

// mybam equipped with a defense against quadratic explosion;
// calling heapsort if depthlimit exhausted

void mybamc(void **A, int N, int M, int depthLimit, 
		 int (*compareXY)(const void*, const void*)) {
  // printf("Enter mybamc N: %d M: %d %d\n", N, M, depthLimit);
  // printf(" gap %d \n", M-N);
  while ( N < M ) {
    // printf("mybamc N: %d M %d  L %i\n", N, M, M-N);
    int L = 1 + M - N;
    // if ( L < 8 ) {
    if ( L < 9) {
      insertionsort(A, N, M, compareXY);
      return;
    }
    if ( depthLimit <= 0 ) {
      heapc(A, N, M, compareXY);
      return;
    }
    depthLimit--;

    // 7 <= L
    int p0 = N + (L>>1); // N + L/2;
    if ( 7 < L ) {
      int pn = N;
      int pm = M;
      // if ( 51 < L ) {
      if ( 40 < L ) {
	int d = (L-2)>>3; // L/8;
	pn = med2(A, pn, pn + d, pn + 2 * d, compareXY);
	p0 = med2(A, p0 - d, p0, p0 + d, compareXY);
	pm = med2(A, pm - 2 * d, pm - d, pm, compareXY);
      }
      p0 = med2(A, pn, p0, pm, compareXY);
    }

    /* optional check when inputs have many equal elements
    if ( compareXY(A[N], A[M]) == 0 ) {
      dflgm(A, N, M, p0, quicksort0c, depthLimit, compareXY);
      return;
    } */

    // p0 is index to 'best' pivot ...    
    iswap(N, p0, A); // ... and is put in first position

    register void *T = A[N]; // pivot
    register int I, J; // indices
    //    register void *AI, *AJ; // array values

      // This is a B&M variant
      I = N+1; J = M; 
      int N2 = I, M2 = J, l, r, eql, eqr;
    Left2:
      while ( I <= J && (r = compareXY(A[I], T)) <= 0 ) {
	  if ( 0 == r ) { iswap(N2, I, A); N2++; }
	  I++;
      }
      while ( I <= J && (r = compareXY(A[J], T)) >= 0 ) {
	if ( 0 == r ) { iswap(M2, J, A); M2--; }
	J--;
      }
      if ( I > J ) goto Skip2; 
      iswap(I, J, A);
      I++; J--;
      goto Left2;

  Skip2:
      // printf("N %i i %i j %i M %i\n",N,I,J,M);
      l = N2-N; r = I-N2;
      eql = ( l < r ? l : r );
      vswap2(A, N, I-eql, eql); 
      int M3 = J+N-N2;
      l = M2-J; r = M-M2;
      eqr = ( l < r ? l : r );
      vswap2(A, I, M-eqr+1, eqr);
      int N3 = I + (M-M2);
      int left = M3-N;
      int right = M-N3;
      if ( left <= right) {
	if ( 0 < left ) mybamc(A, N, M3, depthLimit, compareXY);
	N = N3;
	if ( N < M ) { continue; }
	return;
      }
      if ( 0 < right ) mybamc(A, N3, M, depthLimit, compareXY);
      M = M3;
      if ( N < M ) { continue; }
      return;
  } // end of while loop

} // end of mybamc

#undef iswap
