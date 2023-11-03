/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
NodePluginAudioProcessorEditor::NodePluginAudioProcessorEditor (NodePluginAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (1200, 800);
    
}

NodePluginAudioProcessorEditor::~NodePluginAudioProcessorEditor()
{
}

//==============================================================================
void NodePluginAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void NodePluginAudioProcessorEditor::paintOverChildren(juce::Graphics& g)
{
    g.setColour(juce::Colours::black);
    
    for (auto& node : nodeList)  // 仮定：nodeIOListはNodeIOのポインタのベクター
    {
        std::vector<juce::Path> PathArray = node->getConnectedPaths();
        for (auto& bezierPath : PathArray){
            g.strokePath(bezierPath, juce::PathStrokeType(2.0f));
        }
    }
}

void NodePluginAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}

void NodePluginAudioProcessorEditor::mouseDrag(const juce::MouseEvent& e)
{
    /*
    if (e.mods.isMiddleButtonDown()){ // ホイールの場合
        auto offset = e.getPosition() - lastMousePosition;
        auto newBounds = getBounds() + offset;
        
        DBG(lastMousePosition.toString());
        setBounds(newBounds); // コンポーネントの位置を更新
    }
     */
}

void NodePluginAudioProcessorEditor::mouseDown(const juce::MouseEvent& e)
{
    DBG("mouseDown");
    /*
    // 他の項目も追加可能
    if (e.mods.isMiddleButtonDown()){ // ホイールの場合
        lastMousePosition = e.getPosition();
    }
     */
    if (e.mods.isRightButtonDown()){ // 右クリックの場合だけ
        juce::PopupMenu menu;
        menu.addItem(1, "Add Node");
        
        menu.showMenuAsync (juce::PopupMenu::Options(),
                            [this,e] (int result)
                            {
            if (result == 0)
            {
                // user dismissed the menu without picking anything
            }
            else if (result == 1)
            {
                DBG("make Node");
                // user picked item 1
                // NodeComponent を作成してキャンバスに追加
                auto newNode = std::make_unique<NodeComponent>(this);
                newNode->setBounds(e.getPosition().x, e.getPosition().y, 100, 100);
                addAndMakeVisible(newNode.get());
                // オブジェクトの所有権をコンポーネントに移す（必要ならば）
                nodeList.push_back(std::move(newNode));
            }
        });
    }
}
