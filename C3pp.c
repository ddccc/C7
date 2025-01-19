// c:/bsd/rigel/sort/C3pp.c
// Date: Fri Dec 20 13:30:56 2024
// (C) OntoOO/ Dennis de Champeaux

#define iswap(p, q, A) { void *t3t = A[p]; A[p] = A[q]; A[q] = t3t; }

// This version combines isort +  ..


// static const int dflgmLimit6 = 40; // 2439
// static const int dflgmLimit6 = 45; // 2446
static const int dflgmLimit6 = 50; // 2433, 2427
// static const int dflgmLimit6 = 55; // 2445
// static const int dflgmLimit6 = 70; // 2493


void c3ppc(); // workhorse

void c3pp(void **A, int lo, int hi, int (*compare)()) { 
  // printf("c3pp %d %d %d\n", lo, hi, hi-lo);
  int L = hi - lo;
  if ( L <= 0 ) return;
  int depthLimit = 2.9 * floor(log(L));
  c3ppc(A, lo, hi, depthLimit, compare);
} // end c3pp

void c3ppc(void **A, int lo, int hi, int depthLimit,
	   int (*compareXY)(const void*, const void*)) {

  // printf("Enter c3ppc lo: %d hi: %d %d\n", lo, hi, depthLimit);
  // printf(" gap %d \n", hi-lo);

  while ( lo < hi ) { 
    // printf("c3ppc lo: %d hi %d  L %i\n", lo, hi, hi-lo);
    int L = hi - lo;
    if ( depthLimit <= 0 ) {
      heapc(A, lo, hi, compareXY);
      return;
    }
    depthLimit--;
    /*
    if ( L <= dflgmLimit6 ) {
      insertionsort(A, lo, hi, compareXY);
      return;
    }
    */
    if ( L <= 200 ) {
      dflgm3(A, lo, hi, depthLimit, compareXY);
      return;
    }

    // register int I = lo, J = hi; // indices
    int probeLng = sqrt(L/7.0); 
    // if ( probeLng < 9 ) probeLng = 9; 
if ( probeLng < 15 ) probeLng = 15; 

    // int probeLng = sqrt(L/3.0); // if ( probeLng < 9 ) probeLng = 9; // --
    // int probeLng = sqrt(L/2.0); // if ( probeLng < 9 ) probeLng = 9; // 00
    // int probeLng = sqrt(L/1.9); // if ( probeLng < 9 ) probeLng = 9; // 
    //  int probeLng = sqrt(L/1.8); // if ( probeLng < 9 ) probeLng = 9; // ++
    // int probeLng = sqrt(L/1.7); // if ( probeLng < 9 ) probeLng = 9; // 00
    // int probeLng = sqrt(L/1.6); // if ( probeLng < 9 ) probeLng = 9; // --
    // int probeLng = sqrt(L/1.5); // if ( probeLng < 9 ) probeLng = 9; // ++
    // int probeLng = sqrt(L/1.4); // if ( probeLng < 9 ) probeLng = 9; // --
    // int probeLng = sqrt(L/1.3); // if ( probeLng < 9 ) probeLng = 9; // --
    // int probeLng = sqrt(L/1.2); // if ( probeLng < 9 ) probeLng = 9; // --
    // int probeLng = dflgmLimit6;
    // int middlex = lo + (dflgmLimit2>>1); // lo + dflgmLimit2/2
    // int middle;
    int k; // for sampling
    int lo1 = lo;
    int hi1 = lo + probeLng - 1;
    int offset = L/probeLng;  

    /*
    // int probeLng = 9;
    int halfSegmentLng = probeLng >> 1; // probeLng/2;
    lo1 = middlex - halfSegmentLng; //  lo + (L>>1) - halfSegmentLng;
    hi1 = lo1 + probeLng - 1;
    */

    // assemble the mini array [lo1, hi1]
    for (k = 0; k < probeLng; k++) // iswap(lo1 + k, lo + k * offset, A);
      { int xx = lo + k, yy = lo + k * offset; iswap(xx, yy, A); }
    // sort this mini array to obtain good pivots
    // quicksort0c(A, lo1, hi1, depthLimit, compareXY);
    // cut2c(A, lo1, hi1, depthLimit, compareXY);
    // insertionsort(A, lo, hi1, compareXY);
    c3ppc(A, lo1, hi1, depthLimit, compareXY);
    int thirdth = probeLng/3;
    int pi1 = lo + thirdth, pi2 = hi1 - thirdth; // pivots
    // if ( A[lo] == A[pi1] || A[pi1] == A[pi2] || A[pi2] == A[hi1] ) {
    if ( compareXY(A[lo], A[pi1]) == 0 ||
	 compareXY(A[pi1], A[pi2]) == 0 ||
	 compareXY(A[pi2], A[hi1]) == 0 ) {
      // give up because cannot find a good pivots
      // dflgm is a dutch flag type of algorithm
      int middlex = lo + probeLng >> 1; // lo + probeLng/2
      dflgm(A, lo, hi, middlex, c3ppc, depthLimit, compareXY);
      return;
    }
    /*
       |-----[-----]-----|------------------------------------|
       lo   pi1   pi2    hi1                                  hi
     */
    int z = hi;
    for ( k = hi1; pi2 < k; k-- ) {
      iswap(k, z, A); z--;
    }
    /*
       |-----[-----]---------------------------------(--------|
       lo   pi1   pi2                                z        hi
     */
    void *p1 = A[pi1], *p2 = A[pi2]; // pivots
    while (pi2 < z) {
      k = pi2+1;
      void *ak = A[k];
      // if ( ak < p1 ) {
      if ( compareXY(ak, p1) < 0) {
	A[k] = A[pi1]; A[pi1] = ak; pi1++; pi2++; continue;}
      // if ( ak <= p2 ) { 
      if ( compareXY(ak, p2) <= 0 ) { pi2++; continue; }
      // p2 < ak
      if ( k == z ) 
    /*
       |-----[--------------------------------------]--------|
       lo   pi1                                    pi2      hi
     */
	break;
    /*              k
       |-----[-----]---------------------------------(--------|
       lo   pi1   pi2                                z        hi
     */
      // k < z & p2 < ak
      void *az = A[z];
      // while ( p2 < az ) { 
      while ( compareXY(p2, az) < 0 ) {
	z--; az = A[z]; }
      if ( z <= pi2 ) break;
      // az <= p2  & k < z & p2 < ak
      // if ( az < p1 ) {
      if ( compareXY(az, p1) < 0) {
	A[z] = ak; z--; 
	A[k] = A[pi1]; 
	A[pi1] = az; pi1++; pi2++;
	continue;
      } 
      // p1 <= az <= p2
      A[k] = az; pi2++; A[z] = ak; z--;
    } // end while
      
    /*
       |-----[--------------------------------------]--------|
       lo   pi1                                    pi2      hi
     */
    // checks
    // /*
    for ( k = lo; k < pi1; k++ ) {
      // if ( p1 < A[k] ) { 
      if ( compareXY(p1, A[k] ) < 0 ) {
	printf("L k %i pi1 %i pi2 %i z %i \n",k, pi1, pi2, z ); 
	exit(0); }
    }
    //*/

    // /*
    for ( k = pi1; k <= pi2; k++ ) {
      // if ( A[k] < p1 || p2 < A[k] ) { 
      if ( compareXY(A[k], p1) < 0 || compareXY(p2, A[k]) < 0 ) { 
	printf("M k %i pi1 %i pi2 %i z %i \n",k, pi1, pi2, z ); 
	printf("M lo %i hi %i \n", lo, hi);
	exit(0); }
    }
    // */
    // /* 
    for ( k = pi2+1; k <= hi; k++ ) {
      // if ( A[k] <= p2) {
      if ( compareXY(A[k], p2) < 0 ) {
	printf("R k %i pi1 %i pi2 %i z %i \n",k, pi1, pi2, z ); 
	printf("R lo %i hi %i \n", lo, hi);
	exit(0); }
    }
    // */
    /*
   A   c3pcc(A, lo, pi1-1, depthLimit);
   B   c3pcc(A, pi1, pi2, depthLimit);
   C   c3pcc(A, pi2+1, hi, depthLimit);
    */

    // recurse/ iterate
    if ( pi1 - lo <= pi2 - pi1 ) {
      c3ppc(A, lo, pi1-1, depthLimit, compareXY);
	if ( pi2 - pi1 <= hi - pi2 ) {
	  c3ppc(A, pi1, pi2, depthLimit, compareXY);
	  lo = pi2+1;
	} else {
	  c3ppc(A, pi2+1, hi, depthLimit, compareXY);
	  lo = pi1; hi = pi2;
	}
   } else {
      c3ppc(A, pi1, pi2, depthLimit, compareXY);
      if ( hi - pi2 <= pi1 - lo ) {
	c3ppc(A, pi2+1, hi, depthLimit, compareXY);
	hi = pi1-1;
      } else {
	c3ppc(A, lo, pi1-1, depthLimit, compareXY);
	lo = pi2+1;
      }
    }
  } // end while
} // (*  OF cut2; *) the brackets remind that this was once, 1985, Pascal code

#undef iswap
