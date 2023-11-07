/*
  ==============================================================================

    Output.h
    Created: 4 Nov 2023 11:02:22pm
    Author:  須田丈晴

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class OutputEffector : public BaseEffect {
public:
    void processEffect(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) override {
        // 出力処理
    }
};
