#pragma once

#include "ofMain.h"

enum slideTransition{
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
				
		slideTransition transition;
		ofImage previousSlide;
		

		
	private:
		ofImage bgImage;
		ofVideoPlayer bgVideo;

		// Transition variables
		bool doingTransition;
		float transitionAlpha;
		unsigned long transitionStartTime;

		
		
		int transitionState1, transitionState2;
		vector<int> transitionVector;

		void doTransition(ofImage * current);

};

