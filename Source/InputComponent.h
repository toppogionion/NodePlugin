/*
  ==============================================================================

    InputComponent.h
    Created: 5 Nov 2023 12:13:46am
    Author:  須田丈晴

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "NodeComponent.h"

class InputComponent : public NodeComponent
{
public:
    InputComponent(juce::Component* parentToAttachIO) : NodeComponent(parentToAttachIO)
    {
        setSize(800, 600);
        
        addNodeIO<OutputNodeIO>(juce::Point<float>(90, 20), parentToAttachIO); // InputComponentにはOutputNodeIOを追加
        // ヘッダーにタイトルを設定
        headerComponent.setTitle("Input");
        addAndMakeVisible(headerComponent);  // ヘッダーを可視にする

        addAndMakeVisible(bodyComponent);  // ボディを可視にする
        headerComponent.toFront(true);
    }
};
