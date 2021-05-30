#include <SDL.h>
#include "enemy.h"
#include "square.h"
#include "activeRect.h"

extern SDL_Renderer* renderer;

//show the ship on menu 0 to chose the numbers of players
void showShipMenu0() {
	extern SDL_Texture* shipTexture;
	extern SDL_Rect shipRect[];
	SDL_Rect rect = { 0,0,32,20 };
	SDL_RenderCopy(renderer, shipTexture, &rect, &shipRect[0]);
}

void showMenu0() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);
	extern int phase;
	extern SDL_Texture* imageMenuTexture;
	extern SDL_Rect imageMenuRect;
	SDL_RenderCopy(renderer, imageMenuTexture, NULL, &imageMenuRect);
	if (phase == 1) {
		//the menu is still
		showShipMenu0();
	}
	SDL_RenderPresent(renderer);
}

void showDestroyedEnemies() {
	SDL_Rect rect;
	extern SDL_Texture* enemyTexture;
	for (int i = 0; i < totalDestroyedEnemy; i++) {
		switch (destroyedEnemy[i].colisionType) {
		case 0:
			rect = { 4,112,16,16 };
			break;
		case 1:
			rect = { 20,112,16,16 };
			break;
		case 2:
			rect = { 36,112,16,16 };
			break;
		}
		SDL_RenderCopy(renderer, enemyTexture, &rect, &destroyedEnemy[i].rect);
	}
}

void showEnemyBullets() {
	extern int phase;
	if (phase != 4) {
		extern SDL_Texture* enemyTexture;
		SDL_Rect rect = { 292,31,5,5 };
		for (int i = 0; i < totalEnemyBullets; i++) {
			SDL_RenderCopy(renderer, enemyTexture, &rect, &enemyBullets[i].rect);
		}
	}
	else {
		//show bullets of enemy 9 (boss)
		extern SDL_Texture* enemy2Texture;
		SDL_Rect rect = { 79,48,16,4 };
		for (int i = 0; i < totalEnemyBullets; i++) {
			SDL_RenderCopy(renderer, enemy2Texture, &rect, &enemyBullets[i].rect);
		}
	}
}

//show the sprites of the upgrade balls (red)
void showUpgrades() {
	extern int sprites3;
	extern SDL_Texture* shipTexture;
	SDL_Rect rect;
	for (int i = 0; i < totalUpgradeRects; i++) {
		switch (sprites3) {
		case 0:
			rect = { 7,26,16,14 };
			break;
		case 1:
			rect = { 27,26,16,14 };
			break;
		case 2:
			rect = { 47,26,16,14 };
			break;
		}
		SDL_RenderCopy(renderer, shipTexture, &rect, &upgradeRects[i].rect);
	}
}

void showEnemies() {
	extern SDL_Texture* enemyTexture;
	extern SDL_Texture* enemy2Texture;
	extern SDL_Texture* enemyReverseTexture;
	extern SDL_Texture* enemyReverse2Texture;
	extern int sprites3;
	extern int sprites4;
	SDL_Rect rect;
	for (int i = 0; i < totalEnemies; i++) {
		switch (enemies[i].schedule) {
		case 0:
			switch (sprites3) {
			case 0:
				rect = { 6,5,13,13 };
				break;
			case 1:
				rect = { 27,6,13,13 };
				break;
			case 2:
				rect = { 49,6,13,13 };
				break;
			}
			SDL_RenderCopy(renderer, enemyTexture, &rect, &enemies[i].rect);
			break;
		case 1:
			if (enemies[i].red) {
				if(enemies[i].up)
					rect = { 102,27,16,16 };
				else
					rect = { 63,27,16,16 };
				SDL_RenderCopy(renderer, enemyTexture, &rect, &enemies[i].rect);
			}
			else {
				if (enemies[i].up)
					rect = { 44,27,16,16 };
				else
					rect = { 5,27,16,16 };
				SDL_RenderCopy(renderer, enemyTexture, &rect, &enemies[i].rect);
			}
			break;
		case 2:
			if (enemies[i].red) {
				switch (sprites4) {
				case 0:
					rect = { 89,47,16,16 };
					break;
				case 1:
					rect = { 109,47,16,16 };
					break;
				case 2:
					rect = { 130,47,16,16 };
					break;
				case 3:
					rect = { 151,47,16,16 };
					break;
				}
				SDL_RenderCopy(renderer, enemyTexture, &rect, &enemies[i].rect);
			}
			else {
				switch (sprites4) {
				case 0:
					rect = { 7,47,16,16 };
					break;
				case 1:
					rect = { 27,47,16,16 };
					break;
				case 2:
					rect = { 48,47,16,16 };
					break;
				case 3:
					rect = { 69,47,16,16 };
					break;
				}
				SDL_RenderCopy(renderer, enemyTexture, &rect, &enemies[i].rect);
			}
			break;
		case 3:
			if (enemies[i].red) {
				switch (enemies[i].sprite) {
				case 0:
					rect = { 234,115,16,16 };
					SDL_RenderCopyEx(renderer, enemyReverseTexture, &rect, &enemies[i].rect, 180, NULL, SDL_FLIP_NONE);
					break;
				case 1:
					rect = { 198,115,16,16 };
					SDL_RenderCopyEx(renderer, enemyReverseTexture, &rect, &enemies[i].rect, 180, NULL, SDL_FLIP_NONE);
					break;
				case 2:
					rect = { 216,115,16,16 };
					SDL_RenderCopyEx(renderer, enemyReverseTexture, &rect, &enemies[i].rect, 180, NULL, SDL_FLIP_NONE);
					break;
				case 3:
					rect = { 216,4,16,16 };
					SDL_RenderCopy(renderer, enemyTexture, &rect, &enemies[i].rect);
					break;
				case 4:
					rect = { 198,4,16,16 };
					SDL_RenderCopy(renderer, enemyTexture, &rect, &enemies[i].rect);
					break;
				case 5:
					rect = { 234,4,16,16 };
					SDL_RenderCopy(renderer, enemyTexture, &rect, &enemies[i].rect);
					break;
				case 6:
					rect = { 198,115,16,16 };
					SDL_RenderCopy(renderer, enemyReverseTexture, &rect, &enemies[i].rect);
					break;
				case 7:
					rect = { 216,115,16,16 };
					SDL_RenderCopy(renderer, enemyReverseTexture, &rect, &enemies[i].rect);
					break;
				case 8:
					rect = { 216,4,16,16 };
					SDL_RenderCopyEx(renderer, enemyTexture, &rect, &enemies[i].rect, 180, NULL, SDL_FLIP_NONE);
					break;
				case 9:
					rect = { 198,4,16,16 };
					SDL_RenderCopyEx(renderer, enemyTexture, &rect, &enemies[i].rect, 180, NULL, SDL_FLIP_NONE);
					break;
				case 10:
					rect = { 234,4,16,16 };
					SDL_RenderCopyEx(renderer, enemyTexture, &rect, &enemies[i].rect, 180, NULL, SDL_FLIP_NONE);
					break;
				case 11:
					rect = { 234,115,16,16 };
					SDL_RenderCopy(renderer, enemyReverseTexture, &rect, &enemies[i].rect);
					break;
				}
			}
			else {
				switch (enemies[i].sprite) {
				case 0:
					rect = { 177,115,16,16 };
					SDL_RenderCopyEx(renderer, enemyReverseTexture, &rect, &enemies[i].rect, 180, NULL, SDL_FLIP_NONE);
					break;
				case 1:
					rect = { 145,115,16,16 };
					SDL_RenderCopyEx(renderer, enemyReverseTexture, &rect, &enemies[i].rect, 180, NULL, SDL_FLIP_NONE);
					break;
				case 2:
					rect = { 161,115,16,16 };
					SDL_RenderCopyEx(renderer, enemyReverseTexture, &rect, &enemies[i].rect, 180, NULL, SDL_FLIP_NONE);
					break;
				case 3:
					rect = { 161,4,16,16 };
					SDL_RenderCopy(renderer, enemyTexture, &rect, &enemies[i].rect);
					break;
				case 4:
					rect = { 145,4,16,16 };
					SDL_RenderCopy(renderer, enemyTexture, &rect, &enemies[i].rect);
					break;
				case 5:
					rect = { 177,4,16,16 };
					SDL_RenderCopy(renderer, enemyTexture, &rect, &enemies[i].rect);
					break;
				case 6:
					rect = { 145,115,16,16 };
					SDL_RenderCopy(renderer, enemyReverseTexture, &rect, &enemies[i].rect);
					break;
				case 7:
					rect = { 161,115,16,16 };
					SDL_RenderCopy(renderer, enemyReverseTexture, &rect, &enemies[i].rect);
					break;
				case 8:
					rect = { 161,4,16,16 };
					SDL_RenderCopyEx(renderer, enemyTexture, &rect, &enemies[i].rect, 180, NULL, SDL_FLIP_NONE);
					break;
				case 9:
					rect = { 145,4,16,16 };
					SDL_RenderCopyEx(renderer, enemyTexture, &rect, &enemies[i].rect, 180, NULL, SDL_FLIP_NONE);
					break;
				case 10:
					rect = { 177,4,16,16 };
					SDL_RenderCopyEx(renderer, enemyTexture, &rect, &enemies[i].rect, 180, NULL, SDL_FLIP_NONE);
					break;
				case 11:
					rect = { 177,115,16,16 };
					SDL_RenderCopy(renderer, enemyReverseTexture, &rect, &enemies[i].rect);
					break;
				}
			}
			break;
		case 4:
			if (enemies[i].red) {
				switch (sprites4) {
				case 0:
					rect = { 253,45,16,16 };
					break;
				case 1:
					rect = { 272,45,16,16 };
					break;
				case 2:
					rect = { 294,45,16,16 };
					break;
				case 3:
					rect = { 311,45,16,16 };
					break;
				}
				if (enemies[i].up)
					SDL_RenderCopyEx(renderer, enemyTexture, &rect, &enemies[i].rect, 180, NULL, SDL_FLIP_NONE);
				else
					SDL_RenderCopy(renderer, enemyTexture, &rect, &enemies[i].rect);
			}
			else {
				switch (sprites4) {
				case 0:
					rect = { 173,45,16,16 };
					break;
				case 1:
					rect = { 192,45,16,16 };
					break;
				case 2:
					rect = { 214,45,16,16 };
					break;
				case 3:
					rect = { 231,45,16,16 };
					break;
				}
				if (enemies[i].up)
					SDL_RenderCopyEx(renderer, enemyTexture, &rect, &enemies[i].rect, 180, NULL, SDL_FLIP_NONE);
				else
					SDL_RenderCopy(renderer, enemyTexture, &rect, &enemies[i].rect);
			}
			break;
		case 5:
			if (!enemies[i].stop) {
				if (enemies[i].up) {
					if (enemies[i].red) {
						switch (sprites4 % 2) {
						case 0:
							rect = { 180,92,16,16 };
							break;
						case 1:
							rect = { 198,92,16,16 };
							break;
						}
						SDL_RenderCopy(renderer, enemyReverseTexture, &rect, &enemies[i].rect);
					}
					else {
						switch (sprites4 % 2) {
						case 0:
							rect = { 122,92,16,16 };
							break;
						case 1:
							rect = { 140,92,16,16 };
							break;
						}
						SDL_RenderCopy(renderer, enemyReverseTexture, &rect, &enemies[i].rect);
					}
				}
				else {
					if (enemies[i].red) {
						switch (sprites4 % 2) {
						case 0:
							rect = { 180,27,16,16 };
							break;
						case 1:
							rect = { 198,27,16,16 };
							break;
						}
						SDL_RenderCopy(renderer, enemyTexture, &rect, &enemies[i].rect);
					}
					else {
						switch (sprites4 % 2) {
						case 0:
							rect = { 122,27,16,16 };
							break;
						case 1:
							rect = { 140,27,16,16 };
							break;
						}
						SDL_RenderCopy(renderer, enemyTexture, &rect, &enemies[i].rect);
					}
				}
			}
			else {
				if (enemies[i].red) {
					switch (enemies[i].sprite) {
					case 0:
						rect = { 217,92,16,16 };
						SDL_RenderCopyEx(renderer, enemyReverseTexture, &rect, &enemies[i].rect, 180, NULL, SDL_FLIP_NONE);
						break;
					case 1:
						rect = { 217,26,16,16 };
						SDL_RenderCopy(renderer, enemyTexture, &rect, &enemies[i].rect);
						break;
					case 2:
						rect = { 217,26,16,16 };
						SDL_RenderCopyEx(renderer, enemyTexture, &rect, &enemies[i].rect, 180, NULL, SDL_FLIP_NONE);
						break;
					case 3:
						rect = { 217,92,16,16 };
						SDL_RenderCopy(renderer, enemyReverseTexture, &rect, &enemies[i].rect);
						break;
					}
				}
				else {
					switch (enemies[i].sprite) {
					case 0:
						rect = { 159,92,16,16 };
						SDL_RenderCopyEx(renderer, enemyReverseTexture, &rect, &enemies[i].rect, 180, NULL, SDL_FLIP_NONE);
						break;
					case 1:
						rect = { 159,26,16,16 };
						SDL_RenderCopy(renderer, enemyTexture, &rect, &enemies[i].rect);
						break;
					case 2:
						rect = { 159,26,16,16 };
						SDL_RenderCopyEx(renderer, enemyTexture, &rect, &enemies[i].rect, 180, NULL, SDL_FLIP_NONE);
						break;
					case 3:
						rect = { 159,92,16,16 };
						SDL_RenderCopy(renderer, enemyReverseTexture, &rect, &enemies[i].rect);
						break;
					}
				}
			}
			break;
		case 6:
			if (enemies[i].up) {
				if (enemies[i].red) {
					rect = { 49,362,32,26 };
					SDL_RenderCopy(renderer, enemyReverse2Texture, &rect, &enemies[i].rect);
				}
				else {
					rect = { 10,362,32,26 };
					SDL_RenderCopy(renderer, enemyReverse2Texture, &rect, &enemies[i].rect);
				}
			}
			else {
				if (enemies[i].red) {
					rect = { 49,8,32,26 };
					SDL_RenderCopy(renderer, enemy2Texture, &rect, &enemies[i].rect);
				}
				else {
					rect = { 10,8,32,26 };
					SDL_RenderCopy(renderer, enemy2Texture, &rect, &enemies[i].rect);
				}
			}
			break;
		case 7:
			switch (sprites4) {
			case 0:
				rect = { 68,5,16,16 };
				break;
			case 1:
				rect = { 86,5,16,16 };
				break;
			case 2:
				rect = { 105,5,16,16 };
				break;
			case 3:
				rect = { 123,5,16,16 };
				break;
			}
			if (enemies[i].up)
				SDL_RenderCopyEx(renderer, enemyTexture, &rect, &enemies[i].rect, 180, NULL, SDL_FLIP_NONE);
			else
				SDL_RenderCopy(renderer, enemyTexture, &rect, &enemies[i].rect);
			break;
		case 8:
			rect = { 278,29,10,9 };
			SDL_RenderCopy(renderer, enemyTexture, &rect, &enemies[i].rect);
			break;
		case 9:
			switch (sprites4 % 2) {
			case 0:
				rect = { 11,44,48,48 };
				break;
			case 1:
				rect = { 128,44,48,48 };
				break;
			}
			SDL_RenderCopy(renderer, enemy2Texture, &rect, &enemies[i].rect);
			break;
		}
	}
}

void showShield(int shipIndex) {
	extern bool shield[];
	if (shield[shipIndex]) {
		extern SDL_Texture* shipTexture;
		extern SDL_Rect shieldRect[];
		extern int shieldLife[];
		extern int sprites4;
		SDL_Rect rect;
		switch (sprites4) {
		case 0:
			rect = { 113,45,8,16 };
			break;
		case 1:
			rect = { 124,45,8,16 };
			break;
		case 2:
			rect = { 134,45,8,16 };
			break;
		case 3:
			rect = { 144,45,8,16 };
			break;
		}
		if (shieldLife[shipIndex] < 2)
			rect.x += 43;
		SDL_RenderCopy(renderer, shipTexture, &rect, &shieldRect[shipIndex]);
	}
}

void showMissiles(int shipIndex) {
	extern SDL_Texture* shipTexture;
	SDL_Rect rect = { 21,53,7,3 };
	for (int i = 0; i < totalMissiles[shipIndex]; i++) {
		if(missiles[shipIndex][i].movement == 0)
			SDL_RenderCopy(renderer, shipTexture, &rect, &missiles[shipIndex][i].rect);
		else
			SDL_RenderCopyEx(renderer, shipTexture, &rect, &missiles[shipIndex][i].rect, 45, NULL, SDL_FLIP_NONE);
	}
}

void showDoubleBullets(int shipIndex) {
	extern SDL_Texture* shipTexture;
	SDL_Rect rect = { 47,51,6,6 };
	for (int i = 0; i < totalDoubleBullets[shipIndex]; i++) {
		SDL_RenderCopy(renderer, shipTexture, &rect, &doubleBullets[shipIndex][i].rect);
	}
	extern int option[];
	for (int j = 0; j < option[shipIndex]; j++) {
		for (int i = 0; i < totalDoubleOptionBullets[shipIndex][j]; i++) {
			SDL_RenderCopy(renderer, shipTexture, &rect, &doubleOptionBullets[shipIndex][j][i].rect);
		}
	}

}

void showOption(int shipIndex) {
	extern int option[];
	if (option[shipIndex] > 0) {
		extern int sprites4;
		SDL_Rect rect;
		extern SDL_Texture* shipTexture;
		extern SDL_Rect optionRect[2][2];
		switch (sprites4 % 2) {
		case 0:
			rect = { 93,49,16,10 };
			break;
		case 1:
			rect = { 77,49,16,10 };
			break;
		}
		for (int i = 0; i < option[shipIndex]; i++) {
			SDL_RenderCopy(renderer, shipTexture, &rect, &optionRect[shipIndex][i]);
		}
	}
}

//show main bullets and main option bullets of ship[indexShip]
void showBullets(int shipIndex) {
	extern SDL_Texture* shipTexture;
	extern bool laser[];
	extern int option[];
	SDL_Rect rect;
	if (!laser[shipIndex])
		rect = { 34,52,9,4 };
	else
		rect = { 58,52,15,4 };
	for (int i = 0; i < totalBullets[shipIndex]; i++) {
		SDL_RenderCopy(renderer, shipTexture, &rect, &bullets[shipIndex][i].rect);
	}
	for (int j = 0; j < option[shipIndex]; j++) {
		for (int i = 0; i < totalOptionBullets[shipIndex][j]; i++) {
			SDL_RenderCopy(renderer, shipTexture, &rect, &optionBullets[shipIndex][j][i].rect);
		}
	}
	
}

void showCenterMap() {
	SDL_Rect rect;
	extern SDL_Texture* downBackgroundTexture;
	rect.w = 16;
	rect.h = 14;
	for (int i = 0; i < totalCenterSquares; i++) {
		for (int j = 0; j < centerWindowSquares[i].totalElements; j++) {
			switch (centerWindowSquares[i].typeSquare[j]) {
			case 410:
				rect.x = 171;
				rect.y = 101;
				break;
			case 411:
				rect.x = 171;
				rect.y = 87;
				break;
			case 412:
				rect.x = 171;
				rect.y = 73;
				break;
			case 420:
				rect.x = 187;
				rect.y = 101;
				break;
			case 421:
				rect.x = 187;
				rect.y = 87;
				break;
			case 422:
				rect.x = 187;
				rect.y = 73;
				break;
			case 430:
				rect.x = 203;
				rect.y = 101;
				break;
			case 431:
				rect.x = 203;
				rect.y = 87;
				break;
			case 432:
				rect.x = 203;
				rect.y = 73;
				break;
			case 440:
				rect.x = 219;
				rect.y = 101;
				break;
			case 441:
				rect.x = 219;
				rect.y = 87;
				break;
			case 442:
				rect.x = 219;
				rect.y = 73;
				break;
			case 510:
				rect.x = 237;
				rect.y = 101;
				break;
			case 520:
				rect.x = 253;
				rect.y = 101;
				break;
			case 522:
				rect.x = 253;
				rect.y = 73;
				break;
			case 530:
				rect.x = 269;
				rect.y = 101;
				break;
			case 532:
				rect.x = 269;
				rect.y = 73;
				break;
			case 533:
				rect.x = 269;
				rect.y = 59;
				break;
			case 534:
				rect.x = 269;
				rect.y = 45;
				break;
			case 540:
				rect.x = 285;
				rect.y = 101;
				break;
			case 542:
				rect.x = 285;
				rect.y = 73;
				break;
			case 543:
				rect.x = 285;
				rect.y = 59;
				break;
			case 544:
				rect.x = 285;
				rect.y = 45;
				break;
			case 550:
				rect.x = 301;
				rect.y = 101;
				break;
			case 552:
				rect.x = 301;
				rect.y = 73;
				break;
			case 553:
				rect.x = 301;
				rect.y = 59;
				break;
			case 560:
				rect.x = 317;
				rect.y = 101;
				break;
			case 610:
				rect.x = 337;
				rect.y = 100;
				break;
			case 611:
				rect.x = 337;
				rect.y = 86;
				break;
			case 612:
				rect.x = 337;
				rect.y = 72;
				break;
			case 613:
				rect.x = 337;
				rect.y = 58;
				break;
			case 614:
				rect.x = 337;
				rect.y = 44;
				break;
			case 620:
				rect.x = 353;
				rect.y = 100;
				break;
			case 621:
				rect.x = 353;
				rect.y = 86;
				break;
			case 622:
				rect.x = 353;
				rect.y = 72;
				break;
			case 623:
				rect.x = 353;
				rect.y = 58;
				break;
			case 624:
				rect.x = 353;
				rect.y = 44;
				break;
			case 630:
				rect.x = 369;
				rect.y = 100;
				break;
			case 631:
				rect.x = 369;
				rect.y = 86;
				break;
			case 632:
				rect.x = 369;
				rect.y = 72;
				break;
			case 633:
				rect.x = 369;
				rect.y = 58;
				break;
			case 634:
				rect.x = 369;
				rect.y = 44;
				break;
			case 640:
				rect.x = 385;
				rect.y = 100;
				break;
			case 641:
				rect.x = 385;
				rect.y = 86;
				break;
			case 642:
				rect.x = 385;
				rect.y = 72;
				break;
			case 643:
				rect.x = 385;
				rect.y = 58;
				break;
			case 644:
				rect.x = 385;
				rect.y = 44;
				break;
			case 650:
				rect.x = 401;
				rect.y = 100;
				break;
			case 651:
				rect.x = 401;
				rect.y = 86;
				break;
			case 652:
				rect.x = 401;
				rect.y = 72;
				break;
			case 653:
				rect.x = 401;
				rect.y = 58;
				break;
			case 654:
				rect.x = 401;
				rect.y = 44;
				break;
			case 660:
				rect.x = 417;
				rect.y = 100;
				break;
			case 661:
				rect.x = 417;
				rect.y = 86;
				break;
			case 662:
				rect.x = 417;
				rect.y = 72;
				break;
			case 663:
				rect.x = 417;
				rect.y = 58;
				break;
			case 664:
				rect.x = 417;
				rect.y = 44;
				break;
			case 71:
				rect.x = 5;
				rect.y = 98;
				break;
			case 72:
				rect.x = 21;
				rect.y = 98;
				break;
			case 80:
				rect.x = 0;
				rect.y = 83;
				break;
			case 800:
				rect.x = 0;
				rect.y = 69;
				break;
			case 801:
				rect.x = 0;
				rect.y = 55;
				break;
			case 802:
				rect.x = 0;
				rect.y = 41;
				break;
			case 803:
				rect.x = 0;
				rect.y = 27;
				break;
			case 804:
				rect.x = 0;
				rect.y = 13;
				break;
			case 81:
				rect.x = 16;
				rect.y = 83;
				break;
			case 810:
				rect.x = 16;
				rect.y = 69;
				break;
			case 811:
				rect.x = 16;
				rect.y = 55;
				break;
			case 812:
				rect.x = 16;
				rect.y = 41;
				break;
			case 813:
				rect.x = 16;
				rect.y = 27;
				break;
			case 814:
				rect.x = 16;
				rect.y = 13;
				break;
			case 82:
				rect.x = 32;
				rect.y = 83;
				break;
			case 820:
				rect.x = 32;
				rect.y = 69;
				break;
			case 821:
				rect.x = 32;
				rect.y = 55;
				break;
			case 822:
				rect.x = 32;
				rect.y = 41;
				break;
			case 823:
				rect.x = 32;
				rect.y = 27;
				break;
			case 824:
				rect.x = 32;
				rect.y = 13;
				break;
			case 83:
				rect.x = 48;
				rect.y = 83;
				break;
			case 830:
				rect.x = 48;
				rect.y = 69;
				break;
			case 831:
				rect.x = 48;
				rect.y = 55;
				break;
			case 832:
				rect.x = 48;
				rect.y = 41;
				break;
			case 833:
				rect.x = 48;
				rect.y = 27;
				break;
			case 834:
				rect.x = 48;
				rect.y = 13;
				break;
			}
			SDL_RenderCopy(renderer, downBackgroundTexture, &rect, &centerWindowSquares[i].rect[j]);
		}
	}
}

void showUpMap() {
	SDL_Rect rect;
	extern SDL_Texture* upBackgroundTexture;
	rect.w = 16;
	rect.h = 14;
	for (int i = 0; i < totalUpSquares; i++) {
		switch (upWindowSquares[i].colisionType) {
		case 10:
			rect.x = 5;
			rect.y = 348;
			break;
		case 11:
			rect.x = 21;
			rect.y = 348;
			break;
		case 12:
			rect.x = 37;
			rect.y = 348;
			break;
		case 13:
			rect.x = 53;
			rect.y = 348;
			break;
		case 21:
			rect.x = 71;
			rect.y = 348;
			break;
		case 22:
			rect.x = 87;
			rect.y = 348;
			break;
		case 23:
			rect.x = 103;
			rect.y = 348;
			break;
		case 31:
			rect.x = 121;
			rect.y = 348;
			break;
		case 32:
			rect.x = 137;
			rect.y = 348;
			break;
		case 33:
			rect.x = 153;
			rect.y = 348;
			break;
		}
		SDL_RenderCopy(renderer, upBackgroundTexture, &rect, &upWindowSquares[i].rect);
	}
}

void showDownMap() {
	SDL_Rect rect;
	extern SDL_Texture* downBackgroundTexture;
	rect.w = 16;
	rect.h = 14;
	rect.y = 115;
	for (int i = 0; i < totalDownSquares; i++) {
		switch (downWindowSquares[i].colisionType) {
		case 10:
			rect.x = 5;
			break;
		case 11:
			rect.x = 21;
			break;
		case 12:
			rect.x = 37;
			break;
		case 13:
			rect.x = 53;
			break;
		case 21:
			rect.x = 71;
			break;
		case 22:
			rect.x = 87;
			break;
		case 23:
			rect.x = 103;
			break;
		case 41:
			rect.x = 171;
			break;
		case 42:
			rect.x = 187;
			break;
		case 43:
			rect.x = 203;
			break;
		case 44:
			rect.x = 219;
			break;
		case 51:
			rect.x = 237;
			break;
		case 52:
			rect.x = 253;
			break;
		case 53:
			rect.x = 269;
			break;
		case 54:
			rect.x = 285;
			break;
		case 55:
			rect.x = 301;
			break;
		case 56:
			rect.x = 317;
			break;
		case 61:
			rect.x = 337;
			rect.y = 114;
			break;
		case 62:
			rect.x = 353;
			rect.y = 114;
			break;
		case 63:
			rect.x = 369;
			rect.y = 114;
			break;
		case 64:
			rect.x = 385;
			rect.y = 114;
			break;
		case 65:
			rect.x = 401;
			rect.y = 114;
			break;
		case 66:
			rect.x = 417;
			rect.y = 114;
			break;
		}
		SDL_RenderCopy(renderer, downBackgroundTexture, &rect, &downWindowSquares[i].rect);
	}
}

void showMap() {
	extern int phase;
	showDownMap();
	showUpMap();
	showCenterMap();
}

void showBackground() {
	extern int SCREEN_WIDTH;
	extern SDL_Texture* levelOneTexture;
	extern SDL_Rect backgroundImageRect;
	extern int gameHeight;
	SDL_Rect window;
	window = {0,0,SCREEN_WIDTH,gameHeight};
	SDL_RenderCopy(renderer, levelOneTexture, &backgroundImageRect, &window);
}

void showUpdateRects(int shipIndex) {
	int widthImage = 31;
	int heightImage = 6;
	extern SDL_Texture* menuTexture;
	extern int actualUpgrade[];
	extern int limitUpgrades[2][6];
	extern SDL_Rect menuUpgradeRects[2][6];
	SDL_Rect rect;

	//show upgrade rects on menu
	for (int i = 0; i < 6; i++) {
		if (limitUpgrades[shipIndex][i] > 0) {
			rect = { 6 + 32 * i, 82,widthImage,heightImage };
		}
		else {
			rect = { 38,100,widthImage,heightImage };
		}
		SDL_RenderCopy(renderer, menuTexture, &rect, &menuUpgradeRects[shipIndex][i]);
	}

	//show yellow upgrade rect
	if (actualUpgrade[shipIndex] > 0) {
		if (limitUpgrades[shipIndex][actualUpgrade[shipIndex] - 1] > 0) {
			//yellow with text
			switch (actualUpgrade[shipIndex]) {
			case 1:
				rect = { 6,91,31,6 };
				break;
			case 2:
				rect = { 38,91,31,6 };
				break;
			case 3:
				rect = { 70,91,31,6 };
				break;
			case 4:
				rect = { 102,91,31,6 };
				break;
			case 5:
				rect = { 134,91,31,6 };
				break;
			case 6:
				rect = { 166,91,31,6 };
				break;
			}
		}
		else {
			//yellow without text
			rect = { 6,100,31,6 };
		}
		SDL_RenderCopy(renderer, menuTexture, &rect, &menuUpgradeRects[shipIndex][actualUpgrade[shipIndex] - 1]);
	}
}

//show the ship lifes on menu 1
void showLifeMenu1(int shipIndex) {
	extern SDL_Rect lifeRect[];
	extern SDL_Texture* lifeTexture[];
	SDL_RenderCopy(renderer, lifeTexture[shipIndex], NULL, &lifeRect[shipIndex]);
}

//show text of players and score
void showTextMenu1() {
	extern SDL_Texture* menuTextTexture[];
	extern SDL_Rect menuTextRects[];
	for (int i = 0; i < 4; i++) {
		SDL_RenderCopy(renderer, menuTextTexture[i], NULL, &menuTextRects[i]);
	}
}

void showScoreMenuText() {
	extern SDL_Texture* mountainPuntuationTexture;
	extern SDL_Rect mountainPuntuationRect;
	SDL_RenderCopy(renderer, mountainPuntuationTexture, NULL, &mountainPuntuationRect);
}

//show ship in menu 1
void showShip(int shipIndex) {
	extern bool inmunity[];
	extern int sprites4;
	extern SDL_Texture* shipTexture;
	extern SDL_Rect shipRect[];
	extern int direction[];

	SDL_Rect rect;
	int widthImage = 32;

	if (!inmunity[shipIndex] || sprites4 % 2 == 0) {
		rect = { 0,0,widthImage,20 };
		switch (direction[shipIndex]) {
		case 1:
			rect.x = widthImage;
			break;
		case 2:
			rect.x = 2 * widthImage;
			break;
		}
		SDL_RenderCopy(renderer, shipTexture, &rect, &shipRect[shipIndex]);
	}
}


void show() {
	extern int players;
	extern int lifes[];
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);
	showBackground();
	showTextMenu1();
	showEnemies();
	showUpgrades();
	showEnemyBullets();
	showDestroyedEnemies();
	extern int phase;
	if (phase > 0)
		showMap();
	for (int i = 0; i < players; i++) {
		showLifeMenu1(i);
		showUpdateRects(i);
		if (lifes[i] > 0) {
			showShip(i);
			showBullets(i);
			showOption(i);
			showDoubleBullets(i);
			showMissiles(i);
			showShield(i);
		}
	}
	extern bool mountainPoints;
	if (mountainPoints)
		showScoreMenuText();
	SDL_RenderPresent(renderer);
}