#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"



#define SCREEN_X 0
#define SCREEN_Y 0

#define INIT_PLAYER_X_TILES 20
#define INIT_PLAYER_Y_TILES 21


Scene::Scene()
{
	map = NULL;
	player = NULL;
	hook = NULL;
	bubbleBig[0] = NULL;
	bubbleBig[1] = NULL;
	background = NULL;
	currentTime = NULL;

	puntuacio = 0;
	vides = 3;
	temps = 91.f;
	hit = false;
	restart = false;
	over = false;
	write = false;
	music = true;
	auxLvl = 1;

	invencible = false;
	petarB1 = false;
	petarB2 = false;
	petarM1 = false;
	petarM2 = false;
	petarM3 = false;
	petarM4 = false;
	SoundManager::instance().init();
	engine = SoundManager::instance().getSoundEngine();
}

Scene::~Scene()
{
	if(map != NULL)
		delete map;
	if(player != NULL)
		delete player;
	if (hook != NULL)
		delete hook;
	if (background != NULL)
		delete background;
	if (engine != NULL)
		engine->drop();

}


void Scene::init(const int& numLevel, int videsRest)
{

	if (restart)
	{
		if (write) 
		{
			gameO[0].init("fonts/DroidSerif.ttf");
			write = false;
		}
			

		if (Game::instance().getKey(GLFW_KEY_C))
		{
			temps = 91.f;
			restart = false;
		}
	}

	else if (over)
	{
		if (write) 
		{
			gameO[0].init("fonts/DroidSerif.ttf");
			gameO[1].init("fonts/DroidSerif.ttf");
			write = false;
		}
		
	}
	
	initShaders();

	auxLvl = numLevel;
	spritesheet.loadFromFile("images/BG" + to_string(numLevel) + ".png", TEXTURE_PIXEL_FORMAT_RGBA);
	background = Sprite::createSprite(glm::vec2(384, 208), glm::vec2(1.f, 1.f), &spritesheet, &texProgram);
	background->setPosition(glm::vec2(SCREEN_X, SCREEN_Y));
	map = TileMap::createTileMap("levels/Mapa" + to_string(numLevel) + ".txt", glm::vec2(0.f, 0.f), texProgram);

	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	player->setTileMap(map);
	hit = false;

	//Reset bolles petites si s'ha mort
	if (petarB1) {
		bubbleMid[0] = NULL;
		bubbleMid[1] = NULL;
		petarB1 = false;
	}
	if (petarB2) {
		bubbleMid[2] = NULL;
		bubbleMid[3] = NULL;
		petarB2 = false;
	}
	if (petarM1) {
		bubbleSmll[0] = NULL;
		bubbleSmll[1] = NULL;
		petarM1 = false;
	}
	if (petarM2) {
		bubbleSmll[2] = NULL;
		bubbleSmll[3] = NULL;
		petarM2 = false;
	}
	if (petarM3) {
		bubbleSmll[4] = NULL;
		bubbleSmll[5] = NULL;
		petarM3 = false;
	}
	if (petarM4) {
		bubbleSmll[6] = NULL;
		bubbleSmll[7] = NULL;
		petarM4 = false;
	}

	bubbleBig[0] = new Bubble();
	bubbleBig[0]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, glm::ivec2(32, 32));
	bubbleBig[0]->setPosition(glm::vec2(22 * map->getTileSize(), 2 * map->getTileSize()));
	bubbleBig[0]->setTileMap(map);
	bubbleBig[1] = new Bubble();
	bubbleBig[1]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, glm::ivec2(32, 32));
	bubbleBig[1]->setPosition(glm::vec2(22 * map->getTileSize(), 2 * map->getTileSize()));
	bubbleBig[1]->changeDir();
	bubbleBig[1]->setTileMap(map);

	for (int i = 0; i < 3; i++) {
		if (!text[i].init("fonts/DroidSerif.ttf")) 
			cout << "Could not load font!!!" << endl;
	}

	if (!over)
	{
		if (!gameO[0].init("fonts/DroidSerif.ttf") || !gameO[1].init("fonts/DroidSerif.ttf"))
			cout << "Could not load font!!!" << endl;
	}

	if (numLevel == 1 && music)
	{
		irrklang::ISound* sound = engine->play2D("sounds/Stage1.wav", true, false, true);
		sound->setVolume(0.4f);
		music = false;
	}
	else if (numLevel == 2 && music)
	{
		irrklang::ISound* sound = engine->play2D("sounds/Stage2.wav", true, false, true);
		sound->setVolume(0.4f);
		music = false;
	}
	else if (numLevel == 3 && music)
	{
		irrklang::ISound* sound = engine->play2D("sounds/Stage3.wav", true, false, true);
		sound->setVolume(0.4f);
		music = false;
	}

	projection = glm::ortho(0.f, 384.f, 248.f, 0.f);
	currentTime = 0.0f;
	
}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	player->update(deltaTime);
	if(hook != NULL) hook->update(deltaTime);

	for (int i = 0; i < 2; i++)
	{
		if (!bubbleBig[i]->getElimina())
			bubbleBig[i]->update(deltaTime);
	}
	if (petarB1)
	{
		for (int i = 0; i < 2; i++)
		{
			if (!bubbleMid[i]->getElimina())
				bubbleMid[i]->update(deltaTime);
		}
	}
	if (petarB2)
	{
		for (int i = 2; i < 4; i++)
		{
			if(!bubbleMid[i]->getElimina())
				bubbleMid[i]->update(deltaTime);
		}
	}

	if (petarM1)
	{
		for (int i = 0; i < 2; i++)
		{
			if (!bubbleSmll[i]->getElimina())
				bubbleSmll[i]->update(deltaTime);
		}
	}
	if (petarM2)
	{
		for (int i = 2; i < 4; i++)
		{
			if (!bubbleSmll[i]->getElimina())
				bubbleSmll[i]->update(deltaTime);
		}
	}
	if (petarM3)
	{
		for (int i = 4; i < 6; i++)
		{
			if (!bubbleSmll[i]->getElimina())
				bubbleSmll[i]->update(deltaTime);
		}
	}
	if (petarM4)
	{
		for (int i = 6; i < 8; i++)
		{
			if (!bubbleSmll[i]->getElimina())
				bubbleSmll[i]->update(deltaTime);
		}
	}
	
	if (!restart) temps -= 0.012f;

	posPaux = player->getPosP();

	//bucle 1 per posició grosses
	for (int i = 0; i < 2; i++)
	{
		if (!bubbleBig[i]->getElimina()) {
			posBaux[i] = bubbleBig[i]->getPosB();

			if (!invencible && (posPaux.x < posBaux[i].x + 10 && posPaux.x + 32 > posBaux[i].x + 5) &&
				(posPaux.y < posBaux[i].y + 10 && posPaux.y + 32 > posBaux[i].y + 5))
			{
				if (!hit && vides >= 0)
				{
					if (vides > 0)
					{
						irrklang::ISound* sound = engine->play2D("sounds/LapaSengancha.wav", false, false, true);
						sound->setVolume(0.5f);
						restart = true;
					}
					else if (vides == 0)
					{
						engine->drop();
						SoundManager::instance().init();
						engine = SoundManager::instance().getSoundEngine();
						irrklang::ISound* sound = engine->play2D("sounds/Game Over.wav", false, false, true);
						sound->setVolume(0.5f);
					}
					hit = true;
					vides -= 1;
				}
			}
		}
	}

	if (petarB1) {
		for (int i = 0; i < 2; i++)
		{
			if (!bubbleMid[i]->getElimina()) {
				posBaux[i+2] = bubbleMid[i]->getPosB();

				if (!invencible && (posPaux.x < posBaux[i+2].x + 10 && posPaux.x + 32 > posBaux[i+2].x + 5) &&
					(posPaux.y < posBaux[i+2].y + 10 && posPaux.y + 32 > posBaux[i+2].y + 5))
				{
					if (!hit && vides >= 0)
					{
						if (vides > 0)
						{
							irrklang::ISound* sound = engine->play2D("sounds/LapaSengancha.wav", false, false, true);
							sound->setVolume(0.5f);
							restart = true;
						}
						else if (vides == 0)
						{
							engine->drop();
							SoundManager::instance().init();
							engine = SoundManager::instance().getSoundEngine();
							irrklang::ISound* sound = engine->play2D("sounds/Game Over.wav", false, false, true);
							sound->setVolume(0.5f);
						}
						hit = true;
						vides -= 1;
					}
				}
			}
		}
	}

	//bucle colisions segona part bolles mitjanes
	if (petarB2) {
		for (int i = 2; i < 4; i++)
		{
			if (!bubbleMid[i]->getElimina()) {
				posBaux[i + 2] = bubbleMid[i]->getPosB();

				if (!invencible && (posPaux.x < posBaux[i+2].x + 10 && posPaux.x + 32 > posBaux[i+2].x + 5) &&
					(posPaux.y < posBaux[i+2].y + 10 && posPaux.y + 32 > posBaux[i+2].y + 5))
				{
					if (!hit && vides >= 0)
					{
						if (vides > 0)
						{
							irrklang::ISound* sound = engine->play2D("sounds/LapaSengancha.wav", false, false, true);
							sound->setVolume(0.5f);
							restart = true;
						}
						else if (vides == 0)
						{
							engine->drop();
							SoundManager::instance().init();
							engine = SoundManager::instance().getSoundEngine();
							irrklang::ISound* sound = engine->play2D("sounds/Game Over.wav", false, false, true);
							sound->setVolume(0.5f);
						}
						hit = true;
						vides -= 1;
					}
				}
			}
		}
	}

	if (petarM1) {
		for (int i = 0; i < 2; i++)
		{
			if (!bubbleSmll[i]->getElimina()) {
				posBaux[i + 6] = bubbleSmll[i]->getPosB();

				if (!invencible && (posPaux.x < posBaux[i + 6].x && posPaux.x + 32 > posBaux[i + 6].x) &&
					(posPaux.y < posBaux[i + 6].y && posPaux.y + 32 > posBaux[i + 6].y))
				{
					if (!hit && vides >= 0)
					{
						if (vides > 0)
						{
							irrklang::ISound* sound = engine->play2D("sounds/LapaSengancha.wav", false, false, true);
							sound->setVolume(0.5f);
							restart = true;
						}
						else if (vides == 0)
						{
							engine->drop();
							SoundManager::instance().init();
							engine = SoundManager::instance().getSoundEngine();
							irrklang::ISound* sound = engine->play2D("sounds/Game Over.wav", false, false, true);
							sound->setVolume(0.5f);
						}
						hit = true;
						vides -= 1;
					}
				}
			}
		}
	}
	if (petarM2) {
		for (int i = 2; i < 4; i++)
		{
			if (!bubbleSmll[i]->getElimina()) {
				posBaux[i + 6] = bubbleSmll[i]->getPosB();

				if (!invencible && (posPaux.x < posBaux[i + 6].x && posPaux.x + 32 > posBaux[i + 6].x) &&
					(posPaux.y < posBaux[i + 6].y && posPaux.y + 32 > posBaux[i + 6].y))
				{
					if (!hit && vides >= 0)
					{
						if (vides > 0)
						{
							irrklang::ISound* sound = engine->play2D("sounds/LapaSengancha.wav", false, false, true);
							sound->setVolume(0.5f);
							restart = true;
						}
						else if (vides == 0)
						{
							engine->drop();
							SoundManager::instance().init();
							engine = SoundManager::instance().getSoundEngine();
							irrklang::ISound* sound = engine->play2D("sounds/Game Over.wav", false, false, true);
							sound->setVolume(0.5f);
						}
						hit = true;
						vides -= 1;
					}
				}
			}
		}
	}
	if (petarM3) {
		for (int i = 4; i < 6; i++)
		{
			if (!bubbleSmll[i]->getElimina()) {
				posBaux[i + 6] = bubbleSmll[i]->getPosB();

				if (!invencible && (posPaux.x < posBaux[i + 6].x + 10 && posPaux.x + 32 > posBaux[i + 6].x + 5) &&
					(posPaux.y < posBaux[i + 6].y + 10 && posPaux.y + 32 > posBaux[i + 6].y + 5))
				{
					if (!hit && vides >= 0)
					{
						if (vides > 0)
						{
							irrklang::ISound* sound = engine->play2D("sounds/LapaSengancha.wav", false, false, true);
							sound->setVolume(0.5f);
							restart = true;
						}
						else if (vides == 0)
						{
							engine->drop();
							SoundManager::instance().init();
							engine = SoundManager::instance().getSoundEngine();
							irrklang::ISound* sound = engine->play2D("sounds/Game Over.wav", false, false, true);
							sound->setVolume(0.5f);
						}
						hit = true;
						vides -= 1;
					}
				}
			}
		}
	}
	if (petarM4) {
		for (int i = 6; i < 8; i++)
		{
			if (!bubbleSmll[i]->getElimina()) {
				posBaux[i + 6] = bubbleSmll[i]->getPosB();

				if (!invencible && (posPaux.x < posBaux[i + 6].x + 10 && posPaux.x + 32 > posBaux[i + 6].x + 5) &&
					(posPaux.y < posBaux[i + 6].y + 10 && posPaux.y + 32 > posBaux[i + 6].y + 5))
				{
					if (!hit && vides >= 0)
					{
						if (vides > 0)
						{
							irrklang::ISound* sound = engine->play2D("sounds/LapaSengancha.wav", false, false, true);
							sound->setVolume(0.5f);
							restart = true;
						}
						else if (vides == 0)
						{
							engine->drop();
							SoundManager::instance().init();
							engine = SoundManager::instance().getSoundEngine();
							irrklang::ISound* sound = engine->play2D("sounds/Game Over.wav", false, false, true);
							sound->setVolume(0.5f);
						}
						hit = true;
						vides -= 1;
					}
				}
			}
		}
	}


	if (vides < 0)
	{
		over = true;
		restart = false;
		write = true;
		Scene::init(auxLvl, vides);
	}

	if (temps <= 0.f)
	{
		if (vides >= 0)
		{
			write = true;
			restart = true;
			temps = 91.f;
			vides -= 1;
			Scene::init(auxLvl, vides);
		}
	}

	if (restart)
	{
		write = true;
		Scene::init(auxLvl, vides);
	}

	
	if (Game::instance().getKey(GLFW_KEY_S) && hook == NULL) {
		hook = new Hook();
		hook->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		hook->setPosition(glm::vec2(player->getPosP().x + 16 / 8 * map->getTileSize(), player->getPosP().y - 187 + 32 / 8 * map->getTileSize()));
		hook->setTileMap(map);
	}

	if (hook != NULL && map->collisionRoof(glm::vec2(hook->getPosHook().x, hook->getPosHook().y + 187 - hook->getHighLength()), glm::ivec2(9, hook->getHighLength()))) {
		delete hook;
		hook = NULL;
	}
	
	if (!petarB1 && hook != NULL && circleRect(bubbleBig[0]->getPosB().x + 16, bubbleBig[0]->getPosB().y + 16, 16, hook->getPosHook().x, hook->getPosHook().y + 187 - hook->getHighLength(), 9, hook->getHighLength())) {
		delete hook;
		hook = NULL;
		puntuacio += 200;
		petarB1 = true;
		bubbleMid[0] = new Bubble();
		bubbleMid[0]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, glm::ivec2(16, 16));
		bubbleMid[0]->setPosition(posBaux[0]);
		bubbleMid[0]->setTileMap(map);

		bubbleMid[1] = new Bubble();
		bubbleMid[1]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, glm::ivec2(16, 16));
		bubbleMid[1]->setPosition(posBaux[0]);
		bubbleMid[1]->changeDir();
		bubbleMid[1]->setTileMap(map);

		bubbleBig[0]->tocada();
	}

	if (!petarB2 && hook != NULL && circleRect(bubbleBig[1]->getPosB().x + 16, bubbleBig[1]->getPosB().y + 16, 16, hook->getPosHook().x, hook->getPosHook().y + 187 - hook->getHighLength(), 9, hook->getHighLength())) {
		delete hook;
		hook = NULL;
		puntuacio += 200;
		petarB2 = true;
		bubbleMid[2] = new Bubble();
		bubbleMid[2]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, glm::ivec2(16, 16));
		bubbleMid[2]->setPosition(posBaux[1]);
		bubbleMid[2]->setTileMap(map);

		bubbleMid[3] = new Bubble();
		bubbleMid[3]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, glm::ivec2(16, 16));
		bubbleMid[3]->setPosition(posBaux[1]);
		bubbleMid[3]->changeDir();
		bubbleMid[3]->setTileMap(map);

		bubbleBig[1]->tocada();
	}

	if (!petarM1 && petarB1 && hook != NULL && circleRect(bubbleMid[0]->getPosB().x + 8, bubbleMid[0]->getPosB().y + 8, 8, hook->getPosHook().x, hook->getPosHook().y + 187 - hook->getHighLength(), 9, hook->getHighLength()))
	{
		delete hook;
		hook = NULL;
		puntuacio += 400;
		petarM1 = true;
		bubbleSmll[0] = new Bubble();
		bubbleSmll[0]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, glm::ivec2(8, 8));
		bubbleSmll[0]->setPosition(posBaux[2]);
		bubbleSmll[0]->setTileMap(map);

		bubbleSmll[1] = new Bubble();
		bubbleSmll[1]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, glm::ivec2(8, 8));
		bubbleSmll[1]->setPosition(posBaux[2]);
		bubbleSmll[1]->changeDir();
		bubbleSmll[1]->setTileMap(map);

		bubbleMid[0]->tocada();
	}

	if (!petarM2 && petarB1 && hook != NULL && circleRect(bubbleMid[1]->getPosB().x + 8, bubbleMid[1]->getPosB().y + 8, 8, hook->getPosHook().x, hook->getPosHook().y + 187 - hook->getHighLength(), 9, hook->getHighLength()))
	{
		delete hook;
		hook = NULL;
		puntuacio += 400;
		petarM2 = true;
		bubbleSmll[2] = new Bubble();
		bubbleSmll[2]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, glm::ivec2(8, 8));
		bubbleSmll[2]->setPosition(posBaux[3]);
		bubbleSmll[2]->setTileMap(map);

		bubbleSmll[3] = new Bubble();
		bubbleSmll[3]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, glm::ivec2(8, 8));
		bubbleSmll[3]->setPosition(posBaux[3]);
		bubbleSmll[3]->changeDir();
		bubbleSmll[3]->setTileMap(map);

		bubbleMid[1]->tocada();
	}

	if (!petarM3 && petarB2 && hook != NULL && circleRect(bubbleMid[2]->getPosB().x + 8, bubbleMid[2]->getPosB().y + 8, 8, hook->getPosHook().x, hook->getPosHook().y + 187 - hook->getHighLength(), 9, hook->getHighLength()))
	{
		delete hook;
		hook = NULL;
		puntuacio += 400;
		petarM3 = true;
		bubbleSmll[4] = new Bubble();
		bubbleSmll[4]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, glm::ivec2(8, 8));
		bubbleSmll[4]->setPosition(posBaux[4]);
		bubbleSmll[4]->setTileMap(map);

		bubbleSmll[5] = new Bubble();
		bubbleSmll[5]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, glm::ivec2(8, 8));
		bubbleSmll[5]->setPosition(posBaux[4]);
		bubbleSmll[5]->changeDir();
		bubbleSmll[5]->setTileMap(map);

		bubbleMid[2]->tocada();
	}

	if (!petarM4 && petarB2 && hook != NULL && circleRect(bubbleMid[3]->getPosB().x + 8, bubbleMid[3]->getPosB().y + 8, 8, hook->getPosHook().x, hook->getPosHook().y + 187 - hook->getHighLength(), 9, hook->getHighLength()))
	{
		delete hook;
		hook = NULL;
		puntuacio += 400;
		petarM4 = true;
		bubbleSmll[6] = new Bubble();
		bubbleSmll[6]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, glm::ivec2(8, 8));
		bubbleSmll[6]->setPosition(posBaux[5]);
		bubbleSmll[6]->setTileMap(map);

		bubbleSmll[7] = new Bubble();
		bubbleSmll[7]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, glm::ivec2(8, 8));
		bubbleSmll[7]->setPosition(posBaux[5]);
		bubbleSmll[7]->changeDir();
		bubbleSmll[7]->setTileMap(map);

		bubbleMid[3]->tocada();
	}

}

bool Scene::circleRect(float cx, float cy, float radius, float rx, float ry, float rw, float rh) {

	// temporary variables to set edges for testing
	float testX = cx;
	float testY = cy;

	// which edge is closest?
	if (cx < rx)         testX = rx;      // test left edge
	else if (cx > rx + rw) testX = rx + rw;   // right edge
	if (cy < ry)         testY = ry;      // top edge
	else if (cy > ry + rh) testY = ry + rh;   // bottom edge

	// get distance from closest edges
	float distX = cx - testX;
	float distY = cy - testY;
	float distance = sqrt((distX * distX) + (distY * distY));

	// if the distance is less than the radius, collision!
	if (distance <= radius) {
		return true;
	}
	return false;
}

void Scene::render()
{
	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	background->render();
	map->render();
	player->render();
	if (hook != NULL) hook->render();
	
	
	for (int i = 0; i < 2; i++)
	{
		if (!bubbleBig[i]->getElimina())
				bubbleBig[i]->render();
	}
	

	if (petarB1)
	{
		for (int i = 0; i < 2; i++)
		{
			if (!bubbleMid[i]->getElimina())
				bubbleMid[i]->render();		
		}

	}

	if (petarB2)
	{
		for (int i = 2; i < 4; i++)
		{
			if (!bubbleMid[i]->getElimina())
				bubbleMid[i]->render();
		}

	}
	
	if (petarM1)
	{
		for (int i = 0; i < 2; i++)
		{
			if (!bubbleSmll[i]->getElimina()) 
				bubbleSmll[i]->render();
		}
	}

	if (petarM2)
	{
		for (int i = 2; i < 4; i++)
		{
			if (!bubbleSmll[i]->getElimina())
				bubbleSmll[i]->render();
		}
	}

	if (petarM3)
	{
		for (int i = 4; i < 6; i++)
		{
			if (!bubbleSmll[i]->getElimina())
				bubbleSmll[i]->render();
		}
	}

	if (petarM4)
	{
		for (int i = 6; i < 8; i++)
		{
			if (!bubbleSmll[i]->getElimina())
				bubbleSmll[i]->render();
		}
	}

	text[0].render("Puntuacio: " + to_string(puntuacio), glm::vec2(20, 780), 48, glm::vec4(1, 1, 1, 1));
	if (vides >= 0) text[1].render("Vides: " + to_string(vides), glm::vec2(130, 840), 48, glm::vec4(1, 1, 1, 1));
	if (temps > 0.f) text[2].render("Temps: " + to_string(int(temps)), glm::vec2(1050, 780), 48, glm::vec4(1, 1, 1, 1));

	if (!over)
	{
		if (auxLvl == 1)
		{
			gameO[0].render("Stage 1", glm::vec2(570, 780), 48, glm::vec4(1, 1, 1, 1));
			gameO[1].render("Mt. Fuji", glm::vec2(560, 840), 48, glm::vec4(1, 1, 1, 1));
		}

		if (auxLvl == 2)
		{
			gameO[0].render("Stage 2", glm::vec2(570, 780), 48, glm::vec4(1, 1, 1, 1));
			gameO[1].render("Lake", glm::vec2(600, 840), 48, glm::vec4(1, 1, 1, 1));
		}

		if (auxLvl == 3)
		{
			gameO[0].render("Stage 3", glm::vec2(570, 780), 48, glm::vec4(1, 1, 1, 1));
			gameO[1].render("Easter Island", glm::vec2(510, 840), 48, glm::vec4(1, 1, 1, 1));
		}
	}

	if (restart)
		gameO[0].render("Press C To Try Again!!!", glm::vec2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2), 64, glm::vec4(0, 0, 0, 1));

	if (over)
	{
		gameO[0].render("GAME OVER!!!", glm::vec2(SCREEN_WIDTH / 2 + 130, SCREEN_HEIGHT / 2), 64, glm::vec4(0, 0, 0, 1));
		gameO[1].render("Press Esc To Exit", glm::vec2(SCREEN_WIDTH / 2 + 90, SCREEN_HEIGHT / 2 + 100), 64, glm::vec4(0, 0, 0, 1));
	}

}

void Scene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if(!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if(!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if(!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}



