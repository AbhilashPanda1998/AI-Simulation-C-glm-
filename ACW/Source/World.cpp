#include "World.h"
#include "helpers/IRenderHelpers.h"
#include "helpers/IGuiHelpers.h"
#include <iostream>
#include "glm/gtx/string_cast.hpp"

World::World()
{
	InitObjects();
}


void World::InitObjects()
{
	isEuler = true;
	glm::vec3 postion(0, 0, 0);
	float x = rand() % (GRID_WIDTH - 3) + 1;
	float z = rand() % (GRID_DEPTH - 3) + 1;

	for (int i = 0; i < NUMBER_OF_AGENTS; i++)
	{
		/*m_Agents[i].Translate(postion);
		postion.x = 2;*/

		if (i == 0)
		{
			m_Agents[i].GetColour() = glm::vec3(1, 0, 0);
			m_Agents[i].Init("Red", glm::vec3(6, 0, 8), GRID_WIDTH, GRID_DEPTH);
		}

		if (i == 1)
		{
			m_Agents[i].GetColour() = glm::vec3(1, 0, 0);
			m_Agents[i].Init("Red", glm::vec3(6, 0, 18), GRID_WIDTH, GRID_DEPTH);
		}

		if (i == 2)
		{
			m_Agents[i].GetColour() = glm::vec3(0, 0, 1);
			m_Agents[i].Init("Blue", glm::vec3(6, 0, 28), GRID_WIDTH, GRID_DEPTH);
		}

		if (i == 3)
		{
			m_Agents[i].GetColour() = glm::vec3(0, 0, 1);
			m_Agents[i].Init("Blue", glm::vec3(6, 0, 38), GRID_WIDTH, GRID_DEPTH);
		}
	}

	for (int i = 0; i < GRID_WIDTH; i++)
	{
		m_GridHeights[i][0] = 2;
		m_GridHeights[i][GRID_DEPTH - 1] = 2;
	}

	for (int i = 0; i < GRID_DEPTH; i++)
	{
		m_GridHeights[0][i] = 2;
		m_GridHeights[GRID_WIDTH - 1][i] = 2;
	}

	glm::vec3 colours[3] = { glm::vec3(1, 0, 0), glm::vec3(0, 1, 0), glm::vec3(0, 0, 1) };

	for (int i = 0; i < GRID_WIDTH; i++)
	{
		for (int j = 0; j < GRID_DEPTH; j++)
		{
			m_GridColours[i][j] = glm::vec3(0.52f, 0.52f, 0.52f);
		}
	}

	float oneFourthz = GRID_DEPTH / 4;
	float zPos = 0;
	//For Setting Towers transform
	for (int i = 0; i < NUMBER_OF_TOWERS; i++)
	{
		zPos += oneFourthz;
		m_Towers[i].InitializeTower(glm::vec3(GRID_WIDTH / 2, 0, zPos), m_Agents, NUMBER_OF_AGENTS, this);
	}
}

World::~World()
{

}

void World::Update(float pSeconds)
{
	//For Updating Agents
	for (int i = 0; i < NUMBER_OF_AGENTS; i++)
	{
		m_Agents[i].Update(pSeconds);
		AgentToWallCollision(&m_Agents[i]);
	}

	//For Updating Towers
	for (int i = 0; i < NUMBER_OF_TOWERS; i++)
	{
		m_Towers[i].Update(pSeconds, isEuler, isImprovedEuler, isRangeKutta);
	}

	//Check if agent can take ownership of a tower
	ScanAgentsForOwnership();
	BallToBallCollision();
	BallToWallCollision();
}

void World::Render(const IRenderHelpers& pHelper) const
{
	pHelper.Render(this);

	for (int i = 0; i < NUMBER_OF_AGENTS; i++)
	{
		pHelper.Render(&m_Agents[i]);
	}


	//For Rendering Towers
	for (int i = 0; i < NUMBER_OF_TOWERS; i++)
	{
		pHelper.Render(&m_Towers[i]);
		m_Towers[i].Render(pHelper);
	}
}

void World::RenderGui(const IGuiHelpers& pHelper)
{
	pHelper.RenderGui(this);

	for (int i = 0; i < NUMBER_OF_AGENTS; i++)
	{
		pHelper.RenderGui(&m_Agents[i]);
	}
}

//Scans through all the agents that is near the tower and checks their team type and lets that agent to infilitrate the tower
void World::ScanAgentsForOwnership()
{
	for (int i = 0; i < NUMBER_OF_AGENTS; i++)
	{
		for (int j = 0; j < NUMBER_OF_TOWERS; j++)
		{
			if (glm::length(m_Agents[i].GetPosition() - m_Towers[j].GetPosition()) <= 1.0f && m_Agents[i].GetTeamType()!= m_Towers[j].GetTeamType())
			{
				m_Agents[i].StoreLastvelocity();
				m_Agents[i].InfiltrateTower();
				m_Towers[j].StartOwnerShipProcess(&m_Agents[i]);
				return;
			}
		}
	}
}

//Checks collision between all the balls
void World::BallToBallCollision()
{
	for (int i = 0; i < projectiles.size(); i++)
	{
		for (int j = i + 1; j< projectiles.size(); j++)
		{
			glm::vec3 v = projectiles[i]->GetPosition() - projectiles[j]->GetPosition();

			bool collision = false;
			glm::vec3 normal;

			if (glm::length(v) < projectiles[i]->GetRadius() + projectiles[j]->GetRadius())
			{
				collision = true;
				normal = glm::normalize(v);
			}

			if (collision)
			{
				glm::vec3 Vl1 = (((projectiles[i]->GetMass() - 1 * projectiles[j]->GetMass()) * (glm::dot(projectiles[i]->GetVelocity(), normal) * normal) + (projectiles[j]->GetMass()
					+ 1 * projectiles[j]->GetMass()) * (glm::dot(projectiles[j]->GetVelocity(), normal) * normal))) / (projectiles[i]->GetMass() + projectiles[j]->GetMass());

				glm::vec3 Vl2 = (((projectiles[i]->GetMass() + 1 * projectiles[i]->GetMass()) * (glm::dot(projectiles[i]->GetVelocity(), normal) * normal) + (projectiles[j]->GetMass()
					- 1 * projectiles[i]->GetMass()) * (glm::dot(projectiles[j]->GetVelocity(), normal) * normal))) / (projectiles[i]->GetMass() + projectiles[j]->GetMass());

				glm::vec3 V1 = (projectiles[i]->GetVelocity() - (((glm::dot(projectiles[i]->GetVelocity(), normal) * normal) + Vl1)));
				glm::vec3 V2 = (projectiles[j]->GetVelocity() - (((glm::dot(projectiles[j]->GetVelocity(), normal) * normal) + Vl2)));

				projectiles[i]->SetVelocity(-V1);
				projectiles[j]->SetVelocity(-V2);
			}
		}
	}
}

//Checks collision between all the walls
void World::AgentToWallCollision(Agent* agent)
{
	bool collision = false;
	glm::vec3 normal = glm::vec3(0);

	glm::vec3 agent_pos_v;
	if ((agent->GetPosition().x - agent->GetRadius()) < 0.0f || (agent->GetPosition().x + agent->GetRadius()) > GRID_WIDTH - 1)
	{
		collision = true;
		if ((agent->GetPosition().x - agent->GetRadius()) < 0.0f)
		{
			agent_pos_v = agent->GetPosition() - glm::vec3((agent->GetPosition().x - agent->GetRadius()), agent->GetPosition().y, agent->GetPosition().z);
		}
		if ((agent->GetPosition().x + agent->GetRadius()) > GRID_WIDTH - 1)
		{
			agent_pos_v = agent->GetPosition() - glm::vec3((agent->GetPosition().x + agent->GetRadius()), agent->GetPosition().y, agent->GetPosition().z);
		}
		normal = glm::normalize(agent_pos_v);
	}

	if ((agent->GetPosition().z - agent->GetRadius()) < 0.0f || (agent->GetPosition().z + agent->GetRadius()) > GRID_DEPTH - 1)
	{
		collision = true;
		if ((agent->GetPosition().z - agent->GetRadius()) < 0.0f)
		{
			agent_pos_v = agent->GetPosition() - glm::vec3(agent->GetPosition().x, agent->GetPosition().y, (agent->GetPosition().z - agent->GetRadius()));
		}
		if ((agent->GetPosition().z + agent->GetRadius()) > GRID_DEPTH - 1)
		{
			agent_pos_v = agent->GetPosition() - glm::vec3(agent->GetPosition().x, agent->GetPosition().y, (agent->GetPosition().z + agent->GetRadius()));
		}
		normal = glm::normalize(agent_pos_v);
	}

	if (collision)
	{
		float e = 1.0f; //coefficient_of_restitution
		auto initialVelocity = agent->GetVelocity();
		auto finalVelocity = initialVelocity - ((1 + e) * (glm::dot(initialVelocity, normal)) * normal);
		agent->SetVelocity(finalVelocity);
	}
}

//Checks all the ball collision with walls
void World::BallToWallCollision()
{
	for (int i = 0; i < projectiles.size(); i++)
	{
		if (projectiles[i]->GetPosition().x < 1 || (projectiles[i]->GetPosition().x > GRID_WIDTH-1))
		{
			glm::vec3 normal = glm::vec3(1.0, 0.0, 0.0);
			projectiles[i]->SetVelocity((projectiles[i]->GetVelocity() - 2.f * (normal * projectiles[i]->GetVelocity()) * normal));
		}
		else if (projectiles[i]->GetPosition().z < 1 || projectiles[i]->GetPosition().z > GRID_DEPTH -1 )
		{
			glm::vec3 normal = glm::vec3(0.0, 0.0, 1.0);
			projectiles[i]->SetVelocity((projectiles[i]->GetVelocity() - 2.f * (normal * projectiles[i]->GetVelocity()) * normal));
		}
	}
}



