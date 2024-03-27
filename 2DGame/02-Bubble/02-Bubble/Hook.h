#ifndef _HOOK_INCLUDE
#define _HOOK_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


class Hook
{

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);

private:
	glm::ivec2 tileMapDispl, posHook;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;

};


#endif // _HOOK_INCLUDE


