#include "Map.h"
#include "Math/Vec2.h"
#include "noise/noise.h"


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

	srand(time_t(NULL));
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

	//LloydRelaxation();
	//LloydRelaxation();

	list<Delaunay::edge *> delaunay_edges = triangulation->GetBorders();
	list<Delaunay::edge *>::iterator edge_iter, edges_end = delaunay_edges.end();
	for(edge_iter = delaunay_edges.begin(); edge_iter != edges_end; edge_iter++){
		Map::edge * borde = new Map::edge(*edge_iter);
		edges.push_back(borde);
	}

	list<Delaunay::corner *> delaunay_corners = triangulation->GetCorners();
	list<Delaunay::corner *>::iterator corner_iter, corners_end = delaunay_corners.end();
	for(corner_iter = delaunay_corners.begin(); corner_iter != corners_end; corner_iter++){
		Map::corner * esquina = new Map::corner(*corner_iter);
		corners.push_back(esquina);
	}
	
	list<Delaunay::center *> delaunay_centers = triangulation->GetCenters();
	list<Delaunay::center *>::iterator center_iter, centers_end = delaunay_centers.end();
	for(center_iter = delaunay_centers.begin(); center_iter != centers_end; center_iter++){
		Map::center * centro = new Map::center(*center_iter);
		centers.push_back(centro);
	}
}

void Map::GenerateLand() {
}

void Map::LloydRelaxation(){
	list<Delaunay::center *> centros = triangulation->GetCenters();
	vector<Vec2> new_points;
	list<Delaunay::center *>::iterator center_iter, centers_end = centros.end();
	for(center_iter = centros.begin(); center_iter != centers_end; center_iter++){
		Vec2 center_centroid;
		vector<Delaunay::corner *>::iterator corner_iter, corners_end = (*center_iter)->corners.end();
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