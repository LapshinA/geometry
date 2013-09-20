#include <iostream>
#include <list>
#include <set>
#include <vector>
#include <algorithm>
#include <cfloat>
#include <stdlib.h>

#include <cv.h>
#include <highgui.h>

#include "point.h"
#include "polygon.h"
#include "edge.h"

#define HEIGHT	             (600)
#define WIDTH                (800)
#define DEPTH                (8)
#define NUM_CHANNELS         (3)

using namespace std;


void border_update(set <Edge*> &border, Point &p1, Point &p2)
{
	Edge *edge = new Edge(p1, p2);
	bool bfind = false;
	set<Edge*>::iterator it;

	for(it = border.begin(); it != border.end(); it++)
	{
		if(**it == *edge)
		{
			bfind = true;
			break;
		}	
	}
   
	if(bfind)
	{
		border.erase(*it);
	}
	else 
	{
		edge->flip();
		border.insert(edge);
	}
}

Edge *first_edge(vector<Point> &pts)
{
	int i = 0, m = 0;
  
	for (i = 1; i < pts.size(); i++)
	{
		if (pts[i] < pts[m])
		{
			m = i;
		}
    }
    
	swap(pts[0], pts[m]);

	for (m = 1, i = 2; i < pts.size(); i++) 
	{
		int loc =  pts[i].locate(pts[0], pts[m]);
		
		if ( (loc  ==  LEFT) || (loc  == BETWEEN) )
		{
			m = i;
		}
	}
	
	return new Edge(pts[0], pts[m]);
}


Polygon  *make_triangle(Point &p1, Point &p2, Point &p3)
{
	Polygon *triangle = new Polygon;
	triangle->insert(p1);
	triangle->insert(p2);
	triangle->insert(p3);
	
	return triangle;
}

bool find_point(Edge &edge, vector<Point> &pts, Point &point)
{
	Point *b_point = NULL;
	double t, b_t = FLT_MAX;
	Edge norm = edge;
	
	norm.rotate();
	for (int i = 0; i < pts.size(); i++)
	{
		if (pts[i].locate(edge) == RIGHT) 
		{
			Edge g(edge.dest, pts[i]);
			g.rotate();
			norm.intersect (g, t);
			if (t < b_t) 
			{
				b_point = &pts[i];
				b_t = t;
			}
		}
	}
	
	if (b_point) 
	{
		point = *b_point;
		return true;
	}
	
	return false;
}

Edge *edge_min_remove(set<Edge*> &border)
{
	set<Edge*>::iterator it = border.begin();
	Edge* min = *it;
	
	for(;it != border.end(); it++)
	{
		if(**it < *min)
		{
			min = *it;
		}
	}
	border.erase(min);
	return min;
}

list <Polygon*> *triangulate(vector<Point> &pts, int n)
{
	Point point;
	list<Polygon*> *triangles = new list<Polygon*>; 
	set <Edge*> border;
	Edge *edge = first_edge(pts);
  
	border.insert(edge); 
	while (!border.empty()) 
	{
		edge = edge_min_remove(border);
		if (find_point(*edge, pts, point)) 
		{
			border_update(border, point, edge->org);
			border_update(border, edge->dest, point);
			triangles->push_back(make_triangle(edge->org, edge->dest, point));
		}
		delete edge;
	}
	
	return triangles;
}

void generate_points(vector<Point> &pts, int num)
{
	for (int i = 0; i < num; i++)
	{
		pts.push_back(Point(50 + rand() % 700, 50 + rand() % 500));
	}
}


void drawTriangulate(cv::Mat img, list<Polygon*> *triangles)
{
	list<Polygon*>::iterator it;
	
	for(it = triangles->begin();it != triangles->end();it++)
	{	
		line(img, cv::Point((*it)->point().x,(*it)->point().y), cv::Point((*it)->cw()->point().x,(*it)->cw()->point().y), 
		cv::Scalar( 255, 255, 255 ),1,CV_AA);
		
		line(img, cv::Point((*it)->cw()->point().x,(*it)->cw()->point().y), cv::Point((*it)->ccw()->point().x,(*it)->ccw()->point().y), 
		cv::Scalar( 255, 255, 255 ),1,CV_AA);
		
		line(img, cv::Point((*it)->ccw()->point().x,(*it)->ccw()->point().y), cv::Point((*it)->point().x,(*it)->point().y), 
		cv::Scalar( 255, 255, 255 ),1,CV_AA);
	}	
}


int main( int argc, char** argv )
{
	vector<Point> pts;
	list<Polygon*> *triangles;
    char num_trs[10];
    
	if(argc > 1 && atoi(argv[1]) >= 3)
    {
		generate_points(pts,atoi(argv[1]));
	}
	else
	{
		printf("Expected argument(number of points >= 3)\n");
		exit(1);
	}
	
	triangles = triangulate(pts, pts.size());
	sprintf(num_trs,"%i", triangles->size());
	
    IplImage* hw = cvCreateImage(cvSize(WIDTH,HEIGHT), DEPTH, NUM_CHANNELS); //define
    cvSet(hw,cvScalar(0,0,0));
    CvFont font;
    cvInitFont( &font, CV_FONT_HERSHEY_COMPLEX, 0.5, 0.5);
    cvPutText(hw, "Points - ", cvPoint( 500, 570), &font, CV_RGB(255, 0, 0) );
    cvPutText(hw, "Triangles - ", cvPoint(500,590), &font, CV_RGB(255, 0, 0) );
    cvPutText(hw, argv[1], cvPoint(580, 570), &font, CV_RGB(255, 0, 0) );
    cvPutText(hw, num_trs, cvPoint(605, 590), &font, CV_RGB(255, 0, 0) );
	drawTriangulate(hw, triangles);
	cvNamedWindow("Triangulate", 0);
	cvShowImage("Triangulate", hw);	
	cvWaitKey(0);
	cvReleaseImage(&hw);
	cvDestroyWindow("Triangulate");
    
    return 0;
}
