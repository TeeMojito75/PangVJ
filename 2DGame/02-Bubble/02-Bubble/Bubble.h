#ifndef _BUBBLE_INCLUDE
#define _BUBBLE_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


class Bubble
{

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);

	glm::ivec2 getPosB();

private:
	glm::ivec2 tileMapDispl, posBubble;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;
	bool dir;
	int jumpAngle, startY;
	bool bJumping;
};


#endif // _BUBBLE_INCLUDE
