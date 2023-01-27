#pragma once
#include "GameEntity.h"
#include "Agent.h"
#include "Tower.h"
#include "Ball.h"

class World : public GameEntity
{
public:
	const static int GRID_WIDTH = 50;
	const static int GRID_DEPTH = 50;
	bool isImprovedEuler = false;
	bool isEuler = false;
	bool isRangeKutta = false;
	vector<Ball*> projectiles;
	struct CollisonDatAgentWall {
		bool m_Collision;
		glm::vec3 m_Normal;
	};

private:

	const static int NUMBER_OF_AGENTS = 4;
	const static int NUMBER_OF_TOWERS = 4;

	int m_GridHeights[GRID_WIDTH][GRID_DEPTH];
	glm::vec3 m_GridColours[GRID_WIDTH][GRID_DEPTH];

	Agent m_Agents[NUMBER_OF_AGENTS];
	Tower m_Towers[NUMBER_OF_TOWERS];
	void InitObjects();
	void ScanAgentsForOwnership();
	void BallToBallCollision();
	void BallToWallCollision();
	void AgentToWallCollision(Agent* agent);

public:

	World();
	~World();

	int GetGridHeight(int pX, int pZ) const
	{
		return m_GridHeights[pX][pZ];
	}

	glm::vec3 GetGridColour(int pX, int pZ) const
	{
		return m_GridColours[pX][pZ];
	}

	void Update(float pSeconds);
	void Render(const IRenderHelpers&) const;
	void RenderGui(const IGuiHelpers&);
};

