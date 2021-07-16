#include "CButton.h"
void CButton::openFile(){
    tgui::FileDialog::Ptr fileDialog = tgui::FileDialog::create("Open Audio File (.wav,.flac,.ogg)");
    fileDialog->setFileTypeFilters({{"Audio Files (wav,ogg,flac)", {"*.wav", "*.ogg","*.flac"}}});
    fileDialog->setFileMustExist(true);
    fileDialog->onFileSelect(&CButton::fileSelect,this);
    tgui::Container * panel;
    panel = this->getParent();
    while(panel->getWidgetName()!="panel"){
        panel = panel->getParent();
    }

    panel->getParent()->add(fileDialog,"fileDialog");
    panel->setEnabled(false);
}
void CButton::fileSelect(const std::vector<tgui::Filesystem::Path>& paths){
    if(paths.size()>=1){
        sf::String sfStr(paths[0].asString());
//        if(buffer.loadFromFile(sfStr)){
//            AudioLoaded = true;
//
//        }
    }
    tgui::Container * panel;
    panel = this->getParent();
    while(panel->getWidgetName()!="panel"){
        panel = panel->getParent();
    }
    panel->setEnabled(true);
}
