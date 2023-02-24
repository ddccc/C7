// c:/bsd/rigel/sort/Dflg2.c
// Date: Sat Feb 18 15:37:17 2023

#define iswap(p, q, A) { void *t3t = A[p]; A[p] = A[q]; A[q] = t3t; }

void dflgm2(void **A, int lo, int hi, int pivotx,
	  void (*cut)(void**, int, int, int,
		      int (*)(const void*, const void*)),
	  int depthLimit, int (*compareXY)(const void*, const void*)) {
  /*
    Simple version of partitioning with: L/M/R
    L < pivot, M = pivot, R > pivot
   */
  // printf("dflgm2 lo %i hi %i pivotx %i\n", lo,hi,pivotx);
  register int i, j, k; // indices
  register void* p3; // pivot
  i = lo; j = i; k = hi; 
  p3 = A[pivotx];

    /*   L    M                 R
      |*---)----)------------(----|
     lo    i    j            k    hi
    */

  while ( j <= k ) {
    int r = compareXY(A[j], p3);
    if ( r < 0 ) {
      iswap(i, j, A);
      i++; j++;
    } else 
      if ( 0 < r) {
	iswap(j, k, A);
	k--;
      } else j++;
  }
 
  // printf("lo %i i %i j %i k %i hi %i\n", lo,i,j,k,hi);

 done: ;
    /*
      |---)------------(------|
     lo   i            k      hi
    */
    /*
      int z;
      for ( z = lo; z < i; z++ )
	// if ( p3 <= A[z] ) {
	if ( compareXY(p3, A[z]) <= 0 ) {
	  printf("doneL z %i\n", z);
	  printf("lo %i i %i  k %i hi %i\n", lo,i,k,hi);
	  exit(0);
	}
      for ( z = i; z <= k; z++ )
	// if ( p3 != A[z] ) {
	if ( compareXY(p3, A[z]) != 0 ) {
	  printf("doneM z %i\n", z);
	  printf("lo %i i %i  k %i hi %i\n", lo,i,k,hi);
	  exit(0);
	}
      for ( z = k+1; z <= hi ; z++ )
	// if ( A[z] <= p3 ) {
	if ( compareXY(A[z], p3) <= 0 ) {
	  printf("doneR z %i\n", z);
	  printf("lo %i i %i  k %i hi %i\n", lo,i,k,hi);
	  exit(0);
	}
      // */
    if ( i - lo  < hi - k ) {
      if ( lo < i ) (*cut)(A, lo, i-1, depthLimit, compareXY);
      if ( k < hi ) (*cut)(A, k+1, hi, depthLimit, compareXY);
      return;
    }
    if ( k < hi ) (*cut)(A, k+1, hi, depthLimit, compareXY);
    if ( lo < i ) (*cut)(A, lo, i-1, depthLimit, compareXY);
} // end dflgm2

#undef iswap
