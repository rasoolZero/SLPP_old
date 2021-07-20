#include "Program.h"

Program::Program(tgui::Gui & _gui,sf::RenderWindow * window) : gui(_gui)
{
    manager.setGUI(&gui);
    manager.setContainer(&container);
    tgui::Theme::setDefault("theme.txt");
    setupLayout();
    setupButtons();
    setupPageButtons();
    setupMenuBar(window);
    setPageNumber(0);
}

void Program::run(){
    gui.mainLoop();
}
void Program::update(){
    gui.draw();
}

void Program::setupLayout(){
    tgui::VerticalLayout::Ptr vl = tgui::VerticalLayout::create();
    for(int i=0;i<=8;i++){
        tgui::HorizontalLayout::Ptr hl = tgui::HorizontalLayout::create();
        vl->add(hl,"HL"+std::to_string(i));
    }
    for(int i=1;i<=9;i++)
        vl->insertSpace(i*2,0.2);
    vl->insertSpace(1,0.4);
    vl->insertSpace(0,0.2);
    vl->setSize("50%","100%");
    vl->setPosition("25%","0%");
    tgui::Panel::Ptr panel = tgui::Panel::create();
    panel->add(vl,"VL");
    panel->setPosition(0,MENU_HEIGHT);
    panel->setSize(gui.getTarget()->getSize().x,gui.getTarget()->getSize().y-MENU_HEIGHT);
    gui.add(panel,"panel");
}
void Program::setupButtons(){
    tgui::String const s = U"\u2022";
    for(int i=0;i<8;i++){
        tgui::HorizontalLayout::Ptr hl = gui.get<tgui::HorizontalLayout>("HL"+std::to_string(i+1));
        for(int j=0;j<8;j++){
            CButton::Ptr button = CButton::create();
            if(i==1 || i==6)
                if(j==1 || j==6)
                    button->setText(s);
            if(i==2 || i==5)
                if(j==2 || j==5)
                    button->setText(s);
            button->setRowCol(i,j);
            button->setContainer(&container);
            button->onClick(&CButton::openFile,button);
            hl->add(button,"mainButton"+std::to_string(i)+std::to_string(j));
            hl->insertSpace(j*2,0.2);
        }
    }

}
void Program::setupPageButtons(){
    tgui::HorizontalLayout::Ptr hl = gui.get<tgui::HorizontalLayout>("HL0");
    for(int i=0;i<8;i++){
        CButton::Ptr button = CButton::create();
        button->setRenderer(tgui::Theme::getDefault()->getRenderer("PageButton"));
        button->onClick(&Program::setPageNumber,this,i);
        hl->add(button,"PageButton"+std::to_string(i));
        hl->insertSpace(i*2,0.2);
    }
    CButton::Ptr button = CButton::create();
//    button->setVisible(false);
    button->setText("Stop All\nPlaying\nSounds");
    button->setTextSize(0);
    button->onClick(&AudioContainer::stopAll,&container);
    hl->add(button);
    hl->insertSpace(16,0.4);

    for(int i=0;i<8;i++){
        hl = gui.get<tgui::HorizontalLayout>("HL"+std::to_string(i+1));
        CButton::Ptr button = CButton::create();
        button->setRenderer(tgui::Theme::getDefault()->getRenderer("PageButton"));
        button->onClick(&Program::setPageNumber,this,i+8);
        hl->add(button,"PageButton"+std::to_string(i+8));
        hl->insertSpace(16,0.4);
    }

}


void Program::setPageNumber(int pn){
    if(pn<0)
        pn=0;
    if(pn>15)
        pn=15;

    gui.get<tgui::Button>("PageButton"+std::to_string(pageNumber))->setRenderer(tgui::Theme::getDefault()->getRenderer("PageButton"));
    pageNumber=pn;
    gui.get<tgui::Button>("PageButton"+std::to_string(pn))->setRenderer(tgui::Theme::getDefault()->getRenderer("PageButtonSelected"));
    container.setPageNumber(pageNumber);
}

void Program::trigger(int row,int col,bool down){
    container.trigger(row,col,down);
    gui.get<CButton>("mainButton"+std::to_string(row)+std::to_string(col))->setEnabled(!down);
}

void Program::setupMenuBar(sf::RenderWindow * window){
    tgui::MenuBar::Ptr menu = tgui::MenuBar::create();
    menu->setHeight(MENU_HEIGHT);
    menu->addMenu("File");
    menu->addMenuItem("Open (Ctrl+O)");
    menu->addMenuItem("Save As (Ctrl+Shift+S)");
    menu->addMenuItem("Save (Ctrl+S)");
    menu->addMenuItem("Exit (Esc)");
    menu->connectMenuItem({"File","Open (Ctrl+O)"},ProjectManager::open,&manager);
    menu->connectMenuItem({"File","Save As (Ctrl+Shift+S)"},ProjectManager::saveAs,&manager);
    menu->connectMenuItem({"File","Save (Ctrl+S)"},ProjectManager::save,&manager);
    menu->connectMenuItem({"File","Exit (Esc)"},&sf::RenderWindow::close,window);
    gui.add(menu);
}

void Program::handleEvent(sf::Event event){
    if (event.type == sf::Event::KeyPressed){
        if(event.key.control){
            if(event.key.code == sf::Keyboard::O)
                manager.open();
            if(event.key.code == sf::Keyboard::S){
                if(event.key.shift)
                    manager.saveAs();
                else
                    manager.save();
            }
        }
    }
}
