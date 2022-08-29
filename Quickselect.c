// quickselect.c
//
// Original source code taken from
//    https://en.wikipedia.org/wiki/Quickselect
// and heavily modified by
//    Nigel Horspool, 2021-06-04
// It differs from the usual quickselect in that the function
// is allowed to place the pivot up to DELTA positions away
// from the middle of the range (where a median should be)

// fully sort the list with insertion sort if no more
// elements than this
#define SMALL_LIST   9

// we allow the k-th smallest element to end up DELTA
// positions away from position k
#define DELTAA 2

// swap two elements of array A given their indexes p and q
#define ISWAP(p, q, A) { void *t3t = A[p]; A[p] = A[q]; A[q] = t3t; }


static int partitionqs(void **list, int left, int right, int (*cmp)());


// Partially sorts list such that the k-th smallest element of list
// within left..right inclusive is close to its correct position and
// all elements to the left are less or equal to this element and
// all elements to the right are greater or equal
int quickselectm(void **list, int left, int right, int k,
        int depthLimit, int (*cmp)()) {
    int i = left;
    int j = right;
    for( ; ; ) {
        int size = j - i + 1;
        if (size <= SMALL_LIST) {
            if (size > 1) insertionsort(list, i, j, cmp);
            break;          //    list[k] holds the desired element
        }
        if (depthLimit-- <= 0) {
            heapc(list, left, right, cmp);
            break;
        }
        // Use a median of 3 elements as the pivot.
        // These 3 elements are at the start, the middle and
        // the end. We reorder these elements so that the
        // smallest is at the start, the largest is at the end
        // and the median gets swapped into position left+1. 
        int p = (i+j)>>1;
        if (cmp(list[i],list[p]) > 0) ISWAP(i,p,list);
        if (cmp(list[i],list[j]) > 0) ISWAP(i,j,list);
        int r = cmp(list[p],list[j]);
        if (r == 0) {
            // trouble -- we don't have a sentinel at right end
            // and we MUST have one. (The left side is safe.)
            dflgm3(list, i, j, depthLimit, cmp);
            break;
        }
        if (r > 0) ISWAP(p,j,list);
        ISWAP(i+1,p,list);
        int pivotIndex = partitionqs(list, i+1, j, cmp);
        int diff = k - pivotIndex;
        if (diff < 0) {
            if ((-diff) <= DELTAA) return pivotIndex;
            j = pivotIndex - 1;
        } else {
            if (diff <= DELTAA) return pivotIndex;
            i  = pivotIndex + 1;
        }
    }
    return k;
}

// reorders the elements of A from lo to hi such that all
// elements less or equal to A[lo] occur at the left and
// all elements greater than A[lo] occur at the right with
// the new position of A[lo] being at location A[j],
// where j is the int result of the call
static int partitionqs(void **A, int lo, int hi, int (*cmp)()) {
    void *pivot = A[lo];
    int i = lo+1;
    int j = hi;
    for( ; ; ) {
        while (cmp(A[i],pivot) <= 0)
            i++;
        while (cmp(A[j],pivot) > 0)
            j--;
        if (i >= j)
            break;
        ISWAP(i, j, A);
        i++;  j--;
    }
    // move pivot into correct position in array
    A[lo] = A[j];
    A[j] = pivot;
    return j;
}

#undef SMALL_LIST
#undef DELTA
#undef ISWAP
