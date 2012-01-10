
#include "ofTypes.h"

#pragma once

class ofColorSPTA : public ofColor{

public:
	ofColorSPTA() :ofColor() {
		randomColor();
		a = 255;
	}

	ofColorSPTA(int _r,int _g, int _b){
         r = _r;
         g = _g;
         b = _b;
         a = 255;
     }

	ofColorSPTA(int _r,int _g, int _b, int _a){
         r = _r;
         g = _g;
         b = _b;
         a = _a;
     }

     void set(int _r,int _g, int _b){
         r = _r;
         g = _g;
         b = _b;
     }

     void set(int _r,int _g, int _b, int _a){
         r = _r;
         g = _g;
         b = _b;
         a = _a;
     }

     void randomColor(){
     	r = ofRandom(0,255);
     	g = ofRandom(0,255);
     	b = ofRandom(0,255);
     }

     ofColorSPTA operator+( const ofColorSPTA& color ) const {
         return ofColorSPTA( r+color.r, g+color.g, b+color.b );
     }

     ofColorSPTA operator-( const ofColorSPTA& color ) const {
         return ofColorSPTA( r-color.r, g-color.g, b-color.b );
     }
};
