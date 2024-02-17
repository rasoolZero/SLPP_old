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
        lightManager = std::make_unique<LightManager>(*(midi));
        manager = std::make_unique<ProjectManager>(*this,gui,*(container),*(lightManager));
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
        vl->insertSpace(i*2,0.2f);
    vl->insertSpace(1,0.4f);
    vl->insertSpace(0,0.2f);
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
    tooltip->setText("Click to load sound\nRight Click to open Config and Light Window");
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
            button->onRightClick(&Program::lightWindow,this,i,j);
            button->setToolTip(tooltip);
            hl->add(button,"mainButton"+std::to_string(i)+std::to_string(j));
            hl->insertSpace(j*2,0.2f);
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
        hl->insertSpace(i*2,0.2f);
    }
    tgui::Button::Ptr button = tgui::Button::create();
    button->setText("Stop All\nPlaying\nSounds");
    button->setTextSize(0);
    button->onClick(&AudioContainer::stopAll,container.get());
    hl->add(button);
    hl->insertSpace(16,0.4f);

    for(int i=0;i<8;i++){
        hl = gui.get<tgui::HorizontalLayout>("HL"+std::to_string(i+1));
        tgui::Button::Ptr button = tgui::Button::create();
        button->setRenderer(tgui::Theme::getDefault()->getRenderer("PageButton"));
        button->setToolTip(tooltip);
        button->onClick(&Program::setPageNumber,this,i+8);
        hl->add(button,"PageButton"+std::to_string(i+8));
        hl->insertSpace(16,0.4f);
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
}

void Program::trigger(int row,int col,bool down){
    if(!pollingEvents)
        return ;
    container->trigger(pageNumber,row,col,down);
    lightManager->trigger(pageNumber, row, col, down);
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
    menu->connectMenuItem({"View","Show Buttons with loaded sound (F2)"},&Program::statusWindow,this);
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
            statusWindow();
    }
}

void Program::createConfigWindow(int index,tgui::ChildWindow::Ptr parent){
    tgui::ChildWindow::Ptr childWindow = tgui::ChildWindow::create("Config");
    tgui::VerticalLayout::Ptr vl = tgui::VerticalLayout::create();
    vl->setSize("50%","100%");
    vl->setPosition("25%","0%");
    tgui::HorizontalLayout::Ptr hl1 = tgui::HorizontalLayout::create();
    tgui::HorizontalLayout::Ptr hl2 = tgui::HorizontalLayout::create();
    vl->add(hl1,"ConfigHL1");
    vl->add(hl2,"ConfigHL2");
    for(int i=0;i<4;i++)
        vl->insertSpace(i*2,0.3f);
    childWindow->add(vl);
    setupConfigLoopButton(childWindow,index);
    setupConfigRemoveButton(childWindow,index);
    parent->add(childWindow,"Config");
}

void Program::setupConfigLoopButton(tgui::ChildWindow::Ptr window,int index){
    tgui::Button::Ptr button = tgui::Button::create();
    button->setText(container->getSound(pageNumber,index)->isLooped()?"looping":"not looping");
    button->onClick(&Program::loopButtonClick,this,index);
    window->get<tgui::HorizontalLayout>("ConfigHL1")->add(button,"LoopButton");
}



void Program::setupConfigRemoveButton(tgui::ChildWindow::Ptr window,int index){
    tgui::Button::Ptr button = tgui::Button::create();
    button->setText("Remove Sound");
    button->getRenderer()->setTextColor(sf::Color::Red);
    button->onClick(&Program::removeButtonClick,this,index);
    window->get<tgui::HorizontalLayout>("ConfigHL2")->add(button,"RemoveButton");
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
        container->load(pageNumber,row,col,address);
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

void Program::statusWindow(){
    auto window = tgui::ChildWindow::create("");
    window->setSize(gui.getTarget()->getSize().x,gui.getTarget()->getSize().y);
    window->setPositionLocked();
    auto mainPanel = tgui::ScrollablePanel::create();
    mainPanel->setVerticalScrollbarPolicy(tgui::Scrollbar::Policy::Always);
    mainPanel->setHorizontalScrollbarPolicy(tgui::Scrollbar::Policy::Never);
    window->add(mainPanel);

    auto tooltip = tgui::Label::create();
    tooltip->setRenderer(tgui::Theme::getDefault()->getRenderer("ToolTip"));
    tooltip->setText("Click to play the loaded sound.\nNumbers are the light counters");

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
                std::string text = "";
                if(looped){
                    text+="Looped\n";
                }

                auto lbl = tgui::Label::create();
                lbl->setText(text);
                lbl->setPosition("(parent.innersize - size) / 2");
                lbl->getRenderer()->setTextColor(sf::Color(125,125,125));
                panel->add(lbl);
                panel->setToolTip(tooltip);
                panel->getRenderer()->setBackgroundColor(loaded?sf::Color(220,220,220):sf::Color(30,30,30));
                panel->onClick(&AudioContainer::trigger,container.get(),i,j,k,true);
                hl->add(panel);
            }
            for(int k=0;k<=8;k++)
                hl->insertSpace(k*2,k==4?0.4f:0.2f);

            vl->add(hl);
        }
        for(int j=0;j<=8;j++)
            vl->insertSpace(j*2,j==4?0.4f:0.2f);
        vl->setSize("50%","100%");
        vl->setPosition("25%","0%");


        panel->add(vl);
        verticalLayout->add(panel);
    }
    mainPanel->setVerticalScrollAmount(static_cast<unsigned int>(mainPanel->getContentSize().y/16.f));
    window->onClose([&]{ this->enable();});
    disable();
    gui.add(window);
}

void Program::lightWindow(int row,int col){
    auto window = tgui::ChildWindow::create("");
    window->setSize(gui.getTarget()->getSize().x,gui.getTarget()->getSize().y);
    window->setPositionLocked();

    window->onClose([&]{ this->enable();});
    disable();
    gui.add(window);
    setupLightAnimationWindow(window, row * 8 + col);
    createConfigWindow(row * 8 + col, window);
}


void Program::setupLightAnimationWindow(tgui::ChildWindow::Ptr window, int index)
{
    auto windowPanel = tgui::Panel::create();

    auto animationPanel = tgui::Panel::create({ "100%","90%" });
    animationPanel->setAutoLayout(tgui::AutoLayout::Top);

    auto controlPanel = tgui::Panel::create({ "100%","10%" });
    controlPanel->setAutoLayout(tgui::AutoLayout::Bottom);

    windowPanel->add(animationPanel, "AnimationPanel");
    windowPanel->add(controlPanel, "ControlPanel");
    window->add(windowPanel,"WindowLayout");

    setupLightAnimationPanel(animationPanel, index);
    setupLightAnimationControls(controlPanel, index);
}

void Program::setupLightAnimationPanel(tgui::Panel::Ptr parent, int index)
{
    auto innerPanel = tgui::Panel::create();
    innerPanel->setSize({ "parent.innerheight" , "100%"});
    innerPanel->setPosition("(parent.innersize - size) / 2");
    auto verticalStack = tgui::VerticalLayout::create();
    for (int i = 0; i < 8; i++) {
        auto horizontal = tgui::HorizontalLayout::create();
        for (int j = 0; j < 8; j++) {
            auto button = tgui::Panel::create();
            button->onClick(&Program::cycleLightButton, this, button, index, i * 8 + j);
            auto backgroundColor = light2color(lightManager->getFrameLight(0, pageNumber, index, i * 8 + j));
            button->getRenderer()->setBackgroundColor(backgroundColor);
            button->getRenderer()->setBorderColor(tgui::Color::White);
            button->getRenderer()->setBorders(tgui::Borders(1, 1, 1, 1));
            horizontal->add(button, "Light" + tgui::String(i * 8 + j));
        }
        verticalStack->add(horizontal);
    }
    innerPanel->add(verticalStack);
    parent->add(innerPanel);
}

void Program::setupLightAnimationControls(tgui::Panel::Ptr parent, int index)
{
    parent->setTextSize(parent->getSize().y / 4);


    auto horizontalControlLayout = tgui::HorizontalLayout::create({ "100%" ,"50%" });
    parent->add(horizontalControlLayout);
    horizontalControlLayout->setAutoLayout(tgui::AutoLayout::Top);

    auto firstFrame = tgui::Button::create(L"|◁");
    auto previousFrame = tgui::Button::create(L"◁");
    auto nextFrame = tgui::Button::create(L"▷");
    auto lastFrame = tgui::Button::create(L"▷|");

    nextFrame->onClick(&Program::nextFrame, this, index);
    previousFrame->onClick(&Program::previousFrame, this, index);
    firstFrame->onClick(&Program::firstFrame, this, index);
    lastFrame->onClick(&Program::lastFrame, this, index);

    auto frameInput = tgui::EditBox::create();
    frameInput->setInputValidator(tgui::EditBox::Validator::UInt);
    frameInput->setText("1");
    frameInput->onReturnOrUnfocus(&Program::frameIndexChanged, this, frameInput, index);
    size_t totalFrames = lightManager->getFrameCount(pageNumber, index);
    auto frameCounter = tgui::Label::create("/"+tgui::String(totalFrames));
    frameCounter->setHeight("100%");
    frameCounter->setVerticalAlignment(tgui::Label::VerticalAlignment::Center);
    frameCounter->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
    
    horizontalControlLayout->add(firstFrame);
    horizontalControlLayout->add(previousFrame);
    horizontalControlLayout->add(frameInput, "frameInput");
    horizontalControlLayout->add(frameCounter, "frameCounter");
    horizontalControlLayout->add(nextFrame);
    horizontalControlLayout->add(lastFrame);
    
    horizontalControlLayout->insertSpace(0, 14.0f);
    horizontalControlLayout->addSpace(14.0f);



    auto horizontalTimeLayout = tgui::HorizontalLayout::create({ "100%" ,"50%" });
    parent->add(horizontalTimeLayout);
    horizontalTimeLayout->setAutoLayout(tgui::AutoLayout::Bottom);

    auto timeLabel = tgui::Label::create("Enter frame duration in seconds:");
    timeLabel->setHeight("100%");
    timeLabel->setVerticalAlignment(tgui::Label::VerticalAlignment::Center);
    timeLabel->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Left);
    auto timeInput = tgui::EditBox::create();
    timeInput->onReturnOrUnfocus(&Program::frameDurationChanged, this, timeInput, index);
    float duration = lightManager->getFrameDuratoin(0, pageNumber, index);
    timeInput->setText(tgui::String(duration));
    timeInput->setInputValidator(tgui::EditBox::Validator::Float);
    horizontalTimeLayout->add(timeLabel);
    horizontalTimeLayout->add(timeInput,"timeInput");

    horizontalTimeLayout->setRatio(1, .75f);
    horizontalTimeLayout->setRatio(0, 3.f);
    horizontalTimeLayout->insertSpace(0, 14.f / 3.f);
    horizontalTimeLayout->addSpace(14.f / 3.f);

}

void Program::updateLightButtons(int currentFrame, int buttonIndex)
{
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++) {
            int index = i * 8 + j;
            auto lightButton = gui.get<tgui::Panel>("Light" + tgui::String(index));
            auto currentColor = light2color(lightManager->getFrameLight(currentFrame - 1, pageNumber, buttonIndex, index));
            lightButton->getRenderer()->setBackgroundColor(currentColor);
        }
}

void Program::cycleLightButton(tgui::Panel::Ptr currentLightButton, int buttonIndex, int lightIndex)
{
    auto frameInput = gui.get<tgui::EditBox>("frameInput");
    Uint32 currentFrame;
    if (!frameInput->getText().attemptToUInt(currentFrame))
        return;
    auto currentLight = lightManager->getFrameLight(currentFrame - 1, pageNumber, buttonIndex, lightIndex);
    auto nextLight = cycleLight(currentLight);
    currentLightButton->getRenderer()->setBackgroundColor(light2color(nextLight));
    lightManager->setFrameLight(currentFrame - 1, pageNumber, buttonIndex, lightIndex, nextLight);
}

void Program::nextFrame(int buttonIndex)
{
    auto frameInput = gui.get<tgui::EditBox>("frameInput");
    auto timeInput = gui.get<tgui::EditBox>("timeInput");
    auto frameCounter = gui.get<tgui::Label>("frameCounter");
    int currentFrame = frameInput->getText().toInt();
    int lastFrame = lightManager->getFrameCount(pageNumber,buttonIndex);
    if (currentFrame == lastFrame) {
        lastFrame = lightManager->newFrame(pageNumber,buttonIndex);
    }
    currentFrame++;
    float currentDuration = lightManager->getFrameDuratoin(currentFrame - 1, pageNumber, buttonIndex);
    timeInput->setText(tgui::String(currentDuration));
    frameInput->setText(tgui::String(currentFrame));
    frameCounter->setText("/"+tgui::String(lastFrame));
    updateLightButtons(currentFrame, buttonIndex);
}

void Program::previousFrame(int buttonIndex)
{
    auto frameInput = gui.get<tgui::EditBox>("frameInput");
    auto timeInput = gui.get<tgui::EditBox>("timeInput");
    int currentFrame = frameInput->getText().toInt();
    if (currentFrame == 1)
        return;
    currentFrame--;
    float currentDuration = lightManager->getFrameDuratoin(currentFrame - 1, pageNumber, buttonIndex);
    timeInput->setText(tgui::String(currentDuration));
    frameInput->setText(tgui::String(currentFrame));
    updateLightButtons(currentFrame, buttonIndex);
}

void Program::lastFrame(int buttonIndex)
{
    auto frameInput = gui.get<tgui::EditBox>("frameInput");
    auto timeInput = gui.get<tgui::EditBox>("timeInput");
    int currentFrame = lightManager->getFrameCount(pageNumber, buttonIndex);
    float currentDuration = lightManager->getFrameDuratoin(currentFrame - 1, pageNumber, buttonIndex);
    timeInput->setText(tgui::String(currentDuration));
    frameInput->setText(tgui::String(currentFrame));
    updateLightButtons(currentFrame, buttonIndex);
}

void Program::firstFrame(int buttonIndex)
{
    auto frameInput = gui.get<tgui::EditBox>("frameInput");
    auto timeInput = gui.get<tgui::EditBox>("timeInput");
    int currentFrame = 1;
    float currentDuration = lightManager->getFrameDuratoin(currentFrame - 1, pageNumber, buttonIndex);
    timeInput->setText(tgui::String(currentDuration));
    frameInput->setText(tgui::String(currentFrame));
    updateLightButtons(currentFrame, buttonIndex);
}

void Program::frameDurationChanged(const tgui::EditBox::Ptr& durationInput, int buttonIndex)
{
    auto frameInput = gui.get<tgui::EditBox>("frameInput");
    int currentFrame = frameInput->getText().toInt();
    auto newFrameDuration = durationInput->getText();
    float frameDuration;
    if (!newFrameDuration.attemptToFloat(frameDuration))
        return;
    lightManager->setFrameDuration(currentFrame - 1, pageNumber, buttonIndex, frameDuration);
}

void Program::frameIndexChanged(const tgui::EditBox::Ptr& frameInput, int buttonIndex)
{
    auto timeInput = gui.get<tgui::EditBox>("timeInput");
    auto newFrameIndex = frameInput->getText();
    Uint32 frameIndex;
    if (!newFrameIndex.attemptToUInt(frameIndex))
        return;
    if (frameIndex <= 0 || frameIndex > lightManager->getFrameCount(pageNumber, buttonIndex))
        return;

    float currentDuration = lightManager->getFrameDuratoin(frameIndex - 1, pageNumber, buttonIndex);
    timeInput->setText(tgui::String(currentDuration));
    updateLightButtons(frameIndex, buttonIndex);
}
