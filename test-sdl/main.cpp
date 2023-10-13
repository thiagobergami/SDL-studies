//Using SDL and standard IO
#include <SDL.h>
#include <stdio.h>


// Start SDP and create a windows
bool init();
bool loadMedia();
void close();

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

SDL_Window* gWindow = NULL;

SDL_Surface* gScreenSurface = NULL;

SDL_Surface* gHelloWorld = NULL;

enum KeyPressSurfaces
{
	KEY_PRESS_SURFACE_DEFAULT,
	KEY_PRESS_SURFACE_UP,
	KEY_PRESS_SURFFACE_DOWN,
	KEY_PRESS_SURFACE_LEFT,
	KEY_PRESS_SURFACE_RIGHT,
	KEY_PRESS_SURFACE_TOTAL
};

bool init()
{
	bool success = true;

	if ( SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not be initialize! SDL_ERROR: %s\n", SDL_GetError());
		success = false;
	}
	else
	{	
		
		gWindow = SDL_CreateWindow("Thiago's SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Window could net be created! SDL_Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			gScreenSurface = SDL_GetWindowSurface(gWindow);
		}
	}

	return success;
}

bool loadMedia()
{
	bool success = true;
	const char* image_path = "hello_world.bmp";
	gHelloWorld = SDL_LoadBMP(image_path);

	if (gHelloWorld == NULL)
	{
		printf("Unable to load image %s! SDL Error: %s\n", "hello_world.bmp", SDL_GetError());
		success = false;
	}

	return success;
}

void close()
{
	SDL_FreeSurface(gHelloWorld);
	gHelloWorld = NULL;

	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	SDL_Quit();
}

int main(int argc, char* args[])
{
	//Start up SDL and create window
	if (!init())
	{
		printf("Falied to initialize!\n");
	}
	else
	{
		if (!loadMedia())
		{
			printf("Falied to load media!\n");
		}
		else
		{	
			bool quit = false;
			// Event handler
			SDL_Event e;

			while (!quit)
			{
				while (SDL_PollEvent(&e) != 0)
				{
					if (e.type == SDL_QUIT)
					{
						quit = true;
					}
				}

				SDL_BlitSurface(gHelloWorld, NULL, gScreenSurface, NULL);

				SDL_UpdateWindowSurface(gWindow);
			}
		}			
	}

	close();

	return 0;
}