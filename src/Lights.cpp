#include "Lights.h"

tgui::Color light2color(Lights light) {
	if (light == Lights::Nothing)
		return tgui::Color::Black;
	if (light == Lights::Off)
		return tgui::Color(120, 120, 120);
	if (light == Lights::Green)
		return tgui::Color::Green;
	if (light == Lights::Red)
		return tgui::Color::Red;
	if (light == Lights::Amber)
		return tgui::Color(255, 165, 0);
	if (light == Lights::Yellow)
		return tgui::Color::Yellow;
	return tgui::Color::Black;
}