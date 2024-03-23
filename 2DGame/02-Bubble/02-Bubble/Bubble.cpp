#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Bubble.h"
#include "Game.h"

#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4


void Bubble::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	spritesheet.loadFromFile("images/Boles.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.46, 0.33), &spritesheet, &shaderProgram);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBubble.x), float(tileMapDispl.y + posBubble.y)));
	dir = false;
	bJumping = false;

}

void Bubble::update(int deltaTime)
{
	sprite->update(deltaTime);
	
		if (dir == false)
		posBubble.x -= 2;
		if (map->collisionMoveLeft(posBubble, glm::ivec2(32, 32)))
		{
			posBubble.x += 2;
			dir = !dir;
		}

	
	
		if (dir == true)
		posBubble.x += 2;
		if (map->collisionMoveRight(posBubble, glm::ivec2(32, 32)))
		{
			posBubble.x -= 2;
			dir = !dir;
		}

		if (bJumping)
		{
			jumpAngle += JUMP_ANGLE_STEP;
			if (jumpAngle == 180)
			{
				bJumping = false;
				posBubble.y = startY;
			}
			else
			{
				posBubble.y = int(startY - 96 * sin(3.14159f * jumpAngle / 180.f));
				if (jumpAngle > 90)
					bJumping = !map->collisionMoveDown(posBubble, glm::ivec2(32, 32), &posBubble.y);
			}
		}
		else
		{
			posBubble.y += FALL_STEP;
			if (map->collisionMoveDown(posBubble, glm::ivec2(32, 32), &posBubble.y))
			{
					bJumping = true;
					jumpAngle = 0;
					startY = posBubble.y;
			}
		}
	

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBubble.x), float(tileMapDispl.y + posBubble.y)));
}

void Bubble::render()
{
	sprite->render();
}

void Bubble::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void Bubble::setPosition(const glm::vec2& pos)
{
	posBubble = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBubble.x), float(tileMapDispl.y + posBubble.y)));
}