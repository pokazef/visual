#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "SDL.h"

SDL_Window *win;
SDL_Renderer *ctx;
SDL_Texture *tex;	

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
		fread (buf, sizeof (Uint32), 1024*768, f);
		b += 1024 * sizeof (Uint32);
		
		SDL_UpdateTexture (tex, NULL, buf, 1024 * sizeof (Uint32));
		SDL_RenderClear (ctx);
		SDL_RenderCopy (ctx, tex, NULL, NULL);
		SDL_RenderPresent (ctx);
	}
}

int main (int argc, char *argv[])
{
	win = SDL_CreateWindow
	(
		"visual",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		1024, 768,
		0 //SDL_WINDOW_FULLSCREEN | SDL_WINDOW_OPENGL
	);
	assert (win);
	
	ctx = SDL_CreateRenderer (win, -1, 0 /*SDL_RENDERER_PRESENTVSYNC*/);
	assert (ctx);
	
	tex = SDL_CreateTexture
	(
		ctx,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		1024, 768
	);
	assert (tex);
	
	buf = malloc (1024 * 768 * sizeof (Uint32));
	
	f = fopen ("data.bin", "r");
	loop();
	
	SDL_Quit();
}
