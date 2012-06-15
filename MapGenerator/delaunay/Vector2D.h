/*
 * Vector.h
 *
 *  Created on: Feb 13, 2012
 *      Author: rellik
 */

#ifndef VECTOR_H_
#define VECTOR_H_

#include <cmath>
#include "Point.h"

class Point;

class Vector2D{
	friend ostream & operator<<(ostream &os, const Vector2D &vec);
public:
	Vector2D();
	~Vector2D();
	Vector2D(const Vector2D &vec);
	Vector2D & operator=(const Vector2D &vec);

	Vector2D(Point a_, Point b_);
	Vector2D(double x, double y);
	Vector2D(double ang);
	Vector2D(Point p);

	Vector2D & operator+=(const Vector2D &vec);
	Vector2D & operator-=(const Vector2D &vec);
	Vector2D & operator*=(const double &fac);
	Vector2D & operator/=(const double &fac);

	bool operator==(const Vector2D &vec) const;
	bool operator!=(const Vector2D &vec) const;

	void Normalize();
	void Reflect(const Vector2D &vec);
	void Truncate(double maxLength);
	void perpCCW();
	void perpCW();
	void Rotate(double angle);
	void Rotate(Vector2D obj, Vector2D guia);
	void Translate(Vector2D vec);
	void Translate(Point p);
	void localToWorld(Vector2D Position, Vector2D Heading);

	double dotProduct(const Vector2D &vec) const;
	double crossProduct(const Vector2D &vec) const;
	double Distance(const Vector2D &vec) const;
	double DistanceSqrd(const Vector2D &vec) const;
	bool Sign(const Vector2D &vec) const;
	double Angle(const Vector2D &vec) const;
	double Angle() const;

	bool isZero() const;
	double Length() const;
	double LengthSqrd() const;
	Vector2D Reverse() const;

	Point toPoint();

	double x;
	double y;
};

Vector2D operator+(const Vector2D &lhs, const Vector2D &rhs);
Vector2D operator-(const Vector2D &lhs, const Vector2D &rhs);
Vector2D operator*(const double &fac, const Vector2D &rhs);
Vector2D operator*(const Vector2D &lhs, const double &fac);
Vector2D operator/(const Vector2D &lhs, const double &fac);
Vector2D Normalize(const Vector2D &vec);
double Distance(const Vector2D &vec1, const Vector2D &vec2);

#endif /* VECTOR_H_ */
