#include "header/Angel.h"
#include "Common/CQuad.h"
#include "Common/CSolidCube.h"
#include "Common/CSolidSphere.h"
#include "Common/CWireSphere.h"
#include "Common/CWireCube.h"
#include "Common/CChecker.h"
#include "Common/CCamera.h"
#include "Common/CTexturePool.h"
#include "png_loader.h"
#include "Common/ObjLoader.h"
#include "Common/C2DSprite.h"

#define SPACE_KEY 32
#define SCREEN_SIZE 800
#define HALF_SIZE SCREEN_SIZE /2 
#define VP_HALFWIDTH  20.0f
#define VP_HALFHEIGHT 20.0f
#define GRID_SIZE 20 // must be an even number

// For Model View and Projection Matrix
mat4 g_mxModelView(1.0f);
mat4 g_mxProjection;

// For Objects
CQuad		  *g_pFloor, *g_pFloor2, *g_pFloor3, *g_pFloor4, *g_pFloor5, *g_pFloor6;
CQuad         *g_LeftWall_1, *g_RightWall_1, *g_FrontWall_1, *g_BackWall_1;
CQuad         *g_LeftWall_2, *g_RightWall_2, *g_FrontWall_2, *g_BackWall_2;
CQuad         *g_LeftWall_3, *g_RightWall_3, *g_FrontWall_3, *g_BackWall_3;
CQuad         *g_LeftWall_4, *g_RightWall_4, *g_FrontWall_4, *g_BackWall_4;
CQuad         *g_LeftWall_5, *g_RightWall_5, *g_FrontWall_5, *g_BackWall_5;
CQuad         *g_LeftWall_6, *g_RightWall_6, *g_FrontWall_6, *g_BackWall_6;
CQuad	      *g_pDoor1, *g_pDoor2, *g_pDoor3, *g_pDoor4, *g_pDoor5, *g_pDoor6, *g_pDoor7;
CQuad	      *g_pDoor8, *g_pDoor9, *g_pDoor10, *g_pDoor11, *g_pDoor12, *g_pDoor13, *g_pDoor14;
CSolidCube    *g_pCube, *g_pCubeMap;
CSolidSphere  *g_pSphere, *g_pEarth, *g_pPokemonBall;
ObjLoader	  *g_pObjLoader, *g_pObjLoader2;

//進擊的巨人貼圖設定
CQuad  *g_pAimal[3];
GLuint g_uiAimalTexID[4];

//Deemo貼圖設定
CQuad  *g_pDeemo[3];
GLuint g_uiDeemoTexID[3];

//kart貼圖設定
CQuad  *g_pKart[4];
GLuint g_uiKartTexID[4];

//yugioh貼圖設定
CQuad  *g_pYugioh[2];
CSolidCube  *g_pYugioh_Cube[6];//1黑2青3反 4~6=卡背
GLuint g_uiYugiohTexID[7];

//pokemon貼圖設定
CQuad  *g_pPokemon[6];//6隻神奇寶貝
GLuint g_uiPokemonTexID[7];
float g_fBallTime = 0;
float g_fBallTheta = 0;

// For View Point
GLfloat g_fRadius = 8.0;
GLfloat g_fTheta = 45.0f*DegreesToRadians;
GLfloat g_fPhi = 45.0f*DegreesToRadians;
vec4 cameraPos = vec4(0.0f);
vec4 cameraDir = vec4(0.0f);

bool b_Forward = false;
bool b_Backward = false;
bool b_Room1 = true;
bool b_Room2 = false;
bool b_Room3 = false;
bool b_Room4 = false;
bool b_Room5 = false;
bool b_Room6 = false;
//----------------------------------------------------------------------------
// Part 2 : for single light source
bool g_bAutoRotating = false;
float g_fElapsedTime = 0;
float g_fLightRadius = 6;
float g_fLight6Radius = 8.5;
float g_fKartRadius = 7;
float g_fLightTheta = 0;

float g_fEarthTime = 0;
float g_fEarthTheta = 0;

bool Card_Cover = true;
bool Card_Open = false;
bool During_Attack = true;
float g_fEnergyBallTime = 0;
float g_fEnergyBallTheta = 0;
float g_fCardTime = 0;
float g_fCardTheta = 0;
float g_fDestroyTime = 0;
float g_fDestroyTheta = 0;
float g_fTrapEffectTime = 0;

bool Kill_Titan1 = false;
bool Titan1_Alive = true;
bool Titan1_Died = false;
float g_fTitan1Time = 0;
float g_fTitan1Theta = 0;

bool Kill_Titan2 = false;
bool Titan2_Alive = true;
bool Titan2_Died = false;
float g_fTitan2Time = 0;
float g_fTitan2Theta = 0;

bool Kill_Titan3 = false;
bool Titan3_Alive = true;
bool Titan3_Died = false;
float g_fTitan3Time = 0;
float g_fTitan3Theta = 0;

float g_fKart1Time = 0;
float g_fKart2Time = 1;
float g_fKart3Time = 2;
float g_fKart4Time = 3;
float g_fKart1Theta = 0;
float g_fKart2Theta = 0;
float g_fKart3Theta = 0;
float g_fKart4Theta = 0;

float g_fLightR = 0.85f;
float g_fLightG = 0.85f;
float g_fLightB = 0.85f;

LightSource g_Light1 = {
	color4(g_fLightR, g_fLightG, g_fLightB, 1.0f), // ambient 
	color4(g_fLightR, g_fLightG, g_fLightB, 1.0f), // diffuse
	color4(g_fLightR, g_fLightG, g_fLightB, 1.0f), // specular
	point4(6.0f, 5.0f, 0.0f, 1.0f),   // position
	point4(0.0f, 0.0f, 0.0f, 1.0f),   // halfVector
	vec3(0.0f, 0.0f, 0.0f),			  //spotDirection
	2.0f,	// spotExponent(parameter e); cos^(e)(phi) 
	45.0f,	// spotCutoff;	// (range: [0.0, 90.0], 180.0)  spot 的照明範圍
	0.707f,	// spotCosCutoff; // (range: [1.0,0.0],-1.0), 照明方向與被照明點之間的角度取 cos 後, cut off 的值
	1,	// constantAttenuation	(a + bd + cd^2)^-1 中的 a, d 為光源到被照明點的距離
	0,	// linearAttenuation	    (a + bd + cd^2)^-1 中的 b
	0		// quadraticAttenuation (a + bd + cd^2)^-1 中的 c
};

LightSource g_Light2 = {
	color4(g_fLightR, g_fLightG, g_fLightB, 1.0f), // ambient 
	color4(g_fLightR, g_fLightG, g_fLightB, 1.0f), // diffuse
	color4(g_fLightR, g_fLightG, g_fLightB, 1.0f), // specular
	point4(20.0f, 8.0f, 0.0f, 1.0f),   // position
	point4(0.0f, 0.0f, 0.0f, 1.0f),   // halfVector
	vec3(20.0f, 0.0f, 0.0f),			  //spotDirection
	2.0f,	// spotExponent(parameter e); cos^(e)(phi) 
	90.0f,	// spotCutoff;	// (range: [0.0, 90.0], 180.0)  spot 的照明範圍
	0.707f,	// spotCosCutoff; // (range: [1.0,0.0],-1.0), 照明方向與被照明點之間的角度取 cos 後, cut off 的值
	1,	// constantAttenuation	(a + bd + cd^2)^-1 中的 a, d 為光源到被照明點的距離
	0,	// linearAttenuation	    (a + bd + cd^2)^-1 中的 b
	0		// quadraticAttenuation (a + bd + cd^2)^-1 中的 c
};

LightSource g_Light3 = {
	color4(0.76, 0.23, 0.13, 1.0f), // ambient 
	color4(0.76, 0.23, 0.13, 1.0f), // diffuse
	color4(0.76, 0.23, 0.13, 1.0f), // specular
	point4(0.0f, 8.0f, 20.0f, 1.0f),   // position
	point4(0.0f, 0.0f, 0.0f, 1.0f),   // halfVector
	vec3(0.0f, 0.0f, 20.0f),			  //spotDirection
	2.0f,	// spotExponent(parameter e); cos^(e)(phi) 
	90.0f,	// spotCutoff;	// (range: [0.0, 90.0], 180.0)  spot 的照明範圍
	0.707f,	// spotCosCutoff; // (range: [1.0,0.0],-1.0), 照明方向與被照明點之間的角度取 cos 後, cut off 的值
	1,	// constantAttenuation	(a + bd + cd^2)^-1 中的 a, d 為光源到被照明點的距離
	0,	// linearAttenuation	    (a + bd + cd^2)^-1 中的 b
	0		// quadraticAttenuation (a + bd + cd^2)^-1 中的 c
};

LightSource g_Light4 = {
	color4(g_fLightR, g_fLightG, g_fLightB, 1.0f), // ambient 
	color4(g_fLightR, g_fLightG, g_fLightB, 1.0f), // diffuse
	color4(g_fLightR, g_fLightG, g_fLightB, 1.0f), // specular
	point4(-20.0f, 8.0f, 0.0f, 1.0f),   // position
	point4(0.0f, 0.0f, 0.0f, 1.0f),   // halfVector
	vec3(-20.0f, 0.0f, 0.0f),			  //spotDirection
	2.0f,	// spotExponent(parameter e); cos^(e)(phi) 
	60.0f,	// spotCutoff;	// (range: [0.0, 90.0], 180.0)  spot 的照明範圍
	0.707f,	// spotCosCutoff; // (range: [1.0,0.0],-1.0), 照明方向與被照明點之間的角度取 cos 後, cut off 的值
	1,	// constantAttenuation	(a + bd + cd^2)^-1 中的 a, d 為光源到被照明點的距離
	0,	// linearAttenuation	    (a + bd + cd^2)^-1 中的 b
	0		// quadraticAttenuation (a + bd + cd^2)^-1 中的 c
};

LightSource g_Light5 = {
	color4(g_fLightR, g_fLightG, g_fLightB, 1.0f), // ambient 
	color4(g_fLightR, g_fLightG, g_fLightB, 1.0f), // diffuse
	color4(g_fLightR, g_fLightG, g_fLightB, 1.0f), // specular
	point4(-25.0f, 8.0f, 25.0f, 1.0f),   // position
	point4(0.0f, 0.0f, 0.0f, 1.0f),   // halfVector
	vec3(-20.0f, 0.0f, 20.0f),			  //spotDirection
	2.0f,	// spotExponent(parameter e); cos^(e)(phi) 
	90.0f,	// spotCutoff;	// (range: [0.0, 90.0], 180.0)  spot 的照明範圍
	0.707f,	// spotCosCutoff; // (range: [1.0,0.0],-1.0), 照明方向與被照明點之間的角度取 cos 後, cut off 的值
	1,	// constantAttenuation	(a + bd + cd^2)^-1 中的 a, d 為光源到被照明點的距離
	0,	// linearAttenuation	    (a + bd + cd^2)^-1 中的 b
	0		// quadraticAttenuation (a + bd + cd^2)^-1 中的 c
};

LightSource g_Light6 = {
	color4(g_fLightR, g_fLightG, g_fLightB, 1.0f), // ambient 
	color4(g_fLightR, g_fLightG, g_fLightB, 1.0f), // diffuse
	color4(g_fLightR, g_fLightG, g_fLightB, 1.0f), // specular
	point4(20.0f, 8.0f, 20.0f, 1.0f),   // position
	point4(0.0f, 0.0f, 0.0f, 1.0f),   // halfVector
	vec3(20.0f, 0.0f, 20.0f),			  //spotDirection
	2.0f,	// spotExponent(parameter e); cos^(e)(phi) 
	90.0f,	// spotCutoff;	// (range: [0.0, 90.0], 180.0)  spot 的照明範圍
	0.707f,	// spotCosCutoff; // (range: [1.0,0.0],-1.0), 照明方向與被照明點之間的角度取 cos 後, cut off 的值
	1,	// constantAttenuation	(a + bd + cd^2)^-1 中的 a, d 為光源到被照明點的距離
	0,	// linearAttenuation	    (a + bd + cd^2)^-1 中的 b
	0		// quadraticAttenuation (a + bd + cd^2)^-1 中的 c
};

LightSource g_Light7 = {
	color4(0.12, 0.56, 1.0, 1.0f), // ambient 
	color4(0.12, 0.56, 1.0, 1.0f), // diffuse
	color4(0.12, 0.56, 1.0, 1.0f), // specular
	point4(-22.0f, 8.0f, 13.0f, 1.0f),   // position
	point4(0.0f, 0.0f, 0.0f, 1.0f),   // halfVector
	vec3(-20.0f, 0.0f, 21.0f),			  //spotDirection
	2.0f,	// spotExponent(parameter e); cos^(e)(phi) 
	90.0f,	// spotCutoff;	// (range: [0.0, 90.0], 180.0)  spot 的照明範圍
	0.707f,	// spotCosCutoff; // (range: [1.0,0.0],-1.0), 照明方向與被照明點之間的角度取 cos 後, cut off 的值
	1,	// constantAttenuation	(a + bd + cd^2)^-1 中的 a, d 為光源到被照明點的距離
	0,	// linearAttenuation	    (a + bd + cd^2)^-1 中的 b
	0		// quadraticAttenuation (a + bd + cd^2)^-1 中的 c
};

LightSource g_Light8 = {
	color4(1.0, 0.0, 0.25, 1.0f), // ambient 
	color4(1.0, 0.0, 0.25, 1.0f), // diffuse
	color4(1.0, 0.0, 0.25, 1.0f), // specular
	point4(-20.0f, 8.0f, 13.0f, 1.0f),   // position
	point4(0.0f, 0.0f, 0.0f, 1.0f),   // halfVector
	vec3(-20.0f, 0.0f, 21.0f),			  //spotDirection
	2.0f,	// spotExponent(parameter e); cos^(e)(phi) 
	90.0f,	// spotCutoff;	// (range: [0.0, 90.0], 180.0)  spot 的照明範圍
	0.707f,	// spotCosCutoff; // (range: [1.0,0.0],-1.0), 照明方向與被照明點之間的角度取 cos 後, cut off 的值
	1,	// constantAttenuation	(a + bd + cd^2)^-1 中的 a, d 為光源到被照明點的距離
	0,	// linearAttenuation	    (a + bd + cd^2)^-1 中的 b
	0		// quadraticAttenuation (a + bd + cd^2)^-1 中的 c
};

LightSource g_Light9 = {
	color4(g_fLightR, g_fLightG, g_fLightB, 1.0f), // ambient 
	color4(g_fLightR, g_fLightG, g_fLightB, 1.0f), // diffuse
	color4(g_fLightR, g_fLightG, g_fLightB, 1.0f), // specular
	point4(-18.0f, 8.0f, 13.0f, 1.0f),   // position
	point4(0.0f, 0.0f, 0.0f, 1.0f),   // halfVector
	vec3(-20.0f, 0.0f, 21.0f),			  //spotDirection
	2.0f,	// spotExponent(parameter e); cos^(e)(phi) 
	90.0f,	// spotCutoff;	// (range: [0.0, 90.0], 180.0)  spot 的照明範圍
	0.707f,	// spotCosCutoff; // (range: [1.0,0.0],-1.0), 照明方向與被照明點之間的角度取 cos 後, cut off 的值
	1,	// constantAttenuation	(a + bd + cd^2)^-1 中的 a, d 為光源到被照明點的距離
	0,	// linearAttenuation	    (a + bd + cd^2)^-1 中的 b
	0		// quadraticAttenuation (a + bd + cd^2)^-1 中的 c
};

LightSource g_Light10 = {
	color4(0.12, 0.56, 1.0, 1.0f), // ambient 
	color4(0.12, 0.56, 1.0, 1.0f), // diffuse
	color4(0.12, 0.56, 1.0, 1.0f), // specular
	point4(-21.3f, 2.05f, 21.0f, 1.0f),   // position
	point4(0.0f, 0.0f, 0.0f, 1.0f),   // halfVector
	vec3(-21.5f, 0.0f, 21.0f),			  //spotDirection
	2.0f,	// spotExponent(parameter e); cos^(e)(phi) 
	90.0f,	// spotCutoff;	// (range: [0.0, 90.0], 180.0)  spot 的照明範圍
	0.707f,	// spotCosCutoff; // (range: [1.0,0.0],-1.0), 照明方向與被照明點之間的角度取 cos 後, cut off 的值
	1,	// constantAttenuation	(a + bd + cd^2)^-1 中的 a, d 為光源到被照明點的距離
	0,	// linearAttenuation	    (a + bd + cd^2)^-1 中的 b
	0		// quadraticAttenuation (a + bd + cd^2)^-1 中的 c
};

KartSource g_Kart1 = {
	point4(-15.0f, 1.5f, 8.0f, 1.0f)
};

KartSource g_Kart2 = {
	point4(-7.0f, 1.5f, 0.0f, 1.0f)
};

KartSource g_Kart3 = {
	point4(-15.0f, 1.5f, 8.0f, 1.0f)
};

KartSource g_Kart4 = {
	point4(-23.0f, 1.5f, 0.0f, 1.0f)
};

CardSource g_Card_Front = {
	point4(-20.0f, 0.3f, 13.1f, 1.0f)
};

CardSource g_Card_Back = {
	point4(-20.0f, 0.32f, 13.1f, 1.0f)
};

CardSource g_Monster = {
	point4(-20.0f, 2.0f, 21.0f, 1.0f)
};

CardSource g_Card_M_Fornt = {
	point4(-20.0f, 0.32f, 23.0f, 1.0f)
};

CardSource g_Card_M_Back = {
	point4(-20.0f, 0.3f, 23.0f, 1.0f)
};

BallSource g_Ball = {
	point4(20.0f, 2.0f, 20.0f, 1.0f)
};

TitanSource g_Titan1 = {
	point4(5.0f, 3.5f, 0.0f, 1.0f)
};

TitanSource g_Titan2 = {
	point4(0.0f, 3.5f, 5.0f, 1.0f)
};

TitanSource g_Titan3 = {
	point4(-5.0f, 3.0f, 0.0f, 1.0f)
};
CWireSphere *g_pLight, *g_pLight2, *g_pLight3, *g_pLight4, *g_pLight5, *g_pLight6;
CWireSphere *g_pTrap7, *g_pTrap8, *g_pTrap9;//無0~6，方便對應lightsource而已
CWireSphere *g_pEnergyBall;
//----------------------------------------------------------------------------

// Texture 
GLuint g_uiFTexID[20]; //所有普通貼圖
GLuint g_uiAimalNormalTexID[20];//所有normal貼圖
int g_iTexWidth,  g_iTexHeight;
GLuint g_uiSphereCubeMap; // for Cubic Texture

// 2D 介面所需要的相關變數
C2DSprite *g_p2DKnife;
mat4  g_2DView;
mat4  g_2DProj;
bool  cut_effect_On = false;
//----------------------------------------------------------------------------
// 函式的原型宣告
extern void IdleProcess();

void init( void )
{
	mat4 mxT;
	vec4 vT, vColor;
	// 產生所需之 Model View 與 Projection Matrix
	// 產生所需之 Model View 與 Projection Matrix
	point4  eye(g_fRadius*sin(g_fTheta)*sin(g_fPhi), g_fRadius*cos(g_fTheta), g_fRadius*sin(g_fTheta)*cos(g_fPhi), 1.0f);
	point4  at(0.0f, 0.0f, 0.0f, 1.0f);
	auto camera = CCamera::create();
	camera->updateViewLookAt(eye, at);
	cameraDir = at;
	cameraPos = eye;

	//45要改
	auto texturepool = CTexturePool::create();
	g_uiFTexID[0] = texturepool->AddTexture("texture/checker.png");
	g_uiFTexID[1] = texturepool->AddTexture("texture/Masonry.Brick.png");
	g_uiFTexID[3] = texturepool->AddTexture("texture/metal.png");
	g_uiFTexID[4] = texturepool->AddTexture("texture/Sunny_NX.png");
	g_uiFTexID[5] = texturepool->AddTexture("texture/gold.png");
	g_uiFTexID[6] = texturepool->AddTexture("texture/test_1.jpg");
	g_uiFTexID[7] = texturepool->AddTexture("texture/titan_1.jpg");
	//g_uiFTexID[8] = texturepool->AddTexture("texture/magic_1.jpg");
	g_uiFTexID[9] = texturepool->AddTexture("texture/earth.png");
	g_uiFTexID[10] = texturepool->AddTexture("texture/Sunny_NY.png");
	g_uiFTexID[11] = texturepool->AddTexture("texture/wi_sma.png");
	g_uiFTexID[12] = texturepool->AddTexture("texture/wall.jpg");
	g_uiFTexID[13] = texturepool->AddTexture("texture/pokemon_8.png");
	g_uiFTexID[14] = texturepool->AddTexture("texture/crowd_1.jpg");
	g_uiFTexID[15] = texturepool->AddTexture("texture/Sunny_PY.png");
	g_uiFTexID[16] = texturepool->AddTexture("texture/portal.png");
	g_uiFTexID[17] = texturepool->AddTexture("texture/energy.png");
#ifdef MULTITEXTURE
	g_uiFTexID[2] = texturepool->AddTexture("texture/lightMap1.png");
#endif
	g_uiSphereCubeMap = CubeMap_load_SOIL();
	g_uiAimalTexID[0] = texturepool->AddTexture("texture/boxing3.png");
	g_uiAimalTexID[1] = texturepool->AddTexture("texture/boxing2.png");
	g_uiAimalTexID[2] = texturepool->AddTexture("texture/boxing.png");

	g_uiAimalNormalTexID[0] = texturepool->AddTexture("texture/boxing3_normal.png");
	g_uiAimalNormalTexID[1] = texturepool->AddTexture("texture/boxing2_normal.png");
	g_uiAimalNormalTexID[2] = texturepool->AddTexture("texture/boxing_normal.png");
	g_uiAimalNormalTexID[3] = texturepool->AddTexture("texture/Masonry.Brick.normal.png");
	g_uiAimalNormalTexID[4] = texturepool->AddTexture("texture/test_1_normal.png");
	//g_uiAimalNormalTexID[5] = texturepool->AddTexture("texture/magic_1_normal.png");
	g_uiAimalNormalTexID[6] = texturepool->AddTexture("texture/cat_normal.png");
	g_uiAimalNormalTexID[7] = texturepool->AddTexture("texture/maonkey_normal.png");
	g_uiAimalNormalTexID[8] = texturepool->AddTexture("texture/squirrel-koala_normal.png");
	g_uiAimalNormalTexID[9] = texturepool->AddTexture("texture/squirrels_normal.png");
	g_uiAimalNormalTexID[10] = texturepool->AddTexture("texture/car(1)_normal.png");
	g_uiAimalNormalTexID[11] = texturepool->AddTexture("texture/car(2)_normal.png");

	g_uiDeemoTexID[0] = texturepool->AddTexture("texture/wi_tata.png");
	g_uiDeemoTexID[1] = texturepool->AddTexture("texture/wi_sma.png");
	g_uiDeemoTexID[2] = texturepool->AddTexture("texture/wi_tree.png");

	g_uiKartTexID[0] = texturepool->AddTexture("texture/cat.png");
	g_uiKartTexID[1] = texturepool->AddTexture("texture/monkey.png");
	g_uiKartTexID[2] = texturepool->AddTexture("texture/squirrel-koala.png");
	g_uiKartTexID[3] = texturepool->AddTexture("texture/squirrels.png");

	g_uiYugiohTexID[0] = texturepool->AddTexture("texture/road_1.jpg");
	g_uiYugiohTexID[4] = texturepool->AddTexture("texture/car(1).png");


	g_uiPokemonTexID[0] = texturepool->AddTexture("texture/earth.png");
	g_uiPokemonTexID[1] = texturepool->AddTexture("texture/plane.png");
	//g_uiPokemonTexID[2] = texturepool->AddTexture("texture/pokemon_2.png");
	//g_uiPokemonTexID[3] = texturepool->AddTexture("texture/pokemon_3.png");
	g_uiPokemonTexID[4] = texturepool->AddTexture("texture/helicopter.png");
	//g_uiPokemonTexID[5] = texturepool->AddTexture("texture/pokemon_6.png");
	//g_uiPokemonTexID[6] = texturepool->AddTexture("texture/pokemon_7.png");

	// 產生物件的實體
//所有地板------------------------------------------------------------------------------------------------------------
	g_pFloor = new CQuad;
	g_pFloor2 = new CQuad;
	g_pFloor3 = new CQuad;
	g_pFloor4 = new CQuad;
	g_pFloor5 = new CQuad;
	g_pFloor6 = new CQuad;
#ifdef MULTITEXTURE
	g_pFloor->SetTextureLayer(DIFFUSE_MAP | LIGHT_MAP);
	g_pFloor2->SetTextureLayer(DIFFUSE_MAP | LIGHT_MAP);
	g_pFloor3->SetTextureLayer(DIFFUSE_MAP | LIGHT_MAP);
	g_pFloor4->SetTextureLayer(DIFFUSE_MAP | LIGHT_MAP);
	g_pFloor5->SetTextureLayer(DIFFUSE_MAP | LIGHT_MAP);
	g_pFloor6->SetTextureLayer(DIFFUSE_MAP | LIGHT_MAP);
#endif
	g_pFloor->SetShader();
	g_pFloor->SetTRSMatrix(Scale(20,1,20));
	g_pFloor->SetShadingMode(GOURAUD_SHADING);
	g_pFloor->SetTiling(1,1);
	
	vT.x = 20.0f; vT.y = 0.0f; vT.z = 0;
	mxT = Translate(vT);
	g_pFloor2->SetShader();
	g_pFloor2->SetTRSMatrix(mxT*Scale(20, 1, 20));
	g_pFloor2->SetShadingMode(GOURAUD_SHADING);
	g_pFloor2->SetTiling(40, 40);

	vT.x = 0.0f; vT.y = 0.0f; vT.z = 20.0f;
	mxT = Translate(vT);
	g_pFloor3->SetShader();
	g_pFloor3->SetTRSMatrix(mxT*Scale(20, 1, 20));
	g_pFloor3->SetShadingMode(GOURAUD_SHADING);
	g_pFloor3->SetTiling(1, 1);

	vT.x = -20.0f; vT.y = 0.0f; vT.z = 0.0f;
	mxT = Translate(vT);
	g_pFloor4->SetShader();
	g_pFloor4->SetTRSMatrix(mxT*Scale(20, 1, 20));
	g_pFloor4->SetShadingMode(GOURAUD_SHADING);
	g_pFloor4->SetTiling(1, 1);

	vT.x = -20.0f; vT.y = 0.0f; vT.z = 20.0f;
	mxT = Translate(vT);
	g_pFloor5->SetShader();
	g_pFloor5->SetTRSMatrix(mxT*Scale(20, 1, 20));
	g_pFloor5->SetShadingMode(GOURAUD_SHADING);
	g_pFloor5->SetTiling(1, 1);

	vT.x = 20.0f; vT.y = 0.0f; vT.z = 20.0f;
	mxT = Translate(vT);
	g_pFloor6->SetShader();
	g_pFloor6->SetTRSMatrix(mxT*Scale(20, 1, 20));
	g_pFloor6->SetShadingMode(GOURAUD_SHADING);
	g_pFloor6->SetTiling(1, 1);

	// 設定貼圖
	g_pFloor->SetMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pFloor->SetKaKdKsShini(0, 0.8f, 0.5f, 1);
	g_pFloor2->SetMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pFloor2->SetKaKdKsShini(0, 0.8f, 0.5f, 1);
	g_pFloor3->SetMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pFloor3->SetKaKdKsShini(0, 0.8f, 0.5f, 1);
	g_pFloor4->SetMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pFloor4->SetKaKdKsShini(0, 0.8f, 0.5f, 1);
	g_pFloor5->SetMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pFloor5->SetKaKdKsShini(0, 0.8f, 0.5f, 1);
	g_pFloor6->SetMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pFloor6->SetKaKdKsShini(0, 0.8f, 0.5f, 1);

//所有牆壁------------------------------------------------------------------------------------------------------------
	//第一間--巨人
	vT.x = -9.9f; vT.y = 10.0f; vT.z = 0;
	mxT = Translate(vT);
	g_LeftWall_1 = new CQuad;
	g_LeftWall_1->SetShadingMode(GOURAUD_SHADING);
	g_LeftWall_1->SetShader();
	g_LeftWall_1->SetTRSMatrix(mxT*RotateZ(-90.0f)*Scale(20, 1, 20));
	g_LeftWall_1->SetTiling(5, 5); // 原始為 (10, 10)
	// 設定貼圖
	g_LeftWall_1->SetMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_LeftWall_1->SetKaKdKsShini(0, 0.8f, 0.5f, 1);

	vT.x = 9.9f; vT.y = 10.0f; vT.z = 0;
	mxT = Translate(vT);
	g_RightWall_1 = new CQuad;
	g_RightWall_1->SetShadingMode(GOURAUD_SHADING);
	g_RightWall_1->SetShader();
	g_RightWall_1->SetTRSMatrix(mxT*RotateZ(90.0f)*Scale(20, 1, 20));
	g_RightWall_1->SetTiling(5, 5); // 原始為 (10, 10)
	// 設定貼圖
	g_RightWall_1->SetMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_RightWall_1->SetKaKdKsShini(0, 0.8f, 0.5f, 1);

	vT.x = 0.0f; vT.y = 10.0f; vT.z = 9.9f;
	mxT = Translate(vT);
	g_FrontWall_1 = new CQuad;
	g_FrontWall_1->SetShadingMode(GOURAUD_SHADING);
	g_FrontWall_1->SetShader();
	g_FrontWall_1->SetTRSMatrix(mxT*RotateX(-90.0f)*Scale(20, 1, 20));
	g_FrontWall_1->SetTiling(5, 5); // 原始為 (10, 10)
	// 設定貼圖
	g_FrontWall_1->SetMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_FrontWall_1->SetKaKdKsShini(0, 0.8f, 0.5f, 1);

	vT.x = 0.0f; vT.y = 10.0f; vT.z = -9.9f;
	mxT = Translate(vT);
	g_BackWall_1 = new CQuad;
	g_BackWall_1->SetShadingMode(GOURAUD_SHADING);
	g_BackWall_1->SetShader();
	g_BackWall_1->SetTRSMatrix(mxT*RotateX(90.0f)*Scale(20, 1, 20));
	g_BackWall_1->SetTiling(5, 5); // 原始為 (10, 10)
	// 設定貼圖
	g_BackWall_1->SetMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 0.5f), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_BackWall_1->SetKaKdKsShini(0, 0.8f, 0.5f, 1);

	//第二間--Deemo
	vT.x = 10.1f; vT.y = 10.0f; vT.z = 0;
	mxT = Translate(vT);
	g_LeftWall_2 = new CQuad;
	g_LeftWall_2->SetShadingMode(GOURAUD_SHADING);
	g_LeftWall_2->SetShader();
	g_LeftWall_2->SetTRSMatrix(mxT*RotateZ(-90.0f)*Scale(20, 1, 20));
	g_LeftWall_2->SetTiling(5, 5); // 原始為 (10, 10)
								   // 設定貼圖
	g_LeftWall_2->SetMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_LeftWall_2->SetKaKdKsShini(0, 0.8f, 0.5f, 1);

	vT.x = 29.9f; vT.y = 10.0f; vT.z = 0;
	mxT = Translate(vT);
	g_RightWall_2 = new CQuad;
	g_RightWall_2->SetShadingMode(GOURAUD_SHADING);
	g_RightWall_2->SetShader();
	g_RightWall_2->SetTRSMatrix(mxT*RotateZ(90.0f)*Scale(20, 1, 20));
	g_RightWall_2->SetTiling(5, 5); // 原始為 (10, 10)
									// 設定貼圖
	g_RightWall_2->SetMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 0.5f), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_RightWall_2->SetKaKdKsShini(0, 0.8f, 0.5f, 1);

	vT.x = 20.0f; vT.y = 10.0f; vT.z = 9.9f;
	mxT = Translate(vT);
	g_FrontWall_2 = new CQuad;
	g_FrontWall_2->SetShadingMode(GOURAUD_SHADING);
	g_FrontWall_2->SetShader();
	g_FrontWall_2->SetTRSMatrix(mxT*RotateX(-90.0f)*Scale(20, 1, 20));
	g_FrontWall_2->SetTiling(5, 5); // 原始為 (10, 10)
									// 設定貼圖
	g_FrontWall_2->SetMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_FrontWall_2->SetKaKdKsShini(0, 0.8f, 0.5f, 1);

	vT.x = 20.0f; vT.y = 10.0f; vT.z = -9.9f;
	mxT = Translate(vT);
	g_BackWall_2 = new CQuad;
	g_BackWall_2->SetShadingMode(GOURAUD_SHADING);
	g_BackWall_2->SetShader();
	g_BackWall_2->SetTRSMatrix(mxT*RotateX(90.0f)*Scale(20, 1, 20));
	g_BackWall_2->SetTiling(5, 5); // 原始為 (10, 10)
								   // 設定貼圖
	g_BackWall_2->SetMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 0.5f), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_BackWall_2->SetKaKdKsShini(0, 0.8f, 0.5f, 1);

	//第三間--詭異之間
	vT.x = -9.9f; vT.y = 10.0f; vT.z = 20.0;
	mxT = Translate(vT);
	g_LeftWall_3 = new CQuad;
	g_LeftWall_3->SetShadingMode(GOURAUD_SHADING);
	g_LeftWall_3->SetShader();
	g_LeftWall_3->SetTRSMatrix(mxT*RotateX(90.0f)*RotateZ(-90.0f)*Scale(20, 1, 20));
	g_LeftWall_3->SetTiling(1, 1); // 原始為 (10, 10)
								   // 設定貼圖
	g_LeftWall_3->SetMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_LeftWall_3->SetKaKdKsShini(0, 0.8f, 0.5f, 1);

	vT.x = 9.9f; vT.y = 10.0f; vT.z = 20.0;
	mxT = Translate(vT);
	g_RightWall_3 = new CQuad;
	g_RightWall_3->SetShadingMode(GOURAUD_SHADING);
	g_RightWall_3->SetShader();
	g_RightWall_3->SetTRSMatrix(mxT*RotateX(90.0f)*RotateZ(90.0f)*Scale(20, 1, 20));
	g_RightWall_3->SetTiling(1, 1); // 原始為 (10, 10)
									// 設定貼圖
	g_RightWall_3->SetMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_RightWall_3->SetKaKdKsShini(0, 0.8f, 0.5f, 1);

	vT.x = 0.0f; vT.y = 10.0f; vT.z = 29.9f;
	mxT = Translate(vT);
	g_FrontWall_3 = new CQuad;
	g_FrontWall_3->SetShadingMode(GOURAUD_SHADING);
	g_FrontWall_3->SetShader();
	g_FrontWall_3->SetTRSMatrix(mxT*RotateX(-90)*RotateY(180)*Scale(20, 1, 20));
	g_FrontWall_3->SetTiling(1, 1); // 原始為 (10, 10)
									// 設定貼圖
	g_FrontWall_3->SetMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_FrontWall_3->SetKaKdKsShini(0, 0.8f, 0.5f, 1);

	vT.x = 0.0f; vT.y = 10.0f; vT.z =10.1f;
	mxT = Translate(vT);
	g_BackWall_3 = new CQuad;
	g_BackWall_3->SetShadingMode(GOURAUD_SHADING);
	g_BackWall_3->SetShader();
	g_BackWall_3->SetTRSMatrix(mxT*RotateX(90)*Scale(20, 1, 20));
	g_BackWall_3->SetTiling(1, 1); // 原始為 (10, 10)
								   // 設定貼圖
	g_BackWall_3->SetMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_BackWall_3->SetKaKdKsShini(0, 0.8f, 0.5f, 1);

	//第四間--卡丁車
	vT.x = -29.9f; vT.y = 10.0f; vT.z = 0.0;
	mxT = Translate(vT);
	g_LeftWall_4 = new CQuad;
	g_LeftWall_4->SetShadingMode(GOURAUD_SHADING);
	g_LeftWall_4->SetShader();
	g_LeftWall_4->SetTRSMatrix(mxT*RotateX(90.0f)*RotateZ(-90.0f)*Scale(20, 1, 20));
	g_LeftWall_4->SetTiling(1, 1); // 原始為 (10, 10)
								   // 設定貼圖
	g_LeftWall_4->SetMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_LeftWall_4->SetKaKdKsShini(0, 0.8f, 0.5f, 1);

	vT.x = -10.1f; vT.y = 10.0f; vT.z = 0.0;
	mxT = Translate(vT);
	g_RightWall_4 = new CQuad;
	g_RightWall_4->SetShadingMode(GOURAUD_SHADING);
	g_RightWall_4->SetShader();
	g_RightWall_4->SetTRSMatrix(mxT*RotateX(90.0f)*RotateZ(90.0f)*Scale(20, 1, 20));
	g_RightWall_4->SetTiling(1, 1); // 原始為 (10, 10)
	// 設定貼圖
	g_RightWall_4->SetMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_RightWall_4->SetKaKdKsShini(0, 0.8f, 0.5f, 1);

	vT.x = -20.0f; vT.y = 10.0f; vT.z = 9.9f;
	mxT = Translate(vT);
	g_FrontWall_4 = new CQuad;
	g_FrontWall_4->SetShadingMode(GOURAUD_SHADING);
	g_FrontWall_4->SetShader();
	g_FrontWall_4->SetTRSMatrix(mxT*RotateX(-90)*RotateY(180)*Scale(20, 1, 20));
	g_FrontWall_4->SetTiling(1, 1); // 原始為 (10, 10)
	// 設定貼圖
	g_FrontWall_4->SetMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_FrontWall_4->SetKaKdKsShini(0, 0.8f, 0.5f, 1);

	vT.x = -20.0f; vT.y = 10.0f; vT.z = -9.9f;
	mxT = Translate(vT);
	g_BackWall_4 = new CQuad;
	g_BackWall_4->SetShadingMode(GOURAUD_SHADING);
	g_BackWall_4->SetShader();
	g_BackWall_4->SetTRSMatrix(mxT*RotateX(90)*Scale(20, 1, 20));
	g_BackWall_4->SetTiling(1, 1); // 原始為 (10, 10)
								   // 設定貼圖
	g_BackWall_4->SetMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_BackWall_4->SetKaKdKsShini(0, 0.8f, 0.5f, 1);

	//第五間--遊戲王
	vT.x = -29.9f; vT.y = 10.0f; vT.z = 20.0;
	mxT = Translate(vT);
	g_LeftWall_5 = new CQuad;
	g_LeftWall_5->SetShadingMode(GOURAUD_SHADING);
	g_LeftWall_5->SetShader();
	g_LeftWall_5->SetTRSMatrix(mxT*RotateX(90.0f)*RotateZ(-90.0f)*Scale(20, 1, 20));
	g_LeftWall_5->SetTiling(1, 1); // 原始為 (10, 10)
	// 設定貼圖
	g_LeftWall_5->SetMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_LeftWall_5->SetKaKdKsShini(0, 0.8f, 0.5f, 1);

	vT.x = -10.1f; vT.y = 10.0f; vT.z = 20.0;
	mxT = Translate(vT);
	g_RightWall_5 = new CQuad;
	g_RightWall_5->SetShadingMode(GOURAUD_SHADING);
	g_RightWall_5->SetShader();
	g_RightWall_5->SetTRSMatrix(mxT*RotateX(90.0f)*RotateZ(90.0f)*Scale(20, 1, 20));
	g_RightWall_5->SetTiling(1, 1); // 原始為 (10, 10)
									// 設定貼圖
	g_RightWall_5->SetMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_RightWall_5->SetKaKdKsShini(0, 0.8f, 0.5f, 1);

	vT.x = -20.0f; vT.y = 10.0f; vT.z = 29.9f;
	mxT = Translate(vT);
	g_FrontWall_5 = new CQuad;
	g_FrontWall_5->SetShadingMode(GOURAUD_SHADING);
	g_FrontWall_5->SetShader();
	g_FrontWall_5->SetTRSMatrix(mxT*RotateX(-90)*RotateY(180)*Scale(20, 1, 20));
	g_FrontWall_5->SetTiling(1, 1); // 原始為 (10, 10)
									// 設定貼圖
	g_FrontWall_5->SetMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_FrontWall_5->SetKaKdKsShini(0, 0.8f, 0.5f, 1);

	vT.x = -20.0f; vT.y = 10.0f; vT.z = 10.1f;
	mxT = Translate(vT);
	g_BackWall_5 = new CQuad;
	g_BackWall_5->SetShadingMode(GOURAUD_SHADING);
	g_BackWall_5->SetShader();
	g_BackWall_5->SetTRSMatrix(mxT*RotateX(90)*Scale(20, 1, 20));
	g_BackWall_5->SetTiling(1, 1); // 原始為 (10, 10)
	// 設定貼圖
	g_BackWall_5->SetMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_BackWall_5->SetKaKdKsShini(0, 0.8f, 0.5f, 1);

	//第六間--Pokemon
	vT.x = 10.1f; vT.y = 10.0f; vT.z = 20.0;
	mxT = Translate(vT);
	g_LeftWall_6 = new CQuad;
	g_LeftWall_6->SetShadingMode(GOURAUD_SHADING);
	g_LeftWall_6->SetShader();
	g_LeftWall_6->SetTRSMatrix(mxT*RotateX(90.0f)*RotateZ(-90.0f)*Scale(20, 1, 20));
	g_LeftWall_6->SetTiling(1, 1); // 原始為 (10, 10)
	// 設定貼圖
	g_LeftWall_6->SetMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_LeftWall_6->SetKaKdKsShini(0, 0.8f, 0.5f, 1);

	vT.x = 29.9f; vT.y = 10.0f; vT.z = 20.0;
	mxT = Translate(vT);
	g_RightWall_6 = new CQuad;
	g_RightWall_6->SetShadingMode(GOURAUD_SHADING);
	g_RightWall_6->SetShader();
	g_RightWall_6->SetTRSMatrix(mxT*RotateX(90.0f)*RotateZ(90.0f)*Scale(20, 1, 20));
	g_RightWall_6->SetTiling(1, 1); // 原始為 (10, 10)
	// 設定貼圖
	g_RightWall_6->SetMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_RightWall_6->SetKaKdKsShini(0, 0.8f, 0.5f, 1);

	vT.x = 20.0f; vT.y = 10.0f; vT.z = 29.9f;
	mxT = Translate(vT);
	g_FrontWall_6 = new CQuad;
	g_FrontWall_6->SetShadingMode(GOURAUD_SHADING);
	g_FrontWall_6->SetShader();
	g_FrontWall_6->SetTRSMatrix(mxT*RotateX(-90.0f)*RotateY(180)*Scale(20, 1, 20));
	g_FrontWall_6->SetTiling(1, 1); // 原始為 (10, 10)
									// 設定貼圖
	g_FrontWall_6->SetMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_FrontWall_6->SetKaKdKsShini(0, 0.8f, 0.5f, 1);

	vT.x = 20.0f; vT.y = 10.0f; vT.z = 10.1f;
	mxT = Translate(vT);
	g_BackWall_6 = new CQuad;
	g_BackWall_6->SetShadingMode(GOURAUD_SHADING);
	g_BackWall_6->SetShader();
	g_BackWall_6->SetTRSMatrix(mxT*RotateX(90.0f)*Scale(20, 1, 20));
	g_BackWall_6->SetTiling(1, 1); // 原始為 (10, 10)
								   // 設定貼圖
	g_BackWall_6->SetMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_BackWall_6->SetKaKdKsShini(0, 0.8f, 0.5f, 1);

//所有傳送門------------------------------------------------------------------------------------------------------------
	//第一組--巨人~卡丁車
	vT.x = -9.85f; vT.y = 4.0f; vT.z = -7.0f;
	mxT = Translate(vT);
	g_pDoor1 = new CQuad;
	g_pDoor1->SetShadingMode(GOURAUD_SHADING);
	g_pDoor1->SetShader();
	g_pDoor1->SetTRSMatrix(mxT*RotateX(-90.0f)*RotateZ(-90.0f)*Scale(6, 1, 8));
	g_pDoor1->SetTiling(1, 1); // 原始為 (10, 10)
	g_pDoor1->SetMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pDoor1->SetKaKdKsShini(0, 0.8f, 0.5f, 1);

	vT.x = -10.15f; vT.y = 4.0f; vT.z = -7.0f;
	mxT = Translate(vT);
	g_pDoor2 = new CQuad;
	g_pDoor2->SetShadingMode(GOURAUD_SHADING);
	g_pDoor2->SetShader();
	g_pDoor2->SetTRSMatrix(mxT*RotateX(-90.0f)*RotateZ(90.0f)*Scale(6, 1, 8));
	g_pDoor2->SetTiling(1, 1); // 原始為 (10, 10)
	g_pDoor2->SetMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pDoor2->SetKaKdKsShini(0, 0.8f, 0.5f, 1);

	//第二組--卡丁車~遊戲王
	vT.x = -27.0f; vT.y = 4.0f; vT.z = 9.85f;
	mxT = Translate(vT);
	g_pDoor3 = new CQuad;
	g_pDoor3->SetShadingMode(GOURAUD_SHADING);
	g_pDoor3->SetShader();
	g_pDoor3->SetTRSMatrix(mxT*RotateX(-90.0f)*Scale(6, 1, 8));
	g_pDoor3->SetTiling(1, 1); // 原始為 (10, 10)
	g_pDoor3->SetMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pDoor3->SetKaKdKsShini(0, 0.8f, 0.5f, 1);

	vT.x = -27.0f; vT.y = 4.0f; vT.z = 10.15f;
	mxT = Translate(vT);
	g_pDoor4 = new CQuad;
	g_pDoor4->SetShadingMode(GOURAUD_SHADING);
	g_pDoor4->SetShader();
	g_pDoor4->SetTRSMatrix(mxT*RotateX(90.0f)*Scale(6, 1, 8));
	g_pDoor4->SetTiling(1, 1); // 原始為 (10, 10)
	g_pDoor4->SetMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pDoor4->SetKaKdKsShini(0, 0.8f, 0.5f, 1);

	//第三組--遊戲王~詭異之間
	vT.x = -10.15f; vT.y = 4.0f; vT.z = 27.0f;
	mxT = Translate(vT);
	g_pDoor5 = new CQuad;
	g_pDoor5->SetShadingMode(GOURAUD_SHADING);
	g_pDoor5->SetShader();
	g_pDoor5->SetTRSMatrix(mxT*RotateX(-90.0f)*RotateZ(90.0f)*Scale(6, 1, 8));
	g_pDoor5->SetTiling(1, 1); // 原始為 (10, 10)
	g_pDoor5->SetMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pDoor5->SetKaKdKsShini(0, 0.8f, 0.5f, 1);

	vT.x = -9.85f; vT.y = 4.0f; vT.z = 27.0f;
	mxT = Translate(vT);
	g_pDoor6 = new CQuad;
	g_pDoor6->SetShadingMode(GOURAUD_SHADING);
	g_pDoor6->SetShader();
	g_pDoor6->SetTRSMatrix(mxT*RotateX(-90.0f)*RotateZ(-90.0f)*Scale(6, 1, 8));
	g_pDoor6->SetTiling(1, 1); // 原始為 (10, 10)
	g_pDoor6->SetMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pDoor6->SetKaKdKsShini(0, 0.8f, 0.5f, 1);

	//第四組--詭異之間~神奇寶貝
	vT.x = 9.85f; vT.y = 4.0f; vT.z = 27.0f;
	mxT = Translate(vT);
	g_pDoor7 = new CQuad;
	g_pDoor7->SetShadingMode(GOURAUD_SHADING);
	g_pDoor7->SetShader();
	g_pDoor7->SetTRSMatrix(mxT*RotateX(-90.0f)*RotateZ(90.0f)*Scale(6, 1, 8));
	g_pDoor7->SetTiling(1, 1); // 原始為 (10, 10)
	g_pDoor7->SetMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pDoor7->SetKaKdKsShini(0, 0.8f, 0.5f, 1);

	vT.x = 10.15f; vT.y = 4.0f; vT.z = 27.0f;
	mxT = Translate(vT);
	g_pDoor8 = new CQuad;
	g_pDoor8->SetShadingMode(GOURAUD_SHADING);
	g_pDoor8->SetShader();
	g_pDoor8->SetTRSMatrix(mxT*RotateX(-90.0f)*RotateZ(-90.0f)*Scale(6, 1, 8));
	g_pDoor8->SetTiling(1, 1); // 原始為 (10, 10)
	g_pDoor8->SetMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pDoor8->SetKaKdKsShini(0, 0.8f, 0.5f, 1);

	//第五組--神奇寶貝~Deemo
	vT.x = 27.0f; vT.y = 4.0f; vT.z = 10.15f;
	mxT = Translate(vT);
	g_pDoor9 = new CQuad;
	g_pDoor9->SetShadingMode(GOURAUD_SHADING);
	g_pDoor9->SetShader();
	g_pDoor9->SetTRSMatrix(mxT*RotateX(90.0f)*Scale(6, 1, 8));
	g_pDoor9->SetTiling(1, 1); // 原始為 (10, 10)
	g_pDoor9->SetMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pDoor9->SetKaKdKsShini(0, 0.8f, 0.5f, 1);

	vT.x = 27.0f; vT.y = 4.0f; vT.z = 9.85f;
	mxT = Translate(vT);
	g_pDoor10 = new CQuad;
	g_pDoor10->SetShadingMode(GOURAUD_SHADING);
	g_pDoor10->SetShader();
	g_pDoor10->SetTRSMatrix(mxT*RotateX(-90.0f)*Scale(6, 1, 8));
	g_pDoor10->SetTiling(1, 1); // 原始為 (10, 10)
	g_pDoor10->SetMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pDoor10->SetKaKdKsShini(0, 0.8f, 0.5f, 1);

	//第六組--Deemo~巨人
	vT.x = 10.15f; vT.y = 4.0f; vT.z = -7.0f;
	mxT = Translate(vT);
	g_pDoor11 = new CQuad;
	g_pDoor11->SetShadingMode(GOURAUD_SHADING);
	g_pDoor11->SetShader();
	g_pDoor11->SetTRSMatrix(mxT*RotateX(-90.0f)*RotateZ(-90.0f)*Scale(6, 1, 8));
	g_pDoor11->SetTiling(1, 1); // 原始為 (10, 10)
	g_pDoor11->SetMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pDoor11->SetKaKdKsShini(0, 0.8f, 0.5f, 1);

	vT.x = 9.85f; vT.y = 4.0f; vT.z = -7.0f;
	mxT = Translate(vT);
	g_pDoor12 = new CQuad;
	g_pDoor12->SetShadingMode(GOURAUD_SHADING);
	g_pDoor12->SetShader();
	g_pDoor12->SetTRSMatrix(mxT*RotateX(-90.0f)*RotateZ(90.0f)*Scale(6, 1, 8));
	g_pDoor12->SetTiling(1, 1); // 原始為 (10, 10)
	g_pDoor12->SetMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pDoor12->SetKaKdKsShini(0, 0.8f, 0.5f, 1);

	//第七組--巨人~詭異之間
	vT.x = 0.0f; vT.y = 4.0f; vT.z = 9.85f;
	mxT = Translate(vT);
	g_pDoor13 = new CQuad;
	g_pDoor13->SetShadingMode(GOURAUD_SHADING);
	g_pDoor13->SetShader();
	g_pDoor13->SetTRSMatrix(mxT*RotateX(-90.0f)*Scale(6, 1, 8));
	g_pDoor13->SetTiling(1, 1); // 原始為 (10, 10)
	g_pDoor13->SetMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pDoor13->SetKaKdKsShini(0, 0.8f, 0.5f, 1);

	vT.x = 0.0f; vT.y = 4.0f; vT.z = 10.15f;
	mxT = Translate(vT);
	g_pDoor14 = new CQuad;
	g_pDoor14->SetShadingMode(GOURAUD_SHADING);
	g_pDoor14->SetShader();
	g_pDoor14->SetTRSMatrix(mxT*RotateX(90.0f)*Scale(6, 1, 8));
	g_pDoor14->SetTiling(1, 1); // 原始為 (10, 10)
	g_pDoor14->SetMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pDoor14->SetKaKdKsShini(0, 0.8f, 0.5f, 1);


//其他物件------------------------------------------------------------------------------------------------------------
	g_pCube = new CSolidCube;
	g_pCube->SetTextureLayer( DIFFUSE_MAP | NORMAL_MAP);
	g_pCube->SetShaderName("vsNormalMapLighting.glsl", "fsNormalMapLighting.glsl");
	g_pCube->SetShader();
	// 設定 Cube
	vT.x = 0.0f; vT.y = 1.0f; vT.z = 0.0f;
	mxT = Translate(vT);
	mxT._m[0][0] = 2.0f; mxT._m[1][1] = 2.0f; mxT._m[2][2] = 2.0f;
	g_pCube->SetTRSMatrix(mxT );
	g_pCube->SetShadingMode(GOURAUD_SHADING);
	// materials
	g_pCube->SetMaterials(vec4(0.35f, 0.35f, 0.35f, 1), vec4(0.85f, 0.85f, 0.85f,1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pCube->SetKaKdKsShini(0.25f, 0.8f, 0.2f, 2);

	g_pEarth = new CSolidSphere(1, 16, 16);
	g_pEarth->SetTextureLayer(DIFFUSE_MAP);
	g_pEarth->SetShader();
	// 設定 Sphere
	vT.x = 0.0; vT.y = 5.0; vT.z = 20.0;
	mxT = Translate(vT)* RotateX(-90) * Scale(2.0f, 2.0f, 2.0f);
	g_pEarth->SetTRSMatrix(mxT);
	g_pEarth->SetShadingMode(GOURAUD_SHADING);
	// materials
	g_pEarth->SetMaterials(vec4(0.15f, 0.15f, 0.15f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pEarth->SetKaKdKsShini(0.15f, 0.9f, 0.1f, 2);

	g_pPokemonBall = new CSolidSphere(1, 16, 16);
	g_pPokemonBall->SetTextureLayer(DIFFUSE_MAP);
	g_pPokemonBall->SetShader();

	// 設定 Sphere
	vT.x = 20.0; vT.y = 2.0; vT.z = 20.0;
	mxT = Translate(vT)* RotateX(270);
	g_pPokemonBall->SetTRSMatrix(mxT);
	g_pPokemonBall->SetShadingMode(GOURAUD_SHADING);
	// materials
	g_pPokemonBall->SetMaterials(vec4(0.15f, 0.15f, 0.15f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pPokemonBall->SetKaKdKsShini(0.15f, 0.9f, 0.1f, 2);

	//ObjLoader_1---Start
	g_pObjLoader = new ObjLoader("model/streetlight.obj");
	g_pObjLoader->SetShader();
	vT.x = 26.0; vT.y = 0.0; vT.z = 6.0;
	mxT = Translate(vT)*RotateX(-90);
	g_pObjLoader->SetTRSMatrix(mxT*Scale(0.03, 0.03, 0.03));
	g_pObjLoader->SetShadingMode(GOURAUD_SHADING);
	g_pObjLoader->SetMaterials(vec4(0.35f, 0.35f, 0.35f, 1.0f), vec4(0.85, 0.85, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pObjLoader->SetKaKdKsShini(0.25f, 0.8f, 0.2f, 2);
	//ObjLoader_1---End

	//ObjLoader_2---Start
	g_pObjLoader2 = new ObjLoader("model/bench.obj");
	g_pObjLoader2->SetShader();
	vT.x = 20.0; vT.y = 2.0; vT.z = 4.0;
	mxT = Translate(vT)*RotateX(-90)*RotateZ(180);
	g_pObjLoader2->SetTRSMatrix(mxT*Scale(0.1, 0.1, 0.1));
	g_pObjLoader2->SetShadingMode(GOURAUD_SHADING);
	g_pObjLoader2->SetMaterials(vec4(0.35f, 0.35f, 0.35f, 1.0f), vec4(0.85, 0.85, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pObjLoader2->SetKaKdKsShini(0.25f, 0.8f, 0.2f, 2);
	//ObjLoader_2---End

	g_pCubeMap = new CSolidCube;
	g_pCubeMap->SetTextureLayer(DIFFUSE_MAP);
	g_pCubeMap->SetCubeMapTexName(1);
	g_pCubeMap->SetViewPosition(eye);
	g_pCubeMap->SetShaderName("vsCubeMapping.glsl", "fsCubeMapping.glsl");
	g_pCubeMap->SetShader();
	// 設定 Cube
	vT.x = 40.0f; vT.y = 10.0f; vT.z = -20.0f;
	mxT = Translate(vT);
	mxT._m[0][0] = 20.0f; mxT._m[1][1] = 20.0f; mxT._m[2][2] = 20.0f;
	g_pCubeMap->SetTRSMatrix(mxT);
	g_pCubeMap->SetShadingMode(GOURAUD_SHADING);
	// materials
	g_pCubeMap->SetMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pCubeMap->SetKaKdKsShini(0, 0.8f, 0.5f, 1);
	g_pCubeMap->SetColor(vec4(0.9f, 0.9f, 0.9f, 1.0f));

	g_pYugioh_Cube[0] = new CSolidCube;
	g_pYugioh_Cube[0]->SetTextureLayer(DIFFUSE_MAP | NORMAL_MAP);
	g_pYugioh_Cube[0]->SetShader();
	// 設定 Cube
	vT.x = -25.5f; vT.y = 0.32f; vT.z = 17.0f;
	mxT = Translate(vT);
	mxT._m[0][0] = 2.0f; mxT._m[1][1] = 0.02f; mxT._m[2][2] = 3.0f;
	g_pYugioh_Cube[0]->SetTRSMatrix(mxT);
	g_pYugioh_Cube[0]->SetShadingMode(GOURAUD_SHADING);
	// materials
	g_pYugioh_Cube[0]->SetMaterials(vec4(0.35f, 0.35f, 0.35f, 1), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pYugioh_Cube[0]->SetKaKdKsShini(0.25f, 0.8f, 0.2f, 2);

	g_pYugioh_Cube[3] = new CSolidCube;
	g_pYugioh_Cube[3]->SetTextureLayer(DIFFUSE_MAP | NORMAL_MAP);
	g_pYugioh_Cube[3]->SetShader();
	// 設定 Cube
	vT.x = -25.5f; vT.y = 0.3f; vT.z = 17.0f;
	mxT = Translate(vT);
	mxT._m[0][0] = 2.0f; mxT._m[1][1] = 0.02f; mxT._m[2][2] = 3.0f;
	g_pYugioh_Cube[3]->SetTRSMatrix(mxT);
	g_pYugioh_Cube[3]->SetShadingMode(GOURAUD_SHADING);
	// materials
	g_pYugioh_Cube[3]->SetMaterials(vec4(0.35f, 0.35f, 0.35f, 1), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pYugioh_Cube[3]->SetKaKdKsShini(0.25f, 0.8f, 0.2f, 2);
	
	g_pYugioh_Cube[1] = new CSolidCube;
	g_pYugioh_Cube[1]->SetTextureLayer(DIFFUSE_MAP | NORMAL_MAP);
	g_pYugioh_Cube[1]->SetShader();
	// 設定 Cube
	mxT = Translate(g_Card_M_Fornt.position);
	//mxT._m[0][0] = 2.0f; mxT._m[1][1] = 0.02f; mxT._m[2][2] = 3.0f;
	g_pYugioh_Cube[1]->SetTRSMatrix(mxT*RotateY(180)*Scale(2.0,0.02,3.0));
	g_pYugioh_Cube[1]->SetShadingMode(GOURAUD_SHADING);
	// materials
	g_pYugioh_Cube[1]->SetMaterials(vec4(0.35f, 0.35f, 0.35f, 1), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pYugioh_Cube[1]->SetKaKdKsShini(0.25f, 0.8f, 0.2f, 2);

	g_pYugioh_Cube[4] = new CSolidCube;
	g_pYugioh_Cube[4]->SetTextureLayer(DIFFUSE_MAP | NORMAL_MAP);
	g_pYugioh_Cube[4]->SetShader();
	// 設定 Cube
	mxT = Translate(g_Card_M_Back.position);
	//mxT._m[0][0] = 2.0f; mxT._m[1][1] = 0.02f; mxT._m[2][2] = 3.0f;
	g_pYugioh_Cube[4]->SetTRSMatrix(mxT*Scale(2.0, 0.02, 3.0));
	g_pYugioh_Cube[4]->SetShadingMode(GOURAUD_SHADING);
	// materials
	g_pYugioh_Cube[4]->SetMaterials(vec4(0.35f, 0.35f, 0.35f, 1), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pYugioh_Cube[4]->SetKaKdKsShini(0.25f, 0.8f, 0.2f, 2);

	g_pYugioh_Cube[2] = new CSolidCube;
	g_pYugioh_Cube[2]->SetTextureLayer(DIFFUSE_MAP | NORMAL_MAP);
	g_pYugioh_Cube[2]->SetShader();
	// 設定 Cube
	vT.x = -20.0f; vT.y = 0.3f; vT.z = 13.1f;
	mxT = Translate(vT);
	mxT._m[0][0] = 2.0f; mxT._m[1][1] = 0.02f; mxT._m[2][2] = 3.0f;
	g_pYugioh_Cube[2]->SetTRSMatrix(mxT);
	g_pYugioh_Cube[2]->SetShadingMode(GOURAUD_SHADING);
	// materials
	g_pYugioh_Cube[2]->SetMaterials(vec4(0.35f, 0.35f, 0.35f, 1), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pYugioh_Cube[2]->SetKaKdKsShini(0.25f, 0.8f, 0.2f, 2);

	g_pYugioh_Cube[5] = new CSolidCube;
	g_pYugioh_Cube[5]->SetTextureLayer(DIFFUSE_MAP | NORMAL_MAP);
	g_pYugioh_Cube[5]->SetShader();
	// 設定 Cube
	vT.x = -20.0f; vT.y = 0.32f; vT.z = 13.1f;
	mxT = Translate(vT);
	mxT._m[0][0] = 2.0f; mxT._m[1][1] = 0.02f; mxT._m[2][2] = 3.0f;
	g_pYugioh_Cube[5]->SetTRSMatrix(mxT);
	g_pYugioh_Cube[5]->SetShadingMode(GOURAUD_SHADING);
	// materials
	g_pYugioh_Cube[5]->SetMaterials(vec4(0.35f, 0.35f, 0.35f, 1), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pYugioh_Cube[5]->SetKaKdKsShini(0.25f, 0.8f, 0.2f, 2);

	// For 無垢
	g_pAimal[2] = new CQuad;
#if MULTITEXTURE >= DIFFUSE_MAP | NORMAL_MAP 
	g_pAimal[2]->SetTextureLayer(DIFFUSE_MAP | NORMAL_MAP);
	g_pAimal[2]->SetShaderName("vsNormalMapLighting.glsl", "fsNormalMapLighting.glsl");
#endif

	g_pAimal[2]->SetShader();
	mxT = Translate(-5.0f, 3.0f, 0.0f) * RotateX(90)*RotateZ(-90)* Scale(8, 8, 8);
	g_pAimal[2]->SetTRSMatrix(mxT);
	g_pAimal[2]->SetShadingMode(GOURAUD_SHADING);
	// 設定貼圖
	g_pAimal[2]->SetMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pAimal[2]->SetKaKdKsShini(0, 0.8f, 0.5f, 1);

	// For 亞妮
	g_pAimal[1] = new CQuad;

#if MULTITEXTURE >= DIFFUSE_MAP | NORMAL_MAP
	g_pAimal[1]->SetTextureLayer(DIFFUSE_MAP | NORMAL_MAP);
	g_pAimal[1]->SetShaderName("vsNormalMapLighting.glsl", "fsNormalMapLighting.glsl");
#endif 

	g_pAimal[1]->SetShader();
	mxT = Translate(0.0f, 3.5f, 5.0f) * RotateX(90) * Scale(8, 8, 8);
	g_pAimal[1]->SetTRSMatrix(mxT);
	g_pAimal[1]->SetShadingMode(GOURAUD_SHADING);
	// 設定貼圖
	g_pAimal[1]->SetMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pAimal[1]->SetKaKdKsShini(0, 0.8f, 0.5f, 1);

	// For 萊納
	g_pAimal[0] = new CQuad;

#if MULTITEXTURE >= (DIFFUSE_MAP | NORMAL_MAP)
	g_pAimal[0]->SetTextureLayer(DIFFUSE_MAP | NORMAL_MAP);
	g_pAimal[0]->SetShaderName("vsNormalMapLighting.glsl", "fsNormalMapLighting.glsl");
#endif

	g_pAimal[0]->SetShader();
	mxT = Translate(5.0, 3.5f, 0.0f) * RotateX(90)*RotateZ(90)*Scale(8, 8, 8);
	g_pAimal[0]->SetTRSMatrix(mxT);
	g_pAimal[0]->SetShadingMode(GOURAUD_SHADING);
	// 設定貼圖
	g_pAimal[0]->SetMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pAimal[0]->SetKaKdKsShini(0, 0.8f, 0.5f, 1);

	// For g_pDeemo3
	g_pDeemo[2] = new CQuad;
#if MULTITEXTURE >= DIFFUSE_MAP | NORMAL_MAP 
	g_pDeemo[2]->SetTextureLayer(DIFFUSE_MAP | NORMAL_MAP);
#endif

	g_pDeemo[2]->SetShader();
	mxT = Translate(14.0, 3.0f, -2.5f) * RotateX(90)* RotateZ(-30) * Scale(6, 6, 6);
	g_pDeemo[2]->SetTRSMatrix(mxT);
	g_pDeemo[2]->SetShadingMode(GOURAUD_SHADING);
	// 設定貼圖
	g_pDeemo[2]->SetMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pDeemo[2]->SetKaKdKsShini(0, 0.8f, 0.5f, 1);

	// For g_pDeemo2
	g_pDeemo[1] = new CQuad;

#if MULTITEXTURE >= DIFFUSE_MAP | NORMAL_MAP
	g_pDeemo[1]->SetTextureLayer(DIFFUSE_MAP | NORMAL_MAP);
#endif 

	g_pDeemo[1]->SetShader();
	mxT = Translate(19.0f, 2.5f, -4.0f) * RotateX(90) * Scale(6, 6, 6);
	g_pDeemo[1]->SetTRSMatrix(mxT);
	g_pDeemo[1]->SetShadingMode(GOURAUD_SHADING);
	// 設定貼圖
	g_pDeemo[1]->SetMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pDeemo[1]->SetKaKdKsShini(0, 0.8f, 0.5f, 1);

	// For g_pDeemo1
	g_pDeemo[0] = new CQuad;
#if MULTITEXTURE >= (DIFFUSE_MAP | NORMAL_MAP)
	g_pDeemo[0]->SetTextureLayer(DIFFUSE_MAP | NORMAL_MAP);
#endif
	g_pDeemo[0]->SetShader();
	mxT = Translate(26.0, 2.5f, -2.5f) * RotateX(90)* RotateZ(30) *Scale(6, 6, 6);
	g_pDeemo[0]->SetTRSMatrix(mxT);
	g_pDeemo[0]->SetShadingMode(GOURAUD_SHADING);
	// 設定貼圖
	g_pDeemo[0]->SetMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pDeemo[0]->SetKaKdKsShini(0, 0.8f, 0.5f, 1);

	//For 甲賀忍蛙
	g_pPokemon[0] = new CQuad;
#if MULTITEXTURE >= (DIFFUSE_MAP | NORMAL_MAP)
	g_pPokemon[0]->SetTextureLayer(DIFFUSE_MAP | NORMAL_MAP);
#endif
	g_pPokemon[0]->SetShader();
	mxT = Translate(16.0, 2.5f, 16.0) * RotateX(90)* RotateZ(-30)*Scale(5, 5, 5);
	g_pPokemon[0]->SetTRSMatrix(mxT);
	g_pPokemon[0]->SetShadingMode(GOURAUD_SHADING);
	// 設定貼圖
	g_pPokemon[0]->SetMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pPokemon[0]->SetKaKdKsShini(0, 0.8f, 0.5f, 1);

	//For 音波龍
	g_pPokemon[1] = new CQuad;
#if MULTITEXTURE >= (DIFFUSE_MAP | NORMAL_MAP)
	g_pPokemon[1]->SetTextureLayer(DIFFUSE_MAP | NORMAL_MAP);
#endif
	g_pPokemon[1]->SetShader();
	mxT = Translate(24.0, 2.5f, 16.0f) * RotateX(90)* RotateZ(30)*Scale(5, 5, 5);
	g_pPokemon[1]->SetTRSMatrix(mxT);
	g_pPokemon[1]->SetShadingMode(GOURAUD_SHADING);
	// 設定貼圖
	g_pPokemon[1]->SetMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pPokemon[1]->SetKaKdKsShini(0, 0.8f, 0.5f, 1);

	//For 急凍鳥
	g_pPokemon[2] = new CQuad;
#if MULTITEXTURE >= (DIFFUSE_MAP | NORMAL_MAP)
	g_pPokemon[2]->SetTextureLayer(DIFFUSE_MAP | NORMAL_MAP);
#endif
	g_pPokemon[2]->SetShader();
	mxT = Translate(12.0, 2.5f, 20.0f) * RotateX(90)* RotateZ(-90)*Scale(5, 5, 5);
	g_pPokemon[2]->SetTRSMatrix(mxT);
	g_pPokemon[2]->SetShadingMode(GOURAUD_SHADING);
	// 設定貼圖
	g_pPokemon[2]->SetMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pPokemon[2]->SetKaKdKsShini(0, 0.8f, 0.5f, 1);

	//For 噴火龍
	g_pPokemon[3] = new CQuad;
#if MULTITEXTURE >= (DIFFUSE_MAP | NORMAL_MAP)
	g_pPokemon[3]->SetTextureLayer(DIFFUSE_MAP | NORMAL_MAP);
#endif
	g_pPokemon[3]->SetShader();
	mxT = Translate(28.0, 2.5f, 20.0f) * RotateX(90)* RotateZ(90)*Scale(5, 5, 5);
	g_pPokemon[3]->SetTRSMatrix(mxT);
	g_pPokemon[3]->SetShadingMode(GOURAUD_SHADING);
	// 設定貼圖
	g_pPokemon[3]->SetMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pPokemon[3]->SetKaKdKsShini(0, 0.8f, 0.5f, 1);

	//For 蜥蜴王
	g_pPokemon[4] = new CQuad;
#if MULTITEXTURE >= (DIFFUSE_MAP | NORMAL_MAP)
	g_pPokemon[4]->SetTextureLayer(DIFFUSE_MAP | NORMAL_MAP);
#endif
	g_pPokemon[4]->SetShader();
	mxT = Translate(16.0, 2.5f, 24.0f) * RotateX(90)* RotateZ(-150)*Scale(5, 5, 5);
	g_pPokemon[4]->SetTRSMatrix(mxT);
	g_pPokemon[4]->SetShadingMode(GOURAUD_SHADING);
	// 設定貼圖
	g_pPokemon[4]->SetMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pPokemon[4]->SetKaKdKsShini(0, 0.8f, 0.5f, 1);

	//For 化石翼龍
	g_pPokemon[5] = new CQuad;
#if MULTITEXTURE >= (DIFFUSE_MAP | NORMAL_MAP)
	g_pPokemon[5]->SetTextureLayer(DIFFUSE_MAP | NORMAL_MAP);
#endif
	g_pPokemon[5]->SetShader();
	mxT = Translate(24.0, 2.5f, 24.0f) * RotateX(90)* RotateZ(150)*Scale(5, 5, 5);
	g_pPokemon[5]->SetTRSMatrix(mxT);
	g_pPokemon[5]->SetShadingMode(GOURAUD_SHADING);
	// 設定貼圖
	g_pPokemon[5]->SetMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pPokemon[5]->SetKaKdKsShini(0, 0.8f, 0.5f, 1);

	// For g_pKart4
	g_pKart[3] = new CQuad;
#if MULTITEXTURE >= DIFFUSE_MAP | NORMAL_MAP 
	g_pKart[3]->SetTextureLayer(DIFFUSE_MAP | NORMAL_MAP);
#endif
	g_pKart[3]->SetShaderName("vsNormalMapLighting.glsl", "fsNormalMapLighting.glsl");
	g_pKart[3]->SetShader();
	mxT = Translate(-27.0f, 1.5f, 0.0f) * RotateX(95)* RotateZ(270) * Scale(4.5, 1, 3.75);
	g_pKart[3]->SetTRSMatrix(mxT);
	g_pKart[3]->SetShadingMode(GOURAUD_SHADING);
	// 設定貼圖
	g_pKart[3]->SetMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pKart[3]->SetKaKdKsShini(0, 0.8f, 0.5f, 1);

	// For g_pKart3
	g_pKart[2] = new CQuad;
#if MULTITEXTURE >= DIFFUSE_MAP | NORMAL_MAP 
	g_pKart[2]->SetTextureLayer(DIFFUSE_MAP | NORMAL_MAP);
#endif
	g_pKart[2]->SetShaderName("vsNormalMapLighting.glsl", "fsNormalMapLighting.glsl");
	g_pKart[2]->SetShader();
	mxT = Translate(-20.0f, 1.5f, -7.0f) * RotateX(95) * Scale(4.5, 1, 3.75);
	g_pKart[2]->SetTRSMatrix(mxT);
	g_pKart[2]->SetShadingMode(GOURAUD_SHADING);
	// 設定貼圖
	g_pKart[2]->SetMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pKart[2]->SetKaKdKsShini(0, 0.8f, 0.5f, 1);

	// For g_pKart2
	g_pKart[1] = new CQuad;

#if MULTITEXTURE >= DIFFUSE_MAP | NORMAL_MAP
	g_pKart[1]->SetTextureLayer(DIFFUSE_MAP | NORMAL_MAP);
#endif 
	g_pKart[1]->SetShaderName("vsNormalMapLighting.glsl", "fsNormalMapLighting.glsl");
	g_pKart[1]->SetShader();
	mxT = Translate(-13.0f, 1.5f, 0.0f) * RotateX(95)* RotateZ(90) * Scale(4.5, 1, 3);
	g_pKart[1]->SetTRSMatrix(mxT);
	g_pKart[1]->SetShadingMode(GOURAUD_SHADING);
	// 設定貼圖
	g_pKart[1]->SetMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pKart[1]->SetKaKdKsShini(0, 0.8f, 0.5f, 1);

	// For g_pKart1
	g_pKart[0] = new CQuad;

#if MULTITEXTURE >= (DIFFUSE_MAP | NORMAL_MAP)
	g_pKart[0]->SetTextureLayer(DIFFUSE_MAP | NORMAL_MAP);
#endif
	g_pKart[0]->SetShaderName("vsNormalMapLighting.glsl", "fsNormalMapLighting.glsl");
	g_pKart[0]->SetShader();
	mxT = Translate(-20.0f, 1.5f, 7.0f) * RotateX(95)* RotateZ(180) *Scale(4.5, 1, 3);
	g_pKart[0]->SetTRSMatrix(mxT);
	g_pKart[0]->SetShadingMode(GOURAUD_SHADING);
	// 設定貼圖
	g_pKart[0]->SetMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pKart[0]->SetKaKdKsShini(0, 0.8f, 0.5f, 1);

	// For 黑魔導
	g_pYugioh[0] = new CQuad;
#if MULTITEXTURE >= DIFFUSE_MAP | NORMAL_MAP
	g_pYugioh[0]->SetTextureLayer(DIFFUSE_MAP | NORMAL_MAP);
#endif 
	g_pYugioh[0]->SetShaderName("vsNormalMapLighting.glsl", "fsNormalMapLighting.glsl");
	g_pYugioh[0]->SetShader();
	mxT = Translate(-25.5f, 2.0f, 19.0f) * RotateX(90)* Scale(3.5, 3.5, 3.5);
	g_pYugioh[0]->SetTRSMatrix(mxT);
	g_pYugioh[0]->SetShadingMode(GOURAUD_SHADING);
	// 設定貼圖
	g_pYugioh[0]->SetMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pYugioh[0]->SetKaKdKsShini(0, 0.8f, 0.5f, 1);

	// For 青眼白龍
	g_pYugioh[1] = new CQuad;
#if MULTITEXTURE >= (DIFFUSE_MAP | NORMAL_MAP)
	g_pYugioh[1]->SetTextureLayer(DIFFUSE_MAP | NORMAL_MAP);
#endif
	g_pYugioh[1]->SetShaderName("vsNormalMapLighting.glsl", "fsNormalMapLighting.glsl");
	g_pYugioh[1]->SetShader();
	mxT = Translate(g_Monster.position) * RotateX(90) * Scale(3.5, 3.5, 3.5);
	g_pYugioh[1]->SetTRSMatrix(mxT);
	g_pYugioh[1]->SetShadingMode(GOURAUD_SHADING);
	// 設定貼圖
	g_pYugioh[1]->SetMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pYugioh[1]->SetKaKdKsShini(0, 0.8f, 0.5f, 1);


	// For Reflecting Sphere
	g_pSphere = new CSolidSphere(1.0f, 24, 12);
	g_pSphere->SetTextureLayer(DIFFUSE_MAP);  // 使用 
	g_pSphere->SetCubeMapTexName(1);
	g_pSphere->SetViewPosition(eye);
	g_pSphere->SetShaderName("vsCubeMapping.glsl", "fsCubeMapping.glsl");
	g_pSphere->SetShader();
	vT.x = 0.0f; vT.y = 2.0f; vT.z = -15.0f;
	mxT = Translate(vT);
	mxT._m[0][0] = 2.0f; mxT._m[1][1] = 2.0f; mxT._m[2][2] = 2.0f;
	g_pSphere->SetTRSMatrix(mxT*RotateX(90.0f));
	g_pSphere->SetShadingMode(GOURAUD_SHADING);
	// 設定貼圖
	g_pSphere->SetMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pSphere->SetKaKdKsShini(0, 0.8f, 0.5f, 1);
	g_pSphere->SetColor(vec4(0.9f, 0.9f, 0.9f, 1.0f));

	// 設定 代表 Light 的 WireSphere
	g_pLight = new CWireSphere(0.25f, 6, 3);
	g_pLight->SetLightingDisable();
	g_pLight->SetTextureLayer(NONE_MAP);	// 沒有貼圖
	g_pLight->SetShader();
	mxT = Translate(g_Light1.position);
	g_pLight->SetTRSMatrix(mxT);
	g_pLight->SetColor(g_Light1.diffuse);

	// 設定 代表 Light2 的 WireSphere
	g_pLight2 = new CWireSphere(0.25f, 6, 3);
	g_pLight2->SetLightingDisable();
	g_pLight2->SetTextureLayer(NONE_MAP);	// 沒有貼圖
	g_pLight2->SetShader();
	mxT = Translate(g_Light2.position);
	g_pLight2->SetTRSMatrix(mxT);
	g_pLight2->SetColor(g_Light2.diffuse);

	// 設定 代表 Light3 的 WireSphere
	g_pLight3 = new CWireSphere(0.25f, 6, 3);
	g_pLight3->SetLightingDisable();
	g_pLight3->SetTextureLayer(NONE_MAP);	// 沒有貼圖
	g_pLight3->SetShader();
	mxT = Translate(g_Light3.position);
	g_pLight3->SetTRSMatrix(mxT);
	g_pLight3->SetColor(g_Light3.diffuse);

	// 設定 代表 Light4 的 WireSphere
	g_pLight4 = new CWireSphere(0.25f, 6, 3);
	g_pLight4->SetLightingDisable();
	g_pLight4->SetTextureLayer(NONE_MAP);	// 沒有貼圖
	g_pLight4->SetShader();
	mxT = Translate(g_Light4.position);
	g_pLight4->SetTRSMatrix(mxT);
	g_pLight4->SetColor(g_Light4.diffuse);

	// 設定 代表 Light5 的 WireSphere
	g_pLight5 = new CWireSphere(0.25f, 6, 3);
	g_pLight5->SetLightingDisable();
	g_pLight5->SetTextureLayer(NONE_MAP);	// 沒有貼圖
	g_pLight5->SetShader();
	mxT = Translate(g_Light5.position);
	g_pLight5->SetTRSMatrix(mxT);
	g_pLight5->SetColor(g_Light5.diffuse);

	// 設定 代表 Light6 的 WireSphere
	g_pLight6 = new CWireSphere(0.25f, 6, 3);
	g_pLight6->SetLightingDisable();
	g_pLight6->SetTextureLayer(NONE_MAP);	// 沒有貼圖
	g_pLight6->SetShader();
	mxT = Translate(g_Light6.position);
	g_pLight6->SetTRSMatrix(mxT);
	g_pLight6->SetColor(g_Light6.diffuse);

	// 設定 代表 Trap7 的 WireSphere
	g_pTrap7 = new CWireSphere(0.25f, 18, 3);
	g_pTrap7->SetLightingDisable();
	g_pTrap7->SetTextureLayer(NONE_MAP);	// 沒有貼圖
	g_pTrap7->SetShader();
	mxT = Translate(g_Light7.position)*Scale(2,2,2);
	g_pTrap7->SetTRSMatrix(mxT);
	g_pTrap7->SetColor(g_Light7.diffuse);

	// 設定 代表 Trap8 的 WireSphere
	g_pTrap8 = new CWireSphere(0.25f, 18, 3);
	g_pTrap8->SetLightingDisable();
	g_pTrap8->SetTextureLayer(NONE_MAP);	// 沒有貼圖
	g_pTrap8->SetShader();
	mxT = Translate(g_Light8.position)*Scale(2, 2, 2);
	g_pTrap8->SetTRSMatrix(mxT);
	g_pTrap8->SetColor(g_Light8.diffuse);

	// 設定 代表 Trap9 的 WireSphere
	g_pTrap9 = new CWireSphere(0.25f, 18, 3);
	g_pTrap9->SetLightingDisable();
	g_pTrap9->SetTextureLayer(NONE_MAP);	// 沒有貼圖
	g_pTrap9->SetShader();
	mxT = Translate(g_Light9.position)*Scale(2, 2, 2);
	g_pTrap9->SetTRSMatrix(mxT);
	g_pTrap9->SetColor(g_Light9.diffuse);

	// 設定 代表 EnergyBall 的 WireSphere
	g_pEnergyBall = new CWireSphere(0.25f, 24, 3);
	g_pEnergyBall->SetLightingDisable();
	g_pEnergyBall->SetTextureLayer(NONE_MAP);	// 沒有貼圖
	g_pEnergyBall->SetShader();
	mxT = Translate(g_Light10.position);
	g_pEnergyBall->SetTRSMatrix(mxT);
	g_pEnergyBall->SetColor(g_Light10.diffuse);

	//2D介面
	mat4 mxT2D, mxS2D;
	vec4 vColor2D = vec4(0, 0, 0, 1);

	g_p2DKnife = new C2DSprite; g_p2DKnife->SetShader_2D_Cut();
	vColor2D.x = 0.81; vColor2D.y = 0.81; vColor2D.z = 0.81; vColor2D.w = 0.5; g_p2DKnife->SetDefaultColor(vColor2D);
	mxS2D = Scale(0.1f, 0.1f, 1.0f);
	mxT2D = Translate(0.0f, 0.0f, 0);
	g_p2DKnife->SetTRSMatrix(mxT2D*mxS2D);
	g_p2DKnife->SetViewMatrix(g_2DView);
	g_p2DKnife->SetViewMatrix(g_2DProj);


	// 因為本範例不會動到 Projection Matrix 所以在這裡設定一次即可
	// 就不寫在 OnFrameMove 中每次都 Check
	bool bPDirty;
	mat4 mpx = camera->getProjectionMatrix(bPDirty);
	g_pFloor->SetProjectionMatrix(mpx);
	g_pFloor2->SetProjectionMatrix(mpx);
	g_pFloor3->SetProjectionMatrix(mpx);
	g_pFloor4->SetProjectionMatrix(mpx);
	g_pFloor5->SetProjectionMatrix(mpx);
	g_pFloor6->SetProjectionMatrix(mpx);
	g_FrontWall_1->SetProjectionMatrix(mpx);
	g_BackWall_1->SetProjectionMatrix(mpx);
	g_LeftWall_1->SetProjectionMatrix(mpx);
	g_RightWall_1->SetProjectionMatrix(mpx);
	g_FrontWall_2->SetProjectionMatrix(mpx);
	g_BackWall_2->SetProjectionMatrix(mpx);
	g_LeftWall_2->SetProjectionMatrix(mpx);
	g_RightWall_2->SetProjectionMatrix(mpx);
	g_FrontWall_3->SetProjectionMatrix(mpx);
	g_BackWall_3->SetProjectionMatrix(mpx);
	g_LeftWall_3->SetProjectionMatrix(mpx);
	g_RightWall_3->SetProjectionMatrix(mpx);
	g_FrontWall_4->SetProjectionMatrix(mpx);
	g_BackWall_4->SetProjectionMatrix(mpx);
	g_LeftWall_4->SetProjectionMatrix(mpx);
	g_RightWall_4->SetProjectionMatrix(mpx);
	g_FrontWall_5->SetProjectionMatrix(mpx);
	g_BackWall_5->SetProjectionMatrix(mpx);
	g_LeftWall_5->SetProjectionMatrix(mpx);
	g_RightWall_5->SetProjectionMatrix(mpx);
	g_FrontWall_6->SetProjectionMatrix(mpx);
	g_BackWall_6->SetProjectionMatrix(mpx);
	g_LeftWall_6->SetProjectionMatrix(mpx);
	g_RightWall_6->SetProjectionMatrix(mpx);
	g_pDoor1->SetProjectionMatrix(mpx);
	g_pDoor2->SetProjectionMatrix(mpx);
	g_pDoor3->SetProjectionMatrix(mpx);
	g_pDoor4->SetProjectionMatrix(mpx);
	g_pDoor5->SetProjectionMatrix(mpx);
	g_pDoor6->SetProjectionMatrix(mpx);
	g_pDoor7->SetProjectionMatrix(mpx);
	g_pDoor8->SetProjectionMatrix(mpx);
	g_pDoor9->SetProjectionMatrix(mpx);
	g_pDoor10->SetProjectionMatrix(mpx);
	g_pDoor11->SetProjectionMatrix(mpx);
	g_pDoor12->SetProjectionMatrix(mpx);
	g_pDoor13->SetProjectionMatrix(mpx);
	g_pDoor14->SetProjectionMatrix(mpx);
	g_pCube->SetProjectionMatrix(mpx);
	g_pSphere->SetProjectionMatrix(mpx);
	g_pEarth->SetProjectionMatrix(mpx);
	g_pPokemonBall->SetProjectionMatrix(mpx);
	g_pEnergyBall->SetProjectionMatrix(mpx);
	g_pCubeMap->SetProjectionMatrix(mpx);
	g_pObjLoader->SetProjectionMatrix(mpx);
	g_pObjLoader2->SetProjectionMatrix(mpx);
	g_pAimal[0]->SetProjectionMatrix(mpx);
	g_pAimal[1]->SetProjectionMatrix(mpx);
	g_pAimal[2]->SetProjectionMatrix(mpx);
	g_pDeemo[0]->SetProjectionMatrix(mpx);
	g_pDeemo[1]->SetProjectionMatrix(mpx);
	g_pDeemo[2]->SetProjectionMatrix(mpx);
	g_pPokemon[0]->SetProjectionMatrix(mpx);
	g_pPokemon[1]->SetProjectionMatrix(mpx);
	g_pPokemon[2]->SetProjectionMatrix(mpx);
	g_pPokemon[3]->SetProjectionMatrix(mpx);
	g_pPokemon[4]->SetProjectionMatrix(mpx);
	g_pPokemon[5]->SetProjectionMatrix(mpx);
	g_pKart[0]->SetProjectionMatrix(mpx);
	g_pKart[1]->SetProjectionMatrix(mpx);
	g_pKart[2]->SetProjectionMatrix(mpx);
	g_pKart[3]->SetProjectionMatrix(mpx);
	g_pYugioh[0]->SetProjectionMatrix(mpx);
	g_pYugioh[1]->SetProjectionMatrix(mpx);
	g_pYugioh_Cube[0]->SetProjectionMatrix(mpx);
	g_pYugioh_Cube[3]->SetProjectionMatrix(mpx);
	g_pYugioh_Cube[1]->SetProjectionMatrix(mpx);
	g_pYugioh_Cube[4]->SetProjectionMatrix(mpx);
	g_pYugioh_Cube[2]->SetProjectionMatrix(mpx);
	g_pYugioh_Cube[5]->SetProjectionMatrix(mpx);
	g_pLight->SetProjectionMatrix(mpx);
	g_pLight2->SetProjectionMatrix(mpx);
	g_pLight3->SetProjectionMatrix(mpx);
	g_pLight4->SetProjectionMatrix(mpx);
	g_pLight5->SetProjectionMatrix(mpx);
	g_pLight6->SetProjectionMatrix(mpx);
	g_pTrap7->SetProjectionMatrix(mpx);
	g_pTrap8->SetProjectionMatrix(mpx);
	g_pTrap9->SetProjectionMatrix(mpx);
}

//----------------------------------------------------------------------------
void GL_Display( void )
{

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); // clear the window

	glEnable(GL_BLEND);  // 設定2D Texure Mapping 有作用
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
#ifndef  MULTITEXTURE
	glBindTexture(GL_TEXTURE_2D, g_uiFTexID[0]); 
	g_pFloor->Draw();
#else 
	glActiveTexture(GL_TEXTURE0); // select active texture 0
	glBindTexture(GL_TEXTURE_2D, g_uiFTexID[7]); // 與 Diffuse Map 結合
	glActiveTexture(GL_TEXTURE1); // select active texture 1
	glBindTexture(GL_TEXTURE_2D, g_uiFTexID[2]); // 與 Light Map 結合
	g_pFloor->Draw();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, g_uiFTexID[0]); // 與 Diffuse Map 結合
	glActiveTexture(GL_TEXTURE1); // select active texture 1
	glBindTexture(GL_TEXTURE_2D, g_uiFTexID[2]); // 與 Light Map 結合
	g_pFloor2->Draw();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, g_uiFTexID[8]); // 與 Diffuse Map 結合
	glActiveTexture(GL_TEXTURE1); // select active texture 1
	glBindTexture(GL_TEXTURE_2D, g_uiFTexID[2]); // 與 Light Map 結合
	g_pFloor3->Draw();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, g_uiFTexID[4]); // 與 Diffuse Map 結合
	glActiveTexture(GL_TEXTURE1); // select active texture 1
	glBindTexture(GL_TEXTURE_2D, g_uiFTexID[2]); // 與 Light Map 結合
	g_pFloor4->Draw();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, g_uiYugiohTexID[0]); // 與 Diffuse Map 結合
	glActiveTexture(GL_TEXTURE1); // select active texture 1
	glBindTexture(GL_TEXTURE_2D, g_uiFTexID[2]); // 與 Light Map 結合
	g_pFloor5->Draw();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, g_uiFTexID[10]); // 與 Diffuse Map 結合
	glActiveTexture(GL_TEXTURE1); // select active texture 1
	glBindTexture(GL_TEXTURE_2D, g_uiFTexID[2]); // 與 Light Map 結合
	g_pFloor6->Draw();
//  glBindTexture(GL_TEXTURE_2D, 0);
#endif
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, g_uiFTexID[1]);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, g_uiFTexID[4]);
	g_pCube->Draw();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, g_uiFTexID[9]);
	g_pEarth->Draw();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, g_uiFTexID[3]);
	g_pObjLoader->Draw();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, g_uiFTexID[5]);
	g_pObjLoader2->Draw();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, g_uiPokemonTexID[0]);
	g_pPokemonBall->Draw();

	glActiveTexture(GL_TEXTURE0); // select active texture 0
	glBindTexture(GL_TEXTURE_2D, g_uiFTexID[3]); // 與 Diffuse Map 結合
	glActiveTexture(GL_TEXTURE1); // select active texture 1
	glBindTexture(GL_TEXTURE_CUBE_MAP, g_uiSphereCubeMap); // 與 Light Map 結合
	g_pSphere->Draw();
	glBindTexture(GL_TEXTURE_2D, 0);

	glActiveTexture(GL_TEXTURE0); // select active texture 0
	glBindTexture(GL_TEXTURE_2D, g_uiFTexID[3]); // 與 Diffuse Map 結合
	glActiveTexture(GL_TEXTURE1); // select active texture 1
	glBindTexture(GL_TEXTURE_CUBE_MAP, g_uiSphereCubeMap); // 與 Light Map 結合
	g_pCubeMap->Draw();
	glBindTexture(GL_TEXTURE_2D, 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, g_uiYugiohTexID[6]);
	g_pYugioh_Cube[2]->Draw();

	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, g_uiYugiohTexID[1]);
	//g_pYugioh_Cube[5]->Draw();

	g_pLight->Draw();
	g_pLight2->Draw();
	g_pLight3->Draw();
	g_pLight4->Draw();
	g_pLight5->Draw();
	//g_pLight6->Draw();
	if(During_Attack)g_pEnergyBall->Draw();
	if (Card_Open == false && Card_Cover == false) {
		g_pTrap7->Draw();
		g_pTrap8->Draw();
		g_pTrap9->Draw();
	}

	glActiveTexture(GL_TEXTURE0); // select active texture 0
	glBindTexture(GL_TEXTURE_2D, g_uiFTexID[6]); // 與 Diffuse Map 結合
	g_LeftWall_1->Draw();
	g_RightWall_1->Draw();
	g_FrontWall_1->Draw();

	glActiveTexture(GL_TEXTURE0); // select active texture 0
	glBindTexture(GL_TEXTURE_2D, g_uiFTexID[12]); // 與 Diffuse Map 結合
	g_LeftWall_2->Draw();
	g_FrontWall_2->Draw();

	glActiveTexture(GL_TEXTURE0); // select active texture 0
	glBindTexture(GL_TEXTURE_2D, g_uiFTexID[11]); // 與 Diffuse Map 結合
	g_BackWall_3->Draw();
	g_LeftWall_3->Draw();
	g_RightWall_3->Draw();
	g_FrontWall_3->Draw();

	glActiveTexture(GL_TEXTURE0); // select active texture 0
	glBindTexture(GL_TEXTURE_2D, g_uiFTexID[13]); // 與 Diffuse Map 結合
	g_BackWall_4->Draw();
	g_FrontWall_4->Draw();
	g_LeftWall_4->Draw();
	g_RightWall_4->Draw();

	glActiveTexture(GL_TEXTURE0); // select active texture 0
	glBindTexture(GL_TEXTURE_2D, g_uiFTexID[14]); // 與 Diffuse Map 結合
	g_BackWall_5->Draw();
	g_FrontWall_5->Draw();
	g_LeftWall_5->Draw();
	g_RightWall_5->Draw();

	glActiveTexture(GL_TEXTURE0); // select active texture 0
	glBindTexture(GL_TEXTURE_2D, g_uiFTexID[15]); // 與 Diffuse Map 結合
	g_BackWall_6->Draw();
	g_FrontWall_6->Draw();
	g_LeftWall_6->Draw();
	g_RightWall_6->Draw();

	glActiveTexture(GL_TEXTURE0); // select active texture 0
	glBindTexture(GL_TEXTURE_2D, g_uiFTexID[16]); // 與 Diffuse Map 結合
	g_pDoor1->Draw();
	g_pDoor2->Draw();
	g_pDoor3->Draw();
	g_pDoor4->Draw();
	g_pDoor5->Draw();
	g_pDoor6->Draw();
	g_pDoor7->Draw();
	g_pDoor8->Draw();
	g_pDoor9->Draw();
	g_pDoor10->Draw();
	g_pDoor11->Draw();
	g_pDoor12->Draw();
	g_pDoor13->Draw();
	g_pDoor14->Draw();

	glDepthMask(GL_FALSE);
	//Pokemon
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, g_uiPokemonTexID[1]);
	g_pPokemon[0]->Draw();

	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, g_uiPokemonTexID[2]);
	//g_pPokemon[1]->Draw();

	//glActiveTexture(GL_TEXTURE0); // select active texture 0
	//glBindTexture(GL_TEXTURE_2D, g_uiPokemonTexID[3]);
	//g_pPokemon[2]->Draw();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, g_uiPokemonTexID[4]);
	g_pPokemon[3]->Draw();

	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, g_uiPokemonTexID[5]);
	//g_pPokemon[4]->Draw();

	//glActiveTexture(GL_TEXTURE0); // select active texture 0
	//glBindTexture(GL_TEXTURE_2D, g_uiPokemonTexID[6]);
	//g_pPokemon[5]->Draw();
	glActiveTexture(GL_TEXTURE2); // select active texture 0
	glBindTexture(GL_TEXTURE_2D, 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, g_uiAimalTexID[0]);
	glActiveTexture(GL_TEXTURE2); // select active texture 0
	glBindTexture(GL_TEXTURE_2D, g_uiAimalNormalTexID[0]);
	g_pAimal[0]->Draw();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, g_uiAimalTexID[1]);
	glActiveTexture(GL_TEXTURE2); // select active texture 0
	glBindTexture(GL_TEXTURE_2D, g_uiAimalNormalTexID[1]);
	g_pAimal[1]->Draw();

	glActiveTexture(GL_TEXTURE0); // select active texture 0
	glBindTexture(GL_TEXTURE_2D, g_uiAimalTexID[2]);
	glActiveTexture(GL_TEXTURE2); // select active texture 0
	glBindTexture(GL_TEXTURE_2D, g_uiAimalNormalTexID[2]);
	g_pAimal[2]->Draw();
	glBindTexture(GL_TEXTURE_2D, 0);

	//Kart
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, g_uiKartTexID[0]);
	glActiveTexture(GL_TEXTURE2); // select active texture 0
	glBindTexture(GL_TEXTURE_2D, g_uiAimalNormalTexID[6]);
	g_pKart[0]->Draw();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, g_uiKartTexID[1]);	glActiveTexture(GL_TEXTURE2); // select active texture 0
	glBindTexture(GL_TEXTURE_2D, g_uiAimalNormalTexID[7]);
	g_pKart[1]->Draw();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, g_uiKartTexID[2]);	glActiveTexture(GL_TEXTURE2); // select active texture 0
	glBindTexture(GL_TEXTURE_2D, g_uiAimalNormalTexID[8]);
	g_pKart[2]->Draw();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, g_uiKartTexID[3]);	glActiveTexture(GL_TEXTURE2); // select active texture 0
	glBindTexture(GL_TEXTURE_2D, g_uiAimalNormalTexID[9]);
	g_pKart[3]->Draw();

	//Deemo
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, g_uiDeemoTexID[0]);
	g_pDeemo[0]->Draw();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, g_uiDeemoTexID[1]);
	g_pDeemo[1]->Draw();

	glActiveTexture(GL_TEXTURE0); // select active texture 0
	glBindTexture(GL_TEXTURE_2D, g_uiDeemoTexID[2]);
	g_pDeemo[2]->Draw();
	glActiveTexture(GL_TEXTURE2); // select active texture 0
	glBindTexture(GL_TEXTURE_2D, 0);

	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, g_uiYugiohTexID[1]);
	//g_pYugioh_Cube[3]->Draw();

	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, g_uiYugiohTexID[2]);
	//g_pYugioh_Cube[0]->Draw();

	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, g_uiYugiohTexID[1]);
	//g_pYugioh_Cube[4]->Draw();

	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, g_uiYugiohTexID[3]);
	//g_pYugioh_Cube[1]->Draw();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, g_uiYugiohTexID[4]);
	glActiveTexture(GL_TEXTURE2); // select active texture 0
	glBindTexture(GL_TEXTURE_2D, g_uiAimalNormalTexID[10]);
	g_pYugioh[0]->Draw();

	//glActiveTexture(GL_TEXTURE0); // select active texture 0
	//glBindTexture(GL_TEXTURE_2D, g_uiYugiohTexID[5]);
	//glActiveTexture(GL_TEXTURE2); // select active texture 0
	//glBindTexture(GL_TEXTURE_2D, g_uiAimalNormalTexID[11]);
	//g_pYugioh[1]->Draw();

	glActiveTexture(GL_TEXTURE0); // select active texture 0
	glBindTexture(GL_TEXTURE_2D, g_uiFTexID[12]); // 與 Diffuse Map 結合
	g_BackWall_2->Draw();
	g_RightWall_2->Draw();

	glActiveTexture(GL_TEXTURE0); // select active texture 0
	glBindTexture(GL_TEXTURE_2D, g_uiFTexID[6]); // 與 Diffuse Map 結合
	g_BackWall_1->Draw();

	glDisable(GL_BLEND);	// 關閉 Blending
	glDepthMask(GL_TRUE);	// 開啟對 Z-Buffer 的寫入操作
	if (cut_effect_On && b_Room1)g_p2DKnife->Draw();
	glutSwapBuffers();	// 交換 Frame Buffer
}

//----------------------------------------------------------------------------
// Part 2 : for single light source
void UpdateLightPosition(float dt)
{
	mat4 mxT;
	// 每秒繞 Y 軸轉 90 度
	g_fElapsedTime += dt;
	g_fLightTheta = g_fElapsedTime*(float)M_PI_2;
	if( g_fLightTheta >= (float)M_PI*2.0f ) {
		g_fLightTheta -= (float)M_PI*2.0f;
		g_fElapsedTime -= 4.0f;
	}
	g_Light1.position.x = g_fLightRadius * cosf(g_fLightTheta);
	g_Light1.position.z = g_fLightRadius * sinf(g_fLightTheta);
	mxT = Translate(g_Light1.position);
	g_pLight->SetTRSMatrix(mxT);
}

void UpdatePokemonBallState(float dt){
	mat4 mxT;
	g_fBallTime += dt;
	g_fBallTheta = g_fBallTime*(float)M_PI_2*1.5f;
	g_Ball.position.y = 3.0 + cosf(g_fBallTheta);
	mxT = Translate(g_Ball.position);
	g_pPokemonBall->SetTRSMatrix(mxT* RotateX(270)*RotateZ(90*g_fBallTheta));

	g_Light6.position.x = g_fLight6Radius * cosf(-g_fBallTheta)+20.0f;
	g_Light6.position.y = 3.0 + cosf(g_fBallTheta);
	g_Light6.position.z = g_fLight6Radius * sinf(-g_fBallTheta)+20.0f;
	mxT = Translate(g_Light6.position);
	g_pLight6->SetTRSMatrix(mxT* RotateX(270)*RotateZ(90 * g_fBallTheta));
}

void Trap_Card_Open(float dt) {
	mat4 mxT;
	g_fTrapEffectTime += dt;
	if (g_fTrapEffectTime >= 1.5f && g_fTrapEffectTime <= 2.0f) {
		g_Light7.position.x = -22.0f + 8 *(g_fTrapEffectTime - 1.5f);
		g_Light7.position.y =   8.0f - 30 * (g_fTrapEffectTime - 1.5f);
		g_Light7.position.z = 14.0f + 36 * (g_fTrapEffectTime - 1.5f);

		g_Light8.position.y = 8.0f - 30 * (g_fTrapEffectTime - 1.5f);
		g_Light8.position.z = 14.0f + 36 * (g_fTrapEffectTime - 1.5f);

		g_Light9.position.x = -18.0f - 8 * (g_fTrapEffectTime - 1.5f);
		g_Light9.position.y = 8.0f - 30 * (g_fTrapEffectTime - 1.5f);
		g_Light9.position.z = 14.0f + 36 * (g_fTrapEffectTime - 1.5f);

		mxT = Translate(g_Light7.position) * Scale(2 + 2 * dt, 2 + 2 * dt, 2 + 2 * dt);
		g_pTrap7->SetTRSMatrix(mxT);
		mxT = Translate(g_Light8.position) * Scale(2 + 2 * dt, 2 + 2 * dt, 2 + 2 * dt);
		g_pTrap8->SetTRSMatrix(mxT);
		mxT = Translate(g_Light9.position) * Scale(2 + 2 * dt, 2 + 2 * dt, 2 + 2 * dt);
		g_pTrap9->SetTRSMatrix(mxT);
	}
}

void UpdateCardRotate(float dt)
{
	mat4 mxT;
	g_fCardTime += dt;
	g_fCardTheta = g_fCardTime*(float)M_PI_2*(-1);
	if (g_fCardTheta <= (float)M_PI*(-0.31f)) {
		Card_Open = false;
		Card_Cover = false;
	}
	g_Card_Front.position.x = -20.0f;
	g_Card_Front.position.y = cosf(g_fCardTheta) + 1.0*(-g_fCardTheta);
	g_Card_Front.position.z =sinf(g_fCardTheta)+13.1f;
	g_Card_Back.position.x = -20.0f;
	g_Card_Back.position.y = cosf(g_fCardTheta) + 1.02*(-g_fCardTheta);
	g_Card_Back.position.z =sinf(g_fCardTheta) + 13.1f;
	mxT = Translate(g_Card_Front.position);
	g_pYugioh_Cube[2]->SetTRSMatrix(mxT* RotateX(90* g_fCardTheta)* RotateY(180) *Scale(2,0.02,3));
	mxT = Translate(g_Card_Back.position);
	g_pYugioh_Cube[5]->SetTRSMatrix(mxT* RotateX(90 * g_fCardTheta)* RotateY(180)*Scale(2, 0.02, 3));
}

void Card_Destroy(float dt)
{
	mat4 mxT;
	g_fDestroyTime += dt;
	if (g_fDestroyTime >= 2.0f && g_fDestroyTime <= 3.0f) {
		g_fDestroyTheta = (g_fDestroyTime-2.0f)*(float)M_PI_2;

		g_Card_M_Fornt.position.y = 0.32f - 0.5*(g_fDestroyTheta);
		g_Card_M_Back.position.y = 0.3f - 0.5*(g_fDestroyTheta);

		g_Monster.position.y = 2.0f - 2.5*(g_fDestroyTheta);
		g_Monster.position.z = sinf(-g_fDestroyTheta) + 21.0f;
	}
	if (g_fDestroyTime >= 2.15f) During_Attack = false;
	mxT = Translate(g_Card_M_Fornt.position);
	g_pYugioh_Cube[1]->SetTRSMatrix(mxT* RotateY(180) *Scale(2, 0.02, 3));
	mxT = Translate(g_Card_M_Back.position);
	g_pYugioh_Cube[4]->SetTRSMatrix(mxT* Scale(2, 0.02, 3));

	mxT = Translate(g_Monster.position);
	g_pYugioh[1]->SetTRSMatrix(mxT* RotateX(90 +90* g_fDestroyTheta)/** RotateY(180)*/ *Scale(3.5,3.5,3.5));

}

void Update_Titan1_Died(float dt){
	mat4 mxT;
	g_fTitan1Time += dt;
	g_fTitan1Theta = g_fTitan1Time*(float)M_PI_2*(-1);
	if (g_fTitan1Theta <= (float)M_PI* -0.31f) {
		Titan1_Died = false;
		Titan1_Alive = false;
	}
	g_Titan1.position.x = 5.0f+ sinf(g_fTitan1Theta);
	g_Titan1.position.y = 3.5f + 3.2f*(g_fTitan1Theta);
	mxT = Translate(g_Titan1.position);
	g_pAimal[0]->SetTRSMatrix(mxT* RotateX(90 + 90 * g_fTitan1Theta)* RotateZ(90 + 90 * g_fTitan1Theta) *Scale(8, 8, 8));
}

void Update_Titan2_Died(float dt) {
	mat4 mxT;
	g_fTitan2Time += dt;
	g_fTitan2Theta = g_fTitan2Time*(float)M_PI_2*(-1);
	if (g_fTitan2Theta <= (float)M_PI* -0.31f) {
		Titan2_Died = false;
		Titan2_Alive = false;
	}
	g_Titan2.position.y = 3.5f + 3.2f*(g_fTitan2Theta);
	g_Titan2.position.z = 5.0f+sinf(g_fTitan2Theta);
	mxT = Translate(g_Titan2.position);
	g_pAimal[1]->SetTRSMatrix(mxT* RotateX(90 - 90 * g_fTitan2Theta) *Scale(8, 8, 8));
}

void Update_Titan3_Died(float dt) {
	mat4 mxT;
	g_fTitan3Time += dt;
	g_fTitan3Theta = g_fTitan3Time*(float)M_PI_2*(-1);
	if (g_fTitan3Theta <= (float)M_PI* -0.31f) {
		Titan3_Died = false;
		Titan3_Alive = false;
	}
	g_Titan3.position.x = -5.0f + sinf(g_fTitan3Theta);
	g_Titan3.position.y = 2.7f + 2.5*(g_fTitan3Theta);
	mxT = Translate(g_Titan3.position);
	g_pAimal[2]->SetTRSMatrix(mxT* RotateX(90 + 90 * g_fTitan3Theta)* RotateZ(-90 + 90 * g_fTitan3Theta) *Scale(8, 8, 8));
}


void UpdateAllKart_Rotate(float dt) {
	mat4 mxT;
	g_fKart1Time += dt;
	g_fKart2Time += dt;
	g_fKart3Time += dt;
	g_fKart4Time += dt;

	g_fKart1Theta = g_fKart1Time*(float)M_PI_2;
	g_fKart2Theta = g_fKart2Time*(float)M_PI_2;
	g_fKart3Theta = g_fKart3Time*(float)M_PI_2;
	g_fKart4Theta = g_fKart4Time*(float)M_PI_2;

	g_Kart1.position.x = g_fKartRadius * cosf(g_fKart1Theta) - 20.0f;
	g_Kart1.position.y = 1.5f;
	g_Kart1.position.z = g_fKartRadius * sinf(g_fKart1Theta);
	mxT = Translate(g_Kart1.position);
	g_pKart[0]->SetTRSMatrix(mxT * RotateX(95)*RotateZ(-90 * g_fKart1Theta)*Scale(4.5, 1, 3.0));
	
	g_Kart2.position.x = g_fKartRadius * cosf(g_fKart2Theta) - 20.0f;
	g_Kart2.position.z = g_fKartRadius * sinf(g_fKart2Theta);
	mxT = Translate(g_Kart2.position);
	g_pKart[1]->SetTRSMatrix(mxT * RotateX(95)*RotateZ(-90* g_fKart2Theta)*Scale(4.5, 1, 3.0));
	
	g_Kart3.position.x = g_fKartRadius * cosf(g_fKart3Theta) - 20.0f;
	g_Kart3.position.z = g_fKartRadius * sinf(g_fKart3Theta);
	mxT = Translate(g_Kart3.position);
	g_pKart[2]->SetTRSMatrix(mxT * RotateX(95)*RotateZ(-90* g_fKart3Theta)*Scale(4.5, 1, 3.75));
	
	g_Kart4.position.x = g_fKartRadius * cosf(g_fKart4Theta) - 20.0f;
	g_Kart4.position.z = g_fKartRadius * sinf(g_fKart4Theta);
	mxT = Translate(g_Kart4.position);
	g_pKart[3]->SetTRSMatrix(mxT * RotateX(95)*RotateZ(-90 * g_fKart4Theta)*Scale(4.5, 1, 3.75));
}

void UpdateEarthRotate(float dt)
{
	mat4 mxT;
	vec4 vT;
	vT.x = 0.0; vT.y = 3.8; vT.z = 20.0;
	mxT = Translate(vT)* Scale(1.5f, 1.5f, 1.5f);
	// 每秒繞 Y 軸轉 90 度
	g_fEarthTime += dt;
	g_fEarthTheta = g_fEarthTime*(float)M_PI_2 * 3.0f;
	g_pEarth->SetTRSMatrix(mxT * RotateY(g_fEarthTheta)*RotateZ(g_fEarthTheta));
}

void UpdateEnergyBallRotate(float dt)
{
	mat4 mxT;
	vec4 vT;
	// 每秒繞 Y 軸轉 90 度
	g_fEnergyBallTime += dt;
	g_fEnergyBallTheta = g_fEnergyBallTime*(float)M_PI_2 * 3.0f;
	mxT = Translate(g_Light10.position)* Scale(1.0f+(0.3*cos(g_fEnergyBallTheta)));
	g_pEnergyBall->SetTRSMatrix(mxT * RotateY(10*g_fEnergyBallTheta)*RotateZ(10*g_fEnergyBallTheta));
}

//----------------------------------------------------------------------------
//鏡頭的移動
void UpdateCameraMovement(float delta) {
	auto camera = CCamera::getInstance();
	vec4 newPos = (cameraDir - cameraPos) * delta* 1.5f;
	newPos.y = 0.0f;
	cameraPos = cameraPos + newPos;
	camera->updateViewLookAt(cameraPos, cameraDir);
	
	//碰牆判定(外牆)
	if (cameraPos.x <= -29.5f) {cameraPos.x = -29.0f;}
	if (cameraPos.x >= 29.5f) {cameraPos.x = 29.0f;}
	if (cameraPos.z <= -9.5f) {cameraPos.z = -9.0f;}
	if (cameraPos.z >= 29.5f) {cameraPos.z = 29.0f;}

	//碰牆判定(內牆)
	if (b_Room1) {
		if (cameraPos.x <= -9.5f && cameraPos.z >=  -6.0f) { cameraPos.x = -9.0f; }
		if (cameraPos.x >=  9.5f && cameraPos.z >= -6.0f) { cameraPos.x =  9.0f; }
		if (cameraPos.z >=  9.5f && cameraPos.x <= -1.5f) { cameraPos.z =  9.0f; }
		if (cameraPos.z >=  9.5f && cameraPos.x >=  1.5f) { cameraPos.z =  9.0f; }
		if (cameraPos.x <= -10.0f) {
			b_Room1 = false;
			b_Room4 = true;
		}
		if (cameraPos.x >= 10.0f) {
			b_Room1 = false;
			b_Room2 = true;
		}
		if (cameraPos.z >= 10.0f) {
			b_Room1 = false;
			b_Room3 = true;
		}
	}

	if (b_Room2) {
		if (cameraPos.x <= 10.5f && cameraPos.z >= -6.0f) { cameraPos.x = 11.0f; }
		if (cameraPos.z >= 9.5f && cameraPos.x <= 26.0f) { cameraPos.z = 9.0f; }
		if (cameraPos.x <= 10.0f) {
			b_Room2 = false;
			b_Room1 = true;
		}
		if (cameraPos.z >= 10.0f) {
			b_Room2 = false;
			b_Room6 = true;
		}
	}

	if (b_Room3) {
		if (cameraPos.x <= -9.5f && cameraPos.z <= 26.0f) { cameraPos.x = -9.0f; }
		if (cameraPos.x >=  9.5f && cameraPos.z <= 26.0f) { cameraPos.x = 9.0f; }
		if (cameraPos.z <= 10.5f && cameraPos.x <= -1.5f) { cameraPos.z = 11.0f; }
		if (cameraPos.z <= 10.5f && cameraPos.x >= 1.5f) { cameraPos.z = 11.0f; }
		if (cameraPos.x <= -10.0f) {
			b_Room3 = false;
			b_Room5 = true;
		}
		if (cameraPos.x >= 10.0f) {
			b_Room3 = false;
			b_Room6 = true;
		}
		if (cameraPos.z <= 10.0f) {
			b_Room3 = false;
			b_Room1 = true;
		}
	}

	if (b_Room4) {
		if (cameraPos.x >= -10.5f && cameraPos.z >= -6.0f) { cameraPos.x = -11.0f; }
		if (cameraPos.z >= 9.5f && cameraPos.x >= -26.0f) { cameraPos.z = 9.0f; }
		if (cameraPos.x >= -10.0f) {
			b_Room4 = false;
			b_Room1 = true;
		}
		if (cameraPos.z >= 10.0f) {
			b_Room5 = true;
			b_Room4 = false;
		}
	}

	if (b_Room5) {
		if (cameraPos.x >= -10.5f && cameraPos.z <= 26.0f) { cameraPos.x = -11.0f; }
		if (cameraPos.z <= 10.5f && cameraPos.x >= -26.0f) { cameraPos.z = 11.0f; }
		if (cameraPos.x >= -10.0f) {
			b_Room5 = false;
			b_Room3 = true;
		}
		if (cameraPos.z <= 10.0f) {
			b_Room5 = false;
			b_Room4 = true;
		}
	}

	if (b_Room6) {
		if (cameraPos.x <= 10.5f && cameraPos.z <= 26.0f) { cameraPos.x = 11.0f; }
		if (cameraPos.z <= 10.5f && cameraPos.x <= 26.0f) { cameraPos.z = 11.0f; }
		if (cameraPos.x <= 10.0f) {
			b_Room6 = false;
			b_Room3 = true;
		}
		if (cameraPos.z <= 10.0f) {
			b_Room6 = false;
			b_Room2 = true;
		}
	}

	//燈光判定
	if (cameraPos.x >= -1.0f && cameraPos.x <= 1.0f && cameraPos.z >= -1.0f && cameraPos.z <= 1.0f)g_bAutoRotating = true;
	//殺巨人判定
	if (cameraPos.x >=  3.5f && cameraPos.x <=  6.5f && cameraPos.z >= -2.0f && cameraPos.z <= 2.0f)Kill_Titan1 = true;
	if (cameraPos.x >= -2.0f && cameraPos.x <=  2.0f && cameraPos.z >=  3.5f && cameraPos.z <= 6.5f)Kill_Titan2 = true;
	if (cameraPos.x >= -6.5f && cameraPos.x <= -3.5f && cameraPos.z >= -2.0f && cameraPos.z <= 2.0f)Kill_Titan3 = true;
}
//----------------------------------------------------------------------------

void onFrameMove(float delta)
{
	UpdateAllKart_Rotate(delta);
	UpdateEnergyBallRotate(delta);
	UpdateEarthRotate(delta);
	UpdatePokemonBallState(delta);
	if(b_Forward)UpdateCameraMovement(delta);
	if (b_Backward)UpdateCameraMovement(-delta);
	if( g_bAutoRotating) UpdateLightPosition(delta);
	if (Card_Open && Card_Cover) UpdateCardRotate(delta);
	if (Card_Open == false && Card_Cover == false) Trap_Card_Open(delta);
	if (Card_Open == false && Card_Cover == false) Card_Destroy(delta);
	if (Titan1_Alive && Titan1_Died) Update_Titan1_Died(delta);
	if (Titan2_Alive && Titan2_Died) Update_Titan2_Died(delta);
	if (Titan3_Alive && Titan3_Died) Update_Titan3_Died(delta);
	mat4 mvx;	// view matrix & projection matrix
	bool bVDirty;	// view 與 projection matrix 是否需要更新給物件
	auto camera = CCamera::getInstance();
	mvx = camera->getViewMatrix(bVDirty);
	if (bVDirty) {
		g_pFloor->SetViewMatrix(mvx);
		g_pFloor2->SetViewMatrix(mvx);
		g_pFloor3->SetViewMatrix(mvx);
		g_pFloor4->SetViewMatrix(mvx);
		g_pFloor5->SetViewMatrix(mvx);
		g_pFloor6->SetViewMatrix(mvx);
		g_pDoor1->SetViewMatrix(mvx);
		g_pDoor2->SetViewMatrix(mvx);
		g_pDoor3->SetViewMatrix(mvx);
		g_pDoor4->SetViewMatrix(mvx);
		g_pDoor5->SetViewMatrix(mvx);
		g_pDoor6->SetViewMatrix(mvx);
		g_pDoor7->SetViewMatrix(mvx);
		g_pDoor8->SetViewMatrix(mvx);
		g_pDoor9->SetViewMatrix(mvx);
		g_pDoor10->SetViewMatrix(mvx);
		g_pDoor11->SetViewMatrix(mvx);
		g_pDoor12->SetViewMatrix(mvx);
		g_pDoor13->SetViewMatrix(mvx);
		g_pDoor14->SetViewMatrix(mvx);
		g_pCube->SetViewMatrix(mvx);
		g_pEarth->SetViewMatrix(mvx);
		g_pPokemonBall->SetViewMatrix(mvx);
		g_pEnergyBall->SetViewMatrix(mvx);
		g_pSphere->SetViewMatrix(mvx);
		g_pSphere->SetViewPosition(camera->getViewPosition());
		g_pCubeMap->SetViewMatrix(mvx);
		g_pCubeMap->SetViewPosition(camera->getViewPosition());
		g_pObjLoader->SetViewMatrix(mvx);
		g_pObjLoader2->SetViewMatrix(mvx);
		g_pLight->SetViewMatrix(mvx);
		g_pLight2->SetViewMatrix(mvx);
		g_pLight3->SetViewMatrix(mvx);
		g_pLight4->SetViewMatrix(mvx);
		g_pLight5->SetViewMatrix(mvx);
		g_pLight6->SetViewMatrix(mvx);
		g_pTrap7->SetViewMatrix(mvx);
		g_pTrap8->SetViewMatrix(mvx);
		g_pTrap9->SetViewMatrix(mvx);
		g_LeftWall_1->SetViewMatrix(mvx);
		g_RightWall_1->SetViewMatrix(mvx);
		g_FrontWall_1->SetViewMatrix(mvx);
		g_BackWall_1->SetViewMatrix(mvx);
		g_LeftWall_2->SetViewMatrix(mvx);
		g_RightWall_2->SetViewMatrix(mvx);
		g_FrontWall_2->SetViewMatrix(mvx);
		g_BackWall_2->SetViewMatrix(mvx);
		g_LeftWall_3->SetViewMatrix(mvx);
		g_RightWall_3->SetViewMatrix(mvx);
		g_FrontWall_3->SetViewMatrix(mvx);
		g_BackWall_3->SetViewMatrix(mvx);
		g_LeftWall_4->SetViewMatrix(mvx);
		g_RightWall_4->SetViewMatrix(mvx);
		g_FrontWall_4->SetViewMatrix(mvx);
		g_BackWall_4->SetViewMatrix(mvx);
		g_LeftWall_5->SetViewMatrix(mvx);
		g_RightWall_5->SetViewMatrix(mvx);
		g_FrontWall_5->SetViewMatrix(mvx);
		g_BackWall_5->SetViewMatrix(mvx);
		g_LeftWall_6->SetViewMatrix(mvx);
		g_RightWall_6->SetViewMatrix(mvx);
		g_FrontWall_6->SetViewMatrix(mvx);
		g_BackWall_6->SetViewMatrix(mvx);
		g_pAimal[0]->SetViewMatrix(mvx);
		g_pAimal[1]->SetViewMatrix(mvx);
		g_pAimal[2]->SetViewMatrix(mvx);
		g_pKart[0]->SetViewMatrix(mvx);
		g_pKart[1]->SetViewMatrix(mvx);
		g_pKart[2]->SetViewMatrix(mvx);
		g_pKart[3]->SetViewMatrix(mvx);
		g_pDeemo[0]->SetViewMatrix(mvx);
		g_pDeemo[1]->SetViewMatrix(mvx);
		g_pDeemo[2]->SetViewMatrix(mvx);
		g_pPokemon[0]->SetViewMatrix(mvx);
		g_pPokemon[1]->SetViewMatrix(mvx);
		g_pPokemon[2]->SetViewMatrix(mvx);
		g_pPokemon[3]->SetViewMatrix(mvx);
		g_pPokemon[4]->SetViewMatrix(mvx);
		g_pPokemon[5]->SetViewMatrix(mvx);
		g_pYugioh[0]->SetViewMatrix(mvx);
		g_pYugioh[1]->SetViewMatrix(mvx);
		g_pYugioh_Cube[0]->SetViewMatrix(mvx);
		g_pYugioh_Cube[3]->SetViewMatrix(mvx);
		g_pYugioh_Cube[1]->SetViewMatrix(mvx);
		g_pYugioh_Cube[4]->SetViewMatrix(mvx);
		g_pYugioh_Cube[2]->SetViewMatrix(mvx);
		g_pYugioh_Cube[5]->SetViewMatrix(mvx);
	}

	// 如果需要重新計算時，在這邊計算每一個物件的顏色
	g_pLight->Update(delta);
	g_pLight2->Update(delta);
	g_pLight3->Update(delta);
	g_pLight4->Update(delta);
	g_pLight5->Update(delta);
	g_pLight6->Update(delta);
	g_pTrap7->Update(delta);
	g_pTrap8->Update(delta);
	g_pTrap9->Update(delta);
	g_pEnergyBall->Update(delta);

	g_pFloor->Update(delta, g_Light1);
	g_pDoor1->Update(delta, g_Light1);
	g_pDoor12->Update(delta, g_Light1);
	g_pDoor13->Update(delta, g_Light1);
	g_pCube->Update(delta, g_Light1);
	g_pSphere->Update(delta, g_Light1);
	g_pAimal[0]->Update(delta, g_Light1);
	g_pAimal[1]->Update(delta, g_Light1);
	g_pAimal[2]->Update(delta, g_Light1);
	g_LeftWall_1->Update(delta, g_Light1);
	g_RightWall_1->Update(delta, g_Light1);
	g_FrontWall_1->Update(delta, g_Light1);
	g_BackWall_1->Update(delta, g_Light1);

	g_pFloor2->Update(delta, g_Light2);
	g_pDoor10->Update(delta, g_Light2);
	g_pDoor11->Update(delta, g_Light2);
	g_pDeemo[0]->Update(delta, g_Light2);
	g_pDeemo[1]->Update(delta, g_Light2);
	g_pDeemo[2]->Update(delta, g_Light2);
	g_LeftWall_2->Update(delta, g_Light2);
	g_RightWall_2->Update(delta, g_Light2);
	g_FrontWall_2->Update(delta, g_Light2);
	g_BackWall_2->Update(delta, g_Light2);
	g_pCubeMap->Update(delta, g_Light2);
	g_pObjLoader->Update(delta, g_Light2);
	g_pObjLoader2->Update(delta, g_Light2);

	g_pFloor3->Update(delta, g_Light3);
	g_pDoor6->Update(delta, g_Light3);
	g_pDoor7->Update(delta, g_Light3);
	g_pDoor14->Update(delta, g_Light3);
	g_pEarth->Update(delta, g_Light3);
	g_LeftWall_3->Update(delta, g_Light3);
	g_RightWall_3->Update(delta, g_Light3);
	g_FrontWall_3->Update(delta, g_Light3);
	g_BackWall_3->Update(delta, g_Light3);

	g_pFloor4->Update(delta, g_Light4);
	g_pDoor2->Update(delta, g_Light4);
	g_pDoor3->Update(delta, g_Light4);
	g_pKart[0]->Update(delta, g_Light4);
	g_pKart[1]->Update(delta, g_Light4);
	g_pKart[2]->Update(delta, g_Light4);
	g_pKart[3]->Update(delta, g_Light4);
	g_LeftWall_4->Update(delta, g_Light4);
	g_RightWall_4->Update(delta, g_Light4);
	g_FrontWall_4->Update(delta, g_Light4);
	g_BackWall_4->Update(delta, g_Light4);

	g_pFloor5->Update(delta, g_Light5);
	g_pDoor4->Update(delta, g_Light5);
	g_pDoor5->Update(delta, g_Light5);
	g_pYugioh[0]->Update(delta, g_Light5);
	g_pYugioh[1]->Update(delta, g_Light5);
	g_pYugioh_Cube[0]->Update(delta, g_Light5);
	g_pYugioh_Cube[3]->Update(delta, g_Light5);
	g_pYugioh_Cube[1]->Update(delta, g_Light5);
	g_pYugioh_Cube[4]->Update(delta, g_Light5);
	g_pYugioh_Cube[2]->Update(delta, g_Light5);
	g_pYugioh_Cube[5]->Update(delta, g_Light5);
	g_LeftWall_5->Update(delta, g_Light5);
	g_RightWall_5->Update(delta, g_Light5);
	g_FrontWall_5->Update(delta, g_Light5);
	g_BackWall_5->Update(delta, g_Light5);

	g_pFloor6->Update(delta, g_Light6);
	g_pDoor8->Update(delta, g_Light6);
	g_pDoor9->Update(delta, g_Light6);
	g_pPokemon[0]->Update(delta, g_Light6);
	g_pPokemon[1]->Update(delta, g_Light6);
	g_pPokemon[2]->Update(delta, g_Light6);
	g_pPokemon[3]->Update(delta, g_Light6);
	g_pPokemon[4]->Update(delta, g_Light6);
	g_pPokemon[5]->Update(delta, g_Light6);
	g_pPokemonBall->Update(delta, g_Light6);
	g_LeftWall_6->Update(delta, g_Light6);
	g_RightWall_6->Update(delta, g_Light6);
	g_FrontWall_6->Update(delta, g_Light6);
	g_BackWall_6->Update(delta, g_Light6);
	GL_Display();
}

//----------------------------------------------------------------------------

void Win_Keyboard( unsigned char key, int x, int y )
{
    switch ( key ) {
	case  SPACE_KEY:

		break;
//----------------------------------------------------------------------------
// Part 2 : for single light source
	case 65: // A key
	case 97: // a key
		break;
	case 81: // Q key
	case 113: // q key
		Card_Open = !Card_Open;
		break;
	case 82: // R key
		if( g_fLightR <= 0.95f ) g_fLightR += 0.05f;
		g_Light1.diffuse.x = g_fLightR;
		g_pLight->SetColor(g_Light1.diffuse);
		break;
	case 114: // r key
		if( g_fLightR >= 0.05f ) g_fLightR -= 0.05f;
		g_Light1.diffuse.x = g_fLightR;
		g_pLight->SetColor(g_Light1.diffuse);
		break;
	case 71: // G key
		if( g_fLightG <= 0.95f ) g_fLightG += 0.05f;
		g_Light1.diffuse.y = g_fLightG;
		g_pLight->SetColor(g_Light1.diffuse);
		break;
	case 103: // g key
		if( g_fLightG >= 0.05f ) g_fLightG -= 0.05f;
		g_Light1.diffuse.y = g_fLightG;
		g_pLight->SetColor(g_Light1.diffuse);
		break;
	case 66: // B key
		if( g_fLightB <= 0.95f ) g_fLightB += 0.05f;
		g_Light1.diffuse.z = g_fLightB;
		g_pLight->SetColor(g_Light1.diffuse);
		break;
	case 98: // b key
		if( g_fLightB >= 0.05f ) g_fLightB -= 0.05f;
		g_Light1.diffuse.z = g_fLightB;
		g_pLight->SetColor(g_Light1.diffuse);
		break;
	case 87:  //W
	case 119: //w
		b_Forward = true;
		b_Backward = false;
		break;
	case 83:  //S
	case 115: //s
		b_Forward = false;
		b_Backward = true;
		break;
//----------------------------------------------------------------------------
    case 033:
		glutIdleFunc( NULL );
		delete g_pCube;
		delete g_pEarth;
		delete g_pPokemonBall;
		delete g_pEnergyBall;
		delete g_pCubeMap;
		delete g_pObjLoader;
		delete g_pObjLoader2;
		delete g_pFloor;
		delete g_pFloor2;
		delete g_pFloor3;
		delete g_pFloor4;
		delete g_pFloor5;
		delete g_pFloor6;
		delete g_pDoor1;
		delete g_pDoor2;
		delete g_pDoor3;
		delete g_pDoor4;
		delete g_pDoor5;
		delete g_pDoor6;
		delete g_pDoor7;
		delete g_pDoor8;
		delete g_pDoor9;
		delete g_pDoor10;
		delete g_pDoor11;
		delete g_pDoor12;
		delete g_pDoor13;
		delete g_pDoor14;
		delete g_pLight;
		delete g_pLight2;
		delete g_pLight3;
		delete g_pLight4;
		delete g_pLight5;
		delete g_pLight6;
		delete g_pTrap7;
		delete g_pTrap8;
		delete g_pTrap9;
		delete g_LeftWall_1;
		delete g_RightWall_1;
		delete g_FrontWall_1;
		delete g_BackWall_1;
		delete g_LeftWall_2;
		delete g_RightWall_2;
		delete g_FrontWall_2;
		delete g_BackWall_2;
		delete g_LeftWall_3;
		delete g_RightWall_3;
		delete g_FrontWall_3;
		delete g_BackWall_3;
		delete g_LeftWall_4;
		delete g_RightWall_4;
		delete g_FrontWall_4;
		delete g_BackWall_4;
		delete g_LeftWall_5;
		delete g_RightWall_5;
		delete g_FrontWall_5;
		delete g_BackWall_5;
		delete g_LeftWall_6;
		delete g_RightWall_6;
		delete g_FrontWall_6;
		delete g_BackWall_6;
		delete g_pAimal[0];
		delete g_pAimal[1];
		delete g_pAimal[2];
		delete g_pDeemo[0];
		delete g_pDeemo[1];
		delete g_pDeemo[2];
		delete g_pPokemon[0];
		delete g_pPokemon[1];
		delete g_pPokemon[2];
		delete g_pPokemon[3];
		delete g_pPokemon[4];
		delete g_pPokemon[5];
		delete g_pYugioh[0];
		delete g_pYugioh[1];
		delete g_pYugioh_Cube[0];
		delete g_pYugioh_Cube[3];
		delete g_pYugioh_Cube[1];
		delete g_pYugioh_Cube[4];
		delete g_pYugioh_Cube[2];
		delete g_pYugioh_Cube[5];
		delete g_pKart[0];
		delete g_pKart[1];
		delete g_pKart[2];
		delete g_pKart[3];
		CCamera::getInstance()->destroyInstance();
		CTexturePool::getInstance()->destroyInstance();
        exit( EXIT_SUCCESS );
        break;
    }
}

void Win_KeyboardUp(unsigned char key, int x, int y) {
	switch (key)
	{
	//移動---Start
	case 87:  //W
	case 119: //w
		b_Forward = false;
		break;
	case 83:  //S
	case 115: //s
		b_Backward = false;
		break;
		//移動---End

		//燈光---Start
	case 82: // R key
		if (g_fLightR <= 0.95f) g_fLightR += 0.05f;
		g_Light1.diffuse.x = g_fLightR;
		g_pLight->SetColor(g_Light1.diffuse);
		break;
	case 114: // r key
		if (g_fLightR >= 0.05f) g_fLightR -= 0.05f;
		g_Light1.diffuse.x = g_fLightR;
		g_pLight->SetColor(g_Light1.diffuse);
		break;
	case 71: // G key
		if (g_fLightG <= 0.95f) g_fLightG += 0.05f;
		g_Light1.diffuse.y = g_fLightG;
		g_pLight->SetColor(g_Light1.diffuse);
		break;
	case 103: // g key
		if (g_fLightG >= 0.05f) g_fLightG -= 0.05f;
		g_Light1.diffuse.y = g_fLightG;
		g_pLight->SetColor(g_Light1.diffuse);
		break;
	case 66: // B key
		if (g_fLightB <= 0.95f) g_fLightB += 0.05f;
		g_Light1.diffuse.z = g_fLightB;
		g_pLight->SetColor(g_Light1.diffuse);
		break;
	case 98: // b key
		if (g_fLightB >= 0.05f) g_fLightB -= 0.05f;
		g_Light1.diffuse.z = g_fLightB;
		g_pLight->SetColor(g_Light1.diffuse);
		break;
		//燈光---End
	}

}

//----------------------------------------------------------------------------
void Win_Mouse(int button, int state, int x, int y) {
	switch(button) {
		case GLUT_LEFT_BUTTON:   // 目前按下的是滑鼠左鍵
			cut_effect_On = !cut_effect_On;
			if (Kill_Titan1)Titan1_Died = true;
			if (Kill_Titan2)Titan2_Died = true;
			if (Kill_Titan3)Titan3_Died = true;
			//if ( state == GLUT_DOWN ) ; 
			break;
		case GLUT_MIDDLE_BUTTON:  // 目前按下的是滑鼠中鍵 ，換成 Y 軸
			//if ( state == GLUT_DOWN ) ; 
			break;
		case GLUT_RIGHT_BUTTON:   // 目前按下的是滑鼠右鍵
			//if ( state == GLUT_DOWN ) ;
			break;
		default:
			break;
	} 
}
//----------------------------------------------------------------------------
void Win_SpecialKeyboard(int key, int x, int y) {

	switch(key) {
		case GLUT_KEY_LEFT:		// 目前按下的是向左方向鍵

			break;
		case GLUT_KEY_RIGHT:	// 目前按下的是向右方向鍵

			break;
		default:
			break;
	}
}

//----------------------------------------------------------------------------
// The passive motion callback for a window is called when the mouse moves within the window while no mouse buttons are pressed.
void Win_PassiveMotion(int x, int y) {

	g_fPhi = (float)-M_PI*(x - HALF_SIZE)/(HALF_SIZE); // 轉換成 g_fPhi 介於 -PI 到 PI 之間 (-180 ~ 180 之間)
	g_fTheta = (float)M_PI*(float)y/SCREEN_SIZE;
	cameraDir = vec4(g_fRadius*sin(g_fTheta)*sin(g_fPhi) + cameraPos.x, g_fRadius*cos(g_fTheta) + cameraPos.y, g_fRadius*sin(g_fTheta)*cos(g_fPhi) + cameraPos.z, 1.0f);
	CCamera::getInstance()->updateViewLookAt(cameraPos, cameraDir);
}

// The motion callback for a window is called when the mouse moves within the window while one or more mouse buttons are pressed.
void Win_MouseMotion(int x, int y) {
	g_fPhi = (float)-M_PI*(x - HALF_SIZE)/(HALF_SIZE);  // 轉換成 g_fPhi 介於 -PI 到 PI 之間 (-180 ~ 180 之間)
	g_fTheta = (float)M_PI*(float)y/SCREEN_SIZE;
	cameraDir = vec4(g_fRadius*sin(g_fTheta)*sin(g_fPhi) + cameraPos.x, g_fRadius*cos(g_fTheta) + cameraPos.y, g_fRadius*sin(g_fTheta)*cos(g_fPhi) + cameraPos.z, 1.0f);
	CCamera::getInstance()->updateViewLookAt(cameraPos, cameraDir);
}
//----------------------------------------------------------------------------
void GL_Reshape(GLsizei w, GLsizei h)
{
	glViewport(0, 0, w, h);
	glClearColor( 0.0, 0.0, 0.0, 1.0 ); // black background
	glEnable(GL_DEPTH_TEST);
}

//----------------------------------------------------------------------------

int main( int argc, char **argv )
{
    
	glutInit(&argc, argv);
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
    glutInitWindowSize( SCREEN_SIZE, SCREEN_SIZE );

	// If you use freeglut the two lines of code can be added to your application 
	glutInitContextVersion( 3, 2 );
	glutInitContextProfile( GLUT_CORE_PROFILE );

    glutCreateWindow("Compositing and Blending");

	// The glewExperimental global switch can be turned on by setting it to GL_TRUE before calling glewInit(), 
	// which ensures that all extensions with valid entry points will be exposed.
	glewExperimental = GL_TRUE; 
    glewInit();  

    init();

	glutMouseFunc(Win_Mouse);
	glutMotionFunc(Win_MouseMotion);
	glutPassiveMotionFunc(Win_PassiveMotion);  
    glutKeyboardFunc( Win_Keyboard );	// 處理 ASCI 按鍵如 A、a、ESC 鍵...等等
	glutKeyboardUpFunc(Win_KeyboardUp);
	glutSpecialFunc( Win_SpecialKeyboard);	// 處理 NON-ASCI 按鍵如 F1、Home、方向鍵...等等
    glutDisplayFunc( GL_Display );
	glutReshapeFunc( GL_Reshape );
	glutIdleFunc( IdleProcess );
	
    glutMainLoop();
    return 0;
}