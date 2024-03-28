#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Food.h"
#include "Game.h"

enum Foods
{
	SHUSHI, FRIES, BANANA
};


void Food::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, int type)
{
	spritesheet.loadFromFile("images/Food.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.3333333333333333333333333333333, 1), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(3);

	sprite->setAnimationSpeed(SHUSHI, 8);
	sprite->addKeyframe(SHUSHI, glm::vec2(0.f, 0.f));

	sprite->setAnimationSpeed(FRIES, 8);
	sprite->addKeyframe(FRIES, glm::vec2(0.33333333333333333333333333, 0.f));

	sprite->setAnimationSpeed(BANANA, 8);
	sprite->addKeyframe(BANANA, glm::vec2(0.66666666666666666666666666, 0.f));

	sprite->changeAnimation(type);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posFood.x), float(tileMapDispl.y + posFood.y)));

}

void Food::update(int deltaTime)
{

	sprite->update(deltaTime);
	posFood.y += 1;
	if (map->collisionMoveDown(posFood, glm::ivec2(32, 24), &posFood.y))
	{
		posFood.y -= 1;
	}
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posFood.x), float(tileMapDispl.y + posFood.y)));
}

void Food::render()
{
	sprite->render();
}

void Food::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void Food::setPosition(const glm::vec2& pos)
{
	posFood = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posFood.x), float(tileMapDispl.y + posFood.y)));
}

glm::ivec2 Food::getPosF()
{
	return posFood;
}