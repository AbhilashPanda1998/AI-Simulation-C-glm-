#pragma once
#include "GameEntity.h"
#include <string>
#include <vector>
using namespace std;

enum AgentState
{
	Idle,
	SeekTower,
	FleeEnemy,
	InfluenceTower,
	Dead,
};

class Agent :public GameEntity
{
private:
	glm::vec3 pos;
	float m_Health = 0;
	float m_MaxHealth = 100;
	int m_BoundaryWidth, m_BoundaryHeight;
	glm::vec3 m_Velocity;
	string m_TeamType = "";
	float m_Speed = 4.f;
	glm::vec3 m_InitPos;
	glm::vec3 m_NearestTower;
	glm::vec3 m_NearestEnemy;
	bool isDead;

	struct CollisionData {
		bool m_Collision;
		glm::vec3 m_Normal;
	};
	AgentState m_CurrentState;

public:

	glm::vec3 m_LastVelocity;

	Agent();
	string GetTeamType() const { return m_TeamType; }
	void Update(const float pSeconds);
	void Render(const IRenderHelpers&) const;
	void RenderGui(const IGuiHelpers&);
	void TakeDamage(float damage);
	void Init(const string teamType, const glm::vec3 agentPosition, int boundaryWidth,int boundaryHeight);
	void InfiltrateTower();
	void SetLastVelcoity();
	void StoreLastvelocity();
	void Seek(glm::vec3 toseek);
	void Flee(glm::vec3 toflee);
	void SetTargets(glm::vec3 nearestTower, glm::vec3 nearestEnemy);
	void SetState(AgentState state);

	glm::vec3 GetPosition() const
	{
		glm::mat4 model = GetModel();
		glm::vec3 translation(model[3][0], model[3][1], model[3][2]);
		return translation;
	}

	glm::vec3 GetVelocity() const
	{
		return m_Velocity;
	}

	void SetVelocity(glm::vec3 pVelocity)
	{
		m_Velocity = pVelocity;
	}

	void SetPosition(glm::vec3 pPosition)
	{
		glm::mat4& model = GetModel();
		model[3][0] = pPosition.x;
		model[3][1] = pPosition.y;
		model[3][2] = pPosition.z;
	}

private:
	void ResetAgent();
};

