#include "ProjectManager.h"
#include <fstream>

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
        saveData();
    }
}

void ProjectManager::saveAsP(const std::vector<tgui::Filesystem::Path>& paths){
    if(paths.size()==0)
        return ;
    if(paths[0].getFilename().find(".slpp") == std::string::npos)
        return ;
    projectPath = paths[0];
    saveData();
}

void ProjectManager::openP(const std::vector<tgui::Filesystem::Path>& paths){
    if(paths.size()==0)
        return ;
    if(paths[0].getFilename().find(".slpp") == std::string::npos)
        return ;
    projectPath = paths[0];
    loadData();
}

void ProjectManager::saveData(){
    FILE * file = fopen64(projectPath.asString().toStdString().c_str(),"wb");
    Audio * sound;
    Mix_Chunk * sample;
    for(int i=0;i<16;i++)
        for(int j=0;j<64;j++){
            sound = container->getSound(i,j);
            bool loaded = sound->isLoaded();
            fwrite(&loaded,sizeof(bool),1,file);
            if(loaded){
                bool looped = sound->isLooped();
                sample = sound->getSample();
                fwrite(&looped,sizeof(bool),1,file);
                fwrite(&(sample->alen),sizeof(sample->alen),1,file);
                fwrite(sample->abuf,sizeof(Uint8),sample->alen,file);
            }
        }
    fclose(file);
}

void ProjectManager::loadData(){
    FILE * file = fopen64(projectPath.asString().toStdString().c_str(),"rb");
    Audio * sound;
    Mix_Chunk * sample;
    for(int i=0;i<16;i++)
        for(int j=0;j<64;j++){
            sound = container->getSound(i,j);
            sound->clearSample();
            bool loaded;
            fread(&loaded,sizeof(bool),1,file);
            if(loaded){
                bool looped;
                sample = new Mix_Chunk;
                sample->volume = 128;
                sample->allocated = 1;
                fread(&looped,sizeof(bool),1,file);
                fread(&(sample->alen),sizeof(sample->alen),1,file);
                sample->abuf = new Uint8[sample->alen];
                fread(sample->abuf,sizeof(Uint8),sample->alen,file);
                sound->setLooping(looped);
                sound->setSample(sample);
            }
        }
    fclose(file);
}

void ProjectManager::newP(){
    projectPath = tgui::Filesystem::Path();
    for(int i=0;i<16;i++)
        for(int j=0;j<64;j++)
            container->getSound(i,j)->clearSample();
}
