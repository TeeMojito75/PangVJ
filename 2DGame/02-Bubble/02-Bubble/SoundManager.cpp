#include "SoundManager.h"
#include <iostream>
#include <cmath>

SoundManager::SoundManager() {
	engine = NULL;
}

SoundManager::~SoundManager() {
	engine->drop();
}

void SoundManager::init() {
	engine = createIrrKlangDevice();
}

ISoundEngine* SoundManager::getSoundEngine() {
	return engine;
}