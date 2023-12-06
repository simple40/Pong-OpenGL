#pragma once

#include <irrKlang.h>
#include <iostream>

class AudioManager {
public:
	AudioManager();
	~AudioManager();

	void playSound(std::string audioPath);

private:
	irrklang::ISoundEngine* SoundEngine;
};
