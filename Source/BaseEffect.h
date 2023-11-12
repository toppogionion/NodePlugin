/*
  ==============================================================================

    BaseEffect.h
    Created: 4 Nov 2023 11:17:10pm
    Author:  須田丈晴

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "IPluginProcessor.h"

class NodeComponent;

class BaseEffect : public juce::AudioProcessor {
public:
    explicit BaseEffect(IPluginProcessor& proc,const juce::String& effectName, int numInputChannels, int numOutputChannels);
    virtual ~BaseEffect();

    // 共通の初期化処理
    void prepareToPlay(double sampleRate, int samplesPerBlock) override {
        // 共通の初期化処理をここに書く
    }

    // 共通の解放処理
    void releaseResources() override {
        // 共通の解放処理をここに書く
    }

    // 状態の保存と復元
    void getStateInformation(juce::MemoryBlock& destData) override {
        // 状態を保存する処理
    }
    void setStateInformation(const void* data, int sizeInBytes) override {
        // 状態を復元する処理
    }

    // エフェクト固有の処理を行うメソッド
    void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) override {
        // 共通の処理をここに書く
    }
    

    struct Connection{
        BaseEffect* connectedEffect;
        int channel;
        
        Connection() : connectedEffect(nullptr), channel(-1) {}
    };
    std::vector<Connection> inputConnections;  // 入力接続のベクトル
    std::vector<Connection> outputConnections; // 出力接続のベクトル
    
    // 特定のチャンネルにエフェクトを接続
    void connectEffectToInput(int inputChannel, BaseEffect* effect, int targetChannel) ;
    void disconnectEffectToInput(int inputChannel) ;
    void connectEffectToOutput(int outputChannel, BaseEffect* effect, int targetChannel);
    void disconnectEffectToOutput(int outputChannel);
    
    // UI関連の情報
    juce::Point<int> position; // エフェクトのUI位置
    int numInputs; // 入力端子の数
    int numOutputs; // 出力端子の数

    // UI関連の情報を設定・取得するための関数
    void setNodeComponent(NodeComponent*);
    NodeComponent* getNodeComponent();
    void setPosition(const juce::Point<int> newPosition);
    juce::Point<int> getPosition() const;
    const juce::String getName() const override;
    void setNumInputs(int newNumInputs);
    int getNumInputs() const;
    void setNumOutputs(int newNumOutputs);
    int getNumOutputs() const;
    
    juce::String name; // エフェクトの名前

    IPluginProcessor& processor;
private:
    NodeComponent* nodeComponent = nullptr;
};
