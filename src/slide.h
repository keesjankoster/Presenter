#pragma once

#include "ofMain.h"
#include "item.h"

#if defined(TARGET_RASPBERRY_PI)
#include "ofxOMXPlayer.h"
#endif

enum PresenterSlideTransition{
	PRESENTER_TRANSITION_NONE,
	PRESENTER_TRANSITION_FADE,
	PRESENTER_TRANSITION_SCROLLING,
	PRESENTER_TRANSITION_FADETOBLACK
};

class Slide
{
	public:
		Slide(void);
		~Slide(void);

		void update();
		void draw();
		void next();
		void previous();

		ofColor backgroundColor;
		string backgroundImage;
		string backgroundVideo;
		bool loopBackgroundVideo;
				
		PresenterSlideTransition transition;
		ofImage previousSlide;
		

		
	private:
		ofImage bgImage;
		
#if defined(TARGET_RASPBERRY_PI)
		// Use ofxOMXPlayer on Raspberry Pi.
		ofxOMXPlayer bgVideo;
#else
		// Use ofVideoPlayer on Desktop
		ofVideoPlayer bgVideo;
#endif

		vector< ofPtr<Item> > items;

		// Transition variables
		bool doingTransition;
		float transitionAlpha;
		unsigned long transitionStartTime;
		int transitionState;

		void doTransition(ofImage * current);

};

