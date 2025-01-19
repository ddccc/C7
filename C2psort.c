// c:/bsd/rigel/sort/C2psort.c
// Date: Fri Jan 31 13:32:12 2014, 2017 Sun Mar 03 16:14:28 2019, Sat Jul 23 13:05:12 2022
// (C) OntoOO/ Dennis de Champeaux

#define iswap(p, q, A) { void *t3t = A[p]; A[p] = A[q]; A[q] = t3t; }

static const int cut2pLimit =  3000; 

static void cut2pc();
// cut2 is used as a best in class quicksort implementation 
// with a defense against quadratic behavior due to duplicates
// cut2 is a support function to call up the workhorse cut2c
void cut2p(void **A, int lo, int hi, int (*compare)()) { 
  // printf("cut2 %d %d %d\n", lo, hi, hi-lo);
  int L = hi - lo;
  if ( L <= 0 ) return;
  int depthLimit = 1 + 2.9 * floor(log(L));
  if ( L < cut2pLimit ) { 
    // quicksort0(A, lo, hi, compare);
    cut2c(A, lo, hi, depthLimit, compare);
    return;
  }
  cut2pc(A, lo, hi, depthLimit, compare);
} // end cut2

// Cut2c does 2-partitioning with one pivot.
// Cut2c invokes dflgm when trouble is encountered.
void cut2pc(void **A, int lo, int hi, int depthLimit, int (*compareXY)()) {
  int L;
 Start:
  L = hi - lo + 1;
  if ( L <= 1 ) return;
  if ( depthLimit <= 0 ) {
    heapc(A, lo, hi, compareXY);
    return;
  }
  L = hi - lo +1;
  if ( L < cut2pLimit ) { 
    cut2c(A, lo, hi, depthLimit, compareXY);
    return;
  }
  depthLimit--;

  register void *T; // pivot
  register int I = lo, J = hi; // indices
  int middlex = lo + (L>>1); // lo + L/2
  void *middle;
  // const int small = 4200; // 4.08673e+08 clocktime 30403
  // const int small = 3700; // 4.08484e+08 clocktime 30193
  // const int small = 3300; // 4.08397e+08 clocktime 30018
  // const int small = 3200; // 4.08377e+08 clocktime 30003
  // const int small = 3100; // 4.08361e+08 clocktime 30012
  // const int small = 3000; // 4.08343e+08 clocktime 30043
  // const int small = 2800; // 4.08318e+08 clocktime 30147
  /*
  if ( L < small ) { // use 5 elements for sampling
        int sixth = (L + 1) / 6;
        int e1 = lo  + sixth;
        int e5 = hi - sixth;
	int e3 = middlex; // lo + L/2
        int e4 = e3 + sixth;
        int e2 = e3 - sixth;
        // Sort these elements using a 5-element sorting network
        void *ae1 = A[e1], *ae2 = A[e2], *ae3 = A[e3], *ae4 = A[e4], *ae5 = A[e5];
	void *t;
	// if (ae1 > ae2) { t = ae1; ae1 = ae2; ae2 = t; }
	if ( 0 < compareXY(ae1, ae2) ) { t = ae1; ae1 = ae2; ae2 = t; } // 1-2
	if ( 0 < compareXY(ae4, ae5) ) { t = ae4; ae4 = ae5; ae5 = t; } // 4-5
	if ( 0 < compareXY(ae1, ae3) ) { t = ae1; ae1 = ae3; ae3 = t; } // 1-3
	if ( 0 < compareXY(ae2, ae3) ) { t = ae2; ae2 = ae3; ae3 = t; } // 2-3
	if ( 0 < compareXY(ae1, ae4) ) { t = ae1; ae1 = ae4; ae4 = t; } // 1-4
	if ( 0 < compareXY(ae3, ae4) ) { t = ae3; ae3 = ae4; ae4 = t; } // 3-4
	if ( 0 < compareXY(ae2, ae5) ) { t = ae2; ae2 = ae5; ae5 = t; } // 2-5
	if ( 0 < compareXY(ae2, ae3) ) { t = ae2; ae2 = ae3; ae3 = t; } // 2-3
	if ( 0 < compareXY(ae4, ae5) ) { t = ae4; ae4 = ae5; ae5 = t; } // 4-5
	// ... and reassign
	A[e1] = ae1; A[e2] = ae2; A[e3] = ae3; A[e4] = ae4; A[e5] = ae5;

	if ( compareXY(ae5, ae3) <= 0) {
	  // give up because cannot find a good pivot
	  // dflgm is a dutch flag type of algorithm
	  void cut2c();
	  dflgm(A, lo, hi, e3, cut2c, depthLimit, compareXY);
	  return;
	}

	iswap(e5, hi, A); // right corner OK now
	// put pivot in the left corner
	iswap(lo, e3, A);
	T = A[lo];
  } else { // small <= L
  */
    int k, lo1, hi1; // for sampling
    // int middlex = lo + (L>>1); // lo + L/2

    int probeLng = sqrt(L/6);
    int halfSegmentLng = probeLng >> 1; // probeLng/2;
    int quartSegmentLng = probeLng >> 2; // probeLng/4;
    lo1 = middlex - halfSegmentLng; //  lo + (L>>1) - halfSegmentLng;
    hi1 = lo1 + probeLng - 1;
    int offset = L/probeLng;  

    // assemble the mini array [lo1, hi1]
    for (k = 0; k < probeLng; k++) // iswap(lo1 + k, lo + k * offset, A);
      { int xx = lo1 + k, yy = lo + k * offset; iswap(xx, yy, A); }
    // sort this mini array to obtain good pivots
    // quicksort0c(A, lo1, hi1, depthLimit, compareXY);
    cut2c(A, lo1, hi1, depthLimit, compareXY);
    T = middle = A[middlex];
    if ( compareXY(A[hi1], middle) <= 0 ) {
      // give up because cannot find a good pivot
      // dflgm is a dutch flag type of algorithm
      void cut2c();
      dflgm(A, lo, hi, middlex, cut2c, depthLimit, compareXY);
      return;
    }
    for ( k = lo1; k <= middlex; k++ ) {
      iswap(k, I, A); I++;
    }
    I--;
    for ( k = hi1; middlex < k; k--) {
      iswap(k, J, A); J--;
    }
    J++;
    // }
  
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
	if ( (I - lo) < (hi - J) ) { // smallest one first
	  // cut2c(A, lo, J, depthLimit, compareXY);
	  // lo = I; 
	  addTaskSynchronized(ll, newTask(A, I, hi, depthLimit, compareXY));
	  hi = J;
	  goto Start;
	}
	// cut2c(A, I, hi, depthLimit, compareXY);
	// hi = J;
	addTaskSynchronized(ll, newTask(A, lo, J, depthLimit, compareXY));
	lo = I;
	goto Start;

} // (*  OF cut2; *) the brackets remind that this was once, 1985, Pascal code

#undef iswap
