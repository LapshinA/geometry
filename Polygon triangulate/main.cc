#include <iostream>
#include <list>
#include <cfloat>

#include "point.h"
#include "node.h"
#include "vertex.h"
#include "polygon.h"
#include "edge.h"

#include <cv.h>
#include <highgui.h>

#define NUM_VERTICES (16)
#define HEIGHT	     (600)
#define WIDTH        (800)
#define DEPTH        (8)
#define NUM_CHANNELS (3)

using namespace std;

void find_convex(Polygon &polygon)
{
	Vertex *ver_a = polygon.neighbor(CCW);
	Vertex *ver_b = polygon.get_vertex();
	Vertex *ver_c = polygon.neighbor(CW);
	
	while (ver_c->locate(*ver_a, *ver_b) != RIGHT) 
	{
		ver_a = ver_b;
		ver_b = polygon.advance(CW);
		ver_c = polygon.neighbor(CW);
	}
}

bool is_in_triangle (Point point, Point pt_a, Point pt_b, Point pt_c)
{
	return ((point.locate (pt_a, pt_b) != LEFT) &&
		(point.locate(pt_b, pt_c) != LEFT) &&
        (point.locate(pt_c, pt_a) != LEFT));
}

Vertex *find_intruding(Polygon &polygon)
{
	Vertex *ver_a = polygon.neighbor(CCW);
	Vertex *ver_b = polygon.get_vertex();
	Vertex *ver_c = polygon.advance(CW);
	Vertex *ver_best = NULL;     
	double best_dist = -1.0;
	Point pt_cc = ver_c->point(), pt_aa = ver_a->point();
	Edge edge_ca(pt_cc, pt_aa);
	Vertex *ver_v = polygon.advance(CW);
	
	while (ver_v != ver_a) 
	{
		if (is_in_triangle(*ver_v, *ver_a, *ver_b, *ver_c)) 
		{
			double dist = ver_v->distance(edge_ca);
			if (dist > best_dist) 
			{
				ver_best = ver_v;
				best_dist = dist;
			}
		}
		ver_v = polygon.advance(CW);
  }
	polygon.set_vertex(ver_b);
	
	return ver_best;
}

list<Polygon*> *triangulate (Polygon  &polygon)
{
	list<Polygon*> *triangles = new list<Polygon*>;
	
	if   (polygon.size() == 3)
	{
		triangles->push_back(&polygon);
	}
	else 
	{
		list<Polygon*>::iterator it;
		
		find_convex(polygon);
		Vertex *ver_d = find_intruding(polygon);
		if (ver_d == NULL) 
		{        
			Vertex  *ver_c = polygon.neighbor(CW);
			polygon.advance(CCW);
			Polygon *pol_q = polygon.split(ver_c);
			list<Polygon*> *listp = triangulate(polygon);
			
			for(it = listp->begin();it != listp->end();it++)
			{
				triangles->push_back(*it);
			}
			triangles->push_back(pol_q);
		} 
		else 
		{                 
			Polygon *pol_q = polygon.split(ver_d);
			
			list<Polygon*> *listq = triangulate(*pol_q);
			
			for(it = listq->begin();it != listq->end();it++)
			{
				triangles->push_back(*it);
			}
				
			list<Polygon*> *listp = triangulate(polygon);	
				
			for(it = listp->begin();it != listp->end();it++)
			{
				triangles->push_back(*it);
			}
		}
  }
  return triangles;
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

int main()
{
	
	Point p1(750,250),p2(600,200),p23(500,190),p3(550,50),p4(400,130),p5(300,90),p6(150,170),p7(290,210),p8(150,290), 
	p9(100,210),p10(80,410),p11(150,500),p12(400,400),p13(500,500),p14(450,350),p15(700,400);
	Point s[] = {p1,p2,p23,p3,p4,p5,p6,p7,p8,p9,p10,p11,p12,p13,p14,p15};
	Polygon p;
	list<Polygon*> *triangles; 
	char num_pts[10], num_trs[10];
	
	for(int i = 0; i < NUM_VERTICES; i++)
	{
		p.insert(s[i]);
	}	
	triangles = triangulate(p);
	sprintf(num_pts,"%i", NUM_VERTICES);
	sprintf(num_trs,"%i", triangles->size());
	
	IplImage* hw = cvCreateImage(cvSize(WIDTH,HEIGHT), DEPTH, NUM_CHANNELS);
    cvSet(hw,cvScalar(0,0,0));
    CvFont font;
    cvInitFont( &font, CV_FONT_HERSHEY_COMPLEX, 0.5, 0.5);
    cvPutText(hw, "Vertices  - ", cvPoint( 500, 570), &font, CV_RGB(255, 0, 0) );
    cvPutText(hw, "Triangles - ", cvPoint(500,590), &font, CV_RGB(255, 0, 0) );
    cvPutText(hw, num_pts, cvPoint(605, 570), &font, CV_RGB(255, 0, 0) );
    cvPutText(hw, num_trs, cvPoint(605, 590), &font, CV_RGB(255, 0, 0) );
	drawTriangulate(hw, triangles);
	cvNamedWindow(" Polygon triangulate", 0);
	cvShowImage("Polygone triangulate", hw);	
	cvWaitKey(0);
	cvReleaseImage(&hw);
	cvDestroyWindow("Polygon triangulate");
    
    return 0;
}
