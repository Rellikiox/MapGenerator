#include "PoissonDiskSampling.h"

#include <cmath>
#include <ctime>

PoissonDiskSampling::PoissonDiskSampling(void) {}

PoissonDiskSampling::~PoissonDiskSampling(void) {}
#include <iostream>
using namespace std;
vector<pair<double,double> > PoissonDiskSampling::Generate(int width, int height, double min_dist, int point_count){

	double cell_size = min_dist / 1.414214;

	vector<vector<point *> > grid;
	vector<point> process;
	vector<pair<double,double> > sample;

	int grid_width = ceil(width / cell_size);
	int grid_height = ceil(height / cell_size);
	for(int i = 0; i < grid_width; i++){
		vector<point *> aux;
		for(int j = 0; j < grid_height; j++){
			aux.push_back(NULL);
		}
		grid.push_back(aux);
	}
	srand(time(NULL));
	point first_point(rand() % width, rand() % height);

	process.push_back(first_point);
	sample.push_back(make_pair(first_point.x, first_point.y));
	grid[(int)(first_point.x/cell_size)][(int)(first_point.y/cell_size)] = & first_point;

	while( !process.empty() ){
		int new_point_index = rand() % process.size();
		point new_point = process[new_point_index];
		process.erase(process.begin() + new_point_index);

		for(int i = 0; i < point_count; i++){
			point new_point_around = generatePointAround(new_point, min_dist);
			
			if(inRectangle(new_point_around, width, height) && !inNeighbourhood(grid, new_point_around, min_dist, cell_size, width, height)){ //TODO
				cout << "Nuevo punto: (" << new_point_around.x << ", " << new_point_around.y << ")" << endl;
				process.push_back(new_point_around);
				sample.push_back(make_pair(new_point_around.x, new_point_around.y));
				grid[(int)(new_point_around.x/cell_size)][(int)(new_point_around.y/cell_size)] = & new_point_around;
			}

		}
	}

	return sample;	
}

PoissonDiskSampling::point PoissonDiskSampling::generatePointAround(point p, double min_dist){
	double r1 = (rand() % 1000) / 1000.0;
	double r2 = (rand() % 1000) / 1000.0;

	double radius = min_dist * (r1 + 1);

	double angle = 2 * 3.14159265 * r2;
	//cout << cos(0) << " " << cos(3.14159265 / 2) << endl;
	double new_x = p.x + radius * cos(angle);
	double new_y = p.y + radius * sin(angle);

	return point(new_x, new_y);
}

bool PoissonDiskSampling::inRectangle(point p, int width, int height){
	return (p.x >= 0 && p.y >= 0 && p.x < width && p.y < height);
}

bool PoissonDiskSampling::inNeighbourhood(vector<vector<point *> > grid, point p, double min_dist, double cell_size, int width, int height){
	vector<point *> cells = getCellsAround(grid, p, cell_size, width, height);
	for(int i = 0; i < cells.size(); i++){
		if(cells[i]->distance(p) < min_dist){
			return true;
		}
	}
	return false;
}

vector<PoissonDiskSampling::point *> PoissonDiskSampling::getCellsAround(vector<vector<point *> > grid, point p, double cell_size, int width, int height){
	vector<point *> cells;
	int x_index = p.x / cell_size;
	int y_index = p.y / cell_size;

	int min_x = max(0, x_index - 1);
	int max_x = min((int) (width / cell_size) - 1, x_index + 1);

	int min_y = max(0, y_index - 1);
	int max_y = min((int) (height / cell_size) - 1, y_index + 1);

	for(int i = min_x; i <= max_x; i++){
		for(int j = min_y; j <= max_y; j++){
			if(grid[i][j] != NULL){
				cells.push_back(grid[i][j]);
			}
		}
	}
	return cells;
}