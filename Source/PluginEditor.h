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
    NodeIO()
    {
        setSize(20, 20);
        currentColour = juce::Colours::blue;
    }
    
    void paint(juce::Graphics& g) override
    {
        g.setColour(currentColour);
        g.fillEllipse(getLocalBounds().toFloat());
    }
    
    void mouseDrag(const juce::MouseEvent& e) override
    {
        // ドラッグを開始する前にドラッグコンテナを取得
        auto dragContainer = juce::DragAndDropContainer::findParentDragContainerFor(this);
        
        if (dragContainer != nullptr && !dragContainer->isDragAndDropActive())
        {
            juce::var dragDescription = "NodeIO";
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
        // TODO: 接続情報を保存する、ビジュアルフィードバックを提供する、などの処理をここに追加する
        // ドラッグされた元の NodeIO を取得
        auto sourceNodeIO = dynamic_cast<NodeIO*>(dragSourceDetails.sourceComponent.get());

        if (sourceNodeIO && sourceNodeIO != this) // 自分自身へのドロップを避ける
        {
            // ここで2つの NodeIO を接続する処理などを行います
            DBG("NodeIO dropped onto another NodeIO");
            DBG("source: "+ sourceNodeIO->getUUID().toString());
            DBG("target: "+ this->getUUID().toString());
            
            currentColour = juce::Colours::green;
            repaint();
        }
    }
    void itemDragEnter(const SourceDetails &dragSourceDetails) override
    {
        // Highlight or change appearance when a valid item enters
        currentColour = juce::Colours::blue;
        repaint();
    }

    void itemDragExit(const SourceDetails &dragSourceDetails) override
    {
        // Revert appearance change
        currentColour = juce::Colours::red;
        repaint();
    }
    
    juce::Uuid getUUID(){
        return thisIOUuid;
    }
    
private:
    juce::Uuid thisIOUuid;
    juce::Uuid connectedIOUuid = juce::Uuid::null();
    juce::Colour currentColour;
};

class NodeComponent : public juce::Component
{
public:
    NodeComponent(juce::Component* parentToAttachIO)
    {
        DBG("NodeComponent");
        setSize(100, 100);

        // NodeIOの生成
        auto newNodeIO = std::make_unique<NodeIO>();
        
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
            nodeIO->setTopLeftPosition(position.x - 10, position.y + getHeight() / 2 - 10);
        }
    }

    void paint(juce::Graphics& g) override
    {
        g.setColour(juce::Colours::blue);
        g.fillRect(getLocalBounds());
    }
    
    // ドラッグアンドドロップや右クリックメニューの処理
    void mouseDown(const juce::MouseEvent& e) override
    {
        DBG("Down");
        if (e.mods.isLeftButtonDown()) // 左クリックの場合だけ
        {
            originalPosition = getPosition();
            dragging = true;
        }
    }
    
    void mouseDrag(const juce::MouseEvent& e) override
    {
        DBG("Drag");
        setTopLeftPosition(originalPosition + e.getOffsetFromDragStart());
        updateNodeIOPosition();
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
    void resized() override;
    void mouseDown(const juce::MouseEvent&) override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    NodePluginAudioProcessor& audioProcessor;
    
    // メンバ変数としてstd::vector<std::unique_ptr<NodeComponent>> nodeListを定義
    std::vector<std::unique_ptr<NodeComponent>> nodeList;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NodePluginAudioProcessorEditor)
};
