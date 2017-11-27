/************************************************************
************************************************************/
#include "ParamOf_StructureDesign.h"

/************************************************************
param
************************************************************/
/********************
********************/
static const float OptimumDistance = 623;
static const float CamAngle = 60;

/********************
********************/
static const float offset_Layer = 700;

static ofVec3f ofsVec_x(WIDTH, 0, 0);
static ofVec3f ofsVec_y(0, HEIGHT, 0);
static ofVec3f OptimumDistance_z(0, 0, OptimumDistance);


/********************
********************/
PARAM_OF_STRUCTURE_DESIGN ParamOfStructureDesign[NUM_STRUCTURE_TYPES][NUM_SURFACES] = {
	/********************
	STRUCTURE_TYPE__LAYER,
	********************/
	{
		{ // 0
			{ // Surface_DispCoordInfo
				ofVec3f(0, 0, -offset_Layer * 0), ofVec3f(0, 1, 0), 0,
			},
			{ // cam
				ofVec3f(0, 0, -offset_Layer * 0 + OptimumDistance), // pos
				ofVec3f(0, 0, -offset_Layer * 0), // LookAt
				ofVec3f(0, 1, 0), // Upper
				CamAngle,
			},
		},
		{ // 1
			{ // Surface_DispCoordInfo
				ofVec3f(0, 0, -offset_Layer * 1), ofVec3f(0, 1, 0), 0,
			},
			{ // cam
				ofVec3f(0, 0, -offset_Layer * 1 + OptimumDistance), // pos
				ofVec3f(0, 0, -offset_Layer * 1), // LookAt
				ofVec3f(0, 1, 0), // Upper
				CamAngle,
			},
		},
		{ // 2
			{ // Surface_DispCoordInfo
				ofVec3f(0, 0, -offset_Layer * 2), ofVec3f(0, 1, 0), 0,
			},
			{ // cam
				ofVec3f(0, 0, -offset_Layer * 2 + OptimumDistance), // pos
				ofVec3f(0, 0, -offset_Layer * 2), // LookAt
				ofVec3f(0, 1, 0), // Upper
				CamAngle,
			},
		},
		{ // 3
			{ // Surface_DispCoordInfo
				ofVec3f(0, 0, -offset_Layer * 3), ofVec3f(0, 1, 0), 0,
			},
			{ // cam
				ofVec3f(0, 0, -offset_Layer * 3 + OptimumDistance), // pos
				ofVec3f(0, 0, -offset_Layer * 3), // LookAt
				ofVec3f(0, 1, 0), // Upper
				CamAngle,
			},
		},
		{ // 4
			{ // Surface_DispCoordInfo
				ofVec3f(0, 0, -offset_Layer * 4), ofVec3f(0, 1, 0), 0,
			},
			{ // cam
				ofVec3f(0, 0, -offset_Layer * 4 + OptimumDistance), // pos
				ofVec3f(0, 0, -offset_Layer * 4), // LookAt
				ofVec3f(0, 1, 0), // Upper
				CamAngle,
			},
		},
		{ // 5
			{ // Surface_DispCoordInfo
				ofVec3f(0, 0, -offset_Layer * 5), ofVec3f(0, 1, 0), 0,
			},
			{ // cam
				ofVec3f(0, 0, -offset_Layer * 5 + OptimumDistance), // pos
				ofVec3f(0, 0, -offset_Layer * 5), // LookAt
				ofVec3f(0, 1, 0), // Upper
				CamAngle,
			},
		},
	},
	
	/********************
	STRUCTURE_TYPE__DEVVIEW,
	********************/
	{
		{ // 0
			{ // Surface_DispCoordInfo
				ofVec3f(0, 0, 0) + ofsVec_x * 0 + ofsVec_y * 0, ofVec3f(0, 1, 0), 0,
			},
			{ // cam
				ofVec3f(0, 0, 0) + ofsVec_x * 0 + ofsVec_y * 0 + OptimumDistance_z, // pos
				ofVec3f(0, 0, 0) + ofsVec_x * 0 + ofsVec_y * 0, // LookAt
				ofVec3f(0, 1, 0), // Upper
				CamAngle,
			},
		},
		{ // 1
			{ // Surface_DispCoordInfo
				ofVec3f(0, 0, 0) + ofsVec_x * (-1) + ofsVec_y * 0, ofVec3f(0, 1, 0), 0,
			},
			{ // cam
				ofVec3f(0, 0, 0) + ofsVec_x * (-1) + ofsVec_y * 0 + OptimumDistance_z, // pos
				ofVec3f(0, 0, 0) + ofsVec_x * (-1) + ofsVec_y * 0, // LookAt
				ofVec3f(0, 1, 0), // Upper
				CamAngle,
			},
		},
		{ // 2
			{ // Surface_DispCoordInfo
				ofVec3f(0, 0, 0) + ofsVec_x * 0 + ofsVec_y * 1, ofVec3f(0, 1, 0), 0,
			},
			{ // cam
				ofVec3f(0, 0, 0) + ofsVec_x * 0 + ofsVec_y * 1 + OptimumDistance_z, // pos
				ofVec3f(0, 0, 0) + ofsVec_x * 0 + ofsVec_y * 1, // LookAt
				ofVec3f(0, 1, 0), // Upper
				CamAngle,
			},
		},
		{ // 3
			{ // Surface_DispCoordInfo
				ofVec3f(0, 0, 0) + ofsVec_x * 1 + ofsVec_y * 0, ofVec3f(0, 1, 0), 0,
			},
			{ // cam
				ofVec3f(0, 0, 0) + ofsVec_x * 1 + ofsVec_y * 0 + OptimumDistance_z, // pos
				ofVec3f(0, 0, 0) + ofsVec_x * 1 + ofsVec_y * 0, // LookAt
				ofVec3f(0, 1, 0), // Upper
				CamAngle,
			},
		},
		{ // 4
			{ // Surface_DispCoordInfo
				ofVec3f(0, 0, 0) + ofsVec_x * 0 + ofsVec_y * (-1), ofVec3f(0, 1, 0), 0,
			},
			{ // cam
				ofVec3f(0, 0, 0) + ofsVec_x * 0 + ofsVec_y * (-1) + OptimumDistance_z, // pos
				ofVec3f(0, 0, 0) + ofsVec_x * 0 + ofsVec_y * (-1), // LookAt
				ofVec3f(0, 1, 0), // Upper
				CamAngle,
			},
		},
		{ // 5
			{ // Surface_DispCoordInfo
				ofVec3f(0, 0, 0) + ofsVec_x * 0 + ofsVec_y * (-2), ofVec3f(0, 1, 0), 0,
			},
			{ // cam
				ofVec3f(0, 0, 0) + ofsVec_x * 0 + ofsVec_y * (-2) + OptimumDistance_z, // pos
				ofVec3f(0, 0, 0) + ofsVec_x * 0 + ofsVec_y * (-2), // LookAt
				ofVec3f(0, 1, 0), // Upper
				CamAngle,
			},
		},
	},
	
	/********************
	STRUCTURE_TYPE__CUBE,
	********************/
	{
		{ // 0
			{ // Surface_DispCoordInfo
				ofVec3f(0, 0, 0), ofVec3f(0, 1, 0), 0,
			},
			{ // cam
				ofVec3f(0, 0, OptimumDistance), // pos
				ofVec3f(0, 0, 0), // LookAt
				ofVec3f(0, 1, 0), // Upper
				CamAngle,
			},
		},
		{ // 1
			{ // Surface_DispCoordInfo
				ofVec3f(0, 0, WIDTH), ofVec3f(0, 1, 0), 180,
			},
			{ // cam
				ofVec3f(0, 0, WIDTH - OptimumDistance), // pos
				ofVec3f(0, 0, WIDTH), // LookAt
				ofVec3f(0, 1, 0), // Upper
				CamAngle,
			},
		},
		{ // 2
			{ // Surface_DispCoordInfo
				ofVec3f(-WIDTH/2, 0, WIDTH/2), ofVec3f(0, 1, 0), 90,
			},
			{ // cam
				ofVec3f(-WIDTH/2 + OptimumDistance, 0, WIDTH/2), // pos
				ofVec3f(-WIDTH/2, 0, WIDTH/2), // LookAt
				ofVec3f(0, 1, 0), // Upper
				CamAngle,
			},
		},
		{ // 3
			{ // Surface_DispCoordInfo
				ofVec3f(WIDTH/2, 0, WIDTH/2), ofVec3f(0, 1, 0), -90,
			},
			{ // cam
				ofVec3f(WIDTH/2 - OptimumDistance, 0, WIDTH/2), // pos
				ofVec3f(WIDTH/2, 0, WIDTH/2), // LookAt
				ofVec3f(0, 1, 0), // Upper
				CamAngle,
			},
		},
		{ // 4
			{ // Surface_DispCoordInfo
				ofVec3f(0, HEIGHT/2, HEIGHT/2), ofVec3f(1, 0, 0), 90,
			},
			{ // cam
				ofVec3f(0, HEIGHT/2 - OptimumDistance, HEIGHT/2), // pos
				ofVec3f(0, HEIGHT/2, HEIGHT/2), // LookAt
				ofVec3f(0, 0, 1), // Upper
				CamAngle,
			},
		},
		{ // 5
			{ // Surface_DispCoordInfo
				ofVec3f(0, -HEIGHT/2, HEIGHT/2), ofVec3f(1, 0, 0), -90,
			},
			{ // cam
				ofVec3f(0, -HEIGHT/2 + OptimumDistance, HEIGHT/2), // pos
				ofVec3f(0, -HEIGHT/2, HEIGHT/2), // LookAt
				ofVec3f(0, 0, -1), // Upper
				CamAngle,
			},
		},
	},
};


/************************************************************
for test
************************************************************/
#if (BUILD_SW == CHECK__PARAMOF_STRUCTURE_DESIGN)
	
	/************************************************************
	************************************************************/
	static ofEasyCam EasyCam;
	static ofCamera cam;
	
	static ofImage img;
	static ofFbo fbo;
	
	static int SufraceId = 0;
	
	/************************************************************
	************************************************************/
	/******************************
	******************************/
	void ParamOfStructureDesign_set_SurfaceId(int _id)
	{
		if((0 <= _id) && (_id < NUM_SURFACES)){
			SufraceId = _id;
			printf("SufraceId = %d\n", SufraceId);
		}
	}
	
	/******************************
	******************************/
	void ParamOfStructureDesign_setup()
	{
		img.load("check_surface.png");
		fbo.allocate(WIDTH, HEIGHT, GL_RGBA);
	}
	
	/******************************
	******************************/
	void ParamOfStructureDesign_draw()
	{
		/********************
		********************/
		STRUCTURE_TYPE StructureType = STRUCTURE_TYPE__LAYER;
		// STRUCTURE_TYPE StructureType = STRUCTURE_TYPE__DEVVIEW;
		// STRUCTURE_TYPE StructureType = STRUCTURE_TYPE__CUBE;
		
		/********************
		********************/
		glDisable(GL_CULL_FACE);
		
		/********************
		********************/
		cam.setPosition(ParamOfStructureDesign[StructureType][SufraceId].CamParam.CamPosition);
		cam.lookAt(ParamOfStructureDesign[StructureType][SufraceId].CamParam.CamLookAt, ParamOfStructureDesign[StructureType][SufraceId].CamParam.CamUpper);
		cam.setFov(ParamOfStructureDesign[StructureType][SufraceId].CamParam.CamAngle);
		
		cam.setNearClip(1);
		cam.setFarClip(2000);
		
		/********************
		********************/
		fbo.begin();
		ofClear(0, 0, 0, 0);
		ofSetColor(255);
		img.draw(0, 0, ofGetWidth(), ofGetHeight());
		fbo.end();
		
		
		/********************
		********************/
		ofBackground(0);
		ofPushStyle();
	
		/********************
		Z-buffering and blemding mode do not work together properly in a simple way.
		(oF Essentials. p98)
		
		参考URL
			https://www.sjbaker.org/steve/omniv/alpha_sorting.html
		********************/
		// z buffering
		ofDisableAlphaBlending();
		ofEnableDepthTest();
		
		/*
		// Alpha
		ofEnableAlphaBlending();
		// ofEnableBlendMode(OF_BLENDMODE_ADD);
		ofEnableBlendMode(OF_BLENDMODE_ALPHA);
		*/
		
		/********************
		********************/
		ofSetColor(255);
		
		/********************
		in case of ofFbo
			fbo_src->getTextureReference().bind();
		********************/
		// EasyCam.begin();
		cam.begin();
		ofPushMatrix();
			ofVec3f pos = ParamOfStructureDesign[StructureType][SufraceId].Surface_DispCoordInfo.pos;
			ofVec3f Rotate_vec = ParamOfStructureDesign[StructureType][SufraceId].Surface_DispCoordInfo.Rotate_vec;
			float Rotate_deg = ParamOfStructureDesign[StructureType][SufraceId].Surface_DispCoordInfo.Rotate_deg;
		
			fbo.setAnchorPercent(0.5, 0.5);
			ofTranslate(pos.x, pos.y, pos.z);
			ofRotate(Rotate_deg, Rotate_vec.x, Rotate_vec.y, Rotate_vec.z);
			
			fbo.draw(0, 0);
			
		ofPopMatrix();
		cam.end();
		// EasyCam.end();
		
		ofPopStyle();
		
		ofDisableDepthTest();
	}
	
#endif