/*
  ==============================================================================

    NodeComponent.cpp
    Created: 2 Nov 2023 7:10:38pm
    Author:  須田丈晴

  ==============================================================================
*/

#include "NodeComponent.h"

NodeIO::NodeIO(juce::Point<float> lPosition)
{
    DBG("NodeIO");
    setSize(20, 20);
    setLocalPosition(lPosition);
    setCurrentColour( juce::Colours::blue);
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
            connectedIO->setConnectedIO(nullptr);
            //connectedIO->repaint(); // 可視化を更新するためのrepaint
        }
        if (sourceNodeIO->getConnectedIO())
        {
            sourceNodeIO->getConnectedIO()->setConnectedIO(nullptr);
            //sourceNodeIO->getConnectedIO()->repaint(); // 可視化を更新するためのrepaint
        }
        // 新たに接続
        setConnectedIO(sourceNodeIO);
        sourceNodeIO->setConnectedIO(this);
        
        DBG("source: "+ getUUID().toString());
        DBG("connected: "+ connectedIO->getUUID().toString());
        
        // 可視化を更新するためのrepaint
        currentColour = juce::Colours::green;
        auto topLevel = getTopLevelComponent(this);
        topLevel->repaint();
    }
}

void NodeIO::itemDragEnter(const SourceDetails &dragSourceDetails)
{
    // Highlight or change appearance when a valid item enters
    setCurrentColour( juce::Colours::blue);
    repaint();
}

void NodeIO::itemDragExit(const SourceDetails &dragSourceDetails)
{
    // Revert appearance change
    setCurrentColour(juce::Colours::red);
    repaint();
}

juce::Uuid NodeIO::getUUID(){
    return thisIOUuid;
}

void NodeIO::setConnectedIO(NodeIO* otherIO)
{
    connectedIO = otherIO;
}

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


//==============================================================================


InputNodeIO::InputNodeIO(juce::Point<float> lPosition) : NodeIO(lPosition)
{
    DBG("Input");
    setSize(20, 20);
    setLocalPosition(lPosition);
    setCurrentColour(juce::Colours::blue);
}

bool InputNodeIO::isInterestedInDragSource(const SourceDetails& dragSourceDetails)
{
    // 他のNodeIOからのドラッグを受け入れる場合に真を返します
    DBG(dragSourceDetails.description.toString());
    return dragSourceDetails.description.toString() == "OutputIO";
}

juce::String InputNodeIO::getType() const  { return "InputIO"; }


//==============================================================================


OutputNodeIO::OutputNodeIO(juce::Point<float> lPosition) : NodeIO(lPosition)
{
    DBG("Output");
    setSize(20, 20);
    setLocalPosition(lPosition);
    setCurrentColour(juce::Colours::green);
}

bool OutputNodeIO::isInterestedInDragSource(const SourceDetails& dragSourceDetails)
{
    // 他のNodeIOからのドラッグを受け入れる場合に真を返します
    DBG(dragSourceDetails.description.toString());
    return dragSourceDetails.description.toString() == "InputIO";
}

juce::String OutputNodeIO::getType() const  { return "OutputIO"; }


//==============================================================================


NodeComponent::NodeComponent(juce::Component* parentToAttachIO)
{
    DBG("NodeComponent");
    setSize(400, 300);
    
    addNodeIO<InputNodeIO>(juce::Point<float>(-10, 20), parentToAttachIO);
    addNodeIO<OutputNodeIO>(juce::Point<float>(90, 20), parentToAttachIO);
}

void NodeComponent::setBounds(int x, int y, int w, int h)
{
    juce::Component::setBounds(x, y, w, h);
    updateNodeIOPosition();
}

void NodeComponent::updateNodeIOPosition()
{
    auto position = getPosition();
    for(auto&nodeIO :nodeIOList){
        juce::Point<float> localIOPosition = nodeIO->getLocalPosition();
        nodeIO->setTopLeftPosition(position.x + localIOPosition.x, position.y + localIOPosition.y);
    }
}

void NodeComponent::paint(juce::Graphics& g)
{
    g.setColour(juce::Colours::blue);
    g.fillRect(getLocalBounds());
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
    if (e.mods.isLeftButtonDown()) // 左クリックの場合だけ
    {
        originalPosition = getPosition();
        dragging = true;
        for(auto&nodeIO :nodeIOList){
            nodeIO -> toFront(true);
        }
        toFront(true);
    }
}

void NodeComponent::mouseDrag(const juce::MouseEvent& e)
{
    DBG("Drag");
    if (e.mods.isLeftButtonDown()){
        setTopLeftPosition(originalPosition + e.getOffsetFromDragStart());
        updateNodeIOPosition();
        
        auto topLevel = getTopLevelComponent(this);
        topLevel->repaint();
    }
}

void NodeComponent::mouseUp(const juce::MouseEvent& e)
{
    if (e.mods.isLeftButtonDown()) // 左クリックの場合
    {
        DBG("UP");
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
