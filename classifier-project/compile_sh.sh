rm src/*.h.gch
gcc -Iinclude "src/ext/*.c" "src/io/*.c" "src/core/*.c" "src/main.c" -o "bin/classifier.exe" -Wall -ansi -pedantic -lm
