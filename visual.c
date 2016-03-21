#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "SDL.h"

#define WIDTH 1024
#define HEIGHT 768

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
		fread (screen->pixels, sizeof (Uint32), WIDTH*HEIGHT, f);
		b += WIDTH * sizeof (Uint32);
		
		SDL_UpdateRect(screen, 0, 0, WIDTH, HEIGHT);
	}
}

int main (int argc, char *argv[])
{
	int res;
	
	assert (argc == 2);

	f = fopen (argv[1], "r");

	res = SDL_Init (SDL_INIT_VIDEO);
	assert (res >= 0);
	
	atexit (SDL_Quit);
	
	screen = SDL_SetVideoMode (WIDTH, HEIGHT, 32, SDL_SWSURFACE);
	assert (screen);
	
	SDL_ShowCursor (SDL_DISABLE);

	buf = malloc (WIDTH * HEIGHT * sizeof (Uint32));

	loop();
}
