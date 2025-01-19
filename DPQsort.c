// File: c:/bsd/rigel/sort/C7/DPQsort.c
// Date: Sun Jan 19 09:03:28 2025

void dpqSort(void **a, int left, int right,
	     int (*compar ) (const void *, const void * ) );
void dpq(void **A, int N, int M, int (*compare)()) {
  dpqSort(A, N, M, compare);
} // end dpq


int INSERTION_SORT_THRESHOLD = 47;

    /**
     * Sorts the specified range of the array into ascending order. This
     * method differs from the public {@code sort} method in that the
     * {@code right} index is inclusive, and it does no range checking on
     * {@code left} or {@code right}.
     *
     * @param a the array to be sorted
     * @param left the index of the first element, inclusive, to be sorted
     * @param right the index of the last element, inclusive, to be sorted
     */
    // private static void doSort(int[] a, int left, int right) {
void dualPivotQuicksort();
void dpqSort(void **a, int left, int right,
	  int (*compar ) (const void *, const void * ) ) {
        // Use insertion sort on tiny arrays
        if (right - left + 1 < INSERTION_SORT_THRESHOLD) {
	  int k, j;
	  void *ak;
            for (k = left + 1; k <= right; k++) {
                ak = a[k];
                for (j = k - 1; j >= left &&
		       // ak < a[j]; 
		       compar(  ak,  a[j] ) < 0;
		     j--) {
                    a[j + 1] = a[j];
                }
                a[j + 1] = ak;
            }
        } else { // Use Dual-Pivot Quicksort on large arrays
	  dualPivotQuicksort(a, left, right, compar);
        }
} // end dpqSort

    /**
     * Sorts the specified range of the array into ascending order by the
     * Dual-Pivot Quicksort algorithm.
     *
     * @param a the array to be sorted
     * @param left the index of the first element, inclusive, to be sorted
     * @param right the index of the last element, inclusive, to be sorted
     */
    // private static void dualPivotQuicksort(int[] a, int left, int right) {
void dualPivotQuicksort(void **a, int left, int right,
		   int (*compar ) (const void *, const void * ) ) {
  int length = right-left;

  // Compute indices of five evenly spaced elements
  int sixth = (right - left + 1) / 6;
  int e1 = left  + sixth;
  int e5 = right - sixth;
  int e3 = left + (length>>1); // (left + right)/ 2; // The midpoint
  int e4 = e3 + sixth;
  int e2 = e3 - sixth;

  // Sort these elements using a 5-element sorting network
  void *ae1 = a[e1], *ae2 = a[e2], *ae3 = a[e3], *ae4 = a[e4], *ae5 = a[e5];
  void *t;
  // if (ae1 > ae2) 
  if ( compar(  ae1,  ae2 ) > 0 )
    { t = ae1; ae1 = ae2; ae2 = t; }
  // if (ae4 > ae5) 
  if ( compar(  ae4,  ae5 ) > 0 )
    { t = ae4; ae4 = ae5; ae5 = t; }
  // if (ae1 > ae3) 
  if ( compar(  ae1,  ae3 ) > 0 )
    { t = ae1; ae1 = ae3; ae3 = t; }
  // if (ae2 > ae3) 
  if ( compar(  ae2,  ae3 ) > 0 )
    { t = ae2; ae2 = ae3; ae3 = t; }
  // if (ae1 > ae4) 
  if ( compar(  ae1,  ae4 ) > 0 )
    { t = ae1; ae1 = ae4; ae4 = t; }
  // if (ae3 > ae4) 
  if ( compar(  ae3,  ae4 ) > 0 )
    { t = ae3; ae3 = ae4; ae4 = t; }
  // if (ae2 > ae5) 
  if ( compar(  ae2,  ae5 ) > 0 )
    { t = ae2; ae2 = ae5; ae5 = t; }
  // if (ae2 > ae3) 
  if ( compar(  ae2,  ae3 ) > 0 )
    { t = ae2; ae2 = ae3; ae3 = t; }
  // if (ae4 > ae5) 
  if ( compar(  ae4,  ae5 ) > 0 )
    { t = ae4; ae4 = ae5; ae5 = t; }

  a[e1] = ae1; a[e2] = ae2; a[e3] = ae3; a[e4] = ae4; a[e5] = ae5;

  // Pointers
  int less  = left;  // The index of the first element of center part
  int great = right; // The index before the first element of right part

  /*
   * Use the second and fourth of the five sorted elements as pivots.
   * These values are inexpensive approximations of the first and
   * second terciles of the array. Note that pivot1 <= pivot2.
   *
   * The pivots are stored in local variables, and the first and
   * the last of the sorted elements are moved to the locations
   * formerly occupied by the pivots. When partitioning is complete,
   * the pivots are swapped back into their final positions, and
   * excluded from subsequent sorting.
   */
  // if a[e1] != a[e2] && a[e2] != a[e3] && a[e3] != a[e4] && a[e4] != a[e5]) {
  if ( compar( ae1, ae2 ) != 0 &&
       compar( ae2, ae3 ) != 0 &&
       compar( ae3, ae4 ) != 0 &&
       compar( ae4, ae5 ) != 0 ) {

    void *pivot1 = ae2; 
    void *pivot2 = ae4; 
    
    /*
     * The first and the last elements to be sorted are moved to the
     * locations formerly occupied by the pivots. When partitioning
     * is complete, the pivots are swapped back into their final
     * positions, and excluded from subsequent sorting.
     */
    a[e2] = a[left];
    a[e4] = a[right];

    /*
     * Skip elements, which are less or greater than pivot values.
     */
    // while (a[++less] < pivot1);
    while ( compar(a[++less], pivot1) < 0 );
    // while (a[--great] > pivot2);
    while ( compar(a[--great], pivot2) > 0 );
  
    /*
     * Partitioning:
     *
     *   left part         center part                  right part
     * ------------------------------------------------------------
     * [ < pivot1  |  pivot1 <= && <= pivot2  |   ?   |  > pivot2 ]
     * ------------------------------------------------------------
     *              ^                          ^     ^
     *              |                          |     |
     *             less                        k   great
     *
     * Invariants:
     *
     *              all in (left, less)   < pivot1
     *    pivot1 <= all in [less, k)     <= pivot2
     *              all in (great, right) > pivot2
     *
     * Pointer k is the first index of ?-part
     */
    int k;
    void *ak;
  outer:
    for (k = less; k <= great; k++) {
      ak = a[k];
      // if (ak < pivot1) {
      if ( compar(  ak,  pivot1 ) < 0 ) {
	a[k] = a[less];
	a[less] = ak;
	less++;
      } else 
	// if (ak > pivot2) {
	if ( compar(  ak,  pivot2 ) > 0 ) {
	  // while (a[great] > pivot2) {
	  while ( compar(  a[great],  pivot2 ) > 0 ) {
	    if (great-- == k) {
	      // break outer;
	      goto outer;
	    }
	  }
	  // if (a[great] < pivot1) {
	  if ( compar(  a[great],  pivot1 ) < 0 ) {
	    a[k] = a[less];
	    a[less++] = a[great];
	  } else { // pivot1 <= a[great] <= pivot2
	    a[k] = a[great];
	  }
	  a[great--] = ak;
	}
    }
  
    // Swap pivots into their final positions
    a[left]  = a[less  - 1]; a[less  - 1] = pivot1;
    a[right] = a[great + 1]; a[great + 1] = pivot2;
    
    // Sort left and right parts recursively, excluding known pivots
    dpqSort(a, left, less - 2, compar);
    dpqSort(a, great + 2, right, compar);
    /* Tests suggest (DdC) to simply sort here the middle segment with:
       dpqSort(a, less, great);
       return;
    */
    
    /*
     * If center part is too large (comprises > 4/7 of the array),
     * swap internal pivot values to ends.
     */
    if (less < e1 && e5 < great) {
      /*
       * Skip elements, which are equal to pivot values.
       */
      // while (a[less] == pivot1) {
      while (compar( a[less], pivot1 ) == 0) {
	++less;
      }
      //  while (a[great] == pivot2) {
      while (compar( a[great], pivot2 ) == 0) {
	--great;
      }
      
      /*
       * Partitioning:
       *
       *   left part         center part                  right part
       * +----------------------------------------------------------+
       * | == pivot1 |  pivot1 < && < pivot2  |    ?    | == pivot2 |
       * +----------------------------------------------------------+
       *              ^                        ^       ^
       *              |                        |       |
       *             less                      k     great
       *
       * Invariants:
       *
       *              all in (*,  less) == pivot1
       *     pivot1 < all in [less,  k)  < pivot2
       *              all in (great, *) == pivot2
       *
       * Pointer k is the first index of ?-part.
       */
      int k; void* ak;
    outer2:
      for (k = less - 1; ++k <= great; ) {
	ak = a[k];
	// if (ak == pivot1) { // Move a[k] to left part
	if ( compar(ak, pivot1) == 0 ) { 
	  a[k] = a[less];
	  a[less] = ak;
	  ++less;
	} else 
	  // if (ak == pivot2) { // Move a[k] to right part
	  if ( compar(ak, pivot2) == 0 ) {
	    // while (a[great] == pivot2) {
	    while (compar(a[great], pivot2) == 0) {
	      if (great-- == k) {
		// break outer;
		goto outer2;
	      }
	    }
	    // if (a[great] == pivot1) { // a[great] < pivot2  
	    if ( compar(a[great], pivot1) == 0 ) { 
	      a[k] = a[less];
	      /*
	       * Even though a[great] equals to pivot1, the
	       * assignment a[less] = pivot1 may be incorrect,
	       * if a[great] and pivot1 are floating-point zeros
	       * of different signs. Therefore in float and
	       * double sorting methods we have to use more
	       * accurate assignment a[less] = a[great].
	       */
	      // a[less] = pivot1;
	      a[less] = a[great]; // ++++++++++++++++++++++++++++++++ !!
	      ++less;
	    } else { // pivot1 < a[great] < pivot2
	      a[k] = a[great];
	    }
	    a[great] = ak;
	    --great;
	  }
      }
    }
    
    // Sort center part recursively
    dpqSort(a, less, great, compar);
    
  } else { // Partitioning with one pivot
    /*
     * Use the third of the five sorted elements as pivot.
     * This value is inexpensive approximation of the median.
     */
    void* pivot = a[e3];
    
    /*
     * Partition degenerates to the traditional 3-way
     * (or "Dutch National Flag") partition:
     *
     *   left part   center part            right part
     * -------------------------------------------------
     * [  < pivot  |  == pivot  |    ?    |  > pivot   ]
     * -------------------------------------------------
     *              ^            ^       ^
     *              |            |       |
     *             less          k     great
     *
     * Invariants:
     *
     *   all in (left, less)   < pivot
     *   all in [less, k)     == pivot
     *   all in (great, right) > pivot
     *
     * Pointer k is the first index of ?-part
     */
    int k;
    void *ak;
    for (k = less; k <= great; k++) {
      ak = a[k];
      // if (ak == pivot) {
      if ( compar(  ak,  pivot ) == 0 ) {
	continue;
      }
      // if (ak < pivot1) {
      if ( compar(  ak,  pivot ) < 0 ) {
	a[k] = a[less];
	a[less] = ak;
	less++;
      } else { // ak > pivot1
	/*
	 * We know that pivot1 == a[e3] == pivot2. Thus, we know
	 * that great will still be >= k when the following loop
	 * terminates, even though we don't test for it explicitly.
	 * In other words, a[e3] acts as a sentinel for great.
	 */
	// while (a[great] > pivot) {
	while ( compar(  a[great],  pivot ) > 0 ) {
	  great--;
	}
	// if (a[great] < pivot) {
	if ( compar(  a[great],  pivot ) < 0 ) {
	  a[k] = a[less];
	  a[less++] = a[great];
	} else { // a[great] == pivot
	  // a[k] = pivot;
	  a[k] = a[great]; // +++++++++++++++++++++++++++++++ !!
	}
	a[great--] = ak;
      }
    }
    /*
     * Sort left and right parts recursively.
     * All elements from center part are equal
     * and, therefore, already sorted.
     */
    dpqSort(a, left, less - 1, compar);
    dpqSort(a, great + 1, right, compar);
  }
} // end dualPivotQuicksort
