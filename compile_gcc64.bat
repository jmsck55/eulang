gcc -c -Wall -Werror -fpic -std=c99 -Dunix -DNO_INT128 data.c
gcc -shared -o libeulang.dll data.o -lquadmath
