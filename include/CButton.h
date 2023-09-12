#include <TGUI/TGUI.hpp>
#include <vector>


#ifndef CBUTTON_H
#define CBUTTON_H

class Program;

class CButton : public tgui::Button
{
    public:
    void setRowCol(int _row,int _col){row=_row;col=_col;}


    typedef std::shared_ptr<CButton> Ptr;
    typedef std::shared_ptr<const CButton> ConstPtr;
    CButton(Program & _program,const char* typeName = "Button", bool initRenderer = true) :
        tgui::Button(typeName, initRenderer),program(_program)
    {
    }

    static CButton::Ptr create(Program & _program)
    {
        return std::make_shared<CButton>(_program);
    }

    static CButton::Ptr copy(CButton::ConstPtr widget)
    {
        if (widget)
            return std::static_pointer_cast<CButton>(widget->clone());
        else
            return nullptr;
    }
    void openFile();

    void fileSelect(const std::vector<tgui::Filesystem::Path>& paths);

protected:

    Widget::Ptr clone() const override
    {
        return std::make_shared<CButton>(*this);
    }


private:
    int row=0;
    int col=0;
    Program & program;
};

#endif // CBUTTON_H
