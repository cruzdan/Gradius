#include "enemy.h"
#include <iostream>


void generateEnemy9() {
	extern int SCREEN_WIDTH;
	extern int SCREEN_HEIGHT;
	Enemy enemy;
	enemy.id = totalEnemies;
	enemy.life = 300;
	enemy.rect.w = SCREEN_WIDTH / 6;
	enemy.rect.h = enemy.rect.w;
	enemy.rect.x = (int)(SCREEN_WIDTH * 0.8 - enemy.rect.w / 2);
	enemy.rect.y = (int)(SCREEN_HEIGHT / 2 - enemy.rect.h / 2);
	enemy.red = false;
	enemy.schedule = 9;
	enemy.speedY = SCREEN_WIDTH / 240;
	enemy.shootTimer = 0;
	extern int players;
	if (players > 1) {
		enemy.top = false;
	}
	addToEndEnemy(enemies, totalEnemies, enemy);
}

void generateEnemy7(int i) {
	extern SDL_Rect shipRect[];
	extern int SCREEN_WIDTH;
	Enemy enemy;
	enemy.id = totalEnemies;
	enemy.limit.y = rand() % 4 + 5; // indicates the coordinate in y to start to move to left
	enemy.rect.w = shipRect[0].h;
	enemy.rect.h = shipRect[0].h;
	enemy.rect.x = enemies[i].rect.x + enemies[i].rect.w / 2 - enemy.rect.w / 2;
	enemy.rect.y = enemies[i].rect.y - enemy.rect.h / 2;
	enemy.schedule = 7;
	enemy.speedY = SCREEN_WIDTH / 360;
	enemy.speedX = SCREEN_WIDTH / 360;
	enemy.up = enemies[i].up;
	enemy.top = false; // it indicates if the enemy has reached its coordinate in y
	enemy.shootTimer = 0;
	enemy.red = false;
	addToEndEnemy(enemies, totalEnemies, enemy);
}

//every 0.5 seconds this method generates a type 7 enemy
void nextEnemy(int i) {
	extern int MS;
	enemies[i].shootTimer += MS;
	if (enemies[i].shootTimer >= 500) {
		generateEnemy7(i);
		enemies[i].shootTimer = 0;
	}
}

//add an enemy with its correspondient type in enemies array, this method is used with progress enemies
void generateEnemy(Enemy e) {
	extern SDL_Rect shipRect[];
	extern int SCREEN_WIDTH;
	extern int gameHeight;
	Enemy enemy;
	enemy = e;
	enemy.rect = { SCREEN_WIDTH ,0,shipRect[0].h,shipRect[0].h };
	enemy.life = 1;
	extern int squareHeight;
	extern int squareWidth;
	switch (e.schedule) {
	case 0:
		//they will appear up if up is 1, down in other case
		extern int enemiesType0Appeared;
		extern bool generatingEnemies;
		extern int lineTimer;
		enemiesType0Appeared++;
		enemy.id = totalEnemies;
		enemy.speedX = SCREEN_WIDTH / 144;
		if (e.up)
			enemy.rect.y = (int)(gameHeight * 0.15);
		else {
			enemy.rect.y = (int)(gameHeight * 0.85) - enemy.rect.h;
		}
		addToEndEnemy(enemies, totalEnemies, enemy);
		if (enemiesType0Appeared >= 4) {
			generatingEnemies = false;
			lineTimer = 0;
			enemiesType0Appeared = 0;
		}
		break;
	case 1:
		//they will move initial to up if up is 1 and down in other case
		enemy.speedX = SCREEN_WIDTH / 360;
		enemy.speedY = SCREEN_WIDTH / 360;

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

		if (e.up) {
			enemy.id = totalEnemies;
			enemy.rect.y = (int)(0.3 * gameHeight) - enemy.rect.h / 2;
			enemy.limit = { gameHeight / 5, (int)(0.4 * gameHeight) };
			addToEndEnemy(enemies, totalEnemies, enemy);

			enemy.id = totalEnemies;
			enemy.rect.y = (int)(0.6 * gameHeight) - enemy.rect.h / 2;
			enemy.limit = { gameHeight / 2, (int)(0.7 * gameHeight) };
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
	case 3:
		if (e.up)
			enemy.rect.y = squareHeight;
		else
			enemy.rect.y = (int)(13 * squareHeight);

		for (int i = 0; i < e.total; i++) {
			enemy.id = totalEnemies;
			enemy.rect.x += i * squareWidth;
			addToEndEnemy(enemies, totalEnemies, enemy);
		}
		break;
	case 4:
		enemy.id = totalEnemies;
		enemy.speedX = SCREEN_WIDTH / 180;
		enemy.rect.y = (int)(11 * squareHeight);
		enemy.limit = {(int)(9 * squareHeight), (int)(13 * squareHeight) };
		addToEndEnemy(enemies, totalEnemies, enemy);
		break;
	case 5:
		enemy.id = totalEnemies;
		enemy.speedX = SCREEN_WIDTH / 240;
		enemy.speedY = SCREEN_WIDTH / 240;
		enemy.rect.x = -(int)(0.5 * squareWidth);
		enemy.limit.x = 0; // indicates the times shooted of the enemy

		if(e.up)
			enemy.rect.y = (int)(squareHeight);
		else
			enemy.rect.y = (int)(13 * squareHeight);
		addToEndEnemy(enemies, totalEnemies, enemy);

		if (e.total > 1) {
			enemy.up = !e.up;
			if (enemy.up)
				enemy.rect.y = (int)(squareHeight);
			else
				enemy.rect.y = (int)(13 * squareHeight);
			addToEndEnemy(enemies, totalEnemies, enemy);
		}
		
		break;
	case 6:
		enemy.id = totalEnemies;
		enemy.life = 3;
		if (e.up)
			enemy.rect.y = squareHeight;
		else
			enemy.rect.y = (int)(13 * squareHeight);
		enemy.rect.w = enemy.rect.w * 2;
		addToEndEnemy(enemies, totalEnemies, enemy);
		break;
	}
}

//add an Enemy element to an Enemy arrary on the "index" of the array and the index is increased
void addToEndEnemy(Enemy array[], int& index, Enemy element) {
	array[index] = element;
	index++;
}

//delete a Missile element with the correspondient id, reorganize the elements and the index is decremented
void deleteOfEnemies(Enemy array[], int& index, int id) {
	for (int i = 0; i < index; i++) {
		if (array[i].id == id) {
			for (int j = i + 1; j < index; j++) {
				array[j - 1] = array[j];
				array[j - 1].id = j - 1;
			}
			index--;
			return;
		}
	}
}

//add an EnemyBullet element to an EnemyBullet arrary on the "index" of the array and the index is increased
void addToEndEnemyBullet(EnemyBullet array[], int& index, EnemyBullet element) {
	array[index] = element;
	index++;
}

//delete a EnemyBullet element with the correspondient id, reorganize the elements and the index is decremented
void deleteOfEnemyBullet(EnemyBullet array[], int& index, int id) {
	for (int i = 0; i < index; i++) {
		if (array[i].id == id) {
			for (int j = i + 1; j < index; j++) {
				array[j - 1] = array[j];
				array[j - 1].id = j - 1;
			}
			index--;
			return;
		}
	}
}