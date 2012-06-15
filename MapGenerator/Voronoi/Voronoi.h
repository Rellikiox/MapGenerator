#pragma once

#include "Structures.h"
#include "../Math/Vec2.h"

#include <queue>
#include <vector>
using namespace std;

struct gt;

class Voronoi {
public:
	Voronoi(void);
	~Voronoi(void);

	void AddPoint(Vec2 new_point);
	void AddPoints(vector<Vec2> new_points);

	void Continue();
	void Step();

	void Finish();
	void GetEdges();
private:
	Vec2 min_xy;
	Vec2 max_xy;
	void updateBoundingBox(Vec2 p);

	priority_queue<Vec2, vector<Vec2>, gt> unprocessed_points;
	priority_queue<event*, vector<event*>, gt> unprocessed_events;
	
	arc *root; // First item in the parabolic front linked list.

	void process_point();
	void process_event();

	void check_circle_event(arc *i, double x0);
	bool circle(Vec2 a, Vec2 b, Vec2 c, double *x, Vec2 *o);

	bool intersect(Vec2 p, arc *i, Vec2 *result = 0);
	Vec2 intersection(Vec2 p0, Vec2 p1, double l);

	void finish_edges();
};