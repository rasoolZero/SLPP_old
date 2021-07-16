#ifndef PROGRAM_H
#define PROGRAM_H
#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <CButton.h>
#include <AudioContainer.h>


class Program
{
    public:
        Program(tgui::Gui & _gui);
        void run();
        void update();
        void setPageNumber(int pn);
        void trigger(int row,int col,bool down);


    protected:

    private:
        AudioContainer container;
        tgui::Gui & gui;
        void setupLayout();
        void setupButtons();
        void setupPageButtons();
        int pageNumber=0;
};

#endif // PROGRAM_H
