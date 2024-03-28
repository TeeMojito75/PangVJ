#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Game.h"


void Game::init()
{
	SoundManager::instance().init();
	engine = SoundManager::instance().getSoundEngine();
	irrklang::ISound* sound = engine->play2D("sounds/MenuTheme.wav", true, false, true);
	sound->setVolume(0.3f);

	bPlay = true, start = false, map = true, songPlaying = false;
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	numLevel = 1, view = 0, posIndex = 0;

	menu = new Menu();
	menu->init();

	controls = new Controls();
	controls->init();

	levels = new Levels();
	levels->init();	
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
			engine->drop();
			Game::init();
		}
	}
	keys[key] = true;
	if (key == GLFW_KEY_DOWN && view == 0) { //scroll menu down
		irrklang::ISound* sound = engine->play2D("sounds/MenuMove.wav", false, false, true);
		sound->setVolume(0.1f);
		++posIndex;
		posIndex = (posIndex) % 3;
		menu->setPosIndex(posIndex);
	}
	if (key == GLFW_KEY_UP && view == 0) { //scroll menu up
		irrklang::ISound* sound = engine->play2D("sounds/MenuMove.wav", false, false, true);
		sound->setVolume(0.1f);
		--posIndex;
		if (posIndex < 0) posIndex = 2;
		posIndex = (posIndex) % 3;
		menu->setPosIndex(posIndex);
	}
	if (key == GLFW_KEY_ENTER && view == 0) { 
		irrklang::ISound* sound = engine->play2D("sounds/AccioMenu.wav", false, false, true);
		sound->setVolume(0.4f);
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
		irrklang::ISound* sound = engine->play2D("sounds/NextLvl.wav", false, false, true);
		sound->setVolume(0.2f);
		engine->drop();
		numLevel = levels->getPos() + 1;
		map = false;
		scene = new Scene();
		scene->init(numLevel, 3);
	}
	if (key == GLFW_KEY_RIGHT && start && map) {
		irrklang::ISound* sound = engine->play2D("sounds/MenuMove.wav", false, false, true);
		sound->setVolume(0.2f);
		if (levels->getPos() < 2) levels->setPosIndex((levels->getPos()) + 1);
	}
	if (key == GLFW_KEY_LEFT && start && map) {
		irrklang::ISound* sound = engine->play2D("sounds/MenuMove.wav", false, false, true);
		sound->setVolume(0.2f);
		if (levels->getPos() > 0) levels->setPosIndex((levels->getPos()) - 1);
	}

	//Solo utilitzar las teclas de cambio de mundo dentro de un nivel sino caput
	if (Game::instance().getKey(GLFW_KEY_1))
	{ 
		scene->~Scene();
		scene = new Scene();
		scene->init(1, 3);
	}
	if (Game::instance().getKey(GLFW_KEY_2))
	{
		scene->~Scene();
		scene = new Scene();
		scene->init(2, 3);
	}
	if (Game::instance().getKey(GLFW_KEY_3))
	{
		scene->~Scene();
		scene = new Scene();
		scene->init(3, 3);
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



