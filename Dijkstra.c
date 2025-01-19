// File: c:/bsd/rigel/sort/C7/Dijkstra.c
// Date: Sun Jan 19 08:50:46 2025


void dflgmTest2();
void dflgmd2();
void dflgmTest0(void **A, int N, int M, int (*compareXY)()) { 
  dflgmTest2(A, N, M, 1000, compareXY);
} // end dflgmTest0

void dflgmTest2(void **A, int N, int M, int depthLimit, int (*compareXY)()) {
  // printf("Entering dflgmTest2 ... \n");
  if ( depthLimit <= 0 ) { // avoid quadradic explosion
    heapc(A, N, M, compareXY);
    return;
  }
  int L = M-N+1;

  if ( L <= 10 ) { 
    insertionsort(A, N, M, compareXY);
    return;
  }
  depthLimit--;
    int p0 = N + (L>>1); // N + L/2;
    if ( 7 < L ) {
      int pn = N;
      int pm = M;
      if ( 49 < L ) {
	int d = (L-2)>>3; // L/8;
	pn = med(A, pn, pn + d, pn + 2 * d, compareXY);
	p0 = med(A, p0 - d, p0, p0 + d, compareXY);
	pm = med(A, pm - 2 * d, pm - d, pm, compareXY);
      }
      p0 = med(A, pn, p0, pm, compareXY);
    }

    // choose to compare against the other
    dflgmd2(A, N, M, p0, dflgmTest2, depthLimit, compareXY);
    // dflgm(A, N, M, p0, dflgmTest2, depthLimit, compareXY);

} // end dflgmTest2


// Dijkstra, but had to add insertionsort in the driver
void dflgmd2(void **A, int N, int M, int pivotx,
           void (*cut)(void**, int, int, int,
                       int (*)(const void*, const void*)),
           int depthLimit, int (*compareXY)(const void*, const void*)) {

  // printf("dflgm2 N %i M %i pivotx %i depthLimit %i\n", N,M,pivotx,depthLimit);

    /*
    Wikipedia (i.e. Dijkstra) version of algorithm.
    The invariant for the while loop maintains this ordering
    for the elements in A:
            A[N .. i-1] are less than pivot
            A[i .. j-1] are equal to pivot
            A[j .. k-1] are unsorted
            A[k .. M]   are greater than pivot
    */

    int i = N;
    int j = N;
    int k = M;
    void *pivot = A[pivotx];

    while(j <= k) {
        void *Aj = A[j];
        int r = compareXY(Aj,pivot);
        if (r < 0) {
            if (i != j) {
                // swap A[i] and A[j]
                A[j] = A[i];  A[i] = Aj;
            }
            i++;  j++;
        } else if (r > 0) {
            // swap A[j] and A[k]
            A[j] = A[k];  A[k] = Aj;
            k--;
        } else {
            j++;
        }
    }

    if ( (i-N) <= (M-k) ) {
        if (N < i) (*cut)(A, N, i, depthLimit, compareXY);
        if (j < M) (*cut)(A, j, M, depthLimit, compareXY);
    } else {
        if (j < M) (*cut)(A, j, M, depthLimit, compareXY);
        if (N < i) (*cut)(A, N, i, depthLimit, compareXY);
    }
} // end dflgmd2

