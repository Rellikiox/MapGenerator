/*
* Triangulation.cc
*
*  Created on: Nov 18, 2011
*      Author: rellik
*/

#include "Triangulation.h"

/*
* Clase Triangulation
*/

Triangulation::Triangulation(Point i, Point f) {

	createBorders(i, f);

}

Triangulation::Triangulation(const Triangulation &T){
	pxpy = T.pxpy;
	pxny = T.pxny;
	nxpy = T.nxpy;
	nxny = T.nxny;

	Tri *tri0 = new Tri(pxpy, nxny, nxpy);
	Tri *tri1 = new Tri(pxpy, pxny, nxny);

	tri0->setCa(tri1);
	tri1->setAb(tri0);

	// Los insertamos

	triangles.push_back(tri0);
	triangles.push_back(tri1);
}

Triangulation & Triangulation::operator=(const Triangulation &T){
	if(this != &T){
		this->~Triangulation();

		pxpy = T.pxpy;
		pxny = T.pxny;
		nxpy = T.nxpy;
		nxny = T.nxny;

		Tri *tri0 = new Tri(pxpy, nxpy, nxny);
		Tri *tri1 = new Tri(pxpy, nxny, pxny);

		tri0->setCa(tri1);
		tri1->setAb(tri0);

		// Los insertamos

		triangles.push_back(tri0);
		triangles.push_back(tri1);
	}
	return *this;
}

void Triangulation::createBorders(Point i, Point f) {

	double px;
	double nx;
	double py;
	double ny;

	if(f.getX() > i.getX()){
		px = f.getX();
		nx = i.getX();
	}else{
		px = i.getX();
		nx = f.getX();
	}

	if(f.getY() > i.getY()){
		py = f.getY();
		ny = i.getY();
	}else{
		py = i.getY();
		ny = f.getY();
	}

	std::vector<Point>::iterator piter;

	// Añadimos un exceso grande para que no interfieran con la triangulización

	double excesoX = abs(px - nx)*2;
	double excesoY = abs(py - ny)*2;

	px += excesoX;
	nx -= excesoX;
	py += excesoY;
	ny -= excesoY;

	pxpy.setXY(px, py);
	pxny.setXY(px, ny);
	nxpy.setXY(nx, py);
	nxny.setXY(nx, ny);

	// Creamos los dos triangulos iniciales

	Tri *tri0 = new Tri(pxpy, nxpy, nxny);
	Tri *tri1 = new Tri(pxpy, nxny, pxny);

	tri0->setCa(tri1);
	tri1->setAb(tri0);

	// Los insertamos

	if(!triangles.empty()){
		vector<Tri *>::iterator triter;
		for(triter = triangles.begin(); triter != triangles.end(); triter++){
			delete *triter;
		}
		triangles.clear();
	}

	triangles.push_back(tri0);
	triangles.push_back(tri1);
}

Triangulation::Triangulation(std::vector<Point> vPuntos) {
	puntos = vPuntos;
}

std::vector<Tri *> Triangulation::getTriangles() {
	return triangles;
}

Triangulation::~Triangulation() {
	puntos.clear();
	triangles.clear();
}

bool Triangulation::addPoint(Point p) {

	std::vector<Tri *>::iterator iter;
	bool found = false;

	for (iter = triangles.begin(); iter != triangles.end() && !found; iter++) {
		if ((*iter)->getA() == p || (*iter)->getB() == p || (*iter)->getC() == p)
			found = true;
	}

	if (!found){
		puntos.push_back(p);

	}

	return !found;
}

void Triangulation::addPoints(std::vector<Point> vPoints) {
	std::vector<Point>::iterator iter1;
	std::vector<Point>::iterator iter2;
	bool found;

	for (iter1 = vPoints.begin(); iter1 != vPoints.end(); iter1++) {
		found = false;

		for (iter2 = puntos.begin(); iter2 != puntos.end() && !found; iter2++) {
			if (*iter2 == *iter1)
				found = true;
		}

		if (!found)
			puntos.push_back(*iter1);
	}
}

void Triangulation::continueDelaunay() {

	while (!puntos.empty()) {
		stepDelaunay();
	}

}

std::vector<Tri *> Triangulation::finishedDelaunay() {
	/*
	* Quitamos los cuatro puntos extra iniciales y sus triangulos.
	* Si se han escogido bien estos puntos no deberian modificar la
	* triangulización, por lo que podemos eliminarlos sin problema.
	*/

	removeP(pxpy);
	removeP(pxny);
	removeP(nxpy);
	removeP(nxny);

	return triangles;
}

bool Triangulation::stepDelaunay() {

	if (!puntos.empty()) {

		int rel;
		Tri *origen;
		bool encontrado = false;
		std::vector<Tri *>::iterator triter;

		for (triter = triangles.begin(); !encontrado && triter != triangles.end(); triter++) {
			rel = (*triter)->isInside(puntos[0]);
			if(rel == 0 || rel == 1){
				origen = *triter;
				break;
			}
		}

		if (rel == 1) {
			// Creamos los nuevos triangulos

			Tri *nab = new Tri(origen->getA(), origen->getB(), puntos[0],
				origen->getAb(), NULL, NULL);
			Tri *nbc = new Tri(origen->getB(), origen->getC(), puntos[0],
				origen->getBc(), NULL, nab);
			Tri *nca = new Tri(origen->getC(), origen->getA(), puntos[0],
				origen->getCa(), nab, nbc);

			// Les añadimos los triangulos adyacentes que faltan

			nab->setBc(nbc);
			nab->setCa(nca);
			nbc->setBc(nca);

			/*
			* Actualizo los triangulos que tienen frontera con el antiguo
			* origen y los cambio por los de los nuevos triangulos
			*/

			origen->updateEdges(nab, nbc, nca);

			// Los metemos en la triangulización

			triangles.push_back(nab);
			triangles.push_back(nbc);
			triangles.push_back(nca);

			triangles.erase(
				remove(triangles.begin(), triangles.end(), origen),
				triangles.end());

			// Los legalizamos

			legalize(nab, 0);
			legalize(nbc, 0);
			legalize(nca, 0);

		} else if (rel == 0) {
			/*
			* El punto esta en una linea, por lo que hay que
			* crear cuatro triangulos en vez de tres
			*/
			Tri *vecino;

			Point I, J, K, L;
			Tri *IL, *LJ, *JK, *KI;

			// Encontramos en que frontera de origen esta el punto

			if (origen->getAb() != NULL
				&& origen->getAb()->isInside(puntos[0]) == 0) {
					// Si el punto esta en la linea AB

					// Triangulo que hace frontera

					vecino = origen->getAb();

					/*
					*  Los cuatro puntos que forman los dos triangulos.
					*  IJK es origen y ILJ es vecino
					*/

					I = origen->getA();
					J = origen->getB();
					K = origen->getC();
					L = vecino->opositeP(origen);

					/*
					* Vector con los triangulos adjacentes a vecino (menos origen)
					* Devuelve los triangulos frontera de vecino, sin contar a origen,
					* en sentido horario
					*/

					std::vector<Tri *> opuTri = vecino->opositeT(origen);

					// Punteros a los triangulos que rodean origen y vecino

					IL = opuTri[0];
					LJ = opuTri[1];
					JK = origen->getBc();
					KI = origen->getCa();

			} else if (origen->getBc() != NULL
				&& origen->getBc()->isInside(puntos[0]) == 0) {
					// Si el punto esta en la linea BC

					vecino = origen->getBc();

					I = origen->getB();
					J = origen->getC();
					K = origen->getA();
					L = vecino->opositeP(origen);

					std::vector<Tri *> opuTri = vecino->opositeT(origen);

					IL = opuTri[0];
					LJ = opuTri[1];
					JK = origen->getCa();
					KI = origen->getAb();

			} else if (origen->getCa() != NULL) {
				// Si el punto esta en CA

				vecino = origen->getCa();

				I = origen->getC();
				J = origen->getA();
				K = origen->getB();
				L = vecino->opositeP(origen);

				std::vector<Tri *> opuTri = vecino->opositeT(origen);

				IL = opuTri[0];
				LJ = opuTri[1];
				JK = origen->getAb();
				KI = origen->getBc();
			}

			// Creamos los cuatro triagulos

			Tri *nil = new Tri(I, L, puntos[0]);
			Tri *nlj = new Tri(L, J, puntos[0]);
			Tri *njk = new Tri(J, K, puntos[0]);
			Tri *nki = new Tri(K, I, puntos[0]);

			// Asignamos las fronteras

			nil->setTris(IL, nlj, nki);
			nlj->setTris(LJ, njk, nil);
			njk->setTris(JK, nki, nlj);
			nki->setTris(KI, nil, njk);

			// Los añadimos a la triangularización

			triangles.push_back(nil);
			triangles.push_back(nlj);
			triangles.push_back(njk);
			triangles.push_back(nki);

			if (IL != NULL)
				IL->swapP(vecino, nil);
			if (LJ != NULL)
				LJ->swapP(vecino, nlj);
			if (JK != NULL)
				JK->swapP(origen, njk);
			if (KI != NULL)
				KI->swapP(origen, nki);

			triter = triangles.erase(
				remove(triangles.begin(), triangles.end(), origen),
				triangles.end());
			triter = triangles.erase(
				remove(triangles.begin(), triangles.end(), vecino),
				triangles.end());

			// Legalizamos las nuevas fronteras

			legalize(nil, 0);
			legalize(nlj, 0);
			legalize(njk, 0);
			legalize(nki, 0);

		}

		puntos.erase(puntos.begin());

		if(rel < 0)
			return false;
		else
			return true;
	}

	return false;
}

void Triangulation::removeP(Point p) {

	//	////cout << "Borrando: " << p << endl;

	std::vector<Tri *>::iterator iter = triangles.begin();

	while (iter != triangles.end()) {
		if ((*iter)->getA() == p || (*iter)->getB() == p || (*iter)->getC() == p) {
			(*iter)->updateEdges(NULL, NULL, NULL);
			triangles.erase(remove(triangles.begin(), triangles.end(), *iter),
				triangles.end());
		} else {
			iter++;
		}
	}

}

void Triangulation::removeT(Tri *tri){
	//////cout << ">> Borro: " << *tri << endl;
	tri->updateEdges(NULL, NULL, NULL);
	std::vector<Tri *>::iterator iter;
	bool borrado = false;
	for(iter = triangles.begin(); iter != triangles.end() && !borrado; iter++){
		if(*iter == tri){
			triangles.erase(iter);
			borrado = true;
		}
	}
}

void Triangulation::legalize(Tri *ori, int caso) {

	Point i, j, k, l;
	bool computable = true;

	switch (caso) {
	case 0:
		if (ori->getAb() != NULL) {
			i = ori->getA();
			j = ori->getB();
			k = ori->getC();
			l = ori->opositeP(0);
		} else
			computable = false;
		break;
	case 1:
		if (ori->getBc() != NULL) {
			i = ori->getB();
			j = ori->getC();
			k = ori->getA();
			l = ori->opositeP(1);
		} else
			computable = false;
		break;
	case 2:
		if (ori->getCa() != NULL) {
			i = ori->getC();
			j = ori->getA();
			k = ori->getB();
			l = ori->opositeP(2);
		} else
			computable = false;
		break;
	}

	if (computable && Tri::liesInside(i, j, k, l)) {
		//	////cout << *ori << " tiene que voltearse" << endl;
		if(ori != NULL) flipEdge(ori, caso);
	}
}

void Triangulation::flipEdge(Tri * ori, int caso) {
	Tri *t1 = new Tri;
	Tri *t2 = new Tri;
	Tri *opu;
	std::vector<Tri *> opuTri;

	switch (caso) {
	case 0:

		// Creamos los nuevos triangulos

		t1->setA(ori->opositeP(caso));
		t1->setB(ori->getC());
		t1->setC(ori->getA());

		t2->setA(t1->getA());
		t2->setB(ori->getB());
		t2->setC(ori->getC());

		opu = ori->getAb();
		opuTri = opu->opositeT(ori);

		// Les ponemos las fronteras

		t1->setTris(t2, ori->getCa(), opuTri[0]);
		t2->setTris(opuTri[1], ori->getBc(), t1);

		// Actualizamos las fronteras de los vecinos

		if (opuTri[0] != NULL)
			opuTri[0]->swapP(opu, t1);
		if (opuTri[1] != NULL)
			opuTri[1]->swapP(opu, t2);
		ori->getCa()->swapP(ori, t1);
		ori->getBc()->swapP(ori, t2);

		break;
	case 1:
		t1->setA(ori->opositeP(caso));
		t1->setB(ori->getA());
		t1->setC(ori->getB());

		t2->setA(t1->getA());
		t2->setB(ori->getC());
		t2->setC(ori->getA());

		opu = ori->getBc();
		opuTri = opu->opositeT(ori);

		t1->setTris(t2, ori->getAb(), opuTri[0]);
		t2->setTris(opuTri[1], ori->getCa(), t1);

		// Actualizamos las fronteras de los vecinos

		if (opuTri[0] != NULL)
			opuTri[0]->swapP(opu, t1);
		if (opuTri[1] != NULL)
			opuTri[1]->swapP(opu, t2);
		ori->getAb()->swapP(ori, t1);
		ori->getCa()->swapP(ori, t2);

		break;
	case 2:
		t1->setA(ori->opositeP(caso));
		t1->setB(ori->getB());
		t1->setC(ori->getC());

		t2->setA(t1->getA());
		t2->setB(ori->getA());
		t2->setC(ori->getB());

		opu = ori->getCa();
		opuTri = opu->opositeT(ori);

		t1->setTris(t2, ori->getBc(), opuTri[0]);
		t2->setTris(opuTri[1], ori->getAb(), t1);

		// Actualizamos las fronteras de los vecinos

		if (opuTri[0] != NULL)
			opuTri[0]->swapP(opu, t1);
		if (opuTri[1] != NULL)
			opuTri[1]->swapP(opu, t2);
		ori->getBc()->swapP(ori, t1);
		ori->getAb()->swapP(ori, t2);

		break;
	}

	// Los añadimos a la triangularización

	triangles.push_back(t1);
	triangles.push_back(t2);

	// Borramos los antiguos

	triangles.erase(remove(triangles.begin(), triangles.end(), ori),
		triangles.end());
	triangles.erase(remove(triangles.begin(), triangles.end(), opu),
		triangles.end());


	// Legalizamos los bordes nuevos

	legalize(t1, 1);
	legalize(t1, 2);
	legalize(t2, 0);
	legalize(t2, 1);

}

std::vector<Point> Triangulation::quicksort(std::vector<Point> puntos) {
	if (puntos.size() <= 1)
		return puntos;

	std::vector<Point> less, more;

	Point p0 = puntos[0];

	Point pivote = puntos[puntos.size() / 2];

	std::vector<Point>::iterator iter;

	for (iter = puntos.begin(); iter != puntos.end(); iter++) {
		if (Point::distance(p0, (*iter)) <= Point::distance(p0, pivote)) {
			less.push_back(*iter);
		} else {
			more.push_back(*iter);
		}
	}

	less = quicksort(less);
	more = quicksort(more);

	less.push_back(pivote);
	less.insert(less.end(), more.begin(), more.end());

	return less;

}

void Triangulation::Reset(){
	pxpy = Point();
	pxny = Point();
	nxpy = Point();
	nxny = Point();

	puntos.clear();
	triangles.clear();
}
