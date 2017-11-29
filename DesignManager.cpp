/************************************************************
************************************************************/
#include "DesignManager.h"

/************************************************************
************************************************************/

/******************************
******************************/
DESIGN_MANAGER::DESIGN_MANAGER()
: t_Duration_CamParamChange(0.6)
, t_LastMusic(-1)
, t_ofs_ms(0)
, thread_TimeTable(THREAD__EVENT_TIMETABLE::getInstance())
{
}

/******************************
******************************/
DESIGN_MANAGER::~DESIGN_MANAGER()
{
}


/******************************
******************************/
void DESIGN_MANAGER::exit()
{
	/********************
	********************/
	music.stop();
	music.unloadSound();
	
	/********************
	********************/
	thread_TimeTable->exit();
	try{
		/********************
		stop済みのthreadをさらにstopすると、Errorが出るようだ。
		********************/
		if(thread_TimeTable->isThreadRunning()){
			thread_TimeTable->stopThread();
			while(thread_TimeTable->isThreadRunning()){
				thread_TimeTable->waitForThread(true);
			}
		}
		
	}catch(...){
		printf("Thread exiting Error\n");
	}
}

/******************************
******************************/
void DESIGN_MANAGER::set__Music_SeekTo_ms(int temp_t_SeekTo_ms)
{
	double pos = music.getPosition();
	int pos_ms = music.getPositionMS();
	
	if(pos <= 0){
		printf("> Try seek failed : please try again\n");
		return;
		
	}else{
		const int TotalLength = int(pos_ms / pos);
		const int MARGIN_MS = 5000;
		
		if( (temp_t_SeekTo_ms < 0) || (TotalLength < temp_t_SeekTo_ms + MARGIN_MS) ){
			printf("> Try seek : out of range\n");
			
		}else{
			t_ofs_ms = temp_t_SeekTo_ms;
			printf("> Try seek OK\n");
		}
		
		return;
	}
}

/******************************
******************************/
void DESIGN_MANAGER::PlayAndRec_operation(int EventId)
{
	thread_TimeTable->lock();
	thread_TimeTable->INT_Keyinput(EventId);
	thread_TimeTable->unlock();
}

/******************************
******************************/
void DESIGN_MANAGER::setup()
{
	for(int i = 0; i < NUM_SURFACES; i++){
		fbo[i].allocate(WIDTH, HEIGHT, GL_RGBA);
	}
	
	if(!music.isLoaded()){
		music.loadSound("music.mp3");
		if(!music.isLoaded()) { ERROR_MSG(); std::exit(1); }
		
		music.setVolume(1.0);
		music.setLoop(true);
		music.setMultiPlay( true );
		// music.setSpeed( 1.0 );
	}
	
	DATASET__EVENT temp = {EVENT__NONE/* no use */, " ", 1.0/* t_FadeIn */, 5/* t_life */, 1.0/* t_FadeOut */};
	ReStart(temp);
}

/******************************
******************************/
void DESIGN_MANAGER::ReStart(DATASET__EVENT& TimeTable_Dataset_Event)
{
	/********************
	********************/
	thread_TimeTable->setup();
	thread_TimeTable->set__t_ofs_ms(t_ofs_ms);
	thread_TimeTable->startThread(true, false); // blocking, non verboss
	
	while(!thread_TimeTable->IsReady()){
		// wait for Ready.
	}
	
	/********************
	********************/
	ResetDesign(TimeTable_Dataset_Event);
	
	/********************
	********************/
	music.play();
	music.setPositionMS(t_ofs_ms);
	
	t_ofs_ms = 0;
	t_LastMusic = -1;
}

/******************************
******************************/
void DESIGN_MANAGER::ResetDesign(DATASET__EVENT& TimeTable_Dataset_Event)
{
	Dice_StructureDesignType();
	Dice_AnimVariation_Devview();
	Dice_IdOrder();
	
	for(int i = 0; i < NUM_SURFACES; i++){
		surface[i].disable();
	}
	
	Apply_LastSurface(TimeTable_Dataset_Event);
	Apply_AllCamParam();
}

/******************************
******************************/
int DESIGN_MANAGER::update()
{
	/********************
	********************/
	ofSoundUpdate();
	
	/********************
	********************/
	int t_music = music.getPositionMS();
	if( (t_music < t_LastMusic) || (t_ofs_ms != 0) ){
		music.stop();
		thread_TimeTable->stopThread();
		thread_TimeTable->waitForThread(true);
		
		DATASET__EVENT temp = {EVENT__NONE/* no use */, " ", 1.0/* t_FadeIn */, 5/* t_life */, 1.0/* t_FadeOut */};
		ReStart(temp);
	}else{
		t_LastMusic = t_music;
	}
	
	/********************
	********************/
	DATASET__EVENT Latest_Event_And_Dataset;
	get_EventAndDataset_TimeTableScenario(t_music, Latest_Event_And_Dataset);
	
	if(Latest_Event_And_Dataset.EventId == int(EVENT__OVERWRITE)){
		Slide_surface();
		Apply_LastSurface(Latest_Event_And_Dataset);
		
	}else if(Latest_Event_And_Dataset.EventId == int(EVENT__CUT_CHANGE)){
		increment__id_inOrderList();
		Slide_surface();
		Apply_LastSurface(Latest_Event_And_Dataset);
		Apply_AllCamParam();
		
	}else if(Latest_Event_And_Dataset.EventId == int(EVENT__CAM_WORK)){
		increment__id_inOrderList();
		Slide_surface();
		Apply_LastSurface(Latest_Event_And_Dataset);
		Apply_CamParam_for_CamWork();
		
	}else if(Latest_Event_And_Dataset.EventId == int(EVENT__CHANGE_STRUCTURE)){
		ResetDesign(Latest_Event_And_Dataset);
	}
	
	/********************
	********************/
	for(int i = 0; i < NUM_SURFACES; i++){
		surface[i].update(fbo[i]);
	}
	
	/********************
	********************/
	return t_LastMusic;
}


/******************************
******************************/
void DESIGN_MANAGER::draw(float gui_TotalAlpha)
{
	cal_CamParamCurrent();
	
	set_CamParamCurrent();
	
	cam.begin();
	for(int i = 0; i < NUM_SURFACES; i++){
		surface[i].draw(gui_TotalAlpha, fbo[i]);
	}
	cam.end();
}

/******************************
******************************/
void DESIGN_MANAGER::set_CamParamCurrent()
{
	cam.setPosition(CamParam_current.CamPosition);
	cam.lookAt( CamParam_current.CamLookAt, CamParam_current.CamUpper);
	cam.setFov(CamParam_current.CamAngle);
	
	cam.setNearClip(1);
	cam.setFarClip(2000);
}

/******************************
******************************/
void DESIGN_MANAGER::cal_CamParamCurrent()
{
	float dt = ofGetElapsedTimef() - t_CamParamChangeFrom;
	
	if(dt < t_Duration_CamParamChange){
		/********************
		CamPosition
		********************/
		if( (StructureType == STRUCTURE_TYPE__DEVVIEW) && (AnimVariation_Devview == ANIM_VARIATION__DEVVIEW__CAMPOS_FAST) ){
			CamParam_current.CamPosition = CamParam_to.CamPosition; // すぐ移動完了.
		}else{
			CamParam_current.CamPosition = CamParam_from.CamPosition + (CamParam_to.CamPosition - CamParam_from.CamPosition) * dt/t_Duration_CamParamChange;
		}
		
		/********************
		CamLookAt
		********************/
		if( (StructureType == STRUCTURE_TYPE__DEVVIEW) && (AnimVariation_Devview == ANIM_VARIATION__DEVVIEW__TARGET_FAST) ){
			CamParam_current.CamLookAt = CamParam_to.CamLookAt; // すぐ移動完了.
		}else{
			CamParam_current.CamLookAt = CamParam_from.CamLookAt + (CamParam_to.CamLookAt - CamParam_from.CamLookAt) * dt/t_Duration_CamParamChange;
		}
		
		/********************
		CamAngle
		********************/
		CamParam_current.CamAngle = CamParam_from.CamAngle + (CamParam_to.CamAngle - CamParam_from.CamAngle) * dt/t_Duration_CamParamChange;
		
		/********************
		CamUpper
		********************/
		if(CamParam_from.CamUpper == CamParam_to.CamUpper){
			CamParam_current.CamUpper = CamParam_to.CamUpper;
		}else{
			float d_deg = CamParam_from.CamUpper.angle(CamParam_to.CamUpper)/t_Duration_CamParamChange;
			ofVec3f vec_along = CamParam_from.CamUpper.getCrossed(CamParam_to.CamUpper);
			if(vec_along.length() == 0) vec_along = ofVec3f(0, 0, 1);
			CamParam_current.CamUpper = CamParam_from.CamUpper.rotate(d_deg * dt, vec_along);
		}
		
	}else{
		CamParam_current = CamParam_to;
	}
}

/******************************
******************************/
void DESIGN_MANAGER::increment__id_inOrderList()
{
	const int Last_id_inTheStructure = id_Order[id_inOrderList];
	
	id_inOrderList++;
	if(NUM_SURFACES <= id_inOrderList) id_inOrderList = 0;
	
	const int Next_id_inTheStructure = id_Order[id_inOrderList];
	if(Is_OppositeSurface(Last_id_inTheStructure, Next_id_inTheStructure)){
		id_inOrderList++;
		if(NUM_SURFACES <= id_inOrderList) id_inOrderList = 0;
	}
}

/******************************
******************************/
bool DESIGN_MANAGER::Is_OppositeSurface(const int Last_id_inTheStructure, const int Next_id_inTheStructure)
{
	/********************
	********************/
	if(StructureType != STRUCTURE_TYPE__CUBE) return false;
	
	/********************
	********************/
	switch(Last_id_inTheStructure){
		case 0:
			if(Next_id_inTheStructure == 1)	return true;
			else							return false;
			break;
			
		case 1:
			if(Next_id_inTheStructure == 0)	return true;
			else							return false;
			break;
			
		case 2:
			if(Next_id_inTheStructure == 3)	return true;
			else							return false;
			break;
			
		case 3:
			if(Next_id_inTheStructure == 2)	return true;
			else							return false;
			break;
			
		case 4:
			if(Next_id_inTheStructure == 5)	return true;
			else							return false;
			break;
			
		case 5:
			if(Next_id_inTheStructure == 4)	return true;
			else							return false;
			break;
			
		default:
			return false;
			break;
	}
}

/******************************
******************************/
void DESIGN_MANAGER::Slide_surface()
{
	for(int i = 0; i < NUM_SURFACES - 1; i++){
		surface[i] = surface[i + 1];
	}
}

/******************************
******************************/
void DESIGN_MANAGER::Apply_LastSurface(DATASET__EVENT& TimeTable_Dataset_Event)
{
	const int id_inTheStructure = id_Order[id_inOrderList];
	surface[NUM_SURFACES - 1].set(	fbo[NUM_SURFACES - 1],
									TimeTable_Dataset_Event.phrase,
									TimeTable_Dataset_Event.b_alnum,
									ParamOfStructureDesign[StructureType][id_inTheStructure].Surface_DispCoordInfo, 
									TimeTable_Dataset_Event.t_FadeIn,
									TimeTable_Dataset_Event.t_life,
									TimeTable_Dataset_Event.t_FadeOut
									);
}

/******************************
******************************/
void DESIGN_MANAGER::Apply_AllCamParam()
{
	const int id_inTheStructure = id_Order[id_inOrderList];
	
	CamParam_from		= ParamOfStructureDesign[StructureType][id_inTheStructure].CamParam;
	CamParam_to			= ParamOfStructureDesign[StructureType][id_inTheStructure].CamParam;
	CamParam_current	= ParamOfStructureDesign[StructureType][id_inTheStructure].CamParam;
	
	t_CamParamChangeFrom = ofGetElapsedTimef();
}

/******************************
******************************/
void DESIGN_MANAGER::Apply_CamParam_for_CamWork()
{
	/********************
	********************/
	const int id_inTheStructure = id_Order[id_inOrderList];
	
	CamParam_from	= CamParam_current;
	CamParam_to		= ParamOfStructureDesign[StructureType][id_inTheStructure].CamParam;
	
	/********************
	"ANIM_VARIATION__DEVVIEW__CAMPOS_FAST"の時、最後の所で"クッ"とした動きが気になったので、
	cam angleもanimationすることで、柔らかい動きにしてみた.
	********************/
	if( (StructureType == STRUCTURE_TYPE__DEVVIEW) && (AnimVariation_Devview == ANIM_VARIATION__DEVVIEW__CAMPOS_FAST) ){
		CamParam_from.CamAngle = CamParam_to.CamAngle * 2.5;
	}
		
	/********************
	********************/
	t_CamParamChangeFrom = ofGetElapsedTimef();
}

/******************************
******************************/
void DESIGN_MANAGER::get_EventAndDataset_TimeTableScenario(int t_music, DATASET__EVENT& Latest_Event_And_Dataset)
{
	thread_TimeTable->update(t_music);
	((THREAD__EVENT_TIMETABLE*)thread_TimeTable)->getLatestOutput(Latest_Event_And_Dataset);
}

/******************************
description
	fisher yates法
	偏りをなくすため、回を追うごとに乱数範囲を狭めていくのがコツ
	http://www.fumiononaka.com/TechNotes/Flash/FN0212003.html
******************************/
void DESIGN_MANAGER::Dice_IdOrder()
{
	/********************
	********************/
	for(int i = 0; i < NUM_SURFACES; i++){
		id_Order[i] = i;
	}
	
	/********************
	********************/
	int i = NUM_SURFACES;
	
	while(i--){
		// int j = rand() % (i + 1);
		int j = (int)( ((double)rand() / ((double)RAND_MAX + 1)) * (i + 1) );
		
		int temp = id_Order[i];
		id_Order[i] = id_Order[j];
		id_Order[j] = temp;
	}
	
	/********************
	********************/
	id_inOrderList = 0;
}

/******************************
******************************/
int DESIGN_MANAGER::Dice_StructureDesignType()
{
	int Weight[NUM_STRUCTURE_TYPES];
	
	Weight[STRUCTURE_TYPE__LAYER] = 1;
	Weight[STRUCTURE_TYPE__DEVVIEW] = 1;
	Weight[STRUCTURE_TYPE__CUBE] = 1;
	
	StructureType = (STRUCTURE_TYPE)Dice_index(Weight, NUM_STRUCTURE_TYPES);
}

/******************************
******************************/
int DESIGN_MANAGER::Dice_AnimVariation_Devview()
{
	int Weight[NUM_ANIM_VARIATION__DEVVIEW];
	
	Weight[ANIM_VARIATION__DEVVIEW__STATIC] = 1;
	Weight[ANIM_VARIATION__DEVVIEW__TARGET_FAST] = 1;
	Weight[ANIM_VARIATION__DEVVIEW__CAMPOS_FAST] = 1;
	
	AnimVariation_Devview = (ANIM_VARIATION__DEVVIEW)Dice_index(Weight, NUM_ANIM_VARIATION__DEVVIEW);
}

/******************************
******************************/
int DESIGN_MANAGER::Dice_index(int *Weight, int NUM)
{
	/***********************
	cal sum of Weight
	***********************/
	int TotalWeight = 0;
	int i;
	for(i = 0; i < NUM; i++){
		TotalWeight += Weight[i];
	}
	
	if(TotalWeight == 0) { ERROR_MSG(); std::exit(1); }
	
	/***********************
	random number
	***********************/
	// int RandomNumber = rand() % TotalWeight;
	int RandomNumber = (int)( ((double)rand() / ((double)RAND_MAX + 1)) * (TotalWeight) );
	
	/***********************
	define play mode
	***********************/
	int index;
	for(TotalWeight = 0, index = 0; /* */; index++){
		TotalWeight += Weight[index];
		if(RandomNumber < TotalWeight) break;
	}
	
	return index;
}


