/*
 * LineEquation.cc
 *
 *  Created on: Mar 5, 2012
 *      Author: Chutaos Team
 */

#include "Point.h"
#include <cmath>
#include "LineEquation.h"



ostream&
operator<<(ostream &os, const Point &p) {

	os << "(" << p.x << "," << p.y << "," << p.z << ")";

	return os;
}

double Point::distance(Point p1, Point p2) {

	double ssX = p1.x - p2.x;
	ssX *= ssX;

	double ssY = p1.y - p2.y;
	ssY *= ssY;

	double ssZ = p1.z - p2.z;
	ssZ *= ssZ;

	double distancia = sqrt(ssX + ssY + ssZ);

	return distancia;
}

bool Point::operator==(const Point &p) const {
	bool iguales = true;

	if (abs(x - p.x) >= 0.1)
		iguales = false;
	else if (abs(y - p.y) >= 0.1)
		iguales = false;
	else if (abs(z - p.z) >= 0.1)
		iguales = false;

	return iguales;
}

Point & Point::operator+=(const Point &p) {
	this->x += p.x;
	this->y += p.y;

	return *this;
}

Point & Point::operator-=(const Point &p) {
	this->x -= p.x;
	this->y -= p.y;

	return *this;
}

/*
 bool Point::operator==(const Point &p) {
 bool iguales = true;

 if (x != p.x)
 iguales = false;
 else if (y != p.y)
 iguales = false;
 else if (z != p.z)
 iguales = false;

 return iguales;
 }*/

Point Point::interP(const Point &p1, const Point &p2) {
	double x = abs(p1.x - p2.x);
	double y = abs(p1.y - p2.y);
	Point p(x, y);
	return p;
}

bool Point::isInRadius(const Point &p, double radius) const {
	return (distance(*this, p) <= radius);
}

bool Point::operator!=(const Point &p) const {
	bool dif = false;

	if (abs(x - p.x) >= 0.001) {
		dif = true;
	} else if (abs(y - p.y) >= 0.001)
		dif = true;
	else if (abs(z - p.z) >= 0.001)
		dif = true;

	return dif;
}

void Point::dispersion(Point point) {
	x = point.getX() + ((rand() % 60) - 30);
	y = point.getY() + (((rand() % 60) - 30));
	z = point.z;
}

bool Point::Colinear(Point a, Point b, Point c, double epsilon) {

	equ primeraLinea(a, b);

	double resultado = primeraLinea(c.getX());

	double diferencia = abs(resultado - c.getY());

	return diferencia <= epsilon;
}

