/*
 * LineEquation.cc
 *
 *  Created on: Mar 5, 2012
 *      Author: rellik
 */

#include "LineEquation.h"


equ::equ(Point p1, Point p2) {

	// Calculamos la pendiente
	if (p1.getX() != p2.getX()) {
		vertical = false;
		m = (p2.getY() - p1.getY()) / (p2.getX() - p1.getX());
		b = p1.getY() - p1.getX() * m;
	} else {
		vertical = true;
		m = 0;
		b = p1.getX();
	}

}

equ::equ(Point p, double m_){
	m = m_;
	if(m != 0){
		vertical = false;
		b = p.getY() - p.getX() * m;
	}else{
		vertical = true;
		b = p.getX();
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

void equ::Move(const Vector2D vec) {

	Point p0, p1;

	if (vertical) {
		p0 = Point(b, 0);
		p1 = Point(b, 1);
	} else {
		p0 = Point(0, b);
		p1 = Point(1, m + b);
	}

	p0 += Point(vec.x, vec.y);
	p1 += Point(vec.x, vec.y);

	*this = equ(p0, p1);
}

Point equ::Intersection(equ &e) const {

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

	return Point(x, y);
}

bool equ::Vertical(){
	return vertical;
}

bool equ::Horizontal(){
	return !vertical && m == 0;
}



