/*
  ==============================================================================

    Input.h
    Created: 4 Nov 2023 11:02:15pm
    Author:  須田丈晴

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "BaseEffect.h"

class InputEffector : public BaseEffect {
public:
    InputEffector() : BaseEffect("InputEffect"){
        numInputs = 0;  // InputEffect には入力がありません。
        numOutputs = 1; // DAWへの出力を1つ持ちます。
    }

    ~InputEffector() override {}

    const juce::String getName() const override {
        return name;
    }

    void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) override {
        // InputEffect は入力をそのまま出力に送るだけなので、ここでは何もしません。
        // 実際には入力を受け取り、それを処理するロジックが必要になります。
    }
};
