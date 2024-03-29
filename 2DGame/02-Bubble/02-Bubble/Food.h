#ifndef _FOOD_INCLUDE
#define _FOOD_INCLUDE


#include "Sprite.h"
#include "TileMap.h"



class Food
{

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, int type);
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);

	glm::ivec2 getPosF();
	int getPoints();

private:
	glm::ivec2 tileMapDispl, posFood;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;
	int points;

};


#endif _FOOD_INCLUDE