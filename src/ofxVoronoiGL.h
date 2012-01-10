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

class ofxVoronoiCell{
public:
    ofxVoronoiCell(int _x, int _y, SPTAColor _color){
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

	void drawFBOImg(int x,int y){
		fboImage.draw(x,y);
	}

	unsigned char * getFBOPixels(){//TODO use fbo.readPixels... => refactor update
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
	ofEasyCam cam;

	std::vector<ofxVoronoiCell> points;
	std::vector<std::vector<ofxVoronoiCell> > polygons;

	float width,height;
	float alphaUse, R, error;

	int steps;

	void createVoronoi();

	void drawCone(int x, int y, ofColor& color);
	void drawCone(ofxVoronoiCell p, ofColor& color){
		drawCone(p.x,p.y,color);
	}
	void drawTent(ofxVoronoiCell p1, ofxVoronoiCell p2, ofColor& color);
	void drawPolygon(std::vector<ofxVoronoiCell> & poly);
};
