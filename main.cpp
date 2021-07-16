#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <CButton.h>
#include <MIDI.h>

int main()
{
    tgui::Theme::setDefault("theme.txt");
    tgui::String const s = U"\u2022";
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "SLPP", sf::Style::Fullscreen);
    tgui::Gui gui(window);
    tgui::VerticalLayout::Ptr vl = tgui::VerticalLayout::create();
    for(int i=0;i<8;i++){
       tgui::HorizontalLayout::Ptr hl = tgui::HorizontalLayout::create();
        for(int j=0;j<8;j++){
            CButton::Ptr button = CButton::create();
            if(i==1 || i==6)
                if(j==1 || j==6)
                    button->setText(s);
            if(i==2 || i==5)
                if(j==2 || j==5)
                    button->setText(s);
            button->onClick(&CButton::openFile,button);
            hl->add(button,"mainButton"+std::to_string(i)+std::to_string(j));
        }
        for(int j=0;j<=8;j++)
            hl->insertSpace(j*2,0.2);
        vl->add(hl,"HL"+std::to_string(i));
    }
    for(int i=0;i<=8;i++)
        vl->insertSpace(i*2,0.2);
    vl->setSize("50%","100%");
    vl->setPosition("25%","0%");
    tgui::Panel::Ptr panel = tgui::Panel::create();
    panel->add(vl,"VL");
    gui.add(panel,"panel");
    MIDI midi;

    gui.mainLoop();
    return 0;
}
