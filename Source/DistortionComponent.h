/*
  ==============================================================================

    DistortionComponent.h
    Created: 11 Nov 2023 10:58:11pm
    Author:  須田丈晴

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "NodeComponent.h"
#include "Distortion.h"

class DistortionComponent : public NodeComponent, public juce::Slider::Listener
{
public:
    DistortionComponent(DistortionEffector* effect,juce::Component* parentToAttachIO) : NodeComponent(effect,parentToAttachIO),distortionEffect(effect)
    {
        setSize(200, 200);
        
        addNodeIO<InputNodeIO>(juce::Point<float>(-10, 40), parentToAttachIO, 0); // InputComponentにはOutputNodeIOを追加
        addNodeIO<OutputNodeIO>(juce::Point<float>(190, 40), parentToAttachIO, 0); // InputComponentにはOutputNodeIOを追加
        
        // ヘッダーにタイトルを設定
        headerComponent.setTitle("Distortion");
        addAndMakeVisible(headerComponent);  // ヘッダーを可視にする

        addAndMakeVisible(bodyComponent);  // ボディを可視にする
        headerComponent.toFront(true);
        
        // スライダーの設定
        distortionSlider.setSliderStyle(juce::Slider::Rotary);
        distortionSlider.setRange(0.6, 1.0, 0.01);
        distortionSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
        distortionSlider.setPopupDisplayEnabled(true, false, this);
        distortionSlider.setTextValueSuffix("Distortion");
        bodyComponent.addAndMakeVisible(&distortionSlider);
        distortionLabel.setText("Distortion", juce::dontSendNotification);
        distortionLabel.attachToComponent(&distortionSlider, true);
        bodyComponent.addAndMakeVisible(distortionLabel);
        
        distortionSlider.addListener(this);
    }
    
    void resized() override {
       // スライダーのサイズや位置を設定
       distortionSlider.setBounds(50, 50, 100, 100);
   }
   // Slider::Listenerインターフェースのメソッドを実装
   void sliderValueChanged(juce::Slider* slider) override {
       DBG("set");
       if (slider == &distortionSlider) {
           distortionEffect->setThreshold(distortionSlider.getValue());
       }
   }
    
private:
    DistortionEffector* distortionEffect;
    juce::Slider distortionSlider;
    juce::Label  distortionLabel;
};
