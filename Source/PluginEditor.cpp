/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
NodePluginAudioProcessorEditor::NodePluginAudioProcessorEditor (NodePluginAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    // ウィンドウのサイズ変更を有効にする
    setResizable(true, true);
    setResizeLimits(400, 300, 1200, 800); // ウィンドウの最小サイズと最大サイズを設定
    
    setSize (800, 600);
    
    for (auto& effect : audioProcessor.getEffects()){
        std::unique_ptr<NodeComponent> newNode =  EffectComponentFactory::createComponent(effect, this);
        addAndMakeVisible(newNode.get());
        newNode->addListener(this);
        nodeList.push_back(std::move(newNode));
    }
    
    for (auto& sourceNode : nodeList){
        if(!(sourceNode->getEffect()->getNumOutputs()>0))continue;
        
        for(auto& sourceNodeIO : sourceNode->getNodeIOList()){
            int sourceChannel = sourceNodeIO->getChannel();
         
            BaseEffect* pretargetEffect = sourceNode->getEffect();
            BaseEffect* targetEffect = pretargetEffect->outputConnections[sourceChannel].connectedEffect;
            if(!targetEffect) break;
            NodeComponent* targetNode = targetEffect->getNodeComponent();
            int targetChannel = sourceNode->getEffect()->outputConnections[sourceChannel].channel;
            
            for(auto& targetNodeIO : targetNode->getNodeIOList()){
                if(targetChannel==targetNodeIO->getChannel()){
                    targetNodeIO -> setConnectedIO(sourceNodeIO.get());
                    sourceNodeIO -> setConnectedIO(targetNodeIO.get());
                    break;
                }
            }
        }
    }
    
    repaint();
}

NodePluginAudioProcessorEditor::~NodePluginAudioProcessorEditor()
{
}

//==============================================================================
void NodePluginAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void NodePluginAudioProcessorEditor::paintOverChildren(juce::Graphics& g)
{
    g.setColour(juce::Colours::black);
    
    for (auto& node : nodeList)  // 仮定：nodeIOListはNodeIOのポインタのベクター
    {
        std::vector<juce::Path> PathArray = node->getConnectedPaths();
        for (auto& bezierPath : PathArray){
            g.strokePath(bezierPath, juce::PathStrokeType(2.0f));
        }
    }
}

void NodePluginAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}

void NodePluginAudioProcessorEditor::mouseDrag(const juce::MouseEvent& e)
{
    if (e.mods.isMiddleButtonDown()){ // ホイールの場合
        auto offset = e.getPosition() - lastMousePosition;
        lastMousePosition = e.getPosition();
        for (auto& node : nodeList)  // 仮定：nodeIOListはNodeIOのポインタのベクター
        {
            node -> translate(offset);
        }
        repaint();
    }
}

void NodePluginAudioProcessorEditor::mouseDown(const juce::MouseEvent& e)
{
    DBG("mouseDown");
    // 他の項目も追加可能
    if (e.mods.isMiddleButtonDown()){ // ホイールの場合
        lastMousePosition = e.getPosition();
    }
    if (e.mods.isRightButtonDown()){ // 右クリックの場合だけ
        juce::PopupMenu menu;
        menu.addItem(1, "Input");
        menu.addItem(2, "Output");
        menu.addItem(3, "Through");
        menu.addItem(4, "Distortion");
        
        menu.showMenuAsync (juce::PopupMenu::Options(),
                            [this,e] (int result)
                            {
            if (result == 0)
            {
                // user dismissed the menu without picking anything
            }
            else if (result == 1)
            {
                BaseEffect* effect = audioProcessor.createEffect<InputEffector>();
                effect->setPosition(juce::Point<int>(e.getPosition().x, e.getPosition().y));
                std::unique_ptr<NodeComponent> newNode =  EffectComponentFactory::createComponent(effect, this);
                addAndMakeVisible(newNode.get());
                newNode->addListener(this);
                nodeList.push_back(std::move(newNode));
                audioProcessor.connectDAWInputGraph(effect, true);
            }
            else if (result == 2)
            {
                BaseEffect* effect = audioProcessor.createEffect<OutputEffector>();
                effect->setPosition(juce::Point<int>(e.getPosition().x, e.getPosition().y));
                std::unique_ptr<NodeComponent> newNode =  EffectComponentFactory::createComponent(effect, this);
                addAndMakeVisible(newNode.get());
                newNode->addListener(this);
                nodeList.push_back(std::move(newNode));
                audioProcessor.connectDAWOutputGraph(effect, true);
            }
            else if (result == 3)
            {
                BaseEffect* effect = audioProcessor.createEffect<ThroughEffector>();
                effect->setPosition(juce::Point<int>(e.getPosition().x, e.getPosition().y));
                std::unique_ptr<NodeComponent> newNode =  EffectComponentFactory::createComponent(effect, this);
                addAndMakeVisible(newNode.get());
                newNode->addListener(this);
                nodeList.push_back(std::move(newNode));
            }
            else if (result == 4)
            {
                BaseEffect* effect = audioProcessor.createEffect<DistortionEffector>();
                effect->setPosition(juce::Point<int>(e.getPosition().x, e.getPosition().y));
                std::unique_ptr<NodeComponent> newNode =  EffectComponentFactory::createComponent(effect, this);
                addAndMakeVisible(newNode.get());
                newNode->addListener(this);
                nodeList.push_back(std::move(newNode));
            }
        });
    }
}

void NodePluginAudioProcessorEditor::nodeComponentWillBeDeleted(NodeComponent* nodeComponent)
{
    audioProcessor.deleteEffect(nodeComponent->getEffect());
    nodeList.erase(std::remove_if(nodeList.begin(), nodeList.end(),
                                  [nodeComponent](const std::unique_ptr<NodeComponent>& ptr)
                                  {
                                      return ptr.get() == nodeComponent;
                                  }),
                   nodeList.end());
    repaint();
}
