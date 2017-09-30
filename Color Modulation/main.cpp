#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>

#undef  main


const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;




class LTexture {
public:
	LTexture();
	~LTexture();

	bool loadFromFile(std::string path);
	void free();

	void setColor(Uint8 red, Uint8 green, Uint8 blue);
	void render(int x, int y, SDL_Rect* clip = nullptr);

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


LTexture gModulatedTexture;




int main(int argc, char* argv[]) {

	if (!init()) {
		printf("Failed to initialize!\n");
	} else {
		if (!loadMedia()) {
			printf("Failed to load media!\n");
		} else {
			bool quit = false;
			SDL_Event e;
			Uint8 r = 255;
			Uint8 g = 255;
			Uint8 b = 255;

			while (!quit) {

				while (SDL_PollEvent(&e) != 0) {
					if (e.type == SDL_QUIT) {
						quit = true;
					} else if (e.type == SDL_KEYDOWN) {
						switch (e.key.keysym.sym) {
						case SDLK_q:
							r += 32;
							break;
						case SDLK_w:
							g += 32;
							break;
						case SDLK_e:
							b += 32;
							break;
						case SDLK_a:
							r -= 32;
							break;
						case SDLK_s:
							g -= 32;
							break;
						case SDLK_d:
							b -= 32;
							break;
						}
					}
				}

				SDL_SetRenderDrawColor(gRenderer, 0xff, 0xff, 0xff, 0xff);
				SDL_RenderClear(gRenderer);
				
				gModulatedTexture.setColor(r, g, b);
				gModulatedTexture.render(0, 0);

				SDL_RenderPresent(gRenderer);
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

	if (!gModulatedTexture.loadFromFile("colors.png")) {
		printf("Failed to load colors texture!\n");
		success = false;
	}
	return success;
}



void close() {

	gModulatedTexture.free();


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

void LTexture::setColor(Uint8 red, Uint8 green, Uint8 blue) {
	SDL_SetTextureColorMod(mTexture, red, green, blue);
}

void LTexture::render(int x, int y, SDL_Rect* clip /*= nullptr*/) {


	SDL_Rect renderQuad = { x,y,mWidth,mHeight };


	if (clip != nullptr) {
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}
	SDL_RenderCopy(gRenderer, mTexture, clip, &renderQuad);
}

int LTexture::getWidth() const {
	return mWidth;
}

int LTexture::getHeight() const {
	return mHeight;
}