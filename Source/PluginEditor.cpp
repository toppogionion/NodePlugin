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
    setSize (400, 300);
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

void NodePluginAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}

void NodePluginAudioProcessorEditor::mouseDown(const juce::MouseEvent& e)
{
    juce::PopupMenu menu;
    menu.addItem(1, "Add Node");
    DBG("popup");
    // 他の項目も追加可能
    
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
                                 auto newNode = std::make_unique<NodeComponent>();
                                 newNode->setBounds(e.getPosition().x, e.getPosition().y, 100, 100);
                                 addAndMakeVisible(newNode.get());
                                 // オブジェクトの所有権をコンポーネントに移す（必要ならば）
                                 nodeList.push_back(std::move(newNode));
                             }
                         });
}
