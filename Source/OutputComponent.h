
/*
  ==============================================================================

    OutputComponent.h
    Created: 5 Nov 2023 12:13:46am
    Author:  須田丈晴

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "NodeComponent.h"

class OutputComponent : public NodeComponent
{
public:
    OutputComponent(juce::Component* parentToAttachIO) : NodeComponent(parentToAttachIO)
    {
        setSize(800, 600);
        
        addNodeIO<OutputNodeIO>(juce::Point<float>(-10, 20), parentToAttachIO); // InputComponentにはOutputNodeIOを追加
        // ヘッダーにタイトルを設定
        headerComponent.setTitle("Output");
        addAndMakeVisible(headerComponent);  // ヘッダーを可視にする

        addAndMakeVisible(bodyComponent);  // ボディを可視にする
        headerComponent.toFront(true);
    }
};
