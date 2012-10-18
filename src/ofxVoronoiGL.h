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

	void drawFboOnScreen(int x = 0,int y = 0){ //TODO rename
		fbo.draw(x,y);
	}

	void setPerFeatureVD(bool _perFeature){
		if(perFeatureV == _perFeature)
			return;

		perFeatureV = _perFeature;
		recreateMesh();
	}

	bool isPerFeatureVD(){
		return perFeatureV;
	}

	void setSkeletonMode(bool _skeleton){
		if(skeleton == _skeleton)
			return;

		skeleton = _skeleton;

		recreateMesh();
	}

	bool isSkeletonMode(){
		return skeleton;
	}

	void setConeResolution(int trianglePerCone){
		if(steps == trianglePerCone || trianglePerCone < 3)
			return;

		steps = trianglePerCone;
		alphaUse = 360.f / (float)steps;

		recreateMesh();
	}

	int getConeResolution(){
		return steps;
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


	void setFarClip(float val){
		cam.setFarClip(val);
	}
protected:
	ofCamera cam;
	ofFbo fbo;
	ofPixels pixels;

	ofVboMesh vboMesh;


	std::vector<VoronoiCell> points;
	std::vector<std::vector<VoronoiCell> > polygons;

	float width,height;
	float alphaUse, R, error;
	bool perFeatureV,skeleton,drawCenters;//TODO check which is still used - rename!

	int steps;

	void createVoronoi();

	void createConeMesh(int peakX, int peakY, ofColor &color){

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

	void createConeMesh(VoronoiCell p, ofColor& color){
		createConeMesh(p.x,p.y,color);
	}

	void createTentMesh(VoronoiCell p1, VoronoiCell p2, ofColor &color){
	    //vector between p1 and p2 => p1-p2
	    ofPoint p1p2 = ofPoint(p1.x-p2.x,p1.y-p2.y);
	    ofVec2f norm1 = ofVec2f(-p1p2.y,p1p2.x);
	    ofVec2f norm2 = ofVec2f(p1p2.y,-p1p2.x);
	    norm1.normalize();
	    norm2.normalize();
	    norm1 *= R;
	    norm2 *= R;

	    ofPoint pn1 = norm1;
	    ofPoint pn2 = norm2;

	    ofColor colorToUse = color;
	    if(perFeatureV){
	    	colorToUse.set(255-p1.color.r,255-p1.color.g,255-p1.color.b);
	    }
	    vboMesh.addVertex(ofVec3f(p1.x,p1.y,0));
	    vboMesh.addVertex(ofVec3f(p2.x,p2.y,0));
	    vboMesh.addVertex(ofVec3f(p2.x + pn1.x ,p2.y + pn1.y ,-R));

	    vboMesh.addVertex(ofVec3f(p2.x + pn1.x ,p2.y + pn1.y ,-R));
	    vboMesh.addVertex(ofVec3f(p1.x + pn1.x ,p1.y + pn1.y ,-R));
	    vboMesh.addVertex(ofVec3f(p1.x,p1.y,0));
	    for(int i=0;i<6;++i){
	    	vboMesh.addColor(colorToUse);
	    }

	    if(skeleton){
	    	colorToUse.set(0,0,0);
	    }

		vboMesh.addVertex(ofVec3f(p1.x,p1.y,0));
		vboMesh.addVertex(ofVec3f(p2.x,p2.y,0));
		vboMesh.addVertex(ofVec3f(p2.x + pn2.x ,p2.y + pn2.y ,-R));

		vboMesh.addVertex(ofVec3f(p2.x + pn2.x ,p2.y + pn2.y ,-R));
		vboMesh.addVertex(ofVec3f(p1.x + pn2.x ,p1.y + pn2.y ,-R));
		vboMesh.addVertex(ofVec3f(p1.x,p1.y,0));

		for(int i=0;i<6;++i){
			vboMesh.addColor(colorToUse);
		}

//	    if(drawCenters){
//	        ofSetColor(0,0,0);
//	        ofLine(p1.x,p1.y,p2.x,p2.y);
//	    }
	}

	void createPolygonMesh(std::vector<VoronoiCell> & poly){
	    std::vector<VoronoiCell>::iterator pointIt;
	    std::vector<VoronoiCell>::iterator pointIt2;
	    std::vector<VoronoiCell>::iterator pointIt3;

	    //spezial cases
	    if(poly.size()<3){
	        if(poly.size() == 1){
	            pointIt = poly.begin();
	            VoronoiCell& p = *pointIt;
	            createConeMesh(p,p.color);
	        }else if(poly.size() == 2){
	            pointIt = poly.begin();
	            pointIt2 = poly.begin();
	            VoronoiCell& p1 = *pointIt;
	            VoronoiCell& p2 = *pointIt2;
	            createConeMesh(p1,p1.color);
	            createConeMesh(p2,p1.color);
	            createTentMesh(p1,p2,p1.color);
	        }
	    }

	    pointIt = poly.begin();
	    pointIt2 = poly.begin();
	    pointIt3 = poly.begin();
	    pointIt2++;
	    pointIt3++;
	    pointIt3++;

	    SPTAColor color = (*pointIt).color;
	    bool end = false;
	    while(!end){
	    //for(; pointIt3 != poly.end();pointIt++,pointIt2++,pointIt3++){
	        if(pointIt3 == poly.end()){
	            pointIt3 = poly.begin();
	        }else if(pointIt2 == poly.end()){
	            pointIt2 = poly.begin();
	            end = true;
	        }

	        VoronoiCell& p1 = *pointIt;
	        VoronoiCell& p2 = *pointIt2;
	        VoronoiCell& p3 = *pointIt3;

	        ofVec2f v1 = ofVec2f(p1.x-p2.x,p1.y-p2.y);
	        ofVec2f v2 = ofVec2f(p2.x-p3.x,p2.y-p3.y);
	        SPTAColor colorCone = color;

	        if(skeleton){
	            if(v1.angle(v2)<0){
	                colorCone.set(0,0,0); //TODO use alpha channel?!
	                color = p2.color;
	            }
	        }else if(perFeatureV){
	            colorCone = p2.color;
	        }

	        createConeMesh(p2,colorCone);
	        createTentMesh(p2,p3,color);

	        pointIt++,pointIt2++,pointIt3++;
	    }
	}

	void recreateMesh(){
		vboMesh.clear();

		//draw points
		std::vector<VoronoiCell>::iterator pointIt;
		for(pointIt = points.begin(); pointIt!=points.end();pointIt++){
			VoronoiCell& p = *pointIt;
			createConeMesh(p,p.color);
		}

		//draw polygons
		std::vector<std::vector<VoronoiCell> >::iterator polyIt;
		for(polyIt = polygons.begin();polyIt!=polygons.end();polyIt++){
			std::vector<VoronoiCell>& poly = *polyIt;
			createPolygonMesh(poly);
		}
	}

// not OpenGl ES compatible

//	void drawCone(int x, int y, ofColor& color);
//	void drawCone(VoronoiCell p, ofColor& color){
//		drawCone(p.x,p.y,color);
//	}
//	void drawTent(VoronoiCell p1, VoronoiCell p2, ofColor& color);
//	void drawPolygon(std::vector<VoronoiCell> & poly);
};
