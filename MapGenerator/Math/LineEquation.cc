/*
 * LineEquation.cc
 *
 *  Created on: Mar 5, 2012
 *      Author: rellik
 */

#include "LineEquation.h"


equ::equ(Vec2 p1, Vec2 p2) {

	// Calculamos la pendiente
	if (p1.x != p2.x) {
		vertical = false;
		m = (p2.y - p1.y) / (p2.x - p1.x);
		b = p1.y - p1.y * m;
	} else {
		vertical = true;
		m = 0;
		b = p1.x;
	}

}

equ::equ(Vec2 p, double m_){
	m = m_;
	if(m != 0){
		vertical = false;
		b = p.y - p.x * m;
	}else{
		vertical = true;
		b = p.x;
	}
}

equ::equ(const equ& e) {
	m = e.m;
	b = e.b;
	vertical = e.vertical;
}

equ & equ::operator=(const equ &e) {
	if (this != &e) {
		m = e.m;
		b = e.b;
		vertical = e.vertical;
	}
	return *this;
}

equ::~equ() {
	m = 0;
	b = 0;
	vertical = false;
}

double equ::operator()(const double x) {
	return (x * m + b);
}

void equ::Move(const Vec2 vec) {

	Vec2 p0, p1;

	if (vertical) {
		p0 = Vec2(b, 0);
		p1 = Vec2(b, 1);
	} else {
		p0 = Vec2(0, b);
		p1 = Vec2(1, m + b);
	}

	p0 += Vec2(vec.x, vec.y);
	p1 += Vec2(vec.x, vec.y);

	*this = equ(p0, p1);
}

Vec2 equ::Intersection(equ &e) const {

	double x;
	double y;

	if (this->m != e.m) {
		if (this->vertical) {
			x = this->b;
			y = e(x);
		} else if (e.vertical) {
			x = e.b;
			y = x * this->m + this->b;
		} else {
			x = (e.b - this->b) / (this->m - e.m);
			y = e(x);
		}
	} else {
		if (this->vertical == e.vertical) {
			x = 0;
			y = 0;
		} else {
			if (this->vertical) { // this es vertical, e es horizontal
				x = this->b;
				y = e.b;
			} else { // this es horizontal, e es vertical
				x = e.b;
				y = this->b;
			}
		}
	}

	return Vec2(x, y);
}

bool equ::Vertical(){
	return vertical;
}

bool equ::Horizontal(){
	return !vertical && m == 0;
}



