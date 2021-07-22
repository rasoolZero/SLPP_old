#include "CButton.h"
#include <Program.h>
void CButton::openFile(){
    tgui::FileDialog::Ptr fileDialog = tgui::FileDialog::create("Open Audio File (.wav .flac .ogg .mp3)");
    fileDialog->setFileTypeFilters({{"Audio Files (wav,ogg,flac,mp3)", {"*.wav", "*.ogg","*.flac","*.mp3"}}});
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
        program.load(row,col,sfStr);
    }
    tgui::Container * panel;
    panel = this->getParent();
    while(panel->getWidgetName()!="panel"){
        panel = panel->getParent();
    }
    panel->setEnabled(true);
}
