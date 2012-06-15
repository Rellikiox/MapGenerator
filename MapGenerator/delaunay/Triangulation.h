/*
 * Triangulation.h
 *
 *  Created on: Nov 18, 2011
 *      Author: rellik
 */

#ifndef TRIANGULATION_H_
#define TRIANGULATION_H_

using namespace std;
#include "Tri.h"
#include "Point.h"
#include <cmath>
#include <algorithm>

class Tri;

/**\class Clase Triangulation
 * \brief Triangularización de puntos en el espacio 2D
 *
 */
class Triangulation{
public:
	/**\brief Constructor vacio
	 */
	Triangulation(){}

	/**\brier Constructor para limites
	 * Inicializa la triangularizacion usando los dos puntos
	 * para formar un rectangulo que englobará todos los demas puntos.
	 * @param i Punto superior izquierdo
	 * @param f Punto inferior derecho
	 */
	Triangulation(Point i, Point f);

	/**\brief Constructor desde vector de puntos
	 * Utiiza vPuntos como conjunto de puntos
	 * @param vPuntos
	 */
	Triangulation(std::vector<Point> vPuntos);

	Triangulation(const Triangulation &);

	Triangulation & operator=(const Triangulation &);

	/**\brief Destructor por defecto
	 * Limpia los vectores nodos y triangles.
	 */
	~Triangulation();

	void createBorders(Point i, Point f);

	/**\brief Añade p a la lista de puntos
	 * Añade p a la lista que la Delaunay usa para generar los triangulos.
	 * Sin duplicados.
	 * @param p Punto a insertar
	 */
	bool addPoint(Point p);

	/**\brief Añade el vector de puntos al vector interno
	 * Añade los puntos del vector pasado por parametros al vector que Delaunay usa.
	 * Sin duplicados.
	 * @param vPuntos vector de puntos a insertar
	 */
	void addPoints(std::vector<Point> vPuntos);

	/**\brief Añade un punto a la triangulizacion y recalcula esta misma
	 * Coge el primer punto del vector y lo añade a la triangulización,
	 * recalcula todos los triangulos necesarios y por ultimo elimina
	 * el punto de la lista
	 */
	bool stepDelaunay();

	/**\brief Termina la triangularización
	 * Continua añadiendo puntos hasta que se vacie el vector
	 */
	void continueDelaunay();

	/**\brief Obtiene el estado actual de la triangularización
	 * Devuelve el vector de triangulos tal y como esta en el momento de la llamada,
	 * esto incluye los cuatro puntos extra generados al inicio
	 * @return
	 */
	std::vector<Tri *> getTriangles();

	/**\brief Devuelve la triangularización terminada
	 * Devuelve el estado actual de la triangularización
	 * eliminando los puntos auxiliares
	 * @return Vector de triangulos con la triangularizacion acabada
	 */
	std::vector<Tri *> finishedDelaunay();

	void removeT(Tri *tri);

	std::vector<Point> quicksort(std::vector<Point> puntos);

	void Reset();

private:

	/**\brief Esquina superior derecha
	 */
	Point pxpy;

	/**\brief Esquina inferior derecha
	 */
	Point pxny;

	/**\brief Esquina superior izquierda
	 */
	Point nxpy;

	/**\brief Esquina inferior izquierda
	 */
	Point nxny;

	/**\brief Vector de puntos aun sin procesar
	 */
	std::vector<Point> puntos;

	/**\brief Triangularizacion actual
	 */
	std::vector<Tri *> triangles;

	/**\brief Se asegura de que la arista [caso] sea legal
	 * Comprueba que la arista [caso] sea legal. Si lo es no hace nada,
	 * pero si no lo es la voltea
	 * @param ori Triangulo sobre el que se comprueba la legalidad
	 * @param caso Arista a mirar: 0 -> AB, 1 -> BC, 2 -> CA
	 */
	void legalize(Tri *ori, int caso);

	/**\brief Elimina p de la triangularización
	 * Elimina todos los triangulos que tengan a p como uno de sus vertices
	 * @param p Punto a eliminar dle conjunto
	 */
	void removeP(Point p);

	/**\brief Voltea la arista [caso] del triangulo
	 * Cambia la arista [caso] del triangulo ori para que la
	 * triangularización mantenga las propiedades de Delaunay
	 * @param ori Triangulo desde el que voltear
	 * @param caso Arista objetivo: 0 -> AB, 1 -> BC, 2 -> CA
	 */
	void flipEdge(Tri * ori, int caso);

};

#endif /* TRIANGULATION_H_ */
