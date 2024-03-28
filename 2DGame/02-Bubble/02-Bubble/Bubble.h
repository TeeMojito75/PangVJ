#ifndef _BUBBLE_INCLUDE
#define _BUBBLE_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


class Bubble
{
public:
<<<<<<< Updated upstream
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, glm::ivec2 sizeB);
=======
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, const glm::ivec2 size);
>>>>>>> Stashed changes
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);

	glm::ivec2 getPosB();
<<<<<<< Updated upstream
=======
	glm::ivec2 getSizeB();
>>>>>>> Stashed changes
	void changeDir();

private:
	glm::ivec2 tileMapDispl, posBubble, size;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;
	bool dir;
	int jumpAngle, startY;
	bool bJumping;
};


#endif  _BUBBLE_INCLUDE
