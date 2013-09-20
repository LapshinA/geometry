#include <cstdlib>
#include "polygon.h"


Polygon::Polygon() :  _v(NULL) , _size(0) {}

Polygon::Polygon (Polygon &polygon) {
	_size = polygon._size;
	if (_size == 0)
	{
		_v = NULL;
	}
	else 
	{
		_v = new Vertex (polygon.point());
		for (int i = 1; i < _size; i++) 
		{
			polygon.advance(CW);
			_v = _v->insert (new Vertex (polygon.point()));
		}
		polygon.advance(CW);
		_v = _v->cw();
  }
}

Polygon::Polygon (Vertex *v): _v(v)
{
	resize();
}

void Polygon::resize()
{
	if (_v == NULL)
	{
		_size = 0;
	}
	else 
	{
		Vertex *v = _v->cw();
		for (_size = 1; v != _v; ++_size, v = v->cw());
	}
}

Polygon::~Polygon()
{
	if (_v) 
	{
		Vertex *w = _v->cw();
		while (_v != w) 
		{
			delete w->remove();
			w = _v->cw();
		}
		delete _v;
	}
}

Vertex *Polygon::get_vertex()
{
	return _v;
}

int Polygon::size()
{
	return _size;
}

Point Polygon::point()
{
	return _v->point();
}

Vertex *Polygon::cw()
{
	return _v->cw();
}

Vertex *Polygon::ccw()
{
	return _v->ccw();
}

Vertex *Polygon::neighbor(int rotation)
{
	return _v->neighbor(rotation);
}

Vertex *Polygon::advance(int rotation)
{
	return _v = _v->neighbor(rotation);
}

Vertex *Polygon::set_vertex(Vertex *v)
{
	return _v = v;
}

Vertex *Polygon::insert(Point &p)
{
	if (_size++ == 0)
	{
		_v = new Vertex(p);
	}
	else
	{
		_v = _v->insert(new Vertex(p));
	}
	
	return _v;
}

void Polygon::remove()
{
	Vertex *v = _v;
	
	_v = (--_size == 0) ? NULL : _v->ccw();
	delete v->remove();
}

Polygon *Polygon::split(Vertex *b)
{
	Vertex *bp = _v->split(b);
	resize();
	
	return new Polygon(bp);
}


