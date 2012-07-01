#pragma once

#include "Math/Vec2.h"
#include <vector>
#include <list>

using namespace std;

struct Biome{
	enum Type{
		Snow,
		Tundra,
		Mountain,
		Taiga,
		Shrubland,
		TemperateDesert,
		TemperateRainForest,
		TemperateDeciduousForest,
		Grassland,
		TropicalRainForest,
		TropicalSeasonalForest,
		SubtropicalDesert,
		Ocean,
		Lake,
		Beach,	

		Size,
		None
	};
};

// Forward declarations
struct edge;
struct corner;

// Center of Voronoi cell; Corner of Delaunay triangle
struct center{
	center() : index(0), position(0,0), water(false), ocean(false), coast(false),
		border(false), biome(Biome::None), elevation(0.0), moisture(0.0) {}

	center(unsigned int i, Vec2 p) : index(i), position(p), ocean(false), water(false), 
		coast(false), border(false), biome(Biome::None), elevation(0.0), moisture(0.0) {}

	unsigned int		index;	
	Vec2				position;

	bool water;
	bool ocean;
	bool coast;
	bool border;
	Biome::Type biome;
	double elevation;
	double moisture;

	vector<edge *>		edges;
	vector<corner *>	corners;
	vector<center *>	centers;

	bool RemoveEdge(edge *e);
	bool RemoveCorner(corner *c);
	edge * GetEdgeWith(center *ce);
	void MakeBorder();
	bool IsInsideBoundingBox(int width, int height);

	typedef vector<center *>::iterator PVIter;
	typedef list<center *>::iterator PLIter;
};

// Edge of Voronoi cell and edge of Delaunay triangle
struct edge{
	edge() : index(0), d0(NULL), d1(NULL), v0(NULL), v1(NULL), river_volume(0.0){};
	edge(unsigned int i, center *e1, center *e2, corner *o1, corner *o2);

	unsigned int		index;

	center				* d0, * d1;
	corner 				* v0, * v1;

	Vec2 voronoi_midpoint;
	double river_volume;

	bool Legalize();
	bool Flip();
	void SwitchCorner(corner *old_corner, corner *new_corner);
	corner * GetOpositeCorner(corner *c);
	center * GetOpositeCenter(center *c);

	typedef vector<edge *>::iterator PVIter;
	typedef list<edge *>::iterator PLIter;
};

// Corner of Voronoi cell; Circumcenter of Delaunay triangle
struct corner{
	corner() : index(0), position(0,0), ocean(false), water(false), coast(false), border(false),
		elevation(0.0), moisture(0.0), river_volume(0.0), downslope(NULL) {}

	corner(unsigned int i, Vec2 p) : index(i), position(p), ocean(false), water(false), coast(false), border(false),
		elevation(0.0), moisture(0.0), river_volume(0.0), downslope(NULL) {}

	unsigned int		index;
	Vec2				position;

	bool ocean;
	bool water;
	bool coast;
	bool border;
	double elevation;
	double moisture;
	double river_volume;
	corner *downslope;

	vector<center *>	centers;
	vector<edge *>		edges;	
	vector<corner *>	corners;

	bool IsPointInCircumcircle(Vec2 p);
	Vec2 CalculateCircumcenter();
	center * GetOpositeCenter(center *c0, center *c1);
	void SwitchAdjacent(corner *old_corner, corner * new_corner);
	bool TouchesCenter(center *c);
	edge * GetEdgeConnecting(center *c0, center *c1);
	center * GetOpositeCenter(edge *e);
	bool IsInsideBoundingBox(int width, int height);
	edge * GetEdgeWith(corner *c);

	typedef vector<corner *>::iterator PVIter;
	typedef list<corner *>::iterator PLIter;

	static bool SortByElevation(corner * c1, corner * c2);
	static bool SortByMoisture(corner * c1, corner * c2);
};
