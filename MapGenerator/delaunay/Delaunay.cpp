#include "Delaunay.h"
#include "../Math/LineEquation.h" 
#include <iostream>
using namespace std;

bool Delaunay::center::RemoveEdge( edge *e ) {
	vector<edge *>::iterator edge_iter;
	for(edge_iter = edges.begin(); edge_iter != edges.end(); edge_iter++){
		if(*edge_iter == e){
			edges.erase(edge_iter);
			return true;
		}
	}
	return false;
}

Delaunay::edge * Delaunay::center::GetEdgeWith(center *ce){
	vector<edge *>::iterator edge_iter;
	for(edge_iter = edges.begin(); edge_iter != edges.end(); edge_iter++){
		if((*edge_iter)->d0 == ce || (*edge_iter)->d1 == ce){
			return *edge_iter;
		}
	}
	return NULL;
}

bool Delaunay::center::RemoveCorner( corner *c ) {
	vector<corner *>::iterator corner_iter, corners_end = corners.end();
	for(corner_iter = corners.begin(); corner_iter != corners_end; corner_iter++){
		if(*corner_iter == c){
			corners.erase(corner_iter);
			return true;
		}			
	}
	return false;
}

bool Delaunay::edge::Legalize() {

	if(v0 == NULL || v1 == NULL)
		return false;

	if(v0->IsPointInCircumcircle(v1->GetOpositeCenter(this)->position))
		return this->Flip();
	
	return false;
}

bool Delaunay::edge::Flip() {
	// Obtengo los nuevos center para d0 y d1
	center *cen0 = v0->GetOpositeCenter(d0, d1);
	center *cen1 = v1->GetOpositeCenter(d0, d1);

	if(cen0 == NULL || cen1 == NULL)
		return false;

	edge * e00 = cen0->GetEdgeWith(d0);	// nv0
	edge * e01 = cen0->GetEdgeWith(d1);	// nv1
	edge * e10 = cen1->GetEdgeWith(d0);	// nv0
	edge * e11 = cen1->GetEdgeWith(d1);	// nv1

	e00->SwitchCorner(v0, v1);
	e11->SwitchCorner(v1, v0);

	// Elimino this de d0 y d1
	d0->RemoveEdge(this);
	d1->RemoveEdge(this);

	d0->RemoveCorner(v0);
	d1->RemoveCorner(v1);

	cen0->corners.push_back(v1);
	cen1->corners.push_back(v0);

	cen0->edges.push_back(this);
	cen1->edges.push_back(this);

	v0->centers.clear();
	v0->edges.clear();

	v0->centers.push_back(cen0);
	v0->centers.push_back(cen1);
	v0->centers.push_back(d1);
	v0->position = v0->CalculateCircumcenter();

	v0->edges.push_back(this);
	v0->edges.push_back(e01);
	v0->edges.push_back(e11);

	v1->centers.clear();
	v1->edges.clear();

	v1->centers.push_back(cen0);
	v1->centers.push_back(d0);
	v1->centers.push_back(cen1);
	v1->position = v1->CalculateCircumcenter();
	
	v1->edges.push_back(this);
	v1->edges.push_back(e00);
	v1->edges.push_back(e10);

	d0 = cen0;
	d1 = cen1;

	return true;
}

void Delaunay::edge::SwitchCorner(corner *old_corner, corner *new_corner){
	if(v0 == old_corner){
		v0 = new_corner;
	}else if (v1 == old_corner){
		v1 = new_corner;
	}
}

Delaunay::corner * Delaunay::edge::GetOpositeCorner( corner *c ) {
	if(v0 == c){
		return v1;
	}else if(v1 == c){
		return v0;
	}
	return NULL;
}

bool Delaunay::corner::IsPointInCircumcircle(Vec2 p) {

	if(this->centers.empty())
		return false;

	center *point_circumference = this->centers[0];

	Vec2 corner_center(this->position, point_circumference->position);
	Vec2 corner_p(this->position, p);

	return corner_center.Length() >= corner_p.Length();
}

Vec2 Delaunay::corner::CalculateCircumcenter() {
	if(this->centers.size() != 3)
		return Vec2();

	Vec2 a = centers[0]->position;
	Vec2 b = centers[1]->position;
	Vec2 c = centers[2]->position;

	double d = 2 * (a.x*(b.y - c.y) + b.x*(c.y - a.y) + c.x*(a.y - b.y));
	double new_x = ((a.x*a.x + a.y*a.y)*(b.y - c.y) + (b.x*b.x + b.y*b.y)*(c.y - a.y) + (c.x*c.x + c.y*c.y)*(a.y - b.y)) / d;
	double new_y = ((a.x*a.x + a.y*a.y)*(c.x - b.x) + (b.x*b.x + b.y*b.y)*(a.x - c.x) + (c.x*c.x + c.y*c.y)*(b.x - a.x)) / d;

	return Vec2(new_x, new_y);

	/*
	Vec2 ab_midpoint((a.x + b.x) / 2, (a.y + b.y) / 2);
	equ ab_equ(a, b);

	equ ab_bisector;
	if(ab_equ.Vertical()){
		ab_bisector = equ(ab_midpoint, Vec2(ab_midpoint.x + 1, ab_midpoint.y));
	}else if(ab_equ.Horizontal()){
		ab_bisector = equ(ab_midpoint, Vec2(ab_midpoint.x, ab_midpoint.y + 1));
	}else{
		double ab_bisector_slope = ab_equ.m == 0 ? 0 :  -1 / ab_equ.m;
		ab_bisector = equ(ab_midpoint, ab_bisector_slope);
	}

	Vec2 bc_midpoint((b.x + c.x) / 2, (b.y + c.y) / 2);
	equ bc_equ(b, c);

	equ bc_bisector;
	if(bc_equ.Vertical()){
		bc_bisector = equ(bc_midpoint, Vec2(bc_midpoint.x + 1, bc_midpoint.y));
	}else if(bc_equ.Horizontal()){
		bc_bisector = equ(bc_midpoint, Vec2(bc_midpoint.x, bc_midpoint.y + 1));
	}else{
		double bc_bisector_slope = bc_equ.m == 0 ? 0 :  -1 / bc_equ.m;
		bc_bisector = equ(bc_midpoint, bc_bisector_slope);
	}

	return ab_bisector.Intersection(bc_bisector);*/
}

Delaunay::center * Delaunay::corner::GetOpositeCenter( center *c0, center *c1 ) {
	vector<center *>::iterator center_iter, centers_end = centers.end();
	for(center_iter = centers.begin(); center_iter != centers_end; center_iter++){
		if(*center_iter != c0 && *center_iter != c1)
			return *center_iter;
	} 
	return NULL;
}

Delaunay::center * Delaunay::corner::GetOpositeCenter( edge *e ) {
	vector<edge *>::iterator edge_iter, edges_end = edges.end();
	for(edge_iter = edges.begin(); edge_iter != edges_end; edge_iter++){
		if(*edge_iter == e)
			break;
	}
	// Si no he encontrado el edge devuelvo NULL
	if(edge_iter == edges_end)
		return NULL;

	vector<center *>::iterator center_iter, centers_end = centers.end();
	for(center_iter = centers.begin(); center_iter != centers_end; center_iter++){
		if(*center_iter != e->d0 && *center_iter != e->d1)
			return *center_iter;
	}

	return NULL;
}

bool Delaunay::corner::TouchesCenter( center *c ) {
	for(int i = 0; i < centers.size(); i++)
		if(centers[i] == c)
			return true;
	return false;
}

Delaunay::edge * Delaunay::corner::GetEdgeConnecting( center *c0, center *c1 ) {
	vector<edge *>::iterator edge_iter, edges_end = edges.end();
	for(edge_iter = edges.begin(); edge_iter != edges_end; edge_iter++){
		if(((*edge_iter)->d0 == c0 && (*edge_iter)->d1 == c1) || ((*edge_iter)->d1 == c0 && (*edge_iter)->d0 == c1) )
			return *edge_iter;
	}
	cout << "llego" << endl;
	return NULL;
}

Delaunay::Delaunay(void) {

}

Delaunay::~Delaunay(void) {

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
	CreateBorders(Vec2(x0,y0), Vec2(x1,y1));
}

void Delaunay::CreateBorders(Vec2 p0, Vec2 p1) {
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
	vector<center *>::iterator center_iter, centers_end_iter = centers.end();
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

	vector<corner *>::iterator corner_iter, corners_end = corners.end();
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

vector<Delaunay::edge *> Delaunay::GetBorders() {
	return borders;
}

vector<Delaunay::center *> Delaunay::GetCenters() {
	return centers;
}

vector<Delaunay::corner *> Delaunay::GetCorners() {
	return corners;
}




