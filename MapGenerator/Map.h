#pragma once

#include "Delaunay.h"
#include <vector>

// Forward Declarations
class Vec2;
namespace noise {
	namespace module {
		class Perlin;
	}
}


class Map {
public:
	struct Biome{
		enum Type{
			Snow,
			Tundra,
			Bare,
			Scorched,
			Taiga,
			Shrubland,
			TemperateDesert,
			TemperateRainForest,
			TemperateDeciduousForest,
			Grassland,
			TropicalRainForest,
			TropicalSeasonalForest,
			SubtropicalDesert,

			Size,
			None
		};
	};

	struct center {
		center(Delaunay::center *c_) : c(c_), water(false), ocean(false), coast(false),
			border(false), biome(Biome::None), elevation(0.0), moisture(0.0) {}

		Delaunay::center *c;

		bool water;
		bool ocean;
		bool coast;
		bool border;
		Biome::Type biome;
		double elevation;
		double moisture;
	};

	struct edge {
		edge(Delaunay::edge * c_ = NULL) {
			c = c_;
			if(c != NULL){
				c = c_;
				Vec2 v0 = c->v0 == NULL ? c->d0->position + (c->d1->position - c->d0->position) / 2 : c->v0->position;
				Vec2 v1 = c->v1 == NULL ? c->d0->position + (c->d1->position - c->d0->position) / 2 : c->v1->position;
				voronoi_midpoint = (v0 + v1) / 2;
			}
		}

		Delaunay::edge *c;

		Vec2 voronoi_midpoint;
		double river_volume;
	};

	struct corner{
		corner(Delaunay::corner * c_ = NULL) : c(c_), ocean(false), water(false), coast(false), 
			elevation(0.0), moisture(0.0), river_volume(0.0), downslope(NULL) {}

		Delaunay::corner *c;

		bool ocean;
		bool water;
		bool coast;
		bool border;
		double elevation;
		double moisture;
		double river_volume;
		corner *downslope;
	};

	Map(void);
	~Map(void);

	Map(int width, int height, int point_count);

	void Generate();

	void GeneratePolygons();
	void GenerateLand();

	bool LoadFile(string file_name);
	bool WriteFile(string file_name);

	vector<edge *> GetEdges();
	vector<corner *> GetCorners();
	vector<center *> GetCenters();
private:
	int map_width;
	int map_height;
	Delaunay * triangulation;
	double z_coord;
	noise::module::Perlin * noiseMap;

	vector<edge *> edges;
	vector<corner *> corners;
	vector<center *> centers;

	bool IsIsland(Vec2 position);

	void LloydRelaxation();
};

