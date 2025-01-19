File: c:/bsd/rigel/sort/C7/aaReadMe.txt
Date: Sun Mar 24 07:47:14 2024

A paper describing the algorithms:  
    Hybrid.pdf

The driver is;
-- ComparisonCounting.c
This one deals with sequential algorithms for referenced items
It reports comparison counts and timings - clock ticks.
The timings are slow because the comparison counts are expensive.

Instructions for how to run the driver are in the top - are self
explanatory. 

To make a driver for sorting referenced items one needs:
- a comparison function
- for cut2 the files: Isort, Hsort, Dsort, C2Sort
- for cut2lr2: the files Isort, Hsort, D3sort, Dsort, C2LR2
- the 3-pivot versions:
  for cut4: the cut2lr2 files & C4
  for cut4d: the cut2lr2 files & CD4
  for cut4n: the cut2lr2 files & CN4
  for cut7: the cut2lr2 files & C7
The file ComparisonCounting.c does all this for all of them.

The .h files loads the corresponding file with:
   #include "XXX.c"
for convenience because recompilation is fast enough here.

