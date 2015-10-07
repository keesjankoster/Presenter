#pragma once

#include "ofMain.h"
#include "menuItem.h"

class Menu
{
	public:
		Menu(void);
		~Menu(void);

		void hide();
		void unhide();
		bool isHidden(void);

		void setLocation(int x, int y);

		ofEvent<string> onMenuClick;

		static const int PRESENTER_MENU_ITEM_WIDTH = 250;
		static const int PRESENTER_MENU_ITEM_HEIGHT = 30;
		static const int PRESENTER_MENU_ITEM_SPACER = PRESENTER_MENU_ITEM_HEIGHT / 0.75;

	private:
		void draw(ofEventArgs & args);
		void mousePressed(ofMouseEventArgs & args);
		void mouseReleased(ofMouseEventArgs & args);
		void keyPressed(ofKeyEventArgs & args);

		void addListeners();
		void removeListeners();

		void loadMenuItems(ofDirectory directory, ofPtr< MenuItem > currentItem);

		ofPtr< MenuItem > rootItem;
		ofPtr< MenuItem > activeItem;
		MenuItem * pressedItem;

		bool visible;

		ofTrueTypeFont font_small;
		ofTrueTypeFont font_large;

		ofImage img_hdd;

		int menuX;
		int menuY;
};

