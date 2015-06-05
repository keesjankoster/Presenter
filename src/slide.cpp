#include "slide.h"

Slide::Slide(void){
	// Setup for Slide Transitions.
	doingTransition = false;
	transitionAlpha = 0.0;
	transitionState1 = 0;
	transitionState2 = 0;
	transitionStartTime = 0;
}


Slide::~Slide(void){
}

void Slide::update(){
	// Load background image.
	if(backgroundImage!="none" && !bgImage.isAllocated()){
		ofFile file(ofToDataPath(backgroundImage));
		//cout << "Image: " << ofToDataPath(backgroundImage) << endl;
		if(file.exists()){
			bgImage.loadImage(file);
			//cout << "Image loaded: " << ofToDataPath(backgroundImage) << endl;
		}
	}
	
	if(backgroundVideo!="none"){
		// Load background video.
		if(!bgVideo.isLoaded()){
			ofFile file(ofToDataPath(backgroundVideo));
			if(file.exists()){
				// Load video on Win32.
				bgVideo.loadMovie(ofToDataPath(backgroundVideo));
				bgVideo.setLoopState(OF_LOOP_NONE);
				bgVideo.play();
			}
		}
		// Update video player;
		bgVideo.update();
	}

}

void Slide::draw(){
	// Draw background color.
	ofBackground(backgroundColor);

	// Draw background image.
	if(bgImage.isAllocated()){
		bgImage.draw((ofGetWindowWidth() - bgImage.width) / 2, (ofGetWindowHeight() - bgImage.height) / 2);
	}

	// Draw background video.
	if(bgVideo.isLoaded()){
		int vHeight = ofGetWindowWidth() / bgVideo.getWidth() * bgVideo.getHeight();
		bgVideo.draw(0, 0, ofGetWindowWidth(), vHeight);
	}
	
	// Do slide transition.
	if(transition != PRESENTER_TRANSITION_NONE){
		ofImage screen;
		screen.grabScreen(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
		doTransition(&screen);
	}

}

void Slide::doTransition(ofImage * current){
	// Do some setup before starting the transition.
	if(transitionStartTime == 0 && previousSlide.isAllocated()){
		// Record the transition start time.
		transitionStartTime = ofGetElapsedTimeMillis();

		doingTransition = true;
	}

	if(doingTransition){
		// Get Elapsed time since last run.
		int elapsedTime = ofGetElapsedTimeMillis() - transitionStartTime;
		transitionStartTime = ofGetElapsedTimeMillis();

		switch(transition){
			case PRESENTER_TRANSITION_FADE:
				cout << "FADE: " << ofToString(elapsedTime) << endl;

				ofEnableAlphaBlending();  
				ofSetColor(255,255,255,255 - transitionAlpha);  
				previousSlide.draw(0.0, 0.0);
				ofSetColor(255,255,255, transitionAlpha);  
				current->draw(0.0, 0.0);
				ofDisableAlphaBlending(); 
								
				if(transitionAlpha < 255){
					transitionAlpha += 255 / 2 * elapsedTime / 1000;
					if(transitionAlpha > 255){
						transitionAlpha = 255;
					}
				} else {
					doingTransition = false;
				}
				break;
			case PRESENTER_TRANSITION_SCROLLING:
				cout << "SCROLLING: " << ofToString(elapsedTime) << endl;

				previousSlide.draw(0.0, 0.0);
				current->drawSubsection(0.0, 0.0, current->getWidth(), transitionState1, 0.0, 0.0);

				if(transitionState1 < current->getHeight()){
					transitionState1 += current->getHeight() / 2 * elapsedTime / 1000;
					if(transitionState1 > current->getHeight()){
						transitionState1 = current->getHeight();
					}
				} else {
					doingTransition = false;
				}
				break;
			case PRESENTER_TRANSITION_FADETOBLACK:
				cout << "FADETOBLACK: " << ofToString(elapsedTime) << endl;

				if(transitionState1 == 0){
					ofEnableAlphaBlending();  
					ofSetColor(255,255,255,255 - transitionAlpha);  
					previousSlide.draw(0.0, 0.0);
					ofDisableAlphaBlending();  
					
					if(transitionAlpha < 255){
						transitionAlpha += 255 / 2 * 2 * elapsedTime / 1000;
						if(transitionAlpha > 255){
							transitionAlpha = 255;
						}
					} else {
						transitionState1 = 1;
					}
				} else if(transitionState1 = 1){
					ofEnableAlphaBlending();
					ofSetColor(255,255,255, 255 - transitionAlpha);  
					current->draw(0.0, 0.0);
					ofDisableAlphaBlending();  
					
					if(transitionAlpha > 0){
						transitionAlpha -= 255 / 2 * 2 * elapsedTime / 1000;
						if(transitionAlpha <= 0){
							transitionAlpha = 0;
						}
					} else {
						doingTransition = false;
					}
				}
				break;
		}
	} else {
		current->draw(0., 0.);
	}

}


/*
void Slide::doTransition(ofImage * current){
	// Do some setup before starting the transition.
	if(transitionStartTime == 0 && previousSlide.isAllocated()){
		// Load the pixel data from the previous Slide.
		previousData = previousSlide.getPixels();

		// Record the transition start time.
		transitionStartTime = ofGetElapsedTimeMillis();

		doingTransition = true;
	}
	
	if(doingTransition){
		// Get pixel data from current slide.
		unsigned char * currentData = current->getPixels();

		// Get Elapsed time since last run.
		int elapsedTime = ofGetElapsedTimeMillis() - transitionStartTime;
		transitionStartTime = ofGetElapsedTimeMillis();

		switch(transition){
			case PRESENTER_TRANSITION_FADE:
				cout << "FADE: " << ofToString(elapsedTime) << endl;
				for(int i = 0; i < transitionPixels; i++){
					transitionData[i] = currentData[i] * transitionAlpha + previousData[i] * (1.0 - transitionAlpha);
				}
				if(transitionAlpha < 1.0){
					transitionAlpha += 0.99 / 2 * elapsedTime / 1000;
					if(transitionAlpha > 1.0){
						transitionAlpha = 1.0;
					}
				} else {
					doingTransition = false;
				}
				break;
			case PRESENTER_TRANSITION_SCROLLING:
				cout << ofToString(transitionState1) << " (" << ofToString(transitionPixels) << ")" << endl;
				for(int i = 0; i < transitionState1; i++){
					transitionData[i] = currentData[i];
				}
				for(int i = transitionState1; i < transitionPixels; i++){
					transitionData[i] = previousData[i];
				}
				if(transitionState1 < transitionPixels){
					transitionState1 += transitionPixels / 2 * elapsedTime / 1000;
					if(transitionState1 > transitionPixels){
						transitionState1 = transitionPixels;
					}
				} else {
					doingTransition = false;
				}
				break;
			case PRESENTER_TRANSITION_FADETOBLACK:
				if(transitionState1 == 0){
					for(int i = 0; i < transitionPixels; i++){
						transitionData[i] = previousData[i] * (1.0 - transitionAlpha);
					}
					if(transitionAlpha < 1.0){
						transitionAlpha += 0.99 / 2 * 2 * elapsedTime / 1000;
						if(transitionAlpha > 1.0){
							transitionAlpha = 1.0;
						}
					} else {
						transitionState1 = 1;
					}
				} else if(transitionState1 = 1){
					for(int i = 0; i < transitionPixels; i++){
						transitionData[i] = currentData[i] * (1.0 - transitionAlpha);
					}
					if(transitionAlpha > 0.0){
						transitionAlpha -= 0.99 / 2 * 2 * elapsedTime / 1000;
						if(transitionAlpha <= 0.0){
							transitionAlpha = 0.0;
						}
					} else {
						doingTransition = false;
					}
				}
		}

		transitionTexture.loadData(transitionData, ofGetWindowWidth(), ofGetWindowHeight(), GL_RGB);
		transitionTexture.draw(0.0, 0.0);
	} else {
		current->draw(0., 0.);
	}
}
*/

void Slide::next(){
	
}

void Slide::previous(){

}



