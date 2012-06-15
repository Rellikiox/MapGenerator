/*
 * LineEquation.h
 *
 *  Created on: Mar 5, 2012
 *      Author: rellik
 */

#ifndef LINEEQUATION_H_
#define LINEEQUATION_H_

#include "Vec2.h"

struct equ{
	double m;
	double b;
	bool vertical;

	equ(){};
	equ(Vec2 a, Vec2 b);
	equ(Vec2 p, double m);
	equ(const equ& e);
	equ & operator=(const equ &e);
	~equ();

	double operator()(const double x);

	void Move(const Vec2 vec);
	Vec2 Intersection(equ &e) const;
	bool Horizontal();
	bool Vertical();
};

#endif /* LINEEQUATION_H_ */
