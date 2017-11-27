/************************************************************
oFでの日本語表示 : font.drawString
	参考URL
		openFrameworksで日本語を表示する方法
			https://qiita.com/0x50/items/5b0df23cac2dfbc60247
			
		addon
			https://github.com/hironishihara/ofxTrueTypeFontUC
			
	description
		ofxTrueTypeFontUCは、下のように、非常に自然に表示が行える。
			char buf[256] = "こんにちは";
			// font.drawString("こんにちは", 100, 100);
			font.drawString(buf, 100, 100);
		L"xxx"でなく、charをそのまま渡せばOK.
			
		code上、日本語は、分岐判定などには一切使わず、
		表示のみなので、一旦char buf[]で受け、これをstringに渡し、string(変数名:phrase)を
			font.drawString(phrase, 100, 100);
		と言う感じで渡せばOK.
************************************************************/
#pragma once

/************************************************************
************************************************************/
#include "ofMain.h"
#include "ofxTrueTypeFontUC.h"

#include "sj_common.h"
#include "ParamOf_StructureDesign.h"

/************************************************************
************************************************************/

/**************************************************
**************************************************/
struct NOISE_PARAM{
	float Amp;
	float ofs;
	float Freq_sec;
	
	void set(float _Amp, float _ofs, float  _Freq_sec)
	{
		Amp = _Amp;
		ofs = _ofs;
		Freq_sec = _Freq_sec;
	}
	
	float getSignedNoise(float now)
	{
		return Amp * ofSignedNoise(now/Freq_sec + ofs);
	}
	
	float getNoise(float now)
	{
		return Amp * ofNoise(now/Freq_sec + ofs);
	}
};

/**************************************************
**************************************************/
class SURFACE{
public:
	/****************************************
	****************************************/
	enum FONT_SIZE{
		FONT_SIZE__SS,
		FONT_SIZE__S,
		FONT_SIZE__M,
		FONT_SIZE__L,
		FONT_SIZE__LL,
		FONT_SIZE__LLL,
		
		NUM_FONTSIZE,
	};
	
private:
	/****************************************
	****************************************/
	bool b_Valid;
	string phrase;
	bool b_alnum;
	
	FONT_SIZE FontSize;
	
	ofVec3f pos_phrase;
	float rot_phrase;
	float size_phrase;
	
	SURFACE__DISPCOORD_INFO DispCoordInfo;
	
	NOISE_PARAM NoiseParam_x;
	NOISE_PARAM NoiseParam_y;
	NOISE_PARAM NoiseParam_rot_x_deg;
	NOISE_PARAM NoiseParam_rot_y_deg;
	NOISE_PARAM NoiseParam_rot_z_deg;
	
	float t_Born;
	
	float t_FadeIn;
	float t_life;
	float t_FadeOut;
	
	
public:
	/****************************************
	****************************************/
	SURFACE();
	~SURFACE();
	
	void disable();
	void setup();
	void update(ofFbo& fbo);
	void set(ofFbo& fbo, const string& _phrase, bool _b_alnum, SURFACE__DISPCOORD_INFO& _DispCoordInfo, float _t_FadeIn, float _t_life, float _t_FadeOut);
	void draw(float gui_TotalAlpha, ofFbo& fbo);
};
