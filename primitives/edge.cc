#include "edge.h"
#include <float.h>


Edge::Edge (Point _org, Point _dest) : org (_org), dest (_dest) {}

Edge::Edge () : org (Point (0,0)), dest (Point (1,0)) {}

bool Edge::operator < (Edge &edge)
{
	if (org  < edge.org) return true;
	if (org  > edge.org) return true;
	if (dest < edge.dest) return false;
	if (dest > edge.dest) return true;
	return true;
}

Edge &Edge::rotate()
{
	Point sum_od = org + dest;	
	Point median = 0.5 * sum_od;
	Point res_od = dest - org;
	Point normal(res_od.y, -res_od.x);
	org = median - 0.5 * normal;
	dest = median + 0.5 * normal;
	return *this;
}

Edge &Edge::flip()
{
	return rotate().rotate();
}

Point Edge::point(double par)
{
	Point tmp = dest - org;
	return Point(org + par * tmp);
}

int Edge::intersect(Edge &e, double &par)
{
	Point a = org;
	Point b = dest;
	Point c = e.org;
	Point d = e.dest;
	Point n = Point((d-c).y, (c-d).x);
	double denom = n.x * (b-a).x + n.y * (b-a).y;
  
	if (denom == 0.0) 
	{
		int loc = org.locate(e);
		if ((loc == RIGHT) || (loc == LEFT))
		{
			return PARALLEL;
		}
		else
		{ 
			return COLLINEAR;
		}
	}

	double num = n.x * (a-c).x + n.y * (a-c).y;
	par = - num / denom;
	return SKEW;
}

bool Edge::isVertical()
{
	return (org.x == dest.x);
}
