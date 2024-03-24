File: c:/bsd/rigel/sort/C7/aaReadMe.txt
Date: Sun Mar 24 07:47:14 2024

A paper describing the algorithms:  
    Hybrid.pdf

There are three drivers;
-- ComparisonCounting.c
This one deals with sequential algorithms.
It reports comparison counts and timings - clock tics.
The timings are slow because the comparison counts are expensive.

-- ComparisonCountingp.c
This one deals with the parallel versions.
It reports real time timings

-- ComparisonCountings.c
This one deals with sequential algorithms.
It reports real time timings only.

Instructions for how to run the drivers are in the top of these file.

To make a driver for sorting referenced items one needs:
- a comparison function
- for foursort/cut2 the files: Isort, Hsort, Dsort, C2Sort, FourSort
- for cut2lr2: the files Isort, Hsort, D3sort, Dsort, C2LR2
- the 3-pivot versions:
  for sixsort/cut4: the cut2lr2 files & C4
  for cut4d: the cut2lr2 files & CD4
  for cut4n: the cut2lr2 files & CN4
  for cut7: the cut2lr2 files & C7
The file ComparisonCounting.c does all this for all of them.

The .h files loads the corresponding file with:
   #include "XXX.c"
for convenience because recompilation is fast enough here.

