/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class NodeIO : public juce::Component,public juce::DragAndDropTarget
{
public:
    NodeIO(juce::Point<float> lPosition)
    {
        DBG("NodeIO");
        setSize(20, 20);
        setLocalPosition(lPosition);
        setCurrentColour( juce::Colours::blue);
    }
    
    void paint(juce::Graphics& g) override
    {
        g.setColour(currentColour);
        g.fillEllipse(getLocalBounds().toFloat());
        
    }
    
    juce::Component* getTopLevelComponent(juce::Component* comp)
    {
        while (comp->getParentComponent() != nullptr)
        {
            comp = comp->getParentComponent();
        }
        return comp;
    }
    
    void mouseDrag(const juce::MouseEvent& e) override
    {
        // ドラッグを開始する前にドラッグコンテナを取得
        auto dragContainer = juce::DragAndDropContainer::findParentDragContainerFor(this);
        
        if (dragContainer != nullptr && !dragContainer->isDragAndDropActive())
        {
            juce::var dragDescription = getType();
            dragContainer->startDragging(dragDescription, this);
        }
    }
    
    bool isInterestedInDragSource(const SourceDetails& dragSourceDetails) override
    {
        // 他のNodeIOからのドラッグを受け入れる場合に真を返します
        return dragSourceDetails.description.toString() == "NodeIO";
    }

    void itemDropped(const SourceDetails& dragSourceDetails) override
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
    
    void itemDragEnter(const SourceDetails &dragSourceDetails) override
    {
        // Highlight or change appearance when a valid item enters
        setCurrentColour( juce::Colours::blue);
        repaint();
    }

    void itemDragExit(const SourceDetails &dragSourceDetails) override
    {
        // Revert appearance change
        setCurrentColour(juce::Colours::red);
        repaint();
    }
    
    
    
    juce::Uuid getUUID(){
        return thisIOUuid;
    }
    
    void setConnectedIO(NodeIO* otherIO)
    {
        connectedIO = otherIO;
    }

    NodeIO* getConnectedIO()
    {
        return connectedIO;
    }
    
    juce::Path getConnectedPath(){
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
                bezierPath.startNewSubPath(endPoint  + juce::Point<float>(10,0));
                bezierPath.cubicTo(controlPoint2, controlPoint1, startPoint  + juce::Point<float>(-10,0));
            }
            
        }
        
        return bezierPath;
    }
    
    juce::Point<float> getLocalPosition(){
        return LocalPosition;
    }
    
    void setLocalPosition(juce::Point<float> newPosition){
        LocalPosition = newPosition;
    }
    
    juce::Colour getCurrentColour() const
    {
        return currentColour;
    }
    
    void setCurrentColour(const juce::Colour& colour)
    {
        currentColour = colour;
    }
    
    virtual juce::String getType() const {return "NodeIO";}
    
private:
    juce::Uuid thisIOUuid;
    juce::Uuid connectedIOUuid = juce::Uuid::null();
    NodeIO* connectedIO = nullptr;
    juce::Colour currentColour;
    juce::Point<float> LocalPosition = juce::Point<float>(0,0);
};

class InputNodeIO : public NodeIO
{
public :
    InputNodeIO(juce::Point<float> lPosition) : NodeIO(lPosition)
    {
        DBG("Input");
        setSize(20, 20);
        setLocalPosition(lPosition);
        setCurrentColour(juce::Colours::blue);
    }
    
    bool isInterestedInDragSource(const SourceDetails& dragSourceDetails) override
    {
        // 他のNodeIOからのドラッグを受け入れる場合に真を返します
        DBG(dragSourceDetails.description.toString());
        return dragSourceDetails.description.toString() == "OutputIO";
    }
    
    juce::String getType() const override { return "InputIO"; }

};

class OutputNodeIO : public NodeIO
{
public :
    OutputNodeIO(juce::Point<float> lPosition) : NodeIO(lPosition)
    {
        DBG("Output");
        setSize(20, 20);
        setLocalPosition(lPosition);
        setCurrentColour(juce::Colours::green);
    }
    
    bool isInterestedInDragSource(const SourceDetails& dragSourceDetails) override
    {
        // 他のNodeIOからのドラッグを受け入れる場合に真を返します
        DBG(dragSourceDetails.description.toString());
        return dragSourceDetails.description.toString() == "InputIO";
    }
    
    juce::String getType() const override { return "OutputIO"; }

};

class NodeComponent : public juce::Component
{
public:
    NodeComponent(juce::Component* parentToAttachIO)
    {
        DBG("NodeComponent");
        setSize(400, 300);
        
        addNodeIO<InputNodeIO>(juce::Point<float>(-10, 20), parentToAttachIO);
        addNodeIO<OutputNodeIO>(juce::Point<float>(90, 20), parentToAttachIO);
    }
    
    template <typename NodeIOType>
    void addNodeIO(juce::Point<float> localPosition, juce::Component* parentToAttachIO)
    {
        // NodeIOの生成
        auto newNodeIO = std::make_unique<NodeIOType>(localPosition);
        // NodeIOを指定された親コンポーネントに追加
        parentToAttachIO->addAndMakeVisible(newNodeIO.get());
        nodeIOList.push_back(std::move(newNodeIO));
    }

    
    void setBounds(int x, int y, int w, int h)
    {
        juce::Component::setBounds(x, y, w, h);
        updateNodeIOPosition();
    }
    
    void updateNodeIOPosition()
    {
        auto position = getPosition();
        for(auto&nodeIO :nodeIOList){
            juce::Point<float> localIOPosition = nodeIO->getLocalPosition();
            nodeIO->setTopLeftPosition(position.x + localIOPosition.x, position.y + localIOPosition.y);
        }
    }

    void paint(juce::Graphics& g) override
    {
        g.setColour(juce::Colours::blue);
        g.fillRect(getLocalBounds());
    }
    
    juce::Component* getTopLevelComponent(juce::Component* comp)
    {
        while (comp->getParentComponent() != nullptr)
        {
            comp = comp->getParentComponent();
        }
        return comp;
    }
    
    // ドラッグアンドドロップや右クリックメニューの処理
    void mouseDown(const juce::MouseEvent& e) override
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
    
    void mouseDrag(const juce::MouseEvent& e) override
    {
        DBG("Drag");
        if (e.mods.isLeftButtonDown()){
            setTopLeftPosition(originalPosition + e.getOffsetFromDragStart());
            updateNodeIOPosition();
            
            auto topLevel = getTopLevelComponent(this);
            topLevel->repaint();
        }
    }
    
    void mouseUp(const juce::MouseEvent& e) override
    {
        if (e.mods.isLeftButtonDown()) // 左クリックの場合
        {
            DBG("UP");
            setTopLeftPosition(originalPosition + e.getOffsetFromDragStart());
            dragging = false;
        }
    }
    
    std::vector<juce::Path> getConnectedPaths()
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
    
private:
    juce::Point<int> originalPosition;
    bool dragging = false;
    std::vector<std::unique_ptr<NodeIO>> nodeIOList;
};


//==============================================================================
/**
*/
class NodePluginAudioProcessorEditor  : public juce::AudioProcessorEditor,public juce::DragAndDropContainer
{
public:
    NodePluginAudioProcessorEditor (NodePluginAudioProcessor&);
    ~NodePluginAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void paintOverChildren(juce::Graphics&) override;
    void resized() override;
    void mouseDrag(const juce::MouseEvent&) override;
    void mouseDown(const juce::MouseEvent&) override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    NodePluginAudioProcessor& audioProcessor;
    
    // メンバ変数としてstd::vector<std::unique_ptr<NodeComponent>> nodeListを定義
    std::vector<std::unique_ptr<NodeComponent>> nodeList;
    
    juce::Point<int> lastMousePosition;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NodePluginAudioProcessorEditor)
};
