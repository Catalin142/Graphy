#include "Core/GrPch.h"
#include "Link.h"

Link::Link(const vec2& head1, const vec2& head2, int id1, int id2) : m_Left(head1), m_Right(head2), m_IDLeft(id1), m_IDRight(id2)
{
	getMiddle();
}

void Link::getMiddle()
{
	float distX = std::abs(m_Left.x - m_Right.x);

	if (m_Left.x > m_Right.x)
		m_Middle.x = m_Right.x + distX / 2.0f;

	else
		m_Middle.x = m_Left.x + distX / 2.0f;

	float distY = std::abs(m_Left.y - m_Right.y);

	if (m_Left.y > m_Right.y)
		m_Middle.y = m_Right.y + distY / 2.0f;

	else
		m_Middle.y = m_Left.y + distY / 2.0f;

	m_Middle = m_Middle - vec2(m_BoxSize / 2.0f, m_BoxSize / 2.0f);
}
