// File: c:/bsd/rigel/sort/C7/ComparisonCounting.c
// Date: Fri Jan 10 08:42:56 2014 - Wed Aug 17 20:16:55 2022
// (C) OntoOO/ Dennis de Champeaux

/* 
Counting comparisons 
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
#include "Isort.c"
#include "Hsort.c"
#include "Dsort.c"  // dflgm member
#include "D3sort.c" // recursive dflgm
#include "Qusort.c" // quicksort member
#include "Qusortm.c" // quicksort with merge sort
#include "Qusort00.c" // 

// */
// #include "Qusort.c"
#include "D4.c" 
#include "C2sort.c" 
#include "C2fsort.c"
#include "C2k1.c" // Sun Mar 06 12:45:01 2022
#include "C2k2.c" // Sun Mar 10 2022
// #include "C2k3.c" // Sun Aug 07 10:46:10 2022
#include "C2k4.c" // Thu Aug 11 08:00:47 2022
#include "C2LR.c"
#include "C2LR2.c" // Sat Aug 13 13:33:40 2022
#include "C2Nk1.c" // Mon Aug 08 18:33:02 2022
#include "C3sort.c" // tps member

// #include "dltpq.c"
// #include "./DOUG2/stpq.c"

// #include "C2Split.c"
// #include "DdcChen.c"
// #include "C2LRB.c"
// #include "C2Left.c"
// #include "Mybam.c"
// #include "Myqsort.c"

#include "C4.c"   // cut4 member
#include "CN4.c"
#include "CD4.c"
#include "C7.c"

#include "MergeSort.c"
// #include "Msn.c"

// #include "Dpq2.c"
// #include "Timsort.c"

#include "C2sortNH.c"
#include "Quickselect.c"

#include "C2m.c" // Mon Nov 15 16:09:17 2021 modified C2sort
#include "C2k.c" // Sat Feb 26 13:03:37 2022
// #include "C2k2.c" // Sun Mar 06 12:45:01 2022

#define iswap(p, q, A) { void *t3t = A[p]; A[p] = A[q]; A[q] = t3t; }


void callChensort();
void callBentley();
void callLQ();
void introsort();
void dflgmTest0();
void dflgmTest2();
// void myqs();
void quicksort0();
void cut2();
void cut2f();
void tps();
void tps5();
void cut4();
void cut4n();
void cut4d();
void cut7();
void dpq();
void dpq2();
void timsort();
void cc();
void part3();
void quicksort0k();
void blockSort();
void ddcchen();
// void mybam();
// void myqsort();
void dflgm0();
void mergeSort();
void compare00FourSortAgainstFiveSort();
void compare00DPQAgainstFiveSort();
void compare00DPQAgainstFourSort();
void validateC2();
void validateC21();
void validateC2lr();
void validateC2lrb();
void validateC2left();
void validateC2sort();
void validateC2msort(); // Mon Nov 15 16:23:32 2021
void validateC2ksort(); // Sat Feb 26 13:04:22 2022
void validateC2k1sort(); // Thu Mar 10 20:36:35 2022
void validateC2k2sort(); // Sun Mar 06 12:46:44 2022
// void validateC2k3sort(); // Sun Mar 06 12:46:44 2022
void validateCN4sort();
void validateCD4sort();
void validateQ0();
void validateMyqs();
void validateMybam();
void validateMergeSort();

void range();
void rangeLQ();
// void rangeMyqsort();
void rangeBentley();
void rangeMybam();
void rangeQuicksort0();
void rangeDflgm0();
void rangeCut2();
void rangeCut2lr();
void rangeBlockSort();
void rangeDpq();
void rangeTps();
void rangeMpq();
void rangeCut4();
void rangeCut4n();
void rangeCut4d();
void rangeCut7();
// void rangeDpq2();

void fillRandom();
void fillRandom2();
// int clock();

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

// This comparison formula is used for qsort in callQsort and
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
  // cc("myqs    ", myqs, compareXY, 1); 
  // cc("LQ      ", callLQ, compareIntVal2, 0);
  // cc("Myqsort ", myqsort, compareXY, 1);
  // cc("bentley ", callBentley, compareIntVal2, 0);
  // cc("mybam   ", mybam, compareXY, 1);
  // cc("quick0  ", quicksort0, compareXY, 1);
  // cc("quickm  ", quicksortm, compareXY, 1);
  // cc("quickn  ", quicksortmn, compareXY, 1);
  // cc("quick3  ", quicksort3, compareXY, 1);
  // cc("quicksort0k", quicksort0k, compareXY, 1);
  // cc("dflgm0  ", dflgm0, compareXY, 1);   
  // cc("cut2    ", cut2, compareXY, 1);
  // cc("cut2m   ", cut2m, compareXY, 1); // Mon Nov 15 16:26:06 2021
  // cc("c2lr    ", cut2lr, compareXY, 1); 
  // cc("cut2lr2 ", cut2lr2, compareXY, 1); 
  // cc("cut2k   ", cut2k, compareXY, 1); // 
  // cc("cut2k1  ", cut2k1, compareXY, 1); // 
  // cc("cut2k2  ", cut2k2, compareXY, 1); 
  //     cc("cut2k3  ", cut2k3, compareXY, 1); 
  // cc("cut2k4  ", cut2k4, compareXY, 1); // Thu Aug 11 08:05:32 2022
  // cc("cut2Nk1 ", cut2Nk1, compareXY, 1); // Mon Aug 08 18:46:54 2022
  // cc("cut2NH  ", cut2NH, compareXY, 1);
  // cc("c21     ", c21, compareXY, 1); // parallel? // removed
  // cc("ddcch", ddcchen, compareXY, 1); // removed
  // cc("c2lrb   ", cut2lrb, compareXY, 1); // removed
  // cc("c2left  ", cut2left, compareXY, 1); // removed
  // cc("cut2f", cut2f, compareXY, 1);
  // cc("introsort", introsort, compareXY, 1);
  // cc("BlockSor", blockSort, compareXY, 1);
  // cc("timsort ", timsort, compareXY, 1);
     // 2 pivot
  // cc("dpq     ", dpq, compareXY, 1);
  // cc("cut2f", cut2f, compareXY, 1);
  // cc("introsort", introsort, compareXY, 1);
  // cc("BlockSor", blockSort, compareXY, 1);
  // cc("timsort ", timsort, compareXY, 1);
     // 2 pivot
  // cc("dpq     ", dpq, compareXY, 1);
  // cc("dpq2    ", dpq2, compareXY, 1);
  // cc("tps     ", tps, compareXY, 1);
  // cc("tps5", tps5, compareXY, 1);
     // 3 pivot
  // cc("mpq     ", part3, compareXY, 1);
  // cc("dltpq", dltpq, compareXY, 1);
  // cc("stpq ", stpq, compareXY, 1);
  // cc("cut4    ", cut4, compareXY, 1);
  // cc("cut4n   ", cut4n, compareXY, 1);
  // cc("cut4d   ", cut4d, compareXY, 1);
  // cc("cut7    ", cut7, compareXY, 1);
     // Misc
  // cc("dflgm   ", dflgmTest0, compareXY, 1);
  // cc("mergeSor", mergeSort, compareXY, 1);
  // cc("Chen", callChensort, compareIntVal2, 0);
  // compare00FourSortAgainstFiveSort();
  // compare00DPQAgainstFiveSort();
  // compare00DPQAgainstFourSort();
  // validateC2();
  // validateC21();
  // validateC2lr();
  // validateC2lrb();
  // validateC2left();
  // validateC2sort();
  // validateC2msort(); // Mon Nov 15 16:26:30 2021
  // validateC2ksort(); // 
  // validateC2k1sort(); /
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
  // */

  /*
  int numberOfBlocks = lng/2001, j, k;
  i =0;
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
  // int reps = 5;
  // int reps = 2;
  int reps = 1;
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


// #include <_ansi.h>
#include <stdlib.h>

#ifndef __GNUC__
#define inline
#endif

// static inline char	*med33 _PARAMS((char *, char *, char *, int (*)()));
// static inline void	 swapfunc _PARAMS((char *, char *, int, int));

#define min(a, b)	(a) < (b) ? a : b

/*
 * Qsort routine from Bentley & McIlroy's "Engineering a Sort Function".
 */
#define swapcode(TYPE, parmi, parmj, n) { 		\
	long i = (n) / sizeof (TYPE); 			\
	register TYPE *pi = (TYPE *) (parmi); 		\
	register TYPE *pj = (TYPE *) (parmj); 		\
	do { 						\
		register TYPE	t = *pi;		\
		*pi++ = *pj;				\
		*pj++ = t;				\
        } while (--i > 0);				\
}

#define SWAPINIT(a, es) swaptype = ((char *)a - (char *)0) % sizeof(long) || \
	es % sizeof(long) ? 2 : es == sizeof(long)? 0 : 1;

static inline void swapfunc(a, b, n, swaptype)
     char *a;
     char *b;
     int n;
     int swaptype;
{
	if (swaptype <= 1) 
		swapcode(long, a, b, n)
	else
		swapcode(char, a, b, n)
}

#define swap(a, b)					\
	if (swaptype == 0) {				\
		long t = *(long *)(a);			\
		*(long *)(a) = *(long *)(b);		\
		*(long *)(b) = t;			\
	} else						\
		swapfunc(a, b, es, swaptype)

#define vecswap(a, b, n) 	if ((n) > 0) swapfunc(a, b, n, swaptype)

static inline char * med33(a, b, c, cmp)
     char *a;
     char *b;
     char *c;
     int (*cmp)(); 
{
	return cmp(a, b) < 0 ?
	       (cmp(b, c) < 0 ? b : (cmp(a, c) < 0 ? c : a ))
              :(cmp(b, c) > 0 ? b : (cmp(a, c) < 0 ? a : c ));
}

void bentley(a, n, es, cmp)
     void *a;
     size_t n;
     size_t es;
     int (*cmp)();
{
  char *pa, *pb, *pc, *pd, *pl, *pm, *pn;
  int d, r, swaptype; 
  // The use of swap_cnt and the 2nd invocation of insertionsort has been removed
  // int swap_cnt; 

loop:	SWAPINIT(a, es);
  // swap_cnt = 0;
	if (n < 7) {
		for (pm = (char *) a + es; pm < (char *) a + n * es; pm += es)
			for (pl = pm; pl > (char *) a && cmp(pl - es, pl) > 0;
			     pl -= es)
				swap(pl, pl - es);
		return;
	}
	// icnt++; // invocation cnt
	pm = (char *) a + (n / 2) * es;
	if (n > 7) {
		pl = a;
		pn = (char *) a + (n - 1) * es;
		if (n > 40) {
			d = (n / 8) * es;
			pl = med33(pl, pl + d, pl + 2 * d, cmp);
			pm = med33(pm - d, pm, pm + d, cmp);
			pn = med33(pn - 2 * d, pn - d, pn, cmp);
		}
		pm = med33(pl, pm, pn, cmp);
	}
	swap(a, pm);
	pa = pb = (char *) a + es;

	pc = pd = (char *) a + (n - 1) * es;
	for (;;) {
		while (pb <= pc && (r = cmp(pb, a)) <= 0) {
			if (r == 0) {
			        // swap_cnt = 1;
				swap(pa, pb);
				pa += es;
			}
			pb += es;
		}
		while (pb <= pc && (r = cmp(pc, a)) >= 0) {
			if (r == 0) {
			        // swap_cnt = 1;
				swap(pc, pd);
				pd -= es;
			}
			pc -= es;
		}
		if (pb > pc)
			break;
		swap(pb, pc);
		// swap_cnt = 1;
		pb += es;
		pc -= es;
	}
	/*
	if (swap_cnt == 0) {  // Switch to insertion sort 
		for (pm = (char *) a + es; pm < (char *) a + n * es; pm += es)
			for (pl = pm; pl > (char *) a && cmp(pl - es, pl) > 0; 
			     pl -= es)
				swap(pl, pl - es);
		return;
	}
	*/
	pn = (char *) a + n * es;
	r = min(pa - (char *)a, pb - pa);
	vecswap(a, pb - r, r);
	r = min(pd - pc, pn - pd - es);
	vecswap(pb, pn - r, r);
	/* Ordering on the recursive calls has been added to obtain at most 
	   log2(N) stack space 
	if ((r = pb - pa) > es)
		bentley(a, r / es, es, cmp);
	if ((r = pd - pc) > es) { 
	        // Iterate rather than recurse to save stack space 
		a = pn - r;
		n = r / es;
		goto loop;
	}
	*/
	
    int left =  pb - pa;
    int right = pd - pc;
    if ( left <= right ) {
       if ( left > es ) bentley(a, left / es, es, cmp);
       if ( right > es ) {
	   // Iterate rather than recurse to save stack space 
	   a = pn - right;
	   n = right / es;
	   goto loop;
       }
    } else {
       if ( right > es ) bentley(pn-right, right / es, es, cmp);
       if ( left > es ) {
	   // Iterate rather than recurse to save stack space 
	   // a = pn - left;
	   n = left / es;
	   goto loop;
       }
    }
} // end bentley

/* Copyright (C) 1991,1992,1996,1997,1999,2004 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Written by Douglas C. Schmidt (schmidt@ics.uci.edu).

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, write to the Free
   Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
   02111-1307 USA.  */

/* If you consider tuning this algorithm, you should consult first:
   Engineering a sort function; Jon Bentley and M. Douglas McIlroy;
   Software - Practice and Experience; Vol. 23 (11), 1249-1265, 1993.  */

// #include <alloca.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

/* Byte-wise swap two items of size SIZE. */
#define SWAP(a, b, size)						      \
  do									      \
    {									      \
      register size_t __size = (size);					      \
      register char *__a = (a), *__b = (b);				      \
      do								      \
	{								      \
	  char __tmp = *__a;						      \
	  *__a++ = *__b;						      \
	  *__b++ = __tmp;						      \
	} while (--__size > 0);						      \
    } while (0)

/* Discontinue quicksort algorithm when partition gets below this size.
   This particular magic number was chosen to work best on a Sun 4/260. */
#define MAX_THRESH 4

/* Stack node declarations used to store unfulfilled partition obligations. */
typedef struct
  {
    char *lo;
    char *hi;
  } stack_node;

/* The next 4 #defines implement a very fast in-line stack abstraction. */
/* The stack needs log (total_elements) entries (we could even subtract
   log(MAX_THRESH)).  Since total_elements has type size_t, we get as
   upper bound for log (total_elements):
   bits per byte (CHAR_BIT) * sizeof(size_t).  */
#define STACK_SIZE	(CHAR_BIT * sizeof(size_t))
#define PUSH(low, high)	((void) ((top->lo = (low)), (top->hi = (high)), ++top))
#define	POP(low, high)	((void) (--top, (low = top->lo), (high = top->hi)))
#define	STACK_NOT_EMPTY	(stack < top)


/* Order size using quicksort.  This implementation incorporates
   four optimizations discussed in Sedgewick:

   1. Non-recursive, using an explicit stack of pointer that store the
      next array partition to sort.  To save time, this maximum amount
      of space required to store an array of SIZE_MAX is allocated on the
      stack.  Assuming a 32-bit (64 bit) integer for size_t, this needs
      only 32 * sizeof(stack_node) == 256 bytes (for 64 bit: 1024 bytes).
      Pretty cheap, actually.

   2. Chose the pivot element using a median-of-three decision tree.
      This reduces the probability of selecting a bad pivot value and
      eliminates certain extraneous comparisons.

   3. Only quicksorts TOTAL_ELEMS / MAX_THRESH partitions, leaving
      insertion sort to order the MAX_THRESH items within each partition.
      This is a big win, since insertion sort is faster for small, mostly
      sorted array segments.

   4. The larger of the two sub-partitions is always pushed onto the
      stack first, with the algorithm then concentrating on the
      smaller partition.  This *guarantees* no more than log (total_elems)
      stack size is needed (actually O(1) in this case)!  */

// This is qsort in the Linux C-library
void
_quicksort (void *const pbase, size_t total_elems, size_t size,
	    //	    __compar_d_fn_t cmp, void *arg)
	    int (*cmp)(), void *arg)
{
  register char *base_ptr = (char *) pbase;

  const size_t max_thresh = MAX_THRESH * size;

  if (total_elems == 0)
    /* Avoid lossage with unsigned arithmetic below.  */
    return;

  if (total_elems > MAX_THRESH)
    {
      char *lo = base_ptr;
      char *hi = &lo[size * (total_elems - 1)];
      stack_node stack[STACK_SIZE];
      stack_node *top = stack;

      PUSH (NULL, NULL);

      while (STACK_NOT_EMPTY)
        {
	  // icnt++; // invocation cnt
          char *left_ptr;
          char *right_ptr;

	  /* Select median value from among LO, MID, and HI. Rearrange
	     LO and HI so the three values are sorted. This lowers the
	     probability of picking a pathological pivot value and
	     skips a comparison for both the LEFT_PTR and RIGHT_PTR in
	     the while loops. */

	  char *mid = lo + size * ((hi - lo) / size >> 1);
	  if ((*cmp) ((void *) mid, (void *) lo, arg) < 0) {
	    SWAP (mid, lo, size);
	  }
	  if ((*cmp) ((void *) hi, (void *) mid, arg) < 0)
	    SWAP (mid, hi, size);
	  else
	    goto jump_over;
	  if ((*cmp) ((void *) mid, (void *) lo, arg) < 0)
	    SWAP (mid, lo, size);
	jump_over:;
	  left_ptr  = lo + size;
	  right_ptr = hi - size;

	  /* Here's the famous ``collapse the walls'' section of quicksort.
	     Gotta like those tight inner loops!  They are the main reason
	     that this algorithm runs much faster than others. */
	  do
	    {
	      while ((*cmp) ((void *) left_ptr, (void *) mid, arg) < 0)
		left_ptr += size;

	      while ((*cmp) ((void *) mid, (void *) right_ptr, arg) < 0)
		right_ptr -= size;

	      if (left_ptr < right_ptr)
		{
		  SWAP (left_ptr, right_ptr, size);
		  if (mid == left_ptr)
		    mid = right_ptr;
		  else if (mid == right_ptr)
		    mid = left_ptr;
		  left_ptr += size;
		  right_ptr -= size;
		}
	      else if (left_ptr == right_ptr)
		{
		  left_ptr += size;
		  right_ptr -= size;
		  break;
		}
	    }
	  while (left_ptr <= right_ptr);

          /* Set up pointers for next iteration.  First determine whether
             left and right partitions are below the threshold size.  If so,
             ignore one or both.  Otherwise, push the larger partition's
             bounds on the stack and continue sorting the smaller one. */

          if ((size_t) (right_ptr - lo) <= max_thresh)
            {
              if ((size_t) (hi - left_ptr) <= max_thresh)
		/* Ignore both small partitions. */
                POP (lo, hi);
              else
		/* Ignore small left partition. */
                lo = left_ptr;
            }
          else if ((size_t) (hi - left_ptr) <= max_thresh)
	    /* Ignore small right partition. */
            hi = right_ptr;
          else if ((right_ptr - lo) > (hi - left_ptr))
            {
	      /* Push larger left partition indices. */
              PUSH (lo, right_ptr);
              lo = left_ptr;
            }
          else
            {
	      /* Push larger right partition indices. */
              PUSH (left_ptr, hi);
              hi = right_ptr;
            }
        }
    }

  /* Once the BASE_PTR array is partially sorted by quicksort the rest
     is completely sorted using insertion sort, since this is efficient
     for partitions below MAX_THRESH size. BASE_PTR points to the beginning
     of the array to sort, and END_PTR points at the very last element in
     the array (*not* one beyond it!). */

  // #define min(x, y) ((x) < (y) ? (x) : (y))

  {
    char *const end_ptr = &base_ptr[size * (total_elems - 1)];
    char *tmp_ptr = base_ptr;
    char *thresh = min(end_ptr, base_ptr + max_thresh);
    register char *run_ptr;

    /* Find smallest element in first threshold and place it at the
       array's beginning.  This is the smallest array element,
       and the operation speeds up insertion sort's inner loop. */

    for (run_ptr = tmp_ptr + size; run_ptr <= thresh; run_ptr += size)
      if ((*cmp) ((void *) run_ptr, (void *) tmp_ptr, arg) < 0)
        tmp_ptr = run_ptr;

    if (tmp_ptr != base_ptr)
      SWAP (tmp_ptr, base_ptr, size);

    /* Insertion sort, running from left-hand-side up to right-hand-side.  */

    run_ptr = base_ptr + size;
    while ((run_ptr += size) <= end_ptr)
      {
	tmp_ptr = run_ptr - size;
	while ((*cmp) ((void *) run_ptr, (void *) tmp_ptr, arg) < 0)
	  tmp_ptr -= size;

	tmp_ptr += size;
        if (tmp_ptr != run_ptr)
          {
            char *trav;

	    trav = run_ptr + size;
	    while (--trav >= run_ptr)
              {
                char c = *trav;
                char *hi, *lo;

                for (hi = lo = trav; (lo -= size) >= tmp_ptr; hi = lo)
                  *hi = *lo;
                *hi = c;
              }
          }
      }
  }
} // end _quicksort 

// LQ is qsort in the Linux C-library
void callLQ(void **A, int size, int (*compar ) () ) { 
  struct intval *pi;
  _quicksort(A, size, sizeof(pi), compar, NULL);
} // end callLQ



// ************************************************************


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
	  /*
	    The "j--;" causes the last element of the segment to be miss allocated
	    in the partition.
	    Omitting "j--;" causes recursive calls on the same segment with constant 
	    data.  This triggers the invocation of heapsort on that segment
	   */
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


// ************************************************************
// Dijkstra, but had to add insertionsort in the driver
void dflgm2(void **A, int N, int M, int pivotx,
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
} // end dflgm2


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
    dflgm(A, N, M, p0, dflgmTest2, depthLimit, compareXY);
    // dflgm2(A, N, M, p0, dflgmTest2, depthLimit, compareXY);

} // end dflgmTest2

// ************************************************************

// a very simple quicksort
/*
void myqsc();
void myqs(void **A, int N, int M, int (*compare)()) {
  int L = M-N;
  if (L <= 0) return;
  int dp = 2.5 * floor(log(L));
  myqsc(A, N, M, dp, compare);
}
*/
// void dflgm();
// void heapc();
// void quicksort0c();
/*
void myqsc(void **A, int N, int M, int depthLimit, int (*compare)()) {
  int L;
 again:
   L =  1 + M - N;
   if ( L < 12) {
   // if ( L < 8) {
      insertionsort(A, N, M, compare);
      return;
   }
   if ( depthLimit <= 0 ) {
     heapc(A, N, M, compare);
     return;
   }
   depthLimit--;
   int p0 = N + (L>>1); // N + L/2;
   if ( 7 < L ) {
     int pn = N;
     int pm = M;
     // if ( 51 < L ) {
     if ( 40 < L ) {
       int d = (L-2)>>3; // L/8;
       pn = med2(A, pn, pn + d, pn + 2 * d, compare);
       p0 = med2(A, p0 - d, p0, p0 + d, compare);
       pm = med2(A, pm - 2 * d, pm - d, pm, compare);
     }
     p0 = med2(A, pn, p0, pm, compare);
   }
   // p0 is index to 'best' pivot ... 
   if ( N != p0 )  iswap(N, p0, A); // ... and is put in first position
   void *p = A[N], *ai, *aj;
   int i = N, j = M+1;
 Right0:
   j--;
   aj = A[j];
   if ( compare(p, aj) < 0 ) {
     if ( i < j ) { goto Right0; }
     else goto Done;
   }
 Left:
     // |--------]-------------(-------|
     // N        i             j       M
   if ( i == j ) goto Done; 
   i++;
   ai = A[i];
   if ( compare(ai, p) < 0 ) { goto Left; }
 Right: // ai -> R
   if ( i == j ) { i--; goto Done; } 
   aj = A[j];
   if ( compare(p, aj) < 0 ) { j--; goto Right; }
   // aj -> L
   A[i] = aj; A[j] = ai;
   j--;
   goto Left;
 Done:
   if ( N != i ) iswap(N, i, A);
   if ( i - N <= M - i ) {
     myqsc(A, N, i-1, depthLimit, compare);
     N = i+1;
     goto again;
   }
   myqsc(A, i+1, M, depthLimit, compare);
   M = i-1;
   goto again;


} // end myqs
*/
// ************************************************************

// tps5 another three partition function
void tpsc5();
void tps5(void **A, int N, int M, int (*compare)()) {
  int L = M - N;
  if ( L < 250 ) { 
    cut2(A, N, M, compare);
    return;
  }
  int depthLimit = 2.5 * floor(log(L));
  tpsc5(A, N, M, depthLimit, compare);
} // end tps5

void tpsc5(void **A, int N, int M, int depthLimit, int (*compare)()) 
{
  // register int k; // for-index
  // int z; // for tracing
  register int i, j, up, lw; // indices
  register void *x, *y; // array values
  register void *pl, *pr; // pivots

 // A 3d recursive call is avoided by jumping back to Start.  
 Start:
  // printf("tspc N %d M %d depthLimit %d\n", N,M,depthLimit);

  if ( depthLimit <= 0 ) { // avoid quadradic explosion
    heapc(A, N, M, compare);
    return;
  }
  int L = M - N;
  if ( L < 64 ) {
  // if ( L < cut2Limit ) {
  // if ( L < cut3Limit ) {

    quicksort0c(A, N, M, depthLimit, compare);
    return;
  }
  depthLimit--;

        int sixth = (L + 1) / 6;
        int e1 = N  + sixth;
        int e5 = M - sixth;
        int e3 = N + L/2; // (N+M) / 2; // The midpoint
        int e4 = e3 + sixth;
        int e2 = e3 - sixth;
        // Sort these elements using a 5-element sorting network
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

	// Fix end points
	if ( compare(ae1, A[N]) < 0 ) iswap(N, e1, A);
	if ( compare(A[M], ae5) < 0 ) iswap(M, e5, A);

	void tpsc5();
	// if ( ae2 == ae3 || ae3 == ae4 ) {
	if ( compare(ae2, ae3) == 0 || compare(ae3, ae4) == 0 ) {
	  dflgm(A, N, M, e3, tpsc5, depthLimit, compare);
	  return;
	}

	pl = A[e2]; pr = A[e4]; // left and right pivots
	if ( compare(pl, A[N]) <= 0 || compare(A[M], pr) <= 0 ) {
	  // ascertain that the corners are not empty
	  dflgm(A, N, M, e3, tpsc5, depthLimit, compare);
	  return;
	}

	// int z; // for tracing

	// initialize running indices
	i = N+1; j = M-1; lw = e3-1; up = e3+1;
	x = A[i]; A[i] = A[N]; i++; // create hole at A[N]

	  /* 
	  |)----------(--)-------------(|
	 N i         lw  up            j M
	  N+1 < x < i -> A[x] < pl
	  lw < x < up -> pl <= A[x] <= pr
	  j < x <= M -> pr < A[x]
	  */

  // if ( x < pl ) goto L0;
  if ( compare(x, pl) < 0 ) goto L0;
  // if ( pr < x ) goto R0;
  if ( compare(pr, x) < 0 ) goto R0;
  goto ML0;

 L0:  
   /*
      |---)-----(----)-------(----|
      N   i     lw  up       j    M
      x -> L
   */
  // while ( A[i] < pl ) i++;
  while ( compare(A[i], pl) < 0 ) i++;
  if ( i <= lw ) {
    y = A[i]; A[i++] = x; x = y;
    if ( lw < i ) { i--;
      // if ( pr < x ) goto L1R;
      if ( compare(pr, x) < 0 ) goto L1R;
      goto L1M;
    }
    // if ( pr < x ) goto R0;
    if ( compare(pr, x) < 0 ) goto R0;
    // x -> M
    goto ML0;
  }
  // lw < i
  i--;
  goto L1L;

 ML0:
   /*
      |---)-----(----)-------(----|
      N   i     lw  up       j    M
      x -> ML
   */
  // while ( pl <= A[lw] && A[lw] <= pr ) lw--;
  while ( compare(pl, A[lw]) <= 0 && compare(A[lw], pr) <= 0 ) lw--;
  if ( i <= lw ) {
    y = A[lw]; A[lw--] = x; x = y;
    if ( lw < i ) { i--;
      // if ( x < pl ) goto L1L;
      if ( compare(x, pl) < 0 ) goto L1L;
      goto L1R;
    }
    // if ( x < pl ) goto L0;
    if ( compare(x, pl) < 0 ) goto L0;
    // pr < x
    goto R0;
  }
  // lw < i
  i--;
  goto L1M;

 R0:
   /*
      |---)-----(----)-------(----|
      N   i     lw  up       j    M
      x -> R
   */
  // while ( pr < A[j] ) j--;
  while ( compare(pr, A[j]) < 0 ) j--;
  if ( up <= j ) {
    y = A[j]; A[j--] = x; x = y;
    if ( j < up ) { j++;
      // if ( x < pl ) goto R1L;
      if ( compare(x, pl) < 0 ) goto R1L;
      goto R1M;
    }
    // if ( x < pl ) goto L0;
    if ( compare(x, pl) < 0 ) goto L0;
    // x -> M
    goto MR0;
  }
  // j < up
  j++;
  goto R1R;
  
 MR0:
   /*
      |---)-----(----)-------(----|
      N   i     lw  up       j    M
      x -> MR
   */
  // while ( pl <= A[up] && A[up] <= pr ) up++;
  while ( compare(pl, A[up]) <= 0 && compare(A[up], pr) <= 0 ) up++;
  if ( up <= j ) {
    y = A[up]; A[up++] = x; x = y;
    if ( j < up ) { j++;
      // if ( pr < x ) goto R1R;
      if ( compare(pr, x) < 0 ) goto R1R;
      goto R1L;
    }
    // if ( pr < x ) goto R0;
    if ( compare(pr, x) < 0 ) goto R0;
    // x < pr
    goto L0;
  }
  // j < up
  j++;
  goto R1M;

  L1L: 
   /*
      |---]---------)-------(----|
      N   i        up       j    M
      x -> L
   */
  // while ( pl <= A[up] && A[up] <= pr ) up++;
  while ( compare(pl, A[up]) <= 0 && compare(A[up], pr) <= 0 ) up++;
  if ( up <= j ) {
    y = A[up]; A[up++] = A[++i]; A[i] = x; x = y;
    if ( j < up ) {
      // if ( x < pl ) goto L1Lx;
      if ( compare(x, pl) < 0 ) goto L1Lx;
      // x -> R
      A[N] = A[i]; A[i--] = A[j]; A[j] = x; 
      goto done;
    }
    // if ( x < pl ) { goto L1L; }
    if ( compare(x, pl) < 0 ) goto L1L; 
    // x -> R
    goto L1R;
  }
  // j < up
 L1Lx:
  j++; A[N] = x;
  goto done;

  L1R: 
   /*
      |---]---------)-------(----|
      N   i        up       j    M
      x -> R
   */
  // while ( pr < A[j] ) j--;
  while ( compare(pr, A[j]) < 0 ) j--;
  if ( up <= j ) {
    y = A[j]; A[j--] = x; x = y;
    if ( j < up ) {
      // if ( x < pl ) { j++; A[N] = x; 
      if ( compare(x, pl) < 0 ) { j++; A[N] = x; 
	goto done; 
      }
      // x -> M
      j++; A[N] = A[i]; A[i--] = x; 
      goto done;
    }
    if ( compare(x, pl) < 0 ) goto L1L;
    // x -> M
    goto L1M;
  }
  // j < up
  A[N] = A[i]; A[i] = A[j]; i--; A[j] = x; 
  goto done;

 L1M: 
   /*
      |---]---------)-------(----|
      N   i        up       j    M
      x -> M
   */
  // while ( pl <= A[up] && A[up] <= pr ) up++;
  while ( compare(pl, A[up]) <= 0 && compare(A[up], pr) <= 0 ) up++;
  if ( up <= j ) {
    y = A[up]; A[up++] = x; x = y;
    if ( j < up ) { 
      // if ( pr < x ) { 
      if ( compare(pr, x) < 0 ) {
	A[N] = A[i]; A[i--] = A[j]; A[j] = x; 
	goto done;
      }
      // x -> L
      j++; A[N] = x; 
      goto done;
    }
    // if ( pr < x ) goto L1R;
    if ( compare(pr, x) < 0 ) goto L1R;
    // x < pl
    goto L1L;
  }
  // j < up
  A[N] = A[i]; A[i] = x; i--; j++;
  goto done;

 R1R: 
   /*
      |---)---(-------------[----|
      N   i   lw            j    M
      x -> R
   */
  // while ( pl <= A[lw] && A[lw] <= pr ) lw--;
  while ( compare(pl, A[lw]) <= 0 && compare(A[lw], pr) <= 0 ) lw--;
  if ( i <= lw ) {
    y = A[lw]; A[lw--] = A[--j]; A[j] = x; x = y;
    if ( lw < i ) { i--;
      // if ( x < pl ) { A[N] = x; goto done; }
      if ( compare(x, pl) < 0 ) { A[N] = x; goto done; }
      // x -> R
      A[N] = A[i]; A[i--] = A[--j]; A[j] = x; 
      goto done;
    }
    // if ( x < pl ) goto R1L;
    if ( compare(x, pl) < 0 ) goto R1L;
    // pr < x
    goto R1R;
  }
  // lw < i
  A[N] = A[--i]; A[i] = A[--j]; i--; A[j] = x; 
  goto done;

 R1L: 
   /*
      |---)---(-------------[----|
      N   i   lw            j    M
      x -> L
   */
  // while ( A[i] < pl ) i++;
  while ( compare(A[i], pl) < 0 ) i++;
  if ( i <= lw ) {
    y = A[i]; A[i++] = x; x = y;
    if ( lw < i ) { i--;
      // if ( pr < x ) { 
      if ( compare(pr, x) < 0 ) {
	 A[N] = A[i]; A[i--] = A[--j]; A[j] = x; 
	 goto done;
      }
      // x -> M
      A[N] = A[i]; A[i--] = x; goto done;
    }
    // if ( pr < x ) goto R1R;
    if ( compare(pr, x) < 0 ) goto R1R;
    // x -> M
    goto R1M;
  }
  // lw < i
  i--; A[N] = x; 
  goto done;

 R1M: 
   /*
      |---)---(-------------[----|
      N   i   lw            j    M
      x -> M
   */
  // while ( pl <= A[lw] && A[lw] <= pr ) lw--;
  while ( compare(pl, A[lw]) <= 0 && compare(A[lw], pr) <= 0 ) lw--;
  if ( i <= lw ) {
    y = A[lw]; A[lw--] = x; x = y;
    if ( lw < i ) { i--;
      // if ( x < pl ) { A[N] = x; goto done; }
      if ( compare(x, pl) < 0 ) { A[N] = x; goto done; }
      // x -> R
      A[N] = A[i]; A[i--] = A[--j]; A[j] = x;
      goto done;
    }
    if ( compare(x, pl) < 0 ) goto R1L;
    // pr < x
    goto R1R;
  }
  // lw < i
  A[N] = A[--i]; A[i] = x; i--; // goto done;

 done:
    /*
      |---]---------[---------|
      N   i         j         M
    */

    /*
      for ( z = N+1; z <= i; z++ )
	// if ( pl <= A[z] ) {
        if ( compare(pl, A[z]) <= 0 ) {
	  printf("doneL z %i\n", z);
	  printf("N %i i %i lw %i up %i j %i M %i\n", N,i,lw,up,j,M);
	  exit(0);
	}
      for ( z = i+1; z < j; z++ )
	// if ( A[z] < pl || pr < A[z] ) {
        if ( compare(A[z], pl) < 0 || compare(pr, A[z]) < 0 ) {
	  printf("doneM z %i\n", z);
	  printf("N %i i %i lw %i up %i j %i M %i\n", N,i,lw,up,j,M);
	  exit(0);
	}
      for ( z = j; z <= M ; z++ )
	// if ( A[z] <= pr ) {
	if ( compare(A[z], pr) <= 0 ) {
	  printf("doneR z %i\n", z);
	  printf("N %i i %i lw %i up %i j %i M %i\n", N,i,lw,up,j,M);
	  exit(0);
	}
      */
	// tpsc5(A, N, i, depthLimit);
	// tpsc5(A, i+1, j-1, depthLimit);
	// tpsc5(A, j, M, depthLimit);
      if ( i-N < j-i ) {
	tpsc5(A, N, i, depthLimit, compare);
	if ( j-i < M-j ) {
	  tpsc5(A, i+1, j-1, depthLimit, compare);
	   N = j; goto Start;
	}
	tpsc5(A, j, M, depthLimit, compare);
	N = i+1; M = j-1;
	goto Start;
      }
      tpsc5(A, i+1, j-1, depthLimit, compare);
      if ( i-N < M-j ) {
	tpsc5(A, N, i, depthLimit, compare);
	N = j; goto Start;
      }
      tpsc5(A, j, M, depthLimit, compare);
      M = i; goto Start;
} // end tpsc5


// ************************************************************

// tps6 another three partition function
void tpsc6();
void tps6(void **A, int N, int M, int (*compare)()) {
  int L = M - N;

  if ( L < cut3Limit ) { 
    cut2f(A, N, M, compare);
    return;
  }
  int depthLimit = 2.5 * floor(log(L));
  tpsc6(A, N, M, depthLimit, compare);
} // end tps6

void tpsc6(void **A, int N, int M, int depthLimit, int (*compare)()) 
{
  // register int k; // for-index
  // int z; // for tracing
  register int i, j, up, lw; // indices
  register void *x, *y; // array values
  register void *pl, *pr; // pivots

 // A 3d recursive call is avoided by jumping back to Start.  
 Start:
  // printf("tspc6 N %d M %d depthLimit %d\n", N,M,depthLimit);

  if ( depthLimit <= 0 ) { // avoid quadradic explosion
    heapc(A, N, M, compare);
    return;
  }
  int L = M - N;
  if ( L < cut3Limit ) {
    cut2fc(A, N, M, depthLimit,compare);
    //    quicksort0c(A, N, M, depthLimit, compare);
    return;
  }
  depthLimit--;

        int sixth = (L + 1) / 6;
        int e1 = N  + sixth;
        int e5 = M - sixth;
        int e3 = N + L/2; // (N+M) / 2; // The midpoint
        int e4 = e3 + sixth;
        int e2 = e3 - sixth;
        // Sort these elements using a 5-element sorting network
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

   void tpsc6();
   // if ( ae2 == ae3 || ae3 == ae4 ) {
   if ( compare(ae2, ae3) == 0 || compare(ae3, ae4) == 0 ) {
     dflgm(A, N, M, e3, tpsc6, depthLimit, compare);
     return;
   }
   // Fix end points
   // iswap(N+1, e2, A);
   { int xx = N+1; iswap(xx, e2, A); }
   iswap(M, e4, A);

   pl = A[N+1]; pr = A[M]; // left and right pivots

   // initialize running indices
   i = N+1; j = M; lw = e3-1; up = e3+1;
   x = A[N]; // create hole at A[N]

   /* 
      |]----------(--)-------------[|
      N i         lw  up            j M
      N+1 < x <= i -> A[x] <= pl
      lw < x < up -> pl < A[x] < pr
      j <= x <= M -> pr <= A[x]
   */

   // if ( x <= pl ) goto LgMgRL;
   if ( compare(x, pl) <= 0 ) goto LgMgRL;
   // if ( pr <= x ) goto LgMgRR;
   if ( compare(pr, x) <= 0 ) goto LgMgRR;
   int l,r;

 LgMgRM1:
   /* 
      |]----------(--)-------------[|
      N i         lw  up            j M
      x -> ML
   */
   while ( (l = compare(pl, A[lw]) ) < 0 && compare(A[lw], pr < 0 ) ) lw--;
   if ( i < lw ) {
     y = A[lw]; A[lw--] = x; x = y;
     if ( 0 <= l ) goto LgMgRL;
     goto LgMgRR;
   } else // lw = i
     goto LMgRM;

 LgMgRL:
   /* 
      |]----------(--)-------------[|
      N i         lw  up            j M
      x -> L
   */
   while ( (compare(A[++i], pl)) <= 0 )
     if ( i <= lw ) {
       y = A[i]; A[i] = x; x = y;
       if ( compare(pr, x) <= 0 ) goto LgMgRR;
       goto LgMgRM1;
     } else { // lw < i
       i--; goto LMgRL;
     }

 LgMgRR:
  /* 
      |]----------(--)-------------[|
      N i         lw  up            j M
      x -> R
   */
   while ( (compare(pr,  A[--j])) <= 0 )
     if ( up <= j ) {
       y = A[j]; A[j] = x; x = y;
       if ( compare(x, pl) <= 0 ) goto LgMgRL;
       goto LgMgRM2;
     } else { // j < up
       j++; goto LgMRR;
     }


 LgMgRM2:
   /* 
      |]----------(--)-------------[|
      N i         lw  up            j M
      x -> ML
   */
   while ( compare(pl, A[up]) < 0 && (r = compare(A[up], pr < 0 )) ) up++;
   if ( up < j ) {
     y = A[up]; A[up++] = x; x = y;
     if ( 0 <= r ) goto LgMgRR;
     goto LgMgRL;
   } else // up = j
     goto LgMRM;

 LMgRM:
   /* 
      |]------------)----------[------|
      N i           up         j      M
      x -> M
   */
   while ( compare(pl, A[up]) < 0 && (r = compare(A[up], pr < 0 )) ) up++;
   if ( up < j ) {
     y = A[up]; A[up++] = x; x = y;
     if ( 0 <= r ) goto LMgRR;
     goto LMgRL;
   } else { // up = j
     A[N] = A[i]; A[i--] = x;
     goto done;
   }

 LMgRL:
  /* 
      |]------------)----------[------|
      N i           up         j      M
      x -> L
   */
   while ( compare(pl, A[up]) < 0 && (r = compare(A[up], pr < 0 )) ) up++;
   if ( up < j ) {
     y = A[up]; A[up++] = A[++i]; A[i] = x; x = y;
     if ( 0 <= r ) goto LMgRR;
     goto LMgRL;
   } else { // up = j
     A[N] = x;
     goto done;
   }  

 LMgRR:
  /* 
      |]------------)----------[------|
      N i           up         j      M
      x -> R
   */
   while ( (compare(pr,  A[--j])) <= 0 )
     if ( up <= j ) {
       y = A[j]; A[j] = x; x = y;
       if ( compare(x, pl) <= 0 ) goto LMgRL;
       goto LMgRM;
     } else { // j < up
       A[N] = A[i]; A[i--] = A[j]; A[j] = x;
       goto done;
     }

 LgMRR:
 /* 
      |]----------(--[--------------|
      N i         lw j              M
      x -> R
   */
   while ( (l = compare(pl, A[lw]) ) < 0 && compare(A[lw], pr < 0 ) ) lw--;
   if ( i < lw ) {
     y = A[lw]; A[lw--] = A[--j]; A[j] = x; x = y;
     if ( 0 <= l ) goto LgMRL;
     goto LgMRR;
   } else { // lw = i
     A[N] = A[i]; A[i--] = A[j]; A[j] = x;
     goto done;
   }

 LgMRM:
  /* 
      |]----------(--[--------------|
      N i         lw j              M
      x -> M
   */
  while ( (l = compare(pl, A[lw]) ) < 0 && compare(A[lw], pr < 0 ) ) lw--;
   if ( i < lw ) {
     y = A[lw]; A[lw--] = x; x = y;
     if ( 0 <= l ) goto LgMRL;
     goto LgMRR;
   } else { // lw = i
     A[N] = A[i]; A[i--] = x;
     // goto done; // fall through
   }

LgMRL:
  /* 
      |]----------(--[--------------|
      N i         lw j              M
      x -> L
   */
  while ( (compare(A[++i], pl)) <= 0 )
     if ( i <= lw ) {
       y = A[i]; A[i] = x; x = y;
       if ( compare(pr, x) <= 0 ) goto LgMRR;
       goto LgMRM;
     } else { // lw < i
       A[N] = x;
       // fall through
     }

 done:
    /*
      |---]---------[---------|
      N   i         j         M
    */

    /*
      for ( z = N; z <= i; z++ )
	// if ( pl < A[z] ) {
        if ( compare(pl, A[z]) < 0 ) {
	  printf("doneL z %i\n", z);
	  printf("N %i i %i lw %i up %i j %i M %i\n", N,i,lw,up,j,M);
	  exit(0);
	}
      for ( z = i+1; z < j; z++ )
	// if ( A[z] <= pl || pr <= A[z] ) {
        if ( compare(A[z], pl) <= 0 || compare(pr, A[z]) <= 0 ) {
	  printf("doneM z %i\n", z);
	  printf("N %i i %i lw %i up %i j %i M %i\n", N,i,lw,up,j,M);
	  exit(0);
	}
      for ( z = j; z <= M ; z++ )
	// if ( A[z] < pr ) {
	if ( compare(A[z], pr) < 0 ) {
	  printf("doneR z %i\n", z);
	  printf("N %i i %i lw %i up %i j %i M %i\n", N,i,lw,up,j,M);
	  exit(0);
	}
      */
	// tpsc5(A, N, i, depthLimit);
	// tpsc5(A, i+1, j-1, depthLimit);
	// tpsc5(A, j, M, depthLimit);
      if ( i-N < j-i ) {
	tpsc6(A, N, i, depthLimit, compare);
	if ( j-i < M-j ) {
	  tpsc6(A, i+1, j-1, depthLimit, compare);
	   N = j; goto Start;
	}
	tpsc6(A, j, M, depthLimit, compare);
	N = i+1; M = j-1;
	goto Start;
      }
      tpsc6(A, i+1, j-1, depthLimit, compare);
      if ( i-N < M-j ) {
	tpsc6(A, N, i, depthLimit, compare);
	N = j; goto Start;
      }
      tpsc6(A, j, M, depthLimit, compare);
      M = i; goto Start;
} // end tpsc6



// ************************************************************

/* // These procedures need, if used, comparisonn arguments ... 
void heapSort();
void heapc(void **A, int N, int M) {
  // printf("heapc: %d %d\n", N, M);
  if ( M <= N ) return;
  heapSort(&A[N], M-N+1);
} // end heapc

void heapify();
void siftDown();
void heapSort(void **a, int count) {
  // input:  an unordered array a of length count
  // first place a in max-heap order
  heapify(a, count);
  // in languages with zero-based arrays the children are 2*i+1 and 2*i+2
  int end = count-1; 
  while ( end > 0 ) {
    // (swap the root(maximum value) of the heap with 
    // the last element of the heap)
    // swap(a[end], a[0]);
    iswap(end, 0, a);
    // (decrease the size of the heap by one so that the 
    // previous max value will stay in its proper placement) 
    end = end - 1;
    // (put the heap back in max-heap order)
    siftDown(a, 0, end);
  }
} // end heapSort
         
void heapify(void **a, int count) {
  // (start is assigned the index in a of the last parent node)
  int start = (count - 2) / 2;
  while ( 0 <= start ) {
    // (sift down the node at index start to the proper place such 
    // that all nodes below the start index are in heap order)
    siftDown(a, start, count-1);
    start = start - 1;
  } // (after sifting down the root all nodes/elements are in heap order)
} // end heapify
 
void siftDown(void **a, int start, int end) {
  // input:  end represents the limit of how far down the heap to sift.
  int root = start;
  int child, swapi;
  // (While the root has at least one child)
  while ( root * 2 + 1 <= end ) {
    child = root * 2 + 1; // (root*2 + 1 points to the left child)
    swapi = root; // (keeps track of child to swap with)
    // (check if root is smaller than left child)
    // if ( a[swapi] < a[child] ) 
    if ( compare(a[swapi], a[child]) < 0 ) 
      swapi = child;
    // (check if right child exists, and if it's bigger 
    // than what we're currently swapping with)
    // if ( child+1 <= end && a[swapi] < a[child+1] )
    if ( child+1 <= end && compare(a[swapi],a[child+1]) < 0 )
      swapi = child + 1;
    // (check if we need to swap at all)
    if ( swapi != root ) {
      // swap(a[root], a[swapi]);
      iswap(root, swapi, a);
      root = swapi; // (repeat to continue sifting down the child now)
    } else return;
  }
} // end siftDown
*/

// ************************************************************************

void dpqSort();
void dpq(void **A, int N, int M, int (*compare)()) {
  dpqSort(A, N, M, compare);
} // end dpq


int INSERTION_SORT_THRESHOLD = 47;

    /**
     * Sorts the specified range of the array into ascending order. This
     * method differs from the public {@code sort} method in that the
     * {@code right} index is inclusive, and it does no range checking on
     * {@code left} or {@code right}.
     *
     * @param a the array to be sorted
     * @param left the index of the first element, inclusive, to be sorted
     * @param right the index of the last element, inclusive, to be sorted
     */
    // private static void doSort(int[] a, int left, int right) {
void dualPivotQuicksort();
void dpqSort(void **a, int left, int right,
	  int (*compar ) (const void *, const void * ) ) {
        // Use insertion sort on tiny arrays
        if (right - left + 1 < INSERTION_SORT_THRESHOLD) {
	  int k, j;
	  void *ak;
            for (k = left + 1; k <= right; k++) {
                ak = a[k];
                for (j = k - 1; j >= left &&
		       // ak < a[j]; 
		       compar(  ak,  a[j] ) < 0;
		     j--) {
                    a[j + 1] = a[j];
                }
                a[j + 1] = ak;
            }
        } else { // Use Dual-Pivot Quicksort on large arrays
	  dualPivotQuicksort(a, left, right, compar);
        }
} // end dpqSort

    /**
     * Sorts the specified range of the array into ascending order by the
     * Dual-Pivot Quicksort algorithm.
     *
     * @param a the array to be sorted
     * @param left the index of the first element, inclusive, to be sorted
     * @param right the index of the last element, inclusive, to be sorted
     */
    // private static void dualPivotQuicksort(int[] a, int left, int right) {
void dualPivotQuicksort(void **a, int left, int right,
		   int (*compar ) (const void *, const void * ) ) {
  int length = right-left;

  // Compute indices of five evenly spaced elements
  int sixth = (right - left + 1) / 6;
  int e1 = left  + sixth;
  int e5 = right - sixth;
  int e3 = left + (length>>1); // (left + right)/ 2; // The midpoint
  int e4 = e3 + sixth;
  int e2 = e3 - sixth;

  // Sort these elements using a 5-element sorting network
  void *ae1 = a[e1], *ae2 = a[e2], *ae3 = a[e3], *ae4 = a[e4], *ae5 = a[e5];
  void *t;
  // if (ae1 > ae2) 
  if ( compar(  ae1,  ae2 ) > 0 )
    { t = ae1; ae1 = ae2; ae2 = t; }
  // if (ae4 > ae5) 
  if ( compar(  ae4,  ae5 ) > 0 )
    { t = ae4; ae4 = ae5; ae5 = t; }
  // if (ae1 > ae3) 
  if ( compar(  ae1,  ae3 ) > 0 )
    { t = ae1; ae1 = ae3; ae3 = t; }
  // if (ae2 > ae3) 
  if ( compar(  ae2,  ae3 ) > 0 )
    { t = ae2; ae2 = ae3; ae3 = t; }
  // if (ae1 > ae4) 
  if ( compar(  ae1,  ae4 ) > 0 )
    { t = ae1; ae1 = ae4; ae4 = t; }
  // if (ae3 > ae4) 
  if ( compar(  ae3,  ae4 ) > 0 )
    { t = ae3; ae3 = ae4; ae4 = t; }
  // if (ae2 > ae5) 
  if ( compar(  ae2,  ae5 ) > 0 )
    { t = ae2; ae2 = ae5; ae5 = t; }
  // if (ae2 > ae3) 
  if ( compar(  ae2,  ae3 ) > 0 )
    { t = ae2; ae2 = ae3; ae3 = t; }
  // if (ae4 > ae5) 
  if ( compar(  ae4,  ae5 ) > 0 )
    { t = ae4; ae4 = ae5; ae5 = t; }

  a[e1] = ae1; a[e2] = ae2; a[e3] = ae3; a[e4] = ae4; a[e5] = ae5;

  // Pointers
  int less  = left;  // The index of the first element of center part
  int great = right; // The index before the first element of right part

  /*
   * Use the second and fourth of the five sorted elements as pivots.
   * These values are inexpensive approximations of the first and
   * second terciles of the array. Note that pivot1 <= pivot2.
   *
   * The pivots are stored in local variables, and the first and
   * the last of the sorted elements are moved to the locations
   * formerly occupied by the pivots. When partitioning is complete,
   * the pivots are swapped back into their final positions, and
   * excluded from subsequent sorting.
   */
  // if a[e1] != a[e2] && a[e2] != a[e3] && a[e3] != a[e4] && a[e4] != a[e5]) {
  if ( compar( ae1, ae2 ) != 0 &&
       compar( ae2, ae3 ) != 0 &&
       compar( ae3, ae4 ) != 0 &&
       compar( ae4, ae5 ) != 0 ) {

    void *pivot1 = ae2; 
    void *pivot2 = ae4; 
    
    /*
     * The first and the last elements to be sorted are moved to the
     * locations formerly occupied by the pivots. When partitioning
     * is complete, the pivots are swapped back into their final
     * positions, and excluded from subsequent sorting.
     */
    a[e2] = a[left];
    a[e4] = a[right];

    /*
     * Skip elements, which are less or greater than pivot values.
     */
    // while (a[++less] < pivot1);
    while ( compar(a[++less], pivot1) < 0 );
    // while (a[--great] > pivot2);
    while ( compar(a[--great], pivot2) > 0 );
  
    /*
     * Partitioning:
     *
     *   left part         center part                  right part
     * ------------------------------------------------------------
     * [ < pivot1  |  pivot1 <= && <= pivot2  |   ?   |  > pivot2 ]
     * ------------------------------------------------------------
     *              ^                          ^     ^
     *              |                          |     |
     *             less                        k   great
     *
     * Invariants:
     *
     *              all in (left, less)   < pivot1
     *    pivot1 <= all in [less, k)     <= pivot2
     *              all in (great, right) > pivot2
     *
     * Pointer k is the first index of ?-part
     */
    int k;
    void *ak;
  outer:
    for (k = less; k <= great; k++) {
      ak = a[k];
      // if (ak < pivot1) {
      if ( compar(  ak,  pivot1 ) < 0 ) {
	a[k] = a[less];
	a[less] = ak;
	less++;
      } else 
	// if (ak > pivot2) {
	if ( compar(  ak,  pivot2 ) > 0 ) {
	  // while (a[great] > pivot2) {
	  while ( compar(  a[great],  pivot2 ) > 0 ) {
	    if (great-- == k) {
	      // break outer;
	      goto outer;
	    }
	  }
	  // if (a[great] < pivot1) {
	  if ( compar(  a[great],  pivot1 ) < 0 ) {
	    a[k] = a[less];
	    a[less++] = a[great];
	  } else { // pivot1 <= a[great] <= pivot2
	    a[k] = a[great];
	  }
	  a[great--] = ak;
	}
    }
  
    // Swap pivots into their final positions
    a[left]  = a[less  - 1]; a[less  - 1] = pivot1;
    a[right] = a[great + 1]; a[great + 1] = pivot2;
    
    // Sort left and right parts recursively, excluding known pivots
    dpqSort(a, left, less - 2, compar);
    dpqSort(a, great + 2, right, compar);
    /* Tests suggest (DdC) to simply sort here the middle segment with:
       dpqSort(a, less, great);
       return;
    */
    
    /*
     * If center part is too large (comprises > 4/7 of the array),
     * swap internal pivot values to ends.
     */
    if (less < e1 && e5 < great) {
      /*
       * Skip elements, which are equal to pivot values.
       */
      // while (a[less] == pivot1) {
      while (compar( a[less], pivot1 ) == 0) {
	++less;
      }
      //  while (a[great] == pivot2) {
      while (compar( a[great], pivot2 ) == 0) {
	--great;
      }
      
      /*
       * Partitioning:
       *
       *   left part         center part                  right part
       * +----------------------------------------------------------+
       * | == pivot1 |  pivot1 < && < pivot2  |    ?    | == pivot2 |
       * +----------------------------------------------------------+
       *              ^                        ^       ^
       *              |                        |       |
       *             less                      k     great
       *
       * Invariants:
       *
       *              all in (*,  less) == pivot1
       *     pivot1 < all in [less,  k)  < pivot2
       *              all in (great, *) == pivot2
       *
       * Pointer k is the first index of ?-part.
       */
      int k; void* ak;
    outer2:
      for (k = less - 1; ++k <= great; ) {
	ak = a[k];
	// if (ak == pivot1) { // Move a[k] to left part
	if ( compar(ak, pivot1) == 0 ) { 
	  a[k] = a[less];
	  a[less] = ak;
	  ++less;
	} else 
	  // if (ak == pivot2) { // Move a[k] to right part
	  if ( compar(ak, pivot2) == 0 ) {
	    // while (a[great] == pivot2) {
	    while (compar(a[great], pivot2) == 0) {
	      if (great-- == k) {
		// break outer;
		goto outer2;
	      }
	    }
	    // if (a[great] == pivot1) { // a[great] < pivot2  
	    if ( compar(a[great], pivot1) == 0 ) { 
	      a[k] = a[less];
	      /*
	       * Even though a[great] equals to pivot1, the
	       * assignment a[less] = pivot1 may be incorrect,
	       * if a[great] and pivot1 are floating-point zeros
	       * of different signs. Therefore in float and
	       * double sorting methods we have to use more
	       * accurate assignment a[less] = a[great].
	       */
	      // a[less] = pivot1;
	      a[less] = a[great]; // ++++++++++++++++++++++++++++++++ !!
	      ++less;
	    } else { // pivot1 < a[great] < pivot2
	      a[k] = a[great];
	    }
	    a[great] = ak;
	    --great;
	  }
      }
    }
    
    // Sort center part recursively
    dpqSort(a, less, great, compar);
    
  } else { // Partitioning with one pivot
    /*
     * Use the third of the five sorted elements as pivot.
     * This value is inexpensive approximation of the median.
     */
    void* pivot = a[e3];
    
    /*
     * Partition degenerates to the traditional 3-way
     * (or "Dutch National Flag") partition:
     *
     *   left part   center part            right part
     * -------------------------------------------------
     * [  < pivot  |  == pivot  |    ?    |  > pivot   ]
     * -------------------------------------------------
     *              ^            ^       ^
     *              |            |       |
     *             less          k     great
     *
     * Invariants:
     *
     *   all in (left, less)   < pivot
     *   all in [less, k)     == pivot
     *   all in (great, right) > pivot
     *
     * Pointer k is the first index of ?-part
     */
    int k;
    void *ak;
    for (k = less; k <= great; k++) {
      ak = a[k];
      // if (ak == pivot) {
      if ( compar(  ak,  pivot ) == 0 ) {
	continue;
      }
      // if (ak < pivot1) {
      if ( compar(  ak,  pivot ) < 0 ) {
	a[k] = a[less];
	a[less] = ak;
	less++;
      } else { // ak > pivot1
	/*
	 * We know that pivot1 == a[e3] == pivot2. Thus, we know
	 * that great will still be >= k when the following loop
	 * terminates, even though we don't test for it explicitly.
	 * In other words, a[e3] acts as a sentinel for great.
	 */
	// while (a[great] > pivot) {
	while ( compar(  a[great],  pivot ) > 0 ) {
	  great--;
	}
	// if (a[great] < pivot) {
	if ( compar(  a[great],  pivot ) < 0 ) {
	  a[k] = a[less];
	  a[less++] = a[great];
	} else { // a[great] == pivot
	  // a[k] = pivot;
	  a[k] = a[great]; // +++++++++++++++++++++++++++++++ !!
	}
	a[great--] = ak;
      }
    }
    /*
     * Sort left and right parts recursively.
     * All elements from center part are equal
     * and, therefore, already sorted.
     */
    dpqSort(a, left, less - 1, compar);
    dpqSort(a, great + 1, right, compar);
  }
} // end dualPivotQuicksort


// ********************** MPQ *********************************

void iswap3(int p, int q, void **A) {
  void *t = A[p];
  A[p] = A[q];
  A[q] = t;
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
	      iswap(a, b, A); 
              a++;
           }
           b++;
        }
        // while ( A[c] > q && b <= c ) {
	while ( compar(A[c], q) > 0 && b <= c ) {
	   // if ( A[c] > r ) {
	   if ( compar(A[c], r) > 0 ) {
              // swap(A[c], A[d]);
	      iswap(c, d, A); 
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
		iswap(a, b, A); iswap(a, c, A);   		    
                a++;
             } else
                // swap(A[b],A[c]);
		iswap(c, b, A);
             // swap(A[c],A[d]); 
	     iswap(c, d, A); 
             b++; c--; d--;
          } else {
             // if ( A[c] < p ) {
	     if ( compar(A[c], p) < 0 ) {
                // swap(A[b],A[a]); swap(A[a],A[c]);
		iswap(a, b, A); iswap(a, c, A);   
                a++;
             } else
                // swap(A[b],A[c]);
		iswap(b, c, A);  
             b++; c--;
          }
        }
    } 
    a--; b--; c++; d++;
    // swap(A[left + 1],A[a]); swap(A[a],A[b]);
    iswap3(left+1, a, A); iswap(a, b, A);
    a--;
    // swap(A[left],A[a]); swap(A[right],A[d]);
    iswap(left, a, A); iswap(right, d, A);
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

// ************************************************************************



// c:/bsd/rigel/sort/Qusort adjusted for other pivot 
// Date: Fri Jan 31 13:32:12 2014/ Tue May 19 15:02:00 2015, 2017
// (C) OntoOO/ Dennis de Champeaux

// #include "Isort.c" // insertionsort member
// #include "Hsort.c" // heapsort member

// calculate the median of 3
/*
int med(void **A, int a, int b, int c,
	int (*compareXY ) (const void *, const void * ) ) {
  return
    compareXY( A[a], A[b] ) < 0 ?
    ( compareXY( A[b], A[c] ) < 0 ? b : compareXY( A[a], A[c] ) < 0 ? c : a)
    : compareXY( A[b], A[c] ) > 0 ? b : compareXY( A[a], A[c] ) > 0 ? c : a;
} // end med
*/

void quicksort0ck();
// Quicksort function for invoking quicksort0ck.
void quicksort0k(void **A, int N, int M, int (*compare)()) {
  int L = M - N;
  if ( L <= 0 ) return;
  // printf("quicksort0 %d %d \n", N, M);
  if ( L <= 7 ) { 
    insertionsort(A, N, M, compare);
    return;
  }
  int depthLimit = 2.5 * floor(log(L));
  quicksort0ck(A, N, M, depthLimit, compare);
} // end quicksort0

// void dflgm();
// selectPivotByPseudoMedian = sPBPM
int sPBPM2(void **A, int N, int M, int (*compare)()) {
  int s = 50; // 1 <= s
  int L = M - N;
  if ( L <= s ) return L/2;
  int oneThird = L/3;
  int a0 = sPBPM2(A, N, N-1+oneThird, compare);
  int a1 = sPBPM2(A, N+oneThird, M-1-oneThird, compare);
  int a2 = sPBPM2(A, M-oneThird, M, compare);
  return med(A, a0, a1, a2, compare); 
}

// Quicksort equipped with a defense against quadratic explosion;
// calling heapsort if depthlimit exhausted
void quicksort0ck(void **A, int N, int M, int depthLimit, int (*compareXY)()) {
  // printf("Enter quicksort0c N: %d M: %d %d\n", N, M, depthLimit);
  while ( N < M ) {
    int L = M - N;
    if ( L <= 7 ) {
      insertionsort(A, N, M, compareXY);
      return;
    }
    if ( depthLimit <= 0 ) {
      heapc(A, N, M, compareXY);
      return;
    }
    depthLimit--;

    /*
    // 7 < L
    int p0 = N + (L>>1); // N + L/2;
    int pn = N;
    int pm = M;
    if ( 40 < L ) {
      int d = L>>3; // L/8;
      pn = med(A, pn, pn + d, pn + 2 * d, compareXY);
      p0 = med(A, p0 - d, p0, p0 + d, compareXY);
      pm = med(A, pm - 2 * d, pm - d, pm, compareXY);
 
    }
    p0 = med(A, pn, p0, pm, compareXY);
    */
    int p0 =  sPBPM2(A, N, M, compareXY);

    /* optional check when inputs have many equal elements
    if ( compareXY(A[N], A[M]) == 0 ) {
      dflgm(N, M, p0, quicksort0c, depthLimit,, compareXY);
      return;
    } */
    // p0 is index to 'best' pivot ...
    
    iswap(N, p0, A); // ... and is put in first position

    register void *p = A[N]; // pivot
    register int i, j;
    i = N;
    j = M;
    register void *ai; void *aj;

    /* Split array A[N,M], N<M in two segments using pivot p; 
       construct a partition with A[N,i), A(i,M] and N <= i <= M, and
       N <= k <= i -> A[k] <= p  and  i < k <= M -> p < A[k];
       Allow the worse cases: N=i or i=M.
       Recurse on A[N,i) and A(i,M) (or in the reverse order).
       This code does NOT do swapping; instead it disposes 
       ai/aj in a hole created by setting aj/ai first.  
    */
    /* Start state:
	  |-------------------------------|
          N=i                           j=M
	  N = i < j = M
          N <= k < i -> A[k] <= p    
          N < j < k <= M -> p < A[k]
	  A[N] = p
          N < i -> p < A[i]
    */

    while ( i < j ) {
      /*
	  |-------o---------------(--------|
          N       i               j        M
	  N <= i < j <= M
          N <= k < i -> A[k] <= p    
          N < j < k <= M -> p < A[k]
	  A[N] <= p
          N < i -> p < A[i]
          p + A[N,i) + A(i,j] + A(j,M] is a permutation of the input array
      */
      aj = A[j];
      while ( compareXY(p, aj) < 0 ) { 
        /*
	  |-------o---------------[--------|
          N       i               j        M
	  N = i < j <= M or N < i <= j <= M
          N <= k < i -> A[k] <= p    
          N < j <= k <= M -> p < A[k]
	  A[N] <= p
	  N < i -> p < A[i}
	  p + A[N,i) + A(i,M] is a permutation of the input array
          p < aj = A[j]
	*/
	j--; 
	aj = A[j]; 
      }
      /*
	  |-------o---------------(--------|
          N       i               j        M
	  N = i = j < M or N < i & = i-1 <= j <= M or N <= i < j <= M
          N <= k < i -> A[k] <= p    
          j < k <= M -> p < A[k]
	  A[N] <= p
	  N < i -> p < A[i}
	  p + A[N,i) + A(i,M] is a permutation of the input array
          aj = A[j] <= p
	*/
      if ( j <= i ) {
	/*
	  |-------o-----------------------|
          N       i                       M
	  N = i = j < M or N < i & = i-1 = j < M
          N <= k < i -> A[k] <= p    
          i < k <= M -> p < A[k]
	  A[N] <= p
	  p + A[N,i) + A(i,M] is a permutation of the input array
	*/
	break;
      }
      // i < j 
      A[i] = aj; // fill hole !
      /*
	  |-------]---------------o--------|
          N       i               j        M
	  N <= i < j <= M
          N <= k <= i -> A[k] <= p    
          j < k <= M -> p < A[k]
	  A[N] <= p
	  p + A[N,j) + A(j,M] is a permutation of the input array
	  aj = A[j] <= p
      */
      i++; ai = A[i];
      while ( i < j && compareXY(ai, p) <= 0 ) {
	/*
	  |-------]---------------o--------|
          N       i               j        M
	  N < i < j <= M
          N <= k <= i -> A[k] <= p    
          j < k <= M -> p < A[k]
	  A[N] <= p
	  p + A[N,j) + A(j,M] is a permutation of the input array
	  aj = A[j] <= p
	*/
	i++; ai = A[i]; 
      }
      if ( j <= i ) {
	/*
	  |----------------------o--------|
          N                     i=j       M
	  N < i = j <= M
          N <= k < i -> A[k] <= p    
          j < k <= M -> p < A[k]
	  A[N] <= p
	  p + A[N,j) + A(j,M] is a permutation of the input array
	*/
	break;
      }
      // i < j  & p < ai = A[i] 
      A[j] = ai;
      j--;
      /*
	  |--------o--------------[--------|
          N        i              j        M
	  N < i <= j <= M
          N <= k < i -> A[k] <= p    
          j <= k <= M -> p < A[k]
	  A[N] <= p
          N < i -> p < ai = A[i]
	  p + A[N,i) + A(i,M] is a permutation of the input array
      */
    } // end of while( i < j )

    A[i] = p;
    /*
	  |--------]----------------------|
          N        i                      M
	  N <= i <= M
          N <= k <= i -> A[k] <= p    
          i < k <= M -> p < A[k]
	  A[N] <= p
	  A[N,i] + A(i,M] is a permutation of the input array
    */

    // Recurse on the smallest one and iterate on the other one
    int ia = i-1; int ib = i+1; 
    if ( i-N < M-i ) { 
      if ( N < ia ) quicksort0c(A, N, ia, depthLimit, compareXY);  
      N = ib; 
    } else { 
      if ( ib < M ) quicksort0c(A, ib, M, depthLimit, compareXY);  
      M = ia; 
    }
  }
} // end of quicksort0ck

// ************************************************************************

void blockSortc();
const int BLOCKSIZE = 128;
void blockSort(void **A, int N, int M, int (*compar )) {
  int L = M-N;
  if (L <= 0) return;
  int dp = 2.5 * floor(log(L));
  int indexL[BLOCKSIZE], indexR[BLOCKSIZE];
  blockSortc(A, N, M, indexL, indexR, dp, compar);
}

void blockSortc(void **A, int N, int M, int indexL[], int indexR[],
	   int depthLimit, int (*compareXY)()) {
  int L;
 again:
  // printf("blockSortc N %i M %i\n", N, M);
  L = M - N;
  // if ( L <= 0 ) return;
  if ( L <= 10 ) { 
    insertionsort(A, N, M, compareXY);
    return;
  }

  if ( depthLimit <= 0 ) {
    heapc(A, N, M, compareXY);
    return;
  }
  depthLimit--;

    // 10 < L
    // grab median of 3 or 9 to get a good pivot
    int pm = N + L/2; // (N+M)/2;
    int pl = N;
    int pn = M;
    if (L > 40) { // do median of 9
      int d = L/8;
      pl = med(A, pl, pl + d, pl + 2 * d, compareXY);
      pm = med(A, pm - d, pm, pm + d, compareXY);
      pn = med(A, pn - 2 * d, pn - d, pn, compareXY);
    }
    pm = med(A, pl, pm, pn, compareXY); // pm is index to 'best' pivot ...

    // iter last = end - 1 ;
    int last = M, begin = N;
    // std::iter_swap (pivot_position, last);
    iswap(pm, last, A);
    // const typename std::iterator_traits <iter >::value_type & pivot = *last;
    void *pivot = A[last];
    int pivot_position = last;
    last --;
    
    int num_left = 0;
    int num_right = 0;
    int start_left = 0;
    int start_right = 0;
    int num;
    int j; // for the for-loops 

    // main loop
    while (last - begin + 1 > 2 * BLOCKSIZE)
      { 
       //Compare and store in buffers
          if ( num_left == 0 ) {
              start_left = 0;
              for (j = 0; j < BLOCKSIZE; j++) {
                  indexL[num_left] = j;
                  // num_left += ( !( less( begin[j], pivot ) ) );
		  // num_left += ( !( less( A[begin+j], pivot ) ) );
		  // num_left += ( !( compareXY(A[begin+j], pivot) < 0 ) );
		  num_left += ( compareXY(A[begin+j], pivot) >= 0 );
              }
          }
          if ( num_right == 0 ) {
            start_right = 0;
            for (j = 0; j < BLOCKSIZE; j++) {
                indexR [ num_right ] = j;
                // num_right += ! ( less( pivot , *( last - j ) ) );
                // num_right += ! ( less( pivot , A[last-j] ) );
		// num_right += ! ( compareXY(pivot, A[last-j]) < 0 );
		num_right += ( compareXY(pivot, A[last-j]) >= 0 );
	    }
	  }
	  // rearrange elements
	  // num = std::min ( num_left , num_right );
	  int num = ( num_left <= num_right ? num_left : num_right );
	  for (j = 0; j < num; j++)
	    // std::iter_swap ( begin + indexL[start_left + j], 
	    //	                last - indexR[start_right + j] );
	    iswap3(begin + indexL[start_left + j], 
		  last - indexR[start_right + j], A);
  
        num_left -= num;
        num_right -= num;
        start_left += num;
        start_right += num;
        begin += ( num_left == 0 ) ? BLOCKSIZE : 0;
        last -= ( num_right == 0 ) ? BLOCKSIZE : 0;
  
        } // end main l o o p
  
        // Compare and store in buffers final iteration
        int shiftR = 0, shiftL = 0;
        if ( num_right == 0 && num_left == 0 ) { 
            // for small arrays or in the unlikely
            // case that both buffers are empty
            shiftL = ((last - begin) + 1 ) / 2 ;
            shiftR = (last - begin) + 1 - shiftL;
            start_left = 0; start_right = 0;
            for (j = 0; j < shiftL; j++) {
                indexL[ num_left ] = j;
                // num_left += ( ! less( begin[j], pivot ) );
		num_left += ( compareXY(A[begin+j], pivot) >= 0 );
                indexR [ num_right ] = j;
                // num_right += ! less( pivot, *( last - j ) );
		num_right += ( compareXY(pivot, A[last-j]) >= 0 );
            }
            if ( shiftL < shiftR )
            {
                indexR [ num_right ] = shiftR - 1;
                // num_right += ! less( pivot, *( last - shiftR + 1 ) );
		num_right += ( compareXY(pivot, A[last - shiftR + 1]) >= 0 );
            }
         }
         else if ( num_right != 0 ) {
             shiftL = ( last - begin ) - BLOCKSIZE + 1;
             shiftR = BLOCKSIZE;
              start_left = 0;
              for (j = 0; j < shiftL; j++) {
                  indexL [ num_left ] = j;
                  // num_left += ( ! less( begin[ j ] , pivot ) );
		  num_left += ( compareXY(A[begin+j], pivot) >= 0 );
              }
         }
         else {
             shiftL = BLOCKSIZE;
             shiftR = ( last - begin) - BLOCKSIZE + 1;
             start_right = 0;
             for (j = 0; j < shiftR; j++) {
                indexR [ num_right ] = j;
                // num_right += ! ( less( pivot , *( last - j ) ) );
		num_right += ( compareXY(pivot, A[last - j]) >= 0 );
             }
         }
   
         // rearrange final iteration
         // num = std::min ( num_left , num_right );
	 num = ( num_left <= num_right ? num_left : num_right);
         for (j = 0; j < num; j++)
	   // std::iter_swap ( begin + indexL [ start_left + j ], 
	   //                  last - indexR [ start_right + j ] );
	   iswap3(begin + indexL [ start_left + j ],
		 last - indexR [ start_right + j ], A);
   
         num_left -= num;
         num_right -= num;
         start_left += num;
         start_right += num;
         begin += ( num_left == 0 ) ? shiftL : 0;
         last -= ( num_right == 0 ) ? shiftR : 0;
         // end final iteration
   
	 int pivotIndex;
        // rearrange elements remaining in buffer
        if ( num_left != 0 )
        {
            int lowerI = start_left + num_left - 1;
            int upper = last - begin;
            // search first element to be swapped
            while ( lowerI >= start_left && indexL [ lowerI ] == upper ) {
               upper--; lowerI--;
            }
            while ( lowerI >= start_left )
	      // std::iter_swap ( begin + upper--, begin + indexL [ lowerI--]);
	      iswap3(begin + upper--, begin + indexL [ lowerI--], A);
   
            // std::iter_swap ( pivot_position, begin + upper + 1 ); 
	    iswap3(pivot_position, begin + upper + 1, A);
	    // fetch the pivot
            // return begin + upper + 1;
	    pivotIndex = begin + upper + 1;
        }
        else if ( num_right != 0 ) {
	  int lowerI = start_right + num_right - 1 ;
	  int upper = last - begin;
	  // search first element to be swapped
	  while( lowerI >= start_right && indexR[ lowerI ] == upper ) {
	    upper--; lowerI--;
	  }
   
	  while (lowerI >= start_right )
	    // std::iter_swap(last - upper--, last - indexR[lowerI--]);
	    iswap3(last - upper--, last - indexR[lowerI--], A);
   
	  // std::iter_swap (pivot_position, last - upper ); 
	  iswap3(pivot_position, last - upper, A);
	  // fetch the pivot
	  // return last - upper;
	  pivotIndex = last - upper;
	}
        else { // no remaining elements
	  // std::iter_swap(pivot_position, begin); // fetch the pivot
	  iswap(pivot_position, begin, A);
	  // return begin;
	  pivotIndex = begin;
        }

	// Recurse on smallest segment first.
	depthLimit++;
	if ( pivotIndex - begin < last - pivotIndex ) {
	  blockSortc(A, N, pivotIndex-1, indexL, indexR, depthLimit, compareXY);
	  N = pivotIndex + 1;
	  goto again;
	}
	blockSortc(A, pivotIndex+1, M, indexL, indexR, depthLimit, compareXY);
	M = pivotIndex-1;
	goto again;

} // end blockSortc
   

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

void compare00FourSortAgainstFiveSort() {
  void cut2(), tps();
  compareZeros00("compare00FourSortAgainstFiveSort", 
		 1024*1024, 32, cut2, tps, 
		 // 1024*1024, 16, cut2, tps, 
		 // 1024*1024*16, 2, cut2, tps, 
		 compareXY, compareXY);
} // end compare00FourSortAgainstFiveSort
void compare00DPQAgainstFiveSort() {
  void dpq(), tps();
  compareZeros00("compare00DPQAgainstFiveSort", 
		 1024*1024, 32, dpq, tps, 
		 // 1024*1024, 16, dpq, tps, 
		 // 1024*1024 *16, 2, dpq, tps, 
		 compareXY, compareXY);
} // end compare00DPQAgainstFiveSort
void compare00DPQAgainstFourSort() {
  void dpq(), cut2();
  compareZeros00("compare00DPQAgainstFourSort", 
		 1024*1024, 32, dpq, cut2, 
		 // 1024*1024, 16, dpq, cut2, 
		 // 1024*1024 *16, 2, dpq, cut2, 
		 compareXY, compareXY);
} // end compare00DPQAgainstFiveSort

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

void validateC2() {
  validateAlgorithm("Running validate cut2 ...",
		    //		    quicksort0, cut2);
		    heapc, cut2);
}
/*
void validateC21() {
  validateAlgorithm("Running validate C2Split ...",
		    quicksort0, c21);
}
*/

void validateC2lr() {
  validateAlgorithm("Running validate C2LR ...",
		    quicksort0, cut2lr);
}
void validateC2lr2() {
  validateAlgorithm("Running validate C2LR2 ...",
		    quicksort0, cut2lr2);
}
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
void validateC2sort() {
  validateAlgorithm("Running validate C2sort ...",
		    quicksort0, cut2);
}

void validateC2msort() {
  validateAlgorithm("Running validate C2msort ...",
		    quicksort0, cut2m);
}

void validateC2ksort() {
  validateAlgorithm("Running validate C2ksort ...",
		    quicksort0, cut2k);
}
void validateC2k2sort() {
  validateAlgorithm("Running validate C2k2sort ...",
		    quicksort0, cut2k2);
}
/*
void validateC2k3sort() {
  validateAlgorithm("Running validate C2k3sort ...",
		    quicksort0, cut2k3);
}
*/
void validateC2k4sort() {
  validateAlgorithm("Running validate C2k4sort ...",
		    quicksort0, cut2k4);
}
void validateC2k1sort() {
  validateAlgorithm("Running validate C2k1sort ...",
		    quicksort0, cut2k1);
}

void validateCN4sort() {
validateAlgorithm("Running validate CN4sort ...",
		    quicksort0, cut4n);
}
void validateCD4sort() {
validateAlgorithm("Running validate CD4sort ...",
		    quicksort0, cut4d);
}
void validateQ0() {
  validateAlgorithm("Running validate Q0 ...",
		    quicksort0, heapc);
}
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
void validateMergeSort() {
  validateAlgorithm("Running validate MergeSort ...",
		    quicksort0, mergeSort);
}


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
void rangeLQ() {
  void _quicksort();
  printf("rangeLQ\n");
  range(_quicksort, compareIntVal2, 0);
} // end rangeLQ
/*
void rangeMyqsort() {
  void myqsort();
  printf("rangeMyqsort\n");
  range( myqsort, compareXY, 1);
} // end rangeMyqsort()
*/
void rangeBentley() { 
  void bentley();
  printf("rangeBentley\n");
  range(bentley, compareIntVal2, 0);
} // end rangeBentley
/*
void rangeMybam() {
  void rangeMybam();
  printf("rangeMybam\n");
  range(mybam, compareXY, 1);
} // end rangeQuicksort0
*/
void rangeQuicksort0() {
  void quicksort0();
  printf("rangeQuicksort0\n");
  range(quicksort0, compareXY, 1);
} // end rangeQuicksort0
void rangeDflgm0() {
  void dflgm0();
  printf("rangeDflgm0\n");
  range(dflgm0, compareXY, 1);
} // end rangeDflgm0
void rangeCut2() {
  void cut2();
  printf("rangeCut2\n");
  range(cut2, compareXY, 1);
} // end rangeCut2
void rangeCut2lr() {
  void cut2lr();
  printf("rangeCut2lr\n");
  range(cut2lr, compareXY, 1);
} // end rangeCut2lr
void rangeBlockSort() {
  void blockSort();
  printf("rangeblockSort\n");
  range(blockSort, compareXY, 1);
} // end rangeBlockSort
// 2-pivot
void rangeDpq() {
  void dpq();
  printf("rangeDpq\n");
  range(dpq, compareXY, 1);
} // end rangeDpq
void rangeTps() {
  void tps();
  printf("rangeTps\n");
  range(tps, compareXY, 1);
} // end rangeTps
// 3 pivot
void rangeMpq() {
  void part3();
  printf("rangeMpq\n");
  range(part3, compareXY, 1);
} // end rangeMpq
void rangeCut4() {
  void cut4();
  printf("rangeCut4\n");
  range(cut4, compareXY, 1);
} // end rangeCut4
void rangeCut4n() {
  void cut4n();
  printf("rangeCut4n\n");
  range(cut4n, compareXY, 1);
} // end rangeCut4n
void rangeCut4d() {
  void cut4d();
  printf("rangeCut4d\n");
  range(cut4d, compareXY, 1);
} // end rangeCut4d
void rangeCut7() {
  void cut7();
  printf("rangeCut7\n");
  range(cut7, compareXY, 1);
} // end rangeCut7

#undef iswap
