// File: c:/bsd/rigel/sort/C7/Timsort.c
// Date: Wed Jul 14 13:49:45 2021

// timsort.c --- ported by Nigel Horspool

#define NDEBUG      // disable assertion checking

/*
 * Copyright (c) 2009, 2013, Oracle and/or its affiliates. All rights reserved.
 * Copyright 2009 Google Inc.  All Rights Reserved.
 * ORACLE PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */

/**
 * This is a near duplicate of {@link TimSort}, modified for use with
 * arrays of objects that implement {@link Comparable}, instead of using
 * explicit comparators.
 *
 * <p>If you are using an optimizing VM, you may find that ComparableTimSort
 * offers no performance benefit over TimSort in conjunction with a
 * comparator that simply returns {@code ((Comparable)first).compareTo(Second)}.
 * If this is the case, you are better off deleting ComparableTimSort to
 * eliminate the code duplication.  (See Arrays.java for details.)
 *
 * @author Josh Bloch
 */

 /*
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "timsort.h"
*/

#include <string.h>     // for memmove function

void *myCalloc(char* location, int numElements, int elemSize) {
    void *p = calloc(numElements, elemSize);
    if (p == NULL) {
        fprintf(stderr, "calloc failed for: %s\n", location);
        exit(1);
    }
    return p;
} // end myCalloc

void *myRealloc(char *location, void *object, int newSize) {
    void *p = realloc(object, newSize);
    if (p == NULL) {
        fprintf(stderr, "myRealloc failed for: %s\n", location);
        exit(1);
    }
    return p;
}

#define ObjPtr  void*

// An instance of this struct replicates the instance fields of the
// ComparableTimSort class in the Java class library
typedef struct timsortData {
    /**
     * The array being sorted.
     */
    ObjPtr *a;
    int len;    // size of array being sorted

    /**
     * Temp storage for merges. A workspace array may optionally be
     * provided in constructor, and if so will be used as long as it
     * is big enough.
     */
    ObjPtr *tmp;
    int tmpBase; // base of tmp array slice
    int tmpLen;  // length of tmp array slice

    /**
     * A stack of pending runs yet to be merged.  Run i starts at
     * address base[i] and extends for len[i] elements.  It's always
     * true (so long as the indices are in bounds) that:
     *
     *     runBase[i] + runLen[i] == runBase[i + 1]
     *
     * so we could cut the storage for this, but it's a minor amount,
     * and keeping all the info explicit simplifies the code.
     */
    int stackSize;  // Number of pending runs on stack
    int stackLen;   // size of runBase & runLen arrays
    int *runBase;
    int *runLen;

    // This controls when we get *into* galloping mode.  It is initialized
    // to MIN_GALLOP.  The mergeLo and mergeHi methods nudge it higher for
    // random data, and lower for highly structured data.
    int minGallop;
} TimsortData, *TimsortDataPtr;


// forward declarations
static void timsort2(int low, int high, TimsortDataPtr td, COMPAREFN compareXY);
static void binarySort(ObjPtr a[], int low, int high, int start, COMPAREFN compareXY);
static int countRunAndMakeAscending(ObjPtr a[], int low, int high, COMPAREFN compareXY);
static void reverseRange(ObjPtr a[], int lo, int hi);
static int minRunLength(int n);
static void pushRun(int lo, int rl, TimsortDataPtr td);
static void mergeCollapse(TimsortDataPtr td, COMPAREFN compareXY);
static void mergeForceCollapse(TimsortDataPtr td, COMPAREFN compareXY);
static void mergeAt(int i, TimsortDataPtr td, COMPAREFN compareXY);
static int gallopLeft(ObjPtr key, ObjPtr a[], int base, int len, int hint,
        TimsortDataPtr td, COMPAREFN compareXY);
static int gallopRight(ObjPtr key, ObjPtr a[], int base, int len, int hint,
        TimsortDataPtr td, COMPAREFN compareXY);
static void mergeLo(int base1, int len1, int base2, int len2,
        TimsortDataPtr td, COMPAREFN compareXY);
static void mergeHi(int base1, int len1, int base2, int len2,
        TimsortDataPtr td, COMPAREFN compareXY);
static void ensureCapacity(int minCapacity, TimsortDataPtr td);


/**
 * This is the minimum sized sequence that will be merged.  Shorter
 * sequences will be lengthened by calling binarySort.  If the entire
 * array is less than this length, no merges will be performed.
 *
 * This constant should be a power of two.  It was 64 in Tim Peter's C
 * implementation, but 32 was empirically determined to work better in
 * this implementation.  In the unlikely event that you set this constant
 * to be a number that's not a power of two, you'll need to change the
 * {@link #minRunLength} computation.
 *
 * If you decrease this constant, you must change the stackLen
 * computation in the TimSort constructor, or you risk an
 * ArrayOutOfBounds exception.  See listsort.txt for a discussion
 * of the minimum stack length required as a function of the length
 * of the array being sorted and the minimum merge sequence length.
 */
#define MIN_MERGE 32

/**
 * When we get into galloping mode, we stay there until both runs win less
 * often than MIN_GALLOP consecutive times.
 */
#define MIN_GALLOP 7



/**
 * Maximum initial size of tmp array, which is used for merging.  The array
 * can grow to accommodate demand.
 *
 * Unlike Tim's original C version, we do not allocate this much storage
 * when sorting smaller arrays.  This change was required for performance.
 */
#define INITIAL_TMP_STORAGE_LENGTH 256



// THE ENTRY POINT FOR THE C VERSION OF TIMSORT

void timsort(ObjPtr a[], int low, int high, COMPAREFN compareXY) {
    int nRemaining  = high - low + 1;
    if (nRemaining < 2)
        return;  // Arrays of size 0 and 1 are always sorted

    // If array is small, do a "mini-TimSort" with no merges
    if (nRemaining < MIN_MERGE) {
        int initRunLen = countRunAndMakeAscending(a, low, high, compareXY);
        binarySort(a, low, high+1, low + initRunLen, compareXY);
        return;
    }

    TimsortData td;

    td.a = a;
    td.len = nRemaining;

    // Allocate temp storage (which may be increased later if necessary)
    int tlen = (nRemaining < 2 * INITIAL_TMP_STORAGE_LENGTH) ?
        nRemaining >> 1 : INITIAL_TMP_STORAGE_LENGTH;
    td.tmp = myCalloc("timsort1-a", tlen, sizeof(ObjPtr));  // new Object[tlen];
    td.tmpBase = 0;
    td.tmpLen = tlen;
    td.len = nRemaining;
    td.stackSize = 0;
    td.stackLen = ( td.len <    120  ?  5 :
                    td.len <   1542  ? 10 :
                    td.len < 119151  ? 24 : 49);
    td.runBase = myCalloc("timsort1-b", td.stackLen, sizeof(int));  // new int[stackLen];
    td.runLen =  myCalloc("timsort1-c", td.stackLen, sizeof(int));  // new int[stackLen];
    td.minGallop = MIN_GALLOP;
    timsort2(low, high+1, &td, compareXY);
    free(td.tmp);
    free(td.runBase);
    free(td.runLen);
}


/*
 * The next method (package private and static) constitutes the
 * entire API of this class.
 */

/**
 * Sorts the given range, using the given workspace array slice
 * for temp storage when possible. This method is designed to be
 * invoked from public methods (in class Arrays) after performing
 * any necessary array bounds checks and expanding parameters into
 * the required forms.
 *
 * @param a the array to be sorted
 * @param lo the index of the first element, inclusive, to be sorted
 * @param hi the index of the last element, exclusive, to be sorted
 * @param work a workspace array (slice)
 * @param workBase origin of usable space in work array
 * @param workLen usable size of work array
 * @since 1.8
 */
static void timsort2(int low, int high, TimsortDataPtr td, COMPAREFN compareXY) {
    ObjPtr *a = td->a;
    //    assert(a != NULL && low >= 0 && low <= high);

    /**
     * March over the array once, left to right, finding natural runs,
     * extending short natural runs to minRun elements, and merging runs
     * to maintain stack invariant.
     */
    //ComparableTimSort ts = new ComparableTimSort(a, work, workBase, workLen);
    int nRemaining = high - low;
    int minRun = minRunLength(nRemaining);
    do {
        // Identify next run
        int runLen = countRunAndMakeAscending(a, low, high, compareXY);

        // If run is short, extend to min(minRun, nRemaining)
        if (runLen < minRun) {
            int force = nRemaining <= minRun ? nRemaining : minRun;
            binarySort(a, low, low + force, low + runLen, compareXY);
            runLen = force;
        }

        // Push run onto pending-run stack, and maybe merge
        pushRun(low, runLen, td);
        mergeCollapse(td, compareXY);

        // Advance to find next run
        low += runLen;
        nRemaining -= runLen;
    } while (nRemaining != 0);

    // Merge all remaining runs to complete sort
    // assert(low == high);
    mergeForceCollapse(td, compareXY);
    // assert(td->stackSize == 1);
}

/**
 * Sorts the specified portion of the specified array using a binary
 * insertion sort.  This is the best method for sorting small numbers
 * of elements.  It requires O(n log n) compares, but O(n^2) data
 * movement (worst case).
 *
 * If the initial part of the specified range is already sorted,
 * this method can take advantage of it: the method assumes that the
 * elements from index {@code lo}, inclusive, to {@code start},
 * exclusive are already sorted.
 *
 * @param a the array in which a range is to be sorted
 * @param lo the index of the first element in the range to be sorted
 * @param hi the index after the last element in the range to be sorted
 * @param start the index of the first element in the range that is
 *        not already known to be sorted ({@code lo <= start <= hi})
 */
//@SuppressWarnings({"fallthrough", "rawtypes", "unchecked"})
static void binarySort(ObjPtr a[], int low, int high, int start, COMPAREFN compareXY) {
  // assert(low <= start && start <= high);
    if (start == low)
        start++;
    for ( ; start < high; start++) {
        ObjPtr pivot = a[start];

        // Set left (and right) to the index where a[start] (pivot) belongs
        int left = low;
        int right = start;
        // assert(left <= right);
        /*
         * Invariants:
         *   pivot >= all in [lo, left).
         *   pivot <  all in [right, start).
         */
        while (left < right) {
            int mid = (left + right) >> 1;
            if (compareXY(pivot, a[mid]) < 0)
                right = mid;
            else
                left = mid + 1;
        }
        // assert(left == right);

        /*
         * The invariants still hold: pivot >= all in [lo, left) and
         * pivot < all in [left, start), so pivot belongs at left.  Note
         * that if there are elements equal to pivot, left points to the
         * first slot after them -- that's why this sort is stable.
         * Slide elements over to make room for pivot.
         */
        int n = start - left;  // The number of elements to move
        // Switch is just an optimization for arraycopy in default case
        switch (n) {
        case 2:
            a[left + 2] = a[left + 1];
            // drop through
        case 1:
            a[left + 1] = a[left];
            break;
        default:
                //System.arraycopy(a, left, a, left + 1, n);
                memmove(a+left+1, a+left, n*sizeof(ObjPtr));
        }
        a[left] = pivot;
    }
}

/**
 * Returns the length of the run beginning at the specified position in
 * the specified array and reverses the run if it is descending (ensuring
 * that the run will always be ascending when the method returns).
 *
 * A run is the longest ascending sequence with:
 *
 *    a[lo] <= a[lo + 1] <= a[lo + 2] <= ...
 *
 * or the longest descending sequence with:
 *
 *    a[lo] >  a[lo + 1] >  a[lo + 2] >  ...
 *
 * For its intended use in a stable mergesort, the strictness of the
 * definition of "descending" is needed so that the call can safely
 * reverse a descending sequence without violating stability.
 *
 * @param a the array in which a run is to be counted and possibly reversed
 * @param low index of the first element in the run
 * @param high index after the last element that may be contained in the run.
 *        It is required that {@code lo < hi}.
 * @return  the length of the run beginning at the specified position in
 *          the specified array
 */
//@SuppressWarnings({"unchecked", "rawtypes"})
static int countRunAndMakeAscending(ObjPtr a[], int low, int high, COMPAREFN compareXY) {
  // assert(low < high);
    int runHi = low + 1;
    if (runHi == high)
        return 1;

    // Find end of run, and reverse range if descending
    if (compareXY(a[runHi++], a[low]) < 0) { // Descending
        while (runHi < high && compareXY(a[runHi], a[runHi - 1]) < 0)
            runHi++;
        reverseRange(a, low, runHi);
    } else {                              // Ascending
        while (runHi < high && compareXY(a[runHi], a[runHi - 1]) >= 0)
            runHi++;
    }

    return runHi - low;
}

/**
 * Reverse the specified range of the specified array.
 *
 * @param a the array in which a range is to be reversed
 * @param lo the index of the first element in the range to be reversed
 * @param hi the index after the last element in the range to be reversed
 */
static void reverseRange(ObjPtr a[], int lo, int hi) {
    hi--;
    while (lo < hi) {
        ObjPtr t = a[lo];
        a[lo++] = a[hi];
        a[hi--] = t;
    }
}

/**
 * Returns the minimum acceptable run length for an array of the specified
 * length. Natural runs shorter than this will be extended with
 * {@link #binarySort}.
 *
 * Roughly speaking, the computation is:
 *
 *  If n < MIN_MERGE, return n (it's too small to bother with fancy stuff).
 *  Else if n is an exact power of 2, return MIN_MERGE/2.
 *  Else return an int k, MIN_MERGE/2 <= k <= MIN_MERGE, such that n/k
 *   is close to, but strictly less than, an exact power of 2.
 *
 * For the rationale, see listsort.txt.
 *
 * @param n the length of the array to be sorted
 * @return the length of the minimum run to be merged
 */
static int minRunLength(int n) {
  // assert(n >= 0);
    int r = 0;      // Becomes 1 if any 1 bits are shifted off
    while (n >= MIN_MERGE) {
        r |= (n & 1);
        n >>= 1;
    }
    return n + r;
}

/**
 * Pushes the specified run onto the pending-run stack.
 *
 * @param first index of the first element in the run
 * @param len  the number of elements in the run
 */
static void pushRun(int first, int len, TimsortDataPtr td) {
    td->runBase[td->stackSize] = first;
    td->runLen[td->stackSize] = len;
    td->stackSize++;
}

/**
 * Examines the stack of runs waiting to be merged and merges adjacent runs
 * until the stack invariants are reestablished:
 *
 *     1. runLen[i - 3] > runLen[i - 2] + runLen[i - 1]
 *     2. runLen[i - 2] > runLen[i - 1]
 *
 * This method is called each time a new run is pushed onto the stack,
 * so the invariants are guaranteed to hold for i < stackSize upon
 * entry to the method.
 *
 * Thanks to Stijn de Gouw, Jurriaan Rot, Frank S. de Boer,
 * Richard Bubel and Reiner Hahnle, this is fixed with respect to
 * the analysis in "On the Worst-Case Complexity of TimSort" by
 * Nicolas Auger, Vincent Jug, Cyril Nicaud, and Carine Pivoteau.
 */
static void mergeCollapse(TimsortDataPtr td, COMPAREFN compareXY) {
    while (td->stackSize > 1) {
        int n = td->stackSize - 2;
        if (n > 0 && td->runLen[n-1] <= td->runLen[n] + td->runLen[n+1] ||
            n > 1 && td->runLen[n-2] <= td->runLen[n] + td->runLen[n-1]) {
            if (td->runLen[n - 1] < td->runLen[n + 1])
                n--;
        } else if (n < 0 || td->runLen[n] > td->runLen[n + 1]) {
            break; // Invariant is established
        }
        mergeAt(n, td, compareXY);
    }
}

/**
 * Merges all runs on the stack until only one remains.  This method is
 * called once, to complete the sort.
 */
static void mergeForceCollapse(TimsortDataPtr td, COMPAREFN compareXY) {
    while (td->stackSize > 1) {
        int n = td->stackSize - 2;
        if (n > 0 && td->runLen[n - 1] < td->runLen[n + 1])
            n--;
        mergeAt(n, td, compareXY);
    }
}

/**
 * Merges the two runs at stack indices i and i+1.  Run i must be
 * the penultimate or antepenultimate run on the stack.  In other words,
 * i must be equal to stackSize-2 or stackSize-3.
 *
 * @param i stack index of the first of the two runs to merge
 */
//@SuppressWarnings("unchecked")
static void mergeAt(int i, TimsortDataPtr td, COMPAREFN compareXY) {
  // assert(td->stackSize >= 2);
  // assert(i >= 0);
  // assert(i == td->stackSize - 2 || i == td->stackSize - 3);

    int base1 = td->runBase[i];
    int len1 = td->runLen[i];
    int base2 = td->runBase[i + 1];
    int len2 = td->runLen[i + 1];
    // assert(len1 > 0 && len2 > 0);
    // assert(base1 + len1 == base2);

    /*
     * Record the length of the combined runs; if i is the 3rd-last
     * run now, also slide over the last run (which isn't involved
     * in this merge).  The current run (i+1) goes away in any case.
     */
    td->runLen[i] = len1 + len2;
    if (i == td->stackSize - 3) {
        td->runBase[i + 1] = td->runBase[i + 2];
        td->runLen[i + 1] = td->runLen[i + 2];
    }
    td->stackSize--;

    /*
     * Find where the first element of run2 goes in run1. Prior elements
     * in run1 can be ignored (because they're already in place).
     */
    int k = gallopRight(td->a[base2], td->a, base1, len1, 0, td, compareXY);
    // assert(k >= 0);
    base1 += k;
    len1 -= k;
    if (len1 == 0)
        return;

    /*
     * Find where the last element of run1 goes in run2. Subsequent elements
     * in run2 can be ignored (because they're already in place).
     */
    len2 = gallopLeft(td->a[base1 + len1 - 1], td->a, base2, len2, len2 - 1, td, compareXY);
    // assert(len2 >= 0);
    if (len2 == 0)
        return;

    // Merge remaining runs, using tmp array with min(len1, len2) elements
    if (len1 <= len2)
        mergeLo(base1, len1, base2, len2, td, compareXY);
    else
        mergeHi(base1, len1, base2, len2, td, compareXY);
}

/**
 * Locates the position at which to insert the specified key into the
 * specified sorted range; if the range contains an element equal to key,
 * returns the index of the leftmost equal element.
 *
 * @param key the key whose insertion point to search for
 * @param a the array in which to search
 * @param base the index of the first element in the range
 * @param len the length of the range; must be > 0
 * @param hint the index at which to begin the search, 0 <= hint < n.
 *     The closer hint is to the result, the faster this method will run.
 * @return the int k,  0 <= k <= n such that a[b + k - 1] < key <= a[b + k],
 *    pretending that a[b - 1] is minus infinity and a[b + n] is infinity.
 *    In other words, key belongs at index b + k; or in other words,
 *    the first k elements of a should precede key, and the last n - k
 *    should follow it.
 */
static int gallopLeft(ObjPtr key, ObjPtr a[], int base, int len, int hint,
        TimsortDataPtr td, COMPAREFN compareXY) {
  // assert(len > 0 && hint >= 0 && hint < len);

    int lastOfs = 0;
    int ofs = 1;
    if (compareXY(key, a[base + hint]) > 0) {
        // Gallop right until a[base+hint+lastOfs] < key <= a[base+hint+ofs]
        int maxOfs = len - hint;
        while (ofs < maxOfs && compareXY(key, a[base + hint + ofs]) > 0) {
            lastOfs = ofs;
            ofs = (ofs << 1) + 1;
            if (ofs <= 0)   // int overflow
                ofs = maxOfs;
        }
        if (ofs > maxOfs)
            ofs = maxOfs;

        // Make offsets relative to base
        lastOfs += hint;
        ofs += hint;
    } else { // key <= a[base + hint]
        // Gallop left until a[base+hint-ofs] < key <= a[base+hint-lastOfs]
        int maxOfs = hint + 1;
        while (ofs < maxOfs && compareXY(key, a[base + hint - ofs]) <= 0) {
            lastOfs = ofs;
            ofs = (ofs << 1) + 1;
            if (ofs <= 0)   // int overflow
                ofs = maxOfs;
        }
        if (ofs > maxOfs)
            ofs = maxOfs;

        // Make offsets relative to base
        int tmp = lastOfs;
        lastOfs = hint - ofs;
        ofs = hint - tmp;
    }
    // assert(-1 <= lastOfs && lastOfs < ofs && ofs <= len);

    /*
     * Now a[base+lastOfs] < key <= a[base+ofs], so key belongs somewhere
     * to the right of lastOfs but no farther right than ofs.  Do a binary
     * search, with invariant a[base + lastOfs - 1] < key <= a[base + ofs].
     */
    lastOfs++;
    while (lastOfs < ofs) {
        int m = lastOfs + ((ofs - lastOfs) >> 1);

        if (compareXY(key, a[base + m]) > 0)
            lastOfs = m + 1;  // a[base + m] < key
        else
            ofs = m;          // key <= a[base + m]
    }
    // assert(lastOfs == ofs);    // so a[base + ofs - 1] < key <= a[base + ofs]
    return ofs;
}

/**
 * Like gallopLeft, except that if the range contains an element equal to
 * key, gallopRight returns the index after the rightmost equal element.
 *
 * @param key the key whose insertion point to search for
 * @param a the array in which to search
 * @param base the index of the first element in the range
 * @param len the length of the range; must be > 0
 * @param hint the index at which to begin the search, 0 <= hint < n.
 *     The closer hint is to the result, the faster this method will run.
 * @return the int k,  0 <= k <= n such that a[b + k - 1] <= key < a[b + k]
 */
static int gallopRight(ObjPtr key, ObjPtr a[], int base, int len, int hint,
        TimsortDataPtr td, COMPAREFN compareXY) {
  // assert(len > 0 && hint >= 0 && hint < len);

    int ofs = 1;
    int lastOfs = 0;
    if (compareXY(key, a[base + hint]) < 0) {
        // Gallop left until a[b+hint - ofs] <= key < a[b+hint - lastOfs]
        int maxOfs = hint + 1;
        while (ofs < maxOfs && compareXY(key, a[base + hint - ofs]) < 0) {
            lastOfs = ofs;
            ofs = (ofs << 1) + 1;
            if (ofs <= 0)   // int overflow
                ofs = maxOfs;
        }
        if (ofs > maxOfs)
            ofs = maxOfs;

        // Make offsets relative to b
        int tmp = lastOfs;
        lastOfs = hint - ofs;
        ofs = hint - tmp;
    } else { // a[b + hint] <= key
        // Gallop right until a[b+hint + lastOfs] <= key < a[b+hint + ofs]
        int maxOfs = len - hint;
        while (ofs < maxOfs && compareXY(key, a[base + hint + ofs]) >= 0) {
            lastOfs = ofs;
            ofs = (ofs << 1) + 1;
            if (ofs <= 0)   // int overflow
                ofs = maxOfs;
        }
        if (ofs > maxOfs)
            ofs = maxOfs;

        // Make offsets relative to b
        lastOfs += hint;
        ofs += hint;
    }
    // assert(-1 <= lastOfs && lastOfs < ofs && ofs <= len);

    /*
     * Now a[b + lastOfs] <= key < a[b + ofs], so key belongs somewhere to
     * the right of lastOfs but no farther right than ofs.  Do a binary
     * search, with invariant a[b + lastOfs - 1] <= key < a[b + ofs].
     */
    lastOfs++;
    while (lastOfs < ofs) {
        int m = lastOfs + ((ofs - lastOfs) >> 1);

        if (compareXY(key, a[base + m]) < 0)
            ofs = m;          // key < a[b + m]
        else
            lastOfs = m + 1;  // a[b + m] <= key
    }
    // assert(lastOfs == ofs);    // so a[b + ofs - 1] <= key < a[b + ofs]
    return ofs;
}

/**
 * Merges two adjacent runs in place, in a stable fashion.  The first
 * element of the first run must be greater than the first element of the
 * second run (a[base1] > a[base2]), and the last element of the first run
 * (a[base1 + len1-1]) must be greater than all elements of the second run.
 *
 * For performance, this method should be called only when len1 <= len2;
 * its twin, mergeHi should be called if len1 >= len2.  (Either method
 * may be called if len1 == len2.)
 *
 * @param base1 index of first element in first run to be merged
 * @param len1  length of first run to be merged (must be > 0)
 * @param base2 index of first element in second run to be merged
 *        (must be aBase + aLen)
 * @param len2  length of second run to be merged (must be > 0)
 */
//@SuppressWarnings({"unchecked", "rawtypes"})
static void mergeLo(int base1, int len1, int base2, int len2,
        TimsortDataPtr td, COMPAREFN compareXY) {
  // assert(len1 > 0 && len2 > 0 && base1 + len1 == base2);

    // Copy first run into temp array
    ObjPtr *a = td->a; // For performance
    ensureCapacity(len1, td);

    int cursor1 = td->tmpBase; // Indexes into tmp array
    int cursor2 = base2;   // Indexes int a
    int dest = base1;      // Indexes int a
    //System.arraycopy(a, base1, tmp, cursor1, len1);
    memmove(td->tmp+cursor1, a+base1, len1*sizeof(ObjPtr));

    // Move first element of second run and deal with degenerate cases
    a[dest++] = a[cursor2++];
    if (--len2 == 0) {
        //System.arraycopy(tmp, cursor1, a, dest, len1);
        memmove(a+dest, td->tmp+cursor1, len1*sizeof(ObjPtr));
        return;
    }
    if (len1 == 1) {
        //System.arraycopy(a, cursor2, a, dest, len2);
        memmove(a+dest, a+cursor2, len2*sizeof(ObjPtr));
        a[dest + len2] = td->tmp[cursor1]; // Last elt of run 1 to end of merge
        return;
    }

    int minGallop = td->minGallop;  // Use local variable for performance
outer:
    while (1) {
        int count1 = 0; // Number of times in a row that first run won
        int count2 = 0; // Number of times in a row that second run won

        /*
         * Do the straightforward thing until (if ever) one run starts
         * winning consistently.
         */
        do {
	  // assert(len1 > 1 && len2 > 0);
            if (compareXY(a[cursor2], td->tmp[cursor1]) < 0) {
                a[dest++] = a[cursor2++];
                count2++;
                count1 = 0;
                if (--len2 == 0)
                    goto ExitOuter;
            } else {
                a[dest++] = td->tmp[cursor1++];
                count1++;
                count2 = 0;
                if (--len1 == 1)
                    goto ExitOuter;
            }
        } while ((count1 | count2) < minGallop);

        /*
         * One run is winning so consistently that galloping may be a
         * huge win. So try that, and continue galloping until (if ever)
         * neither run appears to be winning consistently anymore.
         */
        do {
	  // assert(len1 > 1 && len2 > 0);
            count1 = gallopRight(a[cursor2], td->tmp, cursor1, len1, 0, td, compareXY);
            if (count1 != 0) {
                //System.arraycopy(tmp, cursor1, a, dest, count1);
                memmove(a+dest, td->tmp+cursor1, count1*sizeof(ObjPtr));
                dest += count1;
                cursor1 += count1;
                len1 -= count1;
                if (len1 <= 1)  // len1 == 1 || len1 == 0
                    goto ExitOuter;
            }
            a[dest++] = a[cursor2++];
            if (--len2 == 0)
                goto ExitOuter;

            count2 = gallopLeft(td->tmp[cursor1], a, cursor2, len2, 0, td, compareXY);
            if (count2 != 0) {
                //System.arraycopy(a, cursor2, a, dest, count2);
                memmove(a+dest, a+cursor2, count2*sizeof(ObjPtr));
                dest += count2;
                cursor2 += count2;
                len2 -= count2;
                if (len2 == 0)
                    goto ExitOuter;
            }
            a[dest++] = td->tmp[cursor1++];
            if (--len1 == 1)
                goto ExitOuter;
            minGallop--;
        } while (count1 >= MIN_GALLOP || count2 >= MIN_GALLOP);
        if (minGallop < 0)
            minGallop = 0;
        minGallop += 2;  // Penalize for leaving gallop mode
    }  // End of "outer" loop
ExitOuter:
    ;
    //minGallop = minGallop < 1 ? 1 : minGallop;  // Write back to field
    td->minGallop = (minGallop < 1)? 1 : minGallop;

    if (len1 == 1) {
      // assert(len2 > 0);
        //System.arraycopy(a, cursor2, a, dest, len2);
        memmove(a+dest, a+cursor2, len2*sizeof(ObjPtr));
        a[dest + len2] = td->tmp[cursor1]; //  Last elt of run 1 to end of merge
    } else if (len1 == 0) {
      // assert("Comparison method violates its general contract!"==NULL);
    } else {
      // assert(len2 == 0);
      // assert(len1 > 1);
        //System.arraycopy(tmp, cursor1, a, dest, len1);
        memmove(a+dest, td->tmp+cursor1, len1*sizeof(ObjPtr));
    }
}

/**
 * Like mergeLo, except that this method should be called only if
 * len1 >= len2; mergeLo should be called if len1 <= len2.  (Either method
 * may be called if len1 == len2.)
 *
 * @param base1 index of first element in first run to be merged
 * @param len1  length of first run to be merged (must be > 0)
 * @param base2 index of first element in second run to be merged
 *        (must be aBase + aLen)
 * @param len2  length of second run to be merged (must be > 0)
 */
//@SuppressWarnings({"unchecked", "rawtypes"})
static void mergeHi(int base1, int len1, int base2, int len2,
        TimsortDataPtr td, COMPAREFN compareXY) {
  // assert(len1 > 0 && len2 > 0 && base1 + len1 == base2);

    // Copy second run into temp array
    ObjPtr *a = td->a; // For performance
    ensureCapacity(len2, td);
    //System.arraycopy(a, base2, tmp, tmpBase, len2);
    memmove(td->tmp+td->tmpBase, a+base2, len2*sizeof(ObjPtr));

    int cursor1 = base1 + len1 - 1;  // Indexes into a
    int cursor2 = td->tmpBase + len2 - 1; // Indexes into tmp array
    int dest = base2 + len2 - 1;     // Indexes into a

    // Move last element of first run and deal with degenerate cases
    a[dest--] = a[cursor1--];
    if (--len1 == 0) {
        //System.arraycopy(tmp, tmpBase, a, dest - (len2 - 1), len2);
        memmove(a+dest-(len2-1), td->tmp+td->tmpBase, len2*sizeof(ObjPtr));
        return;
    }
    if (len2 == 1) {
        dest -= len1;
        cursor1 -= len1;
        //System.arraycopy(a, cursor1 + 1, a, dest + 1, len1);
        memmove(a+dest+1, a+cursor1+1, len1*sizeof(ObjPtr));
        a[dest] = td->tmp[cursor2];
        return;
    }

    int minGallop = td->minGallop;
outer:
    while (1) {
        int count1 = 0; // Number of times in a row that first run won
        int count2 = 0; // Number of times in a row that second run won

        /*
         * Do the straightforward thing until (if ever) one run
         * appears to win consistently.
         */
        do {
	  // assert(len1 > 0 && len2 > 1);
            if (compareXY(td->tmp[cursor2], a[cursor1]) < 0) {
                a[dest--] = a[cursor1--];
                count1++;
                count2 = 0;
                if (--len1 == 0)
                    goto ExitOuter;
            } else {
                a[dest--] = td->tmp[cursor2--];
                count2++;
                count1 = 0;
                if (--len2 == 1)
                    goto ExitOuter;

            }
        } while ((count1 | count2) < minGallop);

        /*
         * One run is winning so consistently that galloping may be a
         * huge win. So try that, and continue galloping until (if ever)
         * neither run appears to be winning consistently anymore.
         */
        do {
	  // assert(len1 > 0 && len2 > 1);
            count1 = len1 - gallopRight(td->tmp[cursor2], a, base1, len1, len1 - 1, td, compareXY);
            if (count1 != 0) {
                dest -= count1;
                cursor1 -= count1;
                len1 -= count1;
                //System.arraycopy(a, cursor1 + 1, a, dest + 1, count1);
                memmove(a+dest+1, a+cursor1+1, count1*sizeof(ObjPtr));
                if (len1 == 0)
                    goto ExitOuter;
            }
            a[dest--] = td->tmp[cursor2--];
            if (--len2 == 1)
                goto ExitOuter;

            count2 = len2 - gallopLeft(a[cursor1], td->tmp, td->tmpBase, len2, len2 - 1, td, compareXY);
            if (count2 != 0) {
                dest -= count2;
                cursor2 -= count2;
                len2 -= count2;
                //System.arraycopy(tmp, cursor2 + 1, a, dest + 1, count2);
                memmove(a+dest+1, td->tmp+cursor2+1, count2*sizeof(ObjPtr));
                if (len2 <= 1)
                    goto ExitOuter; // len2 == 1 || len2 == 0
            }
            a[dest--] = a[cursor1--];
            if (--len1 == 0)
                goto ExitOuter;
            minGallop--;
        } while (count1 >= MIN_GALLOP || count2 >= MIN_GALLOP);
        if (minGallop < 0)
            minGallop = 0;
        minGallop += 2;  // Penalize for leaving gallop mode
    }  // End of "outer" loop
ExitOuter:
    td->minGallop = (minGallop < 1)? 1 : minGallop;

    if (len2 == 1) {
      // assert(len1 > 0);
        dest -= len1;
        cursor1 -= len1;
        //System.arraycopy(a, cursor1 + 1, a, dest + 1, len1);
        memmove(a+dest+1, a+cursor1+1, len1*sizeof(ObjPtr));
        a[dest] = td->tmp[cursor2];  // Move first elt of run2 to front of merge
    } else if (len2 == 0) {
      // assert("Comparison method violates its general contract!" == NULL);
    } else {
      // assert(len1 == 0);
      // assert(len2 > 0);
        //System.arraycopy(tmp, tmpBase, a, dest - (len2 - 1), len2);
        memmove(a+dest-(len2-1), td->tmp+td->tmpBase, len2*sizeof(ObjPtr));
    }
}

/**
 * Ensures that the external array tmp has at least the specified
 * number of elements, increasing its size if necessary.  The size
 * increases exponentially to ensure amortized linear time complexity.
 *
 * @param minCapacity the minimum required capacity of the tmp array
 * @return tmp, whether or not it grew
 */

int flog2(unsigned int n) {
    int result = 0;

    if (n > 0xffff) {
        result = 16;
        n >>= 16;
    }
    if (n > 0xff) {
        result += 8;
        n >>= 8;
    }
    if (n > 0xf) {
        result += 4;
        n >>= 4;
    }
    if (n > 0x3) {
        result += 2;
        n >>= 2;
    }
    return result + (n > 1);
}




static void ensureCapacity(int minCapacity, TimsortDataPtr td) {
    if (td->tmpLen < minCapacity) {
        // Compute smallest power of 2 > minCapacity
        //int newSize = -1 >>> Integer.numberOfLeadingZeros(minCapacity);
        //newSize++;

        //if (newSize < 0) // Not bloody likely!
            //newSize = minCapacity;
        //else
            //newSize = Math.min(newSize, a.length >>> 1);
        int newSize = 1<<flog2(minCapacity) + 1;
        if (newSize > td->len/2) newSize = td->len/2;

        //@SuppressWarnings({"unchecked", "UnnecessaryLocalVariable"})
        if (td->tmp != NULL)
            td->tmp = myRealloc("ensureCapacity", td->tmp, newSize*sizeof(ObjPtr));
        else
            td->tmp = myCalloc("ensureCapacity", newSize, sizeof(ObjPtr));
        td->tmpLen = newSize;
        td->tmpBase = 0;
    }
}

#undef ObjPtr

