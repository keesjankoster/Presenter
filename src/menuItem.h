#pragma once

#include "ofMain.h"

class MenuItem
{
	public:
		MenuItem(void);
		~MenuItem(void);

		string name;
		string path;
		ofPtr< MenuItem > parent;

		ofTrueTypeFont *font;

		bool isCategory;
		bool isMenuUp;
		
		vector< ofPtr< MenuItem > > items;

		void setLocation(int x, int y);
		void draw(bool pressed);
		bool isMouseOver(void);
		bool isSelected(void);
		void select();
		void deselect();

	private:
		int itemX;
		int itemY;

		static ofImage * img_cat;
		static ofImage * img_pst;
		
		bool selected;
		
};

