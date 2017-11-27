/************************************************************
************************************************************/
#include "test_surface.h"

/************************************************************
************************************************************/
static string st_TestMessage[] = {
	"こんにちは",
	"私の名前は",
	"SAIJO 信広",
};

static int NUM_TEST_MESSAGES = sizeof(st_TestMessage) / sizeof(st_TestMessage[0]);
static int id_TestMessage = 0;

/************************************************************
************************************************************/

/******************************
******************************/
TEST_SURFACE::TEST_SURFACE()
// : StructureType(STRUCTURE_TYPE__LAYER)
// : StructureType(STRUCTURE_TYPE__DEVVIEW)
: StructureType(STRUCTURE_TYPE__CUBE)
, SufraceId(0)
{
}

/******************************
******************************/
TEST_SURFACE::~TEST_SURFACE()
{
}
	
/******************************
******************************/
void TEST_SURFACE::setup()
{
	fbo.allocate(WIDTH, HEIGHT, GL_RGBA);
	surface.setup();
}

/******************************
******************************/
void TEST_SURFACE::update()
{
	surface.update(fbo);
}

/******************************
******************************/
void TEST_SURFACE::draw()
{
	cam.begin();
	
	ofBackground(0);
	surface.draw(255, fbo);
	
	cam.end();
}

/******************************
******************************/
void TEST_SURFACE::setup_surface(int _SurfaceId)
{
	/********************
	********************/
	if(NUM_SURFACES <= _SurfaceId){
		printf("No such surface\n");
		return;
	}
	
	/********************
	********************/
	SufraceId = _SurfaceId;
	printf("surface id = %d\n", SufraceId);
	
	/********************
	********************/
	cam.setPosition(ParamOfStructureDesign[StructureType][SufraceId].CamParam.CamPosition);
	cam.lookAt(ParamOfStructureDesign[StructureType][SufraceId].CamParam.CamLookAt, ParamOfStructureDesign[StructureType][SufraceId].CamParam.CamUpper);
	cam.setFov(ParamOfStructureDesign[StructureType][SufraceId].CamParam.CamAngle);
	
	cam.setNearClip(1);
	cam.setFarClip(2000);
	
	/********************
	********************/
	surface.set(fbo, st_TestMessage[id_TestMessage], false/* b_alnum */, ParamOfStructureDesign[StructureType][SufraceId].Surface_DispCoordInfo, 1.0, 5, 1.0);
	id_TestMessage++;
	if(NUM_TEST_MESSAGES <= id_TestMessage) id_TestMessage = 0;
}


