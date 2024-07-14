#David Pérez Sampedro - 48774775D

# Guardar la gráfica como quickSort.png
set terminal pngcairo enhanced font 'arial,10' size 800, 600
set output 'quickSort.png'

################################################
# quickSort.png
set output "quickSort.png"

# Establecer la leyenda
set key top right

set title "Quicksort"

set xlabel "Array size"
set ylabel "Millions of program steps"

set style line 1 lc rgb '#ce33ff' lt 1 lw 1 pt 7 ps 0.2
set style line 2 lc rgb '#009900' lt 1 lw 1 pt 7 ps 0.2
set style line 3 lc rgb '#0066cc' lt 1 lw 1 pt 7 ps 0.2

plot "qs-vs-hs.Msteps" using 1:2 with lines ls 1 title "random arrays", \
     "qs-vs-hs.Msteps" using 1:4 with lines ls 2 title "sorted arrays", \
     "qs-vs-hs.Msteps" using 1:6 with lines ls 3 title "reverse sorted arrays"

################################################
# heapSort.png
set output "heapSort.png"

# Establecer la leyenda
set key top right

set title "Heapsort"

set xlabel "Array size"
set ylabel "Millions of program steps"

set style line 1 lc rgb '#ce33ff' lt 1 lw 1 pt 7 ps 0.2
set style line 2 lc rgb '#009900' lt 1 lw 1 pt 7 ps 0.2
set style line 3 lc rgb '#0066cc' lt 1 lw 1 pt 7 ps 0.2

plot "qs-vs-hs.Msteps" using 1:3 with lines ls 1 title "random arrays", \
     "qs-vs-hs.Msteps" using 1:5 with lines ls 2 title "sorted arrays", \
     "qs-vs-hs.Msteps" using 1:7 with lines ls 3 title "reverse sorted arrays"


################################################
# qs-vs-hs-RA.png
set output "qs-vs-hs-RA.png"

set title  "Quicksort versus Heapsort (random arrays)"

set xlabel "Array size"
set ylabel "Millions of program steps"

set style line 1 lc rgb '#b833ff' lt 1 lw 1 pt 7 ps 0.2
set style line 2 lc rgb '#33ff55' lt 1 lw 1 pt 7 ps 0.2
set style line 3 lc rgb '#ffca33' lt 1 lw 1
set style line 4 lc rgb '#33fff6' lt 1 lw 1

f1(x) = a*x*log(x) + b
fitFunction1 = "fitting Quicksort time values to f(n) = n log(n)+k"
fit f1(x) "qs-vs-hs.Msteps" using 1:2 via a,b

f2(x) = c*x*log(x) + d
fitFunction2 = "fittig Heapsort time values to f(n) = n log(n)+k"
fit f2(x) "qs-vs-hs.Msteps" using 1:3 via c,d

# Plotear los puntos y las funciones ajustadas en una sola gráfica
plot "qs-vs-hs.Msteps" using 1:2 with lines ls 1 title "Quicksort", \
     "" using 1:3 with lines ls 2 title "Heapsort", \
     f1(x) title fitFunction1 with lines ls 3, \
     f2(x) title fitFunction2 with lines ls 4

################################################
# qs-vs-hs-SA.png
set output "qs-vs-hs-SA.png"

set title  "Quicksort versus Heapsort (sorted arrays)"

set xlabel "Array size"
set ylabel "Millions of program steps"

f3(x) = e*x*log(x) + f
fitFunction3 = "fitting Quicksort time values to f(n) = n log(n)+k"
fit f3(x) "qs-vs-hs.Msteps" using 1:4 via e,f

f4(x) = g*x*log(x) + h
fitFunction4 = "fittig Heapsort time values to f(n) = n log(n)+k"
fit f4(x) "qs-vs-hs.Msteps" using 1:5 via g,h

# Plotear los puntos y las funciones ajustadas en una sola gráfica
plot "qs-vs-hs.Msteps" using 1:4 with lines ls 1 title "Quicksort", \
     "" using 1:5 with lines ls 2 title "Heapsort", \
     f3(x) title fitFunction3 with lines ls 3, \
     f4(x) title fitFunction4 with lines ls 4

################################################
# qs-vs-hs-RSA.png
set output "qs-vs-hs-RSA.png"

set title  "Quicksort versus Heapsort (reverse sorted arrays)"

set xlabel "Array size"
set ylabel "Millions of program steps"

f5(x) = i*x*log(x) + j
fitFunction5 = "fitting Quicksort time values to f(n) = n log(n)+k"
fit f5(x) "qs-vs-hs.Msteps" using 1:6 via i,j

f6(x) = k*x*log(x) + l
fitFunction6 = "fittig Heapsort time values to f(n) = n log(n)+k"
fit f6(x) "qs-vs-hs.Msteps" using 1:7 via k,l

# Plotear los puntos y las funciones ajustadas en una sola gráfica
plot "qs-vs-hs.Msteps" using 1:6 with lines ls 1 title "Quicksort", \
     "" using 1:7 with lines ls 2 title "Heapsort", \
     f5(x) title fitFunction5 with lines ls 3, \
     f6(x) title fitFunction6 with lines ls 4