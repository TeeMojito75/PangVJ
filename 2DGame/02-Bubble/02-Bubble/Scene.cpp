#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"



#define SCREEN_X 0
#define SCREEN_Y 0

#define INIT_PLAYER_X_TILES 20
#define INIT_PLAYER_Y_TILES 20


Scene::Scene()
{
	map = NULL;
	player = NULL;
	hook = NULL;
	bubble = NULL;
	background = NULL;
	currentTime = NULL;

	puntuaci� = 0;
	vides = 3;
	temps = 180.f;
}

Scene::~Scene()
{
	if(map != NULL)
		delete map;
	if(player != NULL)
		delete player;
	if (hook != NULL)
		delete hook;
	if (bubble != NULL)
		delete bubble;
}


void Scene::init(const int& numLevel)
{
	initShaders();
	spritesheet.loadFromFile("images/BG" + to_string(numLevel) + ".png", TEXTURE_PIXEL_FORMAT_RGBA);
	background = Sprite::createSprite(glm::vec2(384, 208), glm::vec2(1.f, 1.f), &spritesheet, &texProgram);
	background->setPosition(glm::vec2(SCREEN_X, SCREEN_Y));
	map = TileMap::createTileMap("levels/Mapa" + to_string(numLevel) + ".txt", glm::vec2(0.f, 0.f), texProgram);
	
	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	player->setTileMap(map);

	hook = new Hook();
	hook->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	hook->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), 5 * map->getTileSize()));
	hook->setTileMap(map);
	
	bubble = new Bubble();
	bubble->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	bubble->setPosition(glm::vec2(20 * map->getTileSize(), 0 * map->getTileSize()));
	bubble->setTileMap(map);

	for (int i = 0; i < 3; i++) {
		// Select which font you want to use
		//if (!text[i].init("fonts/OpenSans-Regular.ttf"))
		//if(!text.init("fonts/OpenSans-Bold.ttf"))
		if(!text[i].init("fonts/DroidSerif.ttf"))
			cout << "Could not load font!!!" << endl;
	}

	projection = glm::ortho(0.f, 384.f, 248.f, 0.f);
	currentTime = 0.0f;
}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	player->update(deltaTime);
	hook->update(deltaTime);
	bubble->update(deltaTime);
	temps -= 0.012f;
	
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
	bubble->render();
	text[0].render("Puntuacio: " + to_string(puntuaci�), glm::vec2(20, 780), 48, glm::vec4(1, 1, 1, 1));
	text[1].render("Vides: " + to_string(vides), glm::vec2(130, 840), 48, glm::vec4(1, 1, 1, 1));
	text[2].render("Temps: " + to_string(temps), glm::vec2(1085, 780), 48, glm::vec4(1, 1, 1, 1));

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



