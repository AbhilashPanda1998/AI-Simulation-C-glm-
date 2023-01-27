#pragma once
#include "GameEntity.h"
#include "Ball.h"
#include "Agent.h"
#include <vector>
using namespace std;

class World;
class Tower : public GameEntity
{
private:
    vector<Ball*> m_Projectiles;
    vector<Agent*> m_Agents;
    float m_Timer = 0;
    float m_ReloadTime = 0.5f;
    bool m_IsImprovedEuler;
    bool m_IsEuler;
    bool m_IsRangeKutta;
    string m_TeamType = "Neutral";
    bool m_IsInfluenceStarted;
    float m_InfluenceTimer;
    Agent* m_InfluencingAgent;
    World* m_World;

private:
    void ScanAreaForAgent();

public:

    Tower();
    ~Tower();
    string GetTeamType() const { return m_TeamType; }
    bool IsOwned() { return isowned;}
    void Update(const float pSeconds, const bool isEuler, const bool isImprovedEuler, const bool isRangeKutta);
    void Render(const IRenderHelpers&) const;
    void RenderGui(const IGuiHelpers&);
    void InitializeTower(const glm::vec3 position, Agent *agents, const int noOfAgents, World *world);
    void ShootProjectile(Agent* target);
    void StartOwnerShipProcess(Agent* influencingAgent);
    void EndOwnershipProcess();

    glm::vec3 GetPosition() const
    {
        glm::mat4 model = GetModel();
        glm::vec3 translation(model[3][0], model[3][1], model[3][2]);
        return translation;
    }

    bool isowned;
};