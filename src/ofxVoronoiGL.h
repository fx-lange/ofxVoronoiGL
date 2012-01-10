#include "ofxOpenCv.h"

#pragma once

class SPTAColor : public ofColor{
public:
	SPTAColor() :ofColor() {
		randomColor();
		a = 255;
	}

	SPTAColor(int _r,int _g, int _b){
         r = _r;
         g = _g;
         b = _b;
         a = 255;
     }

	void randomColor(){
     	r = ofRandom(0,255);
     	g = ofRandom(0,255);
     	b = ofRandom(0,255);
     }
};

class VoronoiCell{
public:
    VoronoiCell(int _x, int _y, SPTAColor _color){
        x = _x;
        y = _y;
        color = _color;
    }
    SPTAColor color;
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

	void drawOnScreen(int x=0,int y=0);

	void drawFBOOnScreen(int x,int y){
		fbo.draw(x,y);
	}

	//Attention, possible loss of Performance when used! //TODO single warning when used first!
	unsigned char * getFBOPixels(){
		fbo.readToPixels(pixels);
		return pixels.getPixels();
	}

	void clear();

	void setPoint(int x,int y);
	void setPoint(ofPoint & p);
	void setPolygon(std::vector<ofPoint> & points);

	float alpha;
	bool drawPoly,perFeatureV,skeleton,drawCenters;//TODO check which is still used - rename!

protected:
	ofFbo fbo;
	ofPixels pixels;

	ofCamera cam;

	std::vector<VoronoiCell> points;
	std::vector<std::vector<VoronoiCell> > polygons;

	float width,height;
	float alphaUse, R, error;

	int steps;

	void createVoronoi();

	void drawCone(int x, int y, ofColor& color);
	void drawCone(VoronoiCell p, ofColor& color){
		drawCone(p.x,p.y,color);
	}
	void drawTent(VoronoiCell p1, VoronoiCell p2, ofColor& color);
	void drawPolygon(std::vector<VoronoiCell> & poly);
};
