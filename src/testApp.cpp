#include "testApp.h"
#include "beatDetect.h"

/////////////
// ofxBeatTracking declarations
/////////
int testApp_buffer_size = 1024;
int testApp_fft_size = 512;

bool detect= true;
bool drumVisible = true;
bool snareVisible = true;
bool hihatVisible = true;

/////////////
// Arduino Firmata communication documentation
//////////
// TODO: update this to be more "Apache" specific
/* 
 * This is a simple example use of ofArduino
 *
 * ofArduino currently only supports the standard Arduino boards
 * (UNO, Duemilanove, Diecimila, NG, and other boards based on the
 * ATMega168 or ATMega328 microcontrollers
 * The Arduio FIO and Arduino Mini should also work.
 * The Arduino MEGA and other variants based on microcontrollers
 * other than the ATMega168 and ATMega328 are not currently supported.
 * 
 * To use this example, open Arduino (preferably Arduino 1.0) and 
 * navigate to File -> Examples -> Firmata and open StandardFirmata.
 * Compile and upload StandardFirmata for your board, then close
 * the Arduino application and run this application.
 *
 * If you have a servo attached, press the left arrow key to rotate
 * the servo head counterclockwise and press the right arrow key to
 * rotate the servo head clockwise.
 *
 * Clicking the mouse over any part of the application will turn the
 * on-board LED on and off.
 *
 */


////////////
// Common methods
////////////

//--------------------------------------------------------------
void testApp::setup(){
    /////////////
	// ofxBeatTracking declarations
    //////////
    // フレームレートを60Hzにセット
	ofSetFrameRate(60);
	
	// オーディオインプットを指定 この値は固定すること
    // Output->0 Input->mono  sampleRate->44100  bufSize->1024
	ofSoundStreamSetup(0, 1, this, 44100, testApp_buffer_size, 4);

    //fbook.loadFont("arial.ttf", 32, true, false);
	ofBackground(20,20,20);
    printf("testApp setup() OK!\n");
    
    /////////////
    // Arduino Firmata communication setup
    //////////
    ofSetVerticalSync(true);
    
    buttonState = "digital pin:";
    potValue = "analog pin:";
    
    bgImage.loadImage("background.png");
	font.loadFont("franklinGothic.otf", 20);
    smallFont.loadFont("franklinGothic.otf", 14);
    
    // replace the string below with the serial port for your Arduino board
    // you can get this from the Arduino application or via command line
    // for OSX, in your terminal type "ls /dev/tty.*" to get a list of serial devices
	ard.connect("/dev/tty.usbmodem621", 57600);
	
	// listen for EInitialized notification. this indicates that
	// the arduino is ready to receive commands and it is safe to
	// call setupArduino()
	ofAddListener(ard.EInitialized, this, &testApp::setupArduino);
	bSetupArduino	= false;	// flag so we setup arduino when its ready, you don't need to touch this :)
    
    /////////////
    // Custom Apache declarations
    /////////////
    servoPosition, servoTarget = 0;
    // TODO: initialize "servoPositions" and "servoTargets" arrays to {0,0,0}
    ofResetElapsedTimeCounter();
}


//--------------------------------------------------------------
void testApp::update() {
    /////////////
	// ofxBeatTracking update
    //////////
    // To update the object BeatDetect
    // The calculation is performed of whether the beat has been detected at this time
    // The result of the calculation is obtained from the function, such as () bd.isBeat
    bd.updateFFT();

    /////////////
    // Arduino Firmata communication update
    //////////
	updateArduino();
}

//--------------------------------------------------------------
void testApp::draw()
{
    /////////////
    // ofxBeatTracking draw
    //////////
    char str[32];
	// 背景黒
	// ofBackground(0, 0, 0);
	// The value of the acquired FFT
    // Can be accessed as if bd.magnitude [i]
    // In the following we will draw the FFT to access in the form of bd.magnitude [i] the FFT obtained
   
	// Draw (column 1) the value of the FFT of the raw
    ofSetColor(155,155,75);
    for (int i = 1; i < (int)testApp_fft_size/2; i++){
        if(i % 16 == 0)
            ofSetColor(200,0,0);
        else
            ofSetColor(155,155,75);
		ofLine(10+(i*3),150,  10+(i*3),150-bd.magnitude[i]*10.0f);
        //printf("%f \n", magnitude_average[i]);
	}

    // Draw (column 1) the value of the FFT, which is averaged
    for (int i = (int)testApp_fft_size/2,b = 1; i<testApp_fft_size ; i++){
        if(i % 16 == 0)
            ofSetColor(200,0,0);
        else
            ofSetColor(155,155,75);
		ofLine(10+(b*3),300,  10+(b*3),300-bd.magnitude[i]*10.0f);
        b++;
        // printf("%f \n", magnitude_average[i]);
	}


    // averaged FFT (column 1)
    ofSetColor(134,113,89);
	for (int i = 1; i < (int)testApp_fft_size/2; i++){
        if(i % 16 == 0)
            ofSetColor(200,0,0);
        else
            ofSetColor(134,113,89);
		ofLine(10+(i*3),500,  10+(i*3),500-bd.magnitude_average[i]*10.0f);
	}
    // column 2
    for (int i = (int)testApp_fft_size/2,b = 1; i<testApp_fft_size ; i++){
        if(i % 16 == 0)
            ofSetColor(200,0,0);
        else
            ofSetColor(134,113,89);
		ofLine(10+(b*3),650,  10+(b*3),650-bd.magnitude_average[i]*10.0f);
        b++;
	}

    //ofSetColor(200,200,200);
	//for (int i = 1; i < (int)testApp_fft_size/2; i++){
	//	//ofLine(200+(i*4),200,200+(i*4),400-magnitude[i]*10.0f);
    //    //ofLine(10+(i*5.3),400,  10+(i*5.3),400-magnitude[i]*10.0f);
    //    sprintf(str, "%.0f", magnitude[i]);
    //    fbook.drawString(str,5+(i*7), 270);
    //    fbook.drawString(str,10+(i*5.3),250-magnitude[i]*10.0f);
    //}

    // ドラム音の検出
    if(drumVisible){
        if(bd.isBeatRange(0,2,2)){
            ofSetColor(255,0,0);
            ofRect(100,700,200,50);
            ofSetColor(0, 255, 0);
            ofDrawBitmapString("BASS!!",100,735);
        }else{
            ofSetColor(0,255,0);
            ofRect(100,700,200,50);
        }
    }

    // スネア音の検出
    if(snareVisible){
        if(bd.isBeatRange(12,18,4)){
            ofSetColor(255,0,0);
            ofRect(350,700,200,50);
            ofSetColor(0, 255, 0);
            ofDrawBitmapString("SNARE!!", 350, 735);
        }else{
            ofSetColor(0,255,0);
            ofRect(350,700,200,50);
        }
    }

    // ハイハット音の検出
    if(hihatVisible){
        if(bd.isBeatRange(27,31,3)){
            ofSetColor(255,0,0);
            ofRect(600,700,200,50);
            ofSetColor(0, 255, 0);
            ofDrawBitmapString("HiHat!!", 600, 735);
        }else{
            ofSetColor(0,255,0);
            ofRect(600,700,200,50);
        }
    }
    
    /////////////
    // Arduino Communication debug texts
    //////////
    
    // bgImage.draw(0,0);
       
    ofEnableAlphaBlending();
    ofSetColor(0, 0, 0, 127);
    ofRect(510, 15, 275, 150);
    ofDisableAlphaBlending();
    
    ofSetColor(255, 255, 255);
	if (!bSetupArduino){
		font.drawString("arduino not ready...\n", 515, 40);
		//ofDrawBitmapString("arduino not ready...\n", 515, 40);
	} else {
		font.drawString(potValue + "\n" + buttonState +
						"\nsending pwm: " + ofToString((int)(128 + 128 * sin(ofGetElapsedTimef()))), 515, 40);
		//ofDrawBitmapString(potValue + "\n" + buttonState +
		//				"\nsending pwm: " + ofToString((int)(128 + 128 * sin(ofGetElapsedTimef()))), 515, 40);
        
        ofSetColor(64, 64, 64);
        smallFont.drawString("If a servo is attached, use the left arrow key to rotate " 
                             "\ncounterclockwise and the right arrow key to rotate clockwise.", 200, 550);
        //ofDrawBitmapString("If a servo is attached, use the left arrow key to rotate " 
        //                     "\ncounterclockwise and the right arrow key to rotate clockwise.", 200, 550);
        ofSetColor(255, 255, 255);
	}
    
    // Move motor when beat is detected (bass & snare || bass & hihat)
    if(bd.isBeatRange(0,2,2) || bd.isBeatRange(12,18,4) || bd.isBeatRange(0,2,2) || bd.isBeatRange(27,31,3)){
        beat();
    }
}


//--------------------------------------------------------------
void testApp::keyPressed(int key){
	/////////////
    // ofxBeatTracking keyPressed
    //////////
    // 真偽値を反転させて画面表示をコントロール
    if(key == 'd'){
        drumVisible = !drumVisible;
    }
    if(key == 's'){
        snareVisible = !snareVisible;
    }
    if(key == 'h'){
        hihatVisible = !hihatVisible;
    }
    
    /////////////
    // Custom Apache Key handling
    //////////
    switch (key) {
        case OF_KEY_RIGHT:
            
            // turn LED on
            ard.sendDigital(18, ARD_HIGH);  // pin 20 if using StandardFirmata from Arduino 0022 or older
            
            if (ofGetKeyPressed(OF_KEY_SHIFT)) {
                // rotate servo head to 180 degrees
                moveServo(180, false);
                cout << "keyPressed(OF_KEY_RIGHT); // with OF_KEY_SHIFT pressed" << endl;
            } else {
                // rotate servo incrementally every time key is pressed
                cout << "keyPressed(OF_KEY_RIGHT);" << endl;
                moveServo(30, true);
            }
            break;
            
        case OF_KEY_LEFT:
            
            // turn LED off
            ard.sendDigital(18, ARD_LOW);  // pin 20 if using StandardFirmata from Arduino 0022 or older
            
            if (ofGetKeyPressed(OF_KEY_SHIFT)) {
                // rotate servo head to 0 degrees
                moveServo(0, false);
                cout << "keyPressed(OF_KEY_LEFT); // with OF_KEY_SHIFT pressed" << endl;
            } else {
                // rotate servo incrementally every time key is pressed
                moveServo(-30, true);
                cout << "keyPressed(OF_KEY_LEFT);" << endl;
            }
            break;
            
        case 32:
            beat();
            cout << "keyPressed(" ");" << endl;
            break;
            
            
        case OF_KEY_SHIFT:
            cout << "keyPressed(OF_KEY_SHIFT);" << endl;
            break;
            
        default:
            cout << "keyPressed(key); // key = " << key << endl;
            break;
    }    
}


/////////////
// Custom Apache methods
/////////

//--------------------------------------------------------------
void testApp::beat() {
    if (ofGetElapsedTimeMillis() < 100) {
        return;
    }
    
    // do something that is visibly different from what you're currently doing
    if (servoPosition > 0) {
        moveServo(-60, true);
    }else {
        moveServo(60, true);
    }
    ofResetElapsedTimeCounter();
}

//--------------------------------------------------------------
void testApp::moveServo(int step, bool relative) { // TODO: accept servo/pin number as well
    
    // calculate new servoPosition based on relative bool
    if (relative) {
        servoPosition += step;
    } else {
        servoPosition = step;
    }
    
    // clamp servoPosition values between 0 and 135
    // (seems to be the range of servos that come with the SIK Guide)
    if (servoPosition > 135) {
        servoPosition = 135;
        //print message to console
        cout << "moveServo(" << step << ", " << relative;
        cout << "); // servoPosition > 135 = " << servoPosition << endl;
    } else if (servoPosition < 0) {
        servoPosition = 0;
        //print message to console
        cout << "moveServo(" << step << ", " << relative;
        cout << "); // servoPosition > 135 = " << servoPosition << endl;
    }
    
    // rotate servo head based on calculated servoPosition
    ard.sendServo(9, servoPosition, false);
}


/////////////
// ofxBeatTracking methods
/////////

//--------------------------------------------------------------
void testApp::audioReceived(float* input, int bufferSize, int nChannels) {
    bd.audioReceived(input, bufferSize);
}


/////////////
// Unused methods
//////////

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    // turn on the onboard LED when the application window is clicked
	ard.sendDigital(13, ARD_HIGH);
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    // turn off the onboard LED when the application window is clicked
	ard.sendDigital(13, ARD_LOW);
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 
    
}


/////////////
// Arduino Firmata communication functions
//////////

//--------------------------------------------------------------
void testApp::setupArduino(const int & version) {
	
	// remove listener because we don't need it anymore
	ofRemoveListener(ard.EInitialized, this, &testApp::setupArduino);
    
    // it is now safe to send commands to the Arduino
    bSetupArduino = true;
    
    // print firmware name and version to the console
    ofLogNotice() << ard.getFirmwareName(); 
    ofLogNotice() << "firmata v" << ard.getMajorFirmwareVersion() << "." << ard.getMinorFirmwareVersion();
    
    // Note: pins A0 - A5 can be used as digital input and output.
    // Refer to them as pins 14 - 19 if using Standard Firmata from Arduino 1.0.
    // If using Arduino 0022 or older, then use 16 - 21.
    // Firmata pin numbering changed in version 2.3 (which is included in Arduino 1.0)
    
    // set pins D2 and A5 to digital input
    ard.sendDigitalPinMode(2, ARD_INPUT);
    ard.sendDigitalPinMode(19, ARD_INPUT);  // pin 21 if using StandardFirmata from Arduino 0022 or older
    
    // set pin A0 to analog input
    ard.sendAnalogPinReporting(0, ARD_ANALOG);
    
    // set pin D13 as digital output
	ard.sendDigitalPinMode(13, ARD_OUTPUT);
    // set pin A4 as digital output
    ard.sendDigitalPinMode(18, ARD_OUTPUT);  // pin 20 if using StandardFirmata from Arduino 0022 or older
    
    // set pin D11 as PWM (analog output)
	ard.sendDigitalPinMode(11, ARD_PWM);
    
    // attach a servo to pin D9
    // servo motors can only be attached to pin D3, D5, D6, D9, D10, or D11
    ard.sendServoAttach(9);
    
    // initialize servo position
    servoPosition = 0; // TODO: re/move this line to testApp::setup()?
    ard.sendServo(9, servoPosition, false);
	
    // Listen for changes on the digital and analog pins
    ofAddListener(ard.EDigitalPinChanged, this, &testApp::digitalPinChanged);
    ofAddListener(ard.EAnalogPinChanged, this, &testApp::analogPinChanged);    
}

//--------------------------------------------------------------
void testApp::updateArduino(){
    
	// update the arduino, get any data or messages.
    // the call to ard.update() is required
	ard.update();
	
	// do not send anything until the arduino has been set up
	if (bSetupArduino) {
        // fade the led connected to pin D11
		ard.sendPwm(11, (int)(128 + 128 * sin(ofGetElapsedTimef())));   // pwm...
	}
    
}

// digital pin event handler, called whenever a digital pin value has changed
// note: if an analog pin has been set as a digital pin, it will be handled
// by the digitalPinChanged function rather than the analogPinChanged function.

//--------------------------------------------------------------
void testApp::digitalPinChanged(const int & pinNum) {
    // do something with the digital input. here we're simply going to print the pin number and
    // value to the screen each time it changes
    buttonState = "digital pin: " + ofToString(pinNum) + " = " + ofToString(ard.getDigital(pinNum));
}

// analog pin event handler, called whenever an analog pin value has changed

//--------------------------------------------------------------
void testApp::analogPinChanged(const int & pinNum) {
    // do something with the analog input. here we're simply going to print the pin number and
    // value to the screen each time it changes
    potValue = "analog pin: " + ofToString(pinNum) + " = " + ofToString(ard.getAnalog(pinNum));
}
