#pragma once

class Vec2{
public:
	/**
	* Constructor vacio.
	*/
	Vec2();

	/**
	* Destructor.
	*/
	~Vec2();

	/**
	* Constructor de copia.
	* @param v Objeto a copiar.
	*/
	Vec2(const Vec2 &v);

	/**
	* Sobrecarga de la asignación.
	* @param v Objeto a copiar
	* @return Objeto copiado
	*/
	Vec2 & operator=(const Vec2 &v);

	/**\
	* Constructor que acepta los valores de x e y
	* @param x Valor x del vector
	* @param y Valor y del vector
	*/
	Vec2(double x, double y);

	/**
	* Constructor a partir de dos vectores existentes, el
	* vector generado es el que los une
	* @param v1 Primer vector
	* @param v2 Segundo vector
	*/
	Vec2(const Vec2 &v1, const Vec2 &v2);

	/**\
	* Constructor que acepta el angulo del vector,
	* genera el vector (cos(ang), sin(ang))
	* @param angle
	*/
	Vec2(double angle);

	/**
	* Sobrecarga de la suma-igualacion
	* @param v Vector que se le sumara a this
	* @return Vector (x + v.x, y + v.y)
	*/
	Vec2 & operator+=(const Vec2 &v);

	/**
	* Sobrecarga de la suma-igualacion
	* @param f Valor que se le sumara al vector
	* @return El vector (x + d, y + d)
	*/
	Vec2 & operator+=(const double &f);

	/**
	* Sobrecarga de la resta-igualacion
	* @param v Vector que se le resta a this
	* @return El vector (x - v.x, y - v.y)
	*/
	Vec2 & operator-=(const Vec2 &v);

	/**
	* Sobrecarga de la resta-igualacion
	* @param f Valor que se le restara al vector
	* @return El vector (x - d, y - d)
	*/
	Vec2 & operator-=(const double &f);

	/**
	* Sobrecarga de la multiplicacion-igualacion
	* @param f Valor por el que se multiplicará el vector
	* @return El vector (x * f, y * f)
	*/
	Vec2 & operator*=(const double &f);

	/**
	* Sobrecarga de la division-igualacion
	* @param f Valor por el que se divide el vector
	* @return El vector (x / f, y / f)
	*/
	Vec2 & operator/=(const double &f);

	/**
	* Sobrecarga de la comparacion de igualdad
	* @param v Vector con el que comparar this
	* @return (x == v.x && y == v.y)
	*/
	bool operator==(const Vec2 &v) const;

	/**
	* Sobrecarga de la comparacion de desigualdad
	* @param v Vector con el que comparar this
	* @return (x != v.x || y != v.y)
	*/
	bool operator!=(const Vec2 &v) const;

	/**
	* Normaliza this, haciendo que tenga longitud 1
	*/
	void Normalize();

	/**
	* Refleja this sobre v, como la trayectoria de una bola de
	* billar despues de chocar contra el borde de la mesa
	* @param v Vector contra el que "rebota" this
	*/
	void Reflect(const Vec2 &v);

	/**
	* Le da la vuelta al vector, de forma que (x, y) se convierte en (-x, -y)
	*/
	void Reverse();

	/**
	* Trunca la longitud de this a maxLength
	* @param maxLength Valor al que se truncara el vector
	*/
	void Truncate(double maxLength);

	/**
	* Rota this angle grados alrededor del eje.
	* @param angle Grados a rotar el vector
	*/
	void RotateAng(double angle);

	/**
	* Rota this angle radianes alrededor del eje.
	* @param angle Radianes a rotar el vector
	*/
	void RotateRad(double radian);

	/**
	* Computa el punto producto de los vectores this y v.
	* Es independiente del orden.
	* @param v Vector con el que computar.
	* @return El punto producto (x * v.x + y * v.y)
	*/
	double DotProduct(const Vec2 &v) const;

	/**
	* Computa el producto cruzado de los vectores this y v.
	* Es dependiente del orden
	* @param v El vector con el que computar
	* @return El producto cruzado (x * v.y - v.x * y)
	*/
	double CrossProduct(const Vec2 &v) const;

	/**
	* Calcula la distancia entre this y v
	* @param v Vector con el que computar
	* @return Distancia euclidea entre los vectores
	*/
	double Distance(const Vec2 &v) const;

	/**
	* Calcula el cuadrado de la distancia entre this y v
	* @param v Vector con el que computar
	* @return Distancia euclidea entre los vectores al cuadrado
	*/
	double DistanceSqrd(const Vec2 &v) const;

	/**
	* Computa el signo entre los vectores this y v.
	* @param v Vector con el que computar
	* @return true si v esta a la derecha de this
	*/
	bool Sign(const Vec2 &v) const;

	/**
	* Computa el angulo que forman this y v
	* @param v Vector con el que computar
	* @return Angulo que forman los vectores en grados
	*/
	double AngleAng(const Vec2 &v) const;

	/**
	* Computa el angulo que forma this con el suelo (1,0)
	* @return Angulo que forma this con (1,0) en grados
	*/
	double AngleAng() const;

	/**
	* Computa el angulo que forman this y v
	* @param v Vector con el que computar
	* @return Angulo que forman los vectores en radianes
	*/
	double AngleRad(const Vec2 &v) const;

	/**
	* Computa el angulo que forma this con el suelo (1,0)
	* @return Angulo que forma this con (1,0) en radianes
	*/
	double AngleRad() const;

	/**
	* Determina si el vector es 0
	* @return true si el vector es 0
	*/
	bool isZero() const;

	/**
	* Computa el modulo del vector
	* @return Modulo del vector
	*/
	double Length() const;

	/**
	* Computa el modulo del vector al cuadrado
	* @return Modulo del vector al cuadrado
	*/
	double LengthSqrd() const;

	/**
	* Valor x del vector
	*/
	double x;

	/**
	* Valor y del vector
	*/
	double y;

private:

	/**
	* Copia v en this
	* @param v Objeto a copiar
	*/
	void Copy(const Vec2 &v);
};

/**
* Devuelve el vector resultante de sumar lhs y rhs
* @param lhs Vector a la izquierda del signo +
* @param rhs Vector a la derecha del signo +
* @return El vector (lhs.x + rhs.x, lhs.y + rhs.y)
*/
Vec2 operator+(const Vec2 &lhs, const Vec2 &rhs);

/**
* Devuelve el vector resultante de restar lhs y rhs
* @param lhs Vector a la izquierda del signo -
* @param rhs Vector a la derecha del signo -
* @return El vector (lhs.x - rhs.x, lhs.y - rhs.y)
*/
Vec2 operator-(const Vec2 &lhs, const Vec2 &rhs);

/**
* Devuelve el vector resultante de multiplicar rhs por fac
* @param fac Valor a la izquierda del signo *
* @param rhs Vector a la derecha del signo *
* @return El vector (rhs.x * fac, rhs.y * fac)
*/
Vec2 operator*(const double &fac, const Vec2 &rhs);

/**
* Devuelve el vector resultante de multiplicar lhs por fac
* @param fac Vector a la izquierda del signo *
* @param rhs Valor a la derecha del signo *
* @return El vector (lhs.x * fac, lhs.y * fac)
*/
Vec2 operator*(const Vec2 &lhs, const double &fac);

/**
* Devuelve el vector resultante de dividir lhs por fac
* @param fac Vector a la izquierda del signo /
* @param rhs Valor a la derecha del signo +
* @return El vector (lhs.x / fac, lhs.y / fac)
*/
Vec2 operator/(const Vec2 &lhs, const double &fac);

/**
* Normaliza el vector vec
* @param vec Vector a computar
* @return El vector vec normalizado
*/
Vec2 Normalize(const Vec2 &vec);

/**
* Refleja v1 sobre v2, como la trayectoria de una bola de
* billar despues de chocar contra el borde de la mesa
* @param v1 Vector a reflejar
* @param v2 Vector contra el que "rebota" v1
* @return El vector resultante de reflejar v1 sobre v2
*/
Vec2 Reflect(const Vec2 &v1, const Vec2 &v2);

/**
* Le da la vuelta al vector v
* @param v Vector a voltear
* @return El vector (-v.x, -v.y)
*/
Vec2 Reverse(const Vec2 &v);

/**
* Trunca la longitud de v a maxLength
* @param v Vector a truncar
* @param maxLength Valor al que se truncara el vector
* @return v truncado a maxLength
*/
Vec2 Truncate(const Vec2 &v, double &maxLength);

/**
* Rota v angle grados alrededor del eje.
* @param v Vector a rotar
* @param angle Grados a rotar el vector
* @return El vector v rotado angle grados
*/
Vec2 RotateAng(const Vec2 &v, double &angle);

/**
* Rota v angle radianes alrededor del eje.
* @param v Vector a rotar
* @param angle Radianes a rotar el vector
* @return El vector v rotado angle radianes
*/
Vec2 RotateRad(const Vec2 &v, double &radian);

/**
* Computa la distancia entre v1 y v2
* @param v1 Primer vector
* @param v2 Segundo vector
* @return Distancia entre los vectores
*/
double Distance(const Vec2 &v1, const Vec2 &v2);

