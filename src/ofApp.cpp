#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetLogLevel(OF_LOG_VERBOSE);

	//calibrate depth and video info
	kinect.setRegistration(true);

	kinect.init();
	//kinect.init(true); // shows infrared instead of RGB video image
	//kinect.init(false, false); // disable video image (faster fps)
	kinect.open();

	if (kinect.isConnected()) {
		ofLogNotice() << "connected";
	}
	else {
		ofLogNotice() << "no kinect";
	}

	midiOut.openPort(1);

	grayImg.allocate(ofGetWidth(), ofGetHeight());
	grayThreshNear.allocate(ofGetWidth(), ofGetHeight());
	grayThreshFar.allocate(ofGetWidth(), ofGetHeight());

	nearThresh = 255;
	farThresh = 240;

	ofSetFrameRate(60);
	//ofSetFullscreen(true);

	//set tilt to 0 on open
	angle = 0;
	kinect.setCameraTiltAngle(angle);
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSetFullscreen(full);
	kinect.update();

	if (kinect.isFrameNew()) {
		//set gray image from depth
		grayImg.setFromPixels(kinect.getDepthPixels());

		grayThreshNear = grayImg;
		grayThreshFar = grayImg;
		grayThreshNear.threshold(nearThresh, true);
		grayThreshFar.threshold(farThresh);
		cvAnd(grayThreshNear.getCvImage(), grayThreshFar.getCvImage(), grayImg.getCvImage(), NULL);


	}

	//update CV images
	grayImg.flagImageChanged();

	// find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
	contourFinder.findContours(grayImg, 10, (ofGetWidth() * ofGetHeight()) / 2, 20, false);

	//find blob X, Y
	for (int i = 0; i < contourFinder.nBlobs; i++) {
		blobX[i] = contourFinder.blobs[i].centroid.x;
		blobY[i] = contourFinder.blobs[i].centroid.y;
		blobD[i] = ofMap(kinect.getDistanceAt(blobX[i], blobY[i]), 500, 800, 127, 0);
		note tempNote;
		tempNote.setup();
		notes.push_back(tempNote);
		
	}
	midi();

}

//--------------------------------------------------------------
void ofApp::draw() {
	ofBackground(0);

	ofSetColor(255);

	//draw from the live kinect
	//kinect.drawDepth(10, 10, 400, 300);
	//draw live RGB image from kinect
	//kinect.draw(0, 0, ofGetWidth(), ofGetHeight());

	//draw live depth
	//grayImg.draw(0, 0, ofGetWidth(), ofGetHeight());
	//draw blob
	//contourFinder.draw(0, 0, ofGetWidth(), ofGetHeight());

	ofSetCircleResolution(100);
	for (int i = 0; i < notes.size(); i++) {
		notes[i].draw(blobX[i], blobY[i], blobD[i]);
	}

	int gap = ofGetWidth() / 14;
	for (int j = 1; j < 14; j++) {
		ofDrawLine((gap * j), 0, (gap * j), ofGetHeight());
	}

	ofDrawBitmapString(scale[k], 10, 10);

	if (major) {
		ofDrawBitmapString("MAJOR", 28, 10);
	}
	else {
		ofDrawBitmapString("MINOR", 28, 10);
	}
}

void ofApp::midi() {
	for (int i = 0; i < notes.size(); i++) {

		notes[i].key = notes[i].getKey(blobX[i], offset, octset, major);
		notes[i].velocity = notes[i].getVel(blobD[i]);
		notes[i].polytouch = notes[i].getPoly(notes[i].getVel(blobD[i]));
		notes[i].bend = notes[i].getBend(blobX[i]);
		notes[i].mod = notes[i].getMod(blobY[i]);

		if (notes[i].key != temp[i]) {
			midiOut.sendNoteOn(channel, notes[i].key, notes[i].velocity);
		} else {
			midiOut.sendControlChange(channel, 1, notes[i].mod);
			midiOut.sendPitchBend(channel, notes[i].bend);
			midiOut.sendPolyAftertouch(channel, notes[i].key, notes[i].polytouch);
		}


		temp[i] = notes[i].key;



		if (notes[i].velocity == 0) {
			midiOut.sendNoteOff(channel, notes[i].key);
			killed = true;
		}
		if (contourFinder.nBlobs < notes.size()) {
			killed = true;
		}
		if (killed) {
			notes.erase(notes.begin() + i);
		}
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	switch (key) {
	case 'w':
		kinect.enableDepthNearValueWhite(!kinect.isDepthNearValueWhite());
		break;

	case '>':
	case '.':
		farThresh++;
		if (farThresh > 255) farThresh = 255;
		break;

	case '<':
	case ',':
		farThresh--;
		if (farThresh < 0) farThresh = 0;
		break;

	case '+':
	case '=':
		nearThresh++;
		if (nearThresh > 255) nearThresh = 255;
		break;

	case '-':
		nearThresh--;
		if (nearThresh < 0) nearThresh = 0;
		break;

	case OF_KEY_UP:
		angle++;
		if (angle > 30) angle = 30;
		kinect.setCameraTiltAngle(angle);
		break;

	case OF_KEY_DOWN:
		angle--;
		if (angle < -30) angle = -30;
		kinect.setCameraTiltAngle(angle);
		break;

	case 'f':
		full = !full;
		break;

	case 'z':
		octset--;
		break;


	case 'x':
		octset++;
		break;

	case 'c':
		offset--;
		if (k > 0) {
			k--;
		}
		else {
			k = 11;
		}
		break;


	case 'v':
		offset++;
		if (k < 11) {
			k++;
		}
		else {
			k = 0;
		}
		break;

	case 'm':
		if (major) {
			major = false;
		} else {
			major = true;
		}
		break;
	}
	}


//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::exit() {
	kinect.setCameraTiltAngle(0);
	kinect.close();
	midiOut.closePort();
}