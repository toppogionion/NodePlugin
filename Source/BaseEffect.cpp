/*
  ==============================================================================

    BaseEffect.cpp
    Created: 7 Nov 2023 1:07:21pm
    Author:  須田丈晴

  ==============================================================================
*/

#include "BaseEffect.h"


BaseEffect::BaseEffect(IPluginProcessor& proc,const juce::String& effectName, int numInputChannels, int numOutputChannels)
: processor(proc),name(effectName), numInputs(numInputChannels), numOutputs(numOutputChannels), AudioProcessor(BusesProperties()
.withInput("Input", juce::AudioChannelSet::discreteChannels(numInputChannels), numInputChannels > 0)
.withOutput("Output", juce::AudioChannelSet::discreteChannels(numOutputChannels), numOutputChannels > 0))  {
    // コンストラクタの初期化リストでメンバ変数を初期化
    position = juce::Point<int>(0,0);
}

BaseEffect::~BaseEffect() {
    // 必要に応じてリソース解放のコードをここに記述
}

void BaseEffect::connectEffectToInput(int inputChannel, BaseEffect* effect, int targetChannel) {
    if (inputChannel < inputConnections.size()) {
        inputConnections[inputChannel].connectedEffect = effect;
        inputConnections[inputChannel].channel = targetChannel;
        
        processor.connectGraph(effect, targetChannel, this, inputChannel);
    }
}

void BaseEffect::disconnectEffectToInput(int inputChannel) {
    if (inputChannel < inputConnections.size()) {
        BaseEffect* effect = inputConnections[inputChannel].connectedEffect;
        int targetChannel = inputConnections[inputChannel].channel;
        processor.disconnectGraph(effect, targetChannel, this, inputChannel);
        
        inputConnections[inputChannel].connectedEffect = nullptr;
        inputConnections[inputChannel].channel = -1;
    }
}

void BaseEffect::connectEffectToOutput(int outputChannel, BaseEffect* effect, int targetChannel) {
    if (outputChannel < outputConnections.size()) {
        outputConnections[outputChannel].connectedEffect = effect;
        outputConnections[outputChannel].channel = targetChannel;
        
        processor.connectGraph(this, outputChannel, effect, targetChannel);
    }
}

void BaseEffect::disconnectEffectToOutput(int outputChannel) {
    if (outputChannel < outputConnections.size()) {
        BaseEffect* effect = outputConnections[outputChannel].connectedEffect;
        int targetChannel = outputConnections[outputChannel].channel;
        processor.disconnectGraph(this, outputChannel, effect, targetChannel);
        
        outputConnections[outputChannel].connectedEffect = nullptr;
        outputConnections[outputChannel].channel = -1;
    }
}

const juce::String BaseEffect::getName() const {
    return name; // 設定されたエフェクト名を返す
}

void BaseEffect::setPosition(const juce::Point<int> newPosition) {
    position = newPosition; // 座標を設定
}

juce::Point<int> BaseEffect::getPosition() const {
    return position; // 座標を取得
}

void BaseEffect::setNumInputs(int newNumInputs) {
    numInputs = newNumInputs; // 入力端子数を設定
    inputConnections.resize(numInputs); // 入力接続の数を変更
}

int BaseEffect::getNumInputs() const {
    return numInputs; // 入力端子数を取得
}

void BaseEffect::setNumOutputs(int newNumOutputs) {
    numOutputs = newNumOutputs; // 出力端子数を設定
    outputConnections.resize(numOutputs); // 出力接続の数を変更
}

int BaseEffect::getNumOutputs() const {
    return numOutputs; // 出力端子数を取得
}
