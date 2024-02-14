#ifndef PROJECTMANAGER_H
#define PROJECTMANAGER_H
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <vector>
#include "AudioContainer.h"
#include "LightManager.h"

class Program;

class ProjectManager
{
    public:
        ProjectManager(Program & _program,tgui::Gui & _gui, AudioContainer & _container, LightManager & _lightManager);
        void saveAs();
        void save();
        void open();
        void newP();

    protected:

    private:
        Program & program;
        tgui::Gui & gui;
        AudioContainer & container;
        LightManager & lightManager;
        tgui::Filesystem::Path projectPath;
        tgui::Filesystem::Path oldPath;

        void saveAsP(const std::vector<tgui::Filesystem::Path>& paths);
        void openP(const std::vector<tgui::Filesystem::Path>& paths);
        void saveData();
        void loadData();
};

#endif // PROJECTMANAGER_H
