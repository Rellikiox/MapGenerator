#pragma once

#include "Vec2.h"

#include <vector>
#include <queue>
#include <list>
using namespace std;

class Delaunay {
public:
	// Structures
	struct edge;
	struct corner;

	// Center of Voronoi cell; Corner of Delaunay triangle
	struct center{
		unsigned int		index;	
		Vec2				position;

		vector<edge *>		edges;
		vector<corner *>	corners;	// no se rellena hasta el final

		bool RemoveEdge(edge *e);
		bool RemoveCorner(corner *c);
		edge * GetEdgeWith(center *ce);
	};

	// Edge of Voronoi cell and edge of Delaunay triangle
	struct edge{
		edge() : index(0), d0(NULL), d1(NULL), v0(NULL), v1(NULL){};
		edge(unsigned int i, center *e1, center *e2, corner *o1, corner *o2) : index(i), d0(e1), d1(e2), v0(o1), v1(o2) {};
		unsigned int		index;

		center				* d0, * d1;
		corner 				* v0, * v1;	
			
		bool Legalize();
		bool Flip();
		void SwitchCorner(corner *old_corner, corner *new_corner);
		corner * GetOpositeCorner(corner *c);
	};

	// Corner of Voronoi cell; Circumcenter of Delaunay triangle
	struct corner{
		unsigned int		index;
		Vec2				position;

		vector<center *>	centers;
		vector<edge *>		edges;	// no se rellena hasta el final

		bool IsPointInCircumcircle(Vec2 p);
		Vec2 CalculateCircumcenter();
		center * GetOpositeCenter(center *c0, center *c1);
		void SwitchAdjacent(corner *old_corner, corner * new_corner);
		bool TouchesCenter(center *c);
		edge * GetEdgeConnecting(center *c0, center *c1);
		center * GetOpositeCenter(edge *e);
	};

	// Functions
	Delaunay(void);
	~Delaunay(void);
	Delaunay(const Delaunay &d);
	Delaunay & operator=(const Delaunay &d);

	Delaunay(Vec2 p0, Vec2 p1);
	Delaunay(double x0, double y0, double x1, double y1);

	void CreateBorders(Vec2 p0, Vec2 p1);

	bool AddPoint(Vec2 new_point);
	bool AddPoint(double x, double y);
	bool AddPoint(pair<double,double> new_point);

	bool AddPoints(vector<Vec2> new_points);
	bool AddPoints(vector<pair<double, double>> new_points);

	void Continue();
	bool Step();

	void Finish();
	void CleanUp();

	list<edge *> GetBorders();
	list<center *> GetCenters();
	list<corner *> GetCorners();

private:	
	Vec2 min_xy;
	Vec2 max_xy;
	queue<Vec2>		points;

	unsigned int	edge_index;
	unsigned int	center_index;
	unsigned int	corner_index;
	list<edge *>	borders;
	list<center *>	centers;
	list<corner *>	corners;

	bool CheckPoint(Vec2 point, corner * & c, edge * & e);
	void OrderPoints(vector<corner *> & corners);
};



