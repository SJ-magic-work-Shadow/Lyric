#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( int argc, char** argv  ){
	ofSetupOpenGL(1024,768,OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	
	if(argc < 4){
		printf("> exe mode textColor IsSimpleFont\n");
		printf("mode\n");
		printf("\t0:BOOT_MODE__REC\n");
		printf("\t1:BOOT_MODE__PLAY\n");
		printf("\n\n");
		
		printf("TextColor\n");
		printf("\t0:white\n");
		printf("\t1:black\n");
		printf("\n\n");
		
		printf("IsSimpleFont\n");
		printf("\t0:no\n");
		printf("\t1:yes\n");
		printf("\n");
		
	}else{
		int mode = atoi(argv[1]);
		int textCol = atoi(argv[2]);
		bool b_IsSimpleFont = atoi(argv[3]);
		
		ofRunApp(new ofApp(mode, textCol, b_IsSimpleFont));
	}
}
