#ifndef PROGRAM_H
#define PROGRAM_H
#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <CButton.h>


class Program
{
    public:
        Program(tgui::Gui & _gui);
        void run();
        void setPageNumber(int pn);


    protected:

    private:
        tgui::Gui & gui;
        void setupLayout();
        void setupButtons();
        void setupPageButtons();
        int pageNumber=0;
};

#endif // PROGRAM_H
