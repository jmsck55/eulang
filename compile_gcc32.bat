gcc -c -Wall -Werror -fpic -std=c99 -DOLD_X86 -Dunix -DNO_INT128 data.c
gcc -shared -o libeulang.dll data.o -lquadmath
