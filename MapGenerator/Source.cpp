
#include "delaunay/Delaunay.h"

#include <SFML\Graphics.hpp>
#include <SFML\Window.hpp>

#include <iostream>
using namespace std;

const int WIDTH = 800;
const int HEIGHT = 600;

const int POINT_SIZE = 2;
const int LINE_SIZE = 1;

sf::Color DELAUNAY_COLOR = sf::Color::Blue;
sf::Color VORONOI_COLOR = sf::Color::Red;

void drawLine(Vec2 a, Vec2 b, sf::Color c, sf::RenderWindow *window);
void drawEdge(Delaunay::edge *e, sf::RenderWindow *window);
void drawCorner(Delaunay::corner *c, sf::RenderWindow *window);
void drawCenter(Delaunay::center *c, sf::RenderWindow *window);
vector<Vec2> LloydRelaxation(list<Delaunay::center *> centers);
Vec2 CalculateCentroid(Delaunay::center *center);

int main(){

	bool show_delaunay = true;
	bool show_voronoi = true;

	Delaunay triangulation(Vec2(0,0), Vec2(WIDTH, HEIGHT));
	list<Delaunay::edge *> edges = triangulation.GetBorders();
	list<Delaunay::corner *> corners = triangulation.GetCorners();
	list<Delaunay::center *> centers = triangulation.GetCenters();
	vector<Vec2> points;

	/*srand(time(NULL));
	for(int i = 0; i < 5000; i++){
	points.push_back(Vec2(rand()%WIDTH, rand()%HEIGHT));
	}
	triangulation.AddPoints(points);*/

	sf::RenderWindow * app = new sf::RenderWindow(sf::VideoMode(WIDTH,HEIGHT,32), "Map Generator");
	app->setFramerateLimit(60);

	bool running = true;
	while(running){
		sf::Event event;
		while(app->pollEvent(event)){
			if(event.type == sf::Event::Closed){
				running = false;
			}else if(event.type == sf::Event::KeyPressed){
				sf::Clock timer;
				sf::Image screen;
				switch(event.key.code){
				case sf::Keyboard::Escape:
					running = false;
					break;
				case sf::Keyboard::C:
					timer.restart();
					triangulation.Continue();
					cout << timer.getElapsedTime().asMicroseconds() / 1000000.0 << endl;
					edges = triangulation.GetBorders();
					centers = triangulation.GetCenters();
					corners = triangulation.GetCorners();
					break;
				case sf::Keyboard::S:
					timer.restart();
					triangulation.Step();
					cout << timer.getElapsedTime().asMicroseconds() / 1000000.0 << endl;
					edges = triangulation.GetBorders();
					centers = triangulation.GetCenters();
					corners = triangulation.GetCorners();
					break;
				case sf::Keyboard::F:
					triangulation.Finish();
					edges = triangulation.GetBorders();
					centers = triangulation.GetCenters();
					corners = triangulation.GetCorners();
					break;
				case sf::Keyboard::D:
					show_delaunay = !show_delaunay;
					break;
				case sf::Keyboard::V:
					show_voronoi = !show_voronoi;
					break;
				case sf::Keyboard::P:
					cout << "debug" << endl;
					break;
				case sf::Keyboard::L:
					timer.restart();
						points = LloydRelaxation(centers);
						triangulation.CleanUp();
						triangulation.CreateBorders(Vec2(0,0), Vec2(WIDTH, HEIGHT));
						triangulation.AddPoints(points);
						triangulation.Continue();
						edges = triangulation.GetBorders();
						centers = triangulation.GetCenters();
						corners = triangulation.GetCorners();
					cout << edges.size() << " " << centers.size() << " " << corners.size() << endl;
					cout << timer.getElapsedTime().asMicroseconds() / 1000000.0 << endl;
					break;
				case sf::Keyboard::F1:
					screen = app->capture();
					screen.saveToFile("screenshot.jpg");
					break;
				default:
					break;
				}
			}else if(event.type == sf::Event::MouseButtonPressed){
				switch(event.mouseButton.button){
				case sf::Mouse::Button::Left:
					Vec2 p(event.mouseButton.x, event.mouseButton.y);
					points.push_back(p);
					triangulation.AddPoint(p);
				}
			}
		}

		app->clear(sf::Color::White);

		if(!edges.empty()){
			list<Delaunay::edge *>::iterator edge_iter, edges_end = edges.end();
			for(edge_iter = edges.begin(); edge_iter != edges_end; edge_iter++){
				drawEdge(*edge_iter, app);
			}
		}

		if(!corners.empty()){
			list<Delaunay::corner *>::iterator corner_iter, corners_end = corners.end();
			for(corner_iter = corners.begin(); corner_iter != corners_end; corner_iter++){
				drawCorner(*corner_iter, app);
			}
		}

		if(!centers.empty()){
			list<Delaunay::center *>::iterator center_iter, centers_end = centers.end();
			for(center_iter = centers.begin(); center_iter != centers_end; center_iter++){
				drawCenter(*center_iter, app);
			}
		}

		if(!points.empty()){
			sf::CircleShape punto;
			punto.setFillColor(sf::Color::Black);
			punto.setRadius(POINT_SIZE);
			vector<Vec2>::iterator vec2_iter, points_end = points.end();
			for(vec2_iter = points.begin(); vec2_iter != points_end; vec2_iter++){
				punto.setPosition(vec2_iter->x - POINT_SIZE, vec2_iter->y - POINT_SIZE);
				app->draw(punto);
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

void drawEdge(Delaunay::edge *e, sf::RenderWindow *window){
	Vec2 v0 = e->v0 == NULL ? e->d0->position + (e->d1->position - e->d0->position) / 2 : e->v0->position;
	Vec2 v1 = e->v1 == NULL ? e->d0->position + (e->d1->position - e->d0->position) / 2 : e->v1->position;
	Vec2 d0 = e->d0 == NULL ? e->v0->position + (e->v1->position - e->v0->position) / 2 : e->d0->position;
	Vec2 d1 = e->d1 == NULL ? e->v0->position + (e->v1->position - e->v0->position) / 2 : e->d1->position;
	if(v0 != NULL && v1 != NULL)
		drawLine(v0, v1, VORONOI_COLOR, window);
	//	if(d0 != NULL && d1 != NULL)
	//		drawLine(d0, d1, DELAUNAY_COLOR, window);
}

void drawCorner( Delaunay::corner *c, sf::RenderWindow *window ) {
	sf::CircleShape point;
	point.setFillColor(VORONOI_COLOR);
	point.setPosition(c->position.x - POINT_SIZE, c->position.y - POINT_SIZE);
	point.setRadius(POINT_SIZE);
	window->draw(point);
	//sf::CircleShape circumference;
	//circumference.setOutlineColor(VORONOI_COLOR);
	//circumference.setOutlineThickness(0.25);
	//circumference.setFillColor(sf::Color::Transparent);
	//double radius = Vec2(c->position, c->centers[0]->position).Length();
	//circumference.setPosition(c->position.x - radius, c->position.y - radius);
	//circumference.setRadius(radius);
	//window->draw(circumference);
}

void drawCenter( Delaunay::center *c, sf::RenderWindow *window ) {
	sf::CircleShape point;
	point.setFillColor(DELAUNAY_COLOR);
	point.setPosition(c->position.x - POINT_SIZE, c->position.y - POINT_SIZE);
	point.setRadius(POINT_SIZE);
}

vector<Vec2> LloydRelaxation( list<Delaunay::center *> centers ) {
	vector<Vec2> new_points;
	list<Delaunay::center *>::iterator center_iter, centers_end = centers.end();
	for(center_iter = centers.begin(); center_iter != centers_end; center_iter++){
		new_points.push_back(CalculateCentroid(*center_iter));
	}
	return new_points;
}

Vec2 CalculateCentroid( Delaunay::center *center ) {
	Vec2 center_centroid;
	vector<Delaunay::corner *>::iterator corner_iter, corners_end = center->corners.end();
	for(corner_iter = center->corners.begin(); corner_iter != corners_end; corner_iter++){
		center_centroid += (*corner_iter)->position;
	}
	center_centroid /= center->corners.size();
	return center_centroid;
}
