#include <SDL_mixer.h>
#include <iostream>
Mix_Music* music[3];
Mix_Chunk* sound[8];

void initSound() {
	music[0] = Mix_LoadMUS("music/02_Beginning of History.mp3");
	if (!music[0]) {
		std::cout << "The beginning music is not loaded" << std::endl;
	}
	music[1] = Mix_LoadMUS("music/03_Stage 1 (Challenger 1985).mp3");
	if (!music[1]) {
		std::cout << "The stage 1 music is not loaded" << std::endl;
	}
	music[2] = Mix_LoadMUS("music/04_Boss Battle (Aircraft Carrier).mp3");
	if (!music[2]) {
		std::cout << "The boss battle music is not loaded" << std::endl;
	}

	sound[0] = Mix_LoadWAV("sound/Sound Effect (6).wav");
	if (!sound[0]) {
		std::cout << "The ship shoot sound is not loaded" << std::endl;
	}
	sound[1] = Mix_LoadWAV("sound/Sound Effect (7).wav");
	if (!sound[1]) {
		std::cout << "The normal enemy dies sound is not loaded" << std::endl;
	}
	sound[2] = Mix_LoadWAV("sound/Sound Effect (2).wav");
	if (!sound[2]) {
		std::cout << "The static enemy dies sound is not loaded" << std::endl;
	}
	sound[3] = Mix_LoadWAV("sound/Sound Effect (11).wav");
	if (!sound[3]) {
		std::cout << "The get upgrade sound is not loaded" << std::endl;
	}
	sound[4] = Mix_LoadWAV("sound/Sound Effect (14).wav");
	if (!sound[4]) {
		std::cout << "The select upgrade sound is not loaded" << std::endl;
	}
	sound[5] = Mix_LoadWAV("sound/Sound Effect (15).wav");
	if (!sound[5]) {
		std::cout << "The select players sound is not loaded" << std::endl;
	}
	sound[6] = Mix_LoadWAV("sound/Sound Effect (18).wav");
	if (!sound[6]) {
		std::cout << "The mountain sound is not loaded" << std::endl;
	}
	sound[7] = Mix_LoadWAV("sound/Sound Effect (17).wav");
	if (!sound[7]) {
		std::cout << "The destroyed boss sound is not loaded" << std::endl;
	}
}