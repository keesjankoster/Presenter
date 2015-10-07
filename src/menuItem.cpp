#include "menuItem.h"
#include "menu.h"

ofImage * MenuItem::img_cat = new ofImage();
ofImage * MenuItem::img_pst = new ofImage();

MenuItem::MenuItem(void){
	// Default variables.
	isCategory = false;
	isMenuUp = false;
	selected = false;

	// TODO: Change Images to Menu, now they are loaded for each MenuItem...
	if(!img_cat->isAllocated()){
		img_cat->loadImage("cat.png");
		cout << "LOAD" << endl;
	}
	if(!img_pst->isAllocated()){
		img_pst->loadImage("pst.png");
	}
}

MenuItem::~MenuItem(void){

}

void MenuItem::draw(bool pressed){
	// Test if Mousecursor is over the button.
	if(isMouseOver()){
		if(pressed){
			ofSetColor(ofColor::black);
			ofRectRounded(itemX, itemY, Menu::PRESENTER_MENU_ITEM_WIDTH, Menu::PRESENTER_MENU_ITEM_HEIGHT, 7);
		}

		ofSetColor(ofColor::darkGrey);
	} else if(isSelected()){
		ofSetColor(ofColor::black);	
	} else {
		ofSetColor(ofColor::lightGray);	
	}

	// Draw Button Border.
	ofNoFill();
	ofSetLineWidth(2);
	ofRectRounded(itemX, itemY, Menu::PRESENTER_MENU_ITEM_WIDTH, Menu::PRESENTER_MENU_ITEM_HEIGHT, 7);
	ofFill();
	
	if(pressed && isMouseOver()){
		ofSetColor(ofColor::white);
	} else {
		ofSetColor(ofColor::black);
	}

	// Draw Button Image.
	if(isCategory || isMenuUp){
		img_cat->draw(itemX + 5, itemY + 3);
	} else {
		img_pst->draw(itemX + 5, itemY + 3);
	}
	
	// Draw Button Text.
	font->drawString(name, itemX + 40, itemY + 20);

}

void MenuItem::setLocation(int x, int y){
	itemX = x;
	itemY = y;
}

bool MenuItem::isMouseOver(void){
	return (ofGetMouseX() >= itemX && ofGetMouseX() <= (itemX + Menu::PRESENTER_MENU_ITEM_WIDTH) && ofGetMouseY() >= itemY && ofGetMouseY() <= (itemY + Menu::PRESENTER_MENU_ITEM_HEIGHT));
}

bool MenuItem::isSelected(void){
	return selected;
}

void MenuItem::deselect(){
	selected = false;
}

void MenuItem::select(){
	for(int i = 0; i < parent->items.size(); i++){
		parent->items[i]->deselect();
	}
	selected = true;
}
