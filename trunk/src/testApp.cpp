/***********************************************************************
 Copyright (c) 2008, 2009, Memo Akten, www.memo.tv
 *** The Mega Super Awesome Visuals Company ***

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 
 ************************************************************************/ 

#include "testApp.h"
#include "ofxXmlSettings.h"


int TCPPort;
int UDPPort;
bool doDump		= false;

string infoString = "UDP->TCP Bridge by Mehmet Akten (c) 2009";
string dataString;
string version = "v0.11";
int bufferSize;

ofxTCPServer TCPServer;
ofxUDPManager UDPReceiver;

ofxXmlSettings XML;


//--------------------------------------------------------------
void testApp::setup(){
	ofSetFrameRate(60);
	ofBackground(40, 40, 40);
	ofSetWindowTitle("UDP->TCP Bridge " + version);
	ofSetColor(0xAAAAAA);
	
	XML.loadFile("settings.xml");
	TCPPort	= XML.getValue("TCP", 3000);
	UDPPort	= XML.getValue("UDP", 3333);
	infoString += " " + version + "\n\nListening on UDP port: " + ofToString(UDPPort) + "\nSending on TCP port: " + ofToString(TCPPort) + "\n\nPress 'd' to toggle debug info";

	TCPServer.setup(TCPPort);
	
	UDPReceiver.Create();
	UDPReceiver.Bind(UDPPort);
	UDPReceiver.SetNonBlocking(true);
}

//--------------------------------------------------------------
void testApp::update() {
	static char data[100000];
	bufferSize = UDPReceiver.Receive(data, 100000);
	
	if(bufferSize>0) {
		dataString = "sending " + ofToString(bufferSize) + " bytes\n";
		TCPServer.sendRawBytesToAll(data, bufferSize);
		if(doDump) {
			for(int i=0; i<bufferSize; i++) {
				if(i%10 == 0) dataString += "\n";
				char c = data[i];
//				printf("%i %c=%i\n", i, c, c);
				dataString += ofToString(c) + ":" + c + " ";
			}
//			printf("%i bytes ----------------- \n\n", bufferSize);
		}
	} else {
		dataString = "no data received.";
	}
}

//--------------------------------------------------------------
void testApp::draw() {
	ofDrawBitmapString(infoString, 20, 30); 
	ofDrawBitmapString(dataString, 20, 120);
}


//--------------------------------------------------------------
void testApp::keyPressed(int key){
	switch(key) {
		case 'd':
		case 'D':
			doDump ^= true;
	}
}
