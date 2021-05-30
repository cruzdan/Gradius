#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include "level.h"
#include "enemy.h"
#include "square.h"
#include "activeRect.h"
#include <SDL_mixer.h>

void moveSquares();
void restart(int shipIndex);
void updateProgress();
void updateBackgroundTimer();
void loopRestart();

int maxBackground = 3; //indicates the updates to move the background

#define PI 3.14159265
extern Mix_Chunk* sound[];

//move the menu image to right until it reaches the full screen
void moveToRightMenu0() {
	extern SDL_Rect imageMenuRect;
	imageMenuRect.x += 2;
	if (imageMenuRect.x >= 0) {
		extern int phase;
		phase = 1;
	}
}

//change the texture text to newText
void changeTextToTexture(std::string newText, SDL_Texture*& texture) {
	extern SDL_Renderer* renderer;
	extern int SCREEN_HEIGHT;
	TTF_Font* font = TTF_OpenFont("Oswald-BoldItalic.ttf", (int)(SCREEN_HEIGHT * 0.04));
	SDL_Surface* textSurface;
	SDL_Color white = { 255,255,255 };
	const char* t = newText.c_str();
	textSurface = TTF_RenderText_Solid(font, t, white);
	texture = SDL_CreateTextureFromSurface(renderer, textSurface);
	SDL_FreeSurface(textSurface);
	TTF_CloseFont(font);
}

//change the texture score text to new score preceded by 0's (word of 7 digits)
void changeTextToScoreTexture(std::string newText, SDL_Texture*& texture) {
	std::string auxiliar = "";
	int total = 7 - (int)newText.length();
	for (int i = 0; i < total; i++) {
		auxiliar += "0";
	}
	auxiliar += newText;
	extern SDL_Renderer* renderer;
	extern int SCREEN_HEIGHT;
	TTF_Font* font = TTF_OpenFont("Oswald-BoldItalic.ttf", (int)(SCREEN_HEIGHT * 0.04));
	SDL_Surface* textSurface;
	SDL_Color white = { 255,255,255 };
	const char* t = auxiliar.c_str();
	textSurface = TTF_RenderText_Solid(font, t, white);
	texture = SDL_CreateTextureFromSurface(renderer, textSurface);
	SDL_FreeSurface(textSurface);
	TTF_CloseFont(font);
}

//updates the final coordinate of the optionRoute[indexShip][indexOption] and updates the next element
void updateCoordinate(int& optionIndex, int shipIndex) {
	extern SDL_Rect optionRect[2][2];
	extern SDL_Point optionRoute[2][2][20];
	extern int nextElement[2][2];
	extern SDL_Rect shipRect[];
	if (optionIndex == 0) {
		//updates optionRoute to the actual coordinate of the ship
		optionRoute[shipIndex][optionIndex][nextElement[shipIndex][optionIndex]].x = shipRect[shipIndex].x;
		optionRoute[shipIndex][optionIndex][nextElement[shipIndex][optionIndex]].y = shipRect[shipIndex].y;
	}
	else {
		//updates optionRoute to the actual coordinate of the option 0
		optionRoute[shipIndex][optionIndex][nextElement[shipIndex][optionIndex]].x = optionRect[shipIndex][0].x;
		optionRoute[shipIndex][optionIndex][nextElement[shipIndex][optionIndex]].y = optionRect[shipIndex][0].y;
	}
	//increment or reset the index of the next element
	if (nextElement[shipIndex][optionIndex] < 19) {
		nextElement[shipIndex][optionIndex]++;
	}
	else {
		nextElement[shipIndex][optionIndex] = 0;
	}
}

//update the option rect to the next coordinate of its route
void moveOptionToNextCoordinate(int optionIndex, int shipIndex) {
	extern SDL_Rect optionRect[2][2];
	extern SDL_Point optionRoute[2][2][20];
	extern int nextElement[2][2];
	optionRect[shipIndex][optionIndex].x = optionRoute[shipIndex][optionIndex][nextElement[shipIndex][optionIndex]].x;
	optionRect[shipIndex][optionIndex].y = optionRoute[shipIndex][optionIndex][nextElement[shipIndex][optionIndex]].y;
}

//move the option coordinate and updates its route
void moveOption(int shipIndex) {
	extern int option[];
	for (int i = 0; i < option[shipIndex]; i++) {
		moveOptionToNextCoordinate(i, shipIndex);
		updateCoordinate(i, shipIndex);
	}
}

//fill the optionRoute coordinates
void fillOptionRoute(SDL_Rect begin,int optionIndex, int shipSpeed, int shipIndex) {
	extern SDL_Point optionRoute[2][2][20];
	for (int i = 0; i < 20; i++) {
		optionRoute[shipIndex][optionIndex][i] = { begin.x,begin.y - i * shipSpeed };
	}
}


void addOption(int optionIndex, SDL_Rect ship,int shipSpeed, int shipIndex) {
	extern SDL_Rect optionRect[2][2];
	if (optionIndex == 0) {
		optionRect[shipIndex][optionIndex] = { ship.x,ship.y + 2 * ship.h,ship.w / 2,ship.h / 2 };
	}
	else {
		optionRect[shipIndex][optionIndex] = { optionRect[shipIndex][0].x, optionRect[shipIndex][0].y + 2 * optionRect[shipIndex][0].h,optionRect[shipIndex][0].w,optionRect[shipIndex][0].h };
	}
	fillOptionRoute(optionRect[shipIndex][optionIndex], optionIndex, shipSpeed, shipIndex);
}

//i indicates if the missile is of: 0->ship,1:option0, 2: option1
void generateMissile(int i, int shipIndex) {
	extern SDL_Rect optionRect[2][2];
	extern SDL_Rect shipRect[];
	SDL_Rect rect;
	rect.w = shipRect[shipIndex].w / 4;
	rect.h = shipRect[shipIndex].h / 4;
	
	switch (i) {
	case 0:
		//ship
		rect.x = shipRect[shipIndex].x + shipRect[shipIndex].w / 2 - rect.w / 2;
		rect.y = shipRect[shipIndex].y + shipRect[shipIndex].h;
		break;
	case 1:
		//option 0
		rect.x = optionRect[shipIndex][0].x + optionRect[shipIndex][0].w - rect.w / 2;
		rect.y = optionRect[shipIndex][0].y + optionRect[shipIndex][0].h;
		break;
	case 2:
		//option 1
		rect.x = optionRect[shipIndex][1].x + optionRect[shipIndex][1].w - rect.w / 2;
		rect.y = optionRect[shipIndex][1].y + optionRect[shipIndex][1].h;
		break;
	}
	extern int squareHeight;
	if (rect.y < squareHeight * 14) {
		Missile element = { i,rect,1,2 };
		addToEndMissile(missiles[shipIndex], totalMissiles[shipIndex], element);
	}
	else {
		extern bool shootMissile[2][3];
		shootMissile[shipIndex][i] = true;
	}
}

//create and upgrade on the center red enemy
void createUpgrade(SDL_Rect enemyRect) {
	extern SDL_Rect shipRect[];
	SDL_Rect rect;
	rect.w = shipRect[0].h;
	rect.h = shipRect[0].h;
	rect.x = enemyRect.x + enemyRect.w / 2 - rect.w / 2;
	rect.y = enemyRect.y + enemyRect.h / 2 - rect.h / 2;
	ActiveRect element = { totalUpgradeRects,rect };
	addToEndActiveRect(upgradeRects, totalUpgradeRects, element);
}

//create a destroyed enemy in the position of enemies[enemyIndex]
void generateDestroyedEnemy(int enemyIndex) {
	Square element;
	element.id = totalDestroyedEnemy;
	element.colisionType = 0;//actual sprite
	element.rect = enemies[enemyIndex].rect;
	addToEndSquare(destroyedEnemy, totalDestroyedEnemy, element);
}

void checkShieldEnemiesColision(int shipIndex) {
	extern bool shield[];
	extern SDL_Rect shieldRect[];
	extern int shieldLife[];
	extern int score[];
	if (shield[shipIndex]) {
		//check colisions with enemies
		for (int i = totalEnemies - 1; i >= 0; i--) {
			if (SDL_HasIntersection(&shieldRect[shipIndex], &enemies[i].rect)) {
				extern int bulletDamage[];
				enemies[i].life -= bulletDamage[shipIndex];
				if (enemies[i].life <= 0) {
					if (enemies[i].schedule == 0) {
						extern int totalLineEnemies[];
						totalLineEnemies[enemies[i].limit.x]--;
						if (totalLineEnemies[enemies[i].limit.x] <= 0) {
							createUpgrade(enemies[i].rect);
						}
					}
					if (enemies[i].red) {
						createUpgrade(enemies[i].rect);
					}
					shieldLife[shipIndex]--;
					if (shieldLife[shipIndex] <= 0) {
						extern int limitUpgrades[2][6];
						limitUpgrades[shipIndex][5]++;
						shield[shipIndex] = false;
					}
					score[shipIndex] += 100;
					extern SDL_Texture* menuTextTexture[];
					if (shipIndex == 0)
						changeTextToScoreTexture(std::to_string(score[shipIndex]), menuTextTexture[1]);
					else
						changeTextToScoreTexture(std::to_string(score[shipIndex]), menuTextTexture[3]);
					Square element;
					element.id = totalDestroyedEnemy;
					element.colisionType = 0;//actual sprite
					element.rect = enemies[i].rect;
					addToEndSquare(destroyedEnemy, totalDestroyedEnemy, element);
					generateDestroyedEnemy(i);
					switch (enemies[i].schedule) {
					case 3:
					case 5:
						Mix_PlayChannel(1, sound[2], 0);
						break;
					case 9:
						Mix_PlayChannel(1, sound[7], 0);
						break;
					default:
						Mix_PlayChannel(1, sound[1], 0);
						break;
					}
					deleteOfEnemies(enemies, totalEnemies, enemies[i].id);
				}
				break;
			}
		}

		//check colisions with enemy bullets
		for (int i = totalEnemyBullets - 1; i >= 0; i--) {
			if (SDL_HasIntersection(&shieldRect[shipIndex], &enemyBullets[i].rect)) {
				shieldLife[shipIndex]--;
				if (shieldLife[shipIndex] <= 0) {
					extern int limitUpgrades[2][6];
					limitUpgrades[shipIndex][5]++;
					shield[shipIndex] = false;
				}
				deleteOfEnemyBullet(enemyBullets, totalEnemyBullets, enemyBullets[i].id);
			}
		}
	}
}

void checkUpgradeRectShipColision(int shipIndex) {
	extern SDL_Rect shipRect[];
	for (int i = totalUpgradeRects - 1; i >= 0; i--) {
		if (SDL_HasIntersection(&shipRect[shipIndex], &upgradeRects[i].rect)) {
			extern int actualUpgrade[];
			if (actualUpgrade[shipIndex] >= 6) {
				actualUpgrade[shipIndex] = 1;
			}
			else {
				actualUpgrade[shipIndex]++;
			}
			Mix_PlayChannel(2, sound[3], 0);
			deleteOfActiveRect(upgradeRects, totalUpgradeRects, upgradeRects[i].id);
		}
	}
}

void checkEnemyBulletShipColision(int shipIndex) {
	extern SDL_Rect shipRect[];
	for (int i = totalEnemyBullets - 1; i >= 0; i--) {
		if (SDL_HasIntersection(&shipRect[shipIndex], &enemyBullets[i].rect)) {
			extern bool inmunity[];
			if(!inmunity[shipIndex])
				restart(shipIndex);
			deleteOfEnemyBullet(enemyBullets, totalEnemyBullets, enemyBullets[i].id);
		}
	}
}

void checkMissileEnemiesColision(int& totalBullets, Missile bullets[], int &score, int shipIndex) {
	for (int j = totalBullets - 1; j >= 0; j--) {
		for (int i = totalEnemies - 1; i >= 0; i--) {
			if (SDL_HasIntersection(&bullets[j].rect, &enemies[i].rect)) {
				extern int bulletDamage[];
				enemies[i].life -= bulletDamage[shipIndex];
				if (enemies[i].life <= 0) {
					if (enemies[i].schedule == 0) {
						extern int totalLineEnemies[];
						//decrement a enemy of the line of 4 nemies
						totalLineEnemies[enemies[i].limit.x]--;
						if (totalLineEnemies[enemies[i].limit.x] <= 0) {
							createUpgrade(enemies[i].rect);
						}
					}
					if (enemies[i].red) {
						createUpgrade(enemies[i].rect);
					}
					score += 100;
					extern SDL_Texture* menuTextTexture[];
					if (shipIndex == 0)
						changeTextToScoreTexture(std::to_string(score), menuTextTexture[1]);
					else
						changeTextToScoreTexture(std::to_string(score), menuTextTexture[3]);
					generateDestroyedEnemy(i);
					switch (enemies[i].schedule) {
					case 3:
					case 5:
						Mix_PlayChannel(1, sound[2], 0);
						break;
					case 9:
						Mix_PlayChannel(1, sound[7], 0);
						break;
					default:
						Mix_PlayChannel(1, sound[1], 0);
						break;
					}
					deleteOfEnemies(enemies, totalEnemies, enemies[i].id);
				}
				extern bool shootMissile[2][3];
				shootMissile[shipIndex][missiles[shipIndex][j].id] = true;
				deleteOfMissile(bullets, totalBullets, bullets[j].id);
				break;
			}
		}
	}
}

void checkBulletEnemiesColision(int& totalBullets,ActiveRect bullets[], int& score, int shipIndex) {
	for (int j = totalBullets - 1; j >= 0; j--) {
		for (int i = totalEnemies - 1; i >= 0; i--) {
			if (SDL_HasIntersection(&bullets[j].rect, &enemies[i].rect)) {
				extern int bulletDamage[];
				extern bool laser[];
				bool defeated = false;
				enemies[i].life -= bulletDamage[shipIndex];
				if (enemies[i].life <= 0) {
					defeated = true;
					if (enemies[i].schedule == 0) {
						extern int totalLineEnemies[];
						//decrement a enemy of the line of 4 nemies
						totalLineEnemies[enemies[i].limit.x]--;
						if (totalLineEnemies[enemies[i].limit.x] <= 0) {
							createUpgrade(enemies[i].rect);
						}
					}
					if (enemies[i].red) {
						createUpgrade(enemies[i].rect);
					}
					score += 100;
					extern SDL_Texture* menuTextTexture[];
					if(shipIndex == 0)
						changeTextToScoreTexture(std::to_string(score), menuTextTexture[1]);
					else
						changeTextToScoreTexture(std::to_string(score), menuTextTexture[3]);
					generateDestroyedEnemy(i);
					switch (enemies[i].schedule) {
					case 3:
					case 5:
						Mix_PlayChannel(1, sound[2], 0);
						break;
					case 9:
						Mix_PlayChannel(1, sound[7], 0);
						break;
					default:
						Mix_PlayChannel(1, sound[1], 0);
						break;
					}
					deleteOfEnemies(enemies, totalEnemies, enemies[i].id);
				}
				if (!laser[shipIndex])
					deleteOfActiveRect(bullets, totalBullets, bullets[j].id);
				else if (!defeated)
					deleteOfActiveRect(bullets, totalBullets, bullets[j].id);
				break;
			}
		}
	}
}

//return true if the rect are in a center square, except a tree
bool checkMapColision(SDL_Rect rect) {
	int xC, yC;
	extern int squareWidth;
	extern int squareHeight;
	xC = (rect.x + rect.w / 2) / squareWidth;
	yC = (rect.y + rect.h / 2) / squareHeight;

	if (yC < 14) {

		extern int progress;
		int x = progress - 14 + xC;

		for (int k = 0; k < totalCenterSquares; k++) {
			if (centerWindowSquares[k].progress > x) {
				break;
			}
			if (centerWindowSquares[k].progress == x) {
				for (int i = 0; i < centerWindowSquares[k].totalElements; i++) {
					if (centerWindowSquares[k].positionY[i] == yC) {
						//the square is not a tree
						if (centerWindowSquares[k].typeSquare[i] > 72)
							return true;
						return false;
					}
				}
			}
		}
	}
	return false;
}

void checkBulletMapColision() {
	//enemy bullets
	for (int i = totalEnemyBullets - 1; i >= 0; i--) {
		if (checkMapColision(enemyBullets[i].rect))
			deleteOfEnemyBullet(enemyBullets, totalEnemyBullets, enemyBullets[i].id);
	}

	extern int players;
	for (int j = 0; j < players; j++) {

		//bullets
		for (int i = totalBullets[j] - 1; i >= 0; i--) {
			if (checkMapColision(bullets[j][i].rect)) {
				deleteOfActiveRect(bullets[j], totalBullets[j], bullets[j][i].id);
			}
		}
		//doubleBullets
		for (int i = totalDoubleBullets[j] - 1; i >= 0; i--) {
			if (checkMapColision(doubleBullets[j][i].rect)) {
				deleteOfActiveRect(doubleBullets[j], totalDoubleBullets[j], doubleBullets[j][i].id);
			}
		}

		extern int option[];
		for (int k = 0; k < option[j]; k++) {

			//option bullets
			for (int i = totalOptionBullets[j][k] - 1; i >= 0; i--) {
				if (checkMapColision(optionBullets[j][k][i].rect)) {
					deleteOfActiveRect(optionBullets[j][k], totalOptionBullets[j][k], optionBullets[j][k][i].id);
				}
			}

			//optionDoubleBullets
			for (int i = totalDoubleOptionBullets[j][k] - 1; i >= 0; i--) {
				if (checkMapColision(doubleOptionBullets[j][k][i].rect)) {
					deleteOfActiveRect(doubleOptionBullets[j][k], totalDoubleOptionBullets[j][k], doubleOptionBullets[j][k][i].id);
				}
			}
		}
	}
}

//check map colisions with ship in phase 1 and 2
void checkShipMapColision(int shipIndex) {
	extern SDL_Rect shipRect[];
	int xC, yC;
	extern int squareWidth;
	extern int squareHeight;
	xC = (shipRect[shipIndex].x + shipRect[shipIndex].w / 2) / squareWidth;
	yC = (shipRect[shipIndex].y + shipRect[shipIndex].h / 2) / squareHeight;

	if (xC >= 15 - totalDownSquares) {
		if (yC < 1) {
			//colision with up squares
			restart(shipIndex);
		}
		else if (yC < 14) {
			extern int progress;
			int x = progress - 14 + xC;
			for (int k = 0; k < totalCenterSquares; k++) {
				if (centerWindowSquares[k].progress > x) {
					break;
				}
				if (centerWindowSquares[k].progress == x) {
					for (int i = 0; i < centerWindowSquares[k].totalElements; i++) {
						if (centerWindowSquares[k].positionY[i] == yC) {
							if (centerWindowSquares[k].typeSquare[i] != 71 && centerWindowSquares[k].typeSquare[i] != 72) {
								restart(shipIndex);
								return;
							}
						}
					}
				}
			}
		}
		else {
			//colision with down squares
			restart(shipIndex);
		}
	}
}

//check map colisions with ship in phase 3
void checkShipMapColision2(int shipIndex) {

	//when the map of the mountains is moving to left
	extern SDL_Rect shipRect[];
	int centerShipX = shipRect[shipIndex].x + shipRect[shipIndex].w / 2;

	if (centerShipX <= downWindowSquares[totalDownSquares - 1].rect.x + downWindowSquares[totalDownSquares - 1].rect.w) {
		int xC, yC;
		extern int squareWidth;
		extern int squareHeight;
		xC = (shipRect[shipIndex].x + shipRect[shipIndex].w / 2) / squareWidth;
		yC = (shipRect[shipIndex].y + shipRect[shipIndex].h / 2) / squareHeight;
		if (yC < 1) {
			//colision with up squares
			restart(shipIndex);
		}
		else if (yC < 14) {
			//center
			for (int i = 0; i < totalCenterSquares; i++) {
				if (centerShipX >= centerWindowSquares[i].rect[0].x && centerShipX <= centerWindowSquares[i].rect[0].x + centerWindowSquares[i].rect[0].w) {
					SDL_Rect rect = { shipRect[shipIndex].x + shipRect[shipIndex].w / 2, shipRect[shipIndex].y + shipRect[shipIndex].h / 2,1,1 };
					for (int j = 0; j < centerWindowSquares[i].totalElements; j++) {
						if (centerWindowSquares[i].typeSquare[0] > 72 && SDL_HasIntersection(&rect, &centerWindowSquares[i].rect[j])) {
							restart(shipIndex);
							return;
						}
					}
				}
			}
		}
		else {
			//colision with down squares
			restart(shipIndex);
		}
	}

}

void checkSHipEnemiesColision(int shipIndex) {
	extern SDL_Rect shipRect[];
	for (int i = totalEnemies - 1; i >= 0; i--) {
		if (SDL_HasIntersection(&shipRect[shipIndex], &enemies[i].rect)) {
			extern bool inmunity[];
			if(!inmunity[shipIndex])
				restart(shipIndex);
			if (enemies[i].schedule != 9) {
				generateDestroyedEnemy(i);
				switch (enemies[i].schedule) {
				case 3:
				case 5:
					Mix_PlayChannel(1, sound[2], 0);
					break;
				case 9:
					Mix_PlayChannel(1, sound[7], 0);
					break;
				default:
					Mix_PlayChannel(1, sound[1], 0);
					break;
				}
				deleteOfEnemies(enemies, totalEnemies, enemies[i].id);
			}
			return;
		}
	}
}


//generate 4 boss enemy bullets
void enemyShoot9(int i) {
	EnemyBullet bullet;
	extern int SCREEN_WIDTH;
	bullet.speedX = -SCREEN_WIDTH / 90;
	bullet.speedY = 0;
	bullet.rect.w = enemies[i].rect.w / 3;
	bullet.rect.h = enemies[i].rect.h / 12;

	bullet.id = totalEnemyBullets;
	bullet.rect.x = enemies[i].rect.x;
	bullet.rect.y = (int)(enemies[i].rect.y + enemies[i].rect.h * 0.0833);
	addToEndEnemyBullet(enemyBullets, totalEnemyBullets, bullet);

	bullet.id = totalEnemyBullets;
	bullet.rect.x = enemies[i].rect.x - bullet.rect.w;
	bullet.rect.y = (int)(enemies[i].rect.y + enemies[i].rect.h * 0.3125);
	addToEndEnemyBullet(enemyBullets, totalEnemyBullets, bullet);

	bullet.id = totalEnemyBullets;
	bullet.rect.x = enemies[i].rect.x - bullet.rect.w;
	bullet.rect.y = (int)(enemies[i].rect.y + enemies[i].rect.h * 0.6041);
	addToEndEnemyBullet(enemyBullets, totalEnemyBullets, bullet);

	bullet.id = totalEnemyBullets;
	bullet.rect.x = enemies[i].rect.x;
	bullet.rect.y = (int)(enemies[i].rect.y + enemies[i].rect.h * 0.8333);
	addToEndEnemyBullet(enemyBullets, totalEnemyBullets, bullet);
}

//generates a enemy bullet with direction of enemy[enemyIndex] to ship[indexShootShip] and change the next shoot to the other ship
void enemyShoot(int enemyIndex) {
	extern SDL_Rect shipRect[];
	extern int indexShootShip;
	extern int SCREEN_WIDTH;
	int distanceX = (shipRect[indexShootShip].x + shipRect[indexShootShip].w / 2) - (enemies[enemyIndex].rect.x + enemies[enemyIndex].rect.w / 2);
	int distanceY = (enemies[enemyIndex].rect.y + enemies[enemyIndex].rect.h / 2) - (shipRect[indexShootShip].y + shipRect[indexShootShip].h / 2);
	double angle = (double)distanceY / (double)distanceX;
	EnemyBullet bullet;
	bullet.id = totalBullets[indexShootShip];
	bullet.rect.w = shipRect[indexShootShip].w / 5;
	bullet.rect.h = shipRect[indexShootShip].h / 4;
	int enemyBulletSpeed = SCREEN_WIDTH / 144;
	if (distanceX > 0) {
		bullet.speedY = (int)(-enemyBulletSpeed * sin(atan(angle)));
		bullet.speedX = (int)(enemyBulletSpeed * cos(atan(angle)));
		bullet.rect.x = (enemies[enemyIndex].rect.x + enemies[enemyIndex].rect.w / 2) + (int)(bullet.rect.w * cos(angle));
		bullet.rect.y = (enemies[enemyIndex].rect.y + enemies[enemyIndex].rect.h / 2) - (int)(bullet.rect.h * sin(angle));
	}
	else {
		bullet.speedY = (int)(enemyBulletSpeed * sin(atan(angle)));
		bullet.speedX = (int)(-enemyBulletSpeed * cos(atan(angle)));
		bullet.rect.x = (enemies[enemyIndex].rect.x + enemies[enemyIndex].rect.w / 2) - (int)(bullet.rect.w * cos(angle));
		bullet.rect.y = (enemies[enemyIndex].rect.y + enemies[enemyIndex].rect.h / 2) + (int)(bullet.rect.h * sin(angle));
	}
	addToEndEnemyBullet(enemyBullets, totalEnemyBullets, bullet);

	//if players == 2, change the next shoot to the other ship
	extern int players;
	if (players > 1) {
		indexShootShip = (indexShootShip + 1) % 2;
	}
}

//delete enemy bullets out of the screen 
void deleteEnemyBullets() {
	extern int SCREEN_WIDTH;
	extern int gameHeight;
	for (int i = totalEnemyBullets - 1; i >= 0; i--) {
		if (enemyBullets[i].rect.x + enemyBullets[i].rect.w < 0 || enemyBullets[i].rect.x > SCREEN_WIDTH
			|| enemyBullets[i].rect.y + enemyBullets[i].rect.h < 0 || enemyBullets[i].rect.y >= gameHeight) {
			deleteOfEnemyBullet(enemyBullets, totalEnemyBullets, enemyBullets[i].id);
		}
	}
}


void moveEnemyBullets() {
	for (int i = 0; i < totalEnemyBullets; i++) {
		enemyBullets[i].rect.x += enemyBullets[i].speedX;
		enemyBullets[i].rect.y += enemyBullets[i].speedY;
	}
}

//detects when an enemy are going to shoot
void detectEnemyShoot() {
	for (int i = 0; i < totalEnemies; i++) {

		switch (enemies[i].schedule) {
		case 3:
			extern int MS;
			enemies[i].shootTimer += MS;
			if (enemies[i].shootTimer >= 2000) {
				enemyShoot(i);
				enemies[i].shootTimer = 0;
			}
			break;
		case 5:
			if (enemies[i].stop && enemies[i].limit.x < 3) {
				extern int MS;
				enemies[i].shootTimer += MS;
				if (enemies[i].shootTimer >= 500) {
					enemyShoot(i);
					enemies[i].stop = false;
					enemies[i].limit.x++; // indicates the times that the enemy has shooted to the ship
					enemies[i].shootTimer = 0;
				}
			}
			break;
		case 7:
			if (enemies[i].top) {
				extern int MS;
				enemies[i].shootTimer += MS;
				if (enemies[i].shootTimer >= 500) {
					enemyShoot(i);
					enemies[i].shootTimer = 0;
				}
			}
			break;
		case 9:
			extern int MS;
			enemies[i].shootTimer += MS;
			if (enemies[i].shootTimer >= 1000) {
				enemyShoot9(i);
				enemies[i].shootTimer = 0;
			}
			break;
		}
	}
}

void updateEnemyBullets() {
	detectEnemyShoot();
	moveEnemyBullets();
	deleteEnemyBullets();
}

void updateEnemySprites() {
	extern int spritesTimer;
	extern int MS;
	spritesTimer += MS;
	if (spritesTimer >= 100) {
		extern int sprites3;
		extern int sprites4;
		sprites3 = (sprites3 + 1) % 3;
		sprites4 = (sprites4 + 1) % 4;
		spritesTimer = 0;
		for (int i = 0; i < totalEnemies; i++) {
			if (enemies[i].schedule == 3) {
				extern SDL_Rect shipRect[];
				int distanceX = (shipRect[0].x + shipRect[0].w / 2) - (enemies[i].rect.x + enemies[i].rect.w / 2);
				int distanceY = (enemies[i].rect.y + enemies[i].rect.h / 2) - (shipRect[0].y + shipRect[0].h / 2);
				double div = (double)distanceY / (double)distanceX;
				if (distanceX > 0) {
					if (div > 1.732) {
						enemies[i].sprite = 3;
					}
					else if (div > 0.57773) {
						enemies[i].sprite = 4;
					}
					else if (div > -0.57773) {
						if (enemies[i].up)
							enemies[i].sprite = 11;
						else
							enemies[i].sprite = 5;
					}
					else if (div > -1.732) {
						enemies[i].sprite = 6;
					}
					else {
						enemies[i].sprite = 7;
					}
				}
				else {
					if (div > 1.732) {
						enemies[i].sprite = 8;
					}
					else if (div > 0.57773) {
						enemies[i].sprite = 9;
					}
					else if (div > -0.57773) {
						if (enemies[i].up)
							enemies[i].sprite = 10;
						else
							enemies[i].sprite = 0;
					}
					else if (div > -1.732) {
						enemies[i].sprite = 1;
					}
					else {
						enemies[i].sprite = 2;
					}
				}
			}
			else if (enemies[i].schedule == 5 && enemies[i].stop) {
				extern SDL_Rect shipRect[];
				if (shipRect[0].x - enemies[i].rect.x > 0) {
					if (enemies[i].rect.y - shipRect[0].y > 0) {
						if(!enemies[i].up)
							//aim up and right
							enemies[i].sprite = 1;
						else
							//aim down and right
							enemies[i].sprite = 3;
					}
					else {
						if (enemies[i].up)
							//aim down and right
							enemies[i].sprite = 3;
						else
							//aim up and right
							enemies[i].sprite = 1;
					}
				}
				else {
					if (enemies[i].rect.y - shipRect[0].y > 0) {
						
						if(!enemies[i].up)
							//aim up and left
							enemies[i].sprite = 0;
						else
							//aim down and left
							enemies[i].sprite = 2;
					}
					else {
						if (enemies[i].up)
							//aim down and left
							enemies[i].sprite = 2;
						else
							//aim up and left
							enemies[i].sprite = 0;
					}
				}
			}
		}
	}
}

//delete an upgrade when it are out of the screen
void deleteUpgradeRect() {
	for (int i = totalUpgradeRects - 1; i >= 0; i--) {
		if (upgradeRects[i].rect.x + upgradeRects[i].rect.w <= 0) {
			deleteOfActiveRect(upgradeRects, totalUpgradeRects, upgradeRects[i].id);
			break;
		}
	}
}

//delete an enemy when it are out of the screen
void deleteEnemies() {
	for (int i = totalEnemies - 1; i >= 0; i--) {
		if (enemies[i].rect.x + enemies[i].rect.w <= 0) {
			deleteOfEnemies(enemies, totalEnemies, enemies[i].id);
			break;
		}
		if (enemies[i].schedule == 8) {
			extern int SCREEN_HEIGHT;
			extern int SCREEN_WIDTH;
			extern int gameHeight;
			if (enemies[i].rect.x > SCREEN_WIDTH || enemies[i].rect.y + enemies[i].rect.h > gameHeight || enemies[i].rect.y < 0) {
				deleteOfEnemies(enemies, totalEnemies, enemies[i].id);
				break;
			}
		}
	}
}



//move the rect at the same time that the background
void moveLeftWithBackground(SDL_Rect& rect){
	extern int backgroundTimer;
	if (backgroundTimer >= maxBackground) {
		extern int speedCenterMap;
		rect.x -= speedCenterMap;
	}
}

void changeSpriteDestroyedEnemies() {
	extern int spritesTimer;
	if (spritesTimer == 0) {
		for (int i = totalDestroyedEnemy - 1; i >= 0; i--) {
			destroyedEnemy[i].colisionType++;
			if (destroyedEnemy[i].colisionType > 2) {
				deleteOfSquares(destroyedEnemy, totalDestroyedEnemy, destroyedEnemy[i].id);
			}
		}
	}
}

void moveDestroyedEnemies() {
	for (int i = 0; i < totalDestroyedEnemy; i++) {
		moveLeftWithBackground(destroyedEnemy[i].rect);
	}
}

//move to left, change the sprite and delete the destoyed enemy 
void updateDestroyedEnemies() {
	moveDestroyedEnemies();
	changeSpriteDestroyedEnemies();
}

//move the upgrades at the same time that the background
void updateUpgradeRects() {
	for (int i = 0; i < totalUpgradeRects; i++) {
		moveLeftWithBackground(upgradeRects[i].rect);
	}
}

//move the enemy 9 when 2 players are playing
void moveEnemy9With2Players(int i) {
	if (enemies[i].top) {
		enemies[i].rect.y += enemies[i].speedY;
		extern int gameHeight;
		if (enemies[i].rect.y + enemies[i].rect.h / 2 >= gameHeight)
			enemies[i].top = false;
	}
	else {
		enemies[i].rect.y -= enemies[i].speedY;
		if (enemies[i].rect.y <= 0)
			enemies[i].top = true;
	}
}



//move the enemy 9 when 1 player is playing
void moveEnemy9(int i) {
	extern SDL_Rect shipRect[];
	if (enemies[i].rect.y + enemies[i].rect.h / 2 - (shipRect[0].y + shipRect[0].h / 2) > 0) {
		//boos is below of ship
		enemies[i].rect.y -= enemies[i].speedY;
	}
	else {
		//boos is above of ship
		enemies[i].rect.y += enemies[i].speedY;
	}
}

void moveEnemy8(int i) {
	if (!enemies[i].stop) {
		extern int mountainX[];
		extern int mountainY;
		int actualY = enemies[i].rect.y;
		enemies[i].rect.x += enemies[i].speedX;
		enemies[i].rect.y = (int)(mountainY - (enemies[i].speedY * abs(sin((enemies[i].rect.x - mountainX[enemies[i].total]) * PI / 180))));
		if ((enemies[i].speedX > 0 && enemies[i].rect.x - mountainX[enemies[i].total] >= 180) || (enemies[i].speedX < 0 && enemies[i].rect.x - mountainX[enemies[i].total] <= -180)) {
			enemies[i].stop = true;
			enemies[i].speedY = enemies[i].rect.y - actualY;
		}
	}
	else {
		enemies[i].rect.x += enemies[i].speedX;
		enemies[i].rect.y += enemies[i].speedY;
	}
}

void moveEnemy7(int i) {
	extern int squareHeight;
	if (enemies[i].up) {
		if (!enemies[i].top) {
			moveLeftWithBackground(enemies[i].rect);
			enemies[i].rect.y += enemies[i].speedY;
			if (enemies[i].rect.y / squareHeight >= enemies[i].limit.y) {
				enemies[i].top = true;
			}
		}
		else {
			enemies[i].rect.x -= enemies[i].speedX;
		}
	}
	else {
		if (!enemies[i].top) {
			moveLeftWithBackground(enemies[i].rect);
			enemies[i].rect.y -= enemies[i].speedY;
			if (enemies[i].rect.y / squareHeight <= enemies[i].limit.y) {
				enemies[i].top = true;
			}
		}
		else {
			enemies[i].rect.x -= enemies[i].speedX;
		}
	}
}

void moveEnemy5(int i) {
	//move to right
	if (!enemies[i].stop) {
		enemies[i].rect.x += enemies[i].speedX;
		if (!enemies[i].up) {
			extern int squareWidth;
			extern int squareHeight;
			extern int progress;
			int x = progress - 14 + (enemies[i].rect.x + enemies[i].rect.w) / squareWidth;
			int y = enemies[i].rect.y / squareHeight;
			if (!enemies[i].climbing) {
				for (int k = 0; k < totalCenterSquares; k++) {
					if (centerWindowSquares[k].progress > x) {
						break;
					}
					if (centerWindowSquares[k].progress == x) {
						if (centerWindowSquares[k].typeSquare[0] == 410) {
							enemies[i].climbing = true;
							enemies[i].limit.y = 11; // max coordinate of mountain (top)
							break;
						}
						else if (centerWindowSquares[k].typeSquare[0] == 510) {
							enemies[i].climbing = true;
							enemies[i].limit.y = 12; // max coordinate of mountain (top)
							break;
						}
					}
				}
			}
			else {
				if (!enemies[i].top) {
					enemies[i].rect.y -= enemies[i].speedY;
					if (y < enemies[i].limit.y) {
						enemies[i].top = true;
					}
				}
				else {
					if (enemies[i].limit.y != 12) {
						enemies[i].rect.y += enemies[i].speedY;
						if (y >= 13) {
							enemies[i].climbing = false;
						}
					}
					else {
						//the enemy that are on mountain 5 will down on this situation
						for (int k = 0; k < totalCenterSquares; k++) {
							if (centerWindowSquares[k].progress > x) {
								break;
							}
							if (centerWindowSquares[k].progress == x && centerWindowSquares[k].typeSquare[0] == 560) {
								enemies[i].rect.y += enemies[i].speedY;
							}
						}
					}
				}
			}
		}
		extern int SCREEN_WIDTH;
		if (enemies[i].rect.x >= SCREEN_WIDTH / 2) {
			enemies[i].stop = true;
		}
	}
	else {
		if (enemies[i].limit.x < 3) {
			moveLeftWithBackground(enemies[i].rect);
		}
		else {
			enemies[i].rect.x -= (int)(2 * enemies[i].speedX);
		}
	}
}

void moveEnemy4(int i) {
	extern int SCREEN_WIDTH;
	enemies[i].rect.x -= enemies[i].speedX;
	int center = (enemies[i].limit.x + enemies[i].limit.y) / 2;
	int longitude = center - enemies[i].limit.x;
	enemies[i].rect.y = (int)(longitude * abs(sin((SCREEN_WIDTH - enemies[i].rect.x) * PI / 180))) + center;
}

void moveEnemy2(int i) {
	extern int SCREEN_WIDTH;
	enemies[i].rect.x -= enemies[i].speedX;
	int center = (enemies[i].limit.x + enemies[i].limit.y) / 2;
	int longitude = center - enemies[i].limit.x;
	enemies[i].rect.y = (int)(longitude * sin((SCREEN_WIDTH - enemies[i].rect.x) * PI / 180)) + center;
}

void moveEnemies() {
	for (int i = 0; i < totalEnemies; i++) {
		switch (enemies[i].schedule) {
		case 0:
			enemies[i].rect.x -= enemies[i].speedX;
			break;
		case 1:
			if (enemies[i].up) {
				enemies[i].rect.y -= enemies[i].speedY;
				if (enemies[i].rect.y <= enemies[i].limit.x) {
					enemies[i].up = false;
				}
			}
			else {
				enemies[i].rect.y += enemies[i].speedY;
				if (enemies[i].rect.y >= enemies[i].limit.y - enemies[i].rect.h) {
					enemies[i].up = true;
				}
			}
			enemies[i].rect.x -= enemies[i].speedX;
			break;
		case 2:
			moveEnemy2(i);
			break;
		case 3:
			moveLeftWithBackground(enemies[i].rect);
			break;
		case 4:
			moveEnemy4(i);
			break;
		case 5:
			moveEnemy5(i);
			break;
		case 6:
			moveLeftWithBackground(enemies[i].rect);
			nextEnemy(i);
			break;
		case 7:
			moveEnemy7(i);
			break;
		case 8:
			moveEnemy8(i);
			break;
		case 9:
			extern int players;
			if(players > 1)
				moveEnemy9With2Players(i);
			else
				moveEnemy9(i);
			break;
		}
	}
}

//check if the ship are on the mountain route
void checkMountainRoute(int shipIndex) {
	extern int progress;
	//when mountain 5 are on game
	if (progress > 30 && progress < 64) {
		extern SDL_Rect shipRect[];
		extern SDL_Point mountainRoute[];
		extern int actualMountainIndex;
		extern int squareWidth;
		extern int squareHeight;
		int x = progress - 14 + (shipRect[shipIndex].x + shipRect[shipIndex].w / 2) / squareWidth;
		int y = (shipRect[shipIndex].y + shipRect[shipIndex].h / 2) / squareHeight;
		if (x == mountainRoute[actualMountainIndex].x && y == mountainRoute[actualMountainIndex].y) {
			actualMountainIndex++;
			if (actualMountainIndex > 2) {
				extern bool mountainPoints;
				mountainPoints = true;
				for (int i = 0; i < totalCenterSquares; i++) {
					//last square of the mountain x
					if (centerWindowSquares[i].progress == 49) {
						extern SDL_Rect mountainPuntuationRect;
						mountainPuntuationRect = { centerWindowSquares[i].rect[0].x - (int) (2.5 * squareWidth), centerWindowSquares[i].rect[0].y - squareHeight, centerWindowSquares[i].rect[0].w, centerWindowSquares[i].rect[0].h };
						break;
					}
				}
				extern int players;
				extern int score[];
				extern SDL_Texture* menuTextTexture[];
				for (int i = 0; i < players; i++) {
					score[i] += 5000;
					changeTextToScoreTexture(std::to_string(score[i]), menuTextTexture[1 + i * 2]);
				}
				Mix_PlayChannel(2, sound[6], 0);
			}
		}
	}
}

void deleteMountainPuntuation() {
	extern SDL_Rect mountainPuntuationRect;
	if (mountainPuntuationRect.x + mountainPuntuationRect.w < 0) {
		extern bool mountainPoints;
		mountainPoints = false;
	}
}

void updateMountainPuntuationRect() {
	extern SDL_Rect mountainPuntuationRect;
	moveLeftWithBackground(mountainPuntuationRect);
	deleteMountainPuntuation();
}

void updateLevelProgress() {
	switch (level) {
	case 1:
		switch (phase) {
		case 0:
			level1Phase0();
			if (generatingEnemies) {
				extern int MS;
				lineTimer += MS;
				if (lineTimer >= timeAppear) {
					lineTimer = 0;
					generateEnemies(enemyRound[actualRound - 1].x, enemyRound[actualRound - 1].y);
				}
			}
			updateBackgroundTimer();
			break;
		case 1:
			checkBulletMapColision();
			extern int players;
			extern int lifes[];
			extern bool inmunity[];
			for (int i = 0; i < players; i++) {
				if (lifes[i] > 0 && !inmunity[i]) {
					checkShipMapColision(i);
				}
			}
			extern int backgroundTimer;
			if (backgroundTimer >= maxBackground) {
				moveSquares();
				updateProgress();
			}
			updateBackgroundTimer();
			extern bool mountainPoints;
			if (!mountainPoints) {
				extern int players;
				extern int lifes[];
				extern bool inmunity[];
				for (int i = 0; i < players; i++) {
					if (lifes[i] > 0 && !inmunity[i]) {
						checkMountainRoute(i);
					}
				}
			}
			else {
				updateMountainPuntuationRect();
			}
			break;
		case 2:
			checkBulletMapColision();
			level1Phase2();
			extern int players;
			extern int lifes[];
			extern bool inmunity[];
			for (int i = 0; i < players; i++) {
				if (lifes[i] > 0 && !inmunity[i]) {
					checkShipMapColision(i);
				}
			}
			updateBackgroundTimer();
			break;
		case 3:
			checkBulletMapColision();
			extern int backgroundTimer;
			if (backgroundTimer >= maxBackground) {
				moveSquares();
				if (totalDownSquares <= 0) {
					extern Mix_Music* music[];
					phase++;
					generateEnemy9();
					//pause music 1 and play music 2
					extern Mix_Music* music[];
					Mix_PauseMusic();
					Mix_PlayMusic(music[2], -1);
				}
			}
			extern int players;
			extern int lifes[];
			extern bool inmunity[];
			for (int i = 0; i < players; i++) {
				if (lifes[i] > 0 && !inmunity[i]) {
					checkShipMapColision2(i);
				}
			}
			updateBackgroundTimer();
			break;
		case 4:
			if (totalEnemies < 1) {
				loopRestart();
			}
			break;
		}
		break;
	}
}

void updateBulletTimer(int shipIndex) {
	if (shoot[shipIndex] == false) {
		extern int MS;
		bulletTimer[shipIndex] += MS;
		if (bulletTimer[shipIndex] >= 150) {
			shoot[shipIndex] = true;
			bulletTimer[shipIndex] = 0;
		}
	}
}

void updateTimerPause() {
	extern bool canPause;
	if (!canPause) {
		extern int MS;
		extern int pauseTimer;
		pauseTimer += MS;
		if (pauseTimer > 500) {
			canPause = true;
			pauseTimer = 0;
		}
	}
}


//check the ship movement with down colisions
void checkMissileDownSquareMovement(int missileIndex, int shipIndex) {
	extern int squareWidth;
	extern int squareHeight;
	extern int progress;
	int x = progress - 14 + (missiles[shipIndex][missileIndex].rect.x + missiles[shipIndex][missileIndex].rect.w) / squareWidth;
	int y = (missiles[shipIndex][missileIndex].rect.y + missiles[shipIndex][missileIndex].rect.h) / squareHeight;
	if (x >= 15 - totalDownSquares) {
		if (y > 13) {
			missiles[shipIndex][missileIndex].movement = 0;
		}
	}
}

//check if the missile are on down squares (it is used in the end of the level 1: phase >= 3)
void checkMissileDown(int missileIndex, int shipIndex) {
	if (totalDownSquares > 0) {
		extern int squareHeight;
		extern SDL_Rect shipRect[];
		int lastX = downWindowSquares[totalDownSquares - 1].rect.x + downWindowSquares[totalDownSquares - 1].rect.w;//last pixel of down squares
		int lastY = (missiles[shipIndex][missileIndex].rect.y + missiles[shipIndex][missileIndex].rect.h) / squareHeight;

		if (missiles[shipIndex][missileIndex].rect.x <= lastX && lastY > 13) 
			missiles[shipIndex][missileIndex].movement = 0;
		else
			missiles[shipIndex][missileIndex].movement = 1;
	}
}

//check the missile movement with the center colisions of the map (except trees)
void checkMissileMovement(int missileIndex, int shipIndex) {
	extern int squareWidth;
	extern int squareHeight;
	extern int progress;
	int x = progress - 14 + (missiles[shipIndex][missileIndex].rect.x + missiles[shipIndex][missileIndex].rect.w) / squareWidth;
	int y = (missiles[shipIndex][missileIndex].rect.y) / squareHeight;
	int rightX = progress - 14 + (missiles[shipIndex][missileIndex].rect.x + missiles[shipIndex][missileIndex].rect.w + bulletSpeed[shipIndex] / 2) / squareWidth; // coordinate to right (bulletSpeed / 2)
	int downY = (missiles[shipIndex][missileIndex].rect.y + bulletSpeed[shipIndex] / 2) / squareHeight;// coordinate to down (bulletSpeed / 2)
	for (int k = 0; k < totalCenterSquares; k++) {
		if (centerWindowSquares[k].progress > rightX) {
			//missile are on space, falling down
			missiles[shipIndex][missileIndex].movement = 1;
			return;
		}
		if (centerWindowSquares[k].progress == rightX) {
			for (int g = centerWindowSquares[k].totalElements - 1; g >= 0; g--) {
				if (centerWindowSquares[k].typeSquare[g] > 72) {
					if (centerWindowSquares[k].positionY[g] == y) {
						//missile crash with colision
						extern bool shootMissile[2][3];
						shootMissile[shipIndex][missiles[shipIndex][missileIndex].id] = true;
						deleteOfMissile(missiles[shipIndex], totalMissiles[shipIndex], missiles[shipIndex][missileIndex].id);
						return;
					}
					else if (centerWindowSquares[k].positionY[g] == downY) {
						//move to right: movement = 0;
						missiles[shipIndex][missileIndex].movement = 0;
						return;
					}
				}
			}
		}
	}
	//missile are on space, falling down
	missiles[shipIndex][missileIndex].movement = 1;
}

//check the missile colisions in phase lower than 3, when has center squares on game 
void checkMissileMapMovement(int shipIndex) {
	if (totalCenterSquares > 0) {
		for (int i = 0; i < totalMissiles[shipIndex]; i++) {
			checkMissileMovement(i, shipIndex);
			if (missiles[shipIndex][i].movement == 1) {
				checkMissileDownSquareMovement(i, shipIndex);
			}	
		}
	}
}

//delete a missile when it are out of the screen
void deleteMissiles(int shipIndex) {
	extern int gameHeight;
	extern int SCREEN_WIDTH;
	for (int i = totalMissiles[shipIndex] - 1; i >= 0; i--) {
		if (missiles[shipIndex][i].rect.y >= gameHeight || missiles[shipIndex][i].rect.x >= SCREEN_WIDTH) {
			extern bool shootMissile[2][3];
			shootMissile[shipIndex][missiles[shipIndex][i].id] = true;
			deleteOfMissile(missiles[shipIndex], totalMissiles[shipIndex], missiles[shipIndex][i].id);
		}
	}
}

void moveMissiles(int shipIndex) {
	for (int i = 0; i < totalMissiles[shipIndex]; i++) {
		missiles[shipIndex][i].rect.x += missiles[shipIndex][i].speed;
		if (missiles[shipIndex][i].movement == 1)
			missiles[shipIndex][i].rect.y += missiles[shipIndex][i].speed;
	}
}

void updateMissiles(int shipIndex) {
	moveMissiles(shipIndex);
	deleteMissiles(shipIndex);
	if(phase < 3){
		checkMissileMapMovement(shipIndex);
	}
	else {
		for (int i = 0; i < totalMissiles[shipIndex]; i++) {
			checkMissileDown(i, shipIndex);
		}
	}
}

//update double bullets of the ship[shipIndex] and its options (move and delete if it are out of the screen)
void updateDoubleBullets(int shipIndex) {
	for (int i = totalDoubleBullets[shipIndex] - 1; i >= 0; i--) {
		doubleBullets[shipIndex][i].rect.x += (int)(0.7 * bulletSpeed[shipIndex]);
		doubleBullets[shipIndex][i].rect.y -= (int)(0.7 * bulletSpeed[shipIndex]);
		extern int SCREEN_WIDTH;
		if (doubleBullets[shipIndex][i].rect.x >= SCREEN_WIDTH || doubleBullets[shipIndex][i].rect.y + doubleBullets[shipIndex][i].rect.h <= 0){
			deleteOfActiveRect(doubleBullets[shipIndex], totalDoubleBullets[shipIndex], doubleBullets[shipIndex][i].id);
		}
	}
	extern int option[];
	for (int j = 0; j < option[shipIndex]; j++) {
		for (int i = totalDoubleOptionBullets[shipIndex][j] - 1; i >= 0; i--) {
			doubleOptionBullets[shipIndex][j][i].rect.x += (int)(0.7 * bulletSpeed[shipIndex]);
			doubleOptionBullets[shipIndex][j][i].rect.y -= (int)(0.7 * bulletSpeed[shipIndex]);
			extern int SCREEN_WIDTH;
			if (doubleOptionBullets[shipIndex][j][i].rect.x >= SCREEN_WIDTH || doubleOptionBullets[shipIndex][j][i].rect.y + doubleOptionBullets[shipIndex][j][i].rect.h <= 0) {
				deleteOfActiveRect(doubleOptionBullets[shipIndex][j], totalDoubleOptionBullets[shipIndex][j], doubleOptionBullets[shipIndex][j][i].id);
			}
		}
	}
}

//update  bullets of the ship[shipIndex] and its options (move and delete if it are out of the screen)
void updateBullets(int shipIndex) {
	for (int i = totalBullets[shipIndex] - 1; i >= 0 ; i--) {
		bullets[shipIndex][i].rect.x += bulletSpeed[shipIndex];
		extern int SCREEN_WIDTH;
		if (bullets[shipIndex][i].rect.x >= SCREEN_WIDTH)
			deleteOfActiveRect(bullets[shipIndex], totalBullets[shipIndex], bullets[shipIndex][i].id);
	}
	//move option bullets
	for (int j = 0; j < 2; j++) {
		for (int i = totalOptionBullets[shipIndex][j] - 1; i >= 0; i--) {
			optionBullets[shipIndex][j][i].rect.x += bulletSpeed[shipIndex];
			extern int SCREEN_WIDTH;
			if (optionBullets[shipIndex][j][i].rect.x >= SCREEN_WIDTH)
				deleteOfActiveRect(optionBullets[shipIndex][j], totalOptionBullets[shipIndex][j], optionBullets[shipIndex][j][i].id);
		}
	}
	
}

void moveCenterMap(int speedSquare) {
	for (int i = 0; i < totalCenterSquares; i++) {
		for (int j = 0; j < centerWindowSquares[i].totalElements; j++) {
			centerWindowSquares[i].rect[j].x -= speedSquare;
		}
	}
}

//moves the center squares to left and if the first square are out of the screen it will be deleted
void updateCenterMap() {
	if (totalCenterSquares > 0) {
		extern int speedCenterMap;
		moveCenterMap(speedCenterMap);
		if (centerWindowSquares[0].rect[0].x + centerWindowSquares[0].rect[0].w <= 0) {
			deleteOfCenterSquares(centerWindowSquares, totalCenterSquares, centerWindowSquares[0].progress);
		}
	}
}

//add the new squares when the last down square are inside the screen by the right side
void updateProgress() {
	extern int SCREEN_WIDTH;
	if (downWindowSquares[totalDownSquares - 1].rect.x + downWindowSquares[totalDownSquares - 1].rect.w <= SCREEN_WIDTH) {
		extern int progress;
		

		progress++;
		if (progress < 144) {
			extern int downColisions[];
			extern int upColisions[];
			extern CenterSquare centerColisions[];
			extern int squareHeight;
			extern int squareWidth;
			Square element;

			//add the down element
			element = { totalDownSquares,{downWindowSquares[totalDownSquares - 1].rect.x + downWindowSquares[totalDownSquares - 1].rect.w,(int)(14 * squareHeight),squareWidth,squareHeight}, downColisions[progress] };
			addToEndSquare(downWindowSquares, totalDownSquares, element);

			//add the up element
			element = { totalUpSquares,{upWindowSquares[totalUpSquares - 1].rect.x + upWindowSquares[totalUpSquares - 1].rect.w,0,squareWidth,squareHeight}, upColisions[progress] };
			addToEndSquare(upWindowSquares, totalUpSquares, element);

			//add the center element 
			if (centerColisions[actualCenterColision].progress == progress + 1) {
				for (int i = 0; i < centerColisions[actualCenterColision].totalElements; i++) {
					centerColisions[actualCenterColision].rect[i] = { downWindowSquares[totalDownSquares - 1].rect.x + downWindowSquares[totalDownSquares - 1].rect.w, (int)(centerColisions[actualCenterColision].positionY[i] * squareHeight),squareWidth,squareHeight };
				}
				addToEndCenterSquare(centerWindowSquares, totalCenterSquares, centerColisions[actualCenterColision]);
				actualCenterColision++;
			}

			//add the correspondient enemy
			extern Enemy progressEnemiesLevel1[];
			extern int actualProgressEnemy;
			if (progressEnemiesLevel1[actualProgressEnemy].progress == progress + 1) {
				generateEnemy(progressEnemiesLevel1[actualProgressEnemy]);
				actualProgressEnemy++;
			}
		}
		else {
			phase++;
		}
	}
}

//move windowSquares 0 to left 
void moveBoundMap(int speedSquare, Square windowSquares[], int index) {
	for (int i = 0; i < index; i++) {
		windowSquares[i].rect.x -= speedSquare;
	}
}


//move the squares to left and if the first square are out of the screen it will be deleted
void updateBoundMap(Square windowSquares[], int& index, int colisions[]) {
	extern int speedCenterMap;
	moveBoundMap(speedCenterMap, windowSquares, index);

	//delete the square of the left side if it are out of the screen
	if (windowSquares[0].rect.x + windowSquares[0].rect.w <= 0) {
		deleteOfSquares(windowSquares, index, 0);
	}
}

//move all the squares of the map
void moveSquares() {
	extern int downColisions[];
	extern int upColisions[];
	updateBoundMap(downWindowSquares, totalDownSquares, downColisions);
	updateBoundMap(upWindowSquares, totalUpSquares, upColisions);
	updateCenterMap();
}

void updateBackground() {
	extern SDL_Rect backgroundImageRect;
	backgroundImageRect.x++;
	if (backgroundImageRect.x == 800) {
		backgroundImageRect.x = 288;
	}
}

void updateBackgroundTimer() {
	extern int backgroundTimer;
	if (backgroundTimer >= maxBackground) {
		updateBackground();
	}
}

void restartBackgroundTimer() {
	extern int backgroundTimer;
	if (backgroundTimer >= maxBackground)
		backgroundTimer = 0;
}

void incrementInmunityTimer(int shipIndex) {
	extern bool inmunity[];
	if (inmunity[shipIndex]) {
		extern int inmunityTimer[];
		extern int MS;
		inmunityTimer[shipIndex] += MS;
		if (inmunityTimer[shipIndex] >= 3000) {
			inmunity[shipIndex] = false;
			inmunityTimer[shipIndex] = 0;
		}
	}
}

void incrementBackgroundTimer() {
	extern int backgroundTimer;
	backgroundTimer++;
}

void update() {
	extern SDL_Rect shipRect[];
	extern int players;
	extern int score[];
	extern int lifes[];
	incrementBackgroundTimer();
	updateLevelProgress();
	moveEnemies();
	deleteEnemies();
	updateEnemySprites();
	updateDestroyedEnemies();
	updateEnemyBullets();
	updateUpgradeRects();

	for (int i = 0; i < players; i++) {
		if (lifes[i] > 0) {
			incrementInmunityTimer(i);
			updateBullets(i);
			updateDoubleBullets(i);
			updateMissiles(i);
			updateBulletTimer(i);
			checkBulletEnemiesColision(totalBullets[i], bullets[i], score[i], i);
			checkBulletEnemiesColision(totalOptionBullets[i][0], optionBullets[i][0], score[i], i);
			checkBulletEnemiesColision(totalOptionBullets[i][1], optionBullets[i][1], score[i], i);
			checkBulletEnemiesColision(totalDoubleBullets[i], doubleBullets[i], score[i], i);
			checkBulletEnemiesColision(totalDoubleOptionBullets[i][0], doubleOptionBullets[i][0], score[i], i);
			checkBulletEnemiesColision(totalDoubleOptionBullets[i][1], doubleOptionBullets[i][1], score[i], i);
			checkMissileEnemiesColision(totalMissiles[i], missiles[i], score[i], i);
			checkEnemyBulletShipColision(i);
			checkUpgradeRectShipColision(i);
			checkSHipEnemiesColision(i);
			checkShieldEnemiesColision(i);
		}
	}
	restartBackgroundTimer();
}