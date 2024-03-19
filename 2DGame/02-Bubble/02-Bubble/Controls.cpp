#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"
#include "Controls.h"



#define SCREEN_X 0
#define SCREEN_Y 0


Controls::Controls()
{
	initShaders();

	glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(384.f, 208.f) };
	glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
	background = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);

	spritesheet[0].loadFromFile("images/Controls.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet[1].loadFromFile("images/Pajaro.png", TEXTURE_PIXEL_FORMAT_RGBA);

	point = Sprite::createSprite(glm::vec2(20, 20), glm::vec2(1.f, 1.f), &spritesheet[1], &texProgram);


	projection = glm::ortho(0.f, 384.f, 208.f, 0.f);
	currentTime = 0.0f;
}

Controls::~Controls()
{

}


void Controls::init()
{
	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);

	texProgram.setUniformMatrix4f("modelview", modelview);
	point->setPosition(glm::vec2(float(65), float(118)));
}

void Controls::update(int deltaTime)
{
	currentTime += deltaTime;
	point->update(deltaTime);
}

void Controls::setPosIndex(int pos) {
	if (pos == 0) point->setPosition(glm::vec2(float(65), float(118)));
	else if (pos == 1) point->setPosition(glm::vec2(float(75), float(147)));
	else if (pos == 2) point->setPosition(glm::vec2(float(114), float(177)));
}

void Controls::render()
{
	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	background->render(spritesheet[0]);
	point->render();
}

void Controls::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if (!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}