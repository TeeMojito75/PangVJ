#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Bubble.h"
#include "Text.h"
#include "Hook.h"
#include "Power.h"
#include "Food.h"
#include <irrKlang.h>
#include <set>
using namespace irrklang;

// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene
{

public:
	Scene();
	~Scene();

	void init(const int& numLvl, int videsRest);
	void update(int deltaTime);
	void render();
	bool circleRect(float cx, float cy, float radius, float rx, float ry, float rw, float rh);


private:
	void initShaders();

private:
	ISoundEngine* engine;
	TileMap *map;
	Player *player;
	Hook* hook;
	Power* power;
	Food* food;

	Bubble* bubbleBig[2];
	Bubble* bubbleMid[4];
	Bubble* bubbleSmll[8];

	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;
	Texture spritesheet;

	int auxLvl;
	Text text[3];
	Text gameO[2];

	int puntuacio;
	int vides;
	float temps; 

	bool hit;
	bool restart;
	bool over;
	bool write;
	bool music;

	bool invencible;
	
	bool petarB1;
	bool petarB2;
	bool petarM1;
	bool petarM2;
	bool petarM3;
	bool petarM4;

	glm::ivec2 posPaux;
	glm::ivec2 posBaux[14];

protected:
	Sprite* background;
};


#endif  _SCENE_INCLUDE

