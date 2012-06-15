/*
 * Tri.h
 *
 *  Created on: Nov 27, 2011
 *      Author: rellik
 */

#ifndef TRI_H_
#define TRI_H_

#include <iostream>
#include "Point.h"
#include "Matrix.h"
#include <vector>

using namespace std;

/**\class Tri
 * \brief Clase que se encarga de controlar Triangulos
 */
class Tri{

	/**\brief Sobrecarga de la salida
	 *
	 * @param os stream de salida
	 * @param tri Objeto a enviar por el stream
	 * @return ostream EL stream modificado
	 */
	friend ostream& operator<<(ostream &, const Tri &);
public:
	/**\brief Constructor vacio
	 */
	Tri();

	~Tri();

	Tri(const Tri &t);

	/**\brief Constructor sobrecargado
	 * Crea un triangulo con los puntos A, B y C y sus adyacencias vacias
	 * @param A Primer punto
	 * @param B Segundo punto
	 * @param C Tercer punto
	 */
	Tri(Point A, Point B, Point C);

	/**\brief Constructor sobrecargado
	 * Crea un triangulo con los puntos A, B y C y las adyacencias AB, BC y CA.
	 * @param A Primer punto
	 * @param B Segundo punto
	 * @param C tercer punto
	 * @param AB Triangulo que comparte a A y a B
	 * @param BC Triangulo que comparte a B y a C
	 * @param CA Triangulo que comparte a C y a A
	 */
	Tri(Point A, Point B, Point C, Tri *AB, Tri *BC, Tri *CA);

	/**\brief Getter de A
	 * @return El punto A
	 */
    Point getA() const;

    /**\brief Getter de AB
     * @return El triangulo AB
     */
    Tri *getAb() const;

    /**\brief Getter de B
     * @return El punto B
     */
    Point getB() const;

    /**\brief Getter de BC
     * @return El triangulo BC
     */
    Tri *getBc() const;

    /**\brief Getter de C
     * @return El punto C
     */
    Point getC() const;

    /**\brief Getter de CA
     * @return El triangulo CA
     */
    Tri *getCa() const;

    /**\brief Setter de A
     * @param a Nuevo a
     */
    void setA(Point a);

    /**\brief Setter de AB
     * @param ab Nuevo AB
     */
    void setAb(Tri *ab);

    /**\brief Setter de B
     * @param b Nuevo B
     */
    void setB(Point b);

    /**\brief Setter de BC
     * @param bc Nuevo BC
     */
    void setBc(Tri *bc);

    /**\brief Setter de C
     * @param c Nuevo C
     */
    void setC(Point c);

    /**\brief Setter de CA
     * @param ca Nuevo CA
     */
    void setCa(Tri *ca);

    /**\brief Setter de AB, BC y CA
     * @param ab Nuevo AB
     * @param bc Nuevo BC
     * @param ca Nuevo CA
     */
    void setTris(Tri *ab, Tri *bc, Tri *ca);

    /**\brief Determina si p esta dentro de this
     * @param p Punto a mirar
     * @return 1 si esta dentro, 0 en una linea, -1 fuera, -2 error
     */
    int isInside(Point p);

    /**\brief Devuelve en que lado de p1p2 esta p2
     * @param p1 Primer punto del vector
     * @param p2 Segundo punto del vector
     * @param ps Punto a determinar
     * @return 1 a la derecha, 0 en la linea, -1 a la izquierda
     */
    static int side(Point p1,Point p2, Point ps);

    /**\brief Obtiene el punto opuesto al caso pasado por parametros
     * @param caso 0 AB, 1 BC, 2 CA
     * @return C si caso == 0, A si caso == 1, B si caso == 2
     */
    Point opositeP(int caso);

    /**\brief Devuelve el punto opuesto a op
     * Devuelve el punto de este triangulo que no es compartido por op
     * @param op triangulo opuesto al punto
     * @return punto opuesto al triangulo
     */
    Point opositeP(Tri *op);

    /**\brief Triangulos opuestos a op
     * Los dos triangulos adyacentes a this que no son op
     * @param op triangulo del que obtener los opuestos
     * @return los dos triangulos opuestos a op
     */
    std::vector<Tri *> opositeT(Tri *op);

    /**\brief Cambia los puntos del triangulo para ponerlos en sentido antihorario
     */
    void makeCounterClockwise();

    /**\brief El triangulo esta ordenado en sentido antihorario
     * @return true si this esta ordenado en sentido antihorario
     */
    bool isCounterClockwise();

    /**\brief comprueba si l esta en el circulo formado por i, j y k
     * Comprueba si l esta dentro del circulo cuya circumferencia pasa por i, j y k
     * @param i Primer punto de la circumferencia
     * @param j Segundo punto de la circumferencia
     * @param k Tercer punto de la circumferencia
     * @param l Punto a comprobar
     * @return true si l esta dentro
     */
    static bool liesInside(Point i, Point j, Point k, Point l);

    /**\brief Intercambia tri1 por tri2
     * Si tri1 es uno de los vecinos de this lo cambia por tri2
     * @param tri1 Triangulo que sale
     * @param tri2 Triangulo que entra
     */
    void swapP(Tri *tri1, Tri *tri2);

    /**\brief Realiza swapP para todos los vecinos de this
     * @param AB Triangulo para sustituir a ab
     * @param BC Triangulo para sustituir a bc
     * @param CA Triangulo para sustituir a ca
     */
    void updateEdges(Tri *AB, Tri *BC, Tri *CA);

    /**\brief Obtiene el centro de masas del triangulo
     * @return Baricentro del triangulo
     */
    Point getBaricenter();

    /**\brief Obtiene el centro del circulo inscrito en el triangulo
     * @return Incentro del triangulo
     */
    Point getIncenter();

	Point getCircumcentro();

    double Area();

private:

    /**\brief Primer punto del triangulo
     */
	Point a;

	/**\brief Segundo punto del triangulo
	 */
	Point b;

	/**\brief Tercer punto del triangulo
	 */
	Point c;

	/**\brief Primer vecino del triangulo
	 */
	Tri *ab;

	/**\brief Segundo vecino del triangulo
	 */
	Tri *bc;

	/**\brief Tercer vecino del triangulo
	 */
	Tri *ca;
};


#endif /* TRI_H_ */
