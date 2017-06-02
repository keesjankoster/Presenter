#include "ofMain.h"
#include "ofApp.h"

#include "ofxArgParser.h"

//========================================================================
int main(int argc, const char** argv){
	ofxArgParser::init(argc, argv);

// Hide Console Window under Windows.
#if !defined(TARGET_RASPBERRY_PI)
	//ShowWindow(GetConsoleWindow(), SW_HIDE);
#endif

	ofSetupOpenGL(1024,768,OF_WINDOW);			// <-------- setup the GL context


	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());

}
