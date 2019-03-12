#!/bin/bash
echo Running ksim.out...
time ./bin/ksim.out $(grep -v '^#' data/parameters.dat)
echo Finished running ksim.out.
echo Running main.out...
time ./bin/main.out $(grep -v '^#' data/parameters.dat)
echo Finished running main.out.
echo Plotting...
rm plots/Decision_matrix.eps
gnuplot src/gnuplot.gnu
echo Done\!
