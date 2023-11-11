/*
  ==============================================================================

    NodeComponentListener.h
    Created: 3 Nov 2023 5:54:22pm
    Author:  須田丈晴

  ==============================================================================
*/

#pragma once

class NodeComponent;

class NodeComponentListener
{
public:
    virtual ~NodeComponentListener() = default;
    virtual void nodeComponentWillBeDeleted(NodeComponent* nodeComponent) = 0;
};
