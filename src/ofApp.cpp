#include "ofApp.h"
#include "ofxArgParser.h"

//--------------------------------------------------------------
void ofApp::setup(){
	// Global settings.
	ofSetWindowTitle("Presenter");
	ofSetVerticalSync(true); 
	ofEnableSmoothing();
	ofSetFrameRate(60);

	// Position the window in the centre of the screen.
	ofSetWindowPosition((ofGetScreenWidth() - ofGetWindowWidth()) / 2, (ofGetScreenHeight() - ofGetWindowHeight()) / 2);

	// Set status to Welcome screen.
	status = PRESENTER_STATUS_WELCOME;

	// Load logo for welcome screen.
	logo.load("logo.png");
	if(logo.getWidth()>1000){
		logo.resize(1000, logo.getHeight() / logo.getWidth() * 1000);
	}

	// Setup Menu.
	menu = new Menu();
	ofAddListener(menu->onMenuClick, this, &ofApp::menuClick);

#if defined(TARGET_RASPBERRY_PI) 
	ofHideCursor();
#else
	// Pars Command Line Arguments
	if (ofxArgParser::hasKey("presentation")) {
		// Load Presentation
		menuClick(ofxArgParser::getValue("presentation"));
	}
#endif

	// Setup Key Press detection
	ofAddListener(ofGetWindowPtr()->events().keyPressed, this, &ofApp::keycodePressed);
			
}

void ofApp::menuClick(string &path){
	// load presentation xml file.
	presentation = new Presentation(path);
			
	// set the status and hide the gui on welcome screen.
	status = PRESENTER_STATUS_PRESENTATION;
#if !defined(TARGET_RASPBERRY_PI)
	ofHideCursor();
#endif
	menu->hide();

	// cout << name << endl;
}

//--------------------------------------------------------------
void ofApp::update(){
	if(status == PRESENTER_STATUS_WELCOME){
		
	} else if (status == PRESENTER_STATUS_PRESENTATION){
		if(presentation != NULL){
			presentation->update();
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	if(status == PRESENTER_STATUS_WELCOME){
		// White background
		ofSetColor(ofColor::white);
		ofBackground(ofColor::white);

		// LUKE Theatre Group logo
		logo.draw(100,(ofGetHeight() - logo.getHeight())/2);

		// Presenter version
		ofSetColor(ofColor(ofColor::dimGrey));
		ofDrawBitmapString("2.0.1", ofGetWidth() - 100, ofGetHeight() - 100);
	} else if (status == PRESENTER_STATUS_PRESENTATION){
		if(presentation != NULL){
			presentation->draw();
		}
	}
}

//--------------------------------------------------------------
void ofApp::exit()
{
    delete menu;

}

void ofApp::endPresentation(){
	status = PRESENTER_STATUS_WELCOME;
#if !defined(TARGET_RASPBERRY_PI)
	ofShowCursor();
#endif
	menu->unhide();
	delete presentation;
	presentation = NULL;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
	
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::keycodePressed(ofKeyEventArgs& e) {
	//cout << "KEY: " << ofToString(e.key) << endl;

#if defined(TARGET_RASPBERRY_PI)
	// Check if Ctrl key is pressed
	if (e.hasModifier(OF_KEY_CONTROL)) {
		// Check for Q (ctrl + q)
		if (e.key == 17) {
			// Shutdown the system
			cout << "SHUTDOWN" << endl;
			system("shutdown now");
		}

		// Check for R (ctrl + r)
		if (e.key == 18) {
			// Reboot system
			cout << "REBOOT" << endl;
			system("shutdown -r now");
		}

	}
#endif

	switch (e.key)
	{
	case 'x':
		if (status == PRESENTER_STATUS_PRESENTATION) {
			presentation->exit();
			endPresentation();
		}
		break;
	case OF_KEY_RIGHT:
		if (status == PRESENTER_STATUS_PRESENTATION) {
			if (presentation->next() == 0) {
				endPresentation();
			}
		}
		break;
	case OF_KEY_LEFT:
		if (status == PRESENTER_STATUS_PRESENTATION) {
			if (presentation->previous() == 0) {
				endPresentation();
			}
		}
		break;
	default:
		break;
	}
	
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	switch (button) 
    {       
		case OF_MOUSE_BUTTON_RIGHT:
			if (status == PRESENTER_STATUS_PRESENTATION){
				if(presentation->next()==0){
					endPresentation();
				}
			}
            break; 
		case OF_MOUSE_BUTTON_LEFT:
			if (status == PRESENTER_STATUS_PRESENTATION){
				if(presentation->previous()==0){
					endPresentation();
				}
			}
            break; 
        default:
            break;
    }
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
