#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>

#undef  main


const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int DELTA = 33;




class LTexture {
public:
	LTexture();
	~LTexture();

	bool loadFromFile(std::string path);
	void free();

	void render(int x, int y);

	int getWidth() const;
	int getHeight() const;

private:

	SDL_Texture* mTexture;
	int mWidth;
	int mHeight;
};

bool init();
bool loadMedia();
void close();

SDL_Window* gWindow = nullptr;
SDL_Renderer* gRenderer = nullptr;

LTexture gFooTexture;
LTexture gBackgroundTexture;




int main(int argc, char* argv[]) {

	if (!init()) {
		printf("Failed to initialize!\n");
	} else {
		if (!loadMedia()) {
			printf("Failed to load media!\n");
		} else {
			bool quit = false;

			SDL_Event e;

			int x = 0;

			Uint32 nextUpdateTime = 0;

			while (!quit) {

				while (SDL_PollEvent(&e) != 0) {
					if (e.type == SDL_QUIT) {
						quit = true;
					}
				}

				Uint32 nowTime = SDL_GetTicks();
				if (nowTime >= nextUpdateTime) {
					SDL_SetRenderDrawColor(gRenderer, 0xff, 0xff, 0xff, 0xff);
					SDL_RenderClear(gRenderer);

					gBackgroundTexture.render(0, 0);

					x = (x + 1) % SCREEN_WIDTH;
					gFooTexture.render(x, 190);

					SDL_RenderPresent(gRenderer);

					nextUpdateTime = nowTime + DELTA;
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
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	} else {
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
			printf("Warning: liner texture filtering not enabled!");
		}

		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

		if (gWindow == nullptr) {
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		} else {
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
			if (gRenderer == nullptr) {
				printf("Renderer could not be created! SDL ERROR: %s\n", SDL_GetError());
				success = false;
			} else {
				SDL_SetRenderDrawColor(gRenderer, 0xff, 0xff, 0xff, 0xff);

				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags)) {
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}
			}
		}
	}

	return success;
}

bool loadMedia() {
	bool success = true;

	if (!gFooTexture.loadFromFile("foo.png")) {
		printf("Failed to load Foo' texture image!\n");
		success = false;
	}

	if (!gBackgroundTexture.loadFromFile("background.png")) {
		printf("Failed to load background texture image!\n");
		success = false;
	}

	return success;
}



void close() {

	gFooTexture.free();
	gBackgroundTexture.free();

	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = nullptr;
	gRenderer = nullptr;

	IMG_Quit();
	SDL_Quit();
}


LTexture::LTexture() :
	mTexture(nullptr),
	mWidth(0),
	mHeight(0) {

}

LTexture::~LTexture() {
	free();
}

bool LTexture::loadFromFile(std::string path) {

	free();

	SDL_Texture* newTexture = nullptr;

	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == nullptr) {
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	} else {
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xff, 0xff));

		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if (newTexture == nullptr) {
			printf("Unable to create textrure from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		} else {
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}

		SDL_FreeSurface(loadedSurface);
	}

	mTexture = newTexture;
	return mTexture != nullptr;
}

void LTexture::free() {

	if (mTexture != nullptr) {
		SDL_DestroyTexture(mTexture);
		mTexture = nullptr;
		mWidth = 0;
		mHeight = 0;
	}
}

void LTexture::render(int x, int y) {
	SDL_Rect renderQuad = { x,y,mWidth,mHeight };
	SDL_RenderCopy(gRenderer, mTexture, nullptr, &renderQuad);
}

int LTexture::getWidth() const {
	return mWidth;
}

int LTexture::getHeight() const {
	return mHeight;
}