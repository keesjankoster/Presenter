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
	presentations.listDir();
	presentations.sort();
	size = presentations.size();

	activeButton = 0;
	for (i = 0; i < size; i++){
		if (presentations.getFile(i).isDirectory()==1){
			ofxUILabelButton *btn = gui->addLabelButton(presentations.getFile(i).getFileName(), false, 300, 30, 0, 0, true);
			btn->setPadding(5);
			btn->setDrawPaddingOutline(true);
			if(i==0){
				btn->setColorPaddedOutline(ofxUIColor(38, 147, 255, 255));
			}
			buttons.push_back(btn);
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

#if defined(TARGET_RASPBERRY_PI) 
	ofHideCursor();    
#endif
		
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
		logo.draw(50,75);

		// Presenter version
		ofSetColor(ofColor(ofColor::dimGrey));
		ofDrawBitmapString("1.0.1", 960, 740);
	} else if (status == PRESENTER_STATUS_PRESENTATION){
		if(presentation != NULL){
			presentation->draw();
		}
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
			loadPresentation(name);
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

void ofApp::endPresentation(){
	status = PRESENTER_STATUS_WELCOME;
#if !defined(TARGET_RASPBERRY_PI)
	ofShowCursor();
#endif
	gui->setVisible(true);
	shutdown->setVisible(true);
	delete presentation;
	presentation = NULL;
}

void ofApp::loadPresentation(string name){
	// load presentation xml file.
	presentation = new Presentation(name);
			
	// set the status and hide the gui on welcome screen.
	status = PRESENTER_STATUS_PRESENTATION;
#if !defined(TARGET_RASPBERRY_PI)
	ofHideCursor();
#endif
	gui->setVisible(false);
	shutdown->setVisible(false);

	cout << name << endl;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
	cout << "KEY: " << ofToString(key) << endl;

    switch (key) 
    {       
        case 'g':
            gui->toggleVisible(); 
			break;
		case 'x':
			if (status == PRESENTER_STATUS_PRESENTATION){
				presentation->exit();
				endPresentation();
			}
			break;
		case OF_KEY_RIGHT:
			if (status == PRESENTER_STATUS_PRESENTATION){
				if(presentation->next()==0){
					endPresentation();
				}
			}
            break; 
		case OF_KEY_LEFT:
			if (status == PRESENTER_STATUS_PRESENTATION){
				if(presentation->previous()==0){
					endPresentation();
				}
			}
            break; 
		case OF_KEY_UP:
			if (status == PRESENTER_STATUS_WELCOME){
				int prevButton = activeButton;
				if(--activeButton == -1){
					activeButton = buttons.size() - 1;
				}
				
				ofxUILabelButton* btn = buttons[prevButton];
				btn->setColorPaddedOutline(ofxUIColor(255, 255, 255, 255));

				btn = buttons[activeButton];
				btn->setColorPaddedOutline(ofxUIColor(38, 147, 255, 255));
			}
            break; 
		case OF_KEY_DOWN:
			if (status == PRESENTER_STATUS_WELCOME){
				int prevButton = activeButton;
				if(++activeButton == buttons.size()){
					activeButton = 0;
				}
				
				ofxUILabelButton* btn = buttons[prevButton];
				btn->setColorPaddedOutline(ofxUIColor(255, 255, 255, 255));

				btn = buttons[activeButton];
				btn->setColorPaddedOutline(ofxUIColor(38, 147, 255, 255));
			}
            break; 
		case OF_KEY_RETURN:
		case 10: //somehow the RETURN key on RPi is 10 and not OF_KEY_RETURN
			if (status == PRESENTER_STATUS_WELCOME){
				ofxUILabelButton* btn = buttons[activeButton];
				loadPresentation(btn->getName());
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
