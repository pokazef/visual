#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "SDL.h"

SDL_Surface *screen;

Uint32 *buf;

FILE *f;

void loop (void)
{
	SDL_Event event;
	int done = 0;
	int a, b = 0;

	while (! done)
	{
		while (SDL_PollEvent (&event)) {
			switch (event.type) {
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_ESCAPE
				 || event.key.keysym.sym == 'q') {
					done = 1;
				}
				break;
			case SDL_QUIT:
				done = 1;
				break;
			}
		}

		fseek (f, b, SEEK_SET);
		if (SDL_MUSTLOCK(screen)) 
			if (SDL_LockSurface(screen) < 0) 
				return;
		fread (screen->pixels, sizeof (Uint32), 1024*768, f);
		if (SDL_MUSTLOCK(screen)) 
			SDL_UnlockSurface(screen);
		b += 1024 * sizeof (Uint32);
		
		SDL_UpdateRect(screen, 0, 0, 1024, 768);
	}
}

int main (int argc, char *argv[])
{
	int res;
	
	res = SDL_Init (SDL_INIT_VIDEO);
	assert (res >= 0);
	
	atexit (SDL_Quit);
	
	screen = SDL_SetVideoMode (1024, 768, 32, SDL_SWSURFACE);
	assert (screen);
	
	buf = malloc (1024 * 768 * sizeof (Uint32));
	
	f = fopen ("data.bin", "r");
	loop();
}
