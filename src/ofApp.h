#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxKinect.h"
#include "ofxMidi.h"
#include "note.h"
#include <vector>

#define POLYPHONY 16

class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();
	void midi();
	void exit();

	void keyPressed(int key);
	void windowResized(int w, int h);

	ofxKinect kinect;
	std::vector <note> notes;

	ofxCvGrayscaleImage grayImg;
	ofxCvGrayscaleImage grayThreshNear;
	ofxCvGrayscaleImage grayThreshFar;

	ofxCvContourFinder contourFinder;

	ofxMidiOut midiOut;

	int nearThresh;
	int farThresh;

	int angle;

	int blobX[16];
	int blobY[16];
	int blobD[16];
	int temp[16];

	int channel = 1;
	int key;
	int velocity;
	int poly;
	int bend;
	int mod;
	bool full = false;
	bool help = false;
	int octset = 0;
	int offset = 0;
	bool major = true;
	string scale[12] = { "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};
	int k = 0; //index for scale

	bool killed = false;
};
