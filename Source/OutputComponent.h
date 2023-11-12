
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
#include "Output.h"

class OutputComponent : public NodeComponent
{
public:
    OutputComponent(OutputEffector* effect,juce::Component* parentToAttachIO) : NodeComponent(effect,parentToAttachIO)
    {
        setSize(100, 100);
        
        addNodeIO<InputNodeIO>(juce::Point<float>(-10, 40), parentToAttachIO, 0); // InputComponentにはOutputNodeIOを追加
        addNodeIO<InputNodeIO>(juce::Point<float>(-10, 60), parentToAttachIO, 1);
        // ヘッダーにタイトルを設定
        headerComponent.setTitle("Output");
        addAndMakeVisible(headerComponent);  // ヘッダーを可視にする

        addAndMakeVisible(bodyComponent);  // ボディを可視にする
        headerComponent.toFront(true);
    }
private:
    OutputEffector* outputEffect;
};
