#pragma once

namespace irrklang
{
	class ISoundEngine;
}

class AudioEngine
{
public:
	AudioEngine();

	void Play2DSound();

private:
	irrklang::ISoundEngine* IKDevice;
};
