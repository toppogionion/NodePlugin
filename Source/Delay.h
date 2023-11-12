/*
  ==============================================================================

    Delay.h
    Created: 12 Nov 2023 9:41:52am
    Author:  須田丈晴

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "BaseEffect.h"

class DelayEffector : public BaseEffect {
private:
    juce::AudioSampleBuffer delayBuffer;
    int delayBufferLength;
    int delayPosition;
    float delayTimeInSeconds; // 遅延させる秒数

public:
    DelayEffector(IPluginProcessor& proc) : BaseEffect(proc,"Delay",1,1)
    {
        setNumInputs(1);  // InputEffect には入力がありません。
        setNumOutputs(1); // DAWへの出力を1つ持ちます。
        setPosition(juce::Point<int>(100,100));
        
        delayTimeInSeconds = 1.0; // 1秒遅延を初期値とする
        updateDelayBufferLength();
    }

    ~DelayEffector() override {}

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
    
    void setDelayTime(float seconds) {
        delayTimeInSeconds = seconds;
        updateDelayBufferLength();
    }

    void updateDelayBufferLength() {
        delayBufferLength = static_cast<int>(delayTimeInSeconds * getSampleRate());
        delayBuffer.setSize(2, delayBufferLength);
        delayBuffer.clear();
        delayPosition = 0;
    }
    
    void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) override {
        const int numSamples = buffer.getNumSamples();

        for (int channel = 0; channel < buffer.getNumChannels(); ++channel) {
            auto* channelData = buffer.getWritePointer(channel);
            auto* delayData = delayBuffer.getWritePointer(channel);

            for (int sample = 0; sample < numSamples; ++sample) {
                const float in = channelData[sample];
                channelData[sample] = delayData[delayPosition];
                delayData[delayPosition] = in;

                if (++delayPosition >= delayBufferLength) delayPosition = 0;
            }
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
