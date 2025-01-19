// File: c:/bsd/rigel/sort/C7/BlockSort.c
// Date: Sun Jan 19 11:45:13 2025

// Likely a port of the version by Edelkamp 2016.
// This version has poor performance.

void blockSortc();
const int BLOCKSIZE = 128;
void blockSort(void **A, int N, int M, int (*compar )) {
  int L = M-N;
  if (L <= 0) return;
  int dp = 2.5 * floor(log(L));
  int indexL[BLOCKSIZE], indexR[BLOCKSIZE];
  blockSortc(A, N, M, indexL, indexR, dp, compar);
}

/* defined elsewhere
void iswap3(int p, int q, void **A) {
  void *t = A[p];
  A[p] = A[q];
  A[q] = t;
} // end of iswap3
*/

void blockSortc(void **A, int N, int M, int indexL[], int indexR[],
	   int depthLimit, int (*compareXY)()) {
  int L;
 again:
  // printf("blockSortc N %i M %i\n", N, M);
  L = M - N;
  // if ( L <= 0 ) return;
  if ( L <= 10 ) { 
    insertionsort(A, N, M, compareXY);
    return;
  }

  if ( depthLimit <= 0 ) {
    heapc(A, N, M, compareXY);
    return;
  }
  depthLimit--;

    // 10 < L
    // grab median of 3 or 9 to get a good pivot
    int pm = N + L/2; // (N+M)/2;
    int pl = N;
    int pn = M;
    if (L > 40) { // do median of 9
      int d = L/8;
      pl = med(A, pl, pl + d, pl + 2 * d, compareXY);
      pm = med(A, pm - d, pm, pm + d, compareXY);
      pn = med(A, pn - 2 * d, pn - d, pn, compareXY);
    }
    pm = med(A, pl, pm, pn, compareXY); // pm is index to 'best' pivot ...

    // iter last = end - 1 ;
    int last = M, begin = N;
    // std::iter_swap (pivot_position, last);
    iswap3(pm, last, A);
    // const typename std::iterator_traits <iter >::value_type & pivot = *last;
    void *pivot = A[last];
    int pivot_position = last;
    last --;
    
    int num_left = 0;
    int num_right = 0;
    int start_left = 0;
    int start_right = 0;
    int num;
    int j; // for the for-loops 

    // main loop
    while (last - begin + 1 > 2 * BLOCKSIZE)
      { 
       //Compare and store in buffers
          if ( num_left == 0 ) {
              start_left = 0;
              for (j = 0; j < BLOCKSIZE; j++) {
                  indexL[num_left] = j;
                  // num_left += ( !( less( begin[j], pivot ) ) );
		  // num_left += ( !( less( A[begin+j], pivot ) ) );
		  // num_left += ( !( compareXY(A[begin+j], pivot) < 0 ) );
		  num_left += ( compareXY(A[begin+j], pivot) >= 0 );
              }
          }
          if ( num_right == 0 ) {
            start_right = 0;
            for (j = 0; j < BLOCKSIZE; j++) {
                indexR [ num_right ] = j;
                // num_right += ! ( less( pivot , *( last - j ) ) );
                // num_right += ! ( less( pivot , A[last-j] ) );
		// num_right += ! ( compareXY(pivot, A[last-j]) < 0 );
		num_right += ( compareXY(pivot, A[last-j]) >= 0 );
	    }
	  }
	  // rearrange elements
	  // num = std::min ( num_left , num_right );
	  int num = ( num_left <= num_right ? num_left : num_right );
	  for (j = 0; j < num; j++)
	    // std::iter_swap ( begin + indexL[start_left + j], 
	    //	                last - indexR[start_right + j] );
	    iswap3(begin + indexL[start_left + j], 
		  last - indexR[start_right + j], A);
  
        num_left -= num;
        num_right -= num;
        start_left += num;
        start_right += num;
        begin += ( num_left == 0 ) ? BLOCKSIZE : 0;
        last -= ( num_right == 0 ) ? BLOCKSIZE : 0;
  
        } // end main l o o p
  
        // Compare and store in buffers final iteration
        int shiftR = 0, shiftL = 0;
        if ( num_right == 0 && num_left == 0 ) { 
            // for small arrays or in the unlikely
            // case that both buffers are empty
            shiftL = ((last - begin) + 1 ) / 2 ;
            shiftR = (last - begin) + 1 - shiftL;
            start_left = 0; start_right = 0;
            for (j = 0; j < shiftL; j++) {
                indexL[ num_left ] = j;
                // num_left += ( ! less( begin[j], pivot ) );
		num_left += ( compareXY(A[begin+j], pivot) >= 0 );
                indexR [ num_right ] = j;
                // num_right += ! less( pivot, *( last - j ) );
		num_right += ( compareXY(pivot, A[last-j]) >= 0 );
            }
            if ( shiftL < shiftR )
            {
                indexR [ num_right ] = shiftR - 1;
                // num_right += ! less( pivot, *( last - shiftR + 1 ) );
		num_right += ( compareXY(pivot, A[last - shiftR + 1]) >= 0 );
            }
         }
         else if ( num_right != 0 ) {
             shiftL = ( last - begin ) - BLOCKSIZE + 1;
             shiftR = BLOCKSIZE;
              start_left = 0;
              for (j = 0; j < shiftL; j++) {
                  indexL [ num_left ] = j;
                  // num_left += ( ! less( begin[ j ] , pivot ) );
		  num_left += ( compareXY(A[begin+j], pivot) >= 0 );
              }
         }
         else {
             shiftL = BLOCKSIZE;
             shiftR = ( last - begin) - BLOCKSIZE + 1;
             start_right = 0;
             for (j = 0; j < shiftR; j++) {
                indexR [ num_right ] = j;
                // num_right += ! ( less( pivot , *( last - j ) ) );
		num_right += ( compareXY(pivot, A[last - j]) >= 0 );
             }
         }
   
         // rearrange final iteration
         // num = std::min ( num_left , num_right );
	 num = ( num_left <= num_right ? num_left : num_right);
         for (j = 0; j < num; j++)
	   // std::iter_swap ( begin + indexL [ start_left + j ], 
	   //                  last - indexR [ start_right + j ] );
	   iswap3(begin + indexL [ start_left + j ],
		 last - indexR [ start_right + j ], A);
   
         num_left -= num;
         num_right -= num;
         start_left += num;
         start_right += num;
         begin += ( num_left == 0 ) ? shiftL : 0;
         last -= ( num_right == 0 ) ? shiftR : 0;
         // end final iteration
   
	 int pivotIndex;
        // rearrange elements remaining in buffer
        if ( num_left != 0 )
        {
            int lowerI = start_left + num_left - 1;
            int upper = last - begin;
            // search first element to be swapped
            while ( lowerI >= start_left && indexL [ lowerI ] == upper ) {
               upper--; lowerI--;
            }
            while ( lowerI >= start_left )
	      // std::iter_swap ( begin + upper--, begin + indexL [ lowerI--]);
	      iswap3(begin + upper--, begin + indexL [ lowerI--], A);
   
            // std::iter_swap ( pivot_position, begin + upper + 1 ); 
	    iswap3(pivot_position, begin + upper + 1, A);
	    // fetch the pivot
            // return begin + upper + 1;
	    pivotIndex = begin + upper + 1;
        }
        else if ( num_right != 0 ) {
	  int lowerI = start_right + num_right - 1 ;
	  int upper = last - begin;
	  // search first element to be swapped
	  while( lowerI >= start_right && indexR[ lowerI ] == upper ) {
	    upper--; lowerI--;
	  }
   
	  while (lowerI >= start_right )
	    // std::iter_swap(last - upper--, last - indexR[lowerI--]);
	    iswap3(last - upper--, last - indexR[lowerI--], A);
   
	  // std::iter_swap (pivot_position, last - upper ); 
	  iswap3(pivot_position, last - upper, A);
	  // fetch the pivot
	  // return last - upper;
	  pivotIndex = last - upper;
	}
        else { // no remaining elements
	  // std::iter_swap(pivot_position, begin); // fetch the pivot
	  iswap3(pivot_position, begin, A);
	  // return begin;
	  pivotIndex = begin;
        }

	// Recurse on smallest segment first.
	depthLimit++;
	if ( pivotIndex - begin < last - pivotIndex ) {
	  blockSortc(A, N, pivotIndex-1, indexL, indexR, depthLimit, compareXY);
	  N = pivotIndex + 1;
	  goto again;
	}
	blockSortc(A, pivotIndex+1, M, indexL, indexR, depthLimit, compareXY);
	M = pivotIndex-1;
	goto again;

} // end blockSortc
   
