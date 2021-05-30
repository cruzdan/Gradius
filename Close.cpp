#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

void closeTextures() {
	extern SDL_Texture* shipTexture;
	extern SDL_Texture* downBackgroundTexture;
	extern SDL_Texture* menuTexture;
	extern SDL_Texture* levelOneTexture;
	extern SDL_Texture* enemyTexture;
	extern SDL_Texture* upBackgroundTexture;
	extern SDL_Texture* enemy2Texture;
	extern SDL_Texture* enemyReverseTexture;
	extern SDL_Texture* enemyReverse2Texture;
	extern SDL_Texture* menuTextTexture[];
	extern SDL_Texture* imageMenuTexture;
	extern SDL_Texture* lifeTexture[];
	extern SDL_Texture* mountainPuntuationTexture;
	SDL_DestroyTexture(shipTexture);
	SDL_DestroyTexture(downBackgroundTexture);
	SDL_DestroyTexture(upBackgroundTexture);
	SDL_DestroyTexture(menuTexture);
	SDL_DestroyTexture(levelOneTexture);
	SDL_DestroyTexture(enemyTexture);
	SDL_DestroyTexture(enemy2Texture);
	SDL_DestroyTexture(enemyReverseTexture);
	SDL_DestroyTexture(enemyReverse2Texture);
	SDL_DestroyTexture(mountainPuntuationTexture);
	for (int i = 0; i < 4; i++) {
		SDL_DestroyTexture(menuTextTexture[i]);
	}
	for (int i = 0; i < 2; i++) {
		SDL_DestroyTexture(lifeTexture[i]);
	}
	SDL_DestroyTexture(imageMenuTexture);
}

void close() {
	extern SDL_Window* window;
	extern SDL_Renderer* renderer;
	extern SDL_Joystick* joystick[];
	extern int totalJoysticks;
	closeTextures();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	for (int i = 0; i < totalJoysticks; i++) {
		SDL_JoystickClose(joystick[i]);
	}
	extern Mix_Music* music[];
	extern Mix_Chunk* sound[];
	for (int i = 0; i < 3; i++) {
		Mix_FreeMusic(music[i]);
	}
	for (int i = 0; i < 8; i++) {
		Mix_FreeChunk(sound[i]);
	}
	Mix_Quit();
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}