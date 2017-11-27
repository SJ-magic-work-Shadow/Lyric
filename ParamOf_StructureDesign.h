/************************************************************
How to check if all the parameters are correct
	#define BUILD_SW CHECK__PARAMOF_STRUCTURE_DESIGN // sj_common.h
	
	check if Surface_DispCoordInfo is correct
		enable easycam in ParamOfStructureDesign_draw()
		manually change "StructureType" in ParamOfStructureDesign_draw().
		press 0-5 to change surface.
		
		easy cam has no clip info, so you can see if each surface is set correctly.
		
		
	check if CamParam is correct
		now, you can say Surface_DispCoordInfo is correct.
		Then, please change from easy cam to cam.
		
		And manually change "StructureType" in ParamOfStructureDesign_draw().
		press 0-5 to change surface.
		
		Surface_DispCoordInfo is correct,
		so when you can see each surface at the size which you want,
		then you can say CamParam is correct.
************************************************************/
#pragma once

/************************************************************
************************************************************/
#include "ofMain.h"
#include "sj_common.h"


/************************************************************
************************************************************/
struct CAM_PARAM{
	ofVec3f CamPosition;
	ofVec3f CamLookAt;
	ofVec3f CamUpper;
	float CamAngle;
};

struct SURFACE__DISPCOORD_INFO{
	ofVec3f pos;
	ofVec3f Rotate_vec;
	float Rotate_deg;
};

struct PARAM_OF_STRUCTURE_DESIGN{
	SURFACE__DISPCOORD_INFO Surface_DispCoordInfo;
	CAM_PARAM CamParam;
};

/************************************************************
************************************************************/
enum STRUCTURE_TYPE{
	STRUCTURE_TYPE__LAYER,
	STRUCTURE_TYPE__DEVVIEW,
	STRUCTURE_TYPE__CUBE,
	
	NUM_STRUCTURE_TYPES,
};

enum ANIM_VARIATION__DEVVIEW{
	ANIM_VARIATION__DEVVIEW__STATIC,
	ANIM_VARIATION__DEVVIEW__TARGET_FAST,
	ANIM_VARIATION__DEVVIEW__CAMPOS_FAST,
	
	NUM_ANIM_VARIATION__DEVVIEW,
};

enum{
	NUM_SURFACES = 6,
};

	
/************************************************************
************************************************************/
extern PARAM_OF_STRUCTURE_DESIGN ParamOfStructureDesign[NUM_STRUCTURE_TYPES][NUM_SURFACES];


