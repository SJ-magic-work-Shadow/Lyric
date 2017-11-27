/************************************************************
************************************************************/
#pragma once

/************************************************************
************************************************************/
#include "ofMain.h"

#include "sj_common.h"
#include "surface.h"
#include "ParamOf_StructureDesign.h"

/************************************************************
************************************************************/

/**************************************************
**************************************************/
class TEST_SURFACE{
private:
	/****************************************
	****************************************/
	ofFbo fbo;
	SURFACE surface;
	ofCamera cam;
	
	STRUCTURE_TYPE StructureType;
	int SufraceId;
	
public:
	/****************************************
	****************************************/
	TEST_SURFACE();
	~TEST_SURFACE();
	
	void setup_surface(int _SurfaceId);
	void setup();
	void update();
	void draw();
};

