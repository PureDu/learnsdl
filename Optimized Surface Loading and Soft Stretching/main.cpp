
#include <SDL2/SDL.h>
#include <stdio.h>
#include <string>


#undef main


const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

bool init();
bool loadMedia();
void close();

SDL_Surface* loadSurface(std::string path);



SDL_Window* gWindow = nullptr;
SDL_Surface* gScreenSurface = nullptr;
SDL_Surface* gStretchedSurface = nullptr;


int main(int argc, char* args[]) {


	if (!init()) {
		printf("Failed to initialize!\n");
	} else {
		if (!loadMedia()) {
			printf("Failed to load media!\n");
		} else {
			bool quit = false;
			SDL_Event e;

			while (!quit) {
				while (SDL_PollEvent(&e) != 0) {
					if (e.type == SDL_QUIT) {
						quit = true;
					}

				}

				SDL_Rect stretchRect;
				stretchRect.x = 0;
				stretchRect.y = 0;
				stretchRect.w = SCREEN_WIDTH;
				stretchRect.h = SCREEN_HEIGHT;
				SDL_BlitScaled(gStretchedSurface, nullptr, gScreenSurface, &stretchRect);

				SDL_UpdateWindowSurface(gWindow);
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
	gStretchedSurface = loadSurface("stretch.bmp");
	if (gStretchedSurface == nullptr) {
		printf("Failed to load stretching image!\n");
		success = false;
	}

	return success;
}

void close() {
	SDL_FreeSurface(gStretchedSurface);
	gStretchedSurface = nullptr;

	SDL_DestroyWindow(gWindow);
	gWindow = nullptr;

	SDL_Quit();
}

SDL_Surface* loadSurface(std::string path) {
	SDL_Surface* optimizedSurface = nullptr;

	SDL_Surface* loadedSurface = SDL_LoadBMP(path.c_str());

	if (loadedSurface == nullptr) {
		printf("Unable to load image %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
	} else {
		optimizedSurface = SDL_ConvertSurface(loadedSurface, gScreenSurface->format, 0);
		if (optimizedSurface == nullptr) {
			printf("Unable to optimize image %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}

		SDL_FreeSurface(loadedSurface);

	}

	return optimizedSurface;
}
