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

    protected:

    private:
        tgui::Gui & gui;
};

#endif // PROGRAM_H
