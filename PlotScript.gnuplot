set boxwidth 0.5
set style fill solid

set term png
set output "R1.png"
plot "R1.txt" using 2:xticlabel(1) with boxes

set term png
set output "R2.png"
plot "R2.txt" using 2:xticlabel(1) with boxes

set term png
set output "R3.png"
plot "R3.txt" using 2:xticlabel(1) with boxes

set term png
set output "R4.png"
plot "R4.txt" using 2:xticlabel(1) with boxes

set term png
set output "R5.png"
plot "R5.txt" using 2:xticlabel(1) with boxes

set term png
set output "RTotal.png"
plot "RTotal.txt" using 2:xticlabel(1) with boxes