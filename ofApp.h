/************************************************************
************************************************************/
#pragma once

/************************************************************
************************************************************/
#include "ofMain.h"
#include "ofxSyphon.h"
#include "ofxGui.h"

#include "sj_common.h"
#include "test_surface.h"
#include "DesignManager.h"

#if (BUILD_SW == CHECK__PARAMOF_STRUCTURE_DESIGN)
void ParamOfStructureDesign_setup();
void ParamOfStructureDesign_draw();
void ParamOfStructureDesign_set_SurfaceId(int _id);
#endif

/************************************************************
************************************************************/

class ofApp : public ofBaseApp{
private:
	/****************************************
	****************************************/
	
	/********************
	********************/
	DESIGN_MANAGER* DesignManager;
	ofxSyphonServer mainOutputSyphonServer;
	
	
	/********************
	********************/
	ofxPanel gui;
	
	ofxFloatSlider gui_TotalAlpha;
	
	bool b_DispGui;
	
	int t_Music;
	
	/********************
	********************/
#if (BUILD_SW == CHECK__SURFACE)
	TEST_SURFACE TestSurface;
#endif

	/****************************************
	****************************************/
	void draw_time(double FrameRate);
	
public:
	/****************************************
	****************************************/
	
	/********************
	********************/
	ofApp(int _BootMode, int _TextCol, bool _b_IsSimpleFont);
	~ofApp();
	void exit();
	
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
	
};
