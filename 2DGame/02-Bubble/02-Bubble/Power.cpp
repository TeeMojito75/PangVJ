#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Power.h"
#include "Game.h"

enum Powers
{
	DOUBLE, GODMODE, TIMESTOP
};


void Power::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	spritesheet.loadFromFile("images/Powers.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(1 / 5, 1 / 2), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(3);

	sprite->setAnimationSpeed(DOUBLE, 8);
	sprite->addKeyframe(DOUBLE, glm::vec2(0.f, 0.f));

	sprite->setAnimationSpeed(GODMODE, 8);
	sprite->addKeyframe(GODMODE, glm::vec2(2 / 5, 0.f));
	sprite->addKeyframe(GODMODE, glm::vec2(3 / 5, 0.f));
	sprite->addKeyframe(GODMODE, glm::vec2(4 / 5, 0.f));
	sprite->addKeyframe(GODMODE, glm::vec2(0 / 5, 0.5f));
	sprite->addKeyframe(GODMODE, glm::vec2(1 / 5, 0.5f));
	sprite->addKeyframe(GODMODE, glm::vec2(2 / 5, 0.5f));
	sprite->addKeyframe(GODMODE, glm::vec2(3 / 5, 0.5f));
	sprite->addKeyframe(GODMODE, glm::vec2(4 / 5, 0.5f));

	sprite->setAnimationSpeed(TIMESTOP, 8);
	sprite->addKeyframe(TIMESTOP, glm::vec2(1 / 5, 0.f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPower.x), float(tileMapDispl.y + posPower.y)));

}

void Power::update(int deltaTime)
{

	sprite->update(deltaTime);
	posPower.y -= 1;
	if (map->collisionMoveDown(posPower, glm::ivec2(32, 32), &posPower.y))
	{
		posPower.y += 1;
	}
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPower.x), float(tileMapDispl.y + posPower.y)));
}

void Power::render()
{
	sprite->render();
}

void Power::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void Power::setPosition(const glm::vec2& pos)
{
	posPower = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPower.x), float(tileMapDispl.y + posPower.y)));
}

glm::ivec2 Power::getPosPo()
{
	return posPower;
}