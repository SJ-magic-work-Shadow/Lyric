/************************************************************
************************************************************/
#include "sj_common.h"

/************************************************************
************************************************************/
BOOT_MODE BootMode;
TEXT_COLOR TextColor;
bool b_IsSimpleFont;

/************************************************************
************************************************************/
static FILE* fp_Log[NUM_THREAD_TIMETABLE];

/************************************************************
************************************************************/
/******************************
******************************/
void fopen_LogFile()
{
	for(int i = 0; i < NUM_THREAD_TIMETABLE; i++){
		char FileName[BUF_SIZE];
		sprintf(FileName, "../../../data/Log_%d.csv", i);
		
		fp_Log[i] = fopen(FileName, "w");
		if(fp_Log[i] == NULL)	{ ERROR_MSG(); std::exit(1); }
	}
}

/******************************
******************************/
void fclose_LogFile()
{
	for(int i = 0; i < NUM_THREAD_TIMETABLE; i++){
		if(fp_Log[i]) fclose(fp_Log[i]);
	}
}

/******************************
******************************/
void fprint_debug_Log(char* message, int FileId)
{
	/*
	if(NUM_THREAD_TIMETABLE <= FileId)	{ ERROR_MSG(); std::exit(1); }
	
	
	if(FileId == -1){
		for(int i = 0; i < NUM_THREAD_TIMETABLE; i++){
			fprintf(fp_Log[i], "%s", message);
		}
	}else{
		fprintf(fp_Log[FileId], "%s", message);
	}
	*/
}


