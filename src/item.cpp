#include "item.h"
#include <sstream>
#include <iostream>

Item::Item(void){
	scale = -1.;
}


Item::~Item(void){
}

void Item::update(){
	
}

void Item::draw(){
	if(type == PRESENTER_SLIDE_ITEM_TYPE_TITLE || type == PRESENTER_SLIDE_ITEM_TYPE_LYRIC){
		// determine the scale for the biggest possible font size
		if(scale == -1){
			float w = (box->getWidth() - 50) / font->stringWidth(value);
			float h = (box->getHeight() - 150) / font->stringHeight(value);

			//cout << "(" << box->getWidth() << "," << box->getHeight() << "," << font->stringWidth(value) << "," << font->stringHeight(value) << ")" << endl;

			scale = w < h ? w : h;

			if(scale > 1){
				scale = 1;
			}
		}

		// set scale to have the biggest possible font size
		ofScale(scale, scale);

		// start y position, on top of screen
		float y = box->getY() + 150;
		if(type == PRESENTER_SLIDE_ITEM_TYPE_TITLE){
			y = box->getY() + ((box->getHeight() - (font->stringHeight(value) * scale)) / 2);
		} 

		// get line by line
		std::istringstream f(value);
		std::string line;    
		while (std::getline(f, line)) {
			float x = box->getX() + (box->getWidth() - (font->stringWidth(line) * scale)) / 2;

			/*cout << "WINDOW: " << ofToString(ofGetWindowWidth()) << endl;
			cout << "SCALE: " << ofToString(scale) << endl;
			cout << "X: " << ofToString(x) << endl;
			cout << "Width: " << ofToString(font->stringWidth(line)) << endl;*/
			
			// draw black shadow
			ofSetColor(ofColor::black);
			font->drawString(line, (x + 5) / scale, (y + 5) / scale);

			// draw white letters
			ofSetColor(ofColor::white);
			font->drawString(line, x / scale, y / scale);

			// set y position for next line
			y += (font->getLineHeight() * scale);
		}

	}
}

