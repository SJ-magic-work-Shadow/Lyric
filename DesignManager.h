/************************************************************
************************************************************/
#pragma once

/************************************************************
************************************************************/
#include "ofMain.h"

#include "sj_common.h"
#include "ParamOf_StructureDesign.h"
#include "surface.h"
#include "th_Event.h"

/************************************************************
************************************************************/

/**************************************************
**************************************************/
class DESIGN_MANAGER{
public:
	/****************************************
	****************************************/
	enum EVENT{
		EVENT__NONE,
		EVENT__OVERWRITE,
		EVENT__CUT_CHANGE,
		EVENT__CAM_WORK,
		EVENT__CHANGE_STRUCTURE,
		
		NUM_EVENTS,
	};
	
private:
	/********************
	singleton
	********************/
	DESIGN_MANAGER();
	~DESIGN_MANAGER();
	DESIGN_MANAGER(const DESIGN_MANAGER&); // Copy constructor. no define.
	DESIGN_MANAGER& operator=(const DESIGN_MANAGER&); // コピー代入演算子. no define.
	
private:
	/****************************************
	****************************************/
	THREAD_BASE* thread_TimeTable;
	
	ofFbo fbo[NUM_SURFACES];
	SURFACE surface[NUM_SURFACES];
	ofCamera cam;
	
	int t_ofs_ms;
	
	int id_Order[NUM_SURFACES];
	int id_inOrderList;
	
	CAM_PARAM CamParam_from;
	CAM_PARAM CamParam_to;
	CAM_PARAM CamParam_current;
	
	float t_CamParamChangeFrom;
	const float t_Duration_CamParamChange;
	
	STRUCTURE_TYPE StructureType;
	ANIM_VARIATION__DEVVIEW AnimVariation_Devview;
	
	ofSoundPlayer music;
	int t_LastMusic;
	
	/****************************************
	****************************************/
	void Dice_IdOrder();
	int Dice_StructureDesignType();
	int Dice_AnimVariation_Devview();
	int Dice_index(int *Weight, int NUM);
	void get_EventAndDataset_TimeTableScenario(int t_music, DATASET__EVENT& Latest_Event_And_Dataset);
	void Slide_surface();
	void Apply_LastSurface(DATASET__EVENT& TimeTable_Dataset_Event);
	void Apply_AllCamParam();
	void Apply_CamParam_for_CamWork();
	void increment__id_inOrderList();
	bool Is_OppositeSurface(const int Last_id_inTheStructure, const int Next_id_inTheStructure);
	void set_CamParamCurrent();
	void cal_CamParamCurrent();
	void ReStart(DATASET__EVENT& TimeTable_Dataset_Event);
	void ResetDesign(DATASET__EVENT& TimeTable_Dataset_Event);
	
public:
	/****************************************
	****************************************/
	/********************
	********************/
	static DESIGN_MANAGER* getInstance(){
		static DESIGN_MANAGER inst;
		return &inst;
	}
	
	/********************
	********************/
	void set__Music_SeekTo_ms(int temp_t_SeekTo_ms);
	void PlayAndRec_operation(int EventId);
	
	void exit();
	void setup();
	void update();
	void draw(float gui_TotalAlpha);
};
