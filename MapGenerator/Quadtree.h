#pragma once

#include "Structures.h"
#include "Math\Vec2.h"

// Forward declaration
struct AABB;

class Quadtree {
public:
	Quadtree(void);
	~Quadtree(void);
};


// Axis-aligned Bounding Box
struct AABB {
	// Center of the region
	Vec2 m_pos;
	// Vector to one of its corners (half the diagonal)
	Vec2 m_half;

	AABB(Vec2 p_pos, Vec2 p_half){
		m_pos = p_pos;
		m_half = p_half;
	}

	bool Contains(const Vec2 p_point) const{
		Vec2 l_min_point = m_pos - m_half;
		if(p_point.x >= l_min_point.x && p_point.y >= l_min_point.y){
			Vec2 l_max_point = m_pos + m_half;
			return p_point.x <= l_max_point.x && p_point.y <= l_max_point.y;
		}
		return false;
	}

	bool Intersects(const AABB & l_sec) const{
		return false;
	}
}