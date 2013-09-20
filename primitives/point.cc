#include "point.h"
#include "edge.h"


Point operator*(double mul , Point &p)
{
  return Point(mul * p.x, mul * p.y);
}

int Point::locate(Point &p0, Point &p1)
{
  Point p2 = *this;
  Point a = p1 - p0;
  Point b = p2 - p0;
  double pr = a. x * b.y - b.x * a.y;
  
  if (pr < 0.0)
    return RIGHT;
  if (pr > 0.0)
    return LEFT;
  if ((a.x * b.x < 0.0) || (a.y * b.y < 0.0))
    return BEHIND;
  if (a.length() < b.length())
    return BEFORE;
  if (p0 == p2)
    return BEGIN;
  if (p1 == p2)
    return END;
  return BETWEEN;
}

int Point::locate(Edge &e)
{
	return locate(e.org, e.dest);
}

double Point::distance(Edge &e)
{
  Edge ab = e;
  ab.flip().rotate();                                  
  Point n(ab.dest - ab.org);                       
  n = (1.0 / n.length()) * n;                     
  Edge f(*this, (*this) + n);                                        
  double t;                   
  f.intersect(e, t);          
                              
  return t;
}


