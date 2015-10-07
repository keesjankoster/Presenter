#pragma once

#include "ofMain.h"
#include "menu.h"
#include "presentation.h"

enum PresenterStatus {
	PRESENTER_STATUS_WELCOME, 
	PRESENTER_STATUS_PRESENTATION
};

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void exit();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		PresenterStatus status;
		ofImage logo;

	protected:
		void endPresentation();
		void menuClick(string & path);

		Presentation *presentation;

		Menu *menu;

		
};
