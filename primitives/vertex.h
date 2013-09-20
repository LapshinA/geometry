#pragma once

#include "node.h"
#include "point.h"

enum 
{
	CW,
	CCW
};

class Vertex: public Node, public Point  {
public:
	Vertex(double x, double y);
	Vertex(Point);
	Vertex *cw();
	Vertex *ccw();
	Vertex *neighbor(int rotation);
	Point point();
	Vertex *insert(Vertex* vertex);
	Vertex *remove();
	void splice(Vertex* vertex);
	Vertex *split(Vertex* vertex);
	friend class Polygon;
};
