#include <MIDI.h>
#include <Program.h>
#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>

int main()
{
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "SLPP", sf::Style::Fullscreen);
    tgui::Gui gui(window);

    Program p(gui,& window);

    std::unique_ptr<MIDI> midi;
    try{
        midi = std::make_unique<MIDI>(p);
    }
    catch(std::runtime_error & e){
        std::cout << e.what();
        return 1;
    }

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            gui.handleEvent(event);
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed)
                if (event.key.code == sf::Keyboard::Escape)
                    window.close();
        }
//        window.clear(clr);
        p.update();
        window.display();
    }
    return 0;
}
