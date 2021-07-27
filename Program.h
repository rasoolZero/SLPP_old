#ifndef PROGRAM_H
#define PROGRAM_H
#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <CButton.h>
#include <AudioContainer.h>
#include <ProjectManager.h>
#include "LightManager.h"
#include <MIDI.h>
#define MENU_HEIGHT 22.f


class Program
{
    enum Operations{save,saveAs,open,newP};
    public:
        Program(tgui::Gui & _gui,sf::RenderWindow * window);
        void run();
        void update();
        void setPageNumber(int pn);
        void trigger(int row,int col,bool down);
        void handleEvent(sf::Event event);
        void load(int row,int col,std::string address);
        void operate(Operations operation);
        void createErrorBox(const char * message);
        void loadedSoundsWindow();
        void lightWindow(int row,int col);

        void disable();
        void enable();

    protected:

    private:
        std::unique_ptr<MIDI> midi;
        std::unique_ptr<AudioContainer> container;
        std::unique_ptr<ProjectManager> manager;
        std::unique_ptr<LightManager> lightManager;
        tgui::Gui & gui;
        void setupLayout();
        void setupButtons();
        void setupPageButtons();
        void setupMenuBar(sf::RenderWindow * window);

        void windowClosed(sf::RenderWindow * window);
        void errorClosed();

        void setupConfigRemoveButton(tgui::ChildWindow::Ptr window,int index);
        void setupConfigLoopButton(tgui::ChildWindow::Ptr window,int index);
        void createConfigWindow(int index,tgui::ChildWindow::Ptr parent);

        void lightPanelClick(int row,int col,int index,tgui::Panel::Ptr panel);

        void removeButtonClick(int index);
        void loopButtonClick(int index);

        bool pollingEvents=false;

        int pageNumber=0;
};

#endif // PROGRAM_H
