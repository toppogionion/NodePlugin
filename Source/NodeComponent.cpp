/*
  ==============================================================================

    NodeComponent.cpp
    Created: 2 Nov 2023 7:10:38pm
    Author:  須田丈晴

  ==============================================================================
*/

#include "NodeComponent.h"

NodeIO::NodeIO(NodeComponent* parentComponent, juce::Point<float> lPosition,int channel):parentNodeComponent(parentComponent),channel(channel)
{
    setSize(20, 20);
    setLocalPosition(lPosition);
    setCurrentColour( juce::Colours::lightgrey);
}

void NodeIO::paint(juce::Graphics& g)
{
    g.setColour(currentColour);
    g.fillEllipse(getLocalBounds().toFloat());
    
}

juce::Component* NodeIO::getTopLevelComponent(juce::Component* comp)
{
    while (comp->getParentComponent() != nullptr)
    {
        comp = comp->getParentComponent();
    }
    return comp;
}

void NodeIO::mouseDrag(const juce::MouseEvent& e)
{
    // ドラッグを開始する前にドラッグコンテナを取得
    auto dragContainer = juce::DragAndDropContainer::findParentDragContainerFor(this);
    
    if (dragContainer != nullptr && !dragContainer->isDragAndDropActive())
    {
        juce::var dragDescription = getType();
        dragContainer->startDragging(dragDescription, this);
    }
    
    if(connectedIO){
        connectedIO ->connectEffect(nullptr);
        connectEffect(nullptr);
        auto topLevel = getTopLevelComponent(this);
        topLevel->repaint();
    }
}

bool NodeIO::isInterestedInDragSource(const SourceDetails& dragSourceDetails)
{
    // 他のNodeIOからのドラッグを受け入れる場合に真を返します
    return dragSourceDetails.description.toString() == "NodeIO";
}

void NodeIO::itemDropped(const SourceDetails& dragSourceDetails)
{
    // ドラッグされた元の NodeIO を取得
    auto sourceNodeIO = dynamic_cast<NodeIO*>(dragSourceDetails.sourceComponent.get());

    if (sourceNodeIO && sourceNodeIO != this) // 自分自身へのドロップを避ける
    {
        if (connectedIO)
        {
            connectedIO->connectEffect(nullptr);
            //connectedIO->repaint(); // 可視化を更新するためのrepaint
        }
        if (sourceNodeIO->getConnectedIO())
        {
            sourceNodeIO->getConnectedIO()->connectEffect(nullptr);
            //sourceNodeIO->getConnectedIO()->repaint(); // 可視化を更新するためのrepaint
        }
        // 新たに接続
        connectEffect(sourceNodeIO);
        sourceNodeIO->connectEffect(this);
        
        // 可視化を更新するためのrepaint
        currentColour = juce::Colours::green;
        sourceNodeIO->setCurrentColour(juce::Colours::green);
        auto topLevel = getTopLevelComponent(this);
        topLevel->repaint();
    }
}

void NodeIO::itemDragEnter(const SourceDetails &dragSourceDetails)
{
    // Highlight or change appearance when a valid item enters
    setCurrentColour( juce::Colours::green);
    repaint();
}

void NodeIO::itemDragExit(const SourceDetails &dragSourceDetails)
{
    // Revert appearance change
    setCurrentColour(juce::Colours::lightgrey);
    repaint();
}

void NodeIO::setConnectedIO(NodeIO* otherIO){
    connectedIO = otherIO;
};

NodeIO* NodeIO::getConnectedIO()
{
    return connectedIO;
}

juce::Path NodeIO::getConnectedPath(){
    juce::Path bezierPath;
    // 接続先が存在すればベジェ曲線を描画する
    if (connectedIO)
    {
        juce::Point<float> startPoint = getBounds().getCentre().toFloat();  // このNodeIOの中心
        juce::Point<float> endPoint = connectedIO->getBounds().getCentre().toFloat();  // 接続先NodeIOの中心

        // ベジェ曲線のコントロールポイントを定義（例として、開始点と終了点の中間を使用）
        juce::Point<float> controlPoint1 = juce::Point<float>((startPoint.x + endPoint.x) * 0.5f, startPoint.y);
        juce::Point<float> controlPoint2 = juce::Point<float>((startPoint.x + endPoint.x) * 0.5f, endPoint.y);

        //DBG("startPoint: "+ startPoint.toString());
        //DBG("endPoint: "+ endPoint.toString());
        
        if(startPoint.x<endPoint.x){
            bezierPath.startNewSubPath(startPoint + juce::Point<float>(10,0));
            bezierPath.cubicTo(controlPoint1, controlPoint2, endPoint + juce::Point<float>(-10,0));
        }else{
            bezierPath.startNewSubPath(endPoint  - juce::Point<float>(10,0));
            bezierPath.cubicTo(controlPoint2, controlPoint1, startPoint  - juce::Point<float>(-10,0));
        }
        
    }
    
    return bezierPath;
}

juce::Point<float> NodeIO::getLocalPosition(){
    return LocalPosition;
}

void NodeIO::setLocalPosition(juce::Point<float> newPosition){
    LocalPosition = newPosition;
}

juce::Colour NodeIO::getCurrentColour() const
{
    return currentColour;
}

void NodeIO::setCurrentColour(const juce::Colour& colour)
{
    currentColour = colour;
}

juce::String NodeIO::getType() const {return "NodeIO";}

int NodeIO::getChannel() {return channel;}

NodeComponent* NodeIO::getParentNodeComponent(){return parentNodeComponent;}

//==============================================================================


InputNodeIO::InputNodeIO(NodeComponent* parentComponent,juce::Point<float> lPosition, int channel) : NodeIO(parentComponent,lPosition,channel)
{
    setSize(20, 20);
    setLocalPosition(lPosition);
    setCurrentColour(juce::Colours::lightgrey);
}

bool InputNodeIO::isInterestedInDragSource(const SourceDetails& dragSourceDetails)
{
    // 他のNodeIOからのドラッグを受け入れる場合に真を返します
    //DBG(dragSourceDetails.description.toString());
    return dragSourceDetails.description.toString() == "OutputIO";
}

juce::String InputNodeIO::getType() const  { return "InputIO"; }

void InputNodeIO:: connectEffect(NodeIO* otherIO)
{
    setConnectedIO(otherIO);
    if(connectedIO == nullptr){
        getParentNodeComponent()->getEffect()->disconnectEffectToInput(getChannel());
        setCurrentColour(juce::Colours::lightgrey);
    }else{
        getParentNodeComponent()->getEffect()->connectEffectToInput(getChannel(),otherIO->getParentNodeComponent()->getEffect(),otherIO->getChannel());
        setCurrentColour(juce::Colours::green);
    }
}
//==============================================================================


OutputNodeIO::OutputNodeIO(NodeComponent* parentComponent,juce::Point<float> lPosition,int channel) : NodeIO(parentComponent,lPosition,channel)
{
    setSize(20, 20);
    setLocalPosition(lPosition);
    setCurrentColour(juce::Colours::lightgrey);
}

bool OutputNodeIO::isInterestedInDragSource(const SourceDetails& dragSourceDetails)
{
    // 他のNodeIOからのドラッグを受け入れる場合に真を返します
    //DBG(dragSourceDetails.description.toString());
    return dragSourceDetails.description.toString() == "InputIO";
}

juce::String OutputNodeIO::getType() const  { return "OutputIO"; }

void OutputNodeIO::connectEffect(NodeIO* otherIO)
{
    setConnectedIO(otherIO);
    if(connectedIO == nullptr){
        getParentNodeComponent()->getEffect()->disconnectEffectToOutput(getChannel());
        setCurrentColour(juce::Colours::lightgrey);
    }else{
        getParentNodeComponent()->getEffect()->connectEffectToOutput(getChannel(),otherIO->getParentNodeComponent()->getEffect(),otherIO->getChannel());
        setCurrentColour(juce::Colours::green);
    }
}
//==============================================================================

HeaderComponent::HeaderComponent()
{
    addAndMakeVisible(titleLabel);
    titleLabel.setInterceptsMouseClicks(false, false); 
}

void HeaderComponent::setTitle(const juce::String& title)
{
    titleLabel.setText(title, juce::dontSendNotification);
}

void HeaderComponent::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::darkgrey); // ヘッダーの背景色
}

void HeaderComponent::resized()
{
    titleLabel.setBounds(getLocalBounds());
}

void HeaderComponent::mouseDown(const juce::MouseEvent& event)
{
    // NodeComponent にイベントを転送
    auto* parent = findParentComponentOfClass<NodeComponent>();
    if (parent != nullptr)
    {
        parent->mouseDown(event.getEventRelativeTo(parent));
    }
}

void HeaderComponent::mouseDrag(const juce::MouseEvent& event)
{
    // NodeComponent にイベントを転送
    auto* parent = findParentComponentOfClass<NodeComponent>();
    if (parent != nullptr)
    {
        parent->mouseDrag(event.getEventRelativeTo(parent));
    }
}

void HeaderComponent::mouseUp(const juce::MouseEvent& event)
{
    // NodeComponent にイベントを転送
    auto* parent = findParentComponentOfClass<NodeComponent>();
    if (parent != nullptr)
    {
        parent->mouseUp(event.getEventRelativeTo(parent));
    }
}
    


//==============================================================================


void BodyComponent::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::white); // ボディの背景色
}

void BodyComponent::resized()
{
    // ここで customComponents 内のコンポーネントのレイアウトを設定
    auto area = getLocalBounds();  // ボディの領域を取得

    for (auto* comp : customComponents)
    {
        comp->setBounds(area.removeFromTop(50));  // 各コンポーネントのサイズを設定
        area.removeFromTop(10);  // コンポーネント間の間隔を空ける
    }

}


//==============================================================================


NodeComponent::NodeComponent(BaseEffect* effect,juce::Component* parentToAttachIO):effect(effect)
{
    setSize(800, 600);
    effect->setNodeComponent(this);
    /*
    addNodeIO<InputNodeIO>(juce::Point<float>(-10, 20), parentToAttachIO);
    addNodeIO<OutputNodeIO>(juce::Point<float>(90, 20), parentToAttachIO);
    // ヘッダーにタイトルを設定
    headerComponent.setTitle("My Node Title");
    addAndMakeVisible(headerComponent);  // ヘッダーを可視にする

    // ボディにスライダーを追加
    auto& slider = bodyComponent.addCustomComponent<juce::Slider>();
    addAndMakeVisible(bodyComponent);  // ボディを可視にする
    headerComponent.toFront(true);
     */
}
NodeComponent::~NodeComponent()
{
    effect->setNodeComponent(nullptr);
}

void NodeComponent::translate(juce::Point<int> deltaPos)
{
    auto currentPosition = effect->getPosition();
    effect -> setPosition(currentPosition + deltaPos);
    setTopLeftPosition(currentPosition + deltaPos);
    updateNodeIOPosition();
}

void NodeComponent::updateNodeIOPosition()
{
    auto position = effect->getPosition();
    for(auto&nodeIO :nodeIOList){
        juce::Point<float> localIOPosition = nodeIO->getLocalPosition();
        nodeIO->setTopLeftPosition(position.x + localIOPosition.x, position.y + localIOPosition.y);
    }
}

void NodeComponent::paint(juce::Graphics& g)
{
    setTopLeftPosition(effect->getPosition());
    updateNodeIOPosition();
    g.setColour(juce::Colours::blue);
    g.fillRect(getLocalBounds());
}

void NodeComponent::resized() {
    // コンポーネントのリサイズ処理
    auto area = getLocalBounds();
    headerComponent.setBounds(area.removeFromTop(30)); // ヘッダーの高さを30に設定
    bodyComponent.setBounds(area); // 残りの領域をボディに割り当て
    headerComponent.toFront(true);
}

juce::Component* NodeComponent::getTopLevelComponent(juce::Component* comp)
{
    while (comp->getParentComponent() != nullptr)
    {
        comp = comp->getParentComponent();
    }
    return comp;
}

// ドラッグアンドドロップや右クリックメニューの処理
void NodeComponent::mouseDown(const juce::MouseEvent& e)
{
    DBG("Down");
    if (e.mods.isRightButtonDown()) // 右クリックの場合
    {
        juce::PopupMenu menu;
        menu.addItem(1, "Delete");
        
        menu.showMenuAsync (juce::PopupMenu::Options(),
                            [this,e] (int result)
                            {
            if (result == 0)
            {
                // user dismissed the menu without picking anything
            }
            else if (result == 1)
            {
                // NodeIOの接続を解除する処理
                for (auto& nodeIO : nodeIOList)
                {
                    if (auto* connectedIO = nodeIO->getConnectedIO())
                    {
                        connectedIO->connectEffect(nullptr); // 接続を解除
                    }
                }
                // リスナーに通知
                listeners.call([this](NodeComponentListener& listener) { listener.nodeComponentWillBeDeleted(this); });
                // NodeComponent を削除する処理
                if (auto* parent = getParentComponent())
                {
                    parent->removeChildComponent(this);
                    delete this;
                }
            }
        });
    }
    else if (e.mods.isLeftButtonDown()) // 左クリックの場合だけ
    {
        originalPosition = effect->getPosition();
        dragging = true;
        for(auto&nodeIO :nodeIOList){
            nodeIO -> toFront(true);
        }
        toFront(true);
    }
}

void NodeComponent::mouseDrag(const juce::MouseEvent& e)
{
    //DBG("Drag");
    if (e.mods.isLeftButtonDown()){
        setTopLeftPosition(originalPosition + e.getOffsetFromDragStart());
        effect -> setPosition(originalPosition + e.getOffsetFromDragStart());
        updateNodeIOPosition();
        
        auto topLevel = getTopLevelComponent(this);
        topLevel->repaint();
    }
}

void NodeComponent::mouseUp(const juce::MouseEvent& e)
{
    if (e.mods.isLeftButtonDown()) // 左クリックの場合
    {
        //DBG("UP");
        effect -> setPosition(originalPosition + e.getOffsetFromDragStart());
        setTopLeftPosition(originalPosition + e.getOffsetFromDragStart());
        dragging = false;
    }
}

std::vector<juce::Path> NodeComponent::getConnectedPaths()
{
    std::vector<juce::Path> PathArray;

    for (auto& nodeIO : nodeIOList)  // 仮定：nodeIOListはNodeIOのポインタのベクター
    {
        if(nodeIO->getType()=="OutputIO"){
            juce::Path bezierPath = nodeIO->getConnectedPath();
            if(!bezierPath.isEmpty()) PathArray.push_back(bezierPath);
        }
    }

    return PathArray;
}

BaseEffect* NodeComponent::getEffect(){return effect;}

std::vector<std::unique_ptr<NodeIO>>& NodeComponent::getNodeIOList() {
    return nodeIOList;
}

void NodeComponent::addListener(NodeComponentListener* listener)
{
    listeners.add(listener);
}

void NodeComponent::removeListener(NodeComponentListener* listener)
{
    listeners.remove(listener);
}
