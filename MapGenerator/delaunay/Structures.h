#pragma once

#include "../Math/Vec2.h"

#include <vector>
using namespace std;

struct edge;
struct corner;

// Center of Voronoi cell; Corner of Delaunay triangle
struct center{
	unsigned int		index;	
	Vec2				position;

	vector<center *>	neighbors;
	vector<edge *>		borders;
	vector<corner *>	corners;
};

// Edge of Voronoi cell; Edge of Delaunay triangle
struct edge{
	unsigned int		index;

	center *			d0, d1;
	center *			v0, v1;
};

// Corner of Voronoi cell; Circumcenter of Delaunay triangle
struct corner{
	unsigned int		index;
	Vec2				position;

	vector<center *>	touches;
	vector<edge *>		portrudes;
	vector<corner *>	adjacent;
};