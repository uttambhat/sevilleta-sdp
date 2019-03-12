set xrange[0:4000]; set yrange[0:100]; unset key; 
set term post eps enh color size 10cm,15cm font "Times-Roman, 15"; set out "plots/Fitness_matrix.eps";
set multiplot layout 5, 1 title "Fitness matrices" font ",20";
set tmargin 3
set title "T=00"
plot "data/fitness/T=00" matrix w image;
set title "T=30"
plot "data/fitness/T=30" matrix w image;
set title "T=50"
plot "data/fitness/T=50" matrix w image;
set title "T=70"
plot "data/fitness/T=70" matrix w image;
set title "T=90"
plot "data/fitness/T=90" matrix w image;
unset multiplot; set term pop;

