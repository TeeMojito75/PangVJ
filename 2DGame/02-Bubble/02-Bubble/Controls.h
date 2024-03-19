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

#endif // _CONTROLS_INCLUDE