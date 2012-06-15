/*
 * Matrix.h
 *
 *  Created on: 03/11/2011
 *      Author: Chutaos Team
 */

#ifndef MATRIX_H_
#define MATRIX_H_

#include <iostream>
#include <math.h>
#include <vector>
//#include "Rational.h"

using namespace std;

/**\class Matrix
 * La clase Matrix define una matriz de clase T
 * Se crean objetos usando Matriz<type>, por ejemplo:
 * Matrix<int>, Matrix<float>, Matrix<double>, Matriz<Rational>
 * Solo tiene constructor con parametros (n,m) filas y columnas
 * y crea una matriz llena de 0, se rellena acediendo a las posiciones
 * e insertando elementos, o con la funcion fill. Se puede acceder de
 * dos formas, la forma directa variablematriz.matrix[0]0] para la posicion (1,1)
 * o mediante el operador(), con sus indices reales variablematriz(1,1) para la posicion (1,1)*/
template<class T>
class Matrix {

	/**\brief Sobrecarga del operador salida.
	 * \param os - ostream de salida.
	 * \param mat - matriz para mostrar.
	 * \return Salida de la matriz.*/
	template<class U>
	friend ostream& operator<<(ostream &os, const Matrix<U> &mat);
public:
	//--------- FORMA CANÓNICA ------------//

	/**\brief Constructor.
	 * \param f - Este numero determinara el numero de filas de la matriz
	 * \param c - Este numero determinara el numero de columnas de la matriz*/
	Matrix(int f, int c);

	/**\brief Destructor*/
	virtual ~Matrix();

	/**\brief Constructor de Copia
	 * \param mat - La matriz a copiar*/
	Matrix(const Matrix<T> &mat);

	/**\brief Devuelve el valor de c.
	 *\return devuelve el valor de c.*/
	int getC(void) const;

	/**\brief Devuelve el valor de f.
	 *\return devuelve el valor de f.*/
	int getF(void) const;

	/**\brief Devuelve el valor de f.
	 *\param n - La posicion de x (columna) de la matriz
	 *\param m - La posicion de y (fila) de la matriz.
	 *\return devuelve el valor de la posicion n m.*/
	T getNM(const int, const int) const;

	/**\brief GetColumna
	 * \param c - La columna a que queremos obtener
	 * \return col Matriz unidimensional que contiene la columna*/
	Matrix<T> getCol(const int);

	/**\brief GetColumna
	 * \param f - La fila a que queremos obtener
	 * \return fil Matriz unidimensional que contiene la fila*/
	Matrix<T> getFil(const int);

	/**\brief Asigna un dato a una posicion (x,y) de la matriz.
	 *\param n - La posicion de x (columna) de la matriz
	 *\param m - La posicion de y (fila) de la matriz.
	 *\param dato - El dato que vamos a añadir.*/
	void setNM(const int, const int, const T);

	/**\brief Operador de asigancion
	 * \param mat - La matriz a asignar
	 * \return tmp Una matriz temporal modificada*/
	Matrix<T> & operator=(const Matrix<T> &mat);

	/**\brief Operador de suma
	 * \param mat - La matriz a sumar
	 * \return tmp Una matriz temporal modificada*/
	Matrix<T> operator+(const Matrix<T> &mat) const;

	/**\brief Operador de multiplicación
	 * \param mat - La matriz a multiplicar
	 * \return tmp Una matriz temporal modificada*/
	Matrix<T> operator*(const Matrix<T> &mat) const;

	/**\brief Operador de multiplicación
	 * \param rat - El T a multiplicar
	 * \return tmp Una matriz temporal modificada*/
	Matrix<T> operator*(const T &rat) const;

	/**\brief Operador de division
	 * \param rat - El T a dividir
	 * \return tmp Una matriz temporal modificada*/
	Matrix<T> operator/(const T &rat) const;

	/**\brief Operador de corchetes
	 * \param n - fila a la que queremos acceder
	 * \param m - columna a la que queremos accder
	 * \return *this modificada*/
	T& operator()(const int n, const int m);

	/**\brief Operador de corchetes
	 * \param n - fila a la que queremos acceder
	 * \param m - columna a la que queremos accder
	 * \return tmp Una matriz temporal modificada*/
	T operator()(const int n, const int m) const;

	/**\brief Operador ==, devuelve true si son iguales
	 * \param mat - La matriz a comparara
	 * \return tre si son iguales*/
	bool operator==(const Matrix<T> &mat);

	/**\brief Operador !=, devuelve true si son diferentes
	 * \param mat - La matriz a comparara
	 * \return true si son diferentes*/
	bool operator!=(const Matrix<T> &mat);

	/**\brief Concatena dos matrices
	 * \param mat - La matriz a concatenar
	 * \return tmp una matriz que contiene la concatenacion this&mat */
	Matrix<T> conMatrix(const Matrix<T> &mat) const;

	/**\brief Devuelve una Matriz cambiando su tamaño por nxm
	 * \param n - numero de filas de la nueva matriz
	 * \param m -  numero de columnas de la nueva matriz
	 * \return tmp Una matriz temporal modificada*/
	Matrix<T> chaMatrix(int n, int m) const;

	/**\brief Devuelve una Sub-Matriz desde la posicion nixmi hasta nfxmf
	 * \param ni - fila donde empezara la nueva matriz
	 * \param mi - columna donde empezara la nueva matriz
	 * \param nf - fila donde acabara la nueva matriz
	 * \param mf - columna donde acabara la nueva matriz
	 * \return tmp Una matriz temporal modificada*/
	Matrix<T> subMatrix(int, int, int, int) const;

	/**\brief Rellena una matriz con un T*.
	 * \param vec - vector de T con los datos de la matriz
	 * \param tam - tamaño del vector T* vec.
	 * \return *this la matriz modificada*/
	Matrix<T> & fill(const T[], const int);

	/**\brief esCuadrada
	 * \return bool - devuelve true si la matriz que lo llama es cuadrada*/
	bool isSquare(void) const;

	/**\brief esIdentidad
	 * \return bool - devuelve true si la matriz que lo llama es la identidad*/
	bool isIdentity(void);

	/**\brief esNegativo
	 * Si contiene todos los elementos menores que 0
	 * \return bool - devuelve true si la matriz que lo llama es negativa*/
	bool isNegative(void);

	/**\brief esPositivo
	 * Si contiene todos los elementos mayores que 0
	 * \return bool - devuelve true si la matriz que lo llama es positiva*/
	bool isPositive(void);

	/**\brief esNegativo
	 * Si contiene algun elemento menor que 0
	 * \return bool - devuelve true si la matriz que lo llama tiene un negativo*/
	bool isOneNegative(void);

	/**\brief esNegativo
	 * Si contiene algun elemento mayor que 0
	 * \return bool - devuelve true si la matriz que lo llama tiene un positivo*/
	bool isOnePositive(void);

	/**\brief Convierte cualquier matriz cuadrada en la Identidad
	 * return this, transformada si era cuadrada.*/
	void setIdentity(void);

	/**\brief Calcula el determinate de una matriz hasta tamaño 3x3
	 * \param mat - La matriz a la que calculamos al inversa
	 * \return det es el determinate de la matriz*/
	T determinant(void) const;

	/**\brief Calcula la traspuesta de una matriz
	 * \return devuelve una matriz temporal con la traspuesta*/
	Matrix<T> transpose(void) const;

	/**\brief Calcula matriz de cofactores
	 * \return devuelve la matriz cofactores*/
	Matrix<T> cofactors(void) const;

	/**\brief Calcula matriz de cofactores
	 * \return devuelve la matriz cofactores*/
	Matrix<T> adjoint(void) const;

	/**\brief Operador de filas, sirve para sumar o restar filas - n1-n2 o n1+n2
	 * \param n1 numero de la fila a la que se le suma/resta
	 * \param op operador a realizar - (para resta) o + (para suma)
	 * \param n2 numero de la fila que suma/resta a n1
	 * \return *this devuelve la matriz modificada*/
	Matrix<T>& operatorFF(const int, const char, const int);

	/**\brief Operador de fila con T, sirve para multiplicar/dividir a una fila un T - n1/rat o n1*rat
	 * \param n1 numero de la fila a la que se le multiplica/divide
	 * \param op operador a realizar * (para multiplicar) o / (para dividir)
	 * \param rat T que multiplica/divide a n1
	 * \return *this devuelve la matriz modificada*/
	Matrix<T>& operatorFR(const int, const char, const T);

	/**\brief Operador de filas y T, es una convinacion de las anteriores
	 * sirve para multiplicar/dividir un T a una fila y despues restar/sumar a otra fila - n1-(n2*rat) o n1+(n2/rat)
	 * \param n1 numero de la fila a la que se le restar/sumar el resultado de n2*rat
	 * \param op operador a realizar * (para multiplicar) o / (para dividir)
	 * \param n2 numero de la fila que suma/resta a n1
	 * \param rat T que multiplica/divide a n1
	 * \return *this devuelve la matriz modificada*/
	Matrix<T>& operatorFFR(const int, const char, const int, const T);

	/**\brief Operador de filas de matrices, asigna la fila n de una matriz a la fila n de la otra
	 * \param mat matriz de la que queremos copiar su fila
	 * \param n fila de la matriz mat que queremos copiar
	 * \return *this matriz modificada*/
	Matrix<T>& operatorCF(const Matrix<T>, const int);

	/**\brief Calcula la inversa de una matriz por determinantes de maximo 3x3
	 * \param mat - La matriz a la que calculamos al inversa*/
	Matrix<T> inverseDet(void) const;

	/**\brief Devuelve true si la columna que le pasamos como argumento es toda de 0's
	 * \param col - numero de columna a comprobar (de 0 a f-1)
	 * \return resultado de si es o no toda de 0*/
	bool colZeros(const int);

	/**\brief Devuelve la fila del donde se encuentra un elemento distinto de 0, que no sea (fil,col)
	 * \param fil - numero de la fila a comproar (de 0 a f-1)
	 * \param col - numero de la columna a comproar (de 0 a c-1)
	 * \return piv, contiene el numero de la fila distinta de 0*/
	int filPivot(const int, const int);

	/**\brief Calcula la inversa de una matriz por Gauss-Jordan
	 * \param mat - La matriz a la que calculamos al inversa*/
	Matrix<T> inverse(void) const;

	/**\brief Coloca la identidad en las columnas del vector como parametro
	 * mediante el metodo Gauss-Jordan, sirva para la tabla de Simplex.
	 * \param base - El vector STL base con las columnas en su orden*/
	Matrix<T>& baseTo(const vector<int>);

private:
	/**\brief Es una matriz de elementos Ts*/
	T **matrix;
	/**\brief Es un entero que representa el numero de filas*/
	int f;
	/**\brief Es un entero que representa el numero de columnas*/
	int c;
	/**\brief Es T de error*/
	T error;
};

//Definicion de las funciones.
template<class T>
Matrix<T>::Matrix(int nf, int nc) {
	if (nf > 0 && nc > 0) {

		matrix = new T*[nf];
		for (int i = 0; i < nf; i++)
			matrix[i] = new T[nc];
		c = nc;
		f = nf;
	} else {
		matrix = NULL;
		c = 0;
		f = 0;
	}
}

template<class T>
Matrix<T>::~Matrix() {
	if(matrix!=NULL)
	{
		for (int y = 0; y < f; y++) {
			delete[] matrix[y];
		}
		delete[] matrix;
		matrix=NULL;
	}
}

template<class T>
Matrix<T>::Matrix(const Matrix & mat) {
	int af = mat.getF();
	int ac = mat.getC();

	matrix = new T*[af];
	for (int i = 0; i < af; i++) {
		matrix[i] = new T[ac];
	}
	c = ac;
	f = af;

	for (int n = 0; n < af; n++)
		for (int m = 0; m < ac; m++) {
			matrix[n][m] = mat.matrix[n][m];
			//setNM(n, m, mat.getNM(n, m));
		}
}

template<class T>
int Matrix<T>::getC(void) const {
	return c;
}

template<class T>
int Matrix<T>::getF(void) const {
	return f;
}
template<class T>
T Matrix<T>::getNM(const int n, const int m) const {
	return matrix[n][m];
}

template<class T>
void Matrix<T>::setNM(const int n, const int m, const T dato) {
	matrix[n][m] = dato;
}

template<class T>
Matrix<T> Matrix<T>::getCol(const int c) {
	Matrix<T> col(f, 1);

	for (int i = 0; i < f; i++)
		col.matrix[i][0] = matrix[i][c - 1];

	return col;
}

template<class T>
Matrix<T> Matrix<T>::getFil(const int f) {
	Matrix<T> fil(1, c);

	for (int i = 0; i < c; i++)
		fil.matrix[0][i] = matrix[f - 1][i];

	return fil;
}

template<class T>
Matrix<T>& Matrix<T>::operator=(const Matrix<T> & mat) {
	if (this != &mat) {
		int af = mat.getF();
		int ac = mat.getC();

		this->~Matrix();

		matrix = new T*[af];
		for (int i = 0; i < af; i++) {
			matrix[i] = new T[ac];
		}
		c = ac;
		f = af;

		for (int n = 0; n < af; n++)
			for (int m = 0; m < ac; m++) {
				//this->setXY(x, y, mat.getXY(x, y));
				matrix[n][m] = mat.matrix[n][m];
			}
	}
	return *this;
}

template<class T>
Matrix<T> Matrix<T>::operator+(const Matrix<T> &mat) const {
	Matrix<T> tmp(f, c);
	if (c == mat.c && f == mat.f) {
		for (int n = 0; n < f; n++)
			for (int m = 0; m < c; m++) {
				tmp.matrix[n][m] = (matrix[n][m] + mat.matrix[n][m]);
			}
	}
	return tmp;
}

template<class T>
Matrix<T> Matrix<T>::operator *(const Matrix<T>& mat) const {
	Matrix<T> tmp(f, mat.c);
	if (c == mat.f) {
		for (int n = 0; n < f; n++)
			for (int m = 0; m < mat.c; m++) {
				tmp.matrix[n][m] = 0;
				for (int z = 0; z < c; z++)
					tmp.matrix[n][m] = tmp.matrix[n][m]
							+ (matrix[n][z] * mat.matrix[z][m]);
			}
	}
	return tmp;
}

template<class T>
Matrix<T> Matrix<T>::operator*(const T& rat) const {
	Matrix<T> tmp(f, c);

	for (int n = 0; n < f; n++)
		for (int m = 0; m < c; m++)
			tmp.matrix[n][m] = (matrix[n][m] * rat);

	return tmp;
}

template<class T>
Matrix<T> Matrix<T>::operator/(const T& rat) const {

	return operator *(rat.inversa());

}

template<class T>
T& Matrix<T>::operator()(const int n, const int m) {
	if (n > 0 && m > 0) {
		if (n <= f && m <= c) {
			return matrix[n - 1][m - 1];
		} else {
			int nf = n, nc = m;
			if (f > n)
				nf = f;
			else
				nf = n;
			if (c > m)
				nc = c;
			else
				nc = m;
			*this = chaMatrix(nf, nc);
			return matrix[n - 1][m - 1];
		}
	} else {
		return error;
	}
}

template<class T>
T Matrix<T>::operator()(const int n, const int m) const {
	if ((n > 0 && n <= f) && (m > 0 && m <= c))
		return matrix[n - 1][m - 1];
	else
		return T();
}

template<class T>
bool Matrix<T>::operator==(const Matrix<T> &mat) {
	bool iguales = false;
	if (f == mat.f && c == mat.c) {
		iguales = true;
		for (int n = 0; n < f && iguales; n++)
			for (int m = 0; m < c && iguales; m++)
				if (matrix[n][m] != mat.matrix[n][m])
					iguales = false;

	}
	return iguales;
}

template<class T>
bool Matrix<T>::operator!=(const Matrix<T> &mat) {
	return (!operator==(mat));
}

template<class U>
ostream& operator<<(ostream &os, const Matrix<U> &mat) {

	for (int n = 0; n < mat.f; n++) {
		for (int m = 0; m < mat.c; m++) {
			os << mat.matrix[n][m] << "\t";
		}
		os << endl;
	}
	return os;
}

template<class T>
Matrix<T> Matrix<T>::chaMatrix(int n, int m) const {
	Matrix<T> tmp(n, m);

	if (n > f)
		n = f;
	if (m > c)
		m = c;
	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
			tmp.matrix[i][j] = matrix[i][j];

	return tmp;
}

template<class T>
Matrix<T> Matrix<T>::subMatrix(int ni, int mi, int nf, int mf) const {
	if (ni > 0 && mi > 0) {
		if (nf > f)
			nf = f;
		if (mf > c)
			mf = c;
		int n = nf - ni + 1;
		int m = mf - mi + 1;
		Matrix<T> tmp(n, m);

		for (int i = 0; i < n; i++)
			for (int j = 0; j < m; j++) {
				tmp.matrix[i][j] = matrix[i + ni - 1][j + mi - 1];
			}

		return tmp;
	} else {
		return *this;
	}
}

template<class T>
Matrix<T> Matrix<T>::conMatrix(const Matrix<T> &mat) const {
	if (f == mat.f) {
		Matrix<T> tmp(f, c + mat.c);

		for (int i = 0; i < f; i++) {
			for (int j = 0; j < c; j++)
				tmp.matrix[i][j] = matrix[i][j];
			for (int j2 = 0; j2 < mat.c; j2++)
				tmp.matrix[i][j2 + c] = mat.matrix[i][j2];
		}

		return tmp;
	} else
		return *this;
}

template<class T>
Matrix<T>& Matrix<T>::fill(const T vec[], const int tam) {
	int i = 0, n = 0, m;
	while (i < tam && n < f) {
		m = 0;
		while (i < tam && m < c) {
			matrix[n][m] = vec[i];
			m++;
			i++;
		}
		n++;
	}

	return *this;
}

template<class T>
bool Matrix<T>::isSquare(void) const {
	return (c == f);
}

template<class T>
bool Matrix<T>::isIdentity(void) {
	bool ident = false;
	if (f == c) {
		Matrix<T> aux(f, c);
		aux.setIdentity();
		if (*this == aux)
			ident = true;
	}
	return ident;
}

template<class T>
bool Matrix<T>::isNegative(void) {
	bool notPos = true;

	for (int n = 0; n < f && notPos; n++)
		for (int m = 0; m < c && notPos; m++)
			if (matrix[n][m] > 0)
				notPos = false;

	return notPos;
}

template<class T>
bool Matrix<T>::isPositive(void) {
	bool notPos = true;

	for (int n = 0; n < f && notPos; n++)
		for (int m = 0; m < c && notPos; m++)
			if (matrix[n][m] < 0)
				notPos = false;

	return notPos;
}

template<class T>
bool Matrix<T>::isOneNegative(void) {
	bool Negat = false;

	for (int n = 0; n < f && !Negat; n++)
		for (int m = 0; m < c && !Negat; m++)
			if (matrix[n][m] < 0)
				Negat = true;

	return Negat;
}

template<class T>
bool Matrix<T>::isOnePositive(void) {
	bool Posit = false;

	for (int n = 0; n < f && !Posit; n++)
		for (int m = 0; m < c && !Posit; m++)
			if (matrix[n][m] > 0)
				Posit = true;

	return Posit;
}

template<class T>
void Matrix<T>::setIdentity(void) {
	if (isSquare()) {
		for (int n = 0; n < f; n++) {
			matrix[n][n] = 1;
			for (int m = 0; m < n; m++) {
				matrix[m][n] = 0;
				matrix[n][m] = 0;
			}
		}
	}
}

template<class T>
T Matrix<T>::determinant(void) const {
	int aux;
	T det(0);
	T mult(0);
	if (isSquare()) {
		if (f == 1) {
			det = matrix[0][0];
		} else if (f == 2) {
			det = matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
		} else {
			for (int n = 0; n < f; n++) {
				mult = 1;
				for (int m = 0; m < c; m++) {
					mult = mult * matrix[(m + n) % f][m];
				}
				det = det + mult;
				/*}for(int n=f-1; n>=0; n--){*/
				mult = 1;
				for (int m = 0; m < c; m++) {
					aux = f - 1 - n - m; //f-1: ya que usamos la misma n, si eliminamos f-1 hay que descomentar el for.
					if (aux < 0) {
						aux += f;
					}
					mult = mult * matrix[aux][m];
				}
				det = det - mult;
			}
		}
	}
	return det;
}

template<class T>
Matrix<T> Matrix<T>::transpose(void) const {
	Matrix<T> tmp(f, c);

	if (isSquare()) {
		for (int n = 0; n < f; n++) {
			tmp.matrix[n][n] = matrix[n][n];
			for (int m = 0; m < n; m++) {
				tmp.matrix[m][n] = matrix[n][m];
				tmp.matrix[n][m] = matrix[m][n];
			}
		}
	}
	return tmp;
}

template<class T>
Matrix<T> Matrix<T>::cofactors(void) const {
	int i, i1, ii, j, j1, jj;
	T det;
	Matrix<T> aux(f, c);
	Matrix<T> tmp(f, c);
	if (isSquare()) {
		for (j = 0; j < f; j++) //Recorre para cada Mij
				{
			for (i = 0; i < f; i++) {
				i1 = 0;
				for (ii = 0; ii < f; ii++) //Todos los aij, para crear la matrix aux.
						{
					if (ii != i) //los elementos de mi fila, no lo cojo.
							{
						j1 = 0;
						for (jj = 0; jj < f; jj++) {
							if (jj != j) //si el elemento es de mi columna, no lo cojo.
									{
								aux.matrix[i1][j1] = matrix[ii][jj];
								j1++;
							}
						}
						i1++;
					}
				}
				det = aux.chaMatrix(f - 1, c - 1).determinant();
				if (((i + j + 2) % 2) != 0)
					det = det * -1;
				tmp.matrix[i][j] = det;
			}
		}
	}
	return tmp;
}

template<class T>
Matrix<T> Matrix<T>::adjoint(void) const {
	return cofactors().transpose();
}

template<class T>
Matrix<T> Matrix<T>::inverseDet(void) const {
	return adjoint() / determinant();
}

template<class T>
Matrix<T>& Matrix<T>::operatorFF(const int n1, const char op, const int n2) {
	if (op == '-') {
		for (int m = 0; m < c; m++) //Restamos la fila i y la fila n para conseguir el cero;
				{
			matrix[n1][m] = matrix[n1][m] - matrix[n2][m];
		}
	} else if (op == '+') {
		for (int m = 0; m < c; m++) //Restamos la fila i y la fila n para conseguir el cero;
				{
			matrix[n1][m] = matrix[n1][m] + matrix[n2][m];
		}
	}
	return *this;
}

template<class T>
Matrix<T>& Matrix<T>::operatorFFR(const int n1, const char op, const int n2,
		const T rat) {
	if (op == '-') {
		for (int m = 0; m < c; m++) //Restamos la fila i y la fila n para conseguir el cero;
				{
			matrix[n1][m] = matrix[n1][m] - (matrix[n2][m] * rat);
		}
	} else if (op == '+') {
		for (int m = 0; m < c; m++) //Restamos la fila i y la fila n para conseguir el cero;
				{
			matrix[n1][m] = matrix[n1][m] + (matrix[n2][m] / rat);
		}
	}
	return *this;
}

template<class T>
Matrix<T>& Matrix<T>::operatorFR(const int n, const char op, const T rat) {
	if (op == '/') {
		for (int m = 0; m < c; m++) //consigue el 1 en (i,i) y aplica a toda slas columas
				{
			matrix[n][m] = matrix[n][m] / rat;
		}
	} else if (op == '*') {
		for (int m = 0; m < c; m++) //consigue el 1 en (i,i) y aplica a toda slas columas
				{
			matrix[n][m] = matrix[n][m] * rat;
		}
	}
	return *this;
}

template<class T>
Matrix<T>& Matrix<T>::operatorCF(const Matrix<T> mat, const int n) {
	if (f == mat.f && c == mat.c) {
		for (int m = 0; m < c; m++) //consigue el 1 en (i,i) y aplica a toda slas columas
				{
			matrix[n][m] = mat.matrix[n][m];
		}
	}
	return *this;
}

template<class T>
bool Matrix<T>::colZeros(const int col) {
	bool zeros = true;
	T zero(0);

	if (col < c)
		for (int n = 0; n < f; n++)
			if (matrix[n][col] != zero)
				zeros = false;

	return zeros;
}

template<class T>
int Matrix<T>::filPivot(const int fil, const int col) {
	int piv = 0;
	T zero(0);
	if (fil < f && col < c) {
		for (int n = 0; n < f; n++) {
			if (n != fil)
				if (matrix[n][col] != zero)
					piv = n;
		}
	}
	return piv;
}

template<class T>
Matrix<T> Matrix<T>::inverse(void) const {
	if (isSquare()) {
		Matrix<T> tmp(f, c), ident(f, c), aux(f, c * 2);
		ident.setIdentity();
		tmp = *this;
		tmp = tmp.conMatrix(ident);
		T den(0);
		int a;

		for (int i = 0; i < f; i++) //repeticion pora cada 1
				{
			if (!tmp.colZeros(i)) {
				den = tmp.matrix[i][i];
				if (den != 0)
					tmp.operatorFR(i, '/', den); //consigue el 1 en (i,i) y aplica a toda slas columas
				else {
					a = tmp.filPivot(i, i); //busco cualquier elemento distitno de 0 en la columna i que no sea el elemento (i,i)
					den = tmp.matrix[a][i]; //uso ese elemento (a,1) para poder generar el 1 en el elemento (i,i)
					/*aux=tmp;//version ineficiente
					 aux.operatorFR(a,'/',den);
					 aux.operatorFF(i,'+',a);
					 tmp.operatorCF(aux,i);*/
					tmp.operatorFFR(i, '+', a, den);
				}
				for (int n = 0; n < f; n++) //conseigue el 0 en las filas de esa columna
						{
					if (i != n) //Si no soy la fila del 1
							{
						den = tmp.matrix[n][i];
						if (den != 0) //si ya es 0 no hace falta hacer nada
								{
							/*aux=tmp; //version ineficiente
							 aux.operatorFR(i,'*',den);//consigue el 1 en (m,i) y aplica a toda slas columas
							 aux.operatorFF(n,'-',i);//Restamos la fila i y la fila n para conseguir el cero;
							 tmp.operatorCF(aux,n);*/
							tmp.operatorFFR(n, '-', i, den);
						}
					}
				}
			}
		}
		if (tmp.subMatrix(1, 1, f, c).isIdentity())
			return tmp.subMatrix(1, c + 1, f, c * 2);
	}
	return *this;
}

template<class T>
Matrix<T>& Matrix<T>::baseTo(const vector<int> base) {
	int tam = base.size();
	if (tam == f) {
		T den(0);
		int a;
		int i = 0;
		for (int p = 0; p < tam; p++) {
			i = base[p] - 1;
			if (!colZeros(i)) {
				den = matrix[p][i];
				if (den != 0)
					operatorFR(p, '/', den); //consigue el 1 en (i,i) y aplica a toda slas columas
				else {
					a = filPivot(p, i); //busco cualquier elemento distitno de 0 en la columna i que no sea el elemento (i,i)
					den = matrix[a][i]; //uso ese elemento (a,1) para poder generar el 1 en el elemento (i,i)
					operatorFFR(p, '+', a, den);
				}
				for (int n = 0; n < f; n++) //conseigue el 0 en las filas de esa columna
						{
					if (p != n) //Si no soy la fila del 1
							{
						den = matrix[n][i];
						if (den != 0) //si ya es 0 no hace falta hacer nada
							operatorFFR(n, '-', p, den);
					}
				}
			}
		}
	}
	return *this;
}

#endif /* MATRIX_H_ */
