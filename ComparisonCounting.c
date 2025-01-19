// File: c:/bsd/rigel/sort/C7/ComparisonCounting.c
// Date: Fri Jan 10 08:42:56 2014 - Wed Aug 17 20:16:55 2022
// (C) OntoOO/ Dennis de Champeaux

/* 
Counting comparisons 
This driver loads 1-pivot versions for referenced items.
Also some 2-pivot version, some ported from Java.
Also many versions that were developed over time but abandoned.
*/

#include <stdio.h>
#include <stddef.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
// #include <pthread.h>
// #include <sys/time.h>

// /* 
// These are included by the includes below
// #include "Isort.c"
#include "Isort.h"
// #include "Hsort.c"
#include "Hsort.h"
// #include "Dsort.c"  // dflgm member
#include "Dsort.h"  // dflgm member
// #include "D3sort.c" // recursive dflgm
#include "D3sort.h" // recursive dflgm
// #include "Qusort.c" // quicksort member
#include "Qusort.h" // quicksort member
// #include "Qusortm.c" // quicksort with merge sort
// #include "Qusort00.c" // 
#include "Dsort2.c"    // Sat Feb 18 19:11:14 2023
#include "D3sort2.c" // Sat Feb 18 19:11:14 2023
// */
#include "D4.c" 
#include "C2sort.c" // cut2
// #include "C2fsort.c"
#include "C2k1.c" // Sun Mar 06 12:45:01 2022
// #include "C2k2.c" // Sun Mar 10 2022
// #include "C2k4.c" // Thu Aug 11 08:00:47 2022
#include "C2LR.c"
#include "C2LR2.h" // Sat Aug 13 13:33:40 2022
//                   #include "C2Nk1.c" // Mon Aug 08 18:33:02 2022 cut2Nk1
// #include "C3sort.c" // tps member

// #include "C2Split.c"
// #include "DdcChen.c"
// #include "C2LRB.c"
// #include "C2Left.c"
// #include "Mybam.c"
// #include "Myqsort.c"

#include "C3pp.c" // Thu Jan 02 21:37:52 2025  2-pivot good for int arrays

#include "C4.h"   // cut4 member
#include "CN4.h"  // ct4n
#include "CD4.h"  // cut4d
#include "C7.h"   // cut7

#include "Bentley.c" 
#include "LQ.c" 
#include "MergeSort.c"
#include "Introsort.c"
#include "Dijkstra.c"
#include "DPQsort.c"
#include "Part3.c"
#include "BlockSort.c" 

// #include "Msn.c"

void *myMalloc(char* location, int size);
#include "Dpq2.c" // defines COMPAREFN which is used inside Timsort.c - crazy indeed
#include "Timsort.c"

// #include "C2sortNH.c"
// #include "Quickselect.c"

// #include "C2M.c" // Mon Nov 15 16:09:17 2021 modified C2sort
// #include "C2k.c" // Sat Feb 26 13:03:37 2022


#define iswap(p, q, A) { void *t3t = A[p]; A[p] = A[q]; A[q] = t3t; }

// void callChensort();
void callBentley();
void callLQ();
// void introsort();
// void dflgmTest0();
// void dflgmTest2();
void quicksort0();
void cut2();
// void cut2f();
// void tps();
// void tps5()

void cut4();
void cut4n();
void cut4d();
void cut7();
void dpq();
void dpq2();
void timsort();
void cc();
void part3();
// void quicksort0k();
void blockSort();
// */
// void ddcchen();
// void mybam();
// void myqsort();
void dflgm0();
void dflgm20();
void c3pp();

void validateC2lr();
/*
// void mergeSort();
// void compare00FourSortAgainstFiveSort();
// void compare00DPQAgainstFiveSort();
// void compare00DPQAgainstFourSort();
// void validateC2();
// void validateC21();
// void validateC2lr();
// void validateC2lrb();
// void validateC2left();
// void validateC2sort();
// void validateC2msort(); // Mon Nov 15 16:23:32 2021 
// void validateC2ksort(); // Sat Feb 26 13:04:22 2022
// void validateC2k1sort(); // Thu Mar 10 20:36:35 2022
// void validateC2k2sort(); // Sun Mar 06 12:46:44 2022
// void validateC2k3sort(); // Sun Mar 06 12:46:44 2022
// void validateCN4sort();
// void validateCD4sort();
// void validateQ0();
// void validateMyqs();
// void validateMybam();
// void validateMergeSort();
*/
/*
void range();
void rangeLQ();
// void rangeMyqsort();
void rangeBentley();
// void rangeMybam();
void rangeQuicksort0();
void rangeDflgm0();
void rangeCut2();
// void rangeCut2lr();
void rangeBlockSort();
void rangeDpq();
// void rangeTps();
void rangeMpq();
void rangeCut4();
void rangeCut4n();
void rangeCut4d();
void rangeCut7();
// void rangeDpq2();
*/
void fillRandom();
void fillRandom2();


// Here global entities used throughout
// int (*compareXY)();
// void **A;  // adding this to the sort algorithms !!

// Example of objects that can be used to populate an array to be sorted:
  // To obtain the int field from X: ((struct intval *) X)->val
  // To obtain the float field from X: ((struct intval *) X)->valf
// +++++++++++++++++++++++++++++++++++++++++++++++++++
  struct intval {
    int val;
    float valf;
  };

long long cnt = 0; // comparison counter
// int icnt = 0; // for using invocation count

// Here an exmple comparison function for these objects:
// **** NOTE, again **** 
//      All the examples below use ::::
//          the intval objects, and 
//          the compareIntVal comparison function
// int compareIntVal (const void *a, const void *b)
int compareXY (const void *a, const void *b)
{
  extern long long cnt;
  cnt++;
  struct intval *pa = (struct intval *) a;
  struct intval *pb = (struct intval *) b;
  return (pa->val - pb->val);
}

// This comparison formula is used for qsort in callLQ and
// for bentley in callBentley 
int compareIntVal2 (const void **a, const void **b)
{ 
  extern long long cnt;
  cnt++;
  struct intval *pa = (struct intval *) *a;
  struct intval *pb = (struct intval *) *b;
  return (pa->val - pb->val);
}


int main (int argc, char *argv[]) {
  printf("Running ComparisonCounting ...\n");
     // 1 pivot
  //               cc("myqs    ", myqs, compareXY, 1); // deleted
  // cc("heapsort", heapc, compareXY, 1);
  // cc("LQ      ", callLQ, compareIntVal2, 0);
  // cc("Myqsort ", myqsort, compareXY, 1);
  // cc("bentley ", callBentley, compareIntVal2, 0);
  // cc("mybam   ", mybam, compareXY, 1);
  // cc("quick0  ", quicksort0, compareXY, 1);
  // cc("quickm  ", quicksortm, compareXY, 1);
  // cc("quickn  ", quicksortmn, compareXY, 1);
  // cc("quick3  ", quicksort3, compareXY, 1);
  // cc("quicksort0k", quicksort0k, compareXY, 1); // deleted
  // cc("dflgm0  ", dflgm0, compareXY, 1);   
  // cc("dflgm20 ", dflgm20, compareXY, 1);   
  // cc("cut2    ", cut2, compareXY, 1);
  // cc("cut2m   ", cut2m, compareXY, 1); // Mon Nov 15 16:26:06 2021 deleted
  // cc("c2lr    ", cut2lr, compareXY, 1); // deleted 
  // cc("cut2lr2 ", cut2lr2, compareXY, 1); 
  // cc("cut2k   ", cut2k, compareXY, 1); // deleted
  // cc("cut2k1  ", cut2k1, compareXY, 1); // deleted
  // cc("cut2k2  ", cut2k2, compareXY, 1); 
  //     cc("cut2k3  ", cut2k3, compareXY, 1); 
  // cc("cut2k4  ", cut2k4, compareXY, 1); // Thu Aug 11 08:05:32 2022
  // cc("cut2Nk1 ", cut2Nk1, compareXY, 1); // Mon Aug 08 18:46:54 2022
  // cc("cut2NH  ", cut2NH, compareXY, 1); // deleted
  // cc("c21     ", c21, compareXY, 1); // parallel? // removed
  // cc("ddcch", ddcchen, compareXY, 1); // removed
  // cc("c2lrb   ", cut2lrb, compareXY, 1); // removed
  // cc("c2left  ", cut2left, compareXY, 1); // removed
  // cc("cut2f", cut2f, compareXY, 1); //removed
  // cc("introsort", introsort, compareXY, 1);
  // cc("BlockSor", blockSort, compareXY, 1);
  // cc("timsort ", timsort, compareXY, 1);
     // 2 pivot
  cc("c3pp    ", c3pp, compareXY, 1);
  cc("dpq     ", dpq, compareXY, 1);
  cc("dpq2    ", dpq2, compareXY, 1);
  // cc("tps     ", tps, compareXY, 1); // removed
  // cc("tps5", tps5, compareXY, 1); // removed
     // 3 pivot
  // cc("mpq     ", part3, compareXY, 1); 
  // cc("cut4    ", cut4, compareXY, 1);
  cc("cut4n   ", cut4n, compareXY, 1);
  //cc("cut4d   ", cut4d, compareXY, 1);
  // cc("cut7    ", cut7, compareXY, 1);
     // Misc
  // cc("dflgm   ", dflgmTest0, compareXY, 1);
  cc("mergeSor", mergeSort, compareXY, 1);  // deleted
  // cc("Chen", callChensort, compareIntVal2, 0); deleted
  // compare00FourSortAgainstFiveSort();
  // compare00DPQAgainstFiveSort();
  // compare00DPQAgainstFourSort();
  // validateC2();
  // validateC21();
  // validateC2lr(); 
  // validateC2lrb();
  // validateC2left();
  // validateC2sort();
  // validateC2msort(); // Mon Nov 15 16:26:30 2021 deleted
  // validateC2ksort(); // 
  // validateC2k1sort(); // deleted
  // validateC2k2sort(); 
  // validateC2k3sort(); // Sun Aug 07 10:51:14 2022 deleted
  // validateCN4sort();
  // validateCD4sort();
  // validateQ0();
  // validateMyqs();
  // validateMybam();
  // validateMergeSort();

  /*
  rangeLQ();
  rangeBentley();
  rangeCut2();
  rangeCut2lr();
  // rangeBlockSort();
  // rangeDpq();
  // rangeTps();
  // rangeMpq();
  rangeCut4();
  rangeCut4n();
  rangeCut4d();
  // */
  // rangeCut7();

  /*
  rangeMyqsort();
  rangeMybam();
  rangeQuicksort0();
  rangeDflgm0();
   */
  return 0;
} // end main


void *myMalloc(char* location, int size) {
  void *p = malloc(size);
  if ( 0 == p ) {
    fprintf(stderr, "malloc fails for: %s\n", location);
    exit(1);
  }
  return p;
} // end myMalloc

// fillarray assigns random values to the int-field of our objects
void fillarray(void **A, int lng, int startv) {
  int range = 32*1024*1024; // restrict the largest number
  // int range = 128; // limited # of different elements
  int i;
  srand(startv);
  struct intval *pi;
  for ( i = 0; i < lng; i++) {
    pi = (struct intval *)A[i];
    pi->val = rand()%range; 
    // pi->val = 0; 
    // pi->val = i;    
    // pi->val = -i; 
  }

  /*
    #define iswap(p, q, A) { void *t3t = A[p]; A[p] = A[q]; A[q] = t3t; }
  // permutation of [0, lng), to avoid duplicates
  srand(startv);
  struct intval *pi;
  int i;
  for ( i = 0; i < lng; i++) {
    pi = (struct intval *)A[i];
    pi->val = i; 
  }
  int j;
  for ( i = lng-1; 0<i ; i--) {
    j = rand()%i;
    iswap(i, j, A);
  }
  #undef iswap
  // */

  /*
  int numberOfBlocks = lng/2001, j, k;
  i = 0;
  for ( j = 0; j < numberOfBlocks; j++ ) 
    for (k = 0; k < 2001; k++) {
      pi = (struct intval *)A[i++];
      pi->val = j;
    }
  // */

  /*
  // adjust [0,100] to get that percentage on noise
  int percentage = 90; 
  int v, v2, r, sign = 0;
    for ( i = 0; i < lng; i++) {
      pi = (struct intval *)A[i];
      if ( 0 == percentage ) pi->val = 0; else {
	v = rand()%range; 
	sign = v%2;
	v2 = (sign) ? v : -v;
	r = v%100;
	pi->val = (r <= percentage) ? v2 : 0;
      }
  }
    // */
} // end of fillarray

long long comparisons; double clocktime; 
// int invocationcnt;

void countcomparisons(int siz, void (*alg1)(), 
		      int (*compar )(), int seed, int bool) {
  // char *label = "test";
  // printf("%s on size: %d \n", label, siz);
  extern long long cnt,  comparisons;
  extern double clocktime;
  struct intval *pi;

  void **A = myMalloc("countcomparisons 1", sizeof(pi) * siz);
  int i;
  for (i = 0; i < siz; i++) {
    pi = myMalloc("countcomparisons 2", sizeof (struct intval));
    A[i] = pi;
  };
  // int reps = 49152;  // 2k
  // int reps = 40000;
  // int reps = 200;
  // int reps = 50;
  // int reps = 40;
  // int reps = 30;
  // int reps = 20;
  // int reps = 10;
  int reps = 5;
  // int reps = 2;
  // int reps = 1;
  for (i = 0; i < reps; i++) fillarray(A, siz, seed); // warm up
  clock_t TFill = clock();
  for (i = 0; i < reps; i++) fillarray(A, siz, seed+i);
  TFill = clock() - TFill;
  clock_t T = clock();
  cnt = 0; // comparison cnt
  // icnt = 0; // invocation cnt 
  for (i = siz; i < siz + reps; i++) { 
    // i = 16777217;
    // fill its content
    fillarray(A, siz, i); // uniform
    // fillRandom(A, siz, i, 0); // constant + random percentage
    // fillRandom2(A, siz, i, 0); // sorted + random percentage
    // sort it
    if ( bool )
      (*alg1)(A, 0, siz-1, compar);
    else
      (*alg1)(A, siz, compar);
  }

  double algTime = (clock() - T - TFill);
  // printf("algTime %f\n", algTime);
  algTime = algTime/reps;
  cnt = cnt/reps;
  // icnt = icnt/reps;
  // printf("is %g time %i\n", cnt, algTime);
  comparisons += cnt; 
  clocktime += algTime;
  // invocationcnt += icnt;
  // free array
  for (i = 0; i < siz; i++) {
    free(A[i]); 
  };
  free(A);

} // end countcomparisons

int siz = 1024*1024*16;
// int siz = 1024*1024*8;
// int siz = 1024*1024*4;
// int siz = 1024*1024*2;
// int siz = 1024*1024;
// int siz = 1024*512;
// int siz = 1024*256;
// int siz = 1024*128;
// int siz = 1024*64; 
// int siz = 1024*40;
// int siz = 1024*32;
// int siz = 1024*16;
// int siz = 1024*8; 
// int siz = 1024*4;
// int siz = 1024*2; 
// int siz = 1024;
// int siz = 800;
// int siz = 128;
// int siz = 3;

void cc(char* label, void (*alg1)(), int (*compar )(), int bool) {
  extern long long comparisons;
  extern double clocktime;
  printf("cc %s ", label);
  // int size = siz;
  comparisons = 0;
  clocktime = 0;
  countcomparisons(siz, alg1, compar, 666, bool);

  printf("size %i comparisons %lld clocktime %f\n",
	 siz, comparisons, clocktime/CLOCKS_PER_SEC);
} // end cc

void bentley();
// Wrapper function to invoke bentley
void callBentley(void **A, int size, int (*compar ) () ) {
  struct intval *pi;
  // printf("callBentley size %i\n", size);
  bentley(A, size, sizeof(pi), compar);
} // end callBentley

// LQ is qsort in the Linux C-library
void callLQ(void **A, int size, int (*compar ) () ) { 
  struct intval *pi;
  _quicksort(A, size, sizeof(pi), compar, NULL);
} // end callLQ


// ************************************************************************
// Infrastructure for testing non-uniform distributions

// fill with random numbers according to percentage, otherwise 0
void fillRandom(void **A, int lng, int startv, int percentage) {
  int range = 32*1024*1024; // restrict the largest number
  if ( percentage < 0 || 100 < percentage ) percentage = 50;
  int i, v, r, v2; 
  srand(startv);
  struct intval *pi;
  for ( i = 0; i < lng; i++) {
    pi = (struct intval *)A[i];
    if ( 0 == percentage ) { pi->val = 0; continue; }
    v = rand()%range;
    v2 = (v < 0) ? -v : v;
    if ( 100 == percentage ) { pi->val = v2; continue; }
    r = v2%100;
    pi->val = (r <= percentage) ? v : 0; 
  }
} // end fillRandom

// fill with random numbers according to percentage, otherwise with i
void fillRandom2(void **A, int lng, int startv, int percentage) {
  int range = 32*1024*1024; // restrict the largest number
  if ( percentage < 0 || 100 < percentage ) percentage = 50;
  int i, v, r, v2; 
  srand(startv);
  struct intval *pi;
  for ( i = 0; i < lng; i++) {
    pi = (struct intval *)A[i];
    if ( 0 == percentage ) { pi->val = i; continue; }
    // if ( 0 == percentage ){  pi->val = -i; continue; }
    v = rand()%range;
    v2 = (v < 0) ? -v : v;
    if ( 100 == percentage ) { pi->val = v2; continue; }
    r = v2%100;
    pi->val = (r <= percentage) ? v : i; 
    // pi->val = (r <= percentage) ? v : -i; 
  }
} // end fillRandom2

void testNonRandom(int p, void **A, int siz, int seedLimit, 
		   void (*alg1)(), void (*alg2)(), 
		   int (*compare1)(), int (*compare2)()  ) {
  // A = A2;
  int z, alg1Time, alg2Time, T, seed, cnt1, cnt2;
  // for (z = 0; z < 3; z++) { // repeat to check stability
  for (z = 0; z < 1; z++) { // repeat to check stability
    alg1Time = 0; alg2Time = 0; cnt1 = 0; cnt2 = 0;
    int TFill = clock();
    for (seed = 0; seed < seedLimit; seed++) 
      fillarray(A, siz, seed);
    TFill = clock() - TFill;
    T = clock();
    for (seed = 0; seed < seedLimit; seed++) { 
      cnt = 0;
      // fillRandom(A, siz, seed, p); // zeros + random
      fillRandom2(A, siz, seed, p); // sorted + random
      // (*alg1)(A, siz, compare1); 
      (*alg1)(A, 0, siz-1, compare1); 
      cnt1 = cnt1 + cnt;
    }
    alg1Time = clock() - T - TFill;
    T = clock(); cnt = 0;
    for (seed = 0; seed < seedLimit; seed++) { 
      cnt = 0;
      // fillRandom(A, siz, seed, p); // zeros + random
      fillRandom2(A, siz, seed, p); // sorted + random
      // (*alg2)(A, siz, compare2);
      (*alg2)(A, 0, siz-1, compare2); 
      cnt2 = cnt2 + cnt;
    }
    alg2Time = clock() - T - TFill;
    printf("%s %d %s %i", "siz: ", siz, " p: ", p);
    printf(" %s %d %s", "alg1Time: ", alg1Time, " ");
    printf("%s %d %s", "alg2Time: ", alg2Time, " ");
    printf("cnt1: %i cnt2: %i ", (cnt1/seedLimit), (cnt2/seedLimit) );
    float frac = 0;
    if ( alg1Time != 0 ) frac = alg2Time / ( 1.0 * alg1Time );
    printf("%s %f %s", "frac: ", frac, "\n");
  }
} // end testNonRandom

void compareZeros00(char *label, int siz, int seedLimit, 
		   void (*alg1)(), void (*alg2)(),
		   int (*compare1)(), int (*compare2)() ) {
  printf("%s on size: %d seedLimit: %d\n", label, siz, seedLimit);
  int seed;
  // int limit = 1024 * 1024 * 16 + 1;
  int limit = 1024 * 1024 + 1;
  while (siz <= limit) {
    printf("%s %d %s %d %s", "siz: ", siz, " seedLimit: ", seedLimit, "\n");
    struct intval *pi;
    void **A = myMalloc("compareAlgorithms0 1", sizeof(pi) * siz);
    // construct array
    int i;
    for (i = 0; i < siz; i++) {
      pi = myMalloc("compareAlgorithms0 2", sizeof (struct intval));
      A[i] = pi;
    };  
    // warm up the process
    for (seed = 0; seed < seedLimit; seed++) 
      fillarray(A, siz, seed);
    // int p;
    // for (p = 0; p < 101; p = p + 20 )  // percentages of random elements
    //   testNonRandom(p, A, siz, seedLimit, alg1, alg2, compare1, compare2);
    testNonRandom(0, A, siz, seedLimit, alg1, alg2, compare1, compare2);
    testNonRandom(20, A, siz, seedLimit, alg1, alg2, compare1, compare2);
    testNonRandom(40, A, siz, seedLimit, alg1, alg2, compare1, compare2);
    // testNonRandom(100, A, siz, seedLimit, alg1, alg2, compare1, compare2);

    // free array
    for (i = 0; i < siz; i++) {
      free(A[i]);
    };
    free(A);
    siz = siz * 2;
    seedLimit = seedLimit / 2;
  }
} // end compareZeros00


void validateAlgorithm0(char* label, int siz, void (*alg1)(), void (*alg2)() ) {
  // siz = 1024*128;
  printf("%s on size: %d\n", label, siz);
  // create the input for alg1 ...
  struct intval *pi;
  void **A = myMalloc("validateAlgorithm0 1", sizeof(pi) * siz);
  int i;
  for (i = 0; i < siz; i++) {
    pi = myMalloc("validateAlgorithm0 2", sizeof (struct intval));
    A[i] = pi;
  };
  fillarray(A, siz, 100);
  // here alternative ways to fill the array
  // int k;
  // for ( k = 0; k < siz; k++ ) { pi = (struct intval *) A[k]; pi->val = 0; }
  // for ( k = 0; k < siz; k++ ) { pi = (struct intval *) A[k]; pi->val = k%5; }
  // for ( k = 0; k < siz; k++ ) { pi = (struct intval *) A[k]; pi->val = siz-k; }
  // ... sort it
  (*alg1)(A, 0, siz-1, compareXY);

  // create the input for alg2 ...
  void **B = myMalloc("validateAlgorithm0 3", sizeof(pi) * siz);
  // struct intval *pi;
  for (i = 0; i < siz; i++) {
    pi =  myMalloc("validateAlgorithm0 4", sizeof (struct intval));
    B[i] = pi;
  };
  fillarray(B, siz, 100);
  // for ( k = 0; k < siz; k++ ) { pi = (struct intval *) B[k]; pi->val = 0; }
  // for ( k = 0; k < siz; k++ ) { pi = (struct intval *) B[k]; pi->val = k%5; }
  // for ( k = 0; k < siz; k++ ) { pi = (struct intval *) B[k]; pi->val = siz-k; }
  // ... sort it
  (*alg2)(B, 0, siz-1, compareXY);

  // check that the two outputs are the same
  int foundError = 0;
  for (i = 0; i < siz; i++)
    // if ( A[i] != B[i] ) {
    if ( compareXY(A[i], B[i]) != 0 ) {
      pi = (struct intval *) A[i]; int ai = pi->val;
      pi = (struct intval *) B[i]; int bi = pi->val;
      printf("validate error i: %i ai %i bi %i\n", i, ai, bi);
      foundError = 1;
    }
  if ( !foundError ) 
    printf("NO error found ...\n");
  // free array
    for (i = 0; i < siz; i++) {
      free(A[i]); 
    };
    free(A);
    for (i = 0; i < siz; i++) {
      free(B[i]); 
    };
    free(B);
} // end validateAlgorithm0

void validateAlgorithm(char* label, void (*alg1)(), void (*alg2)() ) {  
  validateAlgorithm0(label, 16*1024*1024, alg1, alg2); 
  // validateAlgorithm0(label, 1024 * 1024 * 16, alg1, alg2);
  // validateAlgorithm0(label, 1024*1024 * 4, alg1, alg2);
  // validateAlgorithm0(label, 1024*1024, alg1, alg2);
  // validateAlgorithm0(label, 1024*512, alg1, alg2);
  // validateAlgorithm0(label, 1024*256, alg1, alg2);
  // validateAlgorithm0(label, 1024*64, alg1, alg2);
  // validateAlgorithm0(label, 1024*32, alg1, alg2);
  // validateAlgorithm0(label, 1024*16, alg1, alg2);
  // validateAlgorithm0(label, 1024*8, alg1, alg2);
  // validateAlgorithm0(label, 1024*4, alg1, alg2);
  // validateAlgorithm0(label, 1024*2, alg1, alg2);
  // validateAlgorithm0(label, 1024, alg1, alg2);
  // validateAlgorithm0(label, 300, alg1, alg2);
  // validateAlgorithm0(label, 9, alg1, alg2);
} // end validateAlgorithm
/*
void validateC2() {
  validateAlgorithm("Running validate cut2 ...",
		    //		    quicksort0, cut2);
		    heapc, cut2);
}
*/
/*
void validateC21() {
  validateAlgorithm("Running validate C2Split ...",
		    quicksort0, c21);
}
*/
// /*
void validateC2lr() {
  validateAlgorithm("Running validate C2LR ...",
		    quicksort0, cut2lr);
}
// */
/*
void validateC2lr2() {
  validateAlgorithm("Running validate C2LR2 ...",
		    quicksort0, cut2lr2);
}
*/
/*
void validateC2lrb() {
  validateAlgorithm("Running validate C2LRB ...",
		    quicksort0, cut2lrb);
}

void validateC2left() {
  validateAlgorithm("Running validate C2Left ...",
		    quicksort0, cut2left);
}
*/
/*
void validateC2sort() {
  validateAlgorithm("Running validate C2sort ...",
		    quicksort0, cut2);
}
*/
/*
void validateC2msort() {
  validateAlgorithm("Running validate C2msort ...",
		    quicksort0, cut2m);
}
*/
/*
void validateC2ksort() {
  validateAlgorithm("Running validate C2ksort ...",
		    quicksort0, cut2k);
}
*/
/*
void validateC2k2sort() {
  validateAlgorithm("Running validate C2k2sort ...",
		    quicksort0, cut2k2);
}
*/
/*
void validateC2k3sort() {
  validateAlgorithm("Running validate C2k3sort ...",
		    quicksort0, cut2k3);
}
*/
/*
void validateC2k4sort() {
  validateAlgorithm("Running validate C2k4sort ...",
		    quicksort0, cut2k4);
}
*/
/*
void validateC2k1sort() {
  validateAlgorithm("Running validate C2k1sort ...",
		    quicksort0, cut2k1);
}
*/
/*
void validateCN4sort() {
   validateAlgorithm("Running validate CN4sort ...",
		    quicksort0, cut4n);
}
*/
/*
void validateCD4sort() {
   validateAlgorithm("Running validate CD4sort ...",
		    quicksort0, cut4d);
}
*/
/*
void validateQ0() {
  validateAlgorithm("Running validate Q0 ...",
		    quicksort0, heapc);
}
*/
/*
void validateMyqs() {
  validateAlgorithm("Running validate Myqs ...",
		    quicksort0, myqs);
}
void validateMybam() {
  validateAlgorithm("Running validate Mybam ...",
		    quicksort0, mybam);
}
*/
/*
void validateMergeSort() {
  validateAlgorithm("Running validate MergeSort ...",
		    quicksort0, mergeSort);
}
*/

void range(void (*alg1)(), int (*compar)(), int bool) {
  extern long long cnt;
  int siz = 1024;
  // int reps = 1024 * 32;
  int reps = 1024 * 32 * 5;
  int limit = 1024 * 1024 * 16 + 1;
  int i;
  double algTime;
  clock_t TFill, T;
  while (siz <= limit) {
    cnt = 0;
    printf("siz: %d reps: %d ", siz, reps);
    struct intval *pi;
    void **A = myMalloc("range 1", sizeof(pi) * siz);
    // construct array
    for (i = 0; i < siz; i++) {
      pi = myMalloc("range 2", sizeof (struct intval));
      A[i] = pi;
    };
     // warm up the process
    for (i = 0; i < reps; i++) 
      fillarray(A, siz, reps+i);
    TFill = clock();
    for (i = 0; i < reps; i++) 
	fillarray(A, siz, reps+i);
    TFill = clock() - TFill;
    T = clock();
     for (i = 0; i < reps; i++)  {
	fillarray(A, siz, reps+i);
	if ( bool ) (*alg1)(A, 0, siz-1, compar); 
	else (*alg1)(A, siz, sizeof(pi), compar); 
    }
    algTime = clock() - T - TFill;
    printf("cc# %lld algTime %f\n", cnt/reps, 
	   algTime/(CLOCKS_PER_SEC*reps));
    // free array
    for (i = 0; i < siz; i++) {
      free(A[i]);
    };
    free(A);
    siz = siz * 4;
    reps = reps / 4;
  }
} // end range
/*
void rangeLQ() {
  void _quicksort();
  printf("rangeLQ\n");
  range(_quicksort, compareIntVal2, 0);
} // end rangeLQ
*/
/*
void rangeMyqsort() {
  void myqsort();
  printf("rangeMyqsort\n");
  range( myqsort, compareXY, 1);
} // end rangeMyqsort()
*/
/*
void rangeBentley() { 
  void bentley();
  printf("rangeBentley\n");
  range(bentley, compareIntVal2, 0);
} // end rangeBentley
*/
/*
void rangeMybam() {
  void rangeMybam();
  printf("rangeMybam\n");
  range(mybam, compareXY, 1);
} // end rangeQuicksort0
*/
/*
void rangeQuicksort0() {
  void quicksort0();
  printf("rangeQuicksort0\n");
  range(quicksort0, compareXY, 1);
} // end rangeQuicksort0
*/
/*
void rangeDflgm0() {
  void dflgm0();
  printf("rangeDflgm0\n");
  range(dflgm0, compareXY, 1);
} // end rangeDflgm0
*/
/*
void rangeCut2() {
  void cut2();
  printf("rangeCut2\n");
  range(cut2, compareXY, 1);
} // end rangeCut2
*/
/*
void rangeCut2lr() {
  void cut2lr();
  printf("rangeCut2lr\n");
  range(cut2lr, compareXY, 1);
} // end rangeCut2lr
*/
/*
void rangeBlockSort() {
  void blockSort();
  printf("rangeblockSort\n");
  range(blockSort, compareXY, 1);
} // end rangeBlockSort
*/
// 2-pivot
void rangeDpq() {
  void dpq();
  printf("rangeDpq\n");
  range(dpq, compareXY, 1);
} // end rangeDpq
/*
void rangeTps() {
  void tps();
  printf("rangeTps\n");
  range(tps, compareXY, 1);
} // end rangeTps
*/
// 3 pivot
/*
void rangeMpq() {
  void part3();
  printf("rangeMpq\n");
  range(part3, compareXY, 1);
} // end rangeMpq
*/
/*
void rangeCut4() {
  void cut4();
  printf("rangeCut4\n");
  range(cut4, compareXY, 1);
} // end rangeCut4
*/
/*
void rangeCut4n() {
  void cut4n();
  printf("rangeCut4n\n");
  range(cut4n, compareXY, 1);
} // end rangeCut4n
*/
/*
void rangeCut4d() {
  void cut4d();
  printf("rangeCut4d\n");
  range(cut4d, compareXY, 1);
} // end rangeCut4d
*/
/*
void rangeCut7() {
  void cut7();
  printf("rangeCut7\n");
  range(cut7, compareXY, 1);
} // end rangeCut7
*/ 
#undef iswap
