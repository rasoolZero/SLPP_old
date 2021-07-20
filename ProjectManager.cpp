#include "ProjectManager.h"

ProjectManager::ProjectManager()
{

}

void ProjectManager::saveAs(){
    tgui::FileDialog::Ptr fileDialog = tgui::FileDialog::create("Save Project As");
    fileDialog->setFileTypeFilters({{"SLPP Project (.slpp)", {"*.slpp"}}});
    fileDialog->setFilename("project.slpp");
    fileDialog->setConfirmButtonText("Save");
    fileDialog->setFileMustExist(false);
    fileDialog->setMultiSelect(false);
    fileDialog->onFileSelect(&ProjectManager::saveAsP,this);
    gui->add(fileDialog,"saveAsFileDialog");
}

void ProjectManager::open(){
    tgui::FileDialog::Ptr fileDialog = tgui::FileDialog::create("Open Project");
    fileDialog->setFileTypeFilters({{"SLPP Project (.slpp)", {"*.slpp"}}});
    fileDialog->setFileMustExist(true);
    fileDialog->setMultiSelect(false);
    fileDialog->onFileSelect(&ProjectManager::openP,this);
    gui->add(fileDialog,"openFileDialog");
}

void ProjectManager::save(){
    if(projectPath.isEmpty()){
        saveAs();
    }
    else{
        //saveData()
    }
}

void ProjectManager::saveAsP(const std::vector<tgui::Filesystem::Path>& paths){
    if(paths.size()==0)
        return ;
    if(paths[0].getFilename().find(".slpp") == std::string::npos)
        return ;
    projectPath = paths[0];
    //saveData();
}

void ProjectManager::openP(const std::vector<tgui::Filesystem::Path>& paths){
    if(paths.size()==0)
        return ;
    if(paths[0].getFilename().find(".slpp") == std::string::npos)
        return ;
    projectPath = paths[0];
    //loadData();
}
