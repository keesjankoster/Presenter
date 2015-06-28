#include "item.h"


Item::Item(void){
	scale = -1.;
}


Item::~Item(void){
}

void Item::update(){
	
}

void Item::draw(){
	if(type == PRESENTER_SLIDE_ITEM_TYPE_TITLE || type == PRESENTER_SLIDE_ITEM_TYPE_LYRIC){
		font->setSpaceSize(0.60);

		if(scale == -1){
			scale = (ofGetWindowWidth() - 50) / font->stringWidth(value);
			if(scale > 1.5){
				scale = 1.5;
			}
		}

		ofScale(scale, scale);
		ofSetColor(ofColor::black);
		font->drawString(value, 27 / scale, 102 / scale);
		ofSetColor(ofColor::white);
		font->drawString(value, 25 / scale, 100 / scale);

	}
}

