/*
  ==============================================================================

    EffectComponents.h
    Created: 5 Nov 2023 12:13:46am
    Author:  須田丈晴

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "NodeComponent.h"
#include "InputComponent.h"
#include "OutputComponent.h"
#include "ThroughComponent.h"

class NodeComponent;
class BaseEffect;

struct EffectComponentFactory {
    static std::unique_ptr<NodeComponent> createComponent(BaseEffect* effect, juce::Component* editor) {
        juce::String effectType = effect->getName();
        if (effectType == "Input") {
            return std::make_unique<InputComponent>(static_cast<InputEffector*>(effect), editor);
        } else if (effectType == "Output") {
            return std::make_unique<OutputComponent>(static_cast<OutputEffector*>(effect), editor);
        } else if (effectType == "Through") {
            return std::make_unique<ThroughComponent>(static_cast<ThroughEffector*>(effect), editor);
        } else {
            // 未知のEffectタイプの場合
            return nullptr;
        }
    }
};
