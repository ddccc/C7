// myqsort.c
//    -- a modified version of the sort function in the library
// provided with the Gnu C compiler
//
// The modifications make the interface identical to that used in
// de Champeaux's sorting code, where only arrays of pointers to
// objects are sorted.
//
// Source code of qsort.c copied from
//     https://code.woboq.org/userspace/glibc/stdlib/qsort.c.html
// on 2020-11-8


/* Copyright (C) 1991-2019 Free Software Foundation, Inc.
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
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */
/* If you consider tuning this algorithm, you should consult first:
   Engineering a sort function; Jon Bentley and M. Douglas McIlroy;
   Software - Practice and Experience; Vol. 23 (11), 1249-1265, 1993.  */

#include <limits.h>
#include <stdlib.h>
#include <string.h>

// #include "myqsort.h"


/* swap two pointer items  referenced by a and b */
#define SWAP2(a, b)  do { void **t = *a; *a = *b; *b = t; } while(0)

/* Discontinue quicksort algorithm when partition gets below this size.
   New value of 6 works slightly better than the previous value of 4
   on an Intel I5 architecture.
   The magic number 4 was chosen because it worked best on a Sun 4/260. */
#ifndef MAX_THRESH2
#define MAX_THRESH2 6
#endif

/* Stack node declarations used to store unfulfilled partition obligations. */
typedef struct
  {
    void **lo;
    void **hi;
  } stack_node2;

/* The next 4 #defines implement a very fast in-line stack abstraction. */

/* The stack needs log (total_elements) entries (we could even subtract
   log(MAX_THRESH)).  Since total_elements has type size_t, we get as
   upper bound for log (total_elements):
   bits per byte (CHAR_BIT) * sizeof(size_t).  */

#define STACK_SIZE2        (8 * sizeof (size_t))
#define PUSH(low, high)   ((void) ((top->lo = (low)), (top->hi = (high)), ++top))
#define POP(low, high)    ((void) (--top, (low = top->lo), (high = top->hi)))
#define STACK_NOT_EMPTY   (stack < top)

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

void myqsort( void **basePtr, int first, int last, int compareXY(const void *a, const void *b) ) {
    int total_elems = last - first + 1;
    void **A = basePtr+first;

    if (total_elems == 0)
        /* Avoid lossage with unsigned arithmetic below.  */
        return;

    if (total_elems > MAX_THRESH2) {
        void **lo = A;
        void **hi = lo + (last-first);
        stack_node2 stack[STACK_SIZE2];
        stack_node2 *top = stack;

        PUSH(NULL, NULL);
        while (STACK_NOT_EMPTY) {
            void **left_ptr;
            void **right_ptr;

            /* Select median value from among LO, MID, and HI. Rearrange
               LO and HI so the three values are sorted. This lowers the
               probability of picking a pathological pivot value and
               skips a comparison for both the LEFT_PTR and RIGHT_PTR in
               the while loops. */

            void **mid = lo + (hi - lo) / 2;
            if ((*compareXY)(*mid, *lo) < 0)
                SWAP2(mid, lo);
            if ((*compareXY)(*hi, *mid) < 0) {
                SWAP2(mid, hi);
                if ((*compareXY)(*mid, *lo) < 0)
                    SWAP2(mid, lo);
            }

            left_ptr  = lo + 1;
            right_ptr = hi - 1;

            /* Here's the famous ``collapse the walls'' section of quicksort.
               Gotta like those tight inner loops!  They are the main reason
               that this algorithm runs much faster than others. */

            do {
                while ((*compareXY)(*left_ptr, *mid) < 0)
                    left_ptr++;
                while ((*compareXY)(*mid, *right_ptr) < 0)
                    right_ptr--;
                if (left_ptr < right_ptr) {
                    SWAP2(left_ptr, right_ptr);
                    if (mid == left_ptr)
                        mid = right_ptr;
                    else if (mid == right_ptr)
                        mid = left_ptr;
                    left_ptr++;
                    right_ptr--;
                } else if (left_ptr == right_ptr) {
                    left_ptr++;
                    right_ptr--;
                    break;
                }
            } while (left_ptr <= right_ptr);

           /* Set up pointers for next iteration.  First determine whether
               left and right partitions are below the threshold size.  If so,
               ignore one or both.  Otherwise, push the larger partition's
               bounds on the stack and continue sorting the smaller one. */

            if ((right_ptr - lo) <= MAX_THRESH2) {
                if ((hi - left_ptr) <= MAX_THRESH2)
                    /* Ignore both small partitions. */
                    POP(lo, hi);
                else
                    /* Ignore small left partition. */
                    lo = left_ptr;
            }
            else if ((hi - left_ptr) <= MAX_THRESH2)
                /* Ignore small right partition. */
                hi = right_ptr;
            else if ((right_ptr - lo) > (hi - left_ptr)) {
                /* Push larger left partition indices. */
                PUSH(lo, right_ptr);
                lo = left_ptr;
            } else {
                /* Push larger right partition indices. */
                PUSH(left_ptr, hi);
                hi = right_ptr;
            }
        }
    }

 /* Once the BASE_PTR array is partially sorted by quicksort the rest
     is completely sorted using insertion sort, since this is efficient
     for partitions below MAX_THRESH size. BASE_PTR points to the beginning
     of the array to sort, and END_PTR points at the very last element in
     the array (*not* one beyond it!). */

#define MIN(x, y) ((x) < (y) ? (x) : (y))

    {
        void **end_ptr = A + (last-first);
        void **tmp_ptr = A;
        void **thresh = MIN(end_ptr, A + MAX_THRESH2);
        void **run_ptr;

        /* Find smallest element in first threshold and place it at the
           array's beginning.  This is the smallest array element,
           and the operation speeds up insertion sort's inner loop. */

        for (run_ptr = tmp_ptr + 1; run_ptr <= thresh; run_ptr++)
            if ((*compareXY)(*run_ptr, *tmp_ptr) < 0)
                tmp_ptr = run_ptr;

        if (tmp_ptr != A)
            SWAP2(tmp_ptr, A);

        /* Insertion sort, running from left-hand-side up to right-hand-side.  */

        /* Insertion sort, running from left-hand-side up to right-hand-side.  */

        run_ptr = A + 1;
        while (++run_ptr <= end_ptr) {
            tmp_ptr = run_ptr - 1;
            while ((*compareXY)(*run_ptr, *tmp_ptr) < 0)
                tmp_ptr--;
            tmp_ptr++;
            if (tmp_ptr != run_ptr) {
                void **trav = run_ptr;
                void *x = *trav;
                while(trav-- >= tmp_ptr) {
                    *(trav+1) = *trav;
                }
                *tmp_ptr = x;
            }
        }
    }
}
