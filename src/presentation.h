#pragma once

#include <string>
#include <ofxXmlSettings.h>
#include "slide.h"

class Presentation
{
	public:
		Presentation(void);
		Presentation(std::string path);
		~Presentation(void);

		void load(std::string path);
		void update();
		void draw();
		int next(void);
		int previous(void);

	private:
		string dataPathRoot;
		
		ofxXmlSettings presentation;
		vector< ofPtr<Slide> > slides;

		unsigned int currentSlide;
};

