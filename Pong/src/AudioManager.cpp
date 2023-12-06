#include "AudioManager.h"

AudioManager::AudioManager()
{
	 SoundEngine = irrklang::createIrrKlangDevice();
}

AudioManager::~AudioManager()
{

}

void AudioManager::playSound(std::string audioPath)
{
	SoundEngine->play2D(audioPath.c_str(), false);
}
