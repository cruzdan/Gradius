#include <SDL.h>
#include <string>
#include <SDL_mixer.h>
void changeTextToTexture(std::string newText, SDL_Texture*& texture);
void changeTextToScoreTexture(std::string newText, SDL_Texture*& texture);
void beginVariablesInit();

//init the general variables to the begin of the level
void restartLevel() {
	extern int backgroundTimer;
	extern int phase;
	extern int actualRound;
	extern int roundTimer;
	extern bool generatingEnemies;
	extern int lineTimer;
	extern int enemiesType0Appeared;
	extern int totalEnemies;
	extern int totalEnemyBullets;
	extern int sprites3;
	extern int sprites4;
	extern int spritesTimer;
	extern int progress;
	extern int actualProgressEnemy;
	extern int totalDownSquares;
	extern int totalUpSquares;
	extern int totalCenterSquares;
	extern int actualCenterColision;
	extern int totalUpgradeRects;
	extern int indexLineEnemy;
	extern int level;
	extern int actualMountainIndex;
	extern bool mountainPoints;
	extern int generatedEnemies8;

	backgroundTimer = 0;
	phase = 0;
	actualRound = 0;
	roundTimer = 0;
	generatingEnemies = false;
	lineTimer = 0;
	enemiesType0Appeared = 0;
	totalEnemies = 0;
	totalEnemyBullets = 0;
	sprites3 = 0;
	sprites4 = 0;
	spritesTimer = 0;
	progress = 0;
	actualProgressEnemy = 0;
	totalDownSquares = 0;
	totalUpSquares = 0;
	totalCenterSquares = 0;
	actualCenterColision = 0;
	totalUpgradeRects = 0;
	indexLineEnemy = 0;
	actualMountainIndex = 0;
	mountainPoints = false;
	generatedEnemies8 = 0;
	level = 1;
}

void restartSound() {
	//pause music and play music 0
	extern Mix_Music* music[];
	Mix_PauseMusic();
	Mix_PlayMusic(music[0], -1);
}

//restart the level without lose the actual upgrades and score
void loopRestart() {
	restartLevel();
	restartSound();
	extern int players;
	if (players > 1) {
		extern int lifes[];
		if (lifes[0] <= 0) {
			extern SDL_Texture* lifeTexture[];
			lifes[0]++;
			changeTextToTexture(std::to_string(lifes[0]), lifeTexture[0]);
		}
		else if (lifes[1] <= 0) {
			extern SDL_Texture* lifeTexture[];
			lifes[1]++;
			changeTextToTexture(std::to_string(lifes[1]), lifeTexture[1]);
		}
	}
}

void restart(int shipIndex) {
	extern bool inmunity[];
	extern int lifes[];
	extern int players;
	if (players == 1) {
		extern SDL_Texture* lifeTexture[];
		lifes[shipIndex]--;
		inmunity[shipIndex] = true;

		if (lifes[0] <= 0) {
			extern int score[];
			extern SDL_Texture* menuTextTexture[];
			restartLevel();
			changeTextToScoreTexture(std::to_string(score[0]), menuTextTexture[1]);
			beginVariablesInit();
			restartSound();
		}
		changeTextToTexture(std::to_string(lifes[0]), lifeTexture[0]);
	}
	else {
		extern SDL_Texture* lifeTexture[];
		lifes[shipIndex]--;
		inmunity[shipIndex] = true;
		changeTextToTexture(std::to_string(lifes[shipIndex]), lifeTexture[shipIndex]);
		if (lifes[0] <= 0 && lifes[1] <= 0) {
			extern int score[];
			extern SDL_Texture* menuTextTexture[];
			extern SDL_Texture* lifeTexture[];
			restartLevel();

			lifes[0] = 3;
			score[0] = 0;
			changeTextToScoreTexture(std::to_string(score[0]), menuTextTexture[1]);
			changeTextToTexture(std::to_string(lifes[0]), lifeTexture[0]);
			lifes[1] = 3;
			score[1] = 0;
			changeTextToScoreTexture(std::to_string(score[1]), menuTextTexture[3]);
			changeTextToTexture(std::to_string(lifes[1]), lifeTexture[1]);
			beginVariablesInit();
			restartSound();
		}
	}
}