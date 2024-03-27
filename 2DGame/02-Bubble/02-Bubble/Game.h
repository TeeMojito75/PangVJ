#ifndef _GAME_INCLUDE
#define _GAME_INCLUDE

#include "Menu.h"
#include "Controls.h"
#include "Levels.h"
#include <GLFW/glfw3.h>
#include "Scene.h"
#include "SoundManager.h"


#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480


// Game is a singleton (a class with a single instance) that represents our whole application


class Game
{

private:
	Game() {}
	
public:
	static Game &instance()
	{
		static Game G;
	
		return G;
	}
	
	void init();
	bool update(int deltaTime);
	void render();
	
	// Input callback methods
	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMove(int x, int y);
	void mousePress(int button);
	void mouseRelease(int button);

	bool getKey(int key) const;

private:
	ISoundEngine* engine;
	Menu* menu;
	Controls* controls;
	Levels* levels;
	Scene* scene;
	int numLevel, posIndex, view;
	bool start, map, songPlaying;
	bool bPlay; // Continue to play game?
	bool keys[GLFW_KEY_LAST+1]; // Store key states so that 
							    // we can have access at any time

};


#endif // _GAME_INCLUDE


