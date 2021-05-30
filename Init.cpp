#include <iostream>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <string>
#include "level.h"

bool init() {
	extern int SCREEN_WIDTH; 
	extern int SCREEN_HEIGHT;
	extern SDL_Window* window;
	extern SDL_Renderer* renderer;
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", SDL_GetError(), NULL);
		std::cout << "SDL did not initialize " << std::endl;
		SDL_Quit();
		return false;
	}

	if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Image",
			"The image PNG is not working", NULL);
		return false;
	}

	if (TTF_Init() < 0) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Ttf",
			"The ttf is not working", NULL);
		return false;
	}
	if (SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE,
		&window, &renderer) < 0) {
		std::cout << "window or renderer are not initialized " << std::endl;
		return false;
	}
	SDL_SetWindowTitle(window, "Gradius");

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Audio",
			"The audio is not working", NULL);
		return false;
	}
	return true;
}

void roundsInit() {
	switch (level) {
	case 1:
		/*
		roundTimes[0] = 1000/ 2;
		roundTimes[1] = 2000/ 4;
		roundTimes[2] = 2000/ 4;
		roundTimes[3] = 2000/ 4;
		roundTimes[4] = 2000/ 4;
		roundTimes[5] = 1000/ 2;
		roundTimes[6] = 1000/ 2;
		roundTimes[7] = 1000/ 2;
		roundTimes[8] = 1000/ 2;
		roundTimes[9] = 1000/ 2;
		roundTimes[10] = 2000/ 4;
		roundTimes[11] = 2000/ 4;
		roundTimes[12] = 1000/ 2;
		roundTimes[13] = 1000/ 2;
		roundTimes[14] = 1000/ 2;
		roundTimes[15] = 1000/ 2;
		roundTimes[16] = 1000/ 2;
		roundTimes[17] = 1000/ 2;*/
		
		roundTimes[0] = 1000;
		roundTimes[1] = 2000;
		roundTimes[2] = 2000;
		roundTimes[3] = 2000;
		roundTimes[4] = 2000;
		roundTimes[5] = 1000;
		roundTimes[6] = 1000;
		roundTimes[7] = 1000;
		roundTimes[8] = 1000;
		roundTimes[9] = 1000;
		roundTimes[10] = 2000;
		roundTimes[11] = 2000;
		roundTimes[12] = 1000;
		roundTimes[13] = 1000;
		roundTimes[14] = 1000;
		roundTimes[15] = 1000;
		roundTimes[16] = 1000;
		roundTimes[17] = 1000;
		//enemy: type, up
		enemyRound[0] = { 0,1 };
		enemyRound[1] = { 0,0 };
		enemyRound[2] = { 0,1 };
		enemyRound[3] = { 0,0 };
		enemyRound[4] = { 0,1 };
		enemyRound[5] = { 1,1 };
		enemyRound[6] = { 1,0 };
		enemyRound[7] = { 1,1 };
		enemyRound[8] = { 1,0 };
		enemyRound[9] = { 0,1 };
		enemyRound[10] = { 0,0 };
		enemyRound[11] = { 0,1 };
		enemyRound[12] = { 1,1 };
		enemyRound[13] = { 1,0 };
		enemyRound[14] = { 1,1 };
		enemyRound[15] = { 2,1 };
		enemyRound[16] = { 2,0 };
		enemyRound[17] = { 2,1 };
		break;
	}
}

void menuInit() {
	extern int SCREEN_HEIGHT;
	extern SDL_Texture* menuTextTexture[];
	extern SDL_Texture* lifeTexture[];
	extern SDL_Renderer* renderer;
	extern int lifes[];
	extern SDL_Texture* mountainPuntuationTexture;
	SDL_Color white = { 255,255,255 };
	SDL_Surface* textSurface;
	TTF_Font* font = TTF_OpenFont("Oswald-BoldItalic.ttf", (int)(SCREEN_HEIGHT * 0.04));

	textSurface = TTF_RenderText_Solid(font, std::to_string(lifes[0]).c_str(), white);
	lifeTexture[0] = SDL_CreateTextureFromSurface(renderer, textSurface);

	textSurface = TTF_RenderText_Solid(font, std::to_string(lifes[1]).c_str(), white);
	lifeTexture[1] = SDL_CreateTextureFromSurface(renderer, textSurface);

	textSurface = TTF_RenderText_Solid(font, "1 P", white);
	menuTextTexture[0] = SDL_CreateTextureFromSurface(renderer, textSurface);

	textSurface = TTF_RenderText_Solid(font, "0000000", white);
	menuTextTexture[1] = SDL_CreateTextureFromSurface(renderer, textSurface);

	textSurface = TTF_RenderText_Solid(font, "H I", white);
	menuTextTexture[2] = SDL_CreateTextureFromSurface(renderer, textSurface);

	textSurface = TTF_RenderText_Solid(font, "0000000", white);
	menuTextTexture[3] = SDL_CreateTextureFromSurface(renderer, textSurface);

	textSurface = TTF_RenderText_Solid(font, "5000", white);
	mountainPuntuationTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

	TTF_CloseFont(font);
}

//init the variables that could be restarted when a ship dies
void beginVariablesInit() {
	extern SDL_Rect shipRect[];
	extern int SCREEN_WIDTH;
	extern int SCREEN_HEIGHT;
	extern int score[];
	extern int direction[];
	extern int lifes[];
	extern int totalLineEnemies[];
	extern int limitUpgrades[2][6];
	extern int shipSpeed[];
	extern int totalBullets[];
	extern int bulletSpeed[];
	extern int bulletTimer[];
	extern bool shoot[];
	extern bool shield[];
	extern int actualUpgrade[];
	extern bool missile[];
	extern bool shootMissile[2][3];
	extern int totalMissiles[];
	extern bool doubleShoot[];
	extern int totalDoubleBullets[];
	extern bool laser[];
	extern int bulletDamage[];
	extern int option[];
	extern int nextElement[2][2];
	extern int totalOptionBullets[2][2];
	extern int totalDoubleOptionBullets[2][2];
	extern bool inmunity[];
	extern int inmunityTimer[];
	extern int generatedEnemies8;

	shipRect[0].y = SCREEN_HEIGHT / 2 - shipRect[0].h / 2;
	shipRect[0].x = SCREEN_WIDTH / 4 - shipRect[0].w / 2;

	shipRect[1].x = shipRect[0].x;
	shipRect[1].y = shipRect[0].y + shipRect[1].h;

	generatedEnemies8 = 0;

	//two ships with the same values
	for (int i = 0; i < 2; i++) {
		score[i] = 0;
		lifes[i] = 3;
		direction[i] = 0;
		shipSpeed[i] = SCREEN_WIDTH / 360;
		totalBullets[i] = 0;
		bulletSpeed[i] = SCREEN_WIDTH / 45;
		bulletTimer[i] = 0;
		shoot[i] = true;
		actualUpgrade[i] = 0;
		limitUpgrades[i][0] = 20;//speed up
		limitUpgrades[i][1] = 1;//missile
		limitUpgrades[i][2] = 1;//double
		limitUpgrades[i][3] = 1;//laser
		limitUpgrades[i][4] = 2;//option
		limitUpgrades[i][5] = 1;//shield
		missile[i] = false;
		totalMissiles[i] = 0;
		doubleShoot[i] = false;
		totalDoubleBullets[i] = 0;
		laser[i] = false;
		bulletDamage[i] = 1;
		option[i] = 0;
		shield[i] = false;
		inmunity[i] = false;
		inmunityTimer[i] = 0;
		totalLineEnemies[0] = 4;
		for (int j = 0; j < 2; j++) {
			for (int i = 0; i < 3; i++) {
				shootMissile[j][i] = false;
			}
		}

		for (int j = 0; j < 2; j++) {
			for (int i = 0; i < 2; i++) {
				nextElement[j][i] = 0;
				totalOptionBullets[j][i] = 0;
				totalDoubleOptionBullets[j][i] = 0;
			}
		}
	}
}

void initVariables() {
	extern int SCREEN_WIDTH; 
	extern int SCREEN_HEIGHT;
	extern SDL_Rect shipRect[];
	extern SDL_Rect backgroundImageRect;
	extern SDL_Rect menuTextRects[];
	extern SDL_Rect imageMenuRect;
	extern SDL_Rect menuUpgradeRects[2][6];
	extern SDL_Rect lifeRect[];
	extern int mountainX[];
	extern SDL_Point mountainRoute[];

	srand((unsigned int)time(NULL));
	shipRect[0].w = SCREEN_WIDTH / 9;
	shipRect[0].h = SCREEN_HEIGHT / 12;

	shipRect[1].w = shipRect[0].w;
	shipRect[1].h = shipRect[0].h;

	backgroundImageRect = { 288,16,276,208 };
	roundsInit();
	beginVariablesInit();

	for (int i = 0; i < 4; i++) {
		menuTextRects[i].y = (int)(SCREEN_HEIGHT * 0.93);
		menuTextRects[i].h = (int)(SCREEN_HEIGHT * 0.04);
	}
	menuTextRects[0].x = (int)(SCREEN_WIDTH * 0.253);
	menuTextRects[0].w = (int)(SCREEN_WIDTH * 0.053);
	menuTextRects[1].x = (int)(SCREEN_WIDTH * 0.343);
	menuTextRects[1].w = (int)(SCREEN_WIDTH * 0.213);
	menuTextRects[2].x = (int)(SCREEN_WIDTH * 0.625);
	menuTextRects[2].w = (int)(SCREEN_WIDTH * 0.053);
	menuTextRects[3].x = (int)(SCREEN_WIDTH * 0.718);
	menuTextRects[3].w = (int)(SCREEN_WIDTH * 0.213);

	lifeRect[0] = { (int)(SCREEN_WIDTH * 0.156), (int)(SCREEN_HEIGHT * 0.93), (int)(SCREEN_WIDTH * 0.026), (int)(SCREEN_HEIGHT * 0.04) };
	lifeRect[1] = lifeRect[0];
	lifeRect[1].x = lifeRect[0].x + lifeRect[0].w + lifeRect[0].w;

	for (int i = 0; i < 6; i++) {
		menuUpgradeRects[0][i] = { (int)((i + 1) * 0.125 * SCREEN_WIDTH), (int)(0.9 * SCREEN_HEIGHT),(int)(0.121 * SCREEN_WIDTH), (int)(0.031 * SCREEN_HEIGHT) };
	}
	int heightPlayer2 = (int)(menuUpgradeRects[0][0].h * 3 / 4);
	for (int i = 0; i < 6; i++) {
		menuUpgradeRects[1][i] = { (int)((i + 1) * 0.125 * SCREEN_WIDTH), (int)(0.915 * SCREEN_HEIGHT) - heightPlayer2 / 2,(int)(0.121 * SCREEN_WIDTH), heightPlayer2 };
	}

	imageMenuRect.x = -SCREEN_WIDTH;
	imageMenuRect.y = 0;
	imageMenuRect.w = SCREEN_WIDTH;
	imageMenuRect.h = SCREEN_HEIGHT;

	shipRect[0].y = (int)(SCREEN_HEIGHT * 0.58 - shipRect[0].h / 2);
	shipRect[0].x = SCREEN_WIDTH / 4 + SCREEN_WIDTH / 32;

	mountainX[0] = (int)(SCREEN_WIDTH * 0.1388); //first mountain x
	mountainX[1] = (int)(SCREEN_WIDTH * 0.7055);//second mountain x

	for (int i = 0; i < 3; i++) {
		mountainRoute[i].y = 12;
		mountainRoute[i].x = 44 + i;
	}
}

void loadImage() {
	extern SDL_Texture* shipTexture;
	extern SDL_Texture* downBackgroundTexture;
	extern SDL_Texture* upBackgroundTexture;
	extern SDL_Texture* menuTexture;
	extern SDL_Texture* levelOneTexture;
	extern SDL_Renderer* renderer;
	extern SDL_Texture* enemyTexture;
	extern SDL_Texture* enemy2Texture;
	extern SDL_Texture* enemyReverseTexture;
	extern SDL_Texture* enemyReverse2Texture;
	extern SDL_Texture* imageMenuTexture;
	
	SDL_Surface* image;
	image = IMG_Load("images/sprites.png");
	if (!image) {
		std::cout << "failed in load sprites image" << std::endl;
	}
	SDL_SetColorKey(image, SDL_TRUE, SDL_MapRGB(image->format, 0, 0, 0));
	shipTexture = SDL_CreateTextureFromSurface(renderer, image);

	image = IMG_Load("images/NES - Gradius - Background Tiles.png");
	if (!image) {
		std::cout << "failed in load background image" << std::endl;
	}
	SDL_SetColorKey(image, SDL_TRUE, SDL_MapRGB(image->format, 0, 0, 0));
	downBackgroundTexture = SDL_CreateTextureFromSurface(renderer, image);

	image = IMG_Load("images/upBackground.png");
	if (!image) {
		std::cout << "failed in load background image" << std::endl;
	}
	SDL_SetColorKey(image, SDL_TRUE, SDL_MapRGB(image->format, 0, 0, 0));
	upBackgroundTexture = SDL_CreateTextureFromSurface(renderer, image);

	image = IMG_Load("images/NES - Gradius - HUD.png");
	if (!image) {
		std::cout << "failed in load HUD image" << std::endl;
	}
	menuTexture = SDL_CreateTextureFromSurface(renderer, image);

	image = IMG_Load("images/Gradius-Stage1-Volcano.png");
	if (!image) {
		std::cout << "failed in load Stage 1 image" << std::endl;
	}
	levelOneTexture = SDL_CreateTextureFromSurface(renderer, image);

	image = IMG_Load("images/enemies.png");
	if (!image) {
		std::cout << "failed in load enemies 1 image" << std::endl;
	}
	SDL_SetColorKey(image, SDL_TRUE, SDL_MapRGB(image->format, 0, 0, 0));
	enemyTexture = SDL_CreateTextureFromSurface(renderer, image);

	image = IMG_Load("images/enemies2.png");
	if (!image) {
		std::cout << "failed in load enemies 2 image" << std::endl;
	}
	SDL_SetColorKey(image, SDL_TRUE, SDL_MapRGB(image->format, 0, 0, 0));
	enemy2Texture = SDL_CreateTextureFromSurface(renderer, image);

	image = IMG_Load("images/enemiesReverse.png");
	if (!image) {
		std::cout << "failed in load reverse enemies 1 image" << std::endl;
	}
	SDL_SetColorKey(image, SDL_TRUE, SDL_MapRGB(image->format, 0, 0, 0));
	enemyReverseTexture = SDL_CreateTextureFromSurface(renderer, image);

	image = IMG_Load("images/enemies2Reverse.png");
	if (!image) {
		std::cout << "failed in load reverese enemies 2 image" << std::endl;
	}
	SDL_SetColorKey(image, SDL_TRUE, SDL_MapRGB(image->format, 0, 0, 0));
	enemyReverse2Texture = SDL_CreateTextureFromSurface(renderer, image);

	image = IMG_Load("images/menu0.png");
	if (!image) {
		std::cout << "failed in load menu 0 image" << std::endl;
	}
	imageMenuTexture = SDL_CreateTextureFromSurface(renderer, image);

	SDL_FreeSurface(image);
}

void loadJoystick() {
	extern SDL_Joystick* joystick[];
	extern int totalJoysticks;
	for (int i = 0; i < 2; i++) {
		joystick[i] = SDL_JoystickOpen(i);
		if (joystick[i])
			totalJoysticks++;
	}
}