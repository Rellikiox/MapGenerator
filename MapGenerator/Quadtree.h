#pragma once

#include "Math\Vec2.h"
#include <vector>
using namespace std;

// Forward declaration
struct AABB;

template<class T>
class QuadTree {
public:

	QuadTree(void){
		m_northWest = NULL;
		m_northEast = NULL;
		m_southWest = NULL;
		m_southEast = NULL;
		m_divided = false;
	}

	~QuadTree(void) {
		delete m_northWest;
		delete m_northEast;
		delete m_southEast;
		delete m_southWest;
	}

	QuadTree(AABB p_boundary) {
		m_boundary = p_boundary;
		m_divided = false;
		m_northWest = NULL;
		m_northEast = NULL;
		m_southWest = NULL;
		m_southEast = NULL;
	}

	bool Insert(const T p_element, Vec2 p_pos){
		// Exit if the element doesn't belong here
		if(!m_boundary.Contains(p_pos)){
			return false;
		}

		if (!m_divided) {	// Base case

			// Insert the element if there is space in this leaf
			if(m_elements.size() < C_NODE_CAPACITY){
				m_elements.push_back(make_pair(p_element, p_pos));
				return true;
			}

			// If the element doesn't fit, subdivide the leaf
			Subdivide();
		}

		// Try to insert the element in any of its leaves
		if(m_northWest->Insert(p_element, p_pos) || m_northEast->Insert(p_element, p_pos) 
			|| m_southEast->Insert(p_element, p_pos) || m_southWest->Insert(p_element, p_pos)){
				return true;
		}

		// If we get here, something weird happened
		return false;
	}

	vector<T> QueryRange(AABB p_range) {
		vector<T> r_elements;

		if(!m_boundary.Intersects(p_range)){
			return r_elements;
		}

		if (!m_elements.empty()) {	// Base case
			vector<pair<T, Vec2> >::iterator iter;
			for (iter = m_elements.begin(); iter != m_elements.end(); iter++){
				if(p_range.Contains(iter->second)){
					r_elements.push_back(iter->first);
				}
			}
		} else if(m_divided) {	// Recursive case
			vector<T> l_nw_elements = m_northWest->QueryRange(p_range);
			vector<T> l_ne_elements = m_northEast->QueryRange(p_range);
			vector<T> l_se_elements = m_southEast->QueryRange(p_range);
			vector<T> l_sw_elements = m_southWest->QueryRange(p_range);

			r_elements.insert(r_elements.end(), l_nw_elements.begin(), l_nw_elements.end());
			r_elements.insert(r_elements.end(), l_ne_elements.begin(), l_ne_elements.end());
			r_elements.insert(r_elements.end(), l_se_elements.begin(), l_se_elements.end());
			r_elements.insert(r_elements.end(), l_sw_elements.begin(), l_sw_elements.end());
		}

		return r_elements;
	}

	static const int C_NODE_CAPACITY;

private:
	void Subdivide() {
		m_divided = true;

		Vec2 l_new_half = m_boundary.m_half / 2;

		Vec2 l_nw_pos = m_boundary.m_pos - l_new_half;
		AABB l_northWest(l_nw_pos, l_new_half);
		m_northWest = new QuadTree<T>(l_northWest);

		Vec2 l_ne_pos(l_nw_pos.x + m_boundary.m_half.x, l_nw_pos.y);
		AABB l_nothEast(l_ne_pos, l_new_half);
		m_northEast = new QuadTree<T>(l_nothEast);

		Vec2 l_se_pos = m_boundary.m_pos + l_new_half;
		AABB l_southEast(l_se_pos, l_new_half);
		m_southEast = new QuadTree<T>(l_southEast);

		Vec2 l_sw_pos(l_nw_pos.x, l_nw_pos.y + m_boundary.m_half.y);
		AABB l_southWest(l_sw_pos, l_new_half);
		m_southWest = new QuadTree<T>(l_southWest);

		vector<pair<T, Vec2> >::iterator iter;
		for (iter = m_elements.begin(); iter != m_elements.end(); iter++){
			m_northWest->Insert(iter->first, iter->second);
			m_northEast->Insert(iter->first, iter->second);
			m_southEast->Insert(iter->first, iter->second);
			m_southWest->Insert(iter->first, iter->second);
		}
		m_elements.clear();
	}

	AABB m_boundary;

	vector<pair<T, Vec2> > m_elements;

	//	typedef vector<pair<T, Vec2> >::iterator TIter;

	QuadTree *m_northWest;
	QuadTree *m_northEast;
	QuadTree *m_southEast;
	QuadTree *m_southWest;

	bool m_divided;
};

template <class T>
const int QuadTree<T>::C_NODE_CAPACITY = 4;

// Axis-aligned Bounding Box
struct AABB {
	// Center of the region
	Vec2 m_pos;
	// Vector to one of its corners (half the diagonal)
	Vec2 m_half;

	AABB(){};

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

	bool Intersects(const AABB & p_sec) const{
		double l_diff_x = abs(m_pos.x - p_sec.m_pos.x);
		double l_diff_y = abs(m_pos.y - p_sec.m_pos.y);

		if (l_diff_x > (m_half.x + p_sec.m_half.x) || l_diff_y > (m_half.y + p_sec.m_half.y)){
			return false;
		}

		return true;
	}
};