/*
  ==============================================================================

    ThroughComponent.h
    Created: 11 Nov 2023 2:32:26am
    Author:  須田丈晴

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "NodeComponent.h"
#include "Through.h"

class ThroughComponent : public NodeComponent
{
public:
    ThroughComponent(ThroughEffector* effect,juce::Component* parentToAttachIO) : NodeComponent(effect,parentToAttachIO)
    {
        setSize(100, 100);
        
        addNodeIO<InputNodeIO>(juce::Point<float>(-10, 20), parentToAttachIO, 0);
        addNodeIO<InputNodeIO>(juce::Point<float>(-10, 40), parentToAttachIO, 1);
        addNodeIO<OutputNodeIO>(juce::Point<float>(90, 20), parentToAttachIO, 0);
        addNodeIO<OutputNodeIO>(juce::Point<float>(90, 40), parentToAttachIO, 1);
        
        // ヘッダーにタイトルを設定
        headerComponent.setTitle("Through");
        addAndMakeVisible(headerComponent);  // ヘッダーを可視にする

        addAndMakeVisible(bodyComponent);  // ボディを可視にする
        headerComponent.toFront(true);
    }
private:
    ThroughEffector* throughEffect;
};
