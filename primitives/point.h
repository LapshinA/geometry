#pragma once

#include <cmath>

class Edge;
enum 
{
	LEFT,  
	RIGHT,  
	BEFORE,  
	BEHIND, 
	BETWEEN, 
	BEGIN, 
	END
};

class Point{

public:

double x,y;
Point(double  cx = 0.0, double cy = 0.0) : x(cx), y(cy) {}

bool operator>(Point &point)  { return (x > point.x) || (x == point.x && y > point.y);}
bool operator<(Point &point)  { return (x < point.x) || (x == point.x && y < point.y);}
bool operator!=(Point &point) { return (x != point.x && y != point.y);}
bool operator==(Point &point) { return (x == point.x && y == point.y);}

Point operator+(Point point)  { return Point(x + point.x, y + point.y);}
Point operator-(Point point)  { return Point(x - point.x, y - point.y);}
double operator[](int index){ return index == 0 ? x : y;}

friend Point operator*(double s, Point &point);

int locate(Point &point1, Point &point2);
int locate(Edge &edge);
double length() { return sqrt(x*x + y*y);}
double distance(Edge &edge);
};
