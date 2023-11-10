/*
  ==============================================================================

    IPluginProcessor.h
    Created: 8 Nov 2023 9:14:52pm
    Author:  須田丈晴

  ==============================================================================
*/

#pragma once

class BaseEffect;

class IPluginProcessor {
public:
    virtual ~IPluginProcessor() = default;

    virtual void addGraph(std::unique_ptr<BaseEffect> effect) = 0;
    virtual void removeGraph(BaseEffect* effect) = 0;
    virtual void connectGraph(BaseEffect* outputEffect, int outputChannel, BaseEffect* inputEffect, int inputChannel) = 0;
    virtual void disconnectGraph(BaseEffect* outputEffect, int outputChannel, BaseEffect* inputEffect, int inputChannel) = 0;
    virtual void deleteEffect(BaseEffect* effect) =0;
};
