#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Power.h"
#include "Game.h"

enum Powers
{
	SLOW, GODMODE, TIMESTOP
};


void Power::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, int typeF)
{
	spritesheet.loadFromFile("images/Powers.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.2f , 0.5f ), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(3);

	sprite->setAnimationSpeed(SLOW, 8);
	sprite->addKeyframe(SLOW, glm::vec2(0.f, 0.f));

	sprite->setAnimationSpeed(GODMODE, 16);
	sprite->addKeyframe(GODMODE, glm::vec2(0.4f, 0.f));
	sprite->addKeyframe(GODMODE, glm::vec2(0.6f, 0.f));
	sprite->addKeyframe(GODMODE, glm::vec2(0.8f, 0.f));
	sprite->addKeyframe(GODMODE, glm::vec2(0.f, 0.5f));
	sprite->addKeyframe(GODMODE, glm::vec2(0.2f, 0.5f));
	sprite->addKeyframe(GODMODE, glm::vec2(0.4f, 0.5f));
	sprite->addKeyframe(GODMODE, glm::vec2(0.6f, 0.5f));
	sprite->addKeyframe(GODMODE, glm::vec2(0.8f, 0.5f));

	sprite->setAnimationSpeed(TIMESTOP, 8);
	sprite->addKeyframe(TIMESTOP, glm::vec2(0.2f, 0.f));

	sprite->changeAnimation(typeF);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPower.x), float(tileMapDispl.y + posPower.y)));
	type = typeF;
}

void Power::update(int deltaTime)
{

	sprite->update(deltaTime);
	if (!map->collisionMoveDown(posPower, glm::ivec2(16, 16), &posPower.y))
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

int Power::getType()
{
	return type;
}