#include "Agent.h"
#include "helpers/IRenderHelpers.h"
#include "helpers/IGuiHelpers.h"
#include <iostream>
#include "glm/gtx/string_cast.hpp"

using namespace std;

Agent::Agent()
{

}

void Agent::Update(float pSeconds)
{
	switch (m_CurrentState)
	{
	case Idle:
		break;
	case SeekTower:
		pos = m_Velocity * pSeconds;
		Translate(pos);
		break;
	case FleeEnemy:
		break;
	case InfluenceTower:
		SetVelocity(glm::vec3(0));
		break;
	case Dead:
		ResetAgent();
		break;
	default:
		break;
	}

}

void Agent::Render(const IRenderHelpers& pHelper) const
{
	pHelper.Render(this);
}

void Agent::RenderGui(const IGuiHelpers& pHelper)
{
	pHelper.RenderGui(this);
}

//Reduce AgentHealth
void Agent::TakeDamage(float damage)
{
	m_Health = m_Health - damage;
	if (m_Health <= 0)
	{
		isDead = true;
		SetState(Dead);
	}
}

//InitializeTower
void Agent::Init(const string teamType, const glm::vec3 startPosition, int boundaryWidth, int boundaryHeight)
{
	SetState(SeekTower);
	m_TeamType = teamType;
	m_Radius = 1;
	Translate(startPosition);
	m_Health = m_MaxHealth;
	m_BoundaryWidth = boundaryWidth;
	m_BoundaryHeight = boundaryHeight;
	m_Velocity = glm::vec3(2.5f, 0, 2.5f);
	m_InitPos = startPosition;
}

void Agent::InfiltrateTower()
{
	SetState(InfluenceTower);
}

void Agent::SetLastVelcoity()
{
	SetState(SeekTower);
	SetVelocity(m_LastVelocity);
}

void Agent::StoreLastvelocity()
{
	m_LastVelocity = GetVelocity();
}

void Agent::ResetAgent()
{
	m_Health = m_MaxHealth;
	SetPosition(m_InitPos);
	SetState(SeekTower);
}

//Seek behaviour
void Agent::Seek(glm::vec3 toseek)
{
	glm::vec3 desiredvelo = glm::normalize(toseek - GetPosition()) * m_Speed;
	SetVelocity(desiredvelo - GetVelocity());
}

//Flee behaviour
void Agent::Flee(glm::vec3 toflee)
{
	glm::vec3 desiredvelo = glm::normalize(GetPosition() - toflee) * m_Speed;
	SetVelocity(desiredvelo - GetVelocity());
}


void Agent::SetTargets(glm::vec3 nearestTower, glm::vec3 nearestEnemy)
{
	m_NearestTower = nearestTower;
	m_NearestEnemy = nearestEnemy;
}

void Agent::SetState(AgentState state)
{
	m_CurrentState = state;
}
