/************************************************************
************************************************************/
#pragma once

/************************************************************
************************************************************/
#include "ofMain.h"

#include "sj_common.h"

/************************************************************
************************************************************/

/**************************************************
**************************************************/
class THREAD_BASE : public ofThread, private Noncopyable{
protected:
	/****************************************
	****************************************/
	enum{
		NUM_BUFFERS = 2,
		NUM_SAMPLES_PER_BUFFER = 1000,
	};
	
	/****************************************
	****************************************/
	float LastINT_sec;
	bool b_valid;
	
	bool b_1st;
	
	bool b_Empty[NUM_BUFFERS];
	
	int BufferId;
	int id;
	
	FILE* fp;
	FILE* fp_Rec;
	
	bool b_End;
	bool b_EOF;
	
	int t_ofs_ms;
	
	int INT_data_byKeyInput;
	
	/********************
	********************/
	int LogFile_id;
	
	
	/****************************************
	****************************************/
	/********************
	********************/
	void threadedFunction();
	bool Wait_NextBufferFilled(double timeout);
	int get_NextBufferId();
	bool ckeck_if_timeProceed();
	
	void charge(int BufferId_toCharge);
	void change_buffer_id(int t_music_ms, float ElapsedTime);
	void Reset();
	void check_if_setup_ok_in_DerivedClass();
	
	/********************
	********************/
	virtual void set_LogFile_id() = 0;
	virtual FILE* open_ScenarioFile() = 0;
	virtual FILE* open_RecFile() = 0;
	virtual void SetTime_DataToCharge(int time) = 0;
	virtual bool Set_time_and_data__DataToCharge(string& strBuf) = 0;
	virtual int GetTime_DataToCharge() = 0;
	virtual void chargeTimeTable_byCopying(int BufferId_toCharge, int Charge_id) = 0;
	virtual void set_outputData(int BufferId, int id) = 0;
	virtual void set_outputData_None() = 0;
	virtual void set_INTdata_to_outputData(int _data) = 0;
	virtual void fprintf_outputData(int t_music, FILE* fp) = 0;
	virtual int get_TimeData_from_TimeTable(int BufferId, int id) = 0;

public:	
	/****************************************
	****************************************/
	/********************
	********************/
	THREAD_BASE();
	virtual ~THREAD_BASE();
	
	/********************
	********************/
	bool IsReady();
	bool set__t_ofs_ms(int _val);
	void INT_Keyinput(int _data);
	
	void exit();
	void setup();
	void update(int t_music_ms);
};



