rm src/*.h.gch
gcc -o build/cbclassifier src/*.c -Wall -ansi -pedantic -lm
