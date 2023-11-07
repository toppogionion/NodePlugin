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
    void processEffect(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) override {
            // 入力処理
    }
};
