/************************************************************
************************************************************/
#include "surface.h"

/************************************************************
************************************************************/
static ofxTrueTypeFontUC font[SURFACE::NUM_FONTSIZE];
static ofxTrueTypeFontUC font_jp[SURFACE::NUM_FONTSIZE];

/************************************************************
************************************************************/

/******************************
******************************/
SURFACE::SURFACE()
: b_Valid(false)
{
	setlocale(LC_ALL, "jpn");
	
	/********************
	font
		http://photoshopvip.net/13228
			https://www.dafont.com/rvd-beton13.font
			
		http://photoshopvip.net/39939
	********************/
	// string FontName = "LEVIBRUSH.TTF";
	string FontName = "RvD_BETON13.ttf";
	string FontName_jp = "W6.ttc";
	
	/********************
	bool loadFont(string filename, int fontsize, bool bAntiAliased=true, bool makeContours=false, float simplifyAmt=0.3, int dpi=0);
		makeContours = trueにしないと、drawStringAsShapes()でErrorが出る.
	********************/
	font[FONT_SIZE__SS].loadFont(FontName, 30, true, true);
	font[FONT_SIZE__S].loadFont(FontName, 40, true, true);
	font[FONT_SIZE__M].loadFont(FontName, 50, true, true);
	font[FONT_SIZE__L].loadFont(FontName, 60, true, true);
	font[FONT_SIZE__LL].loadFont(FontName, 70, true, true);
	font[FONT_SIZE__LLL].loadFont(FontName, 80, true, true);
	
	font_jp[FONT_SIZE__SS].loadFont(FontName_jp, 30, true, true);
	font_jp[FONT_SIZE__S].loadFont(FontName_jp, 40, true, true);
	font_jp[FONT_SIZE__M].loadFont(FontName_jp, 50, true, true);
	font_jp[FONT_SIZE__L].loadFont(FontName_jp, 60, true, true);
	font_jp[FONT_SIZE__LL].loadFont(FontName_jp, 70, true, true);
	font_jp[FONT_SIZE__LLL].loadFont(FontName_jp, 80, true, true);
}

/******************************
******************************/
SURFACE::~SURFACE()
{
}

/******************************
******************************/
void SURFACE::setup()
{
}

/******************************
******************************/
void SURFACE::set(ofFbo& fbo, const string& _phrase, bool _b_alnum, SURFACE__DISPCOORD_INFO& _DispCoordInfo, float _t_FadeIn, float _t_life, float _t_FadeOut)
{
	/********************
	********************/
	const float rot_max_deg = 30;
	const float size_max = 1.4;
	
	const float noise_Amp_x = 70;
	const float noise_Amp_y = 40;
	const float noise_Amp_rot_deg = 5;
	const float noise_freq_sec = 5.0;
	
	
	/********************
	********************/
	b_Valid = true;
	phrase = _phrase;
	b_alnum = _b_alnum;
	
	for(FontSize = FONT_SIZE__SS; int(FontSize) < int(NUM_FONTSIZE) - 1; FontSize = (FONT_SIZE)(int(FontSize) + 1)){
		if(b_alnum){
			if(ofGetWidth() * 0.35 < font[FontSize].stringWidth(phrase)) break;
		}else{
			if(ofGetWidth() * 0.35 < font_jp[FontSize].stringWidth(phrase)) break;
		}
	}
	
	const float shrink_ratio = 0.7;
	if(b_alnum){
		pos_phrase.x = ofRandom(-fbo.getWidth() * shrink_ratio/2, fbo.getWidth() * shrink_ratio/2 - font[FontSize].stringWidth(phrase));
		pos_phrase.y = ofRandom(-fbo.getHeight() * shrink_ratio/2 + font[FontSize].stringHeight(phrase), fbo.getHeight() * shrink_ratio/2);
	}else{
		pos_phrase.x = ofRandom(-fbo.getWidth() * shrink_ratio/2, fbo.getWidth() * shrink_ratio/2 - font_jp[FontSize].stringWidth(phrase));
		pos_phrase.y = ofRandom(-fbo.getHeight() * shrink_ratio/2 + font_jp[FontSize].stringHeight(phrase), fbo.getHeight() * shrink_ratio/2);
	}
	pos_phrase.z = 0;
	
	rot_phrase = ofRandom(-rot_max_deg, rot_max_deg);
	size_phrase = ofRandom(1, size_max);
	
	DispCoordInfo = _DispCoordInfo;

	NoiseParam_x.set(noise_Amp_x, ofRandom(0, 1000), noise_freq_sec);
	NoiseParam_y.set(noise_Amp_y, ofRandom(0, 1000), noise_freq_sec);
	NoiseParam_rot_x_deg.set(noise_Amp_rot_deg, ofRandom(0, 1000), noise_freq_sec);
	NoiseParam_rot_y_deg.set(noise_Amp_rot_deg, ofRandom(0, 1000), noise_freq_sec);
	NoiseParam_rot_z_deg.set(noise_Amp_rot_deg, ofRandom(0, 1000), noise_freq_sec);
	
	t_Born = ofGetElapsedTimef();
	
	t_FadeIn = _t_FadeIn;
	t_life = _t_life;
	t_FadeOut = _t_FadeOut;
}

/******************************
******************************/
void SURFACE::disable()
{
	b_Valid = false;
}

/******************************
******************************/
void SURFACE::update(ofFbo& fbo)
{
	/********************
	********************/
	if(!b_Valid) return;
	
	/********************
	********************/
	float now = ofGetElapsedTimef();
	
	/********************
	********************/
	const float alpha_max = 220;
	float alpha;
	if(now - t_Born < t_FadeIn){
		if(t_FadeIn == 0){
			alpha = alpha_max;
		}else{
			double tan = alpha_max / t_FadeIn;
			alpha = tan * (now - t_Born);
		}
		
	}else if(now - t_Born < t_life){
		alpha = alpha_max;
	}else if(now - t_Born < t_life + t_FadeOut){
		if(t_FadeOut == 0){
			alpha = alpha_max;
		}else{
			double tan = -alpha_max / t_FadeOut;
			alpha = alpha_max + tan * (now - t_Born - t_life);
		}
		
	}else{
		alpha = 0;
		b_Valid = false;
	}
	
#if (BUILD_SW == CHECK__SURFACE)
	printf("FontSize:%4d/ life:%5.3f\r", FontSize, now - t_Born);
	fflush(stdout);
#endif
	
	/********************
	********************/
	fbo.begin();
	ofPushStyle();
	ofPushMatrix();
		/********************
		********************/
		glDisable(GL_CULL_FACE);
		
		/********************
		********************/
		ofDisableAlphaBlending();
		ofClear(0, 0, 0, 0);
		
		/********************
		********************/
		ofTranslate(WIDTH/2, HEIGHT/2);
		
		ofRotate(rot_phrase + NoiseParam_rot_z_deg.getSignedNoise(now), 0, 0, 1);
		ofRotate(NoiseParam_rot_x_deg.getSignedNoise(now), 1, 0, 0);
		ofRotate(NoiseParam_rot_y_deg.getSignedNoise(now), 0, 1, 0);
		
		ofScale(size_phrase, size_phrase);
		
		/********************
		fbo -> Disp
		ofDisp -syphon-> unity
		で、2回alphaが掛かるので、持ち上げておく.
		********************/
		alpha = sqrt(alpha/255) * 255;
		if(TextColor == TEXTCOLOR_WHITE)	ofSetColor(255, 255, 255, alpha);
		else								ofSetColor(0, 0, 0, alpha);
		
		/********************
		********************/
		if(b_alnum)	font[FontSize].drawStringAsShapes(phrase, pos_phrase.x + NoiseParam_x.getSignedNoise(now), pos_phrase.y + NoiseParam_y.getSignedNoise(now));
		else		font_jp[FontSize].drawStringAsShapes(phrase, pos_phrase.x + NoiseParam_x.getSignedNoise(now), pos_phrase.y + NoiseParam_y.getSignedNoise(now));
	
	ofPopMatrix();
	ofPopStyle();
	fbo.end();
}

/******************************
******************************/
void SURFACE::draw(float gui_TotalAlpha, ofFbo& fbo)
{
	/********************
	********************/
	if(!b_Valid) return;
	
	/********************
	********************/
	glDisable(GL_CULL_FACE);
	
	ofPushStyle();
	ofPushMatrix();
		/********************
		Z-buffering and blemding mode do not work together properly in a simple way.
		(oF Essentials. p98)
		
		参考URL
			https://www.sjbaker.org/steve/omniv/alpha_sorting.html
		********************/
		ofEnableAlphaBlending();
		// ofEnableBlendMode(OF_BLENDMODE_ALPHA);
		ofEnableBlendMode(OF_BLENDMODE_ADD);
		
		ofDisableDepthTest();
		ofEnableSmoothing();
		
		fbo.setAnchorPercent(0.5, 0.5);
		ofTranslate(DispCoordInfo.pos.x, DispCoordInfo.pos.y, DispCoordInfo.pos.z);
		ofRotate(DispCoordInfo.Rotate_deg, DispCoordInfo.Rotate_vec.x, DispCoordInfo.Rotate_vec.y, DispCoordInfo.Rotate_vec.z);
		
		ofSetColor(255, gui_TotalAlpha);
		fbo.draw(0, 0);
		
		ofDisableSmoothing();
	ofPopMatrix();
	ofPopStyle();
}

