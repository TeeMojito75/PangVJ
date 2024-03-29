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
	hook[0] = NULL;
	hook[1] = NULL;
	power[0] = NULL;
	power[1] = NULL;
	power[2] = NULL;
	food[0] = NULL;
	food[1] = NULL;
	food[2] = NULL;
	bubbleBig[0] = NULL;
	bubbleBig[1] = NULL;
	background = NULL;
	currentTime = NULL;

	puntuacio = 0;
	vides = 3;
	temps = 91.f;
	tempsAuxInvencible = 0.f;
	tempsAuxStop = 0.f;
	tempsAuxSlow = 0.f;
	hit = false;
	restart = false;
	over = false;
	write = false;
	music = true;
	victoria = false;
	auxLvl = 1;

	invencible = false;
	slow = false;
	stop = false;

	godmode = false;

	petarB1 = false;
	petarB2 = false;
	petarM1 = false;
	petarM2 = false;
	petarM3 = false;
	petarM4 = false;
	petarS1 = false;
	petarS2 = false;
	petarS3 = false;
	petarS4 = false;
	petarS5 = false;
	petarS6 = false;
	petarS7 = false;
	petarS8 = false;
	SoundManager::instance().init();
	engine = SoundManager::instance().getSoundEngine();
}

Scene::~Scene()
{
	if(map != NULL)
		delete map;
	if(player != NULL)
		delete player;
	if (hook[0] != NULL)
		delete hook;
	if (hook[1] != NULL)
		delete hook;
	if (power[0] != NULL)
		delete power;
	if (power[1] != NULL)
		delete power;
	if (power[2] != NULL)
		delete power;
	/*
	if (food[0] != NULL)
		delete food;
	if (food[1] != NULL)
		delete food;
	if (food[2] != NULL)
		delete food;
		*/
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

	else if (victoria)
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

	food[0] = new Food();
	food[0]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 0);
	food[0]->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), 5 * map->getTileSize()));
	food[0]->setTileMap(map);

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
		petarS1 = false;
		petarS2 = false;
	}
	if (petarM2) {
		bubbleSmll[2] = NULL;
		bubbleSmll[3] = NULL;
		petarM2 = false;
		petarS3 = false;
		petarS4 = false;
	}
	if (petarM3) {
		bubbleSmll[4] = NULL;
		bubbleSmll[5] = NULL;
		petarM3 = false;
		petarS5 = false;
		petarS6 = false;
	}
	if (petarM4) {
		bubbleSmll[6] = NULL;
		bubbleSmll[7] = NULL;
		petarM4 = false;
		petarS7 = false;
		petarS8 = false;
	}

	bubbleBig[0] = new Bubble();
	bubbleBig[0]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, glm::ivec2(32, 32), glm::vec2(22 * map->getTileSize(), 5 * map->getTileSize()));
	bubbleBig[0]->setTileMap(map);
	bubbleBig[1] = new Bubble();
	bubbleBig[1]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, glm::ivec2(32, 32), glm::vec2(22 * map->getTileSize(), 5 * map->getTileSize()));
	bubbleBig[1]->changeDir();
	bubbleBig[1]->setTileMap(map);

	//Text per saber si godMode actiu
	godM.init("fonts/DroidSerif.ttf");

	for (int i = 0; i < 3; i++) {
		if (!text[i].init("fonts/DroidSerif.ttf")) 
			cout << "Could not load font!!!" << endl;
	}

	if (!over || !victoria)
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
	if (food[0] != NULL)food[0]->update(deltaTime);
	if (food[1] != NULL)food[1]->update(deltaTime);
	if (food[2] != NULL)food[2]->update(deltaTime);
	if (power[0] != NULL) power[0]->update(deltaTime);
	if (power[1] != NULL) power[1]->update(deltaTime);
	if (power[2] != NULL) power[2]->update(deltaTime);
	if(hook[0] != NULL) hook[0]->update(deltaTime);
	if (hook[1] != NULL) hook[1]->update(deltaTime);

	if (!stop) {

		for (int i = 0; i < 2; i++)
		{
			if (!bubbleBig[i]->getElimina())
				bubbleBig[i]->update(deltaTime, slow);
		}
		if (petarB1)
		{
			for (int i = 0; i < 2; i++)
			{
				if (!bubbleMid[i]->getElimina())
					bubbleMid[i]->update(deltaTime, slow);
			}
		}
		if (petarB2)
		{
			for (int i = 2; i < 4; i++)
			{
				if (!bubbleMid[i]->getElimina())
					bubbleMid[i]->update(deltaTime, slow);
			}
		}

		if (petarM1)
		{
			for (int i = 0; i < 2; i++)
			{
				if (!bubbleSmll[i]->getElimina())
					bubbleSmll[i]->update(deltaTime, slow);
			}
		}
		if (petarM2)
		{
			for (int i = 2; i < 4; i++)
			{
				if (!bubbleSmll[i]->getElimina())
					bubbleSmll[i]->update(deltaTime, slow);
			}
		}
		if (petarM3)
		{
			for (int i = 4; i < 6; i++)
			{
				if (!bubbleSmll[i]->getElimina())
					bubbleSmll[i]->update(deltaTime, slow);
			}
		}
		if (petarM4)
		{
			for (int i = 6; i < 8; i++)
			{
				if (!bubbleSmll[i]->getElimina())
					bubbleSmll[i]->update(deltaTime, slow);
			}
		}

	}
	
	if (!restart) temps -= 0.012f;

	if (temps == 70)
	{
		food[0] = new Food();
		food[0]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 1);
		food[0]->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), 5 * map->getTileSize()));
		food[0]->setTileMap(map);
	}
	if (temps == 40)
	{
		food[0] = new Food();
		food[0]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 2);
		food[0]->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), 5 * map->getTileSize()));
		food[0]->setTileMap(map);
	}

	posPaux = player->getPosP();

	//bucle 1 per posició grosses
	for (int i = 0; i < 2; i++)
	{
		if (!bubbleBig[i]->getElimina()) {
			posBaux[i] = bubbleBig[i]->getPosB();

			if (!godmode && !invencible && (posPaux.x < posBaux[i].x + 10 && posPaux.x + 32 > posBaux[i].x + 5) &&
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

				if (!godmode && !invencible && (posPaux.x < posBaux[i+2].x + 10 && posPaux.x + 32 > posBaux[i+2].x + 5) &&
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

				if (!godmode && !invencible && (posPaux.x < posBaux[i+2].x + 10 && posPaux.x + 32 > posBaux[i+2].x + 5) &&
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

				if (!godmode && !invencible && (posPaux.x < posBaux[i + 6].x && posPaux.x + 32 > posBaux[i + 6].x) &&
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

				if (!godmode && !invencible && (posPaux.x < posBaux[i + 6].x && posPaux.x + 32 > posBaux[i + 6].x) &&
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

				if (!godmode && !invencible && (posPaux.x < posBaux[i + 6].x + 10 && posPaux.x + 32 > posBaux[i + 6].x + 5) &&
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

				if (!godmode && !invencible && (posPaux.x < posBaux[i + 6].x + 10 && posPaux.x + 32 > posBaux[i + 6].x + 5) &&
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
		hook[0] = NULL;
		hook[1] = NULL;
		power[0] = NULL;
		power[1] = NULL;
		power[2] = NULL;
		food[0] = NULL;
		food[1] = NULL;
		food[2] = NULL;
		slow = false;
		invencible = false;
		stop = false;
		write = true;
		Scene::init(auxLvl, vides);
	}

	if (temps <= 0.f)
	{
		if (vides >= 0)
		{
			write = true;
			restart = true;
			hook[0] = NULL;
			hook[1] = NULL;
			power[0] = NULL;
			power[1] = NULL;
			power[2] = NULL;
			food[0] = NULL;
			food[1] = NULL;
			food[2] = NULL;
			slow = false;
			invencible = false;
			stop = false;
			temps = 91.f;
			vides -= 1;
			Scene::init(auxLvl, vides);
		}
	}

	if (restart)
	{
		write = true;
		hook[0] = NULL;
		hook[1] = NULL;
		power[0] = NULL;
		power[1] = NULL;
		power[2] = NULL;
		food[0] = NULL;
		food[1] = NULL;
		food[2] = NULL;
		slow = false;
		invencible = false;
		stop = false;
		puntuacio = 0;
		Scene::init(auxLvl, vides);
	}

	for (int i = 0; i < 3; ++i) {
		if (food[i] != NULL) {
			if (circleRect(food[i]->getPosF().x + 16, food[i]->getPosF().y + 16, 16, player->getPosP().x, player->getPosP().y, 32, 32)) {
				puntuacio += food[i]->getPoints();
				delete food[i];
				food[i] = NULL;
			}
		}
	}

	for (int i = 0; i < 3; ++i) {
		if (power[i] != NULL) {
			if (circleRect(power[i]->getPosPo().x + 8, power[i]->getPosPo().y + 8, 8, player->getPosP().x, player->getPosP().y, 32, 32)) {
				if (power[i]->getType() == 0) {
					slow = true;
					tempsAuxSlow = 5.f;
				}
				if (power[i]->getType() == 1) {
					invencible = true;
					tempsAuxInvencible = 5.f;
				}
				if (power[i]->getType() == 2) {
					stop = true;
					tempsAuxStop = 5.f;
				}
				delete power[i];
				power[i] = NULL;
			}
		}
	}

	if (slow) {
		if (tempsAuxSlow <= 0) slow = false;
		else tempsAuxSlow -= 0.012;
	}

	if (invencible) {
		if (tempsAuxInvencible <= 0) invencible = false;
		else tempsAuxInvencible -= 0.012;
	}

	if (stop) {
		if (tempsAuxStop <= 0) stop = false;
		else tempsAuxStop -= 0.012;
	}

	
	if (Game::instance().getKey(GLFW_KEY_S) && hook[0] == NULL) {
		hook[0] = new Hook();
		hook[0]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		hook[0]->setPosition(glm::vec2(player->getPosP().x + 16 / 8 * map->getTileSize(), player->getPosP().y - 187 + 32 / 8 * map->getTileSize()));
		hook[0]->setTileMap(map);
	}



	if (hook[0] != NULL && map->collisionRoof(glm::vec2(hook[0]->getPosHook().x, hook[0]->getPosHook().y + 187 - hook[0]->getHighLength()), glm::ivec2(9, hook[0]->getHighLength()))) {
		delete hook[0];
		hook[0] = NULL;
	}
	
	if (!petarB1 && hook[0] != NULL && circleRect(bubbleBig[0]->getPosB().x + 16, bubbleBig[0]->getPosB().y + 16, 16, hook[0]->getPosHook().x, hook[0]->getPosHook().y + 187 - hook[0]->getHighLength(), 9, hook[0]->getHighLength())) {
		delete hook[0];
		hook[0] = NULL;
		puntuacio += 200;

		power[0] = new Power();
		power[0]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 0);
		power[0]->setPosition(bubbleBig[0]->getPosB());
		power[0]->setTileMap(map);

		petarB1 = true;
		bubbleMid[0] = new Bubble();
		bubbleMid[0]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, glm::ivec2(16, 16), posBaux[0]);
		bubbleMid[0]->setTileMap(map);

		bubbleMid[1] = new Bubble();
		bubbleMid[1]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, glm::ivec2(16, 16), posBaux[0]);
		bubbleMid[1]->changeDir();
		bubbleMid[1]->setTileMap(map);

		bubbleBig[0]->tocada();
	}

	if (!petarB2 && hook[0] != NULL && circleRect(bubbleBig[1]->getPosB().x + 16, bubbleBig[1]->getPosB().y + 16, 16, hook[0]->getPosHook().x, hook[0]->getPosHook().y + 187 - hook[0]->getHighLength(), 9, hook[0]->getHighLength())) {
		delete hook[0];
		hook[0] = NULL;
		puntuacio += 200;
		petarB2 = true;
		bubbleMid[2] = new Bubble();
		bubbleMid[2]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, glm::ivec2(16, 16), posBaux[1]);
		bubbleMid[2]->setTileMap(map);

		bubbleMid[3] = new Bubble();
		bubbleMid[3]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, glm::ivec2(16, 16), posBaux[1]);
		bubbleMid[3]->changeDir();
		bubbleMid[3]->setTileMap(map);

		bubbleBig[1]->tocada();
	}

	if (!petarM1 && petarB1 && hook[0] != NULL && circleRect(bubbleMid[0]->getPosB().x + 8, bubbleMid[0]->getPosB().y + 8, 8, hook[0]->getPosHook().x, hook[0]->getPosHook().y + 187 - hook[0]->getHighLength(), 9, hook[0]->getHighLength()))
	{
		delete hook[0];
		hook[0] = NULL;
		puntuacio += 400;
		petarM1 = true;
		bubbleSmll[0] = new Bubble();
		bubbleSmll[0]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, glm::ivec2(8, 8), posBaux[2]);
		bubbleSmll[0]->setTileMap(map);

		bubbleSmll[1] = new Bubble();
		bubbleSmll[1]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, glm::ivec2(8, 8), posBaux[2]);
		bubbleSmll[1]->changeDir();
		bubbleSmll[1]->setTileMap(map);

		bubbleMid[0]->tocada();
	}

	if (!petarM2 && petarB1 && hook[0] != NULL && circleRect(bubbleMid[1]->getPosB().x + 8, bubbleMid[1]->getPosB().y + 8, 8, hook[0]->getPosHook().x, hook[0]->getPosHook().y + 187 - hook[0]->getHighLength(), 9, hook[0]->getHighLength()))
	{
		delete hook[0];
		hook[0] = NULL;
		puntuacio += 400;
		petarM2 = true;
		bubbleSmll[2] = new Bubble();
		bubbleSmll[2]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, glm::ivec2(8, 8), posBaux[3]);
		bubbleSmll[2]->setTileMap(map);

		bubbleSmll[3] = new Bubble();
		bubbleSmll[3]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, glm::ivec2(8, 8), posBaux[3]);
		bubbleSmll[3]->changeDir();
		bubbleSmll[3]->setTileMap(map);

		power[1] = new Power();
		power[1]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 1);
		power[1]->setPosition(bubbleMid[1]->getPosB());
		power[1]->setTileMap(map);

		bubbleMid[1]->tocada();
	}

	if (!petarM3 && petarB2 && hook[0] != NULL && circleRect(bubbleMid[2]->getPosB().x + 8, bubbleMid[2]->getPosB().y + 8, 8, hook[0]->getPosHook().x, hook[0]->getPosHook().y + 187 - hook[0]->getHighLength(), 9, hook[0]->getHighLength()))
	{
		delete hook[0];
		hook[0] = NULL;
		puntuacio += 400;
		petarM3 = true;
		bubbleSmll[4] = new Bubble();
		bubbleSmll[4]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, glm::ivec2(8, 8), posBaux[4]);
		bubbleSmll[4]->setTileMap(map);

		bubbleSmll[5] = new Bubble();
		bubbleSmll[5]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, glm::ivec2(8, 8), posBaux[4]);
		bubbleSmll[5]->changeDir();
		bubbleSmll[5]->setTileMap(map);

		bubbleMid[2]->tocada();
	}

	if (!petarM4 && petarB2 && hook[0] != NULL && circleRect(bubbleMid[3]->getPosB().x + 8, bubbleMid[3]->getPosB().y + 8, 8, hook[0]->getPosHook().x, hook[0]->getPosHook().y + 187 - hook[0]->getHighLength(), 9, hook[0]->getHighLength()))
	{
		delete hook[0];
		hook[0] = NULL;
		puntuacio += 400;
		petarM4 = true;
		bubbleSmll[6] = new Bubble();
		bubbleSmll[6]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, glm::ivec2(8, 8), posBaux[5]);
		bubbleSmll[6]->setTileMap(map);

		bubbleSmll[7] = new Bubble();
		bubbleSmll[7]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, glm::ivec2(8, 8), posBaux[5]);
		bubbleSmll[7]->changeDir();
		bubbleSmll[7]->setTileMap(map);

		power[2] = new Power();
		power[2]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 2);
		power[2]->setPosition(bubbleMid[3]->getPosB());
		power[2]->setTileMap(map);

		bubbleMid[3]->tocada();
	}

	if (!petarS1 && petarM1 && hook[0] != NULL && circleRect(bubbleSmll[0]->getPosB().x + 4, bubbleSmll[0]->getPosB().y + 4, 8, hook[0]->getPosHook().x, hook[0]->getPosHook().y + 187 - hook[0]->getHighLength(), 9, hook[0]->getHighLength()))
	{
		delete hook[0];
		hook[0] = NULL;
		puntuacio += 800;
		petarS1 = true;
		bubbleSmll[0]->tocada();
	}

	if (!petarS2 && petarM1 && hook[0] != NULL && circleRect(bubbleSmll[1]->getPosB().x + 4, bubbleSmll[1]->getPosB().y + 4, 8, hook[0]->getPosHook().x, hook[0]->getPosHook().y + 187 - hook[0]->getHighLength(), 9, hook[0]->getHighLength()))
	{
		delete hook[0];
		hook[0] = NULL;
		puntuacio += 800;
		petarS2 = true;
		bubbleSmll[1]->tocada();
	}

	if (!petarS3 && petarM2 && hook[0] != NULL && circleRect(bubbleSmll[2]->getPosB().x + 4, bubbleSmll[2]->getPosB().y + 4, 8, hook[0]->getPosHook().x, hook[0]->getPosHook().y + 187 - hook[0]->getHighLength(), 9, hook[0]->getHighLength()))
	{
		delete hook[0];
		hook[0] = NULL;
		puntuacio += 800;
		petarS3 = true;
		bubbleSmll[2]->tocada();
	}

	if (!petarS4 && petarM2 && hook[0] != NULL && circleRect(bubbleSmll[3]->getPosB().x + 4, bubbleSmll[3]->getPosB().y + 4, 8, hook[0]->getPosHook().x, hook[0]->getPosHook().y + 187 - hook[0]->getHighLength(), 9, hook[0]->getHighLength()))
	{
		delete hook[0];
		hook[0] = NULL;
		puntuacio += 800;
		petarS4 = true;
		bubbleSmll[3]->tocada();
	}

	if (!petarS5 && petarM3 && hook[0] != NULL && circleRect(bubbleSmll[4]->getPosB().x + 4, bubbleSmll[4]->getPosB().y + 4, 8, hook[0]->getPosHook().x, hook[0]->getPosHook().y + 187 - hook[0]->getHighLength(), 9, hook[0]->getHighLength()))
	{
		delete hook[0];
		hook[0] = NULL;
		puntuacio += 800;
		petarS5 = true;
		bubbleSmll[4]->tocada();
	}

	if (!petarS6 && petarM3 && hook[0] != NULL && circleRect(bubbleSmll[5]->getPosB().x + 4, bubbleSmll[5]->getPosB().y + 4, 8, hook[0]->getPosHook().x, hook[0]->getPosHook().y + 187 - hook[0]->getHighLength(), 9, hook[0]->getHighLength()))
	{
		delete hook[0];
		hook[0] = NULL;
		puntuacio += 800;
		petarS6 = true;
		bubbleSmll[5]->tocada();
	}

	if (!petarS7 && petarM4 && hook[0] != NULL && circleRect(bubbleSmll[6]->getPosB().x + 4, bubbleSmll[6]->getPosB().y + 4, 8, hook[0]->getPosHook().x, hook[0]->getPosHook().y + 187 - hook[0]->getHighLength(), 9, hook[0]->getHighLength()))
	{
		delete hook[0];
		hook[0] = NULL;
		puntuacio += 800;
		petarS7 = true;
		bubbleSmll[6]->tocada();
	}

	if (!petarS8 && petarM4 && hook[0] != NULL && circleRect(bubbleSmll[7]->getPosB().x + 4, bubbleSmll[7]->getPosB().y + 4, 8, hook[0]->getPosHook().x, hook[0]->getPosHook().y + 187 - hook[0]->getHighLength(), 9, hook[0]->getHighLength()))
	{
		delete hook[0];
		hook[0] = NULL;
		puntuacio += 800;
		petarS8 = true;
		bubbleSmll[7]->tocada();
	}

	if (petarB1 && petarB2 && petarM1 && petarM2 && petarM3 && petarM4 && petarS1 && petarS2 && petarS3
		&& petarS4 && petarS5 && petarS6 && petarS7 && petarS8)
	{
		victoria = true;
		restart = false;
		puntuacio = 0;
		over = false;
		hook[0] = NULL;
		hook[1] = NULL;
		power[0] = NULL;
		power[1] = NULL;
		power[2] = NULL;
		food[0] = NULL;
		food[1] = NULL;
		food[2] = NULL;
		slow = false;
		invencible = false;
		stop = false;
		write = true;
		engine->drop();
		SoundManager::instance().init();
		engine = SoundManager::instance().getSoundEngine();
		irrklang::ISound* sound = engine->play2D("sounds/StageCleared.wav", false, false, true);
		sound->setVolume(0.5f);
		Scene::init(auxLvl, 0);
	}

	//Godmode
	if (Game::instance().getKey(GLFW_KEY_G))
	{
		godmode = !godmode;
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
	if (food[0] != NULL && !restart && !over && !victoria) food[0]->render();
	if (food[1] != NULL && !restart && !over && !victoria) food[1]->render();
	if (food[2] != NULL && !restart && !over && !victoria) food[2]->render();
	if (power[0] != NULL && !restart && !over && !victoria) power[0]->render();
	if (power[1] != NULL && !restart && !over && !victoria) power[1]->render();
	if (power[2] != NULL && !restart && !over && !victoria) power[2]->render();
	if (hook[0] != NULL && !restart && !over && !victoria) hook[0]->render();
	if (hook[1] != NULL && !restart && !over && !victoria) hook[1]->render();
	
	
	for (int i = 0; i < 2; i++)
	{
		if (!bubbleBig[i]->getElimina() && !restart && !over && !victoria)
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

	if (godmode)
	{
		godM.render("G!!", glm::vec2(850, 840), 48, glm::vec4(1, 1, 1, 1));
	}

	if (restart)
		gameO[0].render("Press C To Try Again!!!", glm::vec2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2), 64, glm::vec4(0, 0, 0, 1));

	if (over)
	{
		gameO[0].render("GAME OVER!!!", glm::vec2(SCREEN_WIDTH / 2 + 130, SCREEN_HEIGHT / 2), 64, glm::vec4(0, 0, 0, 1));
		gameO[1].render("Press Esc To Exit", glm::vec2(SCREEN_WIDTH / 2 + 90, SCREEN_HEIGHT / 2 + 100), 64, glm::vec4(0, 0, 0, 1));
	}

	if (victoria)
	{
		gameO[0].render("Stage Complete!!!", glm::vec2(SCREEN_WIDTH / 2 + 100, SCREEN_HEIGHT / 2 + 100), 64, glm::vec4(0, 0, 0, 1));
		
		if (auxLvl == 1)
		{
			gameO[1].render("Press " + to_string(auxLvl + 1) + " To Continue", glm::vec2(SCREEN_WIDTH / 2 + 70, SCREEN_HEIGHT / 2 + 180), 64, glm::vec4(0, 0, 0, 1));
		}
		else if (auxLvl == 2)
		{
			gameO[1].render("Press " + to_string(auxLvl + 1) + " To Continue", glm::vec2(SCREEN_WIDTH / 2 + 50, SCREEN_HEIGHT / 2 + 180), 64, glm::vec4(0, 0, 0, 1));
		}
		else 
			gameO[1].render("Congrats!!! Press Esc to restart", glm::vec2(SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 + 200), 64, glm::vec4(0, 0, 0, 1));
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



