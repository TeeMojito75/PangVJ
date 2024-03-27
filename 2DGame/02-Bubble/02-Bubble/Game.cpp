#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Game.h"


void Game::init()

{
	SoundManager::instance().init();
	engine = SoundManager::instance().getSoundEngine();

	bPlay = true, start = false, map = true, songPlaying = false;
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	numLevel = 1, view = 0, posIndex = 0;

	menu = new Menu();
	menu->init();

	controls = new Controls();
	controls->init();

	levels = new Levels();
	levels->init();

	irrklang::ISound* sound = engine->play2D("sounds/02 Stage 01-03 (Mt. Fuji).wav", true, false, true);
	sound->setVolume(0.5f);
	
}

bool Game::update(int deltaTime)
{
	if (start) {
		if (map) levels->update(deltaTime);
		else scene->update(deltaTime);
	}
	else {
		if (view == 1) controls->update(deltaTime);
		else menu->update(deltaTime);
	}
	return bPlay;

}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (start) {
		if (map) levels->render();
		else scene->render();
	}
	else {
		if (view == 1) controls->render();
		else menu->render();
	}
}

void Game::keyPressed(int key)
{
	if (key == GLFW_KEY_ESCAPE) { // Escape code
		if (view == 0) {
			bPlay = false;
		}
		else {
			view = 0;
			start = false;
			map = true;
		}
	}
	keys[key] = true;
	if (key == GLFW_KEY_DOWN) { //scroll menu down
		++posIndex;
		posIndex = (posIndex) % 3;
		menu->setPosIndex(posIndex);
	}
	if (key == GLFW_KEY_UP) { //scroll menu up
		--posIndex;
		if (posIndex < 0) posIndex = 2;
		posIndex = (posIndex) % 3;
		menu->setPosIndex(posIndex);
	}
	if (key == GLFW_KEY_ENTER && view == 0) { 
		if (posIndex == 0) {
			start = true;
			map = true;
			view = -1;
		}
		if (posIndex == 1) {
			view = posIndex;
		}
		if (posIndex == 2) {
			bPlay = false;
		}
	}
	if (key == GLFW_KEY_SPACE && start && map) {
		numLevel = levels->getPos() + 1;
		map = false;
		scene = new Scene();
		scene->init(numLevel, 3);
	}
	if (key == GLFW_KEY_RIGHT && start && map) {
		if (levels->getPos() < 2) levels->setPosIndex((levels->getPos()) + 1);
	}
	if (key == GLFW_KEY_LEFT && start && map) {
		if (levels->getPos() > 0) levels->setPosIndex((levels->getPos()) - 1);
	}
}

void Game::keyReleased(int key)
{
	keys[key] = false;
}

void Game::mouseMove(int x, int y)
{
}

void Game::mousePress(int button)
{
}

void Game::mouseRelease(int button)
{
}

bool Game::getKey(int key) const
{
	return keys[key];
}



