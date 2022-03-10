#pragma once

#include "Maths/Maths.h"
class Link
{
	friend class Tree;

public:
	Link(const vec2& head1, const vec2& head2, int id1, int id2);
	~Link() = default;

	const vec2& getMiddle();
	int getLeftID() const { return m_IDLeft; }
	int getRightID() const { return m_IDRight; }

private:
	const vec2& m_Left; 
	const vec2& m_Right;

	int m_IDLeft;
	int m_IDRight;

	vec2 m_Middle;

	float m_BoxSize = 10.0f;
				 // nu stiu ce nume sa le pun...
};