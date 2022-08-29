// File: c:/bsd/rigel/sort/MergeSort.c
// Date: Wed Apr 07 20:51:35 2021
// (C) OntoOO/ Dennis de Champeaux


static void mergeSortc();
void mergeSort(void **A, int N, int M, int (*compare)(const void*, const void*)) {
  int lng = M - N + 1;
  void  *B[lng];
  mergeSortc(A, B, N, M, compare);
} // end mergeSort

static void merge(); void checkMS();
void mergeSortc(void **A, void **B, int N, int M, int (*compare)()) {
  // printf("mergeSort N: %i M %i\n ", N, M);
	int L = M-N;
	if ( L <= 10 ) { 
	  insertionsort(A, N, M, compare); 
	  return; 
	}
	int k = L>>1; // L/2;
	mergeSortc(A, B, N, N+k, compare); 
	mergeSortc(A, B, N+k+1, M, compare);
	if ( compare(A[N+k], A[N+k+1]) > 0 ) 
	     merge(A, B, N, M, compare);
	// checkMS(A, N, M, compare);
    } // end mergeSortc

void checkMS(void **A, int N, int M, int (*compare)()) {
  int i;
  for ( i = N+1; i <= M; i++)
    if ( compare(A[i], A[i-1]) < 0 ) {
      printf("mergesort check N: %i M: %i err i: %i\n", N,M,i);
      exit(0);
    }
}

void merge(void **A, void **B, int N, int M, int (*compare)()) {
  // printf("merge N: %i M %i\n ", N, M);
	int L = M-N;
	int k = L>>1; // L/2;
	int i = N; int j = N+k+1; int z = 0; // int z = N;
	int leftDone = 0;
	int rightDone = 0;
	void *ai = A[i]; void *aj = A[j];
	while (1) {
	  // while ( ai <= aj ) {
	  while ( compare(ai, aj) <= 0 ) {
	    B[z] = ai; z++;
	    if ( i < N+k ) { i++; ai = A[i]; } else {
	      leftDone = 1; break;
	    }
	  }
	  if ( leftDone ) break;
	  // aj < ai
	  B[z] = aj; z++;
	  if ( M == j ) { rightDone = 1; break; }
	  j++; aj = A[j];
	  // while ( aj < ai ) {
	  while ( compare(aj, ai) < 0 ) {
	    B[z] = aj; z++;
	    if ( j < M ) { j++; aj = A[j]; } else {
	      rightDone = 1; break;
	    }
	  }
	  if ( rightDone ) break;
	  // ai <= aj
	  B[z] = ai; z++;
	  if ( i == N+k ) { leftDone = 1; break; }
	  i++; ai = A[i];
	}
	// System.out.println("------");
	// for ( int p = i; p <= N+k; p++ ) System.out.println(p + " " + A[p]);
	int q;
	if ( leftDone ) {
	  // System.out.println("leftDone z: " + z);
	  // for ( q = N; q < z; q++ ) A[q] = B[q];
	  for ( q = N; q < z+N; q++ ) A[q] = B[q-N];
	} else { // rightDone
	    // System.out.println("rightDone z: " + z + " i: " + i + " k: " + k);
	  for ( q = N+k; i <= q; q-- ) A[q + L - k] = A[q];
	  // for ( q = N; q < z; q++ ) A[q] = B[q];
	  for ( q = N; q < z+N; q++ ) A[q] = B[q-N];
	}
    } // end merge
