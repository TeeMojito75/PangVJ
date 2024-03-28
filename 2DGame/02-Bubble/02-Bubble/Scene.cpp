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

	invencible = true;
	creacio = false;

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

	hook = new Hook();
	hook->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	hook->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), 5 * map->getTileSize()));
	hook->setTileMap(map);

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
		sound->setVolume(0.05f);
		music = false;
	}
	else if (numLevel == 2 && music)
	{
		irrklang::ISound* sound = engine->play2D("sounds/Stage2.wav", true, false, true);
		sound->setVolume(0.05f);
		music = false;
	}
	else if (numLevel == 3 && music)
	{
		irrklang::ISound* sound = engine->play2D("sounds/Stage3.wav", true, false, true);
		sound->setVolume(0.05f);
		music = false;
	}

	projection = glm::ortho(0.f, 384.f, 248.f, 0.f);
	currentTime = 0.0f;
	
}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	player->update(deltaTime);
	hook->update(deltaTime);

	for (int i = 0; i < 2; i++)
	{
		if (!bubbleBig[i]->getElimina())
			bubbleBig[i]->update(deltaTime);
	}

	if (creacio)
	{
		for (int i = 0; i < 2; i++)
		{
			bubbleMid[i]->update(deltaTime);
		}
	}
	
	if (!restart) temps -= 0.012f;

	posPaux = player->getPosP();

	for (int i = 0; i < 2; i++)
	{
		if (!bubbleBig[i]->getElimina()) {
			posBaux[i] = bubbleBig[i]->getPosB();
		}
		
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

	if (Game::instance().getKey(GLFW_KEY_S))
	{
		creacio = true;
		bubbleMid[0] = new Bubble();
		bubbleMid[0]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, glm::ivec2(16,16));
		bubbleMid[0]->setPosition(posBaux[0]);
		bubbleMid[0]->setTileMap(map);

		bubbleMid[1] = new Bubble();
		bubbleMid[1]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, glm::ivec2(16, 16));
		bubbleMid[1]->setPosition(posBaux[0]);
		bubbleMid[1]->changeDir();
		bubbleMid[1]->setTileMap(map);
		
		bubbleBig[0]->tocada();
	}

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
	hook->render();
	
	for (int i = 0; i < 2; i++)
	{
		if (!bubbleBig[i]->getElimina())
			bubbleBig[i]->render();
	}

	if (creacio)
	{
		for (int i = 0; i < 2; i++)
		{
			bubbleMid[i]->render();
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



