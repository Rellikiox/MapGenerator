#include "Vec2.h"
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846264338327
#endif

#ifndef EQ_THRESHOLD
#define EQ_THRESHOLD 0.00001
#endif

ostream & operator<<(ostream &os, const Vec2 &v){
	os << "(" << v.x << "," << v.y << ")";
	return os;
}

Vec2::Vec2() {
	x = 0.0;
	y = 0.0;
}

Vec2::~Vec2() {
	x = 0.0;

	y = 0.0;
}

Vec2::Vec2(const Vec2 & v) {
	Copy(v);
}

Vec2 & Vec2::operator =(const Vec2 & v) {
	if (this != &v) {
		Copy(v);
	}
	return *this;
}

Vec2::Vec2(double x, double y) {
	this->x = x;
	this->y = y;
}

Vec2::Vec2(const Vec2 &v1, const Vec2 &v2){
	this->x = v2.x - v1.x;
	this->y = v2.y - v1.y;
}

Vec2::Vec2(double angle) {
	this->x = cos(angle * M_PI / 180);
	this->y = sin(angle * M_PI / 180);
}

Vec2 & Vec2::operator +=(const Vec2 & v) {
	this->x += v.x;
	this->y += v.y;

	return *this;
}

Vec2 & Vec2::operator +=(const double & f) {
	this->x += f;
	this->y += f;

	return *this;
}

Vec2 & Vec2::operator -=(const Vec2 & v) {
	this->x -= v.x;
	this->y -= v.y;

	return *this;
}

Vec2 & Vec2::operator -=(const double & f) {
	this->x -= f;
	this->y -= f;

	return *this;
}

Vec2 & Vec2::operator *=(const double & f) {
	this->x *= f;
	this->y *= f;

	return *this;
}

Vec2 & Vec2::operator /=(const double & f) {
	this->x /= f;
	this->y /= f;

	return *this;
}

bool Vec2::operator ==(const Vec2 & v) const {

	double diff_x = abs(this->x - v.x);
	double diff_y = abs(this->y - v.y);

	return diff_x < EQ_THRESHOLD && diff_y < EQ_THRESHOLD;
}

bool Vec2::operator !=(const Vec2 & v) const {
	
	double diff_x = abs(this->x - v.x);
	double diff_y = abs(this->y - v.y);

	return diff_x > EQ_THRESHOLD || diff_y > EQ_THRESHOLD;
}

void Vec2::Normalize() {
	double mod = this->Length();

	if (mod > 0) {
		this->x /= mod;
		this->y /= mod;
	}
}

void Vec2::Reflect(const Vec2 & v) {
	*this += 2.0 * this->DotProduct(v) * Vec2(v.x * -1, v.y * -1); //TODO ver a ver porque no acepta Reverse(v)
}

void Vec2::Reverse() {
	this->x *= -1;
	this->y *= -1;
}

void Vec2::Truncate(double maxLength) {
	if (this->Length() > maxLength) {

		this->Normalize();
		*this *= maxLength;
	}
}

void Vec2::RotateAng(double angle) {
	this->RotateRad(angle * M_PI / 180);
}

void Vec2::RotateRad(double radians) {
	double newX = x * cos(radians) - y * sin(radians);
	double newY = x * sin(radians) + y * cos(radians);

	x = newX;
	y = newY;
}

double Vec2::DotProduct(const Vec2 & v) const {
	return (this->x * v.x + this->y * v.y);
}

double Vec2::CrossProduct(const Vec2 & v) const {
	return (this->x * v.y - v.x * this->y);
}

double Vec2::Distance(const Vec2 & v) const {
	// Creo el vector que une this con v y devuelvo su longitud
	Vec2 dist(*this, v);
	return dist.Length();
}

double Vec2::DistanceSqrd(const Vec2 & v) const {
	Vec2 dist(*this, v);
	return dist.LengthSqrd();
}

bool Vec2::Sign(const Vec2 & v) const {
	return (this->x * v.y > v.x * this->y);
}

double Vec2::AngleAng(const Vec2 & v) const {
	return this->AngleRad(v) * 180 / M_PI;
}

double Vec2::AngleAng() const {
	return this->AngleRad() * 180 / M_PI;
}

double Vec2::AngleRad(const Vec2 & v) const {
	// Si cualquiera de los vectores es 0 no computamos nada
	if (this->isZero() || v.isZero())
		return 0;

	// Calculamos el angulo
	double angle = atan2(v.y - y, v.x - x);

	return angle;
}

double Vec2::AngleRad() const {
	if(this->isZero())
		return 0;

	return atan2(y, x);
}

bool Vec2::isZero() const {
	return (this->x == 0 && this->y == 0);
}

double Vec2::Length() const {
	return sqrt(this->x * this->x + this->y * this->y);
}

double Vec2::LengthSqrd() const {
	return (this->x * this->x + this->y * this->y);
}

void Vec2::Copy(const Vec2 & v) {
	this->x = v.x;
	this->y = v.y;
}

Vec2 operator +(const Vec2 & lhs, const Vec2 & rhs) {
	Vec2 aux(lhs);
	aux += rhs;

	return aux;
}

Vec2 operator -(const Vec2 & lhs, const Vec2 & rhs) {
	Vec2 aux(lhs);
	aux -= rhs;

	return aux;
}

Vec2 operator *(const double & fac, const Vec2 & rhs) {
	Vec2 aux(rhs);
	aux *= fac;

	return aux;
}

Vec2 operator *(const Vec2 & lhs, const double & fac) {
	Vec2 aux(lhs);
	aux *= fac;

	return aux;
}

Vec2 operator /(const Vec2 & lhs, const double & fac) {
	Vec2 aux(lhs);
	aux /= fac;

	return aux;
}

Vec2 Normalize(const Vec2 & vec) {
	Vec2 aux(vec);
	aux.Normalize();

	return aux;
}

Vec2 Reflect(const Vec2 & v1, const Vec2 & v2) {
	Vec2 aux(v1);
	aux.Reflect(v2);

	return aux;
}

Vec2 Reverse(const Vec2 &v) {
	Vec2 aux(v);
	aux.Reverse();

	return aux;
}

Vec2 Truncate(const Vec2 & v, double & maxLength) {
	Vec2 aux(v);
	aux.Truncate(maxLength);

	return aux;
}

Vec2 RotateAng(const Vec2 & v, double & angle) {
	Vec2 aux(v);
	aux.RotateAng(angle);

	return aux;
}

Vec2 RotateRad(const Vec2 & v, double & radians) {
	Vec2 aux(v);
	aux.RotateRad(radians);

	return aux;
}

double Distance(const Vec2 & v1, const Vec2 & v2) {
	Vec2 aux(v1, v2);

	return aux.Length();
}