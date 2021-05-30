#pragma once
#include <SDL.h>
struct Square {
	int id;
	SDL_Rect rect;
	int colisionType;
};

struct CenterSquare {
	int progress;
	int typeSquare[6];
	int positionY[6];
	int totalElements;
	SDL_Rect rect[6];
};

void addToEndSquare(Square array[], int& index, Square element);
void deleteOfSquares(Square array[], int& index, int id);
void addToEndCenterSquare(CenterSquare array[], int& index, CenterSquare element);
void deleteOfCenterSquares(CenterSquare array[], int& index, int id);
extern Square downWindowSquares[];
extern int totalDownSquares;
extern Square upWindowSquares[];
extern int totalUpSquares;
extern CenterSquare centerWindowSquares[];
extern int totalCenterSquares;
extern int actualCenterColision;
extern Square destroyedEnemy[];
extern int totalDestroyedEnemy;