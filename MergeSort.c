// File: c:/bsd/rigel/sort/MergeSort.c
// Date: Wed Apr 07 20:51:35 2021
// (C) OntoOO/ Dennis de Champeaux


static void mergeSortc();
void mergeSort(void **A, int lo, int hi, int (*compare)(const void*, const void*)) {
  // printf("mergeSort-- lo: %i hi %i\n ", lo, hi);
  if ( hi <= lo ) return;
  int lng = hi - lo + 1;
  // void *B[lng];
  // void *B = malloc(lng*sizeof(void));
  // struct intval *pi;
  // void *B = malloc(lng*sizeof(pi));
  void *B = malloc(lng*sizeof(A[0]));
  if ( NULL == B ) {
    cut2lr2(A, lo, hi, compare);
    return;
  }
  mergeSortc(A, B, lo, hi, compare);
  free(B);
} // end mergeSort

static void merge(); void checkMS();
void mergeSortc(void **A, void **B, int lo, int hi, int (*compare)()) {
  // printf("mergeSort lo: %i hi %i\n ", lo, hi);
	int L = hi-lo;
      	if ( L <= 10 ) { 
	  insertionsort(A, lo, hi, compare); 
	  return; 
	}
	/*
	if ( L < 1300 ) {
	  int depthLimit = 2.9 * floor(log(L));
	  dflgm3(A, lo, hi, depthLimit, compare);
	  // quicksort0(A, lo, hi, depthLimit, compareXY);
	  return;
	}
	*/
	int k = L>>1; // L/2;
	mergeSortc(A, B, lo, lo+k, compare); 
	mergeSortc(A, B, lo+k+1, hi, compare);
	if ( compare(A[lo+k], A[lo+k+1]) > 0 ) 
	     merge(A, B, lo, hi, compare);
	// checkMS(A, lo, hi, compare);
    } // end mergeSortc

void checkMS(void **A, int lo, int hi, int (*compare)()) {
  int i;
  for ( i = lo+1; i <= hi; i++)
    if ( compare(A[i], A[i-1]) < 0 ) {
      printf("mergesort check lo: %i hi: %i err i: %i\n", lo,hi,i);
      exit(0);
    }
}

void merge(void **A, void **B, int lo, int hi, int (*compare)()) {
  // printf("merge lo: %i hi %i\n ", lo, hi);
	int L = hi-lo;
	int k = L>>1; // L/2;
	int i = lo; int j = lo+k+1;  
	int z = 0; // index for B
	int leftDone = 0;
	int rightDone = 0;
	void *ai = A[i]; void *aj = A[j];
	while (1) {
	  // while ( ai <= aj ) {
	  while ( compare(ai, aj) <= 0 ) {
	    B[z] = ai; z++;
	    if ( i < lo+k ) { i++; ai = A[i]; } else {
	      leftDone = 1; break;
	    }
	  }
	  if ( leftDone ) break;
	  // aj < ai
	  B[z] = aj; z++;
	  if ( hi == j ) { rightDone = 1; break; }
	  j++; aj = A[j];
	  // while ( aj < ai ) {
	  while ( compare(aj, ai) < 0 ) {
	    B[z] = aj; z++;
	    if ( j < hi ) { j++; aj = A[j]; } else {
	      rightDone = 1; break;
	    }
	  }
	  if ( rightDone ) break;
	  // ai <= aj
	  B[z] = ai; z++;
	  if ( i == lo+k ) { leftDone = 1; break; }
	  i++; ai = A[i];
	}

	int q;
	if ( leftDone ) {
	  // printf("leftDone z: %i \n", z);
	  for ( q = lo; q < z+lo; q++ ) A[q] = B[q-lo];
	} else { // rightDone
	  int k2 = L-k;
	  for ( q = lo+k; i <= q; q-- ) A[q + k2] = A[q];
	  for ( q = lo; q < z+lo; q++ ) A[q] = B[q-lo];
	}

    } // end merge
