#include <SDL.h>
#include "enemy.h"
#include "square.h"
#include "activeRect.h"

//game
int SCREEN_WIDTH = 1080;
int SCREEN_HEIGHT = 720;
SDL_Window* window;
SDL_Renderer* renderer;
bool gameOver = false;
int MS = 10;
int gameHeight = (int)(SCREEN_HEIGHT * 0.895);
bool pause = false;
int pauseTimer = 0; //timer to use the joystick when pause the game
bool canPause = true; // indicates if the joystick can pause and continue the game

//menu
int actualMenu = 0; //indicates if the game are on the menu: 0->principal menu, 1->playing game
int players = 1;
SDL_Rect imageMenuRect;// rect of menu 0
SDL_Texture* imageMenuTexture; // image of menu 0
SDL_Texture* lifeTexture[2]; // ship life text
SDL_Rect lifeRect[2]; // rect of ship life text
SDL_Texture* menuTextTexture[4]; // menu information text
SDL_Rect menuTextRects[4]; // text of menu 1
SDL_Rect menuUpgradeRects[2][6]; // square of the upgrade rects of menu 1 (2 ships , 6 upgrades)
SDL_Texture* mountainPuntuationTexture;//score when ship goes through the mountain
SDL_Rect mountainPuntuationRect;
int score[2];
int lifes[2];

//ship
SDL_Rect shipRect[2];
SDL_Texture* shipTexture;
int shipSpeed[2];
int direction[2]; //0: normal, 1: up, 2: down  
bool inmunity[2]; //inmunity to the ship (2 ships)
int inmunityTimer[2]; 

//menu and levels
SDL_Texture* menuTexture; // image of upgrade rects on menu 1 
SDL_Texture* levelOneTexture; // image of the complete level 1

//joystick
SDL_Joystick* joystick[2];
int totalJoysticks = 0;

//background
SDL_Rect backgroundImageRect;//rect of the background of game (space with stars)
SDL_Texture* downBackgroundTexture;// image of levels 1,2,6,7 (down)
SDL_Texture* upBackgroundTexture;// image of levels 1,2,6,7 (up)
int backgroundTimer = 0; //timer to move the background image

//bullets
ActiveRect bullets[2][10];//principal bullet ship
int totalBullets[2];// total ship main bullets
int bulletSpeed[2];
int bulletTimer[2]; // indicates the time to shoot another bullet
bool shoot[2];// indicates if the ship can shoot

//level
int level = 1;
int phase = 0;
int actualRound = 0;
int lastRound = 18 ;
int roundTimes[18];//indicates the time to begin the round i
SDL_Point enemyRound[18]; //indicates the enemy that appears in roundTimes i: (a,b) -> a: enemy type, b: up or dow 
int roundTimer = 0;//timer to  begin the next round
bool generatingEnemies = false; // indicates if the 4 enemies line (type 0) are appearing
int timeAppear = 150; // time in microseconds to appear a type 0 enemy
int lineTimer = 0; //timer to the type 0 enemies
int enemiesType0Appeared = 0; // type 0 enemies appeared
int enemy8Timer = 0; // timer to generate the next enemy type 7 of the phase 2
int generatedEnemies8; // indicates the generated type 8 enemies on phase 2 
int mountainY = (int)(SCREEN_HEIGHT * 0.6375);
int mountainX[2];

//enemy
Enemy enemies[30];//enemies on game
int totalEnemies = 0;//total enemies on game
SDL_Texture* enemyTexture;
SDL_Texture* enemy2Texture;
SDL_Texture* enemyReverseTexture;
SDL_Texture* enemyReverse2Texture;
EnemyBullet enemyBullets[30];//bullets on game
int totalEnemyBullets = 0;

//sprites
int sprites3 = 0;
int sprites4 = 0;
int spritesTimer = 0;

//colisions
int progress = 0;

int actualProgressEnemy = 0; //indicates the actual enemy on phase 1

Square downWindowSquares[16];//down squares on game
int totalDownSquares = 0;
Square upWindowSquares[16];//up squares on game
int totalUpSquares = 0;
CenterSquare centerWindowSquares[64]; // center squares on the board
int totalCenterSquares = 0;
int actualCenterColision = 0; //index of the actual center colisions

//upgrade
ActiveRect upgradeRects[10]; // upgrades on game
int totalUpgradeRects = 0; //total upgrades on game (red balls)
int indexLineEnemy = 0; // index of the line of type 0 enemies, every line has a index
int totalLineEnemies[4]; // this variable indicates the total type 0 enemies on the line i
int actualUpgrade[2]; // index of the actual possible upgrade
int limitUpgrades[2][6]; // indicates the times that an upgrade can be geted

//weapons
bool missile[2]; // indicates if the missile upgrade has geted 
bool shootMissile[2][3]; // this variable indicates if the missile can be shooted: 0: ship, 1: option1, 2: option2
Missile missiles[2][3]; 
int totalMissiles[2];

bool doubleShoot[2];// indicates if the double shoot upgrade has been geted
ActiveRect doubleBullets[2][10];
int totalDoubleBullets[2];

bool laser[2];// indicates if the laser upgrade has been geted
int bulletDamage[2];

int option[2];// indicates if the option upgrade has been geted
SDL_Rect optionRect[2][2];
SDL_Point optionRoute[2][2][20];
int nextElement[2][2]; // 2 ships, 2 option -> index of the next route element: optionRoute   index of actual coordinate of optionRoute

ActiveRect optionBullets[2][2][10];// 2 ships, 2 option
int totalOptionBullets[2][2];// 2 ships, 2 option

ActiveRect doubleOptionBullets[2][2][10];// 2 ships, 2 option
int totalDoubleOptionBullets[2][2];// 2 ships, 2 option

bool shield[2];// indicates if the shield upgrade has been geted
int shieldLife[2];
SDL_Rect shieldRect[2];

int squareWidth = SCREEN_WIDTH / 14;
int squareHeight = gameHeight / 15;
int speedCenterMap = squareWidth / 10;

int indexShootShip = 0; // indicates the ship that will be shooted by an enemy

Square destroyedEnemy[30]; // sprite when an enemy dies
int totalDestroyedEnemy = 0;

bool init();
void initVariables();
void menuInit();
void loadImage();
void close();
void detectKey();
void show();
void update();
void loadJoystick();
void detectKeyJoystick(int i);
void detectKeyJoystickMenu0(SDL_Joystick* joystick);
void detectKeyMenu0();
void showMenu0();
void moveToRightMenu0();
void updateTimerPause();
void initSound();


int main(int argc, char* args[]) {
	if (!init())
		return -1;
	initVariables();
	menuInit();
	loadImage();
	loadJoystick();
	initSound();
	while (!gameOver) {
		if (actualMenu == 0) {
			if (phase == 0) {
				moveToRightMenu0();
			}
			detectKeyJoystickMenu0(joystick[0]);
			detectKeyMenu0();
			showMenu0();
		}
		else {
			for (int i = 0; i < players; i++) {
				if (lifes[i] > 0) {
					if (totalJoysticks > i) {
						detectKeyJoystick(i);
					}
					detectKey();
				}
			}
			if (!pause) {
				update();
				show();
			}
			updateTimerPause();
			for (int i = 0; i < players; i++) {
				direction[i] = 0;
			}
		}
		SDL_Delay(MS);
	}
	close();
	return 0;
}