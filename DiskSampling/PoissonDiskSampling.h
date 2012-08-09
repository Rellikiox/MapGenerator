#pragma once

#include <vector>
using namespace std;

class PoissonDiskSampling {
public:
	PoissonDiskSampling(void);
	~PoissonDiskSampling(void);

	vector<pair<double,double> > Generate(int width, int height, double min_dist, int point_count);

private:



	struct point {
		point(double x_, double y_) : x(x_), y(y_) {};

		int gridIndex(double cell_size, int map_width){
			int x_index = x / cell_size;
			int y_index = y / cell_size;

			return x_index + y_index * map_width;
		};

		double distance(point p){
			return sqrt((x - p.x)*(x - p.x) + (y - p.y)*(y - p.y));
		}

		double x, y;
	};

	point generatePointAround(point p, double min_dist);
	bool inRectangle(point p, int width, int height);
	bool inNeighbourhood(vector<vector<point *> > grid, point p, double min_dist, double cell_size, int width, int height);
	vector<point *> getCellsAround(vector<vector<point *> > grid, point p, double cell_size, int width, int height);
};

