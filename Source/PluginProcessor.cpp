/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Effects.h"

//==============================================================================
NodePluginAudioProcessor::NodePluginAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
   
}

NodePluginAudioProcessor::~NodePluginAudioProcessor()
{
}

//==============================================================================
const juce::String NodePluginAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool NodePluginAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool NodePluginAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool NodePluginAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double NodePluginAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int NodePluginAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int NodePluginAudioProcessor::getCurrentProgram()
{
    return 0;
}

void NodePluginAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String NodePluginAudioProcessor::getProgramName (int index)
{
    return {};
}

void NodePluginAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void NodePluginAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    audioGraph.setPlayConfigDetails(getTotalNumInputChannels(),
                                  getTotalNumOutputChannels(),
                                  sampleRate, samplesPerBlock);
    audioGraph.prepareToPlay(sampleRate, samplesPerBlock);
    
    // 入力ノードの作成
    dawInputNode = audioGraph.addNode(std::make_unique<juce::AudioProcessorGraph::AudioGraphIOProcessor>(juce::AudioProcessorGraph::AudioGraphIOProcessor::audioInputNode));
    dawOutputNode = audioGraph.addNode(std::make_unique<juce::AudioProcessorGraph::AudioGraphIOProcessor>(juce::AudioProcessorGraph::AudioGraphIOProcessor::audioOutputNode));

    // Input/Outputノードを追加
    inputEffect = createEffect<InputEffector>();
    outputEffect = createEffect<OutputEffector>();
    
    audioGraph.addConnection({{dawInputNode->nodeID, 0}, {findNodeForProcessor(inputEffect)->nodeID, 0}});
    audioGraph.addConnection({{dawInputNode->nodeID, 1}, {findNodeForProcessor(inputEffect)->nodeID, 1}});
    
    audioGraph.addConnection({{findNodeForProcessor(outputEffect)->nodeID, 0}, {dawOutputNode->nodeID, 0}});
    audioGraph.addConnection({{findNodeForProcessor(outputEffect)->nodeID, 1}, {dawOutputNode->nodeID, 1}});
}

void NodePluginAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool NodePluginAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void NodePluginAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    audioGraph.processBlock(buffer, midiMessages);
}

//==============================================================================
bool NodePluginAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* NodePluginAudioProcessor::createEditor()
{
    return new NodePluginAudioProcessorEditor (*this);
}

//==============================================================================
void NodePluginAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void NodePluginAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

// エフェクトを追加する
void NodePluginAudioProcessor::addGraph(std::unique_ptr<BaseEffect> effect) {
    auto node = audioGraph.addNode(std::move(effect));
    // 追加したエフェクトのノードを接続するなどの処理
    if (!node)
    {
        // ノードの追加に成功した場合の処理
        DBG("ノードの追加に失敗しました。");
    }
}

// エフェクトを削除する
void NodePluginAudioProcessor::removeGraph(BaseEffect* effect) {
    auto node = findNodeForProcessor(effect);
    if (node) {
        audioGraph.removeNode(node->nodeID);
    }
}

// エフェクト間を接続する
void NodePluginAudioProcessor::connectGraph(BaseEffect* outputEffect, int outputChannel, BaseEffect* inputEffect, int inputChannel) {
    auto outputNode = findNodeForProcessor(outputEffect);
    auto inputNode = findNodeForProcessor(inputEffect);
    if (outputNode && inputNode) {
        audioGraph.addConnection({ {outputNode->nodeID, outputChannel}, {inputNode->nodeID, inputChannel} });
    }
}

// エフェクトの接続を解除する
void NodePluginAudioProcessor::disconnectGraph(BaseEffect* outputEffect, int outputChannel, BaseEffect* inputEffect, int inputChannel) {
    auto outputNode = findNodeForProcessor(outputEffect);
    auto inputNode = findNodeForProcessor(inputEffect);
    audioGraph.removeConnection({ {outputNode->nodeID, outputChannel}, {inputNode->nodeID, inputChannel} });
}

// ヘルパー関数: プロセッサに対応するノードを見つける
juce::AudioProcessorGraph::Node::Ptr NodePluginAudioProcessor::findNodeForProcessor(BaseEffect* processor) {
    for (auto* node : audioGraph.getNodes()) {
        if (node->getProcessor() == processor) {
            return node;
        }
    }
    return nullptr;
}

bool NodePluginAudioProcessor::isOutputReachableFromAnyNode(BaseEffect* startEffect,BaseEffect* endEffect) {
    // 出力ノードに至る経路が存在するか確認する関数

    juce::AudioProcessorGraph::Node::Ptr startNode = findNodeForProcessor(startEffect);
    juce::AudioProcessorGraph::Node::Ptr endNode = findNodeForProcessor(endEffect);
    if(!(startNode && endNode)) return false;
    
    // 出力ノードから逆方向に到達可能なノードを追跡する集合
    std::set<juce::AudioProcessorGraph::NodeID> visitedNodes;

    // 出力ノードからスタート
    std::queue<juce::AudioProcessorGraph::NodeID> nodesToVisit;
    nodesToVisit.push(endNode->nodeID);
    visitedNodes.insert(endNode->nodeID);

    while (!nodesToVisit.empty()) {
        auto currentNodeID = nodesToVisit.front();
        nodesToVisit.pop();

        // 現在のノードへの入力を持つ接続を全て取得
        for (auto& connection : audioGraph.getConnections()) {
            if (connection.destination.nodeID == currentNodeID) {
                juce::AudioProcessorGraph::NodeID sourceNodeID = connection.source.nodeID;

                // 既に訪れたノードでなければ、探索リストに追加
                if (visitedNodes.find(sourceNodeID) == visitedNodes.end()) {
                    nodesToVisit.push(sourceNodeID);
                    visitedNodes.insert(sourceNodeID);

                    // 生成元がオーディオ入力でない場合は、そのノードも有効な信号源と見なせる
                    if (sourceNodeID != startNode->nodeID) {
                        return true;
                    }
                }
            }
        }
    }

    // 出力ノードに至る経路が見つからなければ false
    return false;
}

std::vector<BaseEffect*> NodePluginAudioProcessor::getEffects(){
    return effects;
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new NodePluginAudioProcessor();
}


