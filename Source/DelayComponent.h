/*
  ==============================================================================

    DelayComponent.h
    Created: 12 Nov 2023 9:42:03am
    Author:  須田丈晴

  ==============================================================================
*/

#pragma once
#include "NodeComponent.h"
#include "Delay.h"

class DelayComponent : public NodeComponent, public juce::Slider::Listener
{
public:
    DelayComponent(DelayEffector* effect,juce::Component* parentToAttachIO) : NodeComponent(effect,parentToAttachIO),delayEffect(effect)
    {
        setSize(200, 200);
        
        addNodeIO<InputNodeIO>(juce::Point<float>(-10, 40), parentToAttachIO, 0);
        addNodeIO<OutputNodeIO>(juce::Point<float>(190, 40), parentToAttachIO, 0);
        
        // ヘッダーにタイトルを設定
        headerComponent.setTitle("Delay");
        addAndMakeVisible(headerComponent);  // ヘッダーを可視にする

        addAndMakeVisible(bodyComponent);  // ボディを可視にする
        headerComponent.toFront(true);
        
        // スライダーの設定
        delaySlider.setSliderStyle(juce::Slider::Rotary);
        delaySlider.setRange(0.1, 1.5, 0.1);
        delaySlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
        delaySlider.setPopupDisplayEnabled(true, false, this);
        delaySlider.setTextValueSuffix("Distortion");
        bodyComponent.addAndMakeVisible(&delaySlider);
        delayLabel.setText("Delay", juce::dontSendNotification);
        delayLabel.attachToComponent(&delaySlider, true);
        bodyComponent.addAndMakeVisible(delayLabel);
        
        delaySlider.addListener(this);
    }
    
    void resized() override {
       // スライダーのサイズや位置を設定
        delaySlider.setBounds(50, 50, 100, 100);
    }
    // Slider::Listenerインターフェースのメソッドを実装
    void sliderValueChanged(juce::Slider* slider) override {
        if (slider == &delaySlider) {
            delayEffect->setDelayTime(delaySlider.getValue());
        }
    }
private:
    DelayEffector* delayEffect;
    juce::Slider delaySlider;
    juce::Label  delayLabel;
};
