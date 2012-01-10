#include "ofxVectorMath.h"
#include "ofxOpenCv.h"
#include "ofTypesSPTA.h"

#pragma once


class ofxVoronoiCellFx{
public:
    ofxVoronoiCellFx(int _x, int _y, ofColorSPTA _color){
        x = _x;
        y = _y;
        color = _color;
    }
    ofColorSPTA color;
    int x,y;
};

class ofxVoronoiGL{

public:
	ofxVoronoiGL(){}
	ofxVoronoiGL(float width, float height, float error = 1.0f){
		setup(width,height,error);
	}

	void setup(int _width, int _height, float _error);

	void update();

	void drawDirectOnScreen(int x=0,int y=0);

	void drawFBOOnScreen(int x,int y){
		fbo.draw(x,y);
	}

	void drawFBOImg(int x,int y){
		fboImage.draw(x,y);
	}

	unsigned char * getFBOPixels(){
		return fboImage.getPixels();
	}

	void setPoint(int x,int y);
	void setPoint(ofPoint & p);
	void setPolygon(std::vector<ofPoint> & points);
	void clear(){
        points.clear();
        polygons.clear();
	}

	float alpha;
	bool drawPoly,perFeatureV,skeleton,drawCenters;

protected:
	ofFbo fbo;
	ofImage fboImage;

	std::vector<ofxVoronoiCellFx> points;
	std::vector<std::vector<ofxVoronoiCellFx> > polygons;

	float width,height;
	float alphaUse, R, error;

	int steps;

	void createVoronoi();

	void drawCone(int x, int y, ofColor& color);
	void drawCone(ofxVoronoiCellFx p, ofColor& color){
		drawCone(p.x,p.y,color);
	}
	void drawTent(ofxVoronoiCellFx p1, ofxVoronoiCellFx p2, ofColor& color);
	void drawPolygon(std::vector<ofxVoronoiCellFx> & poly);
};
