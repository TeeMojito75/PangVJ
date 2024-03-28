#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Bubble.h"
#include "Game.h"

#define JUMP_ANGLE_STEP 3
#define JUMP_HEIGHT 86
#define FALL_STEP 2





void Bubble::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram,  glm::ivec2 sizeB)
{
	spritesheet.loadFromFile("images/Boles.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(sizeB, glm::vec2(0.46, 0.33), &spritesheet, &shaderProgram);
	size = sizeB;
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBubble.x), float(tileMapDispl.y + posBubble.y)));
	dir = false;
	bJumping = false;
	eliminar = false;

}

void Bubble::update(int deltaTime)
{
	sprite->update(deltaTime);
	
		if (dir == false)
		posBubble.x -= 2;
		if (map->collisionMoveLeft(posBubble, size))
		{
			posBubble.x += 2;
			dir = !dir;
		}
	
		if (dir == true)
		posBubble.x += 2;
		if (map->collisionMoveRight(posBubble, size))
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
				posBubble.y = int(startY - 86 * sin(3.14159f * jumpAngle / 180.f));
				if (jumpAngle > 90);
				bJumping = (!map->collisionMoveDown(posBubble, size, &posBubble.y)
						&& !map->collisionRoof(posBubble, size));
			}
		}
		else
		{
			posBubble.y += FALL_STEP;
			if (map->collisionMoveDown(posBubble, size, &posBubble.y))
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

glm::ivec2 Bubble::getPosB()
{
	return posBubble;
}


void Bubble::changeDir()
{
	dir = !dir;
}

void Bubble::tocada()
{
	eliminar = true;
}

bool Bubble::getElimina()
{
	return eliminar;
}

glm::ivec2 Bubble::getSizeB()
{
	return size;
}


