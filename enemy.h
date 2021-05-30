#pragma once
#include <SDL.h>

struct Enemy {
	int progress; // progress to appear the enemy
	int schedule;
	bool up;
	int total; // if enemy has schedule 3, indicate how many are in line, if enemy has schedule 5, and are 1 up and 1 down, total = 2
	bool red; // it indicates if the enemy drops an update to the ship
	SDL_Point limit; //.x is limit up, .y is limit down
	SDL_Rect rect;
	int id;
	int speedX;
	int speedY;
	bool top; // indicates if the enemy reach the top of a mountain
	bool climbing;
	bool stop;
	int shootTimer;
	int sprite;
	/*
	* sprites of type 3 enemy:
	0: left,1:diagonal left up, 2: left up
	3: right up, 4: diagonal right up, 5: right
	6: diagonal right down, 7: right down
	8: left down, 9: diagonal left down

	sprites of type 5 enemy:
	0: up and left
	1: up and right
	2: down and left
	3: down and up
	*/
	int life;
};

struct EnemyBullet {
	int id;
	SDL_Rect rect;
	int speedX;
	int speedY;
};
void addToEndEnemy(Enemy array[], int& index, Enemy element);
void deleteOfEnemies(Enemy array[], int& index, int id);
void addToEndEnemyBullet(EnemyBullet array[], int& index, EnemyBullet element);
void deleteOfEnemyBullet(EnemyBullet array[], int& index, int id);
void generateEnemy(Enemy e);
void generateEnemy9();
void generateEnemy7(int i);
void nextEnemy(int i);
extern Enemy enemies[];
extern int totalEnemies;
extern EnemyBullet enemyBullets[];
extern int totalEnemyBullets;