#ifndef _CONTROLS_INCLUDE
#define _CONTROLS_INCLUDE

#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TexturedQuad.h"
#include "Sprite.h"


class Controls {
public:
	Controls();
	~Controls();

	void init();
	void update(int deltaTime);
	void render();

private:
	void initShaders();

private:
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;
	Texture spritesheet;
protected:
	TexturedQuad* background;
};

#endif // _CONTROLS_INCLUDE