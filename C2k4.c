// File c:/bsd/rigel/sort/C2k4.c
// Date: Wed Aug 10 19:36:27 2022
// (C) OntoOO/ Dennis de Champeaux
//  Inspired by in Dennis's C2k2.c file
//  and NH's C2Nk1.c programmed by Nigel Horspool, 2022 Aug 6
//  Adjusted by DdC

// swap two elements of an array given pointers to the two elements, p and q
#define PSWAP(p, q) { void *t2 = *(void**)p; *(void**)p = *(void**)q; *(void**)q = t2; }
#define dflgmLimit 250
// static const int dflgmLimitK3 = 200;

static void cut2k4np(void **A, void **hip, int depthLimit,
		 int (*compareXY)(const void*, const void*));

// cut2k4 is used as a best in class (?) quicksort implementation
// with a defense against quadratic behavior due to duplicates
// cut2k3 is a support function to call up the workhorse cut2k3np
void cut2k4(void **A, int lo, int hi, int (*compare)()) {
    int size = hi - lo + 1;
    int depthLimit = 2.9 * floor(log(size));
    cut2k4np(A+lo, A+hi, depthLimit, compare);
} // end cut2Nk4


// cut2k44nc is used only if dflgm is invoked
static void cut2k4nc(void **A, int N, int M, int depthLimit, int (*compareXY)()) {
    cut2k4np(A+N, A+M, depthLimit, compareXY);
} // end cut2k4nc

int cnt2 = 0;
static void cut2k4np(void **A, void **hip, int depthLimit,
		 int (*compareXY)(const void*, const void*)) {

    while ( A < hip ) {
      cnt2++;
      // printf("while cut2k4np A: %p hip: %p\n", &A, &hip);
	int size = hip-A;
        if ( depthLimit <= 0 ) {
            heapc(A, 0, size-1, compareXY);
            return;
        }
        depthLimit--;

	if ( size < dflgmLimit ) {
	  dflgm3(A, 0, hip-A, depthLimit, compareXY);
	  return;
	}

        void **midp = A + (size>>1); // midp points to array midpoint
        int numberSamples = (int)sqrt(size/7.0);
        if (numberSamples < 9) numberSamples = 9;
	int halfSegmentLng = numberSamples >> 1; // numberSamples/2;
        void **SampleStart = midp - halfSegmentLng;
        void **SampleEnd = SampleStart + numberSamples -2;
        int offset = size / numberSamples;

        // assemble the mini array [SampleStart, SampleEnd)
        void **yyp = A;
        void **xxp;
        for( xxp = SampleStart; xxp <= SampleEnd; xxp++) {
	  if ( xxp == SampleEnd )
            PSWAP(xxp, yyp);
            yyp += offset;
        }
        // sort this mini array to obtain good pivots
        cut2k4np(SampleStart, SampleEnd, depthLimit, compareXY);

        void *pivot = *midp;
        if ( compareXY(*SampleStart, pivot) == 0 ||
	     compareXY(*(SampleEnd), pivot) == 0 ) {
            // pivot has duplicates -> there are likely to be many
            // dflgm is a dutch flag type of algorithm
            dflgm(A, 0, hip-A, midp-A, cut2k4nc, depthLimit, compareXY);
            return;
        }

        void **ip = A;
        void **jp = hip;
        void **kp;

        // Initialize the LEFT partition
        for ( kp = SampleStart; kp <= midp; kp++ ) {
            PSWAP(kp, ip);  ip++;
        }
	ip--;

        // Initialize the RIGHT partition and the PIVOT element
        for ( kp = SampleEnd-1; midp < kp; kp--) {
            PSWAP(kp, jp);
            jp--;
        }
	jp++;

	// printf("Y A: %d ip %i jp %i hip: %d %d \n", A, ip, jp, hip, (hip-A));

	void **k = midp; void **k2 = k+1;
	PSWAP(ip, k); ip--;
	PSWAP(jp, k2); jp++;
	PSWAP(jp, ip);
	void **j2 = ip; ip--; void **i2 = jp; jp++;
	// printf("Z A: %d ip %i j2 %i i2 %i jp %i hip: %d %d \n", 
	//       A, ip, j2, i2, jp, hip, (hip-A));
	// exit(0);

    /*      L         R                        L      R     
      |------------]------]------|---------[-----[---------|
      A           ip      j2     kk2       i2   jp        hip
    */
    Left:

	// while ( compareXY(T, A[++J2]) < 0 ); // stop with A[J2] <= T
	while ( compareXY(pivot, *++j2) < 0 ) ; // stop with j2 <= pivot
	ip++;
	PSWAP(ip, j2);
	if ( j2 < k ) goto Left;


	
    Right:

	// while ( compareXY(A[--I2], T) <= 0); // stop with T <= A[I2]
	while ( compareXY(*--i2, pivot) < 0 );  // stop with pivot <= i2
	jp--;
	PSWAP(jp, i2);
	if ( k2 < i2 ) goto Right;


   /*      L            R                L           R     
      |------------]------------|--------------[---------|
      A           ip            kk2           jp        hip
    */
	void **z;
	if (k-ip <= jp-k2) {
	  for ( z = k; ip < z; z--) { jp--; PSWAP(z, jp) }
	  ip = jp; jp--;
	} else {
	  for ( z = k2; z < jp; z++) { ip++; PSWAP(ip, z) }
	  jp = ip; ip++;
	}

	// Done
	/*
	{ void **z;
	  for ( z = A; z <= jp; z++ ) 
	    if ( compareXY(pivot, *z) < 0 ) {
	      printf("Error Done L A %i ip %i j2 %i i2 %i jp %i hip %i   %i\n",
		     A,ip,j2,i2,jp,hip,z); exit(0); }
	  for ( z = ip; z <= hip; z++ ) 
	    if ( compareXY(*z, pivot) < 0 ) { // equal is ok 
	      printf("Error Done R A %i ip %i j2 %i i2 %i jp %i hip %i   %i\n",
		     A,ip,j2,i2,jp,hip,z); exit(0); }
	}
	*/

        // Tail iteration
        if ( (ip - A) < (hip - ip) ) { // smallest one first
            cut2k4np(A, jp, depthLimit, compareXY);
            A = ip;
        } else {
            cut2k4np(ip, hip, depthLimit, compareXY);
            hip = ip-1;
        }
    } // end while

} // end of cut2k4;

#undef PSWAP
#undef dflgmLimit

