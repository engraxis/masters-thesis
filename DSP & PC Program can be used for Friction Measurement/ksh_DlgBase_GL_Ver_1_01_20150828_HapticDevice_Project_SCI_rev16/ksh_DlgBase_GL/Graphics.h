#ifndef __GRAPHICS_H_
#define __GRAPHICS_H_

#include <windows.h>
#include "resource.h"


#include <GL/glut.h>
#include <stdio.h>

#include "kshFPS.h"
#include "kshFontGL.h"

#include "kshMenuGL.h"




//////////////////////////////////////////////////////////////////////////
//				extern (외부 선언 - 다른 파일에 선언됨)					//
//////////////////////////////////////////////////////////////////////////
// 외부 선언을 알려주는 문장

////////			WinMain.h				//////////////////////
extern HWND	hGLWnd;



/////////////// Control.h //////////////////////////////
extern double Control_Rate;


//////////////////////////////////////////////////////////////////////////


#define RoyalBlue1 0.28f, 0.46f, 1.0f
#define RoyalBlue2 0.260f, 0.43f, 0.93f
#define RoyalBlue3 0.23f, 0.37f, 0.8f
#define RoyalBlue4 0.15f, 0.25f, 0.55f
#define RoyalBlue5 0.0f, 0.13f, 0.4f

#define Grey80 0.8f, 0.8f, 0.8f

#define Coral1 1.0f, 0.5f, 0.31f
#define Coral2 1.0f, 0.45f, 0.34f
#define Coral3 0.93f, 0.42f, 0.31f
#define Coral4 0.8f, 0.37f, 0.27f
#define Coral5 0.55f, 0.24f, 0.18f



#define MAX_TORQUE	1.28
#define MAX_POS 0.9566			// FKine 최대 길이 [m]
#define MAX_FORCE	50			// 50 [N]
#define MAX_KEST	10000			// [N/m]
#define MAX_JPOS_ERROR	0.05		// [rad]




// F/T 센서 Display 관련
void Draw_Rectangle(float x1, float y1, float x2, float y2);		// Draw 사각형.
void Draw_Large_Text(float x, float y, char text[]);			// 큰 글씨 함수.
void Draw_Text(float x, float y, char text[]);					// 글씨 함수.




bool InitGLMainWindow(HDC hDC, HGLRC hRC);
bool DrawMainWindow(HWND hWnd, HDC hDC, HGLRC hRC);
void Show_Information();


// Graphic thread rate
//FpsTracker* gFps = new FpsTracker(10);
float gRate;

double	gData[12][201];
double	gTime, gTimeP;
int		gIndex;
int		gCount;



// OPENGL Window size 변수
// OPENGL Window size 변수
// 960, 800 으로 픽처 컨트롤 크기를 맞춤.  X: 1.5 배,  Y: 1.624 배.
// 다이얼로그 픽처 컨트롤 크기: 640, 492  => 변경: 620, 492
//const int WSIZEX = 960;
const int WSIZEX = 930;
const int WSIZEY = 800;




// Error Message, Error Info
char buffer_Err[size_buf], buf_Err_Info[size_buf];







//////////////////////////////////////////////////////////////////////////
//							Buttons & Sliders							//
//////////////////////////////////////////////////////////////////////////

// FT Sensor Init 버튼.  원점 세팅  // DATA Save
lwtButton BInitFTSensor, BHoming, BDATASAVEButton, BSAVEFILEButton, BFrictionComp, BGravityComp, BMotorOn, BMotorRun;
lwtButton BRIC;
lwtButton BAdaptiveControl, RIC_BReset;




// X, Y, Width, Height, Min, Max, Title, Init_Value
lwtSlider sSaveTime, sDATASlider;


// graphic rendering rate
lwtSlider sGRATE(700, 730, 120, 10, 0, 100,  "Graphic Rendering Rate", gRate);
// Controller sampling rate
lwtSlider sCRATE(700, 760, 120, 10, 0, 2000, "Control Rate", Control_Rate);








#endif











