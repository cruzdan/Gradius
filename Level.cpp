#include <SDL.h>
#include <iostream>
#include <SDL_mixer.h>
#include "level.h"
#include "enemy.h"
#include "square.h"

//map down square type colisions
int downColisions[] = { 10,11,12,13,21,22,23,21,22,23,41,42,43,44,21,22,22,23,21,22,22,23,21,22,22,23,21,22,
	23,21,22,23,22,23,21,22,23,21,22,23,22,23,21,22,51,52,53,54,55,56,22,23,21,22,21,22,23,21,22,23,21,22,
	23,21,22,23,21,22,23,21,22,23,21,22,61,62,63,64,65,66,22,23,21,22,22,23,21,22,22,23,21,22,22,23,21,22,
	22,23,21,22,22,23,21,22,22,23,21,22,23,21,22,23,21,22,23,21,22,23,21,22,23,21,22,23,22,23,21,22,22,23,
	41,42,43,44,21,22,22,23,41,42,43,44,21,22
};

int upColisions[] = { 10,11,12,13,21,22,23,21,22,23,21,22,23,21,22,23,23,31,32,33,22,23,21,22,23,21,22,23,21,
	22,23,21,21,22,23,21,22,23,21,22,23,21,22,23,21,22,23,21,22,23,21,22,23,21,22,23,21,22,23,21,22,23,21,
	22,23,31,32,33,22,23,21,22,23,21,22,23,21,22,23,21,22,23,21,22,23,21,22,23,21,22,23,21,22,23,21,22,23,
	31,32,33,22,23,21,22,23,21,22,23,21,22,23,21,22,23,21,22,23,21,22,23,21,22,23,21,22,23,21,22,23,21,22,
	23,21,22,23,21,22,23,21,22,23,21,21,22
};

CenterSquare centerColisions[] = {
	{8,{71},{13},1},
	{9,{72},{13},1},
	{10,{410},{13},1},
	{11,{420,421,422},{13,12,11},3},
	{12,{430,431,432},{13,12,11},3},
	{13,{440,441},{13,12},2},
	{14,{71},{13},1},
	{15,{72},{13},1},
	{16,{71},{13},1},
	{17,{72},{13},1},
	{18,{71},{13},1},
	{19,{72},{13},1},
	{20,{71},{13},1},
	{21,{72},{13},1},
	{22,{71},{13},1},
	{23,{72},{13},1},
	{40,{71},{13},1},
	{41,{72},{13},1},
	{42,{71},{13},1},
	{43,{72},{13},1},
	{44,{510},{13},1},
	{45,{520,522},{13,11},2},
	{46,{530,532,533,534},{13,11,10,9},4},
	{47,{540,542,543,544},{13,11,10,9},4},
	{48,{550,552,553},{13,11,10},3},
	{49,{560},{13},1},
	{50,{71},{13},1},
	{51,{72},{13},1},
	{52,{71},{13},1},
	{53,{72},{13},1},
	{70,{71},{13},1},
	{71,{72},{13},1},
	{72,{71},{13},1},
	{73,{72},{13},1},
	{74,{610,611,612,613,614},{13,12,11,10,9},5},
	{75,{620,621,622,623,624},{13,12,11,10,9},5},
	{76,{630,631,632,633,634},{13,12,11,10,9},5},
	{77,{640,641,642,643,644},{13,12,11,10,9},5},
	{78,{650,651,652,653,654},{13,12,11,10,9},5},
	{79,{660,661,662,663,664},{13,12,11,10,9},5},
	{80,{71},{13},1},
	{81,{72},{13},1},
	{82,{71},{13},1},
	{83,{72},{13},1},
	{84,{71},{13},1},
	{85,{72},{13},1},
	{86,{71},{13},1},
	{87,{72},{13},1},
	{88,{71},{13},1},
	{89,{72},{13},1},
	{90,{71},{13},1},
	{91,{72},{13},1},
	{92,{71},{13},1},
	{93,{72},{13},1},
	{94,{71},{13},1},
	{95,{72},{13},1},
	{96,{71},{13},1},
	{97,{72},{13},1},
	{98,{71},{13},1},
	{99,{72},{13},1},
	{100,{71},{13},1},
	{101,{72},{13},1},
	{102,{71},{13},1},
	{103,{72},{13},1},
	{114,{80,800,801,802,803,804},{10,9,8,7,6,5},6},
	{115,{81,810,811,812,813,814},{10,9,8,7,6,5},6},
	{116,{82,820,821,822,823,824},{10,9,8,7,6,5},6},
	{117,{83,830,831,832,833,834},{10,9,8,7,6,5},6},
	{124,{71},{13},1},
	{125,{72},{13},1},
	{126,{71},{13},1},
	{127,{72},{13},1},
	{128,{71},{13},1},
	{129,{72},{13},1},
	{130,{410},{13},1},
	{131,{420,421,422},{13,12,11},3},
	{132,{430,431,432},{13,12,11},3},
	{133,{440,441},{13,12},2},
	{134,{71},{13},1},
	{135,{72},{13},1},
	{136,{71},{13},1},
	{137,{72},{13},1},
	{138,{410},{13},1},
	{139,{420,421,422},{13,12,11},3},
	{140,{430,431,432},{13,12,11},3},
	{141,{440,441},{13,12},2},
	{142,{71},{13},1},
	{143,{72},{13},1}
};

//indicates the enemies that appears on phase 1
Enemy progressEnemiesLevel1[] = {
	{2,1,true},
	{5,1,false},
	{8,1,true},
	{9,3,true,3},
	{12,4,false,0,true}, 
	{13,4,false},
	{15,1,true},
	{17,1,false},
	{19,5,true,2},
	{21,2,true},
	{23,2,false,0,true},
	{25,3,false,2},
	{28,6,false},
	{30,4,false},
	{32,4,false,0,true},
	{34,5,false,2},
	{36,2,true},
	{38,2,false},
	{40,2,true},
	{42,2,false},
	{45,5,true},
	{49,5,false},
	{51,4,false,0,true},
	{52,1,true},
	{55,1,false},
	{57,1,true},
	{59,3,false,1},
	{61,2,true},
	{63,2,false},
	{65,3,false,1},
	{67,3,false,1},
	{68,6,false},
	{69,1,true},
	{71,1,false},
	{73,1,true},
	{75,2,true},
	{77,2,false},
	{80,4,false},
	{85,1,true},
	{87,2,true},
	{89,2,false,0,true},
	{90,5,true,0,true},
	{92,1,true},
	{94,5,false},
	{96,3,true,2},
	{98,6,true},
	{99,2,true},
	{101,2,false,0,true},
	{103,1,true},
	{105,1,false},
	{106,5,false,2},
	{107,3,false,1,true},
	{109,6,false},
	{110,5,false,2},
	{114,5,true},
	{115,5,true,0,true},
	{116,4,false},
	{121,4,false},
	{124,4,false},
	{125,4,false},
};

SDL_Point mountainRoute[3];
int actualMountainIndex = 0;
bool mountainPoints = false;

//generates enemy of the especified type (it is used on phase 0)
void generateEnemies(int type, int up) {
	extern SDL_Rect shipRect[];
	extern int SCREEN_WIDTH;
	extern int gameHeight;
	extern int indexLineEnemy;
	
	Enemy enemy;
	enemy.schedule = type;
	enemy.rect = { 0 ,0,shipRect[0].h,shipRect[0].h };
	enemy.red = false;
	enemy.life = 1;
	switch (type) {
	case 0:
		//they will appear up if up is 1, down in other case
		enemiesType0Appeared++;
		enemy.id = totalEnemies;
		enemy.rect.x = SCREEN_WIDTH;
		enemy.speedX = SCREEN_WIDTH / 144;
		enemy.limit.x = indexLineEnemy; // it is the line of this enemy
		if (up)
			enemy.rect.y = (int)(gameHeight * 0.15);
		else {
			enemy.rect.y = (int)(gameHeight * 0.85) - enemy.rect.h;
		}
		addToEndEnemy(enemies, totalEnemies, enemy);
		if (enemiesType0Appeared >= 4) {
			extern int totalLineEnemies[];
			generatingEnemies = false;
			lineTimer = 0;
			enemiesType0Appeared = 0;
			if (indexLineEnemy >= 3)
				indexLineEnemy = 0;
			else
				indexLineEnemy++;
			totalLineEnemies[indexLineEnemy] = 4;
		}
		break;
	case 1:
		//they will move initial to up if up is 1 and down in other case
		enemy.rect.x = SCREEN_WIDTH;
		enemy.speedX = SCREEN_WIDTH / 360;
		enemy.speedY = SCREEN_WIDTH / 360;
		enemy.up = up;

		//adding 3 enemies
		enemy.id = totalEnemies;
		enemy.limit = { gameHeight / 5, gameHeight / 2 };
		enemy.rect.y = (int)(gameHeight * 0.3) - enemy.rect.h / 2;
		addToEndEnemy(enemies, totalEnemies, enemy);

		enemy.id = totalEnemies;
		enemy.limit = { (int)(gameHeight * 0.35), (int)(gameHeight * 0.65) };
		enemy.rect.y = gameHeight / 2 - enemy.rect.h / 2;
		addToEndEnemy(enemies, totalEnemies, enemy);

		enemy.id = totalEnemies;
		enemy.limit = { gameHeight / 2, (int)(gameHeight * 0.8) };
		enemy.rect.y = (int)(gameHeight * 0.7) - enemy.rect.h / 2;
		addToEndEnemy(enemies, totalEnemies, enemy);
		break;
	case 2:
		//they will appear up if up is 1, down in other case
		enemy.speedX = SCREEN_WIDTH / 240;
		enemy.speedY = SCREEN_WIDTH / 240;
		enemy.up = up;
		enemy.rect.x = SCREEN_WIDTH;

		if (up) {
			enemy.id = totalEnemies;
			enemy.rect.y = (int)(0.3 * gameHeight) - enemy.rect.h / 2;
			enemy.limit = {gameHeight / 5, (int)(0.4 * gameHeight)};
			enemy.red = true;
			addToEndEnemy(enemies, totalEnemies, enemy);

			enemy.id = totalEnemies;
			enemy.rect.y = (int)(0.6 * gameHeight) - enemy.rect.h / 2;
			enemy.limit = { gameHeight / 2, (int)(0.7 * gameHeight) };
			enemy.red = true;
			addToEndEnemy(enemies, totalEnemies, enemy);
		}
		else {
			enemy.id = totalEnemies;
			enemy.rect.y = (int)(0.4 * gameHeight) - enemy.rect.h / 2;
			enemy.limit = { (int)(0.3 * gameHeight), gameHeight / 2 };
			addToEndEnemy(enemies, totalEnemies, enemy);

			enemy.id = totalEnemies;
			enemy.rect.y = (int)(0.7 * gameHeight) - enemy.rect.h / 2;
			enemy.limit = { (int)(0.6 * gameHeight), (int)(0.8 * gameHeight) };
			addToEndEnemy(enemies, totalEnemies, enemy);
		}
		break;
	}
}

//generate a type 8 enemy on the mountain specified by first = 0, second = 1
void generateEnemy8(int first) {
	extern int SCREEN_WIDTH;
	extern SDL_Rect shipRect[];
	extern int mountainY;
	extern int mountainX[];
	Enemy enemy;
	enemy.id = totalEnemies;
	enemy.life = 1;
	enemy.schedule = 8;
	enemy.rect.w = shipRect[0].h / 2;
	enemy.rect.h = enemy.rect.w;
	enemy.rect.x = mountainX[first];
	enemy.rect.y = (int)(mountainY - enemy.rect.h / 2);
	int height = SCREEN_WIDTH / 120;
	int length = SCREEN_WIDTH / 360;
	if (rand() < RAND_MAX / 2)
		enemy.speedX = - (rand() % height) - length;
	else
		enemy.speedX = rand() % height + length;
	extern int squareHeight;
	enemy.speedY = rand() % (squareHeight * 7) + squareHeight * 3;// initial length Y of sin movement, only in !stop, in stop is normal speedY

	enemy.stop = false;// true: sin(), false: moving to down
	enemy.total = first;//0: first mountain, 1: second mountain
	enemy.red = false;
	addToEndEnemy(enemies, totalEnemies, enemy);
}

//generate mountain enemies every 200 micro seconds
void level1Phase2() {
	extern int enemy8Timer;
	extern int MS;
	enemy8Timer += MS;
	if (enemy8Timer >= 200) {
		generateEnemy8(0); //first mountain
		generateEnemy8(1); //second mountain
		enemy8Timer = 0;
		extern int generatedEnemies8;
		generatedEnemies8++;
		if (generatedEnemies8 > 40) {
			phase++;
		}
	}
}

//add the first enemy of phase 1
void addFirstElementPhase1() {
	phase++;
	actualRound = 0;
	extern int SCREEN_WIDTH;
	extern int downColisions[];
	extern int upColisions[];
	extern int squareWidth;
	extern int squareHeight;

	Square element = { totalDownSquares,{SCREEN_WIDTH,(int)(14 * squareHeight),squareWidth,squareHeight}, downColisions[0] };
	addToEndSquare(downWindowSquares, totalDownSquares, element);

	element = { totalUpSquares,{SCREEN_WIDTH,0,squareWidth,squareHeight}, upColisions[0] };
	addToEndSquare(upWindowSquares, totalUpSquares, element);

	extern CenterSquare centerColisions[];
	extern int progress;
	if (progress + 1 == centerColisions[actualCenterColision].progress) {
		for (int i = 0; i < centerColisions[actualCenterColision].totalElements; i++) {
			centerColisions[actualCenterColision].rect[i] = { SCREEN_WIDTH, (int)(centerColisions[actualCenterColision].positionY[i] * squareHeight),squareWidth,squareHeight };
		}
		addToEndCenterSquare(centerWindowSquares, totalCenterSquares, centerColisions[actualCenterColision]);
		actualCenterColision++;
	}

	//add the correspondient enemy if it are on progress 0
	extern Enemy progressEnemiesLevel1[];
	extern int actualProgressEnemy;
	if (progressEnemiesLevel1[actualProgressEnemy].progress == progress + 1) {
		generateEnemy(progressEnemiesLevel1[actualProgressEnemy]);
		actualProgressEnemy++;
	}

	//pause music 0 and play music 1
	extern Mix_Music* music[];
	Mix_PauseMusic();
	Mix_PlayMusic(music[1], -1);
}

//increments next round timer to appear an enemy on phase 0, and when the actual round reaches the last round, increments the phase and adds the first enemy of phase 1
void level1Phase0() {
	extern int MS;
	roundTimer += MS;
	if (roundTimer >= roundTimes[actualRound]) {
		roundTimer = 0;
		generateEnemies(enemyRound[actualRound].x, enemyRound[actualRound].y);

		if (enemyRound[actualRound].x == 0) {
			generatingEnemies = true;
		}

		actualRound++;
		if (actualRound >= lastRound) {
			addFirstElementPhase1();
		}
	}
}


