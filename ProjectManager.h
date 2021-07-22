#ifndef PROJECTMANAGER_H
#define PROJECTMANAGER_H
#include <TGUI/TGUI.hpp>
#include <vector>
#include <AudioContainer.h>

class ProjectManager
{
    public:
        ProjectManager();
        void setGUI(tgui::Gui * _gui){gui=_gui;}
        void setContainer(AudioContainer * _container){container=_container;}
        void saveAs();
        void save();
        void open();
        void newP();

    protected:

    private:
        tgui::Gui * gui;
        AudioContainer * container;
        tgui::Filesystem::Path projectPath;

        void saveAsP(const std::vector<tgui::Filesystem::Path>& paths);
        void openP(const std::vector<tgui::Filesystem::Path>& paths);
        void saveData();
        void loadData();
};

#endif // PROJECTMANAGER_H
