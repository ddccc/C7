// File: c:/bsd/rigel/sort/C7/Part3.c
// Date: Sun Jan 19 11:23:26 2025

// This is likely a port of the 2013 Kushagra version.

// This iswap version does NOT work here
// #define iswap3(p, q, A) { void *t3t = A[p]; A[p] = A[q]; A[q] = t3t; }

void iswap3(int p, int q, void **A) {
  void *t = A[p];   A[p] = A[q];   A[q] = t;
} // end of iswap3

void partition3();
void part3(void **A, int N, int M, int (*compare)()) {
  partition3(A, N, M, compare);
} // end part3

void partition3(void **A, int left, int right,
		int (*compar ) (const void *, const void * ) ) {
  int L;
Again:
  // printf("partition3 left %i right %i\n", left, right);
    L = right - left;
    // Original version
    // /*
    if ( L <= 12 ) { 
      insertionsort(A, left, right, compar);
      return;
    }
    // */
    /*
    if ( L < 3000 ) { // 1st modification
      cut2fc(A, left, right, 100, compar);
      return;
    }
    */
    // /*  // original version
	// getting the 3 pivots
    int e = L/6;
    int mid = left + L/2;
                               // B[0] = A[left]; 
    iswap3(left+1, left+e, A);  // B[1] = A[left+e]; 
    iswap3(left+2, mid-e, A);   // B[2] = A[mid-e];
    iswap3(left+3, mid, A);     // B[3] = A[mid];
    iswap3(left+4, mid+e, A);   // B[4] = A[mid+e]; 
    iswap3(left+5, right-e, A); // B[5] = A[right-e]; 
    iswap3(left+6, right, A);   // B[6] = A[right];
    // insertionSortB(B, 0, 6);
    insertionsort(A, left, left + 6, compar);
    // A[left] = B[0]; A[left+e] = B[1]; A[mid-e] = B[2];
    // A[mid] = B[3];
    // A[mid+e] = B[4]; A[right-e] = B[5]; A[right] = B[6];
    iswap3(left, left+1, A); // iswap(left, left+e, A);   // -> p
    iswap3(left+1, left+3, A); // iswap(left+1, mid, A);    // -> q
    iswap3(right, left+5, A);// iswap(right, right-e, A); // -> r 
    // */

    /*
    // 2nd modification
    // getting the 3 pivots
    int probeLng = L/1000000;
    if ( probeLng < 20 ) probeLng = 20; // quite short indeed
    int halfSegmentLng = probeLng >> 1; // probeLng/2;
    int N1 = left + (L>>1) - halfSegmentLng;
    int M1 = N1 + probeLng - 1;
    int quartSegmentLng = probeLng >> 2; // probeLng/4;
    int maxlx = N1 + quartSegmentLng;
    int middlex = N1 + halfSegmentLng;
    int minrx = M1 - quartSegmentLng;
    int offset = L/probeLng;  
    int k;
    for (k = 0; k < probeLng; k++) iswap3(N1 + k, left + k * offset, A);
    // sort this mini array to obtain good pivots
    quicksort0(A, N1, M1, compareXY); 

    iswap(left, maxlx, A); // left pivot
    iswap(right, minrx, A); // right pivot
    iswap3(left+1, middlex, A); // middle pivot
    // */

    int a = left + 2; int b = a;
    int c = right - 1; int d = c;
    void *p = A[left]; void *q = A[left + 1]; void *r = A[right];

    // start the partition
    while ( b <= c ) {
        // while ( A[b] < q && b <= c ) {
        while ( compar(A[b], q) < 0 && b <= c ) {
	   // if ( A[b] < p ) {
	   if ( compar(A[b], p) < 0 ) {
              // swap(A[a], A[b]); 
	      iswap3(a, b, A); 
              a++;
           }
           b++;
        }
        // while ( A[c] > q && b <= c ) {
	while ( compar(A[c], q) > 0 && b <= c ) {
	   // if ( A[c] > r ) {
	   if ( compar(A[c], r) > 0 ) {
              // swap(A[c], A[d]);
	      iswap3(c, d, A); 
              d--;
           }
           c--;
        }
        if ( b <= c ) {
           // if ( A[b] > r ) {
	   if ( compar(A[b], r) > 0 ) {
             // if ( A[c] < p ) {
	     if ( compar(A[c], p) < 0 ) {
                // swap(A[b],A[a]); swap(A[a],A[c]);
		iswap3(a, b, A); iswap3(a, c, A);   		    
                a++;
             } else
                // swap(A[b],A[c]);
		iswap3(c, b, A);
             // swap(A[c],A[d]); 
	     iswap3(c, d, A); 
             b++; c--; d--;
          } else {
             // if ( A[c] < p ) {
	     if ( compar(A[c], p) < 0 ) {
                // swap(A[b],A[a]); swap(A[a],A[c]);
		iswap3(a, b, A); iswap3(a, c, A);   
                a++;
             } else
                // swap(A[b],A[c]);
		iswap3(b, c, A);  
             b++; c--;
          }
        }
    } 
    a--; b--; c++; d++;
    // swap(A[left + 1],A[a]); swap(A[a],A[b]);
    iswap3(left+1, a, A); iswap3(a, b, A);
    a--;
    // swap(A[left],A[a]); swap(A[right],A[d]);
    iswap3(left, a, A); iswap3(right, d, A);
    // recursive calls & tail recursion
    if ( b - left <= right - b ) {
       partition3(A, left, a, compar);
       partition3(A, a+1, b, compar);
       if ( b - d <= right - d ) {
       	  partition3(A, b+1, d-1, compar);
	  left = d; 
	  goto Again;
       } else {
	  partition3(A, d, right, compar);
	  left = b+1; right = d-1;
	  goto Again;
       }
    } else {
       partition3(A, b+1, d-1, compar);
       partition3(A, d, right, compar);
       if ( a - left <= b - a ) {
          partition3(A, left, a, compar);
	  left = a+1; right = b;
	  goto Again;
       }	  
       partition3(A, a+1, b, compar);
       right = a;
       goto Again;
    }
} // end partition3

// #undef iswap3
