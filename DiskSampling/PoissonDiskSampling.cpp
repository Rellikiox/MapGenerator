#include "PoissonDiskSampling.h"

#include <cmath>

PoissonDiskSampling::PoissonDiskSampling(void) {}

PoissonDiskSampling::~PoissonDiskSampling(void) {}

PoissonDiskSampling::PoissonDiskSampling(int p_width, int p_height, double p_min_dist, int p_point_count){
	m_width			= p_width;
	m_height		= p_height;
	m_min_dist		= p_min_dist;
	m_point_count	= p_point_count;
	m_cell_size		= m_min_dist / 1.414214;
	m_grid_width	= ceil(m_width / m_cell_size);
	m_grid_height	= ceil(m_height / m_cell_size);
	m_grid = vector<vector<point *> >(m_grid_width, vector<point *>(m_grid_height, NULL));
}

vector<pair<double,double> > PoissonDiskSampling::Generate(){

	point first_point(rand() % m_width, rand() % m_height);

	m_process.push_back(first_point);
	m_sample.push_back(make_pair(first_point.x, first_point.y));
	int first_point_x = first_point.x/m_cell_size;
	int first_point_y = first_point.y/m_cell_size;
	m_grid[first_point_x][first_point_y] = new point(first_point);

	while( !m_process.empty() ){
		int new_point_index = rand() % m_process.size();
		point new_point = m_process[new_point_index];
		m_process.erase(m_process.begin() + new_point_index);
		
		for(int i = 0; i < m_point_count; i++){
			point new_point_around = generatePointAround(new_point);

			if(inRectangle(new_point_around) && !inNeighbourhood(new_point_around)){ 
				//	cout << "Nuevo punto: (" << new_point_around.x << ", " << new_point_around.y << ")" << endl;
				m_process.push_back(new_point_around);
				m_sample.push_back(make_pair(new_point_around.x, new_point_around.y));
				int new_point_x = new_point_around.x/m_cell_size;
				int new_point_y = new_point_around.y/m_cell_size;
				m_grid[new_point_x][new_point_y] = new point(new_point_around);
			}
		}
	}

	return m_sample;	
}

PoissonDiskSampling::point PoissonDiskSampling::generatePointAround(point p_point){
	double r1 = (double) rand() / RAND_MAX;
	double r2 = (double) rand() / RAND_MAX;

	double radius = m_min_dist * (r1 + 1);

	double angle = 2 * 3.14159265 * r2;

	double new_x = p_point.x + radius * cos(angle);
	double new_y = p_point.y + radius * sin(angle);

	return point(new_x, new_y);
}

bool PoissonDiskSampling::inRectangle(point p_point){
	return (p_point.x >= 0 && p_point.y >= 0 && p_point.x < m_width && p_point.y < m_height);
}

bool PoissonDiskSampling::inNeighbourhood(point p_point){
	vector<point *> cells = getCellsAround(p_point);
	int size = cells.size();
	for(int i = 0; i < size; i++){
		if(cells[i]->distance(p_point) < m_min_dist){
			return true;
		}
	}
	return false;
}

vector<PoissonDiskSampling::point *> PoissonDiskSampling::getCellsAround(point p_point){
	vector<point *> cells;
	int x_index = p_point.x / m_cell_size;
	int y_index = p_point.y / m_cell_size;

	int min_x = max(0, x_index - 1);
	int max_x = min(m_grid_width - 1, x_index + 1);

	int min_y = max(0, y_index - 1);
	int max_y = min(m_grid_height - 1, y_index + 1);

	for(int i = min_x; i <= max_x; i++){
		for(int j = min_y; j <= max_y; j++){
			if(m_grid[i][j] != NULL){
				cells.push_back(m_grid[i][j]);
			}
		}
	}
	return cells;
}