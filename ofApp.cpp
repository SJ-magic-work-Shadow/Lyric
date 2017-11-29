/************************************************************
************************************************************/
#include "ofApp.h"
#include <time.h>


/************************************************************
************************************************************/

/******************************
******************************/
ofApp::ofApp(int _BootMode, int _TextCol, bool _b_IsSimpleFont)
: DesignManager(DESIGN_MANAGER::getInstance())
, b_DispGui(true)
{
	srand((unsigned) time(NULL));
	
	BootMode = BOOT_MODE(_BootMode);
	TextColor = TEXT_COLOR(_TextCol);
	b_IsSimpleFont = _b_IsSimpleFont;
	
	fopen_LogFile();
}

/******************************
******************************/
ofApp::~ofApp()
{
	fclose_LogFile();
}

/******************************
******************************/
void ofApp::exit()
{
#if (BUILD_SW == BUILD_RELEASE)
	DesignManager->exit();
#endif
	
	printf("\n> Good bye\n");
}

//--------------------------------------------------------------
void ofApp::setup(){
	/********************
	********************/
	ofSetWindowTitle("Lyric");
	ofSetVerticalSync(true);
	ofSetFrameRate(60);
	ofSetWindowShape(WIDTH, HEIGHT);
	ofSetEscapeQuitsApp(false);
	
	ofEnableAlphaBlending();
	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	// ofEnableBlendMode(OF_BLENDMODE_ADD);
	// ofEnableSmoothing();
	
	/********************
	********************/
	gui.setup();
	
	gui.add(gui_TotalAlpha.setup("Total Alpha", 255, 0, 255));
	
	
	/********************
	********************/
#if (BUILD_SW == CHECK__PARAMOF_STRUCTURE_DESIGN)
	ParamOfStructureDesign_setup();
	
#elif (BUILD_SW == CHECK__SURFACE)
	TestSurface.setup();
	
#else

	DesignManager->setup();
	mainOutputSyphonServer.setName("Screen Output"); // server name
	
#endif
}

//--------------------------------------------------------------
void ofApp::update(){
#if (BUILD_SW == CHECK__PARAMOF_STRUCTURE_DESIGN)

#elif (BUILD_SW == CHECK__SURFACE)
	TestSurface.update();
	
#else
	t_Music = DesignManager->update();
	
#endif
}

//--------------------------------------------------------------
void ofApp::draw(){
#if (BUILD_SW == CHECK__PARAMOF_STRUCTURE_DESIGN)

	ofBackground(0, 0, 0, 0);
	ParamOfStructureDesign_draw();
	
#elif (BUILD_SW == CHECK__SURFACE)
	ofBackground(0, 0, 0, 0);
	TestSurface.draw();
	
#else

	/********************
	********************/
	ofBackground(0, 0, 0, 0);
	
	ofPushStyle();
	DesignManager->draw(gui_TotalAlpha);
	ofPopStyle();
	
	/********************
	publish
	********************/
	mainOutputSyphonServer.publishScreen();
	
	/********************
	show gui after publishing.
	********************/
	if(b_DispGui){
		gui.draw();
		draw_time(ofGetFrameRate());
	}
	
#endif
}

/******************************
******************************/
void ofApp::draw_time(double FrameRate)
{
	/********************
	********************/
	char buf[BUF_SIZE];
	
	int min	= t_Music / 1000 / 60;
	int sec	= t_Music / 1000 - min * 60;
	int ms	= t_Music % 1000;
	
	sprintf(buf, "%6d:%6d:%6d\n%7.2f", min, sec, ms, FrameRate);
	
	
	/********************
	********************/
	ofSetColor(255, 255, 255);
	ofDrawBitmapString(buf, 300, 20);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
#if (BUILD_SW == CHECK__PARAMOF_STRUCTURE_DESIGN)

	int id = key - '0';
	if((0 <= id) && (id <= 9)){
		ParamOfStructureDesign_set_SurfaceId(id);
	}
	
#elif (BUILD_SW == CHECK__SURFACE)
	int id = key - '0';
	if((0 <= id) && (id <= 9)){
		TestSurface.setup_surface(id);
	}

#else
	
	/********************
	********************/
	enum STATE{
		STATE__NONE,
		STATE__SEEK_INPUT,
	};
	
	enum{
		BUF_SIZE = 100,
	};
	
	static STATE state = STATE__NONE;
	static int input[BUF_SIZE];
	static int index = 0;
	
	
	/********************
	********************/
	if(state == STATE__SEEK_INPUT){
		/********************
		********************/
		switch(key){
			case 'k':
				state = STATE__NONE;
				printf("> Cancel to input SeekTarget\n");
				break;

			case OF_KEY_RETURN:
				index--;
				
				if(index < 0){
					// nothing.
				}else{
					int input_val = 0;
					int i = 0;
					
					while(0 <= index){
						input_val += input[i] * pow(10, index);
						
						i++;
						index--;
					}
					
					DesignManager->set__Music_SeekTo_ms(input_val * 1000);
				}
				state = STATE__NONE;
				
				break;
				
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				if(index < BUF_SIZE){
					input[index] = key - '0';
					index++;
				}
				printf("%d", key - '0');
				fflush(stdout);
				break;
		}
		
	}else{
		/********************
		********************/
		switch(key){
			case 'd':
				b_DispGui = !b_DispGui;
				break;
				
			case 'k':
				state = STATE__SEEK_INPUT;
				index = 0;
				
				printf("> input SeekTarget[sec]\n");
				break;
				
			case ' ':
				DesignManager->PlayAndRec_operation(DESIGN_MANAGER::EVENT__CAM_WORK);
				break;
				
			case 't':
			case 'y':
				DesignManager->PlayAndRec_operation(DESIGN_MANAGER::EVENT__OVERWRITE);
				break;
				
			case 'f':
				DesignManager->PlayAndRec_operation(DESIGN_MANAGER::EVENT__CUT_CHANGE);
				break;
				
			case 'j':
				DesignManager->PlayAndRec_operation(DESIGN_MANAGER::EVENT__CHANGE_STRUCTURE);
				break;
		}
	}
	
#endif	
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
