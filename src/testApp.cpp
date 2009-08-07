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
#include <time.h>

#define		kTitleString	"UDP->TCP Bridge v0.2"
#define		kTextColor		0xAAAAAA
#define		kTextPosX		40

// vars
string		infoString;
string		helpString;
string		dataString;
int			bufferSize;
bool		doDebug;
bool		isIdle			= false;

// history stuff
#define		kHistorySize	500
long		history[kHistorySize];
int			historyHead		= 0;	// current head of circular buffer


ofxTCPServer	TCPServer;
ofxUDPManager	UDPReceiver;

ofxXmlSettings	XML;



void setDebug(bool b) {
	doDebug = b;
	helpString = string("Press 'd' to toggle debug info (check console for more) | ") + (doDebug ? "ON" : "OFF");
}

void printTime(string &s) {
	static time_t rawtime;
	static struct tm * timeinfo;
	static char szTime [80];
	time ( &rawtime );
	timeinfo = localtime ( &rawtime );
	strftime(szTime, 80,"%x %X | ",timeinfo);
	s += string(szTime);
	s += ofToString(ofGetElapsedTimef(), 3) + " : ";	
}


//--------------------------------------------------------------
void testApp::setup(){
	XML.loadFile("settings.xml");
	int TCPPort	= XML.getValue("TCP", 3000);
	int UDPPort	= XML.getValue("UDP", 3333);
	int	freq	= XML.getValue("Frequency", 60);
	
	infoString = string(kTitleString) + " by Mehmet Akten (c) 2009 | www.memo.tv\n\nlistening on UDP port: " + ofToString(UDPPort) + "\nsending on TCP port: " + ofToString(TCPPort) + "\nat " + ofToString(freq) +" Hz";
	
	ofSetVerticalSync(false);
	ofSetFrameRate(freq);
	ofBackground(0, 0, 0);
	ofSetWindowTitle(kTitleString);
	ofSetColor(kTextColor);
	glLineWidth(3);
	
	TCPServer.setup(TCPPort);
	
	UDPReceiver.Create();
	UDPReceiver.Bind(UDPPort);
	UDPReceiver.SetNonBlocking(true);
	
	setDebug(false);
}


//--------------------------------------------------------------
void testApp::update() {
	
	static char data[65536];
	bufferSize = UDPReceiver.Receive(data, 65536);
	
	if(bufferSize>0) {
		isIdle = false;
		TCPServer.sendRawBytesToAll(data, bufferSize);
	}
	
	
	if(doDebug) {
		//printf("%i %i\n", historyHead, bufferSize);
		history[historyHead] = bufferSize;
		if(++historyHead >= kHistorySize) historyHead = 0;
		
		if(bufferSize>0) {
			dataString = "";
			printTime(dataString);
			dataString += "sending " + ofToString(bufferSize) + " bytes\n";
			
			for(int i=0; i<bufferSize; i++) {
				char c = data[i];
				printf("%i %c=%i\n", i, c, c);
			}
			printf("%i bytes ----------------- \n\n", bufferSize);
		} else {
			if(!isIdle) {
				printTime(dataString);
				dataString += "no data received";
				isIdle = true;
			}
		}
	}
}


//--------------------------------------------------------------
void testApp::draw() {
	if(doDebug) {
		glColor3f(0, 0.3f, 0);
		glPushMatrix();
		glScalef(ofGetWidth() * 1.0f/kHistorySize, 1, 1);
		for(int i=0; i<kHistorySize; i++) {
			float value = history[(i+historyHead)%kHistorySize];
			glBegin(GL_LINES);
			glVertex2f(i, ofGetHeight());
			glVertex2f(i, ofGetHeight()-value/2);
			glEnd();
		}
		glPopMatrix();
		
		ofSetColor(kTextColor);
		ofDrawBitmapString(infoString + " | actual : " + ofToString(ofGetFrameRate(), 2) + " Hz", kTextPosX, 40); 
		ofDrawBitmapString(dataString, kTextPosX, 140);
	} else {
		ofSetColor(kTextColor);
		ofDrawBitmapString(infoString, kTextPosX, 40); 
	}
	
	ofDrawBitmapString(helpString, kTextPosX, 120);

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	switch(key) {
		case 'd':
		case 'D':
			setDebug(!doDebug);
			break;
	}
}
