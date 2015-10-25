#include "presentation.h"
#include "slide.h"

Presentation::Presentation(void){

}

Presentation::Presentation(std::string path){
	load(path);
}

Presentation::~Presentation(void){
	slides.clear();
}

void Presentation::load(std::string path){
	// Set data path to Presentation path.
	dataPathRoot = ofToDataPath("");
	ofSetDataPathRoot(path + "\/");
	
	// Load the presentation.xml file.
	presentation.loadFile(ofToDataPath("presentation.xml"));
	presentation.pushTag("slides");
	int numSlides = presentation.getNumTags("slide");

	// Load the slides from the presentation.xml file.
	for(int i = 0; i < numSlides; i++){
		ofPtr<Slide> slide(new Slide());

		// set the presentation
		slide->presentation = this;

		// Setup Slide Transition.
		string transition = presentation.getAttribute("slide", "transition", "none", i);
		if(transition == "fade"){
			slide->transition = PRESENTER_TRANSITION_FADE;
			// cout << "FADE" << endl;
		} else if(transition == "scrolling"){
			slide->transition = PRESENTER_TRANSITION_SCROLLING;
			// cout << "SCROLLING" << endl;
		} else if(transition == "fadetoblack"){
			slide->transition = PRESENTER_TRANSITION_FADETOBLACK;
			// cout << "FADETOBLACK" << endl;
		} else {
			slide->transition = PRESENTER_TRANSITION_NONE;
			// cout << "NONE" << endl;
		}

		// Grab current screen for first slide transition.
		if(i==0 || slide->transition != PRESENTER_TRANSITION_NONE){
			slide->previousSlide.grabScreen(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
		}

		// Setup Background Color.
		ofColor color;
		slide->backgroundColor = color.fromHex(ofHexToInt(presentation.getAttribute("slide", "backgroundcolor", "#000000", i).replace(0,1,"0x")));
		// cout << ofToString(slide->backgroundColor) << endl;

		// Setup Background Image.
		slide->backgroundImage = presentation.getAttribute("slide", "backgroundimage", "none", i);

		// Setup Background Video.
		slide->backgroundVideo = presentation.getAttribute("slide", "backgroundvideo", "none", i);
		if(slide->backgroundVideo != "none"){
			// Setup looping.
			slide->loopBackgroundVideo = (presentation.getAttribute("slide", "loop", "no", i)=="yes");

			// Load video pausing.
			presentation.pushTag("slide", i);
			int numPauses = presentation.getNumTags("pause");
			for(int n = 0; n < numPauses; n++){
				vector< string > pause;
				float time;
				pause = ofSplitString(presentation.getAttribute("pause", "time", "00:00:00", n), ":");
				
				// add hours.
				time = ofToInt(pause[0]) * 60 * 60;
				// add minutes.
				time += ofToInt(pause[1]) * 60;
				// add seconds.
				time += ofToInt(pause[2]);

				slide->pauses.push_back(time);

				// cout << "Pause: " << presentation.getAttribute("pause", "time", "00:00:00", n) << " => " << ofToString(time) << endl;
			}

			presentation.popTag();
		}

		// Load items.
		presentation.pushTag("slide", i);
		int numItems = presentation.getNumTags("item");
		for(int n = 0; n < numItems; n++){
			ofPtr<Item> item(new Item());

			// Get the item type.
			string itemType = presentation.getAttribute("item", "type", "title", n);
			if(itemType == "title"){
				item->type = PRESENTER_SLIDE_ITEM_TYPE_TITLE;
			} else if(itemType == "lyric") {
				item->type = PRESENTER_SLIDE_ITEM_TYPE_LYRIC;
			}

			// Get the item value.
			item->value = presentation.getValue("item", "DEFAULT_TEXT", n);

			// Get font.
			if(item->type == PRESENTER_SLIDE_ITEM_TYPE_TITLE || item->type == PRESENTER_SLIDE_ITEM_TYPE_LYRIC){
				item->font = &fonts[presentation.getAttribute("item", "font", "verdana.ttf", n) + "_64"];
			}
			
			// Add the item to the slide.
			slide->items.push_back(item);
		}
		presentation.popTag();
		
		// Add slide to slides collection.
		slides.push_back(slide);
	}

	// Set the current slide.
	currentSlide = 0;

	// cout << "READY LOADING" << endl;
}

void Presentation::update(){
	if(currentSlide < slides.size()){
		// Update current slide.
		slides[currentSlide]->update();

	}

	// cout << ofToString(slide->backgroundColor) << endl;
}

void Presentation::draw(){
	if(currentSlide < slides.size()){
		// Draw current slide
		slides[currentSlide]->draw();

	}
}

bool Presentation::next(void){
	if(!slides[currentSlide]->next()){
		if(++currentSlide == slides.size()){
			ofSetDataPathRoot(dataPathRoot);
			return false;
		} else {
			slides[currentSlide]->previousSlide.grabScreen(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
			return true;
		}
	}
}

bool Presentation::previous(void){
	if(!slides[currentSlide]->previous()){
		if(--currentSlide == -1){
			ofSetDataPathRoot(dataPathRoot);
			return false;
		} else {
			slides[currentSlide]->previousSlide.grabScreen(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
			return true;
		}
	}
}

void Presentation::exit(){
	ofSetDataPathRoot(dataPathRoot);
}
