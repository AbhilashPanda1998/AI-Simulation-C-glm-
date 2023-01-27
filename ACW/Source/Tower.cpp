#include "Tower.h"
#include "helpers/IRenderHelpers.h"
#include "helpers/IGuiHelpers.h"
#include <iostream>
#include "glm/gtx/string_cast.hpp"
#include <GLFW/glfw3.h>
#include "World.h"

using namespace::std;

Tower::Tower()
{
	
}

Tower::~Tower()
{

}

void Tower::Update(float pSeconds, const bool isEuler, const bool isImprovedEuler, const bool isRangeKutta)
{
	m_IsImprovedEuler =  isImprovedEuler;
	m_IsEuler = isEuler;
	m_IsRangeKutta = isRangeKutta;

	m_Timer += pSeconds;
	if (m_Timer > m_ReloadTime)
	{
		ScanAreaForAgent();
		m_Timer = 0;
	}

	for (int i = 0; i < m_Projectiles.size(); i++)
	{
		m_Projectiles[i]->Update(pSeconds, isEuler, isImprovedEuler, isRangeKutta);
	}

	//Influence factor is determined by the timer
	if (m_IsInfluenceStarted)
	{
		m_InfluenceTimer += pSeconds;
		if (m_InfluenceTimer >= 5.f)
		{
			EndOwnershipProcess();
			m_IsInfluenceStarted = false;
		}
	}
}

void Tower::Render(const IRenderHelpers& pHelper) const
{
	pHelper.Render(this);
	for (int i = 0; i < m_Projectiles.size(); i++)
	{
		pHelper.Render(m_Projectiles[i]);
	}
}

void Tower::RenderGui(const IGuiHelpers& pHelper)
{
	pHelper.RenderGui(this);
}

//Initialize Tower data
void Tower::InitializeTower(const glm::vec3 position, Agent *agents, const int noOfAgents, World* world)
{
	for (int i = 0; i < noOfAgents; i++)
	{
		m_Agents.push_back(&agents[i]);
	}
	m_Radius = 16.f;
	Translate(position);
	Scale(glm::vec3(1, 7, 1.5f));
	GetColour() = glm::vec3(0.5f, 0.2f, 0.07f);
	m_World = world;
}

//Checks if agent is in radius and if the agent is not the same team type as tower
void Tower::ScanAreaForAgent()
{
	for (int i = 0; i < m_Agents.size(); i++)
	{
		if (glm::length(GetPosition() - m_Agents[i]->GetPosition()) <= m_Radius && m_Agents[i]->GetTeamType() != m_TeamType)
		{
			ShootProjectile(m_Agents[i]);
			return;
		}
	}
}

//Shoots Projectile
void Tower::ShootProjectile(Agent* target)
{
	Ball* newBall = new Ball();
	newBall->InitializeProjectile(glm::vec3(GetPosition().x, GetPosition().y + 7.f, GetPosition().z), target);
	m_Projectiles.push_back(newBall);
	m_World->projectiles.push_back(newBall);
}


//Starts getting owned by Agent type
void Tower::StartOwnerShipProcess(Agent* influencingAgent)
{  
	m_InfluencingAgent = influencingAgent;
	m_TeamType = m_InfluencingAgent->GetTeamType();
	isowned = true;

	if(m_TeamType == "Red")
	{
		GetColour() = glm::vec3(1, 0, 0);
	}
	else
	{
		GetColour() = glm::vec3(0, 0, 1);
	}
	m_IsInfluenceStarted = true;
}

//Ends the ownership process
void Tower::EndOwnershipProcess()
{
	m_InfluenceTimer = 0;
	m_InfluencingAgent->SetLastVelcoity();
}