#include "Map.h"
#include "Math/Vec2.h"
#include "noise/noise.h"
#include <ctime>
#include <limits>


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

	// ELEVATION

	AssignOceanCoastLand();

	AssignCornerElevation();

	RedistributeElevations();

	AssignPolygonElevations();

	// MOISTURE

	CalculateDownslopes();

	GenerateRivers();
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
	for each(corner * c in corners) {
		if(!c->IsInsideBoundingBox(map_width, map_height)){
			c->border = true;
			c->ocean = true;
			c->water = true;
		}
	}

	// Determino lo que es agua y lo que es tierra
	for each (corner * c in corners) {
		c->water = !IsIsland(c->position);
	}
}

void Map::AssignOceanCoastLand(){
	queue<center *> centers_queue;
	// Quien es agua o border
	for each (center * c in centers) {
		int adjacent_water = 0;
		for each (corner * q in c->corners) {
			if(q->border){
				c->border = true;
				c->ocean = true;
				q->water = true;
				centers_queue.push(c);
			}
			if(q->water){
				adjacent_water++;
			}
		}
		c->water = (c->ocean || adjacent_water >= c->corners.size() * 0.5);
	}

	// Quien es oceano y quien no
	while(!centers_queue.empty()){
		center * c = centers_queue.front();
		centers_queue.pop();
		for each (center * r in c->centers)	{
			if(r->water && !r->ocean){
				r->ocean = true;
				centers_queue.push(r);
			}
		}
	}

	// Costas de corner
	for each (corner * c in corners) {
		int adj_ocean = 0;
		int adj_land = 0;
		for each (center * p in c->centers) {
			adj_ocean += (int) p->ocean;
			adj_land += (int) !p->water;
		}
		c->ocean = adj_ocean == c->centers.size();
		c->coast = adj_land > 0 && adj_ocean > 0;
		c->water = c->border || (adj_land != c->centers.size() && !c->coast);
	}
}

void Map::AssignCornerElevation(){
	queue<corner *> corner_queue;
	for each (corner * q in corners) {
		if(q->border){
			q->elevation = 0.0;
			corner_queue.push(q);
		}else{
			q->elevation = 99999;
		}
	}

	while(!corner_queue.empty()){
		corner * q = corner_queue.front();
		corner_queue.pop();

		for each (corner * s in q->corners) {
			double new_elevation = q->elevation + 0.01;
			if(!q->water && !s->water){
				new_elevation += 1;
			}
			if(new_elevation < s->elevation){
				s->elevation = new_elevation;
				corner_queue.push(s);
			}
		}
	}

	for each (corner * q in corners) {
		if(q->water){
			q->elevation = 0.0;
		}
	}
}

void Map::RedistributeElevations(){
	vector<corner *> locations = GetLandCorners();
	double SCALE_FACTOR = 1.1;

	sort(locations.begin(), locations.end(), &corner::SortByElevation);

	for(int i = 0; i < locations.size(); i++){
		double y = (double) i / (locations.size() - 1);
		double x = sqrt(SCALE_FACTOR) - sqrt(SCALE_FACTOR * (1-y));
		x = min(x, 1.0);
		locations[i]->elevation = x;
	}
}

void Map::AssignPolygonElevations(){
	for each (center * p in centers) {
		double elevation_sum = 0.0;
		//cout << "Center " << p->index << ":" << endl;
		for each (corner * q in p->corners) {
			elevation_sum += q->elevation;
			//cout << "\t" << q->elevation << endl;			
		}
		p->elevation = elevation_sum / p->corners.size();
		if(p->elevation >= 1)
			cout << "\t--------" << endl << "\t" << p->elevation << endl;
	}
}

void Map::CalculateDownslopes(){
	for each (corner * c in corners) {
		corner * d = c;
		for each (corner * q in c->corners) {
			if(q->elevation <= d->elevation){
				d = q;
			}
		}
		c->downslope = d;
	}
}

void Map::GenerateRivers(){
	int num_rios = (map_height + map_width) / 4;
	for(int i = 0; i < num_rios; i++){
		corner *q = corners[rand()%corners.size()];
		if( q->ocean || q->elevation < 0.3 || q->elevation > 0.9 ) continue;

		while(!q->coast){
			if(q == q->downslope){
				break;
			}
			edge * e = q->GetEdgeWith(q->downslope);
			e->river_volume += 1;
			q->river_volume += 1;
			q = q->downslope;
		}
	}
}


vector<corner *> Map::GetLandCorners(){
	vector<corner *> land_corners;
	for each (corner * c in corners)
		if(!c->water)
			land_corners.push_back(c);
	return land_corners;
}

bool Map::IsIsland(Vec2 position){
	Vec2 center_pos = Vec2(map_width / 2.0, map_height / 2.0);

	position -= center_pos;
	double x_coord = position.x / map_width * 4 + z_coord;
	double y_coord = position.y / map_height * 4 + z_coord;
	double noise_val = noiseMap->GetValue(x_coord, y_coord, z_coord);

	position /= min(map_width, map_height);
	double radius = position.LengthSqrd();

	double factor = 0;
	if(radius > 0.3)
		factor = -1 / log(radius - 0.3) / 10;

	return noise_val >= 0.3*radius + factor;
}

void Map::LloydRelaxation(){
	vector<center *> centros = triangulation->GetCenters();
	vector<Vec2> new_points;
	for each (center * p in centros) {
		Vec2 center_centroid;		
		for each (corner * q in p->corners)	{
			if(q->IsInsideBoundingBox(map_width, map_height)){
				center_centroid += q->position;
			}else{
				Vec2 corner_pos = q->position;
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
		center_centroid /= p->corners.size();
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
