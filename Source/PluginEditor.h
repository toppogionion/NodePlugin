/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class NodeIO : public juce::Component
{
public:
    NodeIO()
    {
        setSize(20, 20);
        //pos = getPosition().toFloat();
    }
    
    void paint(juce::Graphics& g) override
    {
        g.setColour(juce::Colours::red);
        g.fillEllipse(getLocalBounds().toFloat());
        /*
        // ノードの描画処理
        g.fillAll(juce::Colours::green);
        g.setColour(juce::Colours::white);
        g.fillEllipse(pos.getX(),pos.getY(),10,10);  // draw border
         */
    }
    
private:
    juce::Point<float> pos {50, 50};
    juce::Uuid thisIOUuid;
    juce::Uuid connectedIOUuid = juce::Uuid::null();
};

class NodeComponent : public juce::Component
{
public:
    NodeComponent()
    {
        // コンストラクタでの初期化
        DBG("NodeComponent");
        /*
        auto newNodeIO = std::make_unique<NodeIO>();
        newNodeIO->setBounds(getPosition().getX(), getPosition().getY(), 20, 20);
        addAndMakeVisible(newNodeIO.get());
        // オブジェクトの所有権をコンポーネントに移す（必要ならば）
        nodeIOList.push_back(std::move(newNodeIO));
        */
        setSize(100, 100);
        // NodeIOを左端に配置
        auto nodeIO = std::make_unique<NodeIO>();
        nodeIO->setTopLeftPosition(-10, getHeight() / 2 - 10); // 中央の少し左に配置
        addAndMakeVisible(nodeIO.get());
        nodeIOList.push_back(std::move(nodeIO));
    }

    void paint(juce::Graphics& g) override
    {
        // ノードの描画処理
        g.fillAll(juce::Colours::blue);
        g.setColour(juce::Colours::white);
        g.drawRect(getLocalBounds(), 1);  // draw border
    }

    // ドラッグアンドドロップや右クリックメニューの処理
    void mouseDown(const juce::MouseEvent& e) override
        {
            if (e.mods.isLeftButtonDown()) // 左クリックの場合だけ
            {
                originalPosition = getPosition();
                dragging = true;
            }
        }

    void mouseDrag(const juce::MouseEvent& e) override
    {
        if (dragging && e.mods.isLeftButtonDown()) // draggingフラグと左クリックの場合
        {
            setTopLeftPosition(originalPosition + e.getOffsetFromDragStart());
        }
    }

    void mouseUp(const juce::MouseEvent& e) override
    {
        if (e.mods.isLeftButtonDown()) // 左クリックの場合
        {
            setTopLeftPosition(originalPosition + e.getOffsetFromDragStart());
            DBG("UP: "+std::to_string(getPosition().getX())+","+std::to_string(getPosition().getY()));
            dragging = false;
        }
    }
private:
    juce::Point<float> pos {50, 50}; // 初期座標
    juce::Point<int> originalPosition;
    bool dragging = false;
    std::vector<std::unique_ptr<NodeIO>> nodeIOList;
};

//==============================================================================
/**
*/
class NodePluginAudioProcessorEditor  : public juce::AudioProcessorEditor
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
