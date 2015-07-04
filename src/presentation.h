#pragma once

#include <string>
#include <ofxXmlSettings.h>
#include "slide.h"
#include "ofxAssets.h"

class Presentation
{
	public:
		Presentation(void);
		Presentation(std::string path);
		~Presentation(void);

		void load(std::string path);
		void update();
		void draw();
		bool next(void);
		bool previous(void);
		void exit();

	private:
		string dataPathRoot;
		
		ofxXmlSettings presentation;
		vector< ofPtr<Slide> > slides;

		unsigned int currentSlide;

		ofxFontAssets fonts;
};

