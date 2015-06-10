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
	int numSlides;

	// Set data path to Presentation path.
	dataPathRoot = ofToDataPath("");
	ofSetDataPathRoot(ofToDataPath("Presentations/" + path + "/", true));
	
	// Load the presentation.xml file.
	//presentation.loadFile("Presentations/" + path + "/presentation.xml");
	presentation.loadFile(ofToDataPath("presentation.xml"));
	presentation.pushTag("slides");
	numSlides = presentation.getNumTags("slide");

	// Load the slides from the presentation.xml file.
	for(int i = 0; i < numSlides; i++){
		ofPtr<Slide> slide(new Slide());

		// Setup Slide Transition.
		string transition;
		transition = presentation.getAttribute("slide", "transition", "none", i);
		if(transition == "fade"){
			slide->transition = PRESENTER_TRANSITION_FADE;
			cout << "FADE" << endl;
		} else if(transition == "scrolling"){
			slide->transition = PRESENTER_TRANSITION_SCROLLING;
			cout << "SCROLLING" << endl;
		} else if(transition == "fadetoblack"){
			slide->transition = PRESENTER_TRANSITION_FADETOBLACK;
			cout << "FADETOBLACK" << endl;
		} else {
			slide->transition = PRESENTER_TRANSITION_NONE;
			cout << "NONE" << endl;
		}

		// Grab current screen for first slide transition.
		if(i==0 || slide->transition != PRESENTER_TRANSITION_NONE){
			slide->previousSlide.grabScreen(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
		}

		// Setup Background Color.
		ofColor color;
		slide->backgroundColor = color.fromHex(ofHexToInt(presentation.getAttribute("slide", "backgroundcolor", "#000000", i).replace(0,1,"0x")));
		//cout << ofToString(slide->backgroundColor) << endl;

		// Setup Background Image.
		slide->backgroundImage = presentation.getAttribute("slide", "backgroundimage", "none", i);

		// Setup Background Video.
		slide->backgroundVideo = presentation.getAttribute("slide", "backgroundvideo", "none", i);
		if(slide->backgroundVideo != "none"){
			// Setup looping.
			slide->loopBackgroundVideo = (presentation.getAttribute("slide", "loop", "no", i)=="yes");
		}
		
		// Add slide to slides collection.
		slides.push_back(slide);
	}

	// Set the current slide.
	currentSlide = 0;
}

void Presentation::update(){
	if(currentSlide < slides.size()){
		// Update current slide.
		slides[currentSlide]->update();

	}

	//cout << ofToString(slide->backgroundColor) << endl;
}

void Presentation::draw(){
	if(currentSlide < slides.size()){
		// Draw current slide
		slides[currentSlide]->draw();

	}
}

int Presentation::next(void){
	slides[currentSlide]->next();
	if(++currentSlide == slides.size()){
		ofSetDataPathRoot(dataPathRoot);
		return 0;
	} else {
		slides[currentSlide]->previousSlide.grabScreen(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
		return 1;
	}
}

int Presentation::previous(void){
	slides[currentSlide]->previous();
	if(--currentSlide == -1){
		ofSetDataPathRoot(dataPathRoot);
		return 0;
	} else {
		slides[currentSlide]->previousSlide.grabScreen(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
		return 1;
	}
}
