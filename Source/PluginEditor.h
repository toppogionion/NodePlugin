/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "NodeComponent.h"
#include "NodeComponentListener.h"

//==============================================================================
/**
*/
class NodePluginAudioProcessorEditor  : public juce::AudioProcessorEditor,public juce::DragAndDropContainer, public NodeComponentListener
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
    void nodeComponentWillBeDeleted(NodeComponent* ) override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    NodePluginAudioProcessor& audioProcessor;
    
    // メンバ変数としてstd::vector<std::unique_ptr<NodeComponent>> nodeListを定義
    std::vector<std::unique_ptr<NodeComponent>> nodeList;
    
    juce::Point<int> lastMousePosition;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NodePluginAudioProcessorEditor)
};
