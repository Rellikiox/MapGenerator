#include "Map.h"
#include "Math/Vec2.h"
#include "noise/noise.h"
#include <ctime>

Map::Map(void) {
	triangulation = NULL;
	srand(time_t(NULL));
	z_coord = ((double) rand() / RAND_MAX ) * 2 - 1;
}

Map::~Map(void) {
}

Map::Map(int width, int height, int point_count) {
	map_width = width;
	map_height = height;
	triangulation = new Delaunay(Vec2(0,0), Vec2(map_width, map_height));

	srand(time(NULL));
	for(int i = 0; i < point_count; i++){
		triangulation->AddPoint(Vec2(rand()%map_width, rand()%map_height));
	}

	z_coord = ((double) rand() / RAND_MAX ) * 2 - 1;
}

void Map::Generate() {

	GeneratePolygons();

	GenerateLand();
}

void Map::GeneratePolygons() {
	triangulation->Continue();

	LloydRelaxation();
	LloydRelaxation();

	triangulation->Finish();

	edges = triangulation->GetBorders();
	corners = triangulation->GetCorners();
	centers = triangulation->GetCenters();
}

void Map::GenerateLand() {

	corner::PVIter corner_iter, corners_end = corners.end();
	for(corner_iter = corners.begin(); corner_iter != corners_end; corner_iter++){
		if(!(*corner_iter)->IsInsideBoundingBox(map_width, map_height)){
			center::PVIter del_center_iter, del_centers_end = (*corner_iter)->centers.end();
			for(del_center_iter = (*corner_iter)->centers.begin(); del_center_iter != del_centers_end; del_center_iter++){

			}
		}
	}
}

void Map::LloydRelaxation(){
	vector<center *> centros = triangulation->GetCenters();
	vector<Vec2> new_points;
	center::PVIter center_iter, centers_end = centros.end();
	for(center_iter = centros.begin(); center_iter != centers_end; center_iter++){
		Vec2 center_centroid;
		corner::PVIter corner_iter, corners_end = (*center_iter)->corners.end();
		for(corner_iter = (*center_iter)->corners.begin(); corner_iter != corners_end; corner_iter++){
			center_centroid += (*corner_iter)->position;
		}
		center_centroid /= (*center_iter)->corners.size();

		new_points.push_back(center_centroid);
	}

	triangulation->CleanUp();
	triangulation->CreateBorders(Vec2(0,0), Vec2(map_width, map_height));
	triangulation->AddPoints(new_points);
	triangulation->Continue();
}

vector<center *> Map::GetCenters(){
	return centers;
}

vector<corner *> Map::GetCorners(){
	return corners;
}

vector<edge *> Map::GetEdges(){
	return edges;
}
