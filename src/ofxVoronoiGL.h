#pragma once
#include "ofMain.h"

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

	void drawFboOnScreen(int x,int y){
		fbo.draw(x,y);
	}

	//Attention, possible loss of Performance when used! //TODO single warning when used first!
	unsigned char * getFBOPixels(int attachmentPoint=0){
		fbo.readToPixels(pixels,attachmentPoint);
		return pixels.getPixels();
	}

	ofFbo & getFbo(){
		return fbo;
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

	ofVboMesh vboMesh;

	ofCamera cam;

	std::vector<VoronoiCell> points;
	std::vector<std::vector<VoronoiCell> > polygons;

	float width,height;
	float alphaUse, R, error;

	int steps;

	void createVoronoi();
	void createConeMesh(int peakX, int peakY, ofColor &color){
		steps = (int)(360.f / alpha)+1;
		alphaUse = 360.f / (float)steps;
	    for(int i=0;i<steps;i++){
	    	vboMesh.addVertex(ofVec3f(peakX,peakY,0));
	        float x = R * sin(alphaUse*i*PI/180);
	        float y = R * cos(alphaUse*i*PI/180);
	        vboMesh.addVertex(ofVec3f(peakX+x,peakY+y,-R));
	        x = R * sin(alphaUse*(i+1)*PI/180);
	        y = R * cos(alphaUse*(i+1)*PI/180);
	        vboMesh.addVertex(ofVec3f(peakX+x,peakY+y,-R));
	        for(int j=0;j<3;++j){
	        	vboMesh.addColor(color);
	        }
	    }
	}

	void drawCone(int x, int y, ofColor& color);
	void drawCone(VoronoiCell p, ofColor& color){
		drawCone(p.x,p.y,color);
	}
	void drawTent(VoronoiCell p1, VoronoiCell p2, ofColor& color);
	void drawPolygon(std::vector<VoronoiCell> & poly);
};
