#ifndef _TEST_APP
#define _TEST_APP

/////////////
// Arduino Firmata communication includes
/////////////
#pragma once

#include "ofMain.h"
#include "ofEvents.h"


/////////////
// ofxBeatTracking includes
/////////////
// ヘッダをインクルードすればbeatDetect使用可能
#include "beatDetect.h"

class testApp : public ofBaseApp {
public:
    /////////////
    // common functions
    /////////////
	void setup();
	void update();
	void draw();

	void keyPressed  (int key);
	void keyReleased(int key);
    
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);

    /////////////
    // ofxBeatTracking declarations
    /////////////
    ofTrueTypeFont  fbook;
	void audioReceived(float* input, int bufferSize, int nChannels);

    // beatDetectオブジェクトのインスタンスを宣言する
    // 初期化はコンストラクタで完了するので
    // testAppのsetup()ですることは無い
    beatDetect bd;
    
    /////////////
    // Arduino Firmata communication declarations
    /////////////
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

	ofImage				bgImage;
	ofTrueTypeFont		font;
    ofTrueTypeFont      smallFont;
	ofArduino	ard;
	bool		bSetupArduino;			// flag variable for setting up arduino once

    /////////////
    // Custom Apache declarations
    /////////////
    void beat();
private:
    
    /////////////
    // Arduino Firmata communication declarations
    /////////////
    void setupArduino(const int & version);
    void digitalPinChanged(const int & pinNum);
    void analogPinChanged(const int & pinNum);
	void updateArduino();
    void moveServo(int step, bool relative);
    
    string buttonState;
    string potValue;

    /////////////
    // Custom Apache declarations
    /////////////
    int servoPosition;
    int servoTarget;
    // TODO: declare "servoPositions" and "servoTargets" as array[3], 
    // one for each servo we plan on using this project    
};

#endif
