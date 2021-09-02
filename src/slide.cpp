#include "slide.h"

Slide::Slide(void){
	// Setup for Slide Transitions.
	setTransitionDefaults();

	// Set Current Item
	currentItem = -1;

	// Set Current Video Pause.
	currentPause = 0;
	doingPauses = true;
}

Slide::~Slide(void){
	// Clear all items
	items.clear();

	/* ============================================
	// test the next part!!

	// Clear video if necessary
	if(backgroundVideo!="none"){
#if defined(TARGET_RASPBERRY_PI)
		if(bgVideo.isPlaying()){
			bgVideo.close();
		}
#else
		if(bgVideo.isLoaded()){
			bgVideo.closeMovie();
		}
#endif
	}

	// ===========================================*/
}

void Slide::update(){
	// Load background image.
	if(backgroundImage!="none" && !bgImage.isAllocated()){
		ofFile file(ofToDataPath(backgroundImage));
		// cout << "Image: " << ofToDataPath(backgroundImage) << endl;
		if(file.exists()){
			bgImage.loadImage(file);
			//ofLoadImage(bgImage, ofToDataPath(backgroundImage));
			// cout << "Image loaded: " << ofToDataPath(backgroundImage) << endl;
		}
	}

	// cout << "UPDATE 1" << endl;
	
	// Load background video.
	if(backgroundVideo!="none"){
#if defined(TARGET_RASPBERRY_PI)
		// Load video on Raspberry Pi.
		if(!bgVideo.isPlaying() && !bgVideo.isPaused()){
			ofxOMXPlayerSettings settings;
			settings.videoPath = ofToDataPath(backgroundVideo, true);
			settings.enableLooping = loopBackgroundVideo;
				
			bgVideo.setup(settings);
		}

		// Check if need to Pause video.
		if (pauses.size() > 0 && doingPauses) {
			//ofLog(OF_LOG_NOTICE, "Time: " + ofToString((float)bgVideo.getCurrentFrame() / bgVideo.getTotalNumFrames() * bgVideo.getDurationInSeconds()));
			//ofLog(OF_LOG_NOTICE, "Time: " + ofToString(bgVideo.getCurrentFrame()));
			//ofLog(OF_LOG_NOTICE, "Time: " + ofToString(bgVideo.getTotalNumFrames()));
			//ofLog(OF_LOG_NOTICE, "Time: " + ofToString(bgVideo.getDurationInSeconds()));
			//ofLog(OF_LOG_NOTICE, "Pause: " + ofToString(pauses[currentPause]));
			if (currentPause == pauses.size()) {
				bgVideo.setPaused(false);
				doingPauses = false;
				//ofLog(OF_LOG_NOTICE, "PLAY LAST BIT");
			} else if (((float)bgVideo.getCurrentFrame() / bgVideo.getTotalNumFrames() * bgVideo.getDurationInSeconds()) > pauses[currentPause]) {
				bgVideo.setPaused(true);
				//ofLog(OF_LOG_NOTICE, "PAUSE");
			} else {
				bgVideo.setPaused(false);
				//ofLog(OF_LOG_NOTICE, "PLAY");
			}
		}

#else
		// Load video on Desktop.
		if(!bgVideo.isLoaded()){
			//cout << "LOADING VIDEO" << endl;

			ofFile file(ofToDataPath(backgroundVideo));
			if(file.exists()){
				bgVideo.loadMovie(ofToDataPath(backgroundVideo));
				if(loopBackgroundVideo){
					bgVideo.setLoopState(OF_LOOP_NORMAL);
				} else {
					bgVideo.setLoopState(OF_LOOP_NONE);
				}
				bgVideo.play();
			}
		}

		// Check if need to Pause video.
		if(pauses.size() > 0 && doingPauses){
			//cout << "Position: " << ofToString(bgVideo.getPosition() * bgVideo.getDuration()) << " Pause: " << ofToString(pauses[currentPause]) << endl;
			if (currentPause == pauses.size()) {
				bgVideo.setPaused(false);
				doingPauses = false;
			} else if((bgVideo.getPosition() * bgVideo.getDuration()) > pauses[currentPause]){
				bgVideo.setPaused(true);
			} else {
				bgVideo.setPaused(false);
			}
		}

		// Update video player;
		bgVideo.update();
#endif

		// cout << "UPDATE 2" << endl;

		// Items.
		if(items.size() > 0 && currentItem > -1){
			(items)[currentItem]->update();
		}
			
	}

	// cout << "UPDATE 3" << endl;

}

void Slide::draw(){
	// cout << "DRAW 0" << endl;

	// Set default alpha.
	ofSetColor(255, 255, 255, 255);

	// Draw background color.
	ofBackground(backgroundColor);

	// cout << "DRAW 1" << endl;

	// Draw background image.
	if(bgImage.isAllocated()){
		bgImage.draw((ofGetWindowWidth() - bgImage.getWidth()) / 2, (ofGetWindowHeight() - bgImage.getHeight()) / 2);
	}

	// Draw background video.
#if defined(TARGET_RASPBERRY_PI)
	// Load video on Raspberry Pi.
	if(bgVideo.isPlaying()){
		int vHeight = ofGetWindowWidth() / bgVideo.getWidth() * bgVideo.getHeight();
		bgVideo.draw(0, 0, ofGetWindowWidth(), vHeight);
	}
#else
	if(bgVideo.isLoaded()){
		//cout << "DRAWING VIDEO" << endl;

		int vHeight = ofGetWindowWidth() / bgVideo.getWidth() * bgVideo.getHeight();
		bgVideo.draw(0, 0, ofGetWindowWidth(), vHeight);
	}
#endif

	// cout << "DRAW 2" << endl;

	// Items.
	if(items.size() > 0 && currentItem > -1){
		(items)[currentItem]->draw();
	}
	
	// cout << "DRAW 3" << endl;

	// Do slide transition.
	if(transition != PRESENTER_TRANSITION_NONE){
		ofImage screen;
		screen.grabScreen(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
		doTransition(&screen);
	}

	// cout << "DRAW 4" << endl;

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
				// cout << "FADE: " << ofToString(elapsedTime) << endl;

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
				// cout << "SCROLLING: " << ofToString(elapsedTime) << endl;

				previousSlide.draw(0.0, 0.0);
				current->drawSubsection(0.0, 0.0, current->getWidth(), transitionState, 0.0, 0.0);

				if(transitionState < current->getHeight()){
					transitionState += current->getHeight() / 2 * elapsedTime / 1000;
					if(transitionState > current->getHeight()){
						transitionState = current->getHeight();
					}
				} else {
					doingTransition = false;
				}
				break;
			case PRESENTER_TRANSITION_FADETOBLACK:
				// cout << "FADETOBLACK: " << ofToString(elapsedTime) << endl;

				if(transitionState == 0){
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
						transitionState = 1;
					}
				} else if(transitionState == 1){
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
			case PRESENTER_TRANSITION_NONE:
			default:
				current->draw(0., 0.);
				break;
		}
	} else {
		current->draw(0., 0.);
	}

}

void Slide::setTransitionDefaults(){
	doingTransition = false;
	transitionAlpha = 0;
	transitionStartTime = 0;
	transitionState = 0;
}

void Slide::closeVideos(){
	// Close the video to save resources.
#if defined(TARGET_RASPBERRY_PI)
	if(bgVideo.isPlaying()){
		bgVideo.close();
	}
#else
	if(bgVideo.isLoaded()){
		bgVideo.closeMovie();
	}
#endif
}

void Slide::clear() {
	closeVideos();
	bgImage.clear();
	previousSlide.clear();
}

bool Slide::next(){
	if(items.size() > 0 && ++currentItem != items.size()){
		return true;	
	}

	if(pauses.size() > 0 && ++currentPause <= pauses.size()){
		return true;
	}

	currentItem = items.size() - 1;
	currentPause = 0;

	closeVideos();
	setTransitionDefaults();
	return false;
}

bool Slide::previous(){
	if(items.size() > 0 && --currentItem != -1){
		return true;	
	}

	currentPause = 0;

	closeVideos();
	setTransitionDefaults();
	return false;
}



