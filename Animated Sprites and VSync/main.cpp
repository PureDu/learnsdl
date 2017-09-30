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

	void setBlendMode(SDL_BlendMode blending);

	void setAlpha(Uint8 alpha);

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



const int WALKING_ANIMAITION_FRAMES = 4;
SDL_Rect gSpriteClips[WALKING_ANIMAITION_FRAMES];
LTexture gSpriteSheetTexture;




int main(int argc, char* argv[]) {

	if (!init()) {
		printf("Failed to initialize!\n");
	} else {
		if (!loadMedia()) {
			printf("Failed to load media!\n");
		} else {
			bool quit = false;
			SDL_Event e;

			Uint8 a = 255;

			int clipIndex = 0;
			const Uint32 Delta = 1000 / 10;
			Uint32 nextUpdateTime = 0;

			while (!quit) {

				while (SDL_PollEvent(&e) != 0) {
					if (e.type == SDL_QUIT) {
						quit = true;
					} 
				}

				Uint32 nowTimne = SDL_GetTicks();
				if (nowTimne >= nextUpdateTime) {
					SDL_SetRenderDrawColor(gRenderer, 0xff, 0xff, 0xff, 0xff);
					SDL_RenderClear(gRenderer);

					SDL_Rect* currentClip = &gSpriteClips[clipIndex];
					clipIndex = (clipIndex + 1) % WALKING_ANIMAITION_FRAMES;

					gSpriteSheetTexture.render((SCREEN_WIDTH - currentClip->w) / 2, (SCREEN_HEIGHT - currentClip->h) / 2, currentClip);

					SDL_RenderPresent(gRenderer);

					nextUpdateTime = nowTimne + Delta;
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

	if (!gSpriteSheetTexture.loadFromFile("foo.png")) {
		printf("Failed to load front texture!\n");
		success = false;
	} else {
		gSpriteClips[0].x = 0;
		gSpriteClips[0].y = 0;
		gSpriteClips[0].w = 64;
		gSpriteClips[0].h = 205;

		gSpriteClips[1].x = 64;
		gSpriteClips[1].y = 0;
		gSpriteClips[1].w = 64;
		gSpriteClips[1].h = 205;

		gSpriteClips[2].x = 128;
		gSpriteClips[2].y = 0;
		gSpriteClips[2].w = 64;
		gSpriteClips[2].h = 205;

		gSpriteClips[3].x = 196;
		gSpriteClips[3].y = 0;
		gSpriteClips[3].w = 64;
		gSpriteClips[3].h = 205;
	}
	return success;
}



void close() {

	gSpriteSheetTexture.free();


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

void LTexture::setBlendMode(SDL_BlendMode blending) {
	SDL_SetTextureBlendMode(mTexture, blending);
}

void LTexture::setAlpha(Uint8 alpha) {
	SDL_SetTextureAlphaMod(mTexture, alpha);
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