#ifndef _BUBBLE_INCLUDE
#define _BUBBLE_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


class Bubble
{
public:

	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, glm::ivec2 sizeB);

	void update(int deltaTime, bool slow);
	void render();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);

	glm::ivec2 getPosB();

	glm::ivec2 getSizeB();

	void changeDir();
	void tocada();

	bool getElimina();
	

private:
	glm::ivec2 tileMapDispl, posBubble, size;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;
	bool dir;
	int jumpAngle, startY;
	bool bJumping;
	bool eliminar;
};


#endif  _BUBBLE_INCLUDE
