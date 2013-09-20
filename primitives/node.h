#pragma once


class Node {
protected:
	Node *_next;	
	Node *_prev;	
public:
	Node() : _next(this) ,_prev(this){}
	virtual ~Node (){}
	Node *next() {return _next;}
	Node *prev() {return _prev;}
	Node *insert(Node*);  
	Node *remove();
	void splice(Node*);
};
