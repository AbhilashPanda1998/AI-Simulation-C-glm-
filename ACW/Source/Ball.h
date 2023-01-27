#pragma once
#include "GameEntity.h"
#include "Floor.h"
#include "Agent.h"

class Ball : public GameEntity
{
private:
    glm::vec3 m_Velocity;
    float m_Mass = 1.f;
    float m_CoefficientOfRestitution = 0.85f;
    int m_MaxBounceCount = 8;
    int m_BounceCount = 0;
    float m_Damage = 50.f;
    bool m_HasHitAgent = false;
    glm::vec3 m_ForceApplied = glm::vec3(0, -9.81f, 0);  //Gravitational Pull
    glm::vec3 m_Normal = glm::vec3(0, 1, 0);
    bool m_IsImprovedEuler;
    bool m_IsEuler;
    bool m_IsRangeKutta;
    Floor m_Ground;
    Agent *m_Agent;


public:

    Ball();

    glm::vec3 GetPosition() const
    {
        glm::mat4 model = GetModel();
        glm::vec3 translation(model[3][0], model[3][1], model[3][2]);
        return translation;
    }

    void SetPosition(glm::vec3 pPosition)
    {
        glm::mat4 & model = GetModel();
        model[3][0] = pPosition.x;
        model[3][1] = pPosition.y;
        model[3][2] = pPosition.z;
    }

    glm::vec3 GetVelocity() const
    {
        return m_Velocity;
    }

    void SetVelocity(glm::vec3 pVelocity)
    {
        m_Velocity = pVelocity;
    }

    void SetRadius(float pRadius)
    {
        m_Radius = pRadius;
    }

    void SetMass(float mass)
    {
        m_Mass = mass;
    }

    float GetMass() const
    {
        return m_Mass;
    }

private:

    void UpdateProjectile(const float deltaSeconds);
    void CheckCollisionWithGround();
    void CheckCollisionWithAgent();

public:

    void Render(const IRenderHelpers&) const;
    void RenderGui(const IGuiHelpers&);
    void Update(float pDeltaTime, const bool isEuler, const bool isImprovedEuler, const bool isRangeKutta);
    void InitializeProjectile(const glm::vec3 towerPosition, Agent* agent);
};

