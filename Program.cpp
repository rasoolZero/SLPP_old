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
        manager = std::make_unique<ProjectManager>(*this,gui,*(container.get()));
        pollingEvents=true;
        setPageNumber(0);

    }
    catch(std::runtime_error & e){
        tgui::MessageBox::Ptr box = tgui::MessageBox::create();
        box->setText(e.what());
        box->addButton("Close");
        box->onButtonPress(&Program::windowClosed,this,window);
        disable();
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
            CButton::Ptr button = CButton::create(*this);
            if(i==1 || i==6)
                if(j==1 || j==6)
                    button->setText(s);
            if(i==2 || i==5)
                if(j==2 || j==5)
                    button->setText(s);
            button->setRowCol(i,j);
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
        tgui::Button::Ptr button = tgui::Button::create();
        button->setRenderer(tgui::Theme::getDefault()->getRenderer("PageButton"));
        button->setToolTip(tooltip);
        button->onClick(&Program::setPageNumber,this,i);
        hl->add(button,"PageButton"+std::to_string(i));
        hl->insertSpace(i*2,0.2);
    }
    tgui::Button::Ptr button = tgui::Button::create();
//    button->setVisible(false);
    button->setText("Stop All\nPlaying\nSounds");
    button->setTextSize(0);
    button->onClick(&AudioContainer::stopAll,container.get());
    hl->add(button);
    hl->insertSpace(16,0.4);

    for(int i=0;i<8;i++){
        hl = gui.get<tgui::HorizontalLayout>("HL"+std::to_string(i+1));
        tgui::Button::Ptr button = tgui::Button::create();
        button->setRenderer(tgui::Theme::getDefault()->getRenderer("PageButton"));
        button->setToolTip(tooltip);
        button->onClick(&Program::setPageNumber,this,i+8);
        hl->add(button,"PageButton"+std::to_string(i+8));
        hl->insertSpace(16,0.4);
    }

}


void Program::setPageNumber(int pn){
    if(!pollingEvents)
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
    if(!pollingEvents)
        return ;
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
    menu->connectMenuItem({"File","New (Ctrl+N)"},&Program::operate,this,Operations::newP);
    menu->connectMenuItem({"File","Open (Ctrl+O)"},&Program::operate,this,Operations::open);
    menu->connectMenuItem({"File","Save As (Ctrl+Shift+S)"},&Program::operate,this,Operations::saveAs);
    menu->connectMenuItem({"File","Save (Ctrl+S)"},&Program::operate,this,Operations::save);
    menu->connectMenuItem({"File","Exit (Esc)"},&sf::RenderWindow::close,window);

    menu->addMenu("View");
    menu->addMenuItem("Show Buttons with loaded sound (F2)");
    menu->connectMenuItem({"View","Show Buttons with loaded sound (F2)"},&Program::loadedSoundsWindow,this);
    gui.add(menu);
}

void Program::handleEvent(sf::Event event){
    if(!pollingEvents)
        return ;
    if (event.type == sf::Event::KeyPressed){
        if(event.key.control){
            if(event.key.code == sf::Keyboard::O)
                operate(Operations::open);
            if(event.key.code == sf::Keyboard::N)
                operate(Operations::newP);
            if(event.key.code == sf::Keyboard::S){
                if(event.key.shift)
                    operate(Operations::saveAs);
                else
                    operate(Operations::save);
            }
        }
        if(event.key.code == sf::Keyboard::F2)
            loadedSoundsWindow();
    }
}

void Program::createConfigWindow(int index){
    tgui::ChildWindow::Ptr childWindow = tgui::ChildWindow::create("Config");
    tgui::HorizontalLayout::Ptr hl = tgui::HorizontalLayout::create();
    hl->setSize("100%","30%");
    hl->setPosition("0%","35%");
    childWindow->add(hl,"configHL");
    childWindow->onClose([&]{ this->enable();});
    setupConfigLoopButton(childWindow,index);
    setupConfigRemoveButton(childWindow,index);
    for(int i=0;i<3;i++)
        hl->insertSpace(i*2,0.3f);
    disable();
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

void Program::load(int row,int col,std::string address){
    try{
        container->load(row,col,address);
    }
    catch(std::runtime_error & e){
        createErrorBox(e.what());
    }
}

void Program::createErrorBox(const char * message){
    tgui::MessageBox::Ptr box = tgui::MessageBox::create();
    box->setText(message);
    box->addButton("Ok");
    box->onButtonPress(&Program::errorClosed,this);
    disable();
    gui.add(box,"ErrorBox");
}

void Program::errorClosed(){
    gui.remove(gui.get("ErrorBox"));
    enable();
}

void Program::operate(Operations operation){
    if(operation==Operations::newP)
        manager->newP();
    if(operation==Operations::open)
        manager->open();
    if(operation==Operations::save)
        manager->save();
    if(operation==Operations::saveAs)
        manager->saveAs();
}

void Program::disable(){
    auto panel = tgui::Panel::create({"100%", "100%"});
    panel->getRenderer()->setBackgroundColor({0, 0, 0, 0});
    gui.add(panel, "TransparentBackground");
    pollingEvents=false;
}
void Program::enable(){
    pollingEvents=true;
    gui.remove(gui.get("TransparentBackground"));
}

void Program::loadedSoundsWindow(){
    auto window = tgui::ChildWindow::create("");
    window->setSize(gui.getTarget()->getSize().x,gui.getTarget()->getSize().y);
    window->setPositionLocked();
    auto mainPanel = tgui::ScrollablePanel::create();
    mainPanel->setVerticalScrollbarPolicy(tgui::Scrollbar::Policy::Always);
    mainPanel->setHorizontalScrollbarPolicy(tgui::Scrollbar::Policy::Never);
    window->add(mainPanel);

    auto verticalLayout = tgui::VerticalLayout::create();
    verticalLayout->setSize("100%","1600%");
    mainPanel->add(verticalLayout);
    for(int i=0;i<16;i++){
        auto panel = tgui::Panel::create();
        auto label = tgui::Label::create();
        label->setText("Page "+std::to_string(i+1));
        panel->add(label);


        auto vl = tgui::VerticalLayout::create();
        for(int j=0;j<8;j++){
            auto hl = tgui::HorizontalLayout::create();

            for(int k=0;k<8;k++){
                auto panel = tgui::Panel::create();
                bool loaded = container->getSound(i,j*8+k)->isLoaded();
                bool looped = container->getSound(i,j*8+k)->isLooped();
                if(looped){
                    auto lbl = tgui::Label::create();
                    lbl->setText("Looped");
                    lbl->setPosition("(parent.innersize - size) / 2");
                    lbl->getRenderer()->setTextColor(sf::Color(125,125,125));
                    panel->add(lbl);
                }
                panel->getRenderer()->setBackgroundColor(loaded?sf::Color(220,220,220):sf::Color(30,30,30));
                panel->onClick([&]{std::cout << "h" << std::endl;});
                hl->add(panel);
            }
            for(int k=0;k<=8;k++)
                hl->insertSpace(k*2,k==4?0.4:0.2);

            vl->add(hl);
        }
        for(int j=0;j<=8;j++)
            vl->insertSpace(j*2,j==4?0.4:0.2);
        vl->setSize("50%","100%");
        vl->setPosition("25%","0%");


        panel->add(vl);
        verticalLayout->add(panel);
    }
    mainPanel->setVerticalScrollAmount(mainPanel->getContentSize().y/16);
    window->onClose([&]{ this->enable();});
    disable();
    gui.add(window);
}
