#ifndef _LEVELS_INCLUDE
#define _LEVELS_INCLUDE

#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TexturedQuad.h"
#include "Sprite.h"


class Levels {
public:
	Levels();
	~Levels();

	void init();
	void update(int deltaTime);
	void render();
	void setPosIndex(int pos);

private:
	void initShaders();

private:
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;
	Texture spritesheet[2];
protected:
	TexturedQuad* background;
	Sprite* point;
};

#endif // _LEVELS_INCLUDE