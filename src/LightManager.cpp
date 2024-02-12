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
