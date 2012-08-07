#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    rotateX = 0, rotateY = 0;
    pmouseX =-1, pmouseY  =-1;
    width = 500;
    height = 500;
    drawEnabled = first = true;


    voronoi.setup(width,height,1.f);
    coneResolution = voronoi.getConeResolution();

    red = 0;
}

//--------------------------------------------------------------
void testApp::update(){
    voronoi.update();

    msg = "SPACE -> enable drawing\np -> set voronoi center\nl -> set polygon vertices\ne -> finish polygon\nf -> toggle perFeatureMode\nc -> clear\n";
   msg += "\n+/- -> triangles Per Cone " + ofToString(voronoi.getConeResolution());
   msg += "\nto understand the idea behind \nthe algorithm just drag the \nmouse and press SPACE \nto enable drawing again";

}

//--------------------------------------------------------------
void testApp::draw(){
	ofBackground(0);
    ofPushMatrix();

    ofTranslate(ofGetWidth()/4.0,ofGetHeight()/2.0);
    ofRotateX(rotateY);
    ofRotateY(rotateX);
    ofTranslate(-width/2.0,-height/2.0);

    voronoi.createVoronoi();

    //draw rect
	ofNoFill();
	ofSetColor(255,255,255);
	ofRect(0,0,width,height);

    ofPopMatrix();

    ofSetColor(255,255,255);

    voronoi.drawFboOnScreen(ofGetWidth()-width,0);

    ofSetColor(255,255,255);
    ofDrawBitmapString(msg,ofGetWidth()-width/2,height+50);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    if(key==' '){
        rotateX=0;
        rotateY=0;
        drawEnabled = true;
    }else if(key=='e'){
        drawPoly = false;
        if(pointsTemp.empty()){
        	return;
        }
        voronoi.setPolygon(pointsTemp);
    }else if(key=='c'){
        voronoi.clear();
    }else if(key=='+'){
    	coneResolution +=3;
        voronoi.setConeResolution(coneResolution);
    }else if(key=='-'){
    	coneResolution -=3;
        voronoi.setConeResolution(coneResolution);
    }else if(key=='f'){
    	voronoi.setPerFeatureVD(!voronoi.isPerFeatureVD());
    }
    if(drawEnabled){
        if(key=='p'){
            voronoi.setPoint(mouseX-ofGetWidth()/4.0+width/2.0,mouseY-ofGetHeight()/2.0+height/2.0);
        }else if(key=='l'){
            if(!drawPoly){
                drawPoly = true;
                pointsTemp.clear();
            }
            pointsTemp.push_back(ofPoint(mouseX-ofGetWidth()/4.0+width/2.0,mouseY-ofGetHeight()/2.0+height/2.0));
        }
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    if(first){
        pmouseX = x;
        pmouseY = y;
        first = false;
    }
    rotateX += (float)(x - pmouseX)*0.5;
    rotateY += (float)(pmouseY - y)*0.5;
    pmouseX = x;
    pmouseY = y;
    drawEnabled = false;
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    first = true;
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
}

