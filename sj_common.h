/************************************************************
************************************************************/
#pragma once

/************************************************************
************************************************************/
#include "stdio.h"

#include "ofMain.h"
#include "ofxGui.h"

/************************************************************
************************************************************/
#define BUILD_RELEASE						0
#define CHECK__PARAMOF_STRUCTURE_DESIGN		1
#define CHECK__SURFACE						2
	#define BUILD_SW BUILD_RELEASE

/************************************************************
************************************************************/
#define ERROR_MSG(); printf("Error in %s:%d\n", __FILE__, __LINE__);

enum{
	WIDTH = 1280,
	HEIGHT = 720,
};

enum{
	BUF_SIZE = 1000,
};

enum BOOT_MODE{
	BOOT_MODE__REC,
	BOOT_MODE__PLAY,
};

enum TEXT_COLOR{
	TEXTCOLOR_WHITE,
	TEXTCOLOR_BLACK,
};

enum{
	THREAD_TIMETABLE__EVENT,
	
	NUM_THREAD_TIMETABLE,
};

/************************************************************
************************************************************/

/**************************************************
**************************************************/
class Noncopyable{
protected:
	Noncopyable() {}
	~Noncopyable() {}

private:
	void operator =(const Noncopyable& src);
	Noncopyable(const Noncopyable& src);
};


/************************************************************
************************************************************/
extern BOOT_MODE BootMode;
extern TEXT_COLOR TextColor;
extern bool b_IsSimpleFont;

/************************************************************
************************************************************/
extern void fopen_LogFile();
extern void fclose_LogFile();
extern void fprint_debug_Log(char* message, int FileId);


