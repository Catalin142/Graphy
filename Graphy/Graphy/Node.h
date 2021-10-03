#pragma once

#include "Maths/Maths.h"
#include "Tree.h"

class Node
{
	friend class Tree;
	friend class TreeEditor;
	friend class GraphEditor;

public:
	Node(int id, const std::string& number, const vec2& position, int radius);
	Node() = default;

private:
	void Render();
	void SetPosition(const vec2& pos) { m_Position = pos; }

	bool isPressed(int x, int y);
	bool isHovered(int x, int y);

	void Refresh();

	int m_ID;
	std::string m_Name;

	vec2 m_Position;
	int m_Radius;

	vec2 m_TextPosition;
	int m_TextSize = 2;
	vec3 m_Color = vec3(0.0f, 0.0f, 0.0f);
	vec3 m_HoveredColor = m_Color;

	bool m_isMoved = false;
	bool m_isHovered = false;

	int m_InternalDegree = 0;
	int m_ExternalDegree = 0;
};