// c:/bsd/rigel/sort/C7/Cut7p.c
// Date: Tue Oct 15 16:48:12 2019
// (C) OntoOO/ Dennis de Champeaux


#define iswap(p, q, A) { void *t3t = A[p]; A[p] = A[q]; A[q] = t3t; }

static void cut7pc(void **, int, int, int, int (*)(const void*, const void*));

void cut7p(void **A, int N, int M, int (*compare)()) {
  // printf("cut7 N %i M %i \n", N, M);
  int L = M - N;
  if ( L <= 0 ) return;
  int depthLimit = 1 + 2.9 * floor(log(L));
  cut7pc(A, N, M, depthLimit, compare);
} // end cut7

// int callCnt;
void cut7pc(void **A, int N, int M, int depthLimit,
		int (*compare)(const void*, const void*)) {
  int L;
  int i, j; // running indices
  void *ai, *aj;

 Start:
  // printf("cut7pc N %i M %i L %i\n", N, M, M-N);
  L = M - N + 1;
  if ( L <= 1 ) return; 


  if ( L < 600 ) {
    d4c(A, N, M, depthLimit, compare);
    return;
  }
  if ( depthLimit <= 0 ) {
    heapc(A, N, M, compare);
    return;
  }
  depthLimit--;

  int middlex = N + (L>>1); // N + L/2;

  const int use1pivot = 500;
  const int use3sample = 700;
  const int use5sample = 900;


  int maxlx, minrx;
  register void *maxl, *middle, *minr;   
  register int lw, up; // indices
  i = lw = N; j = up = M;
  int N1, M1, k;

  if ( L <= use5sample ) { // using 5 elements
    int e1, e2, e3, e4, e5;
    e1 = maxlx = N; e5 = minrx = M;
    e3 = middlex;
    int quartSegmentLng = L >> 2; // L/4
    e2 = e3 - quartSegmentLng;
    e4 = e3 + quartSegmentLng;

    void *ae1 = A[e1], *ae2 = A[e2], *ae3 = A[e3], *ae4 = A[e4], *ae5 = A[e5];
    void *t;
    // if (ae1 > ae2) { t = ae1; ae1 = ae2; ae2 = t; }
    if ( 0 < compare(ae1, ae2) ) { t = ae1; ae1 = ae2; ae2 = t; } // 1-2
    if ( 0 < compare(ae4, ae5) ) { t = ae4; ae4 = ae5; ae5 = t; } // 4-5
    if ( 0 < compare(ae1, ae3) ) { t = ae1; ae1 = ae3; ae3 = t; } // 1-3
    if ( 0 < compare(ae2, ae3) ) { t = ae2; ae2 = ae3; ae3 = t; } // 2-3
    if ( 0 < compare(ae1, ae4) ) { t = ae1; ae1 = ae4; ae4 = t; } // 1-4
    if ( 0 < compare(ae3, ae4) ) { t = ae3; ae3 = ae4; ae4 = t; } // 3-4
    if ( 0 < compare(ae2, ae5) ) { t = ae2; ae2 = ae5; ae5 = t; } // 2-5
    if ( 0 < compare(ae2, ae3) ) { t = ae2; ae2 = ae3; ae3 = t; } // 2-3
    if ( 0 < compare(ae4, ae5) ) { t = ae4; ae4 = ae5; ae5 = t; } // 4-5
    // ... and reassign
    A[e1] = ae1; A[e2] = ae2; A[e3] = ae3; A[e4] = ae4; A[e5] = ae5;
    // pivots
    maxl = A[maxlx]; middle = A[middlex]; minr = A[minrx];
    lw = N+1; iswap(lw, middlex, A);
    // up = M-1; // iswap(up, mrx, A);
    up = M; // iswap(up, mrx, A);
  } else { // using many elements
    int probeLng = sqrt(L/5.8);
    probeLng = (probeLng <= 500 ? probeLng : 500);
    int halfSegmentLng = probeLng >> 1; // probeLng/2;
    int quartSegmentLng = probeLng >> 2; // probeLng/4;
    N1 = middlex - halfSegmentLng; //  N + (L>>1) - halfSegmentLng;
    M1 = N1 + probeLng - 1;
    maxlx = N1 + quartSegmentLng;
    // int middlex = N1 + halfSegmentLng;
    minrx = M1 - quartSegmentLng;
    // mrx = middlex + (quartSegmentLng>>1);
    int offset = L/probeLng;  
    // assemble the mini array [N1, M1]
    for (k = 0; k < probeLng; k++) // iswap(N1 + k, N + k * offset, A);
      { int xx = N1 + k, yy = N + k * offset; iswap(xx, yy, A); }
    // sort this mini array to obtain good pivots
    // protect against constant arrays
    cut7pc(A, N1, M1, depthLimit, compare);
    // pivots
    maxl = A[maxlx]; middle = A[middlex]; minr = A[minrx];
  }

  // check that segments can be properly initialized
  if ( compare(maxl, middle) == 0 || 
       compare(middle, minr) == 0 ) {
    // no good pivots available, thus escape
    dflgm(A, N, M, middlex, cut7pc, depthLimit, compare);
    return;
  }
  if ( use5sample < L ) {
    // Swap these segments to the corners
    for ( k = N1; k <= maxlx; k++ ) {
      iswap(k, i, A); i++;
    }
    i--;
    lw = i+1;
    for ( k = maxlx+1; k <= middlex; k++ ) {
      iswap(k, lw, A); lw++;
    }
    lw--;
    for ( k = M1; minrx <= k; k--) {
      iswap(k, j, A); j--;
    }
    j++;
    up = j-1;
    for ( k = minrx-1; middlex < k; k--) {
      iswap(k, up, A); up--;
    }
    up++;
  } 

    void *alw, *aup; // values  

    // Here the general layout
    /*  L     ML                        MR     R
     |-----]------]------------------[------[-----|
     N     i     lw                  up     j     M

     There are invariants to be maintained (which are >essential< 
     for machine assisted correctness proofs):
     maxl < middle < minr
       N <= x <= i --> A[x] <= maxl
       i < x <= lw --> maxl < A[x] <= middle
       up <= x < j --> middle < A[x] < minr
       j <= x <= M --> minr <= A[x]
   */
    // Ready to roll ... 

    while ( ++lw < up ) {
      alw = A[lw];
      while ( compare(alw, middle) <= 0 ) {
	if ( compare(alw, maxl) <= 0 ) {
	  A[lw] = A[++i]; A[i] = alw;
	}
	if ( up <= ++lw ) { lw = up-1; goto GapClosed; }
	alw = A[lw];
      }
      // alw -> right

      if ( --up <= lw ) { goto InsertALW; }
      aup = A[up];
      while ( compare(middle, aup) < 0 ) {
    	if ( compare(minr, aup) <= 0 ) {
	  A[up] = A[--j]; A[j] = aup;
	}
	if ( --up <= lw )  { goto InsertALW; }
	aup = A[up];
      }
      // aup -> left

      if ( compare(aup, maxl) <= 0 ) {
	A[lw] = A[++i]; A[i] = aup;
      } else A[lw] = aup; 
      if ( compare(minr, alw) <= 0 ) {
	A[up] = A[--j]; A[j] = alw;
      } else A[up] = alw;
     }

    // fall through
    lw--;
    goto GapClosed;

 InsertALW:
    if ( compare(minr, alw) <= 0 ) {
      A[up] = A[--j]; A[j] = alw;
    } else A[up] = alw;
    lw--;

 GapClosed: ;

  // +++++++++++++++ gap closed+++++++++++++++++++
  /*      L     ML                     MR     R
       |-----]--------------][-------------[-----|
       N     i             lwup            j     M
  */
  // int k; // for testing

  /*
    for ( k = N; k <= i; k++ ) if ( compare(maxl, A[k]) < 0 ) {
	printf("Err0 L k: %i\n",k);
	exit(0);
      }
    for ( k = i+1; k <= lw; k++ ) 
      if ( compare(A[k], maxl) <= 0 || compare(middle, A[k]) < 0) {
	printf("Err0 ML k: %i\n",k);
	printf("N %i i %i lw %i up %i j %i M %i\n",N,i,lw,up,j,M);
	// printf("---- callCnt %i\n", callCnt);
	exit(0);
      }
    for ( k = up; k < j; k++ ) 
      if ( compare(A[k], middle) <= 0 || compare(minr, A[k]) <= 0) {
	printf("Err0 MR k: %i\n",k);
	printf("Err0 N %i i %i lw %i up %i j %i M %i\n",N,i,lw,up,j,M);
	exit(0);
      }
    for ( k = j; k <= M; k++ ) if ( compare(A[k], minr) < 0 ) {
	printf("Err0 R k: %i\n",k);
	exit(0);
      }

    // printf("Rcall N %i i %i lw %i up %i j %i M %i\n",N,i,lw,up,j,M);
    // */

  /*      L     ML                     MR     R
       |-----]--------------][-------------[-----|
       N     i             lwup            j     M
  */
	if ( lw-N < M-up ) {
	  if ( j-up < M-j ) {
	    addTaskSynchronized(ll, newTask(A, j, M, depthLimit, compare));
	    addTaskSynchronized(ll, newTask(A, up, j-1, depthLimit, compare));
	  } else {
	    addTaskSynchronized(ll, newTask(A, up, j-1, depthLimit, compare));
	    addTaskSynchronized(ll, newTask(A, j, M, depthLimit, compare));
	  }
	  addTaskSynchronized(ll, newTask(A, i+1, lw, depthLimit, compare));
	  M = i;
	  goto Start;
	}
	if ( i-N < lw-i ) {
	  addTaskSynchronized(ll, newTask(A, N, i, depthLimit, compare));
	  addTaskSynchronized(ll, newTask(A, i+1, lw, depthLimit, compare));
	} else {
	  addTaskSynchronized(ll, newTask(A, i+1, lw, depthLimit, compare));
	  addTaskSynchronized(ll, newTask(A, N, i, depthLimit, compare));
	}
	addTaskSynchronized(ll, newTask(A, j, M, depthLimit, compare));
	N = up; M = j-1;
	goto Start;

} // end 3-pivot module end cut7p


