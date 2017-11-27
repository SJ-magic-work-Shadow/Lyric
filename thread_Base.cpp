/************************************************************
************************************************************/
#include "thread_Base.h"


/************************************************************
************************************************************/

/******************************
******************************/
THREAD_BASE::THREAD_BASE()
: fp(NULL)
, fp_Rec(NULL)
, LogFile_id(THREAD_TIMETABLE__EVENT)
, INT_data_byKeyInput(-1)
, b_1st(true)
, LastINT_sec(-1)
, b_valid(false)
{
}

/******************************
******************************/
THREAD_BASE::~THREAD_BASE()
{
	if(fp)		fclose(fp);
	if(fp_Rec)	fclose(fp_Rec);
}

/******************************
******************************/
void THREAD_BASE::threadedFunction()
{
	while(isThreadRunning()) {
		bool b_EOF_Copy;
		bool b_Empty_copy[NUM_BUFFERS];
		
		lock();
		for(int i = 0; i < NUM_BUFFERS; i++){
			b_Empty_copy[i] = b_Empty[i];
		}
		b_EOF_Copy = b_EOF;
		unlock();
		
		for(int i = 0; i < NUM_BUFFERS; i++){
			if(!b_EOF_Copy && b_Empty_copy[i]){
				charge(i);
			}
		}
		if(b_1st){
			lock();
			if(ckeck_if_timeProceed() == false) { ERROR_MSG(); std::exit(1); }
			b_1st = false;
			unlock();
		}
		
		sleep(1);
	}
}

/******************************
******************************/
bool THREAD_BASE::ckeck_if_timeProceed()
{
	if(BootMode == BOOT_MODE__REC) return true;
	
	int Last_time_ms = get_TimeData_from_TimeTable(0, 0);
	for(int i = 1; i < NUM_SAMPLES_PER_BUFFER; i++){
		int new_time_ms = get_TimeData_from_TimeTable(0, i);
		
		if(new_time_ms == -1)					return true;
		else if(new_time_ms <= Last_time_ms)	return false;
		
		Last_time_ms = new_time_ms;
	}
	
	return true;
}

/******************************
******************************/
int THREAD_BASE::get_NextBufferId()
{
	int NextBufferId = BufferId + 1;
	if(NUM_BUFFERS <= NextBufferId) NextBufferId = 0;
	
	return NextBufferId;
}

/******************************
param
	timeout
		timeout in second.

return
	0	:OK. Completed within timeout.
	1	:NG. 実際は、0secで抜ける想定なので、ERROR exitとしてある.
******************************/
bool THREAD_BASE::Wait_NextBufferFilled(double timeout)
{
	/********************
	********************/
	bool b_Log_printed = false; // 一度でも待たされた場合はOne time Logを残す.
	
	double time_StepIn_sec = ofGetElapsedTimef();
	
	/********************
	********************/
	int NextBufferId = get_NextBufferId();
	
	while( ofGetElapsedTimef() - time_StepIn_sec < timeout ){
		this->lock();
		bool b_Empty_copy = b_Empty[NextBufferId];
		this->unlock();
		
		if(!b_Empty_copy){
			return 0;
			
		}else if(!b_Log_printed){
			b_Log_printed = true;
			
			/* */
			char buf_Log[BUF_SIZE];
			sprintf(buf_Log, ",,Wait NextBuffer Filled occured\n");
			fprint_debug_Log(buf_Log, LogFile_id);
		}
		// Sleep(1); // ms
		usleep(1000);
	}
	
	ERROR_MSG();
	std::exit(1);
	return 1;
}

/******************************
******************************/
void THREAD_BASE::Reset()
{
	this->lock();
	{
		/********************
		********************/
		if(fp)	{ fclose(fp); fp = NULL; }
		
		fp = open_ScenarioFile();
		if( (fp == NULL) || (fp_Rec == NULL) ){ // fp_Rec is opened at Constructor, but check here.
			ERROR_MSG();
			b_valid = false;
			
			stopThread();
			while(isThreadRunning()){
				waitForThread(true);
			}
			
			// ofExit();
	
		}else{
			b_valid = true;
		}
		
		/********************
		********************/
		b_End = false;
		b_EOF = false;
		
		for(int i = 0; i < NUM_BUFFERS; i++){
			b_Empty[i] = true;
		}
		
		BufferId = 0;
		id = 0;
		t_ofs_ms = 0;
		
	}	
	this->unlock();
}

/******************************
******************************/
bool THREAD_BASE::IsReady()
{
	/********************
	give "threadedFunction()" chance to run.
	********************/
	usleep(1000);
	
	/********************
	fileの最後まで読み込みが完了している.
	全てのBufferを使わずに最後まで格納できてしまうこともあるので.
	********************/
	lock();
	bool b_EOF_Copy = b_EOF;
	unlock();
	if(b_EOF_Copy) return true;


	/********************
	********************/
	bool b_Empty_copy[NUM_BUFFERS];
	
	this->lock();
	for(int i = 0; i < NUM_BUFFERS; i++){
		b_Empty_copy[i] = b_Empty[i];
	}
	this->unlock();
	
	for(int i = 0; i < NUM_BUFFERS; i++){
		if(b_Empty_copy[i] == true)	return false;
	}
	
	return true;
}

/******************************
******************************/
bool THREAD_BASE::set__t_ofs_ms(int _val)
{
	if(!isThreadRunning()){
		t_ofs_ms = _val;
	}else{
		ERROR_MSG();
	}
}

/******************************
******************************/
void THREAD_BASE::INT_Keyinput(int _data)
{
	INT_data_byKeyInput = _data;
}
/******************************
******************************/
void THREAD_BASE::charge(int BufferId_toCharge)
{
	/********************
	********************/
	if(!b_valid) return;
	
	/********************
	********************/
	float ElapsedTime_f = ofGetElapsedTimef();
	
	/********************
	********************/
	lock();
	bool b_EOF_Copy = b_EOF;
	unlock();
	if(b_EOF_Copy)	return;
	
	/********************
	********************/
	char buf_Log[BUF_SIZE];
	sprintf(buf_Log, "%.3f,,[%d] Charge Start\n", ElapsedTime_f, BufferId_toCharge);
	fprint_debug_Log(buf_Log, LogFile_id);
	
	
	char buf[BUF_SIZE];
	int Charge_id = 0;
	
	while(1){
		if( fgets(buf, BUF_SIZE, fp) == NULL ) {
			SetTime_DataToCharge(-1);
			chargeTimeTable_byCopying(BufferId_toCharge, Charge_id);
			
			lock();
			b_Empty[BufferId_toCharge] = false;
			b_EOF = true;
			unlock();
			
			/* */
			sprintf(buf_Log, "%.3f,,[%d] Last Charge Finish\n", ElapsedTime_f, BufferId_toCharge);
			fprint_debug_Log(buf_Log, LogFile_id);
			return;
			
		}else{
			string strBuf = buf;
			if(Set_time_and_data__DataToCharge(strBuf)){
				/********************
				********************/
				if(t_ofs_ms == 0){
					chargeTimeTable_byCopying(BufferId_toCharge, Charge_id);
					Charge_id++;
				}else{
					if(t_ofs_ms <= GetTime_DataToCharge()){
						chargeTimeTable_byCopying(BufferId_toCharge, Charge_id);
						Charge_id++;
						
						t_ofs_ms = 0;
						
					}else{
						// discard.
					}
				}
				
				/********************
				********************/
				if(NUM_SAMPLES_PER_BUFFER <= Charge_id){
					lock();
					b_Empty[BufferId_toCharge] = false;
					unlock();
					
					/* */
					sprintf(buf_Log, "%.3f,,[%d] Charge Finish\n", ElapsedTime_f, BufferId_toCharge);
					fprint_debug_Log(buf_Log, LogFile_id);
					return;
				}
			}
		}
	} // while(1)
}

/******************************
******************************/
void THREAD_BASE::setup()
{
	check_if_setup_ok_in_DerivedClass();
	
	set_LogFile_id();
	
	Reset();
}

/******************************
******************************/
void THREAD_BASE::exit()
{
}

/******************************
description
	base classのconstructorでは、仮想関数をcallしてはいけない.
	∵	base classが生成される段階では、派生classの実態がない.
		故に派生class内で定義された仮想関数がまだ実体化されていないのだ.
	純粋仮想関数の場合は、runtime error.
	通常の仮想関数の場合は、base classのそれがcallされてしまう.
	
	このため、本当は、base classのconstructorに書きたかった初期化内容を派生class側のconstructorに書かなければならないことがある.
	本methodはこれがきちんと処理されているかのcheck.
******************************/
void THREAD_BASE::check_if_setup_ok_in_DerivedClass()
{
	if(fp_Rec == NULL){
		ERROR_MSG(); std::exit(1);
	}
}

/******************************
******************************/
void THREAD_BASE::update(int t_music_ms)
{
	/********************
	********************/
	set_outputData_None();
	
	/********************
	********************/
	if(!b_valid) return;
	if(b_End)	return;
	
	/********************
	********************/
	float ElapsedTime_f = ofGetElapsedTimef();
	
	/********************
	********************/
	if(BootMode == BOOT_MODE__PLAY){
		while( get_TimeData_from_TimeTable(BufferId, id) <= t_music_ms ){
			/********************
			********************/
			if( (LastINT_sec == -1) || (ElapsedTime_f - LastINT_sec < 0.2) ){
				set_outputData(BufferId, id);
			}
			
			/********************
			********************/
			id++;
			if(NUM_SAMPLES_PER_BUFFER <= id) change_buffer_id(t_music_ms, ElapsedTime_f);
			
			if(get_TimeData_from_TimeTable(BufferId, id) == -1){
				b_End = true;
				return;
			}
		}
		
	}else{ // BOOT_MODE__REC
		if(INT_data_byKeyInput != -1){
			/********************
			********************/
			set_outputData(BufferId, id);
			set_INTdata_to_outputData(INT_data_byKeyInput);
			fprintf_outputData(t_music_ms, fp_Rec);
			INT_data_byKeyInput = -1; // clear.
			
			/********************
			********************/
			id++;
			if(NUM_SAMPLES_PER_BUFFER <= id) change_buffer_id(t_music_ms, ElapsedTime_f);
			
			if(get_TimeData_from_TimeTable(BufferId, id) == -1){
				b_End = true;
				return;
			}
		}
	}
	
	/********************
	********************/
	LastINT_sec = ElapsedTime_f;
}


/******************************
******************************/
void THREAD_BASE::change_buffer_id(int t_music_ms, float ElapsedTime)
{
	char buf_Log[BUF_SIZE];
	
	/* */
	sprintf(buf_Log, "%.3f,%d,Buffer Change Start(BufferId from = %d)\n", ElapsedTime, t_music_ms, BufferId);
	fprint_debug_Log(buf_Log, LogFile_id);
	
	Wait_NextBufferFilled(1);
	
	this->lock();
	b_Empty[BufferId] = true;
	this->unlock();
	
	BufferId = get_NextBufferId();
	id = 0;
	
	/* */
	sprintf(buf_Log, "%.3f,%d,Buffer Change Finish(BufferId to = %d)\n", ElapsedTime, t_music_ms, BufferId);
	fprint_debug_Log(buf_Log, LogFile_id);
}
