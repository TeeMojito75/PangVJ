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

private:
	void initShaders();

private:
	ISoundEngine* engine;
	TileMap *map;
	Player *player;
	Hook* hook;
<<<<<<< Updated upstream

	Bubble* bubbleBig[2];
	Bubble* bubbleMid[2];

=======
	Bubble* bubble[10];
>>>>>>> Stashed changes
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
	bool creacio;

	glm::ivec2 posPaux;
	glm::ivec2 posBaux[2];

protected:
	Sprite* background;
};


#endif // _SCENE_INCLUDE

