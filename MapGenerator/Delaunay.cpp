#include "Delaunay.h"
#include "Math/LineEquation.h" 
#include <iostream>
using namespace std;

Delaunay::Delaunay(void) {
	edge_index = 0;
	corner_index = 0;
	center_index = 0;
}

Delaunay::~Delaunay(void) {
	this->CleanUp();
}

Delaunay::Delaunay( const Delaunay &d ) {

}

Delaunay & Delaunay::operator=(const Delaunay &d){
	if(this != &d){

	}
	return *this;
}

Delaunay::Delaunay( Vec2 p0, Vec2 p1 ) {
	corner_index = 0;
	edge_index = 0;
	center_index = 0;
	CreateBorders(p0,p1);	
}

Delaunay::Delaunay( double x0, double y0, double x1, double y1 ) {
	corner_index = 0;
	edge_index = 0;
	center_index = 0;
	CreateBorders(Vec2(x0,y0), Vec2(x1,y1));
}

void Delaunay::CreateBorders(Vec2 p0, Vec2 p1) {
	min_xy = p0;
	max_xy = p1;

	// Creamos los center y los colocamos
	center *d0 = new center();
	d0->index = center_index++;
	d0->position = p0;
	center *d1 = new center();
	d1->position = Vec2(p0.x, p1.y);
	d1->index = center_index++;
	center *d2 = new center();
	d2->position = Vec2(p1.x, p0.y);
	d2->index = center_index++;
	center *d3 = new center();
	d3->position = p1;
	d3->index = center_index++;

	centers.push_back(d0);
	centers.push_back(d1);
	centers.push_back(d2);
	centers.push_back(d3);

	// Creamos los corner y los colocamos
	corner *v0 = new corner();
	v0->index = corner_index++;
	v0->centers.push_back(d0);
	v0->centers.push_back(d1);
	v0->centers.push_back(d2);
	v0->position = v0->CalculateCircumcenter();

	corner *v1 = new corner();
	v1->index = corner_index++;
	v1->centers.push_back(d1);
	v1->centers.push_back(d2);
	v1->centers.push_back(d3);
	v1->position = v1->CalculateCircumcenter();

	// Unimos centers con corners
	d0->corners.push_back(v0);
	d1->corners.push_back(v0);
	d1->corners.push_back(v1);
	d2->corners.push_back(v0);
	d2->corners.push_back(v1);
	d3->corners.push_back(v1);

	corners.push_back(v0);
	corners.push_back(v1);


	// Creamos los edge
	edge *e0 = new edge(edge_index++, d0, d1, v0, NULL);
	edge *e1 = new edge(edge_index++, d0, d2, v0, NULL);
	edge *e2 = new edge(edge_index++, d1, d2, v0, v1);
	edge *e3 = new edge(edge_index++, d1, d3, v1, NULL);
	edge *e4 = new edge(edge_index++, d2, d3, v1, NULL);

	borders.push_back(e0);
	borders.push_back(e1);
	borders.push_back(e2);
	borders.push_back(e3);
	borders.push_back(e4);

	d0->edges.push_back(e1);
	d0->edges.push_back(e0);
	d1->edges.push_back(e0);
	d1->edges.push_back(e2);
	d1->edges.push_back(e3);
	d2->edges.push_back(e1);
	d2->edges.push_back(e2);
	d2->edges.push_back(e4);
	d3->edges.push_back(e3);
	d3->edges.push_back(e4);

	v0->edges.push_back(e0);
	v0->edges.push_back(e1);
	v0->edges.push_back(e2);
	v1->edges.push_back(e2);
	v1->edges.push_back(e3);
	v1->edges.push_back(e4);
}

bool Delaunay::AddPoint( Vec2 new_point ) {
	center::PVIter center_iter, centers_end_iter = centers.end();
	for(center_iter = centers.begin(); center_iter != centers_end_iter; center_iter++)
		if((*center_iter)->position == new_point)
			return false;

	points.push(new_point);
	return true;
}

bool Delaunay::AddPoint( double x, double y ) {
	return AddPoint(Vec2(x,y));
}

bool Delaunay::AddPoint( pair<double,double> new_point ) {
	return AddPoint(Vec2(new_point.first, new_point.second));
}

bool Delaunay::AddPoints( vector<Vec2> new_points ) {
	bool point_existed = false;
	while(!new_points.empty()){
		point_existed = AddPoint(*new_points.begin());
		new_points.erase(new_points.begin());
	}
	return !point_existed;
}

bool Delaunay::AddPoints( vector<pair<double, double>> new_points ) {
	bool point_existed = false;
	while(!new_points.empty()){
		Vec2 new_point(new_points[0].first, new_points[0].second);
		point_existed = AddPoint(new_point);
		new_points.erase(new_points.begin());
	}
	return !point_existed;
}

void Delaunay::Continue() {
	while(!points.empty()){
		Step();
	}
}

bool Delaunay::Step() {
	if(points.empty())
		return false;
	// Corner en el que esta el punto
	corner	* point_corner = NULL;
	// Edge en el que cae el punto
	edge	* point_edge = NULL;

	Vec2 point = points.front();
	points.pop();

	if(CheckPoint(point, point_corner, point_edge)){
		if(point_corner != NULL){

			// Creo el nuevo center

			center *new_center = new center();
			new_center->index = center_index++;
			new_center->position = point;
			centers.push_back(new_center);

			// Me guardo los antiguos center

			center *center_0 = point_corner->centers[0];
			center *center_1 = point_corner->centers[1];
			center *center_2 = point_corner->centers[2];

			// Elimino el antiguo corner de los antiguos center

			center_0->RemoveCorner(point_corner);
			center_1->RemoveCorner(point_corner);
			center_2->RemoveCorner(point_corner);

			// Creo los nuevos corner

			corner *new_corner_01 = new corner();
			new_corner_01->index = corner_index++;
			corners.push_back(new_corner_01);
			corner *new_corner_12 = new corner();
			new_corner_12->index = corner_index++;
			corners.push_back(new_corner_12);
			corner *new_corner_20 = new corner();
			new_corner_20->index = corner_index++;
			corners.push_back(new_corner_20);

			// Añado los nuevos corner a los antiguos center

			center_0->corners.push_back(new_corner_20);
			center_0->corners.push_back(new_corner_01);
			center_1->corners.push_back(new_corner_01);
			center_1->corners.push_back(new_corner_12);
			center_2->corners.push_back(new_corner_12);
			center_2->corners.push_back(new_corner_20);

			// Me guardo los antiguos edge

			edge *old_edge_01 = point_corner->GetEdgeConnecting(center_0, center_1);
			edge *old_edge_12 = point_corner->GetEdgeConnecting(center_1, center_2);
			edge *old_edge_20 = point_corner->GetEdgeConnecting(center_2, center_0);

			// Cambio los edge antiguos para que apunten a los nuevos corner

			old_edge_01->SwitchCorner(point_corner, new_corner_01);
			old_edge_12->SwitchCorner(point_corner, new_corner_12);
			old_edge_20->SwitchCorner(point_corner, new_corner_20);

			// Creo los nuevos edge y les pongo sus center y corner

			edge *new_edge_p0 = new edge(edge_index++, new_center, center_0, new_corner_01, new_corner_20);
			borders.push_back(new_edge_p0);
			edge *new_edge_p1 = new edge(edge_index++, new_center, center_1, new_corner_01, new_corner_12);
			borders.push_back(new_edge_p1);
			edge *new_edge_p2 = new edge(edge_index++, new_center, center_2, new_corner_12, new_corner_20);
			borders.push_back(new_edge_p2);

			// Añado los nuevos edge a los antiguos center

			center_0->edges.push_back(new_edge_p0);
			center_1->edges.push_back(new_edge_p1);
			center_2->edges.push_back(new_edge_p2);

			// Añado los center a los edge a los nuevos corner, les añado sus center y calculo sus posiciones

			new_corner_01->edges.push_back(new_edge_p0);
			new_corner_01->edges.push_back(old_edge_01);
			new_corner_01->edges.push_back(new_edge_p1);
			new_corner_01->centers.push_back(center_0);
			new_corner_01->centers.push_back(center_1);
			new_corner_01->centers.push_back(new_center);
			new_corner_01->position = new_corner_01->CalculateCircumcenter();

			new_corner_12->edges.push_back(new_edge_p1);
			new_corner_12->edges.push_back(old_edge_12);
			new_corner_12->edges.push_back(new_edge_p2);
			new_corner_12->centers.push_back(center_1);
			new_corner_12->centers.push_back(center_2);
			new_corner_12->centers.push_back(new_center);
			new_corner_12->position = new_corner_12->CalculateCircumcenter();

			new_corner_20->edges.push_back(new_edge_p2);
			new_corner_20->edges.push_back(old_edge_20);
			new_corner_20->edges.push_back(new_edge_p0);
			new_corner_20->centers.push_back(center_2);
			new_corner_20->centers.push_back(center_0);
			new_corner_20->centers.push_back(new_center);
			new_corner_20->position = new_corner_20->CalculateCircumcenter();

			// Añado los nuevos edge al nuevo center

			new_center->edges.push_back(new_edge_p0);
			new_center->edges.push_back(new_edge_p1);
			new_center->edges.push_back(new_edge_p2);

			// Añado los nuevos corner al nuevo center

			new_center->corners.push_back(new_corner_01);
			new_center->corners.push_back(new_corner_12);
			new_center->corners.push_back(new_corner_20);

			old_edge_01->Legalize();
			old_edge_12->Legalize();
			old_edge_20->Legalize();

			corners.erase(
				remove(corners.begin(), corners.end(), point_corner),
				corners.end());
			delete point_corner;

		}else if(point_edge != NULL){



		}else{
			return false;
		}

		return true;
	}
	return false;
}

bool Delaunay::CheckPoint( Vec2 point, corner * & out_corner, edge * & out_edge ) {
	out_edge = NULL;
	out_corner = NULL;

	corner::PVIter corner_iter, corners_end = corners.end();
	for(corner_iter = corners.begin(); corner_iter != corners_end; corner_iter++){

		Vec2 a = (*corner_iter)->centers[0]->position;
		Vec2 b = (*corner_iter)->centers[1]->position;
		Vec2 c = (*corner_iter)->centers[2]->position;

		Vec2 v11(a,b), v12(a,point);
		double c1 = v11.CrossProduct(v12);

		Vec2 v21(b,c), v22(b,point);
		double c2 = v21.CrossProduct(v22);

		Vec2 v31(c,a), v32(c,point);
		double c3 = v31.CrossProduct(v32);

		if(c1 == 0 && c2*c3 > 0){	// Esta en la linea ab
			out_edge = (*corner_iter)->GetEdgeConnecting((*corner_iter)->centers[0],  (*corner_iter)->centers[1]);
			return true;
		}else if(c2 == 0 && c3*c1 > 0){	// Esta en la linea bc
			out_edge = (*corner_iter)->GetEdgeConnecting((*corner_iter)->centers[1],  (*corner_iter)->centers[2]);
			return true;
		}else if(c3 == 0 && c1*c2 > 0){	// Esta en la linea ca
			out_edge = (*corner_iter)->GetEdgeConnecting((*corner_iter)->centers[2],  (*corner_iter)->centers[0]);
			return true;
		}else if(c1*c2 > 0 && c2*c3 > 0){ // Esta dentro
			out_corner = *corner_iter;
			return true;
		}
	}
	return false;
}

vector<edge *> Delaunay::GetBorders() {
	return borders;
}

vector<center *> Delaunay::GetCenters() {
	return centers;
}

vector<corner *> Delaunay::GetCorners() {
	return corners;
}

void Delaunay::CleanUp() {
	while(!borders.empty()){
		delete borders.back();
		borders.pop_back();
	}
	while(!centers.empty()){
		delete centers.back();
		centers.pop_back();
	}
	while(!corners.empty()){
		delete corners.back();
		corners.pop_back();
	}
	while(!points.empty())
		points.pop();
	edge_index = 0;
	center_index = 0;
	corner_index = 0;
}

void Delaunay::Finish() {
	center::PVIter center_iter, centers_end = centers.end();
	for(center_iter = centers.begin(); center_iter != centers_end; center_iter++){
		OrderPoints((*center_iter)->corners);
	}
}

void Delaunay::OrderPoints( vector<corner *> & corners ) {
	vector<corner *> result;
	if (corners.size() >= 4) {
		corner * leftMostPoint = corners[0];
		corner::PVIter corner_iter, corners_end = corners.end();
		for (corner_iter = corners.begin() + 1; corner_iter != corners.end(); corner_iter++) {
			if ((*corner_iter)->position.x < leftMostPoint->position.x 
				|| ((*corner_iter)->position.x == leftMostPoint->position.x && (*corner_iter)->position.y < leftMostPoint->position.y)) {
				leftMostPoint = *corner_iter;
			}
		}
		corner * current = leftMostPoint;
		int leftMostInt;

		do {
			result.insert(result.begin(), current);
			if (current == corners[0])
				leftMostInt = 1;
			else
				leftMostInt = 0;
			for (unsigned int i = leftMostInt + 1; i < corners.size(); i++) {
				Vec2 v1(current->position, corners[leftMostInt]->position);
				Vec2 v2(current->position, corners[i]->position);
				if (v1.CrossProduct(v2) > 0) {
					leftMostInt = i;
				}
			}
			current = corners[leftMostInt];
		} while (current != result.back());
		corners = result;
	}
}




