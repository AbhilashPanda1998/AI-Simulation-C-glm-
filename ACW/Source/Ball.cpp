#include "Ball.h"
#include "helpers/IRenderHelpers.h"
#include "helpers/IGuiHelpers.h"
#include <iostream>
#include  "glm/gtx/string_cast.hpp"

Ball::Ball()
{

}

void Ball::Render(const IRenderHelpers& pHelper) const
{
	pHelper.Render(this);
}

void Ball::RenderGui(const IGuiHelpers& pHelper)
{
	pHelper.RenderGui(this);
}

void Ball::Update(float pDeltaTime, const bool isImrpovedEuler, const bool isEuler, const bool isRangeKutta)
{
	m_IsRangeKutta = isRangeKutta;
	m_IsEuler = isEuler;
	m_IsImprovedEuler = isImrpovedEuler;


	if (GetPosition().y <= 1.f)
	{
		float y = 1.f;
		SetPosition(glm::vec3(GetPosition().x, y, GetPosition().z));
	}
	Translate(m_Velocity * pDeltaTime);
	UpdateProjectile(pDeltaTime);
	CheckCollisionWithGround();
	CheckCollisionWithAgent();
}


//Initial push to the projectile
void Ball::InitializeProjectile(glm::vec3 towerPosition, Agent* agent)
{
	m_Radius = 0.25f;
	m_Agent = agent;
	glm::vec3 targetDirection = glm::normalize(agent->GetPosition() - towerPosition);
	m_Velocity = glm::vec3(targetDirection.x, targetDirection.y, targetDirection.z) * 25.f;
	GetColour() = glm::vec3(0.5f, 0.2f, 0.07f);
	SetPosition(glm::vec3(towerPosition));
}


//For Numerical Integrations
void Ball::UpdateProjectile(float deltaTime)
{
	if (m_IsEuler)
	{
		//EULER

		glm::vec3 acceleration = m_ForceApplied / m_Mass;
		m_Velocity = m_Velocity + (acceleration * (deltaTime) * 15.f);
		SetPosition(GetPosition() + (m_Velocity * deltaTime));
	}
	else if (m_IsImprovedEuler)
	{
		// IMPROVED EULER

		glm::vec3 acceleration = m_ForceApplied / m_Mass;
		glm::vec3 previousVelocity = m_Velocity;
		m_Velocity = m_Velocity + (acceleration - (0.1f / m_Mass) * m_Velocity) * deltaTime * 15.f;
		SetPosition(GetPosition() + ((previousVelocity + m_Velocity) / 2.f) * deltaTime);
	}
	else if (m_IsRangeKutta)
	{
		// RANGE KUTTA
		glm::vec3 k1, k2, k3, k4, k5;
		glm::vec3 acceleration = m_ForceApplied / m_Mass;

		k1 = deltaTime * acceleration;
		k2 = deltaTime * (acceleration + 0.5f * k1);
		k3 = deltaTime * (acceleration + 0.5f * k2);
		k4 = deltaTime * (acceleration + 0.5f * k3);
		k5 = deltaTime * (acceleration + k4);

		m_Velocity = m_Velocity + (1.f / 6.f) * (k1 + 2.f * k2 + 2.f * k3 + k4 + k5);
		SetPosition(GetPosition() + deltaTime * m_Velocity);
	}
}

//Collision with ground and response
void Ball::CheckCollisionWithGround()
{
	if (m_BounceCount >= m_MaxBounceCount)
	{
		m_ForceApplied = glm::vec3(0);
		SetVelocity(m_ForceApplied);
	}
	else
	{
		float distanceFromGround = GetPosition().y - m_Ground.GetPosition().y;
		if (distanceFromGround < 0.9f)
		{
			m_BounceCount++;
			float dotProduct = glm::dot(m_Normal, m_Velocity);
			m_Velocity = m_Velocity - (1 + m_CoefficientOfRestitution) * (dotProduct)*m_Normal;
			SetVelocity(m_Velocity);
		}
	}
}

//Collision with agent and response
void Ball::CheckCollisionWithAgent()
{
	if (!m_HasHitAgent)
	{
		glm::vec3 v = GetPosition() - m_Agent->GetPosition();
		if (glm::length(v) < GetRadius() + m_Agent->GetRadius())
		{
			auto u = GetVelocity();
			auto v = u - ((2) * (glm::dot(m_Normal, m_Velocity)) * m_Normal);
			SetVelocity(-v * 0.1f);  // negate the velocity vector before setting it as the new velocity
			m_HasHitAgent = true;
			m_Agent->TakeDamage(m_Damage);
		}
	}
}
