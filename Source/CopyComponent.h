/*
  ==============================================================================

    CopyComponent.h
    Created: 12 Nov 2023 9:03:49am
    Author:  須田丈晴

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "NodeComponent.h"
#include "Copy.h"

class CopyComponent : public NodeComponent
{
public:
    CopyComponent(CopyEffector* effect,juce::Component* parentToAttachIO) : NodeComponent(effect,parentToAttachIO)
    {
        setSize(100, 100);
        
        addNodeIO<InputNodeIO>(juce::Point<float>(-10, 20), parentToAttachIO, 0);
        addNodeIO<OutputNodeIO>(juce::Point<float>(90, 20), parentToAttachIO, 0);
        addNodeIO<OutputNodeIO>(juce::Point<float>(90, 40), parentToAttachIO, 1);
        
        // ヘッダーにタイトルを設定
        headerComponent.setTitle("Copy");
        addAndMakeVisible(headerComponent);  // ヘッダーを可視にする

        addAndMakeVisible(bodyComponent);  // ボディを可視にする
        headerComponent.toFront(true);
    }
private:
    CopyEffector* copyEffect;
};
