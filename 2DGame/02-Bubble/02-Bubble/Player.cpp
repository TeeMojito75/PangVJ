#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Player.h"
#include "Game.h"

#define FALL_STEP 2

enum PlayerAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT, STAND_STAIRS, MOVE_STAIRS
};


void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	spritesheet.loadFromFile("images/Pj1completo.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(28, 28), glm::vec2(0.25, 0.20), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(6);
	
		sprite->setAnimationSpeed(STAND_LEFT, 8);
		sprite->addKeyframe(STAND_LEFT, glm::vec2(0.5f, 0.4f));
		
		sprite->setAnimationSpeed(STAND_RIGHT, 8);
		sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.5f, 0.f));

		sprite->setAnimationSpeed(STAND_STAIRS, 8);
		sprite->addKeyframe(STAND_STAIRS, glm::vec2(0.5f, 0.8f));

		//No fa l'animació sencera i no sé perquè
		sprite->setAnimationSpeed(MOVE_STAIRS, 12);
		sprite->addKeyframe(MOVE_STAIRS, glm::vec2(0.75f, 0.f));
		sprite->addKeyframe(MOVE_STAIRS, glm::vec2(0.75f, 0.2f));
		sprite->addKeyframe(MOVE_STAIRS, glm::vec2(0.75f, 0.4f));
		sprite->addKeyframe(MOVE_STAIRS, glm::vec2(0.75f, 0.6f));
		
		sprite->setAnimationSpeed(MOVE_LEFT, 8);
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.2f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.4f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.6f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.8f));
		
		sprite->setAnimationSpeed(MOVE_RIGHT, 8);
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25f, 0.f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25f, 0.2f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25f, 0.4f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25f, 0.6f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25f, 0.8f));
		
	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	
}

void Player::update(int deltaTime)
{
	sprite->update(deltaTime);
	if(Game::instance().getKey(GLFW_KEY_LEFT))
	{
		if(sprite->animation() != MOVE_LEFT)
			sprite->changeAnimation(MOVE_LEFT);
		posPlayer.x -= 2;
		if(map->collisionMoveLeft(posPlayer, glm::ivec2(28, 28)))
		{
			posPlayer.x += 2;
			sprite->changeAnimation(STAND_LEFT);
		}
	}
	else if(Game::instance().getKey(GLFW_KEY_RIGHT))
	{
		if(sprite->animation() != MOVE_RIGHT)
			sprite->changeAnimation(MOVE_RIGHT);
		posPlayer.x += 2;
		if(map->collisionMoveRight(posPlayer, glm::ivec2(28, 28)))
		{
			posPlayer.x -= 2;
			sprite->changeAnimation(STAND_RIGHT);
		}
	}
	else
	{
		if(sprite->animation() == MOVE_LEFT)
			sprite->changeAnimation(STAND_LEFT);
		else if(sprite->animation() == MOVE_RIGHT)
			sprite->changeAnimation(STAND_RIGHT);
	}
	
	if (!map->collisionMoveDown(posPlayer, glm::ivec2(28, 28), &posPlayer.y) 
		&& !map->collisionStairs(posPlayer, glm::ivec2(28, 28), &posPlayer.y)
		&& !map->collisionSpecStairs(posPlayer, glm::ivec2(28, 28), &posPlayer.y))
	{
		posPlayer.y += FALL_STEP;
	}

	if(map->collisionMoveDown(posPlayer, glm::ivec2(28, 28), &posPlayer.y))
	{
			
	}
	
	//Falta aconseguir funcionalitat amb escales de gel
	if (map->collisionStairs(posPlayer, glm::ivec2(28, 28), &posPlayer.y) || map->collisionSpecStairs(posPlayer, glm::ivec2(28, 28), &posPlayer.y))
	{
		if (sprite->animation() != STAND_STAIRS) sprite->changeAnimation(STAND_STAIRS);

		if (Game::instance().getKey(GLFW_KEY_UP))
		{
			if (sprite->animation() != MOVE_STAIRS) sprite->changeAnimation(MOVE_STAIRS);
			posPlayer.y -= 2;
		}

		if (Game::instance().getKey(GLFW_KEY_DOWN))
		{
			if (sprite->animation() != MOVE_STAIRS) sprite->changeAnimation(MOVE_STAIRS);
			posPlayer.y += 2;
		}
	}

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Player::render()
{
	sprite->render();
}

void Player::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Player::setPosition(const glm::vec2 &pos)
{
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}




