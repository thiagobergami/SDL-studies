//Using SDL and standard IO
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>

// Start SDP and create a windows
bool init();
bool loadMedia();
void close();


//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

enum KeyPressSurfaces
{
	KEY_PRESS_SURFACE_DEFAULT,
	KEY_PRESS_SURFACE_UP,
	KEY_PRESS_SURFACE_DOWN,
	KEY_PRESS_SURFACE_LEFT,
	KEY_PRESS_SURFACE_RIGHT,
	KEY_PRESS_SURFACE_TOTAL
};

SDL_Surface* loadSurface(std::string path);

//Loads individual image as texture
SDL_Texture* loadTexture(std::string path);

SDL_Window* gWindow = NULL;

SDL_Surface* gScreenSurface = NULL;

SDL_Surface* gCurrentSurface  = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Current displayed texture
SDL_Texture* gTexture = NULL;

SDL_Texture* gKeyPressSurfaces[KEY_PRESS_SURFACE_TOTAL];



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
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled!");
		}

		gWindow = SDL_CreateWindow("Thiago's SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Window could net be created! SDL_Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			gRenderer = SDL_CreateRenderer(gWindow, -1 , SDL_RENDERER_ACCELERATED);
			if (gRenderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}
			}
			
		}
	}

	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Load default surface
	gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT] = loadTexture("keys/press.bmp");
	if (gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT] == NULL)
	{
		printf("Failed to load default image!\n");
		success = false;
	}

	//Load up surface
	gKeyPressSurfaces[KEY_PRESS_SURFACE_UP] = loadTexture("keys/up.bmp");
	if (gKeyPressSurfaces[KEY_PRESS_SURFACE_UP] == NULL)
	{
		printf("Failed to load up image!\n");
		success = false;
	}

	//Load down surface
	gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN] = loadTexture("keys/down.bmp");
	if (gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN] == NULL)
	{
		printf("Failed to load down image!\n");
		success = false;
	}

	//Load left surface
	gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT] = loadTexture("keys/left.bmp");
	if (gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT] == NULL)
	{
		printf("Failed to load left image!\n");
		success = false;
	}

	//Load right surface
	gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT] = loadTexture("keys/right.bmp");
	if (gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT] == NULL)
	{
		printf("Failed to load right image!\n");
		success = false;
	}


	return success;
}

void close()
{
	//Free Loaded image
	SDL_DestroyTexture(gTexture);
	gTexture = NULL;

	//Destroy window	
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

SDL_Texture* loadTexture(std::string path)
{	
	//The final texture
	SDL_Texture* newTexture = NULL;

	SDL_Surface* loadedSurface = IMG_Load(path.c_str());

	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL Error: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if (newTexture == NULL)
		{
			printf("Unable to optimize image %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}

		SDL_FreeSurface(loadedSurface);
		
	}
	return newTexture;
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

			gTexture = gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];

			while (!quit)
			{
				while (SDL_PollEvent(&e) != 0)
				{
					if (e.type == SDL_QUIT)
					{
						quit = true;
					}
					//User presses a key
					else if (e.type == SDL_KEYDOWN)
					{
						//Select surfaces based on key press
						switch (e.key.keysym.sym)
						{
						case SDLK_UP:
							gTexture = gKeyPressSurfaces[KEY_PRESS_SURFACE_UP];
							break;

						case SDLK_DOWN:
							gTexture = gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN];
							break;

						case SDLK_LEFT:
							gTexture = gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT];
							break;

						case SDLK_RIGHT:
							gTexture = gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT];
							break;

						default:
							gTexture = gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];
							break;
						}
					}
				}

				//Clear screen
				SDL_RenderClear(gRenderer);

				//Render texture to screen
				SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);

				//Update screen
				SDL_RenderPresent(gRenderer);

				//SDL_Rect stretchRect;
				//stretchRect.x = 0;
				//stretchRect.y = 0;
				//stretchRect.w = SCREEN_WIDTH;
				//stretchRect.h = SCREEN_HEIGHT;

				//SDL_BlitScaled(gCurrentSurface, NULL, gScreenSurface, &stretchRect);

				////Update the surface
				//SDL_UpdateWindowSurface(gWindow);
			}
		}			
	}

	close();

	return 0;
}