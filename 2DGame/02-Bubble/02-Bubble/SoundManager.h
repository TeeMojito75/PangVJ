#ifndef _SOUND_MANAGER_INCLUDE
#define _SOUND_MANAGER_INCLUDE

#include <irrKlang.h>
using namespace irrklang;

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

// SoundManager is a singleton (a class with a single instance) that represents the sound engine.

class SoundManager {

public:
	SoundManager();
	~SoundManager();
	static SoundManager& instance() {
		static SoundManager SM;
		return SM;
	}

	void init();
	ISoundEngine* getSoundEngine();

private:
	ISoundEngine* engine;
};

#endif // _SOUND_MANAGER_INCLUDE