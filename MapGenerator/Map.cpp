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

	z_coord = ((double) rand() / RAND_MAX ) * 4;
	z_coord = 0.979888;
	cout << z_coord << endl;
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
	noiseMap = new noise::module::Perlin();	

	// Establezco los bordes del mapa
	corner::PVIter corner_iter, corners_end = corners.end();
	for(corner_iter = corners.begin(); corner_iter != corners_end; corner_iter++){
		if(!(*corner_iter)->IsInsideBoundingBox(map_width, map_height)){
			center::PVIter del_center_iter, del_centers_end = (*corner_iter)->centers.end();
			for(del_center_iter = (*corner_iter)->centers.begin(); del_center_iter != del_centers_end; del_center_iter++){
				(*del_center_iter)->MakeBorder();				
			}
		}
	}

	// Determino lo que es agua y lo que es tierra
	for(corner_iter = corners.begin(); corner_iter != corners_end; corner_iter++){
		(*corner_iter)->water = !IsIsland((*corner_iter)->position);
	}

	center::PVIter center_iter, centers_end = centers.end();
	for(center_iter = centers.begin(); center_iter != centers_end; center_iter++){
		int corner_count = 0;
		corners_end = (*center_iter)->corners.end();
		for(corner_iter = (*center_iter)->corners.begin(); corner_iter != corners_end; corner_iter++){
			if((*corner_iter)->water){
				corner_count++;
			}
		}
		if((double) corner_count / (*center_iter)->corners.size() > 0.6){
			(*center_iter)->water = true;
		}else{
			(*center_iter)->water = false;
		}
	}
}

void Map::AssingOceanCoastLand(){

}

bool Map::IsIsland(Vec2 position){
	Vec2 center_pos = Vec2(map_width / 2.0, map_height / 2.0);

	position -= center_pos;
	double x_coord = position.x / map_width * 4 + z_coord;
	double y_coord = position.y / map_height * 4 + z_coord;
	double noise_val = noiseMap->GetValue(x_coord, y_coord, z_coord);

	position /= min(map_width, map_height);
	double radius = position.LengthSqrd();

	double factor = -1 / log(radius) / 10;

	if( noise_val < 0.3*radius + factor){
		return false;
	}else{
		return true;
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
			//center_centroid += (*corner_iter)->position;

			if((*corner_iter)->IsInsideBoundingBox(map_width, map_height)){
				center_centroid += (*corner_iter)->position;
			}else{
				Vec2 corner_pos = (*corner_iter)->position;
				if(corner_pos.x < 0){
					corner_pos.x = 0;
				}else if(corner_pos.x >= map_width){
					corner_pos.x = map_width;
				}
				if(corner_pos.y < 0){
					corner_pos.y = 0;
				}else if(corner_pos.y >= map_height){
					corner_pos.y = map_height;
				}
				center_centroid += corner_pos;
			}
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
