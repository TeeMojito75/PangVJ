#ifndef _POWER_INCLUDE
#define _POWER_INCLUDE


#include "Sprite.h"
#include "TileMap.h"



class Power
{

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);

	glm::ivec2 getPosPo();

private:
	glm::ivec2 tileMapDispl, posPower;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;

};


#endif _POWER_INCLUDE