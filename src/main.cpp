#include "Program.h"
#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>

int main()
{
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "SLPP", sf::Style::Fullscreen);
    sf::Image icon;
    icon.loadFromFile("themes/Icon.png");
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    tgui::Gui gui(window);

    Program p(gui,& window);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            gui.handleEvent(event);
            p.handleEvent(event);
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
