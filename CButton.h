#include <TGUI/TGUI.hpp>


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

protected:

    Widget::Ptr clone() const override
    {
        return std::make_shared<CButton>(*this);
    }

    private:
};

#endif // CBUTTON_H
