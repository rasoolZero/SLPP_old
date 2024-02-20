#include "LightManager.h"

LightManager::LightManager(MIDI & _midi) : midi(_midi), currentLightState(_midi)
{
	animations.resize(16);
	for (auto& page : animations) 
		page.resize(64);

}

void LightManager::trigger(int pageNumber, int row, int column, bool down)
{
	auto index = row * 8 + column;
	if (down)
		animations[pageNumber][index](this->currentLightState);
}

Lights LightManager::getFrameLight(int frameIndex, int pageNumber, int buttonIndex, int lightIndex)
{
	return animations[pageNumber][buttonIndex][frameIndex][lightIndex];
}

void LightManager::setFrameLight(int frameIndex, int pageNumber, int buttonIndex, int lightIndex, Lights light)
{
	animations[pageNumber][buttonIndex][frameIndex][lightIndex] = light;
}

void LightManager::setFrameDuration(int frameIndex, int pageNumber, int buttonIndex, float duration)
{
	animations[pageNumber][buttonIndex][frameIndex].setDuration(sf::seconds(duration));
}

float LightManager::getFrameDuratoin(int frameIndex, int pageNumber, int buttonIndex)
{
	return animations[pageNumber][buttonIndex][frameIndex].getDuration().asSeconds();
}

size_t LightManager::getFrameCount(int pageNumber, int buttonIndex)
{
	return animations[pageNumber][buttonIndex].size();
}

int LightManager::newFrame(int pageNumber, int buttonIndex)
{
	animations[pageNumber][buttonIndex].push_back(LightFrame());
	return getFrameCount(pageNumber, buttonIndex);
}
