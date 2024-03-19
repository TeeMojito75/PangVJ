#ifndef _MENU_INCLUDE
#define _MENU_INCLUDE

#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TexturedQuad.h"
#include "Sprite.h"


class Menu {
public:
	Menu();
	~Menu();

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

#endif // _MENU_INCLUDE