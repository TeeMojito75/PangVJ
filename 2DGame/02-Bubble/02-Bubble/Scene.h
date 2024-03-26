#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Bubble.h"
#include "Text.h"

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

private:
	void initShaders();

private:
	TileMap *map;
	Player *player;
	Bubble* bubble;
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;
	Texture spritesheet;

	int auxLvl;
	
	Text text[3];
	Text gameO[2];

	int puntuaci�;
	int vides;
	float temps; 

	bool hit;
	bool restart;
	bool over;

	glm::ivec2 posPaux;
	glm::ivec2 posBaux;
protected:
	Sprite* background;
};


#endif // _SCENE_INCLUDE

