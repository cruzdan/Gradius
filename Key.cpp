#include <iostream>
#include <SDL_mixer.h>
#include "activeRect.h"
extern Mix_Chunk* sound[];

void generateMissile(int i, int shipIndex);
void addOption(int i, SDL_Rect ship, int shipSpeed, int shipIndex);
void moveOption(int shipIndex);
void changeTextToTexture(std::string newText, SDL_Texture*& texture);

extern SDL_Rect shipRect[];
extern int shipSpeed[];
extern bool shield[];
extern SDL_Rect shieldRect[];
extern int SCREEN_WIDTH;

//in menu 0, when the image does not reach the final position, move the image to the final position, when 
//the image are in the final position, select the number of choosed players 
void selectPlayers() {
	extern int SCREEN_HEIGHT;
	extern int phase;
	
	if (phase == 0) {
		extern SDL_Rect imageMenuRect;
		phase = 1;
		imageMenuRect.x = 0;
	}
	else {
		extern Mix_Music* music[];
		Mix_PlayMusic(music[0], -1);
		Mix_PlayChannel(2, sound[5], 0);
		extern int players;
		extern int actualMenu;
		phase = 0;
		actualMenu = 1;
		shipRect[0].y = SCREEN_HEIGHT / 2 - shipRect[0].h / 2;
		shipRect[0].x = SCREEN_WIDTH / 4 - shipRect[0].w / 2;
		if (players == 2) {
			extern SDL_Rect menuUpgradeRects[2][6];
			extern SDL_Texture* menuTextTexture[];
			changeTextToTexture("2P", menuTextTexture[0]);
			for (int i = 0; i < 6; i++) {
				menuUpgradeRects[0][i].h = (int)(menuUpgradeRects[0][i].h * 3 / 4);
				menuUpgradeRects[0][i].y -= menuUpgradeRects[0][i].h;
			}
		}
	}
}

//move the ship to 1 players in menu 0
void moveToUpMenu0() {
	extern int players;
	extern int SCREEN_HEIGHT;
	players = 1;
	shipRect[0].y = (int)(SCREEN_HEIGHT * 0.58 - shipRect[0].h / 2);
}

//move the ship to 2 players in menu 0
void moveToDownMenu0() {
	extern int players;
	extern int SCREEN_HEIGHT;
	players = 2;
	shipRect[0].y = (int)(SCREEN_HEIGHT * 0.64 - shipRect[0].h / 2);
}

//detects joystick key in menu 0
void detectKeyJoystickMenu0(SDL_Joystick* joystick) {
	if (SDL_JoystickGetAxis(joystick, 1) == SDL_JOYSTICK_AXIS_MIN) {
		moveToUpMenu0();
	}
	else if (SDL_JoystickGetAxis(joystick, 1) == SDL_JOYSTICK_AXIS_MAX) {
		moveToDownMenu0();
	}
	if (SDL_JoystickGetButton(joystick, 2)) {
		selectPlayers();
	}
}

//detects key on menu 0 
void detectKeyMenu0() {
	extern bool gameOver;
	const Uint8* keys = SDL_GetKeyboardState(NULL);
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		int typeEvent = event.type;
		if (typeEvent == SDL_QUIT) {
			gameOver = true;
		}
		else if (typeEvent == SDL_KEYDOWN) {
			if (keys[SDL_SCANCODE_W] || keys[SDL_SCANCODE_UP]) {
				moveToUpMenu0();
			}
			if (keys[SDL_SCANCODE_S] || keys[SDL_SCANCODE_DOWN]) {
				moveToDownMenu0();
			}
			if (keys[SDL_SCANCODE_SPACE]) {
				selectPlayers();
			}
		}
	}
}

void selectUpgrade(int playerIndex) {
	extern int actualUpgrade[];
	extern int limitUpgrades[2][6];
	if (actualUpgrade[playerIndex] > 0 && limitUpgrades[playerIndex][actualUpgrade[playerIndex] - 1] > 0) {
		switch (actualUpgrade[playerIndex]) {
		case 1:
			shipSpeed[playerIndex] += SCREEN_WIDTH / 360;
			break;
		case 2:
			extern bool missile[];
			extern int option[];
			extern bool shootMissile[2][3];
			missile[playerIndex] = true;
			shootMissile[playerIndex][0] = true;
			switch (option[playerIndex]) {
			case 1:
				shootMissile[playerIndex][1] = true;
				break;
			case 2:
				shootMissile[playerIndex][1] = true;
				shootMissile[playerIndex][2] = true;
				break;
			}
			break;
		case 3:
			extern bool doubleShoot[];
			extern bool laser[];
			bulletSpeed[playerIndex] = SCREEN_WIDTH / 90;
			doubleShoot[playerIndex] = true;
			laser[playerIndex] = false;
			break;
		case 4:
			extern int bulletDamage[];
			bulletDamage[playerIndex] = 2;
			laser[playerIndex] = true;
			bulletSpeed[playerIndex] = SCREEN_WIDTH / 36;
			doubleShoot[playerIndex] = false;
			break;
		case 5:
			addOption(option[playerIndex], shipRect[playerIndex], shipSpeed[playerIndex], playerIndex);
			option[playerIndex]++;
			if (missile[playerIndex])
				shootMissile[playerIndex][option[playerIndex]] = true;
			break;
		case 6:
			extern int shieldLife[];
			shield[playerIndex] = true;
			shieldLife[playerIndex] = 10;
			shieldRect[playerIndex].x = shipRect[playerIndex].x + shipRect[playerIndex].w;
			shieldRect[playerIndex].y = shipRect[playerIndex].y;
			shieldRect[playerIndex].h = shipRect[playerIndex].h;
			shieldRect[playerIndex].w = shipRect[playerIndex].w / 3;
			break;
		}
		limitUpgrades[playerIndex][actualUpgrade[playerIndex] - 1]--;
		actualUpgrade[playerIndex] = 0;
		Mix_PlayChannel(2, sound[4], 0);
	}
}

//pause the game with joysticks
void pauseGame() {
	extern bool canPause;
	if (canPause) {
		extern bool pause;
		pause = !pause;
		canPause = false;
	}
}

//shoot the ship missile and options of ship[playerIndex]
void shootShipMissile(int playerIndex) {
	extern bool missile[];
	if (missile[playerIndex]) {
		extern bool shootMissile[2][3];
		for (int j = 0; j < 3; j++) {
			if (shootMissile[playerIndex][j]) {
				shootMissile[playerIndex][j] = false;
				generateMissile(j, playerIndex);
			}
		}
	}
}

//shoot the normal, double , laser and option bullets of ship[playerIndex]
void shootShip(int playerIndex) {
	
	if (shoot[playerIndex]) {
		extern bool laser[];

		if (totalBullets[playerIndex] < 10) {
			ActiveRect element;
			element.id = totalBullets[playerIndex];
			if (laser[playerIndex])
				element.rect = { shipRect[playerIndex].x + shipRect[playerIndex].w ,shipRect[playerIndex].y + shipRect[playerIndex].h / 2,(int)(3 * shipRect[playerIndex].w / 4),shipRect[playerIndex].h / 5 };
			else
				element.rect = { shipRect[playerIndex].x + shipRect[playerIndex].w ,shipRect[playerIndex].y + shipRect[playerIndex].h / 2,shipRect[playerIndex].w / 3,shipRect[playerIndex].h / 3 };
			addToEndActiveRect(bullets[playerIndex], totalBullets[playerIndex], element);
			shoot[playerIndex] = false;
			Mix_PlayChannel(0, sound[0], 0);
		}
		extern bool doubleShoot[];
		if (doubleShoot[playerIndex] && totalDoubleBullets[playerIndex] < 10) {
			ActiveRect element;
			element.id = totalDoubleBullets[playerIndex];
			element.rect = { shipRect[playerIndex].x + shipRect[playerIndex].w ,shipRect[playerIndex].y + shipRect[playerIndex].h / 2,shipRect[playerIndex].w / 3,shipRect[playerIndex].h / 3 };
			addToEndActiveRect(doubleBullets[playerIndex], totalDoubleBullets[playerIndex], element);
		}
		extern SDL_Rect optionRect[2][2];
		extern int option[2];
		for (int j = 0; j < option[playerIndex]; j++) {
			if (totalOptionBullets[playerIndex][j] < 10) {
				ActiveRect element;
				element.id = totalOptionBullets[playerIndex][j];
				if (laser[playerIndex])
					element.rect = { optionRect[playerIndex][j].x + optionRect[playerIndex][j].w ,optionRect[playerIndex][j].y + optionRect[playerIndex][j].h / 2,(int)(3 * shipRect[playerIndex].w / 4) ,shipRect[playerIndex].h / 5 };
				else
					element.rect = { optionRect[playerIndex][j].x + optionRect[playerIndex][j].w ,optionRect[playerIndex][j].y + optionRect[playerIndex][j].h / 2,shipRect[playerIndex].w / 3,shipRect[playerIndex].h / 3 };
				addToEndActiveRect(optionBullets[playerIndex][j], totalOptionBullets[playerIndex][j], element);
			}
			if (doubleShoot[playerIndex] && totalDoubleOptionBullets[playerIndex][j] < 10) {
				ActiveRect element;
				element.id = totalDoubleOptionBullets[playerIndex][j];
				element.rect = { optionRect[playerIndex][j].x + optionRect[playerIndex][j].w ,optionRect[playerIndex][j].y + optionRect[playerIndex][j].h / 2,shipRect[playerIndex].w / 3,shipRect[playerIndex].h / 3 };
				addToEndActiveRect(doubleOptionBullets[playerIndex][j], totalDoubleOptionBullets[playerIndex][j], element);
			}
		}
	}
}

//move the ship, shield and options to down
void moveToDown(int playerIndex) {
	extern int gameHeight;
	extern int direction[];
	if (shipRect[playerIndex].y + shipRect[playerIndex].h < gameHeight) {
		shipRect[playerIndex].y += shipSpeed[playerIndex];
		if (shield[playerIndex])
			shieldRect[playerIndex].y += shipSpeed[playerIndex];
	}
	direction[playerIndex] = 2;
	moveOption(playerIndex);
}

//move the ship, shield and options to up
void moveToUp(int playerIndex) {
	extern int direction[];
	if (shipRect[playerIndex].y > 0) {
		shipRect[playerIndex].y -= shipSpeed[playerIndex];
		if (shield[playerIndex])
			shieldRect[playerIndex].y -= shipSpeed[playerIndex];
	}
	direction[playerIndex] = 1;
	moveOption(playerIndex);
}

//move the ship, shield and options to left
void moveToLeft(int playerIndex) {
	if (shipRect[playerIndex].x > 0) {
		shipRect[playerIndex].x -= shipSpeed[playerIndex];
		if (shield[playerIndex])
			shieldRect[playerIndex].x -= shipSpeed[playerIndex];
	}
	moveOption(playerIndex);
}

//move the ship, shield and options to right
void moveToRight(int playerIndex) {
	if (shipRect[playerIndex].x + shipRect[playerIndex].w < SCREEN_WIDTH) {
		shipRect[playerIndex].x += shipSpeed[playerIndex];
		if (shield[playerIndex])
			shieldRect[playerIndex].x += shipSpeed[playerIndex];
	}
	moveOption(playerIndex);
}

//detects joystick key in menu 1
void detectKeyJoystick(int playerIndex) {
	extern SDL_Joystick* joystick[];
	extern bool pause;
	if(!pause){
		if (SDL_JoystickGetAxis(joystick[playerIndex], 0) == SDL_JOYSTICK_AXIS_MAX) {
			moveToRight(playerIndex);
		}
		else if (SDL_JoystickGetAxis(joystick[playerIndex], 0) == SDL_JOYSTICK_AXIS_MIN) {
			moveToLeft(playerIndex);
		}
		if (SDL_JoystickGetAxis(joystick[playerIndex], 1) == SDL_JOYSTICK_AXIS_MIN) {
			moveToUp(playerIndex);
		}
		else if (SDL_JoystickGetAxis(joystick[playerIndex], 1) == SDL_JOYSTICK_AXIS_MAX) {
			moveToDown(playerIndex);
		}
		if (SDL_JoystickGetButton(joystick[playerIndex], 2)) {
			shootShip(playerIndex);
		}
		if (SDL_JoystickGetButton(joystick[playerIndex], 1)) {
			shootShipMissile(playerIndex);
		}
		if (SDL_JoystickGetButton(joystick[playerIndex], 3)) {
			selectUpgrade(playerIndex);
		}
	}
	if (SDL_JoystickGetButton(joystick[playerIndex], 9)) {
		pauseGame();
	}
}

//detects key on menu 1
void detectKey() {
	SDL_Event event;
	const Uint8* keys = SDL_GetKeyboardState(NULL);
	while (SDL_PollEvent(&event)) {
		extern bool gameOver;
		int typeEvent = event.type;
		if (typeEvent == SDL_QUIT) {
			gameOver = true;
		}
		else if (typeEvent == SDL_KEYDOWN) {
			extern bool pause;
			if (keys[SDL_SCANCODE_P]) {
				pause = !pause;
			}
			if (!pause) {
				if (keys[SDL_SCANCODE_A]) {
					moveToLeft(0);
				}
				if (keys[SDL_SCANCODE_D]) {
					moveToRight(0);
				}
				if (keys[SDL_SCANCODE_W]) {
					moveToUp(0);
				}
				if (keys[SDL_SCANCODE_S]) {
					moveToDown(0);
				}
				if (keys[SDL_SCANCODE_F]) {
					shootShip(0);
				}
				if (keys[SDL_SCANCODE_H]) {
					shootShipMissile(0);
				}
				if (keys[SDL_SCANCODE_T]) {
					selectUpgrade(0);
				}
				if (keys[SDL_SCANCODE_ESCAPE]) {
					gameOver = true;
				}
				extern int players;
				if (players > 1) {
					if (keys[SDL_SCANCODE_LEFT]) {
						moveToLeft(1);
					}
					if (keys[SDL_SCANCODE_RIGHT]) {
						moveToRight(1);
					}
					if (keys[SDL_SCANCODE_UP]) {
						moveToUp(1);
					}
					if (keys[SDL_SCANCODE_DOWN]) {
						moveToDown(1);
					}
					if (keys[SDL_SCANCODE_J]) {
						shootShip(1);
					}
					if (keys[SDL_SCANCODE_L]) {
						shootShipMissile(1);
					}
					if (keys[SDL_SCANCODE_I]) {
						selectUpgrade(1);
					}
				}
			}
		}
	}
}