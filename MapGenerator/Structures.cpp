
#include "Structures.h"
#include "Math/LineEquation.h" 
#include <iostream>

edge::edge(unsigned int i, center *e1, center *e2, corner *o1, corner *o2) : index(i), d0(e1), d1(e2), v0(o1), v1(o2), river_volume(0.0) {
	Vec2 pv0 = v0 == NULL ? d0->position + (d1->position - d0->position) / 2 : v0->position;
	Vec2 pv1 = v1 == NULL ? d0->position + (d1->position - d0->position) / 2 : v1->position;
	voronoi_midpoint = (pv0 + pv1) / 2;
};

bool center::RemoveEdge( edge *e ) {
	vector<edge *>::iterator edge_iter;
	for(edge_iter = edges.begin(); edge_iter != edges.end(); edge_iter++){
		if(*edge_iter == e){
			edges.erase(edge_iter);
			return true;
		}
	}
	return false;
}

edge * center::GetEdgeWith(center *ce){
	vector<edge *>::iterator edge_iter;
	for(edge_iter = edges.begin(); edge_iter != edges.end(); edge_iter++){
		if((*edge_iter)->d0 == ce || (*edge_iter)->d1 == ce){
			return *edge_iter;
		}
	}
	return NULL;
}

bool center::RemoveCorner( corner *c ) {
	vector<corner *>::iterator corner_iter, corners_end = corners.end();
	for(corner_iter = corners.begin(); corner_iter != corners_end; corner_iter++){
		if(*corner_iter == c){
			corners.erase(corner_iter);
			return true;
		}			
	}
	return false;
}

void center::MakeBorder(){
	border = true;
	water = true;
	ocean = true;

	corner::PVIter corner_iter, corners_end = corners.end();
	for(corner_iter = corners.begin(); corner_iter != corners_end; corner_iter++){
		(*corner_iter)->border = true;
		(*corner_iter)->water = true;
		(*corner_iter)->ocean = true;
	}
}

bool center::IsInsideBoundingBox(int width, int height){
	if(position.x < 0 || position.x >= width || position.y < 0 || position.y >= height)
		return false;

	return true;
}

bool center::Contains(Vec2 p_pos){

	if(corners.size() < 3)
		return false;

	Vec2 l_first_sec(corners[0]->position, corners[1]->position);
	Vec2 l_first_pos(corners[0]->position, p_pos);
	bool sign = l_first_sec.CrossProduct(l_first_pos) > 0;
	corner::PVIter iter;
	for(iter = corners.begin() + 1; iter != corners.end() - 1; iter++){
		Vec2 l_a_b((*iter)->position, (*(iter+1))->position);
		Vec2 l_a_p((*iter)->position, p_pos);
		if(sign != (l_a_b.CrossProduct(l_a_p) > 0)){
			return false;
		}
	}

	return true;
}

pair<Vec2,Vec2> center::GetBoundingBox(){
	double l_min_x = corners[0]->position.x, l_max_x = corners[0]->position.x;
	double l_min_y = corners[0]->position.y, l_max_y = corners[0]->position.y;

	corner::PVIter iter;
	for(iter = corners.begin() + 1; iter != corners.end(); iter++){
		if ((*iter)->position.x < l_min_x) {
			l_min_x = (*iter)->position.x;
		} else if ((*iter)->position.x > l_max_x) {
			l_max_x = (*iter)->position.x;
		}
		if ((*iter)->position.y < l_min_y) {
			l_min_y = (*iter)->position.y;
		} else if ((*iter)->position.y > l_max_y) {
			l_max_y = (*iter)->position.y;
		}
	}

	Vec2 l_min_pos(l_min_x, l_min_y);
	Vec2 l_max_pos(l_max_x, l_max_y);
	Vec2 l_half_diagonal(Vec2(l_min_pos, l_max_pos) / 2);

	return make_pair(l_min_pos + l_half_diagonal, l_half_diagonal);
}

void center::SortCorners(){
	corner * item = NULL;
	int hole = 0;
	for(int i = 1; i < corners.size(); i++){
		item = corners[i];
		hole = i;
		while( hole > 0 && GoesBefore(item->position, corners[hole - 1]->position)){
			corners[hole] = corners[hole - 1];
			hole--;
		}
		corners[hole] = item;
	}
}

bool center::GoesBefore(Vec2 p_a, Vec2 p_b){
	if((p_a - position).x >= 0 && (p_b - position).x < 0)
		return true;

	if(p_a.x == 0 && p_b.x == 0)
		return p_a.y < p_b.y;

	Vec2 ca(position, p_a);
	Vec2 cb(position, p_b);
	return ca.CrossProduct(cb) > 0;
}

bool edge::Legalize() {

	if(v0 == NULL || v1 == NULL)
		return false;

	if(v0->IsPointInCircumcircle(v1->GetOpositeCenter(this)->position))
		return this->Flip();

	return false;
}

bool edge::Flip() {
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

	e00->Legalize();
	e01->Legalize();
	e10->Legalize();
	e11->Legalize();

	return true;
}

void edge::SwitchCorner(corner *old_corner, corner *new_corner){
	if(v0 == old_corner){
		v0 = new_corner;
	}else if (v1 == old_corner){
		v1 = new_corner;
	}
}

corner * edge::GetOpositeCorner( corner *c ) {
	if(v0 == c){
		return v1;
	}else if(v1 == c){
		return v0;
	}
	return NULL;
}

center * edge::GetOpositeCenter(center *c){
	if(d0 == c){
		return d1;
	}else if(d1 == c){
		return d0;
	}
	return NULL;
}

edge * corner::GetEdgeWith(corner *c){
	for each (edge * e in edges)
		if(e->v0 == c || e->v1 == c)
			return e;
	return NULL;
}

bool corner::IsPointInCircumcircle(Vec2 p) {

	if(this->centers.empty())
		return false;

	center *point_circumference = this->centers[0];

	Vec2 corner_center(this->position, point_circumference->position);
	Vec2 corner_p(this->position, p);

	return corner_center.Length() >= corner_p.Length();
}

Vec2 corner::CalculateCircumcenter() {
	if(this->centers.size() != 3)
		return Vec2();

	Vec2 a = centers[0]->position;
	Vec2 b = centers[1]->position;
	Vec2 c = centers[2]->position;
	/*
	double d = 2 * (a.x*(b.y - c.y) + b.x*(c.y - a.y) + c.x*(a.y - b.y));
	double new_x = ((a.x*a.x + a.y*a.y)*(b.y - c.y) + (b.x*b.x + b.y*b.y)*(c.y - a.y) + (c.x*c.x + c.y*c.y)*(a.y - b.y)) / d;
	double new_y = ((a.x*a.x + a.y*a.y)*(c.x - b.x) + (b.x*b.x + b.y*b.y)*(a.x - c.x) + (c.x*c.x + c.y*c.y)*(b.x - a.x)) / d;

	return Vec2(new_x, new_y);*/


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

	return ab_bisector.Intersection(bc_bisector);
}

center * corner::GetOpositeCenter( center *c0, center *c1 ) {
	vector<center *>::iterator center_iter, centers_end = centers.end();
	for(center_iter = centers.begin(); center_iter != centers_end; center_iter++){
		if(*center_iter != c0 && *center_iter != c1)
			return *center_iter;
	} 
	return NULL;
}

center * corner::GetOpositeCenter( edge *e ) {
	bool found = false;
	for each (edge * ed in edges) {
		if(ed == e){
			found = true;
			break;
		}
	}
	// Si no he encontrado el edge devuelvo NULL
	if(!found)
		return NULL;

	for each (center * c in centers) {
		if(c != e->d0 && c != e->d1)
			return c;
	}

	return NULL;
}

bool corner::TouchesCenter( center *c ) {
	for(int i = 0; i < centers.size(); i++)
		if(centers[i] == c)
			return true;
	return false;
}

edge * corner::GetEdgeConnecting( center *c0, center *c1 ) {
	vector<edge *>::iterator edge_iter, edges_end = edges.end();
	for(edge_iter = edges.begin(); edge_iter != edges_end; edge_iter++){
		if(((*edge_iter)->d0 == c0 && (*edge_iter)->d1 == c1) || ((*edge_iter)->d1 == c0 && (*edge_iter)->d0 == c1) )
			return *edge_iter;
	}
	return NULL;
}

bool corner::IsInsideBoundingBox(int width, int height){
	if(position.x < 0 || position.x >= width || position.y < 0 || position.y >= height)
		return false;

	return true;
}

bool corner::SortByElevation(corner *c1, corner *c2){
	return c1->elevation < c2->elevation;
}

bool corner::SortByMoisture(corner *c1, corner *c2){
	return c1->moisture < c2->moisture;
}