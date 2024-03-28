#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Hook.h"
#include "Game.h"



void Hook::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	spritesheet.loadFromFile("images/Gancho2.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(9, 187), glm::vec2(1 / 23.f, 1 / 3.f), &spritesheet, &shaderProgram);
	highLength = 34;
	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(0, 32);

	for (int i = 0; i < 23; i++) {
		sprite->addKeyframe(0, glm::vec2(i / 23.f, 0.f));
	}
	for (int i = 0; i < 23; i++) {
		sprite->addKeyframe(0, glm::vec2(i / 23.f, 1 / 3.f));
	}
	for (int i = 0; i < 23; i++) {
		sprite->addKeyframe(0, glm::vec2(i / 23.f, 2 / 3.f));
	}

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posHook.x), float(tileMapDispl.y + posHook.y)));

}

void Hook::update(int deltaTime)
{
	highLength += 1.15;
	sprite->update(deltaTime);
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

glm::ivec2 Hook::getPosHook()
{
	return posHook;
}

float Hook::getHighLength()
{
	return highLength;
}