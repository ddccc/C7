// c:/bsd/rigel/sort/C2Split.c
// Date: Tue Apr 28 13:34:34 2020
// (C) OntoOO/ Dennis de Champeaux

#define iswap(p, q, A) { void *t3t = A[p]; A[p] = A[q]; A[q] = t3t; }

// const int cut2split =  256*1024; // 
// const int cut2split =  2*1024*1024; // 
// const int cut2split =  1024*1024; // 

// const int cut2split =  600*1024; // 11886 
static const int cut2split =  512*1024; //  11793
// const int cut2split =  400*1024; // 11981

// const int cut2split2 = 256*1024; // 11872
// const int cut2split2 = 200*1024; // 11995
static const int cut2split2 = 128*1024; // 11793
// const int cut2split2 = 64*1024; // 11964
// const int cut2split2 = 32*1024; //
// const int cut2split2 = 16*1024; //

static const int cut2Limitx =  1150; 

/* Infrastructure for the tasks:
  To obtain the int n field from X: ((struct task *) X)->n
  To obtain the int m field from X: ((struct task *) X)->m
  To obtain the task next field from X: ((struct task *) X)->next
  */

static void *myMalloc();

struct task {
  int n;
  int m;
  struct task *next;
};
int getN(struct task *t) { return ((struct task *) t)->n; }
int getM(struct task *t) { return ((struct task *) t)->m; }
struct task *getNext(struct task *t) { return ((struct task *) t)->next; }

void setN(struct task *t, int n) { ((struct task *) t)->n = n; }
void setM(struct task *t, int m) { ((struct task *) t)->m = m; }
void setNext(struct task *t, struct task* tn) { 
  ((struct task *) t)->next = tn; }

int taskCnt = 0;

struct task *newTask(int N, int M) {
  taskCnt++;
  struct task *t = (struct task *) 
    myMalloc("Called by: newTask()", sizeof (struct task));
  setN(t, N); setM(t, M);  setNext(t, NULL);
  return t;
} // end newTask

struct stack {
  struct task *first;
  int size;
};
struct task *getFirst(struct stack *ll) { 
  return ((struct stack *) ll)->first; }
void setFirst(struct stack *ll, struct task *t) { 
  ((struct stack *) ll)->first = t; }
struct stack *newStack() {
  struct stack *ll = (struct stack *)
    myMalloc("Called by: newstack()", sizeof (struct stack));
  setFirst(ll, NULL);
  return ll;
} // end newStack
int isEmpty(struct stack *ll) { 
  return ( NULL == getFirst(ll) ); 
} // end isEmpty
struct task *pop(struct stack *ll) {
  struct task *t = getFirst(ll);
  if ( NULL == t ) return NULL;
  setFirst(ll, getNext(t));
  return t;
} // end pop
void push(struct stack *ll, struct task *t) {
  if ( !isEmpty(ll) ) setNext(t, getFirst(ll)); 
  setFirst(ll, t);
} // end push

static void c21c();

void c21(void **A, int N, int M, int (*compare)()) { 
  // printf("c21 %d %d %d\n", N, M, M-N);
  int L = M - N;
  if ( L <= 0 ) return;
  if ( L < cut2Limitx ) { 
    quicksort0(A, N, M, compare);
    return;
  }
  // int depthLimit = 2.5 * floor(log(L));
  c21c(A, N, M, compare);
} // end cut2


static int compareTask (const void *a, const void *b)
{
  struct task *pa = (struct task *) a;
  struct task *pb = (struct task *) b;
  return (pa->n - pb->n);
}

void c22c();
void c21c(void **A, int N, int M, int (*compare)()) {
  // printf("c21c %d %d %d\n", N, M, M-N);
  int L;
  L = M - N + 1;
  int depthLimit = 2.5 * floor(log(L));
  if ( L < cut2split ) { 
    cut2c(A, N, M, depthLimit, compare);
    return;
  }
  taskCnt = 0;
  struct stack* stck = newStack();
  c22c(A, N, M, depthLimit, compare, stck, cut2split);
  struct task *ti;
  // printf("taskCnt %i\n", taskCnt);
  void **tasks = myMalloc("c21c", sizeof(ti) * taskCnt);
  int numtask = 0;
  while (1) {
    struct task* tk = pop(stck);
    if (NULL == tk) break;
    tasks[numtask] = tk;
    numtask++;
  }
  // printf("numtask: %i \n", numtask);
  quicksort0(tasks, 0, numtask-1, compareTask);
  int j;
  taskCnt = 0;
  for (j = 0; j < numtask; j++) {
    struct task* tk = tasks[j];
    N = getN(tk); M = getM(tk);  L = M - N + 1;
    free(tk);
    // printf("j %i N %i M %i\n", j, N, M);
    depthLimit = 2.5 * floor(log(L));
    c22c(A, N, M, depthLimit, compare, stck, cut2split2);
  }
  free(tasks);
  // printf("taskCnt %i\n", taskCnt);
  tasks = myMalloc("c21c", sizeof(ti) * taskCnt);
  numtask = 0;
  while (1) {
    struct task* tk = pop(stck);
    if (NULL == tk) break;
    tasks[numtask] = tk;
    numtask++;
  }
  quicksort0(tasks, 0, numtask-1, compareTask);
  for (j = 0; j < numtask; j++) {
    struct task* tk = tasks[j];
    N = getN(tk); M = getM(tk);  L = M - N + 1;
    free(tk);
    // printf("j %i N %i M %i\n", j, N, M);
    depthLimit = 2.5 * floor(log(L));
    cut2c(A, N, M, depthLimit, compare);
  }
  free(tasks);

  free(stck);
}

void c22c(void **A, int N, int M, int depthLimit, 
	  int (*compare)(), struct stack* stck, int splitter) {
  int L;
 Start:
  L = M - N + 1;
  if ( L <= splitter) {
    struct task * tk = newTask(N, M);
    push(stck, tk);
    return;
  }
  if ( depthLimit <= 0 ) {
    heapc(A, N, M, compare);
    return;
  }
  depthLimit--;


  register void *T; // pivot
  register int I = N, J = M; // indices
  int middlex = N + (L>>1); // N + L/2
  void *middle;

   int k, N1, M1; // for sampling
    // int middlex = N + (L>>1); // N + L/2

    int probeLng = sqrt(L/9);
    int halfSegmentLng = probeLng >> 1; // probeLng/2;
    //    int quartSegmentLng = probeLng >> 2; // probeLng/4;
    N1 = middlex - halfSegmentLng; //  N + (L>>1) - halfSegmentLng;
    M1 = N1 + probeLng - 1;
    int offset = L/probeLng;  

    // assemble the mini array [N1, M1]
    for (k = 0; k < probeLng; k++) // iswap(N1 + k, N + k * offset, A);
      { int xx = N1 + k, yy = N + k * offset; iswap(xx, yy, A); }
    // sort this mini array to obtain good pivots
    if ( probeLng < 120 ) quicksort0c(A, N1, M1, depthLimit, compare); else {
      // protect against constant arrays
      int p0 = N1 + (probeLng>>1);
      int pn = N1, pm = M1, d = (probeLng-3)>>3;
      pn = med(A, pn, pn + d, pn + 2 * d, compare);
      p0 = med(A, p0 - d, p0, p0 + d, compare);
      pm = med(A, pm - 2 * d, pm - d, pm, compare);
      p0 = med(A, pn, p0, pm, compare);
      if ( p0 != middlex ) iswap(p0, middlex, A); 
      dflgm(A, N1, M1, middlex, quicksort0c, depthLimit, compare);
    }
    T = middle = A[middlex];
    if ( compare(A[M1], middle) <= 0 ) {
      // give up because cannot find a good pivot
      // dflgm is a dutch flag type of algorithm
      void cut2c();
      dflgm(A, N, M, middlex, cut2c, depthLimit, compare);
      return;
    }
    for ( k = N1; k <= middlex; k++ ) {
    iswap(k, I, A); I++;
    }
    I--;
    for ( k = M1; middlex < k; k--) {
      iswap(k, J, A); J--;
    }
    J++;


 register void *AI, *AJ; // array values
	// The left segment has elements <= T
	// The right segment has elements >= T
  Left:
	while ( compare(A[++I], T) <= 0 ); 
	AI = A[I];
	while ( compare(T, A[--J]) < 0 ); 
	AJ = A[J];
	if ( I < J ) { // swap
	  A[I] = AJ; A[J] = AI;
	  goto Left;
	}
	// Tail iteration
	if ( (I - N) < (M - J) ) { // smallest one first
	  c22c(A, N, J, depthLimit, compare, stck, splitter);
	  N = I; 
	  goto Start;
	}
	c22c(A, I, M, depthLimit, compare, stck, splitter);
	M = J;
	goto Start;
}

#undef iswap




  
