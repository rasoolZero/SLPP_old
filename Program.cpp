#include "Program.h"

Program::Program(tgui::Gui & _gui,sf::RenderWindow * window) : gui(_gui)
{
    tgui::Theme::setDefault("theme.txt");
    setupLayout();
    setupButtons();
    setupPageButtons();
    setupMenuBar(window);
    try{
        container = std::make_unique<AudioContainer>();
        midi = std::make_unique<MIDI>(*this);
        loaded=true;
        setPageNumber(0);
        manager.setGUI(&gui);
        manager.setContainer(container.get());
    }
    catch(std::runtime_error & e){
        tgui::MessageBox::Ptr box = tgui::MessageBox::create();
        box->setText(e.what());
        box->addButton("Close");
        box->onButtonPress(&Program::windowClosed,this,window);
        auto panel = tgui::Panel::create({"100%", "100%"});
        panel->getRenderer()->setBackgroundColor({0, 0, 0, 0});
        gui.add(panel, "TransparentBackground");
        gui.add(box,"ErrorBox");
    }
}
void Program::windowClosed(sf::RenderWindow * window){
    window->close();
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
    auto tooltip = tgui::Label::create();
    tooltip->setRenderer(tgui::Theme::getDefault()->getRenderer("ToolTip"));
    tooltip->setText("Click to load sound\nRight Click to open Config Window");
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
            button->setContainer(container.get());
            button->onClick(&CButton::openFile,button);
            button->onRightClick(&Program::createConfigWindow,this,i*8+j);
            button->setToolTip(tooltip);
            hl->add(button,"mainButton"+std::to_string(i)+std::to_string(j));
            hl->insertSpace(j*2,0.2);
        }
    }

}
void Program::setupPageButtons(){
    auto tooltip = tgui::Label::create();
    tooltip->setRenderer(tgui::Theme::getDefault()->getRenderer("ToolTip"));
    tooltip->setText("Click to change page");

    tgui::HorizontalLayout::Ptr hl = gui.get<tgui::HorizontalLayout>("HL0");
    for(int i=0;i<8;i++){
        CButton::Ptr button = CButton::create();
        button->setRenderer(tgui::Theme::getDefault()->getRenderer("PageButton"));
        button->setToolTip(tooltip);
        button->onClick(&Program::setPageNumber,this,i);
        hl->add(button,"PageButton"+std::to_string(i));
        hl->insertSpace(i*2,0.2);
    }
    CButton::Ptr button = CButton::create();
//    button->setVisible(false);
    button->setText("Stop All\nPlaying\nSounds");
    button->setTextSize(0);
    button->onClick(&AudioContainer::stopAll,container.get());
    hl->add(button);
    hl->insertSpace(16,0.4);

    for(int i=0;i<8;i++){
        hl = gui.get<tgui::HorizontalLayout>("HL"+std::to_string(i+1));
        CButton::Ptr button = CButton::create();
        button->setRenderer(tgui::Theme::getDefault()->getRenderer("PageButton"));
        button->setToolTip(tooltip);
        button->onClick(&Program::setPageNumber,this,i+8);
        hl->add(button,"PageButton"+std::to_string(i+8));
        hl->insertSpace(16,0.4);
    }

}


void Program::setPageNumber(int pn){
    if(!gui.get("panel")->isEnabled())
        return;
    if(pn<0)
        pn=0;
    if(pn>15)
        pn=15;

    gui.get<tgui::Button>("PageButton"+std::to_string(pageNumber))->setRenderer(tgui::Theme::getDefault()->getRenderer("PageButton"));
    pageNumber=pn;
    gui.get<tgui::Button>("PageButton"+std::to_string(pn))->setRenderer(tgui::Theme::getDefault()->getRenderer("PageButtonSelected"));
    container->setPageNumber(pageNumber);
}

void Program::trigger(int row,int col,bool down){
    container->trigger(row,col,down);
    gui.get<CButton>("mainButton"+std::to_string(row)+std::to_string(col))->setEnabled(!down);
}

void Program::setupMenuBar(sf::RenderWindow * window){
    tgui::MenuBar::Ptr menu = tgui::MenuBar::create();
    menu->setHeight(MENU_HEIGHT);
    menu->addMenu("File");
    menu->addMenuItem("New (Ctrl+N)");
    menu->addMenuItem("Open (Ctrl+O)");
    menu->addMenuItem("Save As (Ctrl+Shift+S)");
    menu->addMenuItem("Save (Ctrl+S)");
    menu->addMenuItem("Exit (Esc)");
    menu->connectMenuItem({"File","New (Ctrl+N)"},ProjectManager::newP,&manager);
    menu->connectMenuItem({"File","Open (Ctrl+O)"},ProjectManager::open,&manager);
    menu->connectMenuItem({"File","Save As (Ctrl+Shift+S)"},ProjectManager::saveAs,&manager);
    menu->connectMenuItem({"File","Save (Ctrl+S)"},ProjectManager::save,&manager);
    menu->connectMenuItem({"File","Exit (Esc)"},&sf::RenderWindow::close,window);
    gui.add(menu);
}

void Program::handleEvent(sf::Event event){
    if(!loaded)
        return ;
    if (event.type == sf::Event::KeyPressed){
        if(event.key.control){
            if(event.key.code == sf::Keyboard::O)
                manager.open();
            if(event.key.code == sf::Keyboard::N)
                manager.newP();
            if(event.key.code == sf::Keyboard::S){
                if(event.key.shift)
                    manager.saveAs();
                else
                    manager.save();
            }
        }
    }
}

void Program::createConfigWindow(int index){
    tgui::ChildWindow::Ptr childWindow = tgui::ChildWindow::create("Config");
    tgui::HorizontalLayout::Ptr hl = tgui::HorizontalLayout::create();
    hl->setSize("100%","30%");
    hl->setPosition("0%","35%");
    childWindow->add(hl,"configHL");
    gui.get("panel")->setEnabled(false);
    childWindow->onClose([&]{ this->gui.get("panel")->setEnabled(true);});
    setupConfigLoopButton(childWindow,index);
    setupConfigRemoveButton(childWindow,index);
    for(int i=0;i<3;i++)
        hl->insertSpace(i*2,0.3f);
    gui.add(childWindow,"Config");
}

void Program::setupConfigLoopButton(tgui::ChildWindow::Ptr window,int index){
    tgui::Button::Ptr button = tgui::Button::create();
    button->setText(container->getSound(pageNumber,index)->isLooped()?"looping":"not looping");
    button->onClick(&Program::loopButtonClick,this,index);
    window->get<tgui::HorizontalLayout>("configHL")->add(button,"LoopButton");
}


void Program::setupConfigRemoveButton(tgui::ChildWindow::Ptr window,int index){
    tgui::Button::Ptr button = tgui::Button::create();
    button->setText("Remove Sound");
    button->getRenderer()->setTextColor(sf::Color::Red);
    button->onClick(&Program::removeButtonClick,this,index);
    window->get<tgui::HorizontalLayout>("configHL")->add(button,"RemoveButton");
}

void Program::removeButtonClick(int index){
    container->getSound(pageNumber,index)->clearSample();
}

void Program::loopButtonClick(int index){
    Audio * sound = container->getSound(pageNumber,index);
    bool looping = sound->isLooped();
    looping=!looping;
    sound->setLooping(looping);
    sound->stopPlaying();
    gui.get<tgui::Button>("LoopButton")->setText(looping?"looping":"not looping");
}
