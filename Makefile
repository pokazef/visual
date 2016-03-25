visual: visual.c
	gcc -D_FILE_OFFSET_BITS=64 -I/usr/include/SDL -lSDL -o visual visual.c
