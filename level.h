#pragma once
extern int level;
extern int phase;
extern int actualRound;
extern int roundTimes[];
extern int roundTimer;
extern int lastRound;
extern int roundTimes[];
extern bool generatingEnemies;
extern int lineTimer;
extern int enemiesType0Appeared;
extern int timeAppear;
extern SDL_Point enemyRound[];

void level1Phase0();
void level1Phase2();
void generateEnemies(int type, int up);