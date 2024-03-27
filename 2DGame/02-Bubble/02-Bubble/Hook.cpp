#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Hook.h"
#include "Game.h"


enum HookAnims
{
	GO_UP
};


void Hook::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	spritesheet.loadFromFile("images/Gancho.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(9, 189), glm::vec2(0.0416666666f, 0.4643734643734644f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(GO_UP, 8);
	sprite->addKeyframe(GO_UP, glm::vec2(0.9583333333333334f, 0.5356265356265356f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posHook.x), float(tileMapDispl.y + posHook.y)));

}

void Hook::update(int deltaTime)
{

	sprite->update(deltaTime);



	if (Game::instance().getKey(GLFW_KEY_LEFT))
	{
		if (sprite->animation() != GO_UP)
			sprite->changeAnimation(GO_UP);
		posHook.x -= 2;
		if (map->collisionMoveLeft(posHook, glm::ivec2(32, 32)))
		{
			posHook.x += 2;
			sprite->changeAnimation(GO_UP);
		}
	}

	else if (Game::instance().getKey(GLFW_KEY_RIGHT))
	{
		if (sprite->animation() != GO_UP)
			sprite->changeAnimation(GO_UP);
		posHook.x += 2;
		if (map->collisionMoveRight(posHook, glm::ivec2(32, 32)))
		{
			posHook.x -= 2;
			sprite->changeAnimation(GO_UP);
		}
	}




	else
	{
		if (sprite->animation() == GO_UP)
			sprite->changeAnimation(GO_UP);

		else if (sprite->animation() == GO_UP)
			sprite->changeAnimation(GO_UP);

		else if (sprite->animation() == GO_UP && map->collisionStairs(posHook, glm::ivec2(32, 32)))
			sprite->changeAnimation(GO_UP);

		else if (sprite->animation() == GO_UP && !map->collisionStairs(posHook, glm::ivec2(32, 32)))
			sprite->changeAnimation(GO_UP);
		else if (sprite->animation() == GO_UP && map->collisionMoveDown(posHook, glm::ivec2(32, 32), &posHook.y))
			sprite->changeAnimation(GO_UP);
	}


	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posHook.x), float(tileMapDispl.y + posHook.y)));
}

void Hook::render()
{
	sprite->render();
}

void Hook::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void Hook::setPosition(const glm::vec2& pos)
{
	posHook = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posHook.x), float(tileMapDispl.y + posHook.y)));
}




