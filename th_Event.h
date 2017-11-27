/************************************************************
************************************************************/
#pragma once

/************************************************************
************************************************************/
#include "ofMain.h"

#include "sj_common.h"
#include "thread_Base.h"

/************************************************************
class
************************************************************/

/**************************************************
**************************************************/
struct DATASET__EVENT{
	int EventId;
	string phrase;
	bool b_alnum;
	float t_FadeIn;
	float t_life;
	float t_FadeOut;
};
struct TIME_N_DATASET__EVENT{
	int t_music_ms;
	DATASET__EVENT DataSet;
};

/**************************************************
Singleton
	https://ja.wikipedia.org/wiki/Singleton_%E3%83%91%E3%82%BF%E3%83%BC%E3%83%B3
**************************************************/
class THREAD__EVENT_TIMETABLE : public THREAD_BASE
{
private:
	/****************************************
	****************************************/
	TIME_N_DATASET__EVENT *TimeTable[NUM_BUFFERS]; 
	TIME_N_DATASET__EVENT data_to_output;
	TIME_N_DATASET__EVENT data_to_charge;
	
	/****************************************
	function
	****************************************/
	/********************
	singleton
	********************/
	THREAD__EVENT_TIMETABLE();
	~THREAD__EVENT_TIMETABLE();
	THREAD__EVENT_TIMETABLE(const THREAD__EVENT_TIMETABLE&); // Copy constructor. no define.
	THREAD__EVENT_TIMETABLE& operator=(const THREAD__EVENT_TIMETABLE&); // コピー代入演算子. no define.
	
	/********************
	********************/
	void set_LogFile_id();
	FILE* open_ScenarioFile();
	FILE* open_RecFile();
	void SetTime_DataToCharge(int time);
	bool Set_time_and_data__DataToCharge(string& strBuf);
	int GetTime_DataToCharge();
	void chargeTimeTable_byCopying(int BufferId_toCharge, int Charge_id);
	void set_outputData(int BufferId, int id);
	void set_outputData_None();
	int get_TimeData_from_TimeTable(int BufferId, int id);
	void set_INTdata_to_outputData(int _data);
	void fprintf_outputData(int t_music, FILE* fp);
	
	/********************
	********************/
	void phrase_isAlnum();
	void del_NotPrimitiveLetter(string& str);
	
public:
	/****************************************
	****************************************/
	/********************
	********************/
	static THREAD__EVENT_TIMETABLE* getInstance(){
		static THREAD__EVENT_TIMETABLE inst;
		return &inst;
	}
	
	void getLatestOutput(DATASET__EVENT& Dataset);
};

