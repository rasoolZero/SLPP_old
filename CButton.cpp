#include "CButton.h"
#include "Program.h"
#include <dirent.h>
void CButton::openFile(){
    tgui::FileDialog::Ptr fileDialog = tgui::FileDialog::create("Open Audio File (.wav .flac .ogg .mp3)");
    fileDialog->setFileTypeFilters({{"Audio Files (wav,ogg,flac,mp3)", {"*.wav", "*.ogg","*.flac","*.mp3"}}});
    fileDialog->setFileMustExist(true);
    fileDialog->onFileSelect(&CButton::fileSelect,this);
    program.disable();
    this->getParentGui()->add(fileDialog,"fileDialog");
}
void CButton::fileSelect(const std::vector<tgui::Filesystem::Path>& paths){
    program.enable();
    if(paths.size()>=1){
        chdir(paths[0].getParentPath().asString().toStdString().c_str());
        sf::String sfStr(paths[0].asString());
        program.load(row,col,sfStr);
    }
}
