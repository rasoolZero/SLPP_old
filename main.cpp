#include <MIDI.h>
#include <Program.h>
#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>

int main()
{
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "SLPP", sf::Style::Fullscreen);
    tgui::Gui gui(window);

    Program p(gui);

    std::unique_ptr<MIDI> midi;
    try{
        midi = std::make_unique<MIDI>();
    }
    catch(std::runtime_error & e){
        std::cout << e.what();
        return 1;
    }

    p.run();
    return 0;
}
