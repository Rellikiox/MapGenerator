
#include "Structures.h"
#include "Math/LineEquation.h" 

edge::edge(unsigned int i, center *e1, center *e2, corner *o1, corner *o2) : index(i), d0(e1), d1(e2), v0(o1), v1(o2) {
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