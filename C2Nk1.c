// File c:/bsd/rigel/sort/C2Nk1.c
// Date: Mon Aug 08 18:25:11 2022
// (C) OntoOO/ Dennis de Champeaux
//  Inspired by the cut2k2 function in Dennis's C2k2.c file
//  Programmed by Nigel Horspool, 2022 Aug 6
//  Adjusted by DdC

// swap two elements of an array given pointers to the two elements, p and q
#define PSWAP(p, q) { void *t2 = *(void**)p; *(void**)p = *(void**)q; *(void**)q = t2; }

static const int dflgmLimitK3 = 200;

static void cut2Nk1np(void **A, void **hip, int depthLimit,
		 int (*compareXY)(const void*, const void*));

// cut2k3 is used as a best in class quicksort implementation
// with a defense against quadratic behavior due to duplicates
// cut2k3 is a support function to call up the workhorse cut2k3np
void cut2Nk1(void **A, int lo, int hi, int (*compare)()) {
    int size = hi - lo + 1;
    int depthLimit = 2.9 * floor(log(size));
    cut2Nk1np(A+lo, A+hi, depthLimit, compare);
} // end cut2Nk1


// cut4nc is used only if dflgm is invoked
static void cut2Nk1nc(void **A, int N, int M, int depthLimit, int (*compareXY)()) {
    cut2Nk1np(A+N, A+M, depthLimit, compareXY);
} // end cut2Nk1nc


static void cut2Nk1np(void **A, void **hip, int depthLimit,
		 int (*compareXY)(const void*, const void*)) {

    while ( 1 ) {
	int size = hip-A+1;
        if ( depthLimit <= 0 ) {
            heapc(A, 0, size-1, compareXY);
            return;
        }
        depthLimit--;

	if ( size < dflgmLimitK3 ) {
	  dflgm3(A, 0, hip-A, depthLimit, compareXY);
	  return;
	}

        void **midp = A + (size>>1); // midp points to array midpoint

        int numberSamples = (int)sqrt(size/5.0);
        if (numberSamples < 5) numberSamples = 5;
        int offset = size / numberSamples;
        void **SampleStart = midp - numberSamples/2;
        void **SampleEnd = SampleStart + numberSamples;

        // assemble the mini array [SampleStart, SampleEnd)
        void **yyp = A;
        void **xxp;
        for( xxp = SampleStart; xxp < SampleEnd; xxp++) {
            PSWAP(xxp, yyp);
            yyp += offset;
        }
        // sort this mini array to obtain good pivots
        cut2Nk1np(SampleStart, SampleEnd-1, depthLimit, compareXY);

        void *pivot = *midp;
        if ( compareXY(*SampleStart, pivot) == 0 ||
                compareXY(*(SampleEnd-1), pivot) == 0) {
            // pivot has duplicates -> there are likely to be many
            // dflgm is a dutch flag type of algorithm
            dflgm(A, 0, hip-A, midp-A, cut2Nk1nc, depthLimit, compareXY);
            return;
        }

        void **ip = A;
        void **jp = hip;
        void **kp;

        // Initialize the LEFT partition
        for ( kp = SampleStart; kp < midp; kp++ ) {
            PSWAP(kp, ip);  ip++;
        }

        // Initialize the RIGHT partition and the PIVOT element
        for ( kp = SampleEnd-1; kp >= midp; kp--) {
            PSWAP(kp, jp);
            jp--;
        }

        // The partitioning at this point
        //
        //     LEFT           XX          PIVOT   RIGHT
        //    __________________________________________
        //   |       |                   |     |        |
        //   | <= p  |        ???        |  p  |  >= p  |
        //   |_______|___________________|_____|________|
        //   A        ip               jp              hip
        //

        // expand LEFT region until we hit an element >= pivot
        while ( compareXY(pivot, *ip) < 0 )
            ip++; // stop with *ip >= pivot

        if (ip <= jp) {
            // the unexplored region is not empty
            kp = ip;

            //     LEFT     R2          XX        PIVOT   RIGHT
            //    ______________________________________________
            //   |       |        |              |     |        |
            //   | <= p  |  >= p  |      ???     |  p  |  >= p  |
            //   |_______|________|______________|_____|________|
            //   A        ip     kp            jp              hip
            //

            // ip points to first element in R2 region
            // kp points to last element in R2 region
            // jp points to last element in XX region

            do {
                // this next loop cannot scan past the
                // position labelled PIVOT in the diagram
                while ( compareXY(*++kp, pivot) > 0 )
                    ; // stop with *kp <= pivot
                PSWAP(ip, kp);
                ip++;
            } while( kp < jp );
        }

        // Tail iteration
        if ( (ip - A) < (hip - ip) ) { // smallest one first
            cut2Nk1np(A, ip-1, depthLimit, compareXY);
            A = ip;
        } else {
            cut2Nk1np(ip, hip, depthLimit, compareXY);
            hip = ip-1;
        }
    } // end while
    // process small subarray
    // dflgm3(A, 0, hip-A, depthLimit, compareXY);

} // end of cut2k3;

#undef PSWAP

