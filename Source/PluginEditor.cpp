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
        menu.addItem(1, "Add Node");
        
        menu.showMenuAsync (juce::PopupMenu::Options(),
                            [this,e] (int result)
                            {
            if (result == 0)
            {
                // user dismissed the menu without picking anything
            }
            else if (result == 1)
            {
                DBG("make Node");
                // user picked item 1
                /*
                // NodeComponent を作成してキャンバスに追加
                auto newNode = std::make_unique<NodeComponent>(this);
                newNode->setBounds(e.getPosition().x, e.getPosition().y, 100, 100);
                addAndMakeVisible(newNode.get());
                newNode->addListener(this);
                // オブジェクトの所有権をコンポーネントに移す（必要ならば）
                nodeList.push_back(std::move(newNode));
                 */
            }
        });
    }
}

void NodePluginAudioProcessorEditor::nodeComponentWillBeDeleted(NodeComponent* nodeComponent)
{
    nodeList.erase(std::remove_if(nodeList.begin(), nodeList.end(),
                                  [nodeComponent](const std::unique_ptr<NodeComponent>& ptr)
                                  {
                                      return ptr.get() == nodeComponent;
                                  }),
                   nodeList.end());
    repaint();
}

void NodePluginAudioProcessorEditor::nodeComponentConnectionChanged(NodeComponent* nodeComponent)
{
        // NodeComponentの接続状態が変更されたときの処理
        // 例: AudioProcessorGraphの接続を更新する
}
