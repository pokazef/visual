#include <stdlib.h>
#include <stdio.h>
#include <strings.h>

#include "SDL.h"

#define WIDTH 1024
#define HEIGHT 768

SDL_Surface *screen;

Uint32 *buf;

FILE *f;

int move_x = 0;
int move_y = 0;
int zoom = 1;

int base = 0;

long long map (int line)
{
	int res;
	
	res = base + line * zoom;
	
	return res;
}

void loop (void)
{
	SDL_Event event;
	int done = 0;
	long long a, b = 0;
	int res;

	while (! done)
	{
		while (SDL_PollEvent (&event)) {
			switch (event.type) {
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
					case SDLK_ESCAPE:
					case 'q':
						done = 1;
					break;

					case SDLK_UP:
						move_y-=1;
					break;

					case SDLK_DOWN:
						move_y+=1;
					break;

					case SDLK_LEFT:
						move_x-=1;
					break;

					case SDLK_RIGHT:
						move_x+=1;
					break;

					case SDLK_PAGEUP:
						move_y-=10;
					break;

					case SDLK_PAGEDOWN:
						move_y+=10;
					break;
					
					case 'a':
						zoom = zoom * 2;
					break;

					case 'z':
						zoom = zoom / 2;
						if (zoom == 0) zoom = 1;
					break;
				}
				break;
			case SDL_KEYUP:
				switch (event.key.keysym.sym)
				{
					case SDLK_UP:
						move_y+=1;
					break;

					case SDLK_DOWN:
						move_y-=1;
					break;

					case SDLK_LEFT:
						move_x+=1;
					break;

					case SDLK_RIGHT:
						move_x-=1;
					break;

					case SDLK_PAGEUP:
						move_y+=10;
					break;

					case SDLK_PAGEDOWN:
						move_y-=10;
					break;
				}
				break;
			case SDL_QUIT:
				done = 1;
				break;
			}
		}

		for (a = -HEIGHT/2; a < HEIGHT/2; a++)
		{
			res = fseeko (f, map(a)*WIDTH*4, SEEK_SET);
			if (res == 0)
				res = fread (screen->pixels + (a+HEIGHT/2)*WIDTH*4, 4, WIDTH, f);
			if (res <= 0)
				memset (screen->pixels + (a+HEIGHT/2)*WIDTH*4, 0x20, WIDTH*4);
		}
		base += move_y * zoom;
		
		SDL_UpdateRect (screen, 0, 0, WIDTH, HEIGHT);
	}
}

void bail (char *msg)
{
	puts (msg);
	exit (1);
}

int main (int argc, char *argv[])
{
	int res;
	
	if (argc < 2) bail ("Not enough arguments");

	f = fopen (argv[1], "r");
	if (! f) bail ("Couldn't open file");

	res = SDL_Init (SDL_INIT_VIDEO);
	if (res < 0) bail ("SDL_Init");
	
	atexit (SDL_Quit);
	
	screen = SDL_SetVideoMode (WIDTH, HEIGHT, 32, SDL_HWSURFACE);
	if (! screen) bail ("SDL_SetVideoMode");
	
	SDL_ShowCursor (SDL_DISABLE);

	buf = malloc (WIDTH * HEIGHT * sizeof (Uint32));

	loop();
}
