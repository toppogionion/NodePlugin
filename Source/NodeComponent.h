/*
  ==============================================================================

    NodeComponent.h
    Created: 2 Nov 2023 7:10:38pm
    Author: Toppogionion

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class NodeIO : public juce::Component,public juce::DragAndDropTarget
{
public:
    NodeIO(juce::Point<float> );
    
    void paint(juce::Graphics& ) override;
    
    juce::Component* getTopLevelComponent(juce::Component* );
    
    void mouseDrag(const juce::MouseEvent& ) override;
    
    bool isInterestedInDragSource(const SourceDetails& ) override;
    
    void itemDropped(const SourceDetails& ) override;
    
    void itemDragEnter(const SourceDetails &) override;

    void itemDragExit(const SourceDetails &) override;
    
    juce::Uuid getUUID();
    
    void setConnectedIO(NodeIO* );

    NodeIO* getConnectedIO();
    
    juce::Path getConnectedPath();
    
    juce::Point<float> getLocalPosition();
    
    void setLocalPosition(juce::Point<float> );
    
    juce::Colour getCurrentColour() const;
    
    void setCurrentColour(const juce::Colour& );
    
    virtual juce::String getType() const ;
    
private:
    juce::Uuid thisIOUuid;
    juce::Uuid connectedIOUuid = juce::Uuid::null();
    NodeIO* connectedIO = nullptr;
    juce::Colour currentColour;
    juce::Point<float> LocalPosition = juce::Point<float>(0,0);
};


//==============================================================================


class InputNodeIO : public NodeIO
{
public :
    InputNodeIO(juce::Point<float> );
    
    bool isInterestedInDragSource(const SourceDetails& ) override;
    
    juce::String getType() const override;

};


//==============================================================================


class OutputNodeIO : public NodeIO
{
public :
    OutputNodeIO(juce::Point<float> );
    
    bool isInterestedInDragSource(const SourceDetails& ) override;
    
    juce::String getType() const override;

};


//==============================================================================


class NodeComponent : public juce::Component
{
public:
    NodeComponent(juce::Component* );
    
    template <typename NodeIOType>
    void addNodeIO(juce::Point<float> localPosition, juce::Component* parentToAttachIO)
    {
        // NodeIOの生成
        auto newNodeIO = std::make_unique<NodeIOType>(localPosition);
        // NodeIOを指定された親コンポーネントに追加
        parentToAttachIO->addAndMakeVisible(newNodeIO.get());
        nodeIOList.push_back(std::move(newNodeIO));
    }

    
    void setBounds(int x, int y, int w, int h);
    
    void updateNodeIOPosition();

    void paint(juce::Graphics& ) override;
    
    juce::Component* getTopLevelComponent(juce::Component* );
    // ドラッグアンドドロップや右クリックメニューの処理
    void mouseDown(const juce::MouseEvent& ) override;
    
    void mouseDrag(const juce::MouseEvent& ) override;
    
    void mouseUp(const juce::MouseEvent& ) override;
    
    std::vector<juce::Path> getConnectedPaths();
    
private:
    juce::Point<int> originalPosition;
    bool dragging = false;
    std::vector<std::unique_ptr<NodeIO>> nodeIOList;
};