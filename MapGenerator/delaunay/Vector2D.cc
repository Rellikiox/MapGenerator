/*
 * Vector.cc
 *
 *  Created on: Feb 13, 2012
 *      Author: rellik
 */

#include "Vector2D.h"
const double M_PI = 3.14159265;

ostream & operator<<(ostream &os, const Vector2D &vec) {

	os << "[" << vec.x << "|" << vec.y << "]";

	return os;
}

Vector2D::Vector2D() {
	x = 0;
	y = 0;
}

Vector2D::~Vector2D() {
	x = 0;
	y = 0;
}

Vector2D::Vector2D(const Vector2D & vec) {
	x = vec.x;
	y = vec.y;
}

Vector2D & Vector2D::operator =(const Vector2D & vec) {
	if (this != &vec) {
		x = vec.x;
		y = vec.y;
	}
	return *this;
}

Vector2D::Vector2D(Point a, Point b) {
	x = b.getX() - a.getX();
	y = b.getY() - a.getY();
}

Vector2D::Vector2D(double x_, double y_) {
	x = x_;
	y = y_;
}

Vector2D::Vector2D(double ang) {
	x = cos(ang * M_PI / 180);
	y = sin(ang * M_PI / 180);
}

Vector2D::Vector2D(Point p) {
	x = p.getX();
	y = p.getY();
}

Vector2D & Vector2D::operator+=(const Vector2D & vec) {
	x += vec.x;
	y += vec.y;

	return *this;
}

Vector2D & Vector2D::operator -=(const Vector2D & vec) {
	x -= vec.x;
	y -= vec.y;
	return *this;
}

Vector2D & Vector2D::operator *=(const double & fac) {
	x *= fac;
	y *= fac;
	return *this;
}

Vector2D & Vector2D::operator/=(const double &fac) {
	x /= fac;
	y /= fac;
	return *this;
}

bool Vector2D::operator==(const Vector2D &vec) const {
	return ((x == vec.x) && (y == vec.y));
}

bool Vector2D::operator!=(const Vector2D &vec) const {
	return ((x != vec.x) || (y != vec.y));
}

double Vector2D::Length() const {
	return sqrt(x * x + y * y);
}

double Vector2D::LengthSqrd() const {
	return (x * x + y * y);
}

void Vector2D::Normalize() {
	double vector_length = this->Length();

	if (vector_length > 0) {
		this->x /= vector_length;
		this->y /= vector_length;
	}
}

void Vector2D::Reflect(const Vector2D & vec) {
	*this += 2.0 * this->dotProduct(vec) * vec.Reverse();
}

void Vector2D::Truncate(double maxLength) {
	if (this->Length() > maxLength) {
		this->Normalize();

		*this *= maxLength;
	}
}

void Vector2D::perpCCW() {
	double auxX = x;
	x = -y;
	y = auxX;
}

void Vector2D::perpCW() {
	double auxX = x;
	x = y;
	y = -auxX;
}

void Vector2D::Rotate(double angle) {
	angle *= M_PI / 180;
	double newX = x * cos(angle) - y * sin(angle);
	double newY = x * sin(angle) + y * cos(angle);

	x = newX;
	y = newY;
}

void Vector2D::Rotate(Vector2D obj, Vector2D guia) {
	double angle = guia.Angle(obj);
	this->Rotate(angle);
}

void Vector2D::Translate(Vector2D vec) {
	x += vec.x;
	y += vec.y;
}

void Vector2D::Translate(Point p) {
	x += p.getX();
	y += p.getY();
}

void Vector2D::localToWorld(Vector2D Position, Vector2D Heading) {
	double angle = Vector2D(0, 10).Angle(Heading);
	this->Rotate(angle);
	this->Translate(Position);
}

double Vector2D::dotProduct(const Vector2D &vec) const {
	return (x * vec.x + y * vec.y);
}

double Vector2D::crossProduct(const Vector2D &vec) const {
	return (x * vec.y - y * vec.x);
}

double Vector2D::Distance(const Vector2D &vec) const {
	double ySep = vec.y - y;
	double xVec = vec.x - x;

	return sqrt(ySep * ySep + xVec * xVec);
}

double Vector2D::DistanceSqrd(const Vector2D &vec) const {
	double ySep = vec.y - y;
	double xVec = vec.x - x;

	return (ySep * ySep + xVec * xVec);
}

bool Vector2D::isZero() const {
	return (x == 0 && y == 0);
}

bool Vector2D::Sign(const Vector2D &vec) const {
	return (y * vec.x > x * vec.y);
}

double Vector2D::Angle(const Vector2D &vec) const {
	if (this->Length() == 0 || vec.Length() == 0)
		return 0;
	else {
		double angle = acos(
				this->dotProduct(vec) / (this->Length() * vec.Length()))
				* 180/M_PI;

		if (this->crossProduct(vec) < 0)
			angle *= -1;

		return angle;
	}
}

double Vector2D::Angle() const {
	if (this->Length() == 0)
		return 0;
	Vector2D vec(1, 0);
	return acos(this->dotProduct(vec) / (this->Length() * vec.Length())) * 180
			/ M_PI;
}

Vector2D Vector2D::Reverse() const {
	return Vector2D(-x, -y);
}

Point Vector2D::toPoint() {
	return Point(x, y);
}

//////// NON-MEMBER ////////

Vector2D operator+(const Vector2D &lhs, const Vector2D &rhs) {
	Vector2D res(lhs);

	res.x += rhs.x;
	res.y += rhs.y;

	return res;
}

Vector2D operator-(const Vector2D &lhs, const Vector2D &rhs) {
	Vector2D res(lhs);

	res.x -= rhs.x;
	res.y -= rhs.y;

	return res;
}

Vector2D operator*(const double &fac, const Vector2D &lhs) {
	Vector2D res(lhs);

	res.x *= fac;
	res.y *= fac;

	return res;
}

Vector2D operator*(const Vector2D &rhs, const double &fac) {
	Vector2D res(rhs);

	res.x *= fac;
	res.y *= fac;

	return res;
}

Vector2D operator/(const Vector2D &lhs, const double &fac) {
	Vector2D res(lhs);

	res.x /= fac;
	res.y /= fac;

	return res;
}

Vector2D Normalize(const Vector2D &vec) {
	double vector_length = vec.Length();

	Vector2D result;

	if (vector_length > 0) {
		result.x = vec.x / vector_length;
		result.y = vec.y / vector_length;
	}
	return result;
}

double Distance(const Vector2D &vec1, const Vector2D &vec2) {
	double ssX = vec1.x - vec2.x;
	ssX *= ssX;

	double ssY = vec1.y - vec2.y;
	ssY *= ssY;

	return sqrt(ssX + ssY);
}

