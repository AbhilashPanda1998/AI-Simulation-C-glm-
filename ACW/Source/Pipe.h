#pragma once
#include "GameEntity.h"
class Pipe :
    public GameEntity
{
public:
    Pipe();
    float m_BaseRadius;
    float m_LipRadius;
    float m_BaseHeight;
    float m_LipHeight;

    void Render(const IRenderHelpers&) const;
    void RenderGui(const IGuiHelpers&);
};

