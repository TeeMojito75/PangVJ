#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Bubble.h"
#include "Text.h"
#include "Hook.h"
#include <irrKlang.h>
using namespace irrklang;

// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene
{

public:
	Scene();
	~Scene();

	void init(const int& numLvl);
	void update(int deltaTime);
	void render();

private:
	void initShaders();

private:
	ISoundEngine* engine;
	TileMap *map;
	Player *player;
	Hook* hook;
	Bubble* bubble;
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;
	Texture spritesheet;
	Text text[3];
	int puntuació;
	int vides;
	float temps; 
protected:
	Sprite* background;
};


#endif // _SCENE_INCLUDE

