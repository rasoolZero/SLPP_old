#include "LightManager.h"

LightManager::LightManager(MIDI & _midi) : midi(_midi)
{
	animations.resize(16);
	for (auto& page : animations) 
		page.resize(64);

}

void LightManager::trigger(int pageNumber, int row, int column, bool down)
{
	auto index = row * 8 + column;
	if (down)
		animations[pageNumber][index](this->midi);
}

Lights LightManager::getFrameLight(int frameIndex, int pageNumber, int row, int column, int lightIndex)
{
	int index = row * 8 + column;
	return animations[pageNumber][index][frameIndex][lightIndex];
}

void LightManager::setFrameLight(int frameIndex, int pageNumber, int row, int column, int lightIndex, Lights light)
{
	int index = row * 8 + column;
	animations[pageNumber][index][frameIndex][lightIndex] = light;
}

void LightManager::setFrameDuration(int frameIndex, int pageNumber, int row, int column, float duration)
{
	int index = row * 8 + column;
	animations[pageNumber][index][frameIndex].setDuration(sf::seconds(duration));
}

float LightManager::getFrameDuratoin(int frameIndex, int pageNumber, int row, int column)
{
	int index = row * 8 + column;
	return animations[pageNumber][index][frameIndex].getDuration().asSeconds();
}

size_t LightManager::getFrameCount(int pageNumber, int row, int column)
{
	int index = row * 8 + column;
	return animations[pageNumber][index].size();
}
