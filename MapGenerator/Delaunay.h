#pragma once

#include "Structures.h"
#include "Math/Vec2.h"

#include <vector>
#include <queue>
using namespace std;

class Delaunay {
public:

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

	vector<edge *> GetBorders();
	vector<center *> GetCenters();
	vector<corner *> GetCorners();

private:	
	Vec2 min_xy;
	Vec2 max_xy;
	queue<Vec2>		points;

	unsigned int	edge_index;
	unsigned int	center_index;
	unsigned int	corner_index;
	vector<edge *>	borders;
	vector<center *>	centers;
	vector<corner *>	corners;

	bool CheckPoint(Vec2 point, corner * & c, edge * & e);
	void OrderPoints(vector<corner *> & corners);
};



