#ifndef PROGRAM_H
#define PROGRAM_H
#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <CButton.h>
#include <AudioContainer.h>
#include <ProjectManager.h>
#include <MIDI.h>
#define MENU_HEIGHT 22.f


class Program
{
    public:
        Program(tgui::Gui & _gui,sf::RenderWindow * window);
        void run();
        void update();
        void setPageNumber(int pn);
        void trigger(int row,int col,bool down);
        void handleEvent(sf::Event event);
        void createConfigWindow(int index);
        void load(int row,int col,std::string address);


    protected:

    private:
        std::unique_ptr<MIDI> midi;
        std::unique_ptr<AudioContainer> container;
        tgui::Gui & gui;
        ProjectManager manager;
        void setupLayout();
        void setupButtons();
        void setupPageButtons();
        void setupMenuBar(sf::RenderWindow * window);

        void windowClosed(sf::RenderWindow * window);

        void setupConfigRemoveButton(tgui::ChildWindow::Ptr window,int index);
        void setupConfigLoopButton(tgui::ChildWindow::Ptr window,int index);

        void removeButtonClick(int index);
        void loopButtonClick(int index);

        bool loaded=false;

        int pageNumber=0;
};

#endif // PROGRAM_H
