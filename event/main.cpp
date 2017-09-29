
#include <SDL2/SDL.h>
#include <stdio.h>


#undef main


const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

bool init();
bool loadMedia();
void close();



SDL_Window* gWindow = nullptr;
SDL_Surface* gScreenSurface = nullptr;
SDL_Surface* gXSurface = nullptr;


int main(int argc, char* args[]) {


	if (!init()) {
		printf("Failed to initialize!\n");
	} else {
		if (!loadMedia()) {
			printf("Failed to load media!\n");
		} else {
			SDL_BlitSurface(gXSurface, nullptr, gScreenSurface, nullptr);
			SDL_UpdateWindowSurface(gWindow);

			SDL_Event e;
			bool quit = false;
			while (!quit) {

				while (SDL_PollEvent(&e) != 0) {
					if (e.type == SDL_QUIT) {
						quit = true;
					}
				}
			}
		}
	}

	close();

	return 0;
}


bool init() {
	bool success = true;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		success = false;
	} else {
		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == nullptr) {
			printf("Window could not be crated! SDL_Error %s\n", SDL_GetError());
		} else {
			gScreenSurface = SDL_GetWindowSurface(gWindow);
		}
	}

	return success;
}


bool loadMedia() {
	bool success = true;
	gXSurface = SDL_LoadBMP("x.bmp");
	if (gXSurface == nullptr) {
		printf("Unable to load image %s ! SDL Error: %s \n", "x.bmp", SDL_GetError());
		success = false;
	}

	return success;
}

void close() {
	SDL_FreeSurface(gXSurface);
	gXSurface = nullptr;
	SDL_DestroyWindow(gWindow);
	gWindow = nullptr;

	SDL_Quit();
}
