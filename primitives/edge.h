#pragma once

#include "point.h"

enum 
{ 
	COLLINEAR, 
	PARALLEL, 
	SKEW, 
	SKEW_CROSS, 
	SKEW_NO_CROSS 
};

class Edge {
	
public:
	Point org;
	Point dest;
	Edge(Point _org, Point _dest);
	Edge();
	Edge &rotate();
	Edge &flip();
	Point point(double par);
	int intersect (Edge& edge, double& par);
	bool isVertical();
	bool operator < (Edge &edge);
	bool operator == (Edge &edge) {return (org == edge.org && dest == edge.dest);}
};
