#pragma once
#include <TGUI/TGUI.hpp>
enum class Lights
{
	Nothing = 0,
	Red = 15,
	Amber = 63,
	Yellow = 62,
	Green = 60,
	Off = 12,
};

tgui::Color light2color(Lights light);