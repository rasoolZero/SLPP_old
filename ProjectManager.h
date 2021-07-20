#ifndef PROJECTMANAGER_H
#define PROJECTMANAGER_H
#include <TGUI/TGUI.hpp>
#include <vector>

class ProjectManager
{
    public:
        ProjectManager();
        void setGUI(tgui::Gui * _gui){gui=_gui;}
        void saveAs();
        void save();
        void open();

    protected:

    private:
        tgui::Gui * gui;
        tgui::Filesystem::Path projectPath;

        void saveAsP(const std::vector<tgui::Filesystem::Path>& paths);
        void openP(const std::vector<tgui::Filesystem::Path>& paths);
};

#endif // PROJECTMANAGER_H
