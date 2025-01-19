// c:/bsd/rigel/sort/C7/C2.c
// Date: Fri Jan 31 13:32:12 2014, 2017 Sun Mar 03 16:14:28 2019
// (C) OntoOO/ Dennis de Champeaux
#include "Isort.h"

void *myMalloc();

static void c2c();
// c2 is a support function to call up the workhorse c2c
void c2(void **A, int N, int M, int (*compare)()) { 
  if ( M <= N ) return;
  // printf("cut2 %d %d %d\n", N, M, M-N);
  int L = M - N + 1;
  void **C = myMalloc("c2", L*sizeof(A)); // copy file
  c2c(A, C, N, M, compare);
} // end cut2

// static void quicksort1c(void **, int, int, int, int (*)(const void*, const void*));

// Cut2c does 2-partitioning with one pivot.
// Cut2c invokes dflgm when trouble is encountered.

void c2c(void **A, void **C, int N, int M, int (*compare)()) {
  // printf("c2c %d %d %d\n", N, M, M-N);
  int L;
  L = M - N + 1;
  if ( L < 9 ) { 
    insertionsort(A, N, M, compare);
    return;
  }
  int mid = N + (L>>1); // N + L/2;
  c2c(A, C, N, mid, compare); // sort left
  c2c(A, C, mid+1, M, compare); // sort right
  int i = N, j = mid+1, k = 0;
  while ( i <= mid && j <= M ) // merge
    C[k++] = ( compare(A[i], A[j]) <= 0 ?  A[i++] : A[j++] );
  while ( i <= mid ) C[k++] =  A[i++]; // remaining left
  // while ( j <= M ) C[k++] =  A[j++]; // remaining right - skip
  i = N; k = 0;
  // while ( i <= M ) A[i++] = C[k++]; // move back - skip
  while ( i < j ) A[i++] = C[k++]; // move back - instead

} // (* end of c2c; *) the brackets remind that this was once, 1985, Pascal code
