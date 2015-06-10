#pragma once

enum PresenterSlideItemType{
	PRESENTER_SLIDE_ITEM_TYPE_TITLE,
	PRESENTER_SLIDE_ITEM_TYPE_LYRIC,
};

class Item
{
	public:
		Item(void);
		~Item(void);

		void update();
		void draw();

		PresenterSlideItemType type;

		
		
};

