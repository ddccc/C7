File: c:/bsd/rigel/sort/C7/aaReadMe.txt
Date: Sat Dec 14 07:03:07 2019

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
- for cut2lr: the foursort/cut2 files & C2LR
- the 3-pivot versions:
  for sixsort/cut4: the cut2lr files & C4
  for cut4d: the cut2lr files & CD4
  for cut4n: the cut2lr files & CN4
  for cut7: the cut2lr files & C7
The file ComparisonCounting.c does all this for all of them.
