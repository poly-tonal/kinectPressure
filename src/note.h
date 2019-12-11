#pragma once
#ifndef _NOTE
#define _NOTE
//#include "ofxMidi.h"
#include "ofMain.h"


class note
{
public:
	void setup();
	int getKey(int _x, int offset, int octset, bool major);
	int getVel(int _d);
	int getPoly(int vel);
	int getBend(int _x);
	int getMod(int _y);


	void draw(int _x, int _y, int _d);

	int channel;
	int key;
	int velocity;
	int polytouch;
	int bend;
	int mod;
	int x, y;

	int majScale[15] = { 60, 62, 64, 65, 67, 69, 71, 72, 74, 76, 77, 79, 81, 83, 84};
	int minScale[15] = { 60, 62, 63, 65, 67, 68, 70, 72, 74, 75, 77, 79, 80, 82, 84};

	bool played;
	float radius = 0;
	ofColor c = ofColor(255);
	note();
};
#endif
