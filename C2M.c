// c:/bsd/rigel/sort/C7/C2M.c
// Date: Sat Jan 04 16:19:17 2020
// (C) OntoOO/ Dennis de Champeaux

// const int cut2Limit =  600; 

// const int block = 15*1024; // 384   13.3
// const int block = 31*1024; // 384   13.3
// const int block = 63*1024; // 384   13.8
// const int block = 8*1024; // 385   13.4
// const int block = 4*1024; // 386   13.3
// const int block = 2*1024; // 387   13.2
// const int block = 1024; // 387   13.2
// const int block = 1024; // 388   13.2
// const int block = 1024; // 390   13.2 with 5 element selector
// const int block = 600; // 391   13.0 with 5 element selector
// const int block = 400; // 393   13.0 with 5 element selector
// const int block = 300; // 393   13.0 with 5 element selector
// const int block = 300; // 393   12.6 with  600   d5  11    
// const int block = 300; // 393   12.6 with  600   d5  8    
// const int block = 300; // 393   12.6 with  700   d5  8    
// const int block = 400; // 392   12.6 with  700   d5  8    
const int block = 200; // 394   12.6 with  700   d5  8    

void *myMalloc();
void c2mc();
void c2c();
void d5c();

/* This sorter is a hybrid with:
- d5c
    insertionsort
    c2c  mergesort
    simple quicksort
- cut2c type quickort
    with escapes to cut2c is segment initialization fails
*/

void c2m(void **A, int N, int M, int (*compare)()) { 
  // printf("c2m %d %d %d\n", N, M, M-N);
  int L = M - N + 1;
  void **C = myMalloc("c2m", block*sizeof(A));
  if ( L <= block ) {
    c2c(A, C, N, M, compare);
    return;
  }
  int depthLimit = 1 + 2.9 * floor(log(L));
  c2mc(A, C, N, M, depthLimit, compare);
} // end c2m



// c2mc does 2-partitioning with one pivot and uses merge sort on small segments
// c2mc invokes cut2c when trouble is encountered.
void c2mc(void **A, void **C, int N, int M, int depthLimit, int (*compareXY)()) {
  int L;
 Start:
  // printf("c2mc %d %d %d\n", N, M, M-N);
  L = M - N + 1;
  if ( L <= 1 ) return;

  if ( L <= 700 ) {
    d5c(A, C, N, M, depthLimit, compareXY);
    return;
  }
  if ( depthLimit <= 0 ) {
    heapc(A, N, M, compareXY);
    return;
  }
  depthLimit--;

  register void *T; // pivot
  register int I = N, J = M; // indices
  int middlex = N + (L>>1); // N + L/2
  void *middle;

  const int small = 3200;

  if ( L < small ) { // use 5 elements for sampling
        int sixth = (L + 1) / 6;
        int e1 = N  + sixth;
        int e5 = M - sixth;
	int e3 = middlex; // N + L/2
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
	  // cut2c(A, N, M, depthLimit, compareXY);
	  // dflgm is a dutch flag type of algorithm
	  void cut2c();
	  dflgm(A, N, M, e3, cut2c, depthLimit, compareXY);
	  return;
	}

	iswap(e5, M, A); // right corner OK now
	// put pivot in the left corner
	iswap(N, e3, A);
	T = A[N];
  } else { // small <= L

    int k, N1, M1; // for sampling
    // int middlex = N + (L>>1); // N + L/2

    int probeLng = sqrt(L/9);
    int halfSegmentLng = probeLng >> 1; // probeLng/2;
    int quartSegmentLng = probeLng >> 2; // probeLng/4;
    N1 = middlex - halfSegmentLng; //  N + (L>>1) - halfSegmentLng;
    M1 = N1 + probeLng - 1;
    int offset = L/probeLng;  

    // assemble the mini array [N1, M1]
    for (k = 0; k < probeLng; k++) // iswap(N1 + k, N + k * offset, A);
      { int xx = N1 + k, yy = N + k * offset; iswap(xx, yy, A); }
    // sort this mini array to obtain good pivots
    // cut2c(A, N1, M1, depthLimit, compareXY);
    // c2c(A, C, N1, M1, compareXY);
    c2mc(A, C, N1, M1, depthLimit, compareXY);
    T = middle = A[middlex];

    if ( compareXY(A[M1], middle) <= 0 ) {
      // give up because cannot find a good pivot
      // cut2c(A, N, M, depthLimit, compareXY);
      // void **C2 = myMalloc("c2mc2", L*sizeof(A));
      // c2c(A, C2, N, M, compareXY);
      // void cut2c();
      dflgm(A, N, M, middlex, cut2c, depthLimit, compareXY);
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
  }

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
	if ( (I - N) < (M - J) ) { // smallest one first
	  c2mc(A, C, N, J, depthLimit, compareXY);
	  N = I; 
	  goto Start;
	}
	c2mc(A, C, I, M, depthLimit, compareXY);
	M = J;
	goto Start;

} // (*  OF c2m; *) the brackets remind that this was once, 1985, Pascal code
