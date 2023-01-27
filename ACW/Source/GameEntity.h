#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
using namespace std;

class IRenderHelpers;
class IGuiHelpers;

class GameEntity
{
private:

	unsigned int m_ID;
	static unsigned int s_NextID;
	glm::mat4 m_Model;
	glm::vec3 m_Colour;

protected:
	float m_Radius = 0;

public:
	GameEntity() : m_Colour(1.0f, 1.0f, 1.0f), m_ID(s_NextID)
	{
		s_NextID++;
	}

	unsigned int ID() const { return m_ID; }

	glm::mat4 GetModel() const { return m_Model; }
	glm::mat4& GetModel() { return m_Model; }
	glm::vec3 GetColour() const { return m_Colour; }
	glm::vec3 & GetColour() { return m_Colour; }
	float GetRadius() const{ return m_Radius; }
	void Scale(glm::vec3 pScale)
	{
		m_Model = glm::scale(m_Model, pScale);
	}

	void Translate(glm::vec3 pTranslation)
	{
		m_Model = glm::translate(m_Model, pTranslation);
	}

	void Rotate(float pRadians, glm::vec3 pAxis)
	{
		m_Model = glm::rotate(m_Model, pRadians, pAxis);
	}
	
	virtual void Render(const IRenderHelpers&) const = 0;
	virtual void RenderGui(const IGuiHelpers&);
};
