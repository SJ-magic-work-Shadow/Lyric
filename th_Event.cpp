/************************************************************
************************************************************/
#include <unistd.h>
#include "th_Event.h"

/************************************************************
function
************************************************************/

/******************************
******************************/
THREAD__EVENT_TIMETABLE::THREAD__EVENT_TIMETABLE()
{
	fp_Rec = open_RecFile();
	
	for(int i = 0; i < NUM_BUFFERS; i++){
		TimeTable[i] = new TIME_N_DATASET__EVENT[NUM_SAMPLES_PER_BUFFER];
	}
}

/******************************
******************************/
THREAD__EVENT_TIMETABLE::~THREAD__EVENT_TIMETABLE()
{
	for(int i = 0; i < NUM_BUFFERS; i++){
		delete[] TimeTable[i];
	}
}

/******************************
******************************/
void THREAD__EVENT_TIMETABLE::set_LogFile_id()
{
	LogFile_id = THREAD_TIMETABLE__EVENT;
}

/******************************
******************************/
FILE* THREAD__EVENT_TIMETABLE::open_ScenarioFile()
{
	FILE* fp = fopen("../../../data/Lyric.txt", "r");
	return fp;
}

/******************************
******************************/
FILE* THREAD__EVENT_TIMETABLE::open_RecFile()
{
	FILE* fp = fopen("../../../data/Lyric_Rec.txt", "w");
	return fp;
}

/******************************
******************************/
void THREAD__EVENT_TIMETABLE::SetTime_DataToCharge(int time)
{
	data_to_charge.t_music_ms = time;
}

/******************************
******************************/
bool THREAD__EVENT_TIMETABLE::Set_time_and_data__DataToCharge(string& strBuf)
{
	/********************
	delete : space, tab 
	********************/
	size_t pos;
	// while((pos = strBuf.find_first_of(" 　\t")) != string::npos){ // 半角・全角space, \t 削除
	while((pos = strBuf.find_first_of("\t")) != string::npos){ //  今回、phraseにはspaceもあるので、\tのみ削除
		strBuf.erase(pos, 1);
	}
	
	/********************
	split
	********************/
	vector<string> strMetas = ofSplitString(strBuf,"<|>");
	// printf("size = %d\n", strMetas.size());
	
	if( (strMetas.size() == 7) && (strMetas[0] == "<Event>") ){
		/********************
		********************/
		data_to_charge.t_music_ms 			= atoi(strMetas[1].c_str());
		
		data_to_charge.DataSet.EventId		= atoi(strMetas[2].c_str());
		data_to_charge.DataSet.phrase		= strMetas[3];
		data_to_charge.DataSet.t_FadeIn		= atof(strMetas[4].c_str());
		data_to_charge.DataSet.t_life		= atof(strMetas[5].c_str());
		data_to_charge.DataSet.t_FadeOut	= atof(strMetas[6].c_str());
		
		/********************
		********************/
		phrase_isAlnum();
		if(data_to_charge.DataSet.b_alnum && b_IsSimpleFont) del_NotPrimitiveLetter(data_to_charge.DataSet.phrase);
		
		return true;
		
	}else{
		return false;
	}
}

/******************************
description
	RvD_BETON13.ttf など、primitiveな文字しか用意のないfontを使う場合(surface.cpp)、本methodで no primitive letterを削除する.
	削除しない場合でも、console上にError表示が出るだけでError終了などする訳ではないが、気持ち悪いので.
	
	phrase_isAlnum()の後で使用し、日本語phraseは、ここを通らないようにすること(otherwise:全文字 削除してしまうので)
	
note
	stringの便利なmethod
		find_first_of, find_last_of, find_first_not_of, find_first_last_of
******************************/
void THREAD__EVENT_TIMETABLE::del_NotPrimitiveLetter(string& str)
{
	size_t pos;

	while( (pos = str.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 ")) != string::npos ){
		str.erase(pos, 1);
	}
}

/******************************
******************************/
void THREAD__EVENT_TIMETABLE::phrase_isAlnum()
{
	data_to_charge.DataSet.b_alnum = true;
	
	int length = data_to_charge.DataSet.phrase.length();
	for(int i = 0; i < length; i++){
		int c = data_to_charge.DataSet.phrase.c_str()[i];
		if( !isalnum(c) && !isspace(c) && !ispunct(c) && !iscntrl(c) ){
			data_to_charge.DataSet.b_alnum = false;
			return;
		}
	}
	
	return;
}

/******************************
******************************/
int THREAD__EVENT_TIMETABLE::GetTime_DataToCharge()
{
	return data_to_charge.t_music_ms;
}

/******************************
******************************/
void THREAD__EVENT_TIMETABLE::chargeTimeTable_byCopying(int BufferId_toCharge, int Charge_id)
{
	TimeTable[BufferId_toCharge][Charge_id] = data_to_charge;
}

/******************************
******************************/
void THREAD__EVENT_TIMETABLE::set_outputData(int BufferId, int id)
{
	data_to_output = TimeTable[BufferId][id];
}

/******************************
******************************/
void THREAD__EVENT_TIMETABLE::set_outputData_None()
{
	TIME_N_DATASET__EVENT temp = {0, {0/* EVENT__NONE(DesignManager.h) */, "none", 0, 0, 0}};
	data_to_output = temp;
}

/******************************
description
	data_to_outputのmemberを上書きする.
	data_to_outputのmemberは、最派生classでしか、わからない.
	上書きdataで、どのmemberを上書きするかは、最派生classで決める.
******************************/
void THREAD__EVENT_TIMETABLE::set_INTdata_to_outputData(int _data)
{
	data_to_output.DataSet.EventId = _data;
}

/******************************
description
	use '\t' as delimiter.
******************************/
void THREAD__EVENT_TIMETABLE::fprintf_outputData(int t_music, FILE* fp)
{
	fprintf(fp, "<Event>\t<|>\t%d\t<|>\t%d\t<|>\t%s\t<|>\t%.3f\t<|>\t%.3f\t<|>\t%.3f\n",
				t_music, data_to_output.DataSet.EventId, data_to_output.DataSet.phrase.c_str(), data_to_output.DataSet.t_FadeIn, data_to_output.DataSet.t_life, data_to_output.DataSet.t_FadeOut);
}

/******************************
******************************/
int THREAD__EVENT_TIMETABLE::get_TimeData_from_TimeTable(int BufferId, int id)
{
	return TimeTable[BufferId][id].t_music_ms;
}

/******************************
******************************/
void THREAD__EVENT_TIMETABLE::getLatestOutput(DATASET__EVENT& Dataset)
{
	Dataset = data_to_output.DataSet;
}




