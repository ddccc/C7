// c:/bsd/rigel/sort/C2sort-NH.c
// Extensively modified version of C2sort.c where the samples used
// for pivot selection are only partially sorted and where the chosen
// pivot is not always the exact median of the samples.
// Programmed by Nigel Horspool, 2021-06-04

#include "Isort.h"

#define iswap(p, q, A) { void *t3t = A[p]; A[p] = A[q]; A[q] = t3t; }

static int quickselectm(void **A, int lo, int hi, int k,
        int depthLimit, int (*cmp)());

static const int dflgmLimitNH2 = 250;
static const int iLimitNH2 = 9;

static void cut2NHc();
// cut2 is used as a best in class quicksort implementation 
// with a defense against quadratic behavior due to duplicates
// cut2 is a support function to call up the workhorse cut2c
void cut2NH(void **A, int N, int M, int (*compare)()) { 
  // printf("cut2 %d %d %d\n", N, M, M-N);
  int L = M - N;
  if ( L <= 0 ) return;
  int depthLimit = 2.9 * floor(log(L));
  cut2NHc(A, N, M, depthLimit, compare);
} // end cut2

// calculate the median of 3
static int medqNH2(void **A, int a, int b, int c,
	int (*cmp ) (const void *, const void * ) ) {
  return
    cmp( A[a], A[b] ) < 0 ?
    ( cmp( A[b], A[c] ) < 0 ? b : cmp( A[a], A[c] ) < 0 ? c : a)
    : cmp( A[b], A[c] ) > 0 ? b : cmp( A[a], A[c] ) > 0 ? c : a;
} // end medqNH2

void cut2NHc(void **A, int N, int M, int depthLimit, 
		 int (*cmp)(const void*, const void*)) {

    while ( N < M ) {
        int L = M - N;

        if ( L <= iLimitNH2) {
            insertionsort(A, N, M, cmp);
            return;
        }

        if ( L < dflgmLimitNH2 ) {
            int p0 = N + (L>>1); // N + L/2;
            if ( 7 < L ) {
                int pn = N;
                int pm = M;
                if ( 40 < L ) {
                    int d = (L-2)>>3; // L/8;
                    pn = medqNH2(A, pn, pn + d, pn + 2 * d, cmp);
                    p0 = medqNH2(A, p0 - d, p0, p0 + d, cmp);
                    pm = medqNH2(A, pm - 2 * d, pm - d, pm, cmp);
                }
                p0 = medqNH2(A, pn, p0, pm, cmp);
            }
            dflgm(A, N, M, p0, cut2NHc, depthLimit, cmp);
            return;
        }
        if ( depthLimit <= 0 ) {
            heapc(A, N, M, cmp);
            return;
        }
        depthLimit--;

        register void *T; // pivot
        register int I = N, J = M; // indices
        int middlex = N + (L>>1); // N + L/2
        void *middle;
        int k, N1, M1; // for sampling
        int probeLng = (L < 81*7)? 9 : sqrt(L/7.0);
        int halfSegmentLng = probeLng >> 1; // probeLng/2;
        N1 = middlex - halfSegmentLng; //  N + (L>>1) - halfSegmentLng;
        M1 = N1 + probeLng - 1;
        int offset = L/probeLng;  

        // assemble the mini array [N1, M1]
        for (k = 0; k < probeLng; k++) {
            int xx = N1 + k, yy = N + k * offset; iswap(xx, yy, A);
        }
        // partially sort this mini array to obtain a median
        middlex = quickselectm(A, N1, M1, middlex, depthLimit, cmp);
        T = middle = A[middlex];
        if ( cmp(A[M1], middle) <= 0 ) {
            // give up because cannot find a good pivot
            // dflgm is a dutch flag type of algorithm
            dflgm(A, N, M, middlex, cut2NHc, depthLimit, cmp);
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
        // register void *AI, *AJ; // array values
        // The left segment has elements <= T
        // The right segment has elements >= T
       for( ; ; ) {
            while ( cmp(A[++I], T) <= 0 ); 
            while ( cmp(A[--J], T) >= 0 ); 
            if (I >= J) break;
            iswap(I, J, A);
        }
        // Tail iteration
        if ( (I - N) < (M - J) ) { // smallest one first
            cut2NHc(A, N, J, depthLimit, cmp);
            N = I; 
        } else {
            cut2NHc(A, I, M, depthLimit, cmp);
            M = J;
        }
    } // end while
}

#undef iswap
