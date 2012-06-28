
#include "Map.h"

#include "Structures.h"

#include <SFML\Graphics.hpp>
#include <SFML\Window.hpp>

#include "noise/noise.h"
using namespace noise;

#include <iostream>
using namespace std;

const int WIDTH = 800;
const int HEIGHT = 600;

const int POINT_SIZE = 1;
const int LINE_SIZE = 1;

sf::Color DELAUNAY_COLOR = sf::Color::Black;
sf::Color VORONOI_COLOR = sf::Color::Black;
sf::Color WATER_COLOR = sf::Color(sf::Uint8(52), sf::Uint8(58), sf::Uint8(94));
sf::Color LAND_COLOR = sf::Color(sf::Uint8(178), sf::Uint8(166), sf::Uint8(148));
sf::Color LAKE_COLOR = sf::Color(sf::Uint8(95), sf::Uint8(134), sf::Uint8(169));

void drawLine(Vec2 a, Vec2 b, sf::Color c, sf::RenderWindow *window);
void drawEdge(edge *e, sf::RenderWindow *window);
void drawCorner(corner *c, sf::RenderWindow *window);
void drawCenter(center *c, sf::RenderWindow *window);

int main(){

	bool show_delaunay = true;
	bool show_voronoi = true;

	Map mapa(WIDTH, HEIGHT, 1000);

	sf::Clock timer;
	timer.restart();
	mapa.Generate();
	cout << timer.getElapsedTime().asMicroseconds() / 1000000.0 << endl;

	vector<edge *> edges = mapa.GetEdges();
	vector<corner *> corners = mapa.GetCorners();
	vector<center *> centers = mapa.GetCenters();

	sf::RenderWindow * app = new sf::RenderWindow(sf::VideoMode(WIDTH,HEIGHT,32), "Map Generator");
	app->setFramerateLimit(60);

	double avg_fps = 0;
	double pases_count = 0;

	bool running = true;
	while(running){
		sf::Event event;
		while(app->pollEvent(event)){
			if(event.type == sf::Event::Closed){
				running = false;
			}else if(event.type == sf::Event::KeyPressed){
				sf::Image screen;
				switch(event.key.code){
				case sf::Keyboard::Escape:
					running = false;
					break;
				case sf::Keyboard::P:
					cout << "debug" << endl;
					break;
				case sf::Keyboard::F1:
					screen = app->capture();
					screen.saveToFile("screenshot.jpg");
					break;
				default:
					break;
				}
			}
		}

		app->clear(sf::Color::White);

		if(!centers.empty()){
			center::PVIter center_iter, centers_end = centers.end();
			for(center_iter = centers.begin(); center_iter != centers_end; center_iter++){
				drawCenter(*center_iter, app);
			}
		}

		if(!edges.empty()){
			edge::PVIter edge_iter, edges_end = edges.end();
			for(edge_iter = edges.begin(); edge_iter != edges_end; edge_iter++){
				drawEdge(*edge_iter, app);
			}
		}

		if(!corners.empty()){
			corner::PVIter corner_iter, corners_end = corners.end();
			for(corner_iter = corners.begin(); corner_iter != corners_end; corner_iter++){
				drawCorner(*corner_iter, app);
			}
		}

		app->display();
	}

	return 0;
}


void drawLine(Vec2 a, Vec2 b, sf::Color c, sf::RenderWindow *window){
	sf::Vertex line[2] = {
		sf::Vector2f(a.x, a.y),
		sf::Vector2f(b.x, b.y)
	};
	line[0].color = c;
	line[1].color = c;

	window->draw(line, 2, sf::Lines);
}

void drawEdge(edge *e, sf::RenderWindow *window){
	Vec2 v0 = e->v0 == NULL ? e->d0->position + (e->d1->position - e->d0->position) / 2 : e->v0->position;
	Vec2 v1 = e->v1 == NULL ? e->d0->position + (e->d1->position - e->d0->position) / 2 : e->v1->position;
	Vec2 d0 = e->d0 == NULL ? e->v0->position + (e->v1->position - e->v0->position) / 2 : e->d0->position;
	Vec2 d1 = e->d1 == NULL ? e->v0->position + (e->v1->position - e->v0->position) / 2 : e->d1->position;
	if(v0 != NULL && v1 != NULL)
		drawLine(v0, v1, VORONOI_COLOR, window);
	//	if(d0 != NULL && d1 != NULL)
	//		drawLine(d0, d1, DELAUNAY_COLOR, window);
}

void drawCorner( corner *c, sf::RenderWindow *window ) {
	sf::CircleShape point;
	if(c->water)
		point.setFillColor(WATER_COLOR);
	else
	{
		point.setFillColor(LAND_COLOR);
	}
	point.setPosition(c->position.x - POINT_SIZE, c->position.y - POINT_SIZE);
	point.setRadius(POINT_SIZE);
	window->draw(point);
}

void drawCenter( center *c, sf::RenderWindow *window ) {
	sf::ConvexShape polygon;
	polygon.setPointCount(c->corners.size());
	Vec2 min_point = c->corners[0]->position;
	for(int i = 0; i < c->corners.size(); i++){
		Vec2 aux = c->corners[i]->position;
		if(min_point.x >= aux.x)
			min_point.x = aux.x;
		if(min_point.y >= aux.y)
			min_point.y = aux.y;
		polygon.setPoint(i, sf::Vector2f(aux.x,aux.y));
	}
	if(c->water){
		polygon.setFillColor(WATER_COLOR);
	} else {
		polygon.setFillColor(LAND_COLOR);
	}
	polygon.setPosition(0,0);
	window->draw(polygon);

	sf::CircleShape punto;
	punto.setFillColor(DELAUNAY_COLOR);
	punto.setRadius(POINT_SIZE);
	punto.setPosition(c->position.x - POINT_SIZE, c->position.y - POINT_SIZE);
	window->draw(punto);
}
