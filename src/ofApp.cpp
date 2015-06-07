#include "ofApp.h"

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
	logo.loadImage("logo.png");
	if(logo.getWidth()>550){
		logo.resize(550, logo.getHeight() / logo.getWidth() * 550);
	}

	// Create presentation chooser buttons.
	gui = new ofxUICanvas();
	gui->setPosition(650, 250);
	gui->setDrawBack(false);
	
	// Setup custom theme for GUI.
	ofxUIColor cb = ofxUIColor(38, 147, 255, 200);
    ofxUIColor co = ofxUIColor(38, 147, 255, 255);
    ofxUIColor coh = OFX_UI_COLOR_OUTLINE_HIGHLIGHT;
    ofxUIColor cf = ofxUIColor(255, 255, 255, 255);
    ofxUIColor cfh = OFX_UI_COLOR_FILL_HIGHLIGHT;
    ofxUIColor cp = ofxUIColor(38, 147, 255, 255);
    ofxUIColor cpo = OFX_UI_COLOR_PADDED_OUTLINE;
    gui->setUIColors( cb, co, coh, cf, cfh, cp, cpo );

	gui->addLabel("Select a presentation:", OFX_UI_FONT_MEDIUM)->setColorFill(ofxUIColor(0, 0, 0, 255));
    gui->addSpacer();

	// Each directory in bin/data/Presentations is concidered a presentation.
	int i, size;
	ofDirectory presentations;
	presentations = ofDirectory("Presentations");
	presentations.sort();
	presentations.listDir();
	size = presentations.size();

	for (i = 0; i < size; i++){
		if (presentations.getFile(i).isDirectory()==1){
			gui->addLabelButton(presentations.getFile(i).getFileName(), false, 300, 30, 0, 0, true);
		}
	}
    
    gui->autoSizeToFitWidgets();
    ofAddListener(gui->newGUIEvent,this,&ofApp::guiEvent);

	// Create shutdown button.
	shutdown = new ofxUICanvas();
	shutdown->setPosition(ofGetWindowWidth()-60, 10);
	shutdown->setDrawBack(false);

	shutdown->setGlobalButtonDimension(44);
	shutdown->addImageButton("Shutdown", "shutdown.png", false);
	shutdown->autoSizeToFitWidgets();
    ofAddListener(shutdown->newGUIEvent,this,&ofApp::guiEvent);
		
}

//--------------------------------------------------------------
void ofApp::update(){
	if(status == PRESENTER_STATUS_WELCOME){
		
	} else if (status == PRESENTER_STATUS_PRESENTATION){
		presentation->update();
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	if(status == PRESENTER_STATUS_WELCOME){
		ofBackground(ofColor::white);
		logo.draw(50,75);
	} else if (status == PRESENTER_STATUS_PRESENTATION){
		presentation->draw();
	}
}

void ofApp::guiEvent(ofxUIEventArgs &e)
{
	string name = e.widget->getName(); 
	int kind = e.widget->getKind(); 
    
    if(kind == OFX_UI_WIDGET_LABELBUTTON)
    {
        ofxUIButton *button = (ofxUIButton *) e.widget;
		if(!button->getValue()){
			// load presentation xml file.
			presentation = new Presentation(name);
			
			// set the status and hide the gui on welcome screen.
			status = PRESENTER_STATUS_PRESENTATION;
			gui->setVisible(false);
			shutdown->setVisible(false);

			cout << name << "\t value: " << button->getValue() << endl;
		}
    }
    else if(kind == OFX_UI_WIDGET_IMAGEBUTTON)
    {
        ofxUIImageButton *button = (ofxUIImageButton *) e.widget; 
        if(button->getValue()){

#if defined(TARGET_RASPBERRY_PI)
			// On Raspberry Pi, shutdown.
			std::system("sudo shutdown -h now");
#else
			// Desktop applications, close the application.
			std::exit(0);         
#endif

		}              
    }
}

//--------------------------------------------------------------
void ofApp::exit()
{
    delete gui; 
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
    switch (key) 
    {       
        case 'g':
            gui->toggleVisible(); 
			break;
		case OF_KEY_RIGHT:
			if (status == PRESENTER_STATUS_PRESENTATION){
				if(presentation->next()==0){
					status = PRESENTER_STATUS_WELCOME;
					gui->setVisible(true);
					shutdown->setVisible(true);
					delete presentation;
				}
			}
            break; 
		case OF_KEY_LEFT:
			if (status == PRESENTER_STATUS_PRESENTATION){
				if(presentation->previous()==0){
					status = PRESENTER_STATUS_WELCOME;
					gui->setVisible(true);
					shutdown->setVisible(true);
					delete presentation;
				}
			}
            break; 
        default:
            break;
    }
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
void ofApp::mousePressed(int x, int y, int button){

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
