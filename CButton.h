#include <TGUI/TGUI.hpp>
#include <SFML/Audio.hpp>
#include <vector>


#ifndef CBUTTON_H
#define CBUTTON_H


class CButton : public tgui::Button
{
    public:

    typedef std::shared_ptr<CButton> Ptr;
    typedef std::shared_ptr<const CButton> ConstPtr;
    CButton(const char* typeName = "Button", bool initRenderer = true) :
        tgui::Button(typeName, initRenderer)
    {
    }

    static CButton::Ptr create()
    {
        return std::make_shared<CButton>();
    }

    static CButton::Ptr copy(CButton::ConstPtr widget)
    {
        if (widget)
            return std::static_pointer_cast<CButton>(widget->clone());
        else
            return nullptr;
    }
    void openFile(){
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

    void fileSelect(const std::vector<tgui::Filesystem::Path>& paths){
        if(paths.size()>=1){
            sf::String sfStr(paths[0].asString());
            if(buffer.loadFromFile(sfStr)){
                AudioLoaded = true;

            }
        }
        tgui::Container * panel;
        panel = this->getParent();
        while(panel->getWidgetName()!="panel"){
            panel = panel->getParent();
        }
        panel->setEnabled(true);
    }

protected:

    Widget::Ptr clone() const override
    {
        return std::make_shared<CButton>(*this);
    }


private:
    sf::SoundBuffer buffer;
    bool AudioLoaded;
};

#endif // CBUTTON_H
