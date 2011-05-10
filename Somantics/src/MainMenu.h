/**
 * MainMenu.h
 * ReacticklesMagic
 *
 * Created by Marek Bereza on 26/04/2011.
 *
 */
#include "Reactickle.h"
#include "InteractiveObject.h"
#include "ReactickleButton.h"
#include "SimpleButton.h"

class MainMenu: public Reactickle, public ReactickleButtonListener, public SimpleButtonListener {
public:
	void setup();
	void draw();
	
	// this is a list of all the things that can be interacted with
	vector<InteractiveObject*> items;
	
	// this is a subset of items - just the ones that are listed
	// as reactickles
	vector<ReactickleButton*> reactickleButtons;
	void initMenu();
	void touchDown(float x, float y, int touchId);
	void touchMoved(float x, float y, int touchId);
	void touchUp(float x, float y, int touchId);
	void reactickleSelected(string name);
	void buttonPressed(string name);
private:
	void arrange();
	
	// the scrolling reactickles area
	ofRectangle scrollRect;
	
	// how much we're scrolled away from zero
	float scrollOffset;
	
	// scrolling touch stuff
	bool touching;
	float touchX;
	float deltaX;
	float totalWidth;
	
	SimpleButton settingsButton;
	SimpleButton aboutButton;
};