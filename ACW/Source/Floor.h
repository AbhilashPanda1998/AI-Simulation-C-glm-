#pragma once
#include "GameEntity.h"
class Floor :
    public GameEntity
{
    public:
        Floor();
        void Render(const IRenderHelpers&) const;
        void RenderGui(const IGuiHelpers&);

        glm::vec3 GetPosition() const
        {
            glm::mat4 model = GetModel();
            glm::vec3 translation(model[3][0], model[3][1], model[3][2]);
            return translation;
        }
};

