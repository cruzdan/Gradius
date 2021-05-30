#pragma once
#include <SDL.h>

//this structure is used for bullets
struct ActiveRect {
	int id;
	SDL_Rect rect;
};

struct Missile {
	int id;
	SDL_Rect rect;
	int movement;
	int speed;
};
extern ActiveRect upgradeRects[];
extern int totalUpgradeRects;
extern Missile missiles[2][3];
extern int totalMissiles[2];
extern ActiveRect bullets[2][10];
extern int totalBullets[];
extern int bulletSpeed[];
extern int bulletTimer[];
extern bool shoot[];
extern ActiveRect doubleBullets[2][10];
extern int totalDoubleBullets[];
extern ActiveRect optionBullets[2][2][10];
extern int totalOptionBullets[2][2];
extern ActiveRect doubleOptionBullets[2][2][10];
extern int totalDoubleOptionBullets[2][2];
void addToEndActiveRect(ActiveRect array[], int& index, ActiveRect element);
void deleteOfActiveRect(ActiveRect array[], int& index, int id);
void deleteOfMissile(Missile array[], int& index, int id);
void addToEndMissile(Missile array[], int& index, Missile element);