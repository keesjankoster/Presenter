#pragma once

#include "ofMain.h"
#include "item.h"

class Presentation;

#if defined(TARGET_RASPBERRY_PI)
#include "ofxOMXPlayer.h"
#endif

enum PresenterSlideTransition {
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
		void clear();
		bool next(void);
		bool previous(void);

		ofColor backgroundColor;
		string backgroundImage;
		string backgroundVideo;
		bool loopBackgroundVideo;
				
		PresenterSlideTransition transition;
		ofImage previousSlide;
		
		// Items (Titles, Lyrics) on a slide.
		vector< ofPtr<Item> > items;
		vector< float > pauses;

		Presentation* presentation;
		
	private:
		void closeVideos();

		ofImage bgImage;
		//ofTexture bgImage;
		
#if defined(TARGET_RASPBERRY_PI)
		// Use ofxOMXPlayer on Raspberry Pi.
		ofxOMXPlayer bgVideo;
#else
		// Use ofVideoPlayer on Desktop
		ofVideoPlayer bgVideo;
#endif

		// Item varables
		int currentItem;
		int currentPause;

		// Transition variables
		bool doingTransition;
		float transitionAlpha;
		unsigned long transitionStartTime;
		int transitionState;

		void setTransitionDefaults();
		void doTransition(ofImage * current);

};

