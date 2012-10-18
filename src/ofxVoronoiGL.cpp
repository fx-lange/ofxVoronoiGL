#include "ofxVoronoiGL.h"

void ofxVoronoiGL::setup(int _width, int _height, float _error){
	width = _width;
	height = _height;
	error = _error;

	ofFbo::Settings fboSettings;
	fboSettings.width = width;
	fboSettings.height = height;
	fboSettings.internalformat = GL_RGB;
//	fboSettings.numSamples = 0;
	fboSettings.useDepth = true;
	fboSettings.useStencil = true;
	fboSettings.depthStencilAsTexture = true;
//	fboSettings.numColorbuffers = 2; //an extra texture for the depthbuffer
	fbo.allocate(fboSettings);

	R = sqrt(width*width + height*height);
	//TODO musst be calculated by 360 / (2cos⁽⁻¹⁾(R-E / R))
	steps = 60;
	alphaUse = 360.f / (float)steps;
	skeleton = perFeatureV = false;

	cam.enableOrtho();
	cam.move(0,0,10);
	cam.setFarClip(300);//TODO should be set depended to width and height

	vboMesh.setMode(OF_PRIMITIVE_TRIANGLES);
}

void ofxVoronoiGL::setPoint(int x,int y){
	SPTAColor sc = SPTAColor();
	 points.push_back(VoronoiCell(x,y,sc));
	 createConeMesh( x,  y, sc);
}

void ofxVoronoiGL::setPoint(ofPoint & p){
	setPoint(p.x,p.y);
}

void ofxVoronoiGL::setPolygon(std::vector<ofPoint> & points){
	std::vector<VoronoiCell> cells;
	std::vector<ofPoint>::iterator pointIt;
	for(pointIt = points.begin(); pointIt != points.end();pointIt++){
		ofPoint & p = *pointIt;
		cells.push_back(VoronoiCell(p.x,p.y,SPTAColor()));
	}
	createPolygonMesh(cells);
	polygons.push_back(cells);
}

void ofxVoronoiGL::update(){
	fbo.begin();
	cam.begin();
	ofClear(0);
	createVoronoi();
	cam.end();
	fbo.end();
}

void ofxVoronoiGL::clear(){
        points.clear();
        polygons.clear();
        fbo.begin();
        ofClear(0);
        fbo.end();
        vboMesh.clear();
	}


void ofxVoronoiGL::createVoronoi(){
		glEnable(GL_DEPTH_TEST);
		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL );

		vboMesh.draw();

		glDisable(GL_DEPTH_TEST); //REVISIT instead of enable and disable -> popStyle
	}

// not OpenGl ES compatible

//void ofxVoronoiGL::drawCone(int peakX, int peakY, ofColor &color){
//    ofSetColor(color.r,color.g,color.b);
//
//    glPushMatrix();
//    glTranslatef(peakX, peakY,0);
//
//    glBegin(GL_TRIANGLES);
//    for(int i=0;i<steps;i++){
//        glVertex3f(0,0,0);
//        float x = R * sin(alphaUse*i*PI/180);
//        float y = R * cos(alphaUse*i*PI/180);
//        glVertex3f(x,y,-R);
//        x = R * sin(alphaUse*(i+1)*PI/180);
//        y = R * cos(alphaUse*(i+1)*PI/180);
//        glVertex3f(x,y,-R);
//    }
//    glEnd();
//
//    if(drawCenters){
//        ofFill();
//        ofSetColor(0,0,0);
//        ofEllipse(0,0,5,5);
//    }
//    glPopMatrix();
//
//}
//
//void ofxVoronoiGL::drawPolygon(std::vector<VoronoiCell> & poly){
//    std::vector<VoronoiCell>::iterator pointIt;
//    std::vector<VoronoiCell>::iterator pointIt2;
//    std::vector<VoronoiCell>::iterator pointIt3;
//
//    //spezial cases
//    if(poly.size()<3){
//        if(poly.size() == 1){
//            pointIt = poly.begin();
//            VoronoiCell& p = *pointIt;
//            drawCone(p,p.color);
//        }else if(poly.size() == 2){
//            pointIt = poly.begin();
//            pointIt2 = poly.begin();
//            VoronoiCell& p1 = *pointIt;
//            VoronoiCell& p2 = *pointIt2;
//            drawCone(p1,p1.color);
//            drawCone(p2,p1.color);
//            drawTent(p1,p2,p1.color);
//        }
//    }
//
//    pointIt = poly.begin();
//    pointIt2 = poly.begin();
//    pointIt3 = poly.begin();
//    pointIt2++;
//    pointIt3++;
//    pointIt3++;
//
//    SPTAColor color = (*pointIt).color;
//    bool end = false;
//    while(!end){
//    //for(; pointIt3 != poly.end();pointIt++,pointIt2++,pointIt3++){
//        if(pointIt3 == poly.end()){
//            pointIt3 = poly.begin();
//        }else if(pointIt2 == poly.end()){
//            pointIt2 = poly.begin();
//            end = true;
//        }
//
//        VoronoiCell& p1 = *pointIt;
//        VoronoiCell& p2 = *pointIt2;
//        VoronoiCell& p3 = *pointIt3;
//
//        ofVec2f v1 = ofVec2f(p1.x-p2.x,p1.y-p2.y);
//        ofVec2f v2 = ofVec2f(p2.x-p3.x,p2.y-p3.y);
//        SPTAColor colorCone = color;
//
//        if(skeleton){
//            if(v1.angle(v2)<0){
//                colorCone.set(0,0,0); //TODO use alpha channel?!
//                color = p2.color;
//            }
//        }else if(perFeatureV){
//            colorCone = p2.color;
//        }
//
//        drawCone(p2,colorCone);
//        drawTent(p2,p3,color);
//
//        pointIt++,pointIt2++,pointIt3++;
//    }
//}
//
//void ofxVoronoiGL::drawTent(VoronoiCell p1, VoronoiCell p2, ofColor &color){
//    //vector between p1 and p2 => p1-p2
//    ofPoint p1p2 = ofPoint(p1.x-p2.x,p1.y-p2.y);
//    ofVec2f norm1 = ofVec2f(-p1p2.y,p1p2.x);
//    ofVec2f norm2 = ofVec2f(p1p2.y,-p1p2.x);
//    norm1.normalize();
//    norm2.normalize();
//    norm1 *= R;
//    norm2 *= R;
//
//    ofPoint pn1 = norm1;
//    ofPoint pn2 = norm2;
//
//    if(perFeatureV){
//        ofSetColor(255-p1.color.r,255-p1.color.g,255-p1.color.b);
//    }else{
//        ofSetColor(color.r,color.g,color.b);
//    }
//    glBegin(GL_TRIANGLES);
//    glVertex3f(p1.x,p1.y,0);
//    glVertex3f(p2.x,p2.y,0);
//    glVertex3f(p2.x + pn1.x ,p2.y + pn1.y ,-R);
//
//    glVertex3f(p2.x + pn1.x ,p2.y + pn1.y ,-R);
//    glVertex3f(p1.x + pn1.x ,p1.y + pn1.y ,-R);
//    glVertex3f(p1.x,p1.y,0);
//    glEnd();
//
//
//    if(skeleton){
//        ofSetColor(0,0,0);
//    }
//
//    glBegin(GL_QUADS);
//       glVertex3f(p1.x,p1.y,0);
//       glVertex3f(p2.x,p2.y,0);
//       glVertex3f(p2.x + pn2.x ,p2.y + pn2.y ,-R);
//       glVertex3f(p1.x + pn2.x ,p1.y + pn2.y ,-R);
//   glEnd();
//
//    if(drawCenters){
//        ofSetColor(0,0,0);
//        ofLine(p1.x,p1.y,p2.x,p2.y);
//    }
//}
