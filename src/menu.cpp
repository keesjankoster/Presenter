#include "menu.h"

Menu::Menu(void){
	// Default variables.
	visible = true;
	menuX = 1500;
	menuY = 250;

	// Load Menu resources.
	font_small.loadFont("DroidSans.ttf", 12);
	font_large.loadFont("DroidSans.ttf", 18);

	img_hdd.loadImage("hdd.png");

	// Load menu items.
	ofPtr<MenuItem> item(new MenuItem());
	item->name = "Presentations";
	item->path = "Presentations";
	rootItem = item;
	activeItem = item;

	loadMenuItems(ofDirectory("Presentations"), item);

	// Listen to events for drawing, mouse and keyboard.
	addListeners();
}

Menu::~Menu(void){
	// Remove the listeners
	removeListeners();
}

void Menu::draw(ofEventArgs & args){
	if(visible){
		float x = menuX;
		float y = menuY;

		// Menu Header.
		ofSetColor(ofColor::black);
		font_large.drawString("Select Presentation:", x, y);

		// Menu Path.
		img_hdd.draw(x, y + 5);
		ofSetColor(102, 102, 102, 255);
		font_small.drawString(activeItem->path, x + 24, y + 22);
		
		for(int i = 0; i < activeItem->items.size(); i++){
			y += PRESENTER_MENU_ITEM_SPACER;
			
			// Draw MenuItem button.
			activeItem->items[i]->setLocation(x, y);
			activeItem->items[i]->draw(activeItem->items[i].get() == pressedItem);
		}
	}
}

void Menu::mousePressed(ofMouseEventArgs & args){
	for(int i = 0; i < activeItem->items.size(); i++){
		if(activeItem->items[i]->isMouseOver()){
			pressedItem = activeItem->items[i].get();
			break;
		}
	}
}

void Menu::mouseReleased(ofMouseEventArgs & args){
	for(int i = 0; i < activeItem->items.size(); i++){
		if(activeItem->items[i]->isMouseOver()){
			if(activeItem->items[i].get()==pressedItem){
				if(pressedItem->isMenuUp){
					activeItem = activeItem->parent;
				} else if(pressedItem->isCategory) {
					activeItem = activeItem->items[i];
				} else {
					ofNotifyEvent(onMenuClick, activeItem->items[i]->path);
				}
			};
			break;
		}
	}
	pressedItem = NULL;
}

void Menu::keyPressed(ofKeyEventArgs & args){
	// Find selected MenuItem.
	int selected;
	for(selected = 0; selected < activeItem->items.size(); selected++){
		if(activeItem->items[selected]->isSelected()){
			break;
		}
	}

	cout << args.key << endl;
	
	switch (args.key) 
    {       
        case OF_KEY_UP:    // KEY_UP
			if(selected-- == 0){
				selected = activeItem->items.size() - 1;
			}
			activeItem->items[selected]->select();
			break;
		case OF_KEY_DOWN:	 // KEY_DOWN
			if(++selected == activeItem->items.size()){
				selected = 0;
			}
			activeItem->items[selected]->select();
			break;
		case OF_KEY_RETURN:    // KEY_RETURN
		case 10:	// KEY_RETURN (RPi)
			if(activeItem->items[selected]->isMenuUp){
				activeItem = activeItem->parent;
			} else if(activeItem->items[selected]->isCategory) {
				activeItem = activeItem->items[selected];
			} else {
				ofNotifyEvent(onMenuClick, activeItem->items[selected]->path);
			}
            break; 
        default:
            break;
    }
}

void Menu::addListeners(){
	ofAddListener(ofEvents().draw, this, &Menu::draw);
	ofAddListener(ofEvents().mousePressed, this, &Menu::mousePressed);
	ofAddListener(ofEvents().mouseReleased, this, &Menu::mouseReleased);
	ofAddListener(ofEvents().keyPressed, this, &Menu::keyPressed);
}

void Menu::removeListeners(){
	ofRemoveListener(ofEvents().draw, this, &Menu::draw);
	ofRemoveListener(ofEvents().mousePressed, this, &Menu::mousePressed);
	ofRemoveListener(ofEvents().mouseReleased, this, &Menu::mouseReleased);
	ofRemoveListener(ofEvents().keyPressed, this, &Menu::keyPressed);
}

void Menu::hide(){
	visible = false;
	removeListeners();
}

void Menu::unhide(){
	visible = true;
	addListeners();
}

bool Menu::isHidden(void){
	return !(visible);
}

void Menu::loadMenuItems(ofDirectory directory, ofPtr< MenuItem > currentItem){
	// Each directory in the Presentations directory is a MenuItem:
	// - Directories with a presentation.xml file are Presentations.
	// - Directories with subdirectories are Categories.

	int i, size;

	// Check if there is a presentation.xml file.
	directory.listDir();
	directory.sort();
	size = directory.size();

	for(i = 0; i < size; i++){
		ofFile file = directory.getFile(i);
		if (file.isDirectory()){
			ofPtr<MenuItem> item(new MenuItem());
			item->name = file.getFileName();
			item->parent = currentItem;

			ofFile presentation = ofFile(file.getAbsolutePath() + "/presentation.xml");
			if(presentation.exists()){
				// Directory is a Presentation.
				item->isCategory = false;
				item->path = file.getAbsolutePath();
			} else {
				// Directory is a Category.
				item->isCategory = true;
				item->path = currentItem->path + " / " + item->name;

				// Add MenuItem to navigate up.
				ofPtr<MenuItem> itemUp(new MenuItem);
				itemUp->name = "...";
				itemUp->isMenuUp = true;
				itemUp->font = &font_small;
				itemUp->parent = item;
				item->items.push_back(itemUp);

				loadMenuItems(ofDirectory(file.path()), item);

			}
			item->font = &font_small;
			currentItem->items.push_back(item);
		}
	}
	if(currentItem->items.size() > 0){
		currentItem->items[0]->select();
	}
}

void Menu::setLocation(int x, int y){
	menuX = x;
	menuY = y;
}
