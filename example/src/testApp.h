#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxVoronoiGL.h"

class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);

        float width,height;

        //Navigation
        float rotateX, rotateY;
        int pmouseX, pmouseY;

        //Drawing
        bool first;
        bool drawEnabled,drawPoly;

        int red;

        ofxVoronoiGL voronoi;
        vector<ofPoint> pointsTemp;

        string msg;
};
#endif