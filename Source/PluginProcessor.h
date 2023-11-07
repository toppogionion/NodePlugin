/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Effects.h"

//==============================================================================
/**
*/
class NodePluginAudioProcessor  : public juce::AudioProcessor
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif
{
public:
    //==============================================================================
    NodePluginAudioProcessor();
    ~NodePluginAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    void addGraph(std::unique_ptr<BaseEffect> effect);
    void removeGraph(BaseEffect* effect);
    void connectGraph(BaseEffect* outputEffect, int outputChannel, BaseEffect* inputEffect, int inputChannel);
    void disconnectGraph(BaseEffect* effect);
    
    juce::AudioProcessorGraph::Node::Ptr findNodeForProcessor(BaseEffect* processor);

    template <typename EffectType>
    BaseEffect* createEffect()
    {
        // Effectの生成
        auto newEffect = std::make_unique<EffectType>();
        // addGraphに渡す前に一時的なポインタを取得
        BaseEffect* effectPtr = newEffect.get();
        // グラフにエフェクトを追加
        addGraph(std::move(newEffect));
        // ベクトルにエフェクトを追加
        effects.push_back(effectPtr);
        
        return effectPtr;
    }
    
    void deleteEffect(BaseEffect* effect) {
        // AudioProcessorGraph からノードを削除する処理
        removeGraph(effect);

        // effects ベクトルからエフェクトを削除する処理
        auto it = std::find(effects.begin(), effects.end(), effect);
        if (it != effects.end()) {
            effects.erase(it); // ベクトルからエフェクトを削除
        }
    }
    
    bool isOutputReachableFromAnyNode(BaseEffect* startEffect,BaseEffect* endEffect) ;
    
private:
    //==============================================================================
    BaseEffect* inputEffect;
    BaseEffect* outputEffect;
    juce::AudioProcessorGraph audioGraph;
    std::vector<BaseEffect*> effects;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NodePluginAudioProcessor)
};
