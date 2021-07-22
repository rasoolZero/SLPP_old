#include "CButton.h"
#include <Program.h>
void CButton::openFile(){
    tgui::FileDialog::Ptr fileDialog = tgui::FileDialog::create("Open Audio File (.wav .flac .ogg .mp3)");
    fileDialog->setFileTypeFilters({{"Audio Files (wav,ogg,flac,mp3)", {"*.wav", "*.ogg","*.flac","*.mp3"}}});
    fileDialog->setFileMustExist(true);
    fileDialog->onFileSelect(&CButton::fileSelect,this);
    tgui::Container * cont;
    cont = this->getParent();
    while(cont->getWidgetName()!="panel"){
        cont = cont->getParent();
    }

    program.disable();
    cont->getParent()->add(fileDialog,"fileDialog");
}
void CButton::fileSelect(const std::vector<tgui::Filesystem::Path>& paths){
    program.enable();
    if(paths.size()>=1){
        sf::String sfStr(paths[0].asString());
        program.load(row,col,sfStr);
    }
}
