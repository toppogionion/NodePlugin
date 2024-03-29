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
#include "DistortionComponent.h"
#include "CopyComponent.h"
#include "DelayComponent.h"

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
        } else if (effectType == "Distortion") {
            return std::make_unique<DistortionComponent>(static_cast<DistortionEffector*>(effect), editor);
        }else if (effectType == "Copy") {
            return std::make_unique<CopyComponent>(static_cast<CopyEffector*>(effect), editor);
        }else if (effectType == "Delay") {
            return std::make_unique<DelayComponent>(static_cast<DelayEffector*>(effect), editor);
        }else {
            // 未知のEffectタイプの場合
            return nullptr;
        }
    }
};
