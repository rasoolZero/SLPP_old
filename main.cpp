#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>

using namespace sf;
using namespace tgui;
int main()
{
    tgui::Theme::setDefault("theme.txt");
    tgui::String const s = U"\u2022";
    RenderWindow window(VideoMode::getDesktopMode(), "SLPP", Style::Fullscreen);
    tgui::Gui gui(window);
    tgui::VerticalLayout::Ptr vl = tgui::VerticalLayout::create();
    for(int i=0;i<8;i++){
       tgui::HorizontalLayout::Ptr hl = tgui::HorizontalLayout::create();
        for(int j=0;j<8;j++){
            tgui::Button::Ptr button = tgui::Button::create();
            if(i==1 || i==6)
                if(j==1 || j==6)
                    button->setText(s);
            if(i==2 || i==5)
                if(j==2 || j==5)
                    button->setText(s);
            hl->add(button,"MainButton"+std::to_string(i)+std::to_string(j));
        }
        for(int j=0;j<=8;j++)
            hl->insertSpace(j*2,0.2);
        vl->add(hl,"HL"+std::to_string(i));
    }
    for(int i=0;i<=8;i++)
        vl->insertSpace(i*2,0.2);
    vl->setSize("50%","100%");
    vl->setPosition("25%","0%");
    gui.add(vl,"VL");


    gui.mainLoop();
    return 0;
}
