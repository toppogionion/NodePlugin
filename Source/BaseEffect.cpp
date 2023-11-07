/*
  ==============================================================================

    BaseEffect.cpp
    Created: 7 Nov 2023 1:07:21pm
    Author:  須田丈晴

  ==============================================================================
*/

#include "BaseEffect.h"


BaseEffect::BaseEffect(const juce::String& effectName)
: name(effectName), numInputs(0), numOutputs(0), AudioProcessor (BusesProperties()
  .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
  .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
  )  {
    // コンストラクタの初期化リストでメンバ変数を初期化
    position = juce::Point<int>(0,0);
}

BaseEffect::~BaseEffect() {
    // 必要に応じてリソース解放のコードをここに記述
}

void BaseEffect::connectEffectToInput(int inputChannel, BaseEffect* effect) {
    if (inputChannel < inputConnections.size()) {
        inputConnections[inputChannel].connectedEffect = effect;
    }
}

void BaseEffect::connectEffectToOutput(int outputChannel, BaseEffect* effect) {
    if (outputChannel < outputConnections.size()) {
        outputConnections[outputChannel].connectedEffect = effect;
    }
}

const juce::String BaseEffect::getName() const {
    return name; // 設定されたエフェクト名を返す
}

void BaseEffect::setPosition(const juce::Point<int>& newPosition) {
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
