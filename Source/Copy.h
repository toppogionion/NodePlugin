/*
  ==============================================================================

    Copy.h
    Created: 12 Nov 2023 9:03:33am
    Author:  須田丈晴

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "BaseEffect.h"

class CopyEffector : public BaseEffect {
public:
    CopyEffector(IPluginProcessor& proc) : BaseEffect(proc,"Copy",1,2)
    {
        setNumInputs(1);  // InputEffect には入力がありません。
        setNumOutputs(2); // DAWへの出力を1つ持ちます。
        setPosition(juce::Point<int>(100,100));
        
    }

    ~CopyEffector() override {}

    const juce::String getName() const override {
        return name;
    }
    
    // JUCEのAudioProcessorクラスから必要な関数をオーバーライドします
    void prepareToPlay(double sampleRate, int samplesPerBlock) override {
        // 初期化処理
    }

    void releaseResources() override {
        // 解放処理
    }
    
    void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) override {
        if (buffer.getNumChannels() < 2) {
            // バッファーに十分なチャンネルがない場合、処理を行わない
            return;
        }
        auto* channelData = buffer.getWritePointer(0);
        // 0および1チャンネルに0チャンネルのデータをコピー
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample) {
            buffer.setSample(0, sample, channelData[sample]);  // 0チャンネル
            buffer.setSample(1, sample, channelData[sample]);  // 1チャンネル
        }
    }
    
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override {
        // このエフェクトがサポートするバスのレイアウトを指定します。
        // 通常は、ステレオ入出力やモノ入出力が必要かどうかなどをここで指定します。
        // ここでは単純化のためにすべてのレイアウトをサポートするとします。
        return true;
    }

    void getStateInformation(juce::MemoryBlock& destData) override {
        // 状態を保存する処理
    }

    void setStateInformation(const void* data, int sizeInBytes) override {
        // 状態を復元する処理
    }

    
    
    int getNumPrograms() override {
        // プログラム（プリセット）の数を返します。
        return 1;
    }

    int getCurrentProgram() override {
        // 現在のプログラム（プリセット）のインデックスを返します。
        return 0;
    }

    void setCurrentProgram(int index) override {
        // 現在のプログラム（プリセット）を設定します。
    }

    const juce::String getProgramName(int index) override {
        // 指定されたプログラム（プリセット）の名前を返します。
        return {};
    }

    void changeProgramName(int index, const juce::String& newName) override {
        // プログラム（プリセット）の名前を変更します。
    }

    double getTailLengthSeconds() const override {
        // エフェクトのテールタイム（残響などが完全に消えるまでの時間）を返します。
        return 0.0;
    }
    
    bool acceptsMidi() const override {
        // このエフェクトがMIDIメッセージを受け入れるかどうかを返します。
        return false;
    }

    bool producesMidi() const override {
        // このエフェクトがMIDIメッセージを生成するかどうかを返します。
        return false;
    }

    bool hasEditor() const override {
        // このエフェクトにカスタムエディタがあるかどうかを返します。
        // カスタムエディタがない場合は false を返します。
        return false;
    }
    
    juce::AudioProcessorEditor* createEditor() override {
        // エフェクトのカスタムエディタを生成して返します。
        // エディタがない場合は nullptr を返します。
        return nullptr;
    }
};
