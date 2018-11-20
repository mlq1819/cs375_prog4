C++
make
It seems to work fine (for dynamic programming, at least); 
there is the occasional bug that it will not read the file
if the output already has stuff in it; to fix this, simply
remove the output file
sample command:
$rm output.txt;./program4 smallInput.txt output.txt 3;cat output.txt
Also, the program pre-sorts the array with quicksort before using the
dynamic programming algorithm; this may effect running time