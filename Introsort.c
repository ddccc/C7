// File: c:/bsd/rigel/sort/C7/Intrsort.c
// Date: Sat Jan 18 20:13:45 2025

void introsort_r();
void insertion();

  // int a[], int n){
void introsort(void **A, int N, int M, int (*compare)()) {
  // introsort_r ( a, 0, n-1, (int(2*log(double(n)))));
  int L = M-N;
  int dp = 2.5 * floor(log(L));
  introsort_r(A, N, M, dp, compare);
  // _insertion(a, n);
  insertion(A, N, M, compare);
} // end introsort
// */
// /*
int med();

int partition();
void introsort_r(void **A, int first, int last, int depth, int (*compare)()) {
  while (last - first > 16 ) {
    // printf("introsort_r first %i last %i depth %i\n", first, last, depth);
    if (depth <=0 ) {
      // _heapsort(&a[first], first, last-first+1 );
      // printf("HEAPSORT %i last %i depth %i\n", first, last, depth);
      heapc(A, first, last, compare);
      return;
    }
    else {
      depth--;
      int pivot; 
      int middle = 1 + first + (last - first)/2;
      int m = med(A, first, middle, last, compare);
      pivot = partition(A, first, last, m, compare);
      introsort_r(A, pivot, last, depth, compare);
      last = pivot;
    }
  }
} // end introsort_r

int partition ( void **A, int first, int last, int m, int (*compare)() ) {
  // printf("partition first %i last %i m %i\n", first, last, m);
  int i= first; int j=last;  
  void **pi = A[m];
        while (1)  
        {  
	  // while (a[i].smaller(x)) i++;  
	  while ( compare(A[i], pi) < 0 ) i++;
	    // The "j--;" causes the last element of the segment to be miss allocated
	    // in the partition.
	    // Omitting "j--;" causes recursive calls on the same segment with constant 
	    // data.  This triggers the invocation of heapsort on that segment

	  // j--; 
	  // while (x.smaller(a[j])) j=j-1;  
	  while ( compare(pi, A[j]) < 0 ) j--; 
	  if(!(i < j)) return i;  
	  // exchange(i,j);  
	  void **t = A[i]; A[i] = A[j]; A[j] = t;
	  i++;  
        }  
} // end partition


// void insertion ( int a[], int n ) {
void insertion ( void **A, int N, int M, int (*compare)()) {
  // printf("insertion N %i M %i\n", N, M);
  int i;
  //for ( i = 1; i < n; i++ ) {
  for ( i = N+1; i <= M; i++ ) {
    // int j, save = A[i];
    int j; void *save = A[i];
    // for ( j = i; j >= 1 && a[j - 1] > save; j-- )
    for ( j = i; j >= 1+N && compare(A[j-1], save) > 0; j-- )
      // a[j] = a[j - 1];
      A[j] = A[j - 1];
    // a[j] = save;
    A[j] = save;
  }
} // end insertion
