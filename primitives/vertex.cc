#include "vertex.h"


Vertex::Vertex (double x, double y) : Point (x, y) {}

Vertex::Vertex (Point p) : Point (p) {}

Vertex *Vertex::cw()
{
  return (Vertex*)_next;
}

Vertex *Vertex::ccw()
{
  return (Vertex*)_prev;
}

Vertex *Vertex::neighbor (int rotation)
{
  return ((rotation == CW) ? cw() : ccw());
}

Point Vertex::point()
{
  return  *((Point*)this);
}

Vertex *Vertex::insert (Vertex *v)
{
  return (Vertex *) (Node::insert (v));
}

Vertex *Vertex::remove()
{
  return (Vertex *) (Node::remove ());
}

void Vertex::splice (Vertex *b)
{
  Node::splice (b);
}

Vertex *Vertex::split(Vertex *b)
{	                
  Vertex *bp = b->ccw()->insert(new Vertex(b->point()));
  insert(new Vertex(point()));
                       
  splice(bp);
  return bp;
}
