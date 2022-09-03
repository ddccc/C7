// File: c:/bsd/rigel/sort/C7/ComparisonCountings.c
// Date: Thu Sep 12 19:43:32 2019
// (C) OntoOO/ Dennis de Champeaux

/*
How to use this driver:
Compile with: 
   gcc ComparisonCounting.c
   gcc -O2 ComparisonCounting.c  <== best
   gcc -O3 ComparisonCounting.c
Execute with: ./a.exe

The main function contains lines with the calls of the different 
algorithms.
The procedure fillarray allows to generate arrays of different sizes 
with different content.
*/

/* 
Counting comparisons & timings 
*/
#include <stdio.h>
#include <stddef.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
// #include <strings.h>
#include <sys/time.h>

// Example of objects that can be used to populate an array to be sorted:
  // To obtain the int field from X: ((struct intval *) X)->val
  // To obtain the float field from X: ((struct intval *) X)->valf
typedef struct intval {
  int val;
  float valf;
} *Data;

// #include <pthread.h>
// #include <sys/time.h>

// /* 
// These are included by the includes below
// #include "Isort.c"
#include "Isort.h"
#include "Hsort.c"
#include "Dsort.c"  // dflgm member
#include "Qusort.c" // quicksort member
// */

// #include "Qusort.c"
#include "D4.c"
#include "D3sort.c" 
#include "C2sort.c" 
#include "C2LR.c"
#include "C2Left.c"
#include "C3sort.c" // tps member
#include "C4.c"     // cut4 member
#include "C7.c"


void callLQ();
void callBentley();
void quicksort0();
void cut2();
void tps();
void cut4();
void cut7();
void c7p();
void dpq();
void cc();
void part3(); // MPQ
void cut2lr();
void testAlg();
void validateAlg();

// double cnt = 0; // comparison counter
long long cnt = 0;
// Here an exmple comparison function for these objects:
// **** NOTE, again **** 
//      All the examples below use ::::
//          the intval objects, and 
//          the compareIntVal comparison function
// int compareIntVal (const void *a, const void *b)
int compareXY (const void *a, const void *b)
{
  // cnt++; // no comparison cnt in this version
  return ((struct intval *)a)->val - ((struct intval *)b)->val;
}

// This comparison formula is used for qsort in callQsort and
// for bentley in callBentley 
int compareIntVal2 (const void **a, const void **b)
{ // cnt++; // no comparison cnt in this version
  struct intval *pa = (struct intval *) *a;
  struct intval *pb = (struct intval *) *b;
  return (pa->val - pb->val);
}

int main (int argc, char *argv[]) {
  printf("Running ComparisonCounting ...\n");
     // 1 pivot
  // cc("LQ     ", callLQ, compareIntVal2, 0);
  cc("bentley", callBentley, compareIntVal2, 0);
  cc("cut2   ", cut2, compareXY, 1);
  cc("cut2lr ", cut2lr, compareXY, 1);
  // cc("c2left  ", cut2left, compareXY, 1); 
     // 2 pivot
  // cc("dpq    ", dpq, compareXY, 1);
  // cc("tps    ", tps, compareXY, 1);
     // 3 pivot
  // cc("mpq    ", part3, compareXY, 1);
  cc("cut4   ", cut4, compareXY, 1);
  // cc("c7     ", cut7, compareXY, 1);
     // Misc
  // testAlg();
  // validateAlg();
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
  struct intval *pi;

  srand(startv);
  for ( i = 0; i < lng; i++) {
    pi = (struct intval *)A[i];
    pi->val = rand()%range; 
    // pi->val = 0; 
    // pi->val = i; 
    // pi->val = -i; 
  }
  // */
  /*
  // permutation of [0, lng), to avoid duplicates
  srand(startv);
  for ( i = 0; i < lng; i++) {
    pi = (struct intval *)A[i];
    pi->val = i; 
  }
  int j;
  for ( i = lng-1; 0<i ; i--) {
    j = rand()%i;
    iswap(i, j, A);
  }
  */

} // end of fillarray

long long comparisons; double clocktime; 

void countcomparisons(int siz, void (*alg1)(), 
		      int (*compar )(), int seed, int bool) {
  // printf("%s on size: %d ", label, siz);
  struct intval *pi;
  // void **A = myMalloc("countcomparisons 1", sizeof(pi) * siz);
  void **A = myMalloc("countcomparisons 1", sizeof(pi) * siz);
  int i;
  for (i = 0; i < siz; i++) {
    pi = myMalloc("countcomparisons 2", sizeof (struct intval));
    A[i] = pi;
  };
  int reps = 20;
  // int reps = 5;
  // int reps = 5 * 1024 *4;
  for (i = 0; i < reps; i++) fillarray(A, siz, seed); // warm up
  // measure the array fill time
  // int TFill = clock();
      struct timeval tim;
      gettimeofday(&tim, NULL);
      double TFILL=tim.tv_sec+(tim.tv_usec/1000000.0);
    
  for (i = 0; i < reps; i++) fillarray(A, siz, seed+i);
  // TFill = clock() - TFill;
      gettimeofday(&tim, NULL);
      TFILL=tim.tv_sec+(tim.tv_usec/1000000.0) - TFILL;
  // now we know how much time it takes to fill the array

  // measure the time to fill & sort the array
  // int T = clock();
      gettimeofday(&tim, NULL);
      double T=tim.tv_sec+(tim.tv_usec/1000000.0);

  cnt = 0;
  for (i = siz; i < siz + reps; i++) { 
    // fill its content
    fillarray(A, siz, i);
    // sort it
    if ( bool ) 
      (*alg1)(A, 0, siz-1, compar);
    else
      (*alg1)(A, siz, compar);
  }
  // int algTime = (clock() - T - TFill)/reps;
      gettimeofday(&tim, NULL);
      double algTime=tim.tv_sec+(tim.tv_usec/1000000.0) - T - TFILL;

  cnt = cnt/reps;
  // printf("is %g time %i\n", cnt, algTime);
  comparisons += cnt;
  clocktime += algTime/reps;
  // free array
  for (i = 0; i < siz; i++) {
    free(A[i]); 
  };
  free(A);
} // end countcomparisons

int siz = 1024*1024*16;
// int siz = 1024*1024*4;
// int siz = 1024*1024;
// int siz = 1024*256;
// int siz = 1024*64;
// int siz = 1024*32;
// int siz = 1024*16;
// int siz = 1024*4;
// int siz = 1024;

void cc(char* label, void (*alg1)(), int (*compar )(), int bool) {
  // printf("cc %s ", label);
  int size = siz;
  // int repeat = 0;
  //  printf("size %i min # comparisons: %g\n", 
  //	 size, 1.44*size*log(size));
  int repeat = 4;
  int reps = 1;
  int i;
  while ( repeat < 5) { // no reps with with 5
    double nln = size * log(size) / log(2.0);
    // printf("cc %s size %d min # comparisons: %12.0f\n", label, size, nln);
    printf("cc %s ", label);
    comparisons = 0;
    clocktime = 0;
    for ( i = 0; i < reps; i++)
      countcomparisons(size, alg1, compar, i, bool);
    comparisons = comparisons / reps; clocktime = clocktime / reps;

    // printf("size %i comparisons %g clocktime %i clock2 %8.2f\n",
    // printf("size %i comparisons %lld clocktime %i clock2 %8.2f\n",
    //         size, comparisons, clocktime, 1000000 * clocktime/ nln);
    printf("size %i comparisons %lld clocktime %f\n",
	   size, comparisons, clocktime);
    size *= 2; repeat++;
  }
  
} // end cc


// ************************************************************
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

#include <alloca.h>
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
          char *left_ptr;
          char *right_ptr;

	  /* Select median value from among LO, MID, and HI. Rearrange
	     LO and HI so the three values are sorted. This lowers the
	     probability of picking a pathological pivot value and
	     skips a comparison for both the LEFT_PTR and RIGHT_PTR in
	     the while loops. */

	  char *mid = lo + size * ((hi - lo) / size >> 1);

	  if ((*cmp) ((void *) mid, (void *) lo, arg) < 0)
	    SWAP (mid, lo, size);
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

  #define min(x, y) ((x) < (y) ? (x) : (y))

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

// #define min(a, b)	(a) < (b) ? a : b

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

// ************************************************************

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

// ************************************************************************

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

// Infrastructure for testing non-uniform distributions

// fill with random numbers according to percentage, otherwise 0
void fillRandom(void **A, int lng, int startv, int percentage) {
  if ( percentage < 0 || 100 < percentage ) percentage = 50;
  int i, v, r, v2; 
  srand(startv);
  struct intval *pi;
  for ( i = 0; i < lng; i++) {
    pi = (struct intval *)A[i];
    if ( 0 == percentage ) pi->val = 0; else {
      v = rand();
      v2 = (v < 0) ? -v : v;
      r = v2%100;
      pi->val = (r <= percentage) ? v : 0; 
    }
  }
} // end fillRandom

// fill with random numbers according to percentage, otherwise with i
void fillRandom2(void **A, int lng, int startv, int percentage) {
  if ( percentage < 0 || 100 < percentage ) percentage = 50;
  int i, v, r, v2; 
  srand(startv);
  struct intval *pi;
  for ( i = 0; i < lng; i++) {
    pi = (struct intval *)A[i];
    // if ( 0 == percentage ) pi->val = i; else {
    if ( 0 == percentage ) pi->val = -i; else {
      v = rand();
      v2 = (v < 0) ? -v : v;
      r = v2%100;
      // pi->val = (r <= percentage) ? v : i; 
      pi->val = (r <= percentage) ? v : -i; 
    }
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

// Check that adjacent objects in an array are ordered.
// If not, it reports an error 
void check(void **A, int N, int M) {
  int i; int cnt = 0;
  void *x, *y;
  struct intval *pi;
  for (i = N + 1; i <= M; i++) {
    x = A[i-1]; y = A[i];
    if ( compareXY(y, x) < 0 ) {
      pi = (struct intval *) y;
      printf("%s %d %s %d %s", "Error at: ", i, 
	     " A[i]: ", pi->val, "\n");
      cnt++;
    }
  }
  printf("check # errors: %d\n", cnt);
} // end check


void testAlg1(char* label, int siz, void (*alg1)(),
		     int (*compare1)()) {
  printf("%s on size: %d\n", label, siz);
  // create array
  struct intval *pi;
  void **A = myMalloc("testAlgorithm0 1", sizeof(pi) * siz);
  int i;
  for (i = 0; i < siz; i++) {
    pi = myMalloc("testAlgorithm0 2", sizeof (struct intval));
    A[i] = pi;
  };
  // fill its content
  fillarray(A, siz, 0);
  // sort it
  (*alg1)(A, 0, siz-1, compare1);
  // and check it
  check(A, 0, siz-1);
  // free array
    for (i = 0; i < siz; i++) {
      free(A[i]);
    };
    free(A);
} // end testAlg0

void testAlg2(char* label, int siz, void (*alg1)()) {
  testAlg1(label, siz, alg1, compareXY);
} // end testAlgorithm00

void testAlg3(char* label, void (*alg1)() ) {
  // testAlg2(label, 1024*1024*16, alg1);
  // testAlg2(label, 1024 * 1024 *2, alg1); 
  testAlg2(label, 16, alg1);
} // end testAlgorithm000

void testAlg() {
  // testAlg3("Test cut4 ...", cut4);
  // testAlg3("Test cut7 ...", cut7);
}

// validateAlgorithm0 is used to check algorithm alg2 against a
// trusted algorithm alg1.
// The check consists of making sure that starting from identical
// inputs they produce identical outputs
void validateAlgorithm0(char* label, int siz, void (*alg1)(), void (*alg2)() ) {
  printf("%s on size: %d\n", label, siz);
  // create the input for alg1 ...
  struct intval *pi;
  void **A = myMalloc("validateAlgorithm0 1", sizeof(pi) * siz);
  int i;
  for (i = 0; i < siz; i++) {
    pi = myMalloc("validateAlgorithm0 2", sizeof (struct intval));
    A[i] = pi;
  };
  fillarray(A, siz, 0);
  // ... sort it
  (*alg1)(A, 0, siz-1, compareXY);
  // (*alg1)(A, 0, siz, compareXY); // special for quickSortD

  // create the input for alg2 ...
  void **B = myMalloc("validateAlgorithm0 3", sizeof(pi) * siz);
  // struct intval *pi;
  for (i = 0; i < siz; i++) {
    pi =  myMalloc("validateAlgorithm0 4", sizeof (struct intval));
    B[i] = pi;
  };
  fillarray(B, siz, 0);
  // ... sort it
  (*alg2)(B, 0, siz-1, compareXY);
 
  // check that the two outputs are the same
  int foundError = 0;
  for (i = 0; i < siz; i++)
    // if ( A[i] != B[i] ) {
    if ( compareXY(A[i], B[i]) != 0 ) {
      printf("validate error i: %d\n", i);
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

// Like validateAlgorithm0 but with fixed array size
void validateAlgorithm(char* label, void (*alg1)(), void (*alg2)() ) {
  validateAlgorithm0(label, 1024 * 1024 * 16, alg1, alg2);
  // validateAlgorithm0(label, 1024*1024*2, alg1, alg2);
  // validateAlgorithm0(label, 1025, alg1, alg2);
} // end validateAlgorithm

// Example:: replace XYZ by what you want to validate
void validateAlg() {
  // void quicksort0(), XYZ();
  // validateAlgorithm("Running validate XYZ ...", quicksort0, XYZ);
  // validateAlgorithm("Running validate cut7 ...", quicksort0, cut7);

} // end validateAlg()

