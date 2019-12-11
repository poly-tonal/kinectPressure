#include "note.h"

note::note() {
}

void note::setup() {
	//init values for each note and set a random colour
	//c.setHsb(ofRandom(255), 255, 255);
	c = 255;
	key = 0;
	velocity = 0;
	bend = 0;
	polytouch = 0;
	mod = 0;
	radius = 0;
	channel = 1;
	played = false;

}

int note::getKey(int _x, int offset, int octset, bool major) {
	int i = 0;
	i = (ofMap(_x, 0, ofGetWidth(), 15, 0));
	if (major) {
		return(majScale[i] + offset + (octset * 12));
	}else{
		return(minScale[i] + offset + (octset * 12));
	}
}

int note::getVel(int _d) {
	return(ofMap(_d, 127, 0, 127, 60));
}

int note::getBend(int _x){
	//if(key < prevKey || key > prevKey){
	//bend = ofMap(_x, 0, 640, 0, MIDI_MAX_BEND); //scale so each note bends from the neutral
	//}
	return(bend);
}

int note::getPoly(int vel) {
	polytouch = velocity;
	return(polytouch);
}

int note::getMod(int _y) {
	return(ofMap(_y, 0, ofGetHeight(), 127, 0));
}
	//if (!played) {
		//midiOut.sendNoteOn(channel, key, velocity);
		//midiOut.sendPolyAftertouch(channel, key, polytouch);
		//midiOut.sendPitchBend(channel, bend);
		//played = !played;
	//}


void note::draw(int _x, int _y, int _d) {

		x = ofMap(_x, 0, 640, ofGetWidth(), 0);
		y = ofMap(_y, 0, 480, 0, ofGetHeight());

		radius = _d;
		ofSetColor(c);

		ofDrawCircle(x, y, radius);

}


