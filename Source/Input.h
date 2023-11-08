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
    InputEffector() : BaseEffect("InputEffect",2,2)
    {
        setNumInputs(0);  // InputEffect には入力がありません。
        setNumOutputs(2); // DAWへの出力を1つ持ちます。
    }

    ~InputEffector() override {}

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
        // InputEffect は入力をそのまま出力に送るだけなので、ここでは何もしません。
        // 実際には入力を受け取り、それを処理するロジックが必要になります。
        // 必要に応じてバッファの内容を確認
        /*
        for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
        {
            
            // チャンネルごとの最初の10サンプルを出力
            juce::String channelData = "Input  Channel " + juce::String(channel) + ": ";
            for (int sample = 0; sample < std::min(10, buffer.getNumSamples()); ++sample)
            {
                channelData += juce::String(buffer.getSample(channel, sample)) + " ";
            }
            DBG(channelData);
        }
         */
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
