#include "ofxVoronoiGL.h"

void ofxVoronoiGL::setup(int _width, int _height, float _error){
	width = _width;
	height = _height;
	error = _error;

	fbo.allocate(width,height,GL_RGB,1);
	fboImage.allocate(width,height,OF_IMAGE_COLOR);

	R = sqrt(width*width + height*height);
	//FXTODO musst be calculate by 2cos⁽⁻¹)(R-E / R)
	alpha = 3;
	steps = (int)(360.f / alpha)+1;
	alphaUse = 360.f / (float)steps;
}


void ofxVoronoiGL::update(){
//	fbo.clear();//TODO hier stand ma clear
	fbo.begin();
	createVoronoi();
	fbo.end();

	//preDraw to be able to use opencv methods
	ofSetColor(255,255,255);
	fbo.draw(0,0);
	fboImage.grabScreen(0,0,width,height);
	ofFill();
	ofRect(0,0,width,height);
}
void ofxVoronoiGL::setPoint(int x,int y){
	 points.push_back(ofxVoronoiCellFx(x,y,ofColorSPTA()));
}
void ofxVoronoiGL::setPoint(ofPoint & p){
	setPoint(p.x,p.y);
}
void ofxVoronoiGL::setPolygon(std::vector<ofPoint> & points){
	std::vector<ofxVoronoiCellFx> cells;
	std::vector<ofPoint>::iterator pointIt;
	for(pointIt = points.begin(); pointIt != points.end();pointIt++){
		ofPoint & p = *pointIt;
		cells.push_back(ofxVoronoiCellFx(p.x,p.y,ofColorSPTA()));
	}
	polygons.push_back(cells);
}

void ofxVoronoiGL::drawDirectOnScreen(int x,int y){
	ofPushMatrix();
	ofTranslate(x,y);
	createVoronoi();
	ofPopMatrix();
}

void ofxVoronoiGL::createVoronoi(){
		glEnable(GL_DEPTH_TEST);
		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

		//draw points
		std::vector<ofxVoronoiCellFx>::iterator pointIt;
		for(pointIt = points.begin(); pointIt!=points.end();pointIt++){
			ofxVoronoiCellFx& p = *pointIt;
			drawCone(p,p.color);
		}

		//draw polygons
		std::vector<std::vector<ofxVoronoiCellFx> >::iterator polyIt;
		for(polyIt = polygons.begin();polyIt!=polygons.end();polyIt++){
			std::vector<ofxVoronoiCellFx>& poly = *polyIt;
			drawPolygon(poly);
		}

		glDisable(GL_DEPTH_TEST); //FXTODO instead of enable and disable -> popStyle
	}

void ofxVoronoiGL::drawCone(int peakX, int peakY, ofColor &color){
    ofSetColor(color.r,color.g,color.b);

    glPushMatrix();
    glTranslatef(peakX, peakY, 0);

    glBegin(GL_TRIANGLES);
    for(int i=0;i<steps;i++){
        glVertex3f(0,0,0);
        float x = R * sin(alphaUse*i*PI/180);
        float y = R * cos(alphaUse*i*PI/180);
        glVertex3f(x,y,-R);
        x = R * sin(alphaUse*(i+1)*PI/180);
        y = R * cos(alphaUse*(i+1)*PI/180);
        glVertex3f(x,y,-R);
    }
    glEnd();

    if(drawCenters){
        ofFill();
        ofSetColor(0,0,0);
        ofEllipse(0,0,5,5);
    }
    glPopMatrix();

}

void ofxVoronoiGL::drawPolygon(std::vector<ofxVoronoiCellFx> & poly){
    std::vector<ofxVoronoiCellFx>::iterator pointIt;
    std::vector<ofxVoronoiCellFx>::iterator pointIt2;
    std::vector<ofxVoronoiCellFx>::iterator pointIt3;

    //Sonderfälle
    if(poly.size()<3){
        if(poly.size() == 1){
            pointIt = poly.begin();
            ofxVoronoiCellFx& p = *pointIt;
            drawCone(p,p.color);
        }else if(poly.size() == 2){
            pointIt = poly.begin();
            pointIt2 = poly.begin();
            ofxVoronoiCellFx& p1 = *pointIt;
            ofxVoronoiCellFx& p2 = *pointIt2;
            drawCone(p1,p1.color);
            drawCone(p2,p1.color);
            drawTent(p1,p2,p1.color);
        }
    }

    pointIt = poly.begin();
    pointIt2 = poly.begin();
    pointIt3 = poly.begin();
    pointIt2++;
    pointIt3++;
    pointIt3++;

    ofColorSPTA color = (*pointIt).color;
    bool end = false;
    while(!end){
    //for(; pointIt3 != poly.end();pointIt++,pointIt2++,pointIt3++){
        if(pointIt3 == poly.end()){
            pointIt3 = poly.begin();
        }else if(pointIt2 == poly.end()){
            pointIt2 = poly.begin();
            end = true;
        }

        ofxVoronoiCellFx& p1 = *pointIt;
        ofxVoronoiCellFx& p2 = *pointIt2;
        ofxVoronoiCellFx& p3 = *pointIt3;

        ofxVec2f v1 = ofxVec2f(p1.x-p2.x,p1.y-p2.y);
        ofxVec2f v2 = ofxVec2f(p2.x-p3.x,p2.y-p3.y);
        ofColorSPTA colorCone = color;

        if(skeleton){
            if(v1.angle(v2)<0){
                colorCone.set(0,0,0);
                color = p2.color;
            }
        }else if(perFeatureV){
            colorCone = p2.color;
        }

        drawCone(p2,colorCone);
        drawTent(p2,p3,color);

        pointIt++,pointIt2++,pointIt3++;
    }
}

void ofxVoronoiGL::drawTent(ofxVoronoiCellFx p1, ofxVoronoiCellFx p2, ofColor &color){
    //vector between p1 and p2 => p1-p2
    ofPoint p1p2 = ofPoint(p1.x-p2.x,p1.y-p2.y);
    ofxVec2f norm1 = ofxVec2f(-p1p2.y,p1p2.x);
    ofxVec2f norm2 = ofxVec2f(p1p2.y,-p1p2.x);
    norm1.normalize();
    norm2.normalize();
    norm1 *= R;
    norm2 *= R;

    ofPoint pn1 = norm1;
    ofPoint pn2 = norm2;

    if(perFeatureV){
        ofSetColor(255-p1.color.r,255-p1.color.g,255-p1.color.b);
    }else{
        ofSetColor(color.r,color.g,color.b);
    }
    glBegin(GL_QUADS);
    glVertex3f(p1.x,p1.y,0);
    glVertex3f(p2.x,p2.y,0);
    glVertex3f(p2.x + pn1.x ,p2.y + pn1.y ,-R);
    glVertex3f(p1.x + pn1.x ,p1.y + pn1.y ,-R);
    glEnd();


    if(skeleton){
        ofSetColor(0,0,0);
    }

    glBegin(GL_QUADS);
       glVertex3f(p1.x,p1.y,0);
       glVertex3f(p2.x,p2.y,0);
       glVertex3f(p2.x + pn2.x ,p2.y + pn2.y ,-R);
       glVertex3f(p1.x + pn2.x ,p1.y + pn2.y ,-R);
   glEnd();

    if(drawCenters){
        ofSetColor(0,0,0);
        ofLine(p1.x,p1.y,p2.x,p2.y);
    }
}
