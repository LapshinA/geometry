#pragma once

#include "vertex.h"
#include "edge.h"

class Polygon {
private:
	Vertex *_v;
	int _size;
	void resize();
public:
	Polygon();
	Polygon(Polygon& polygon);
	Polygon(Vertex* vertex);
	~Polygon ();
	Vertex *get_vertex();
	int size();
	Point point();
	Edge edge();
	Vertex *cw();
	Vertex *ccw();
	Vertex *neighbor(int rotation);
	Vertex *advance(int rotation);
	Vertex *set_vertex(Vertex* vertex);
	Vertex *insert(Point& point);
	void remove();
	Polygon *split(Vertex* vertex);
};
