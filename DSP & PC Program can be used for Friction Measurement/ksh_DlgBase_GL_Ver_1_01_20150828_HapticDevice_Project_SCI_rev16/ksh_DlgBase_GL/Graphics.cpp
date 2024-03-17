#include "Parameters.h"
#include "Graphics.h"

#include "math.h"



extern bool bButton_DataSave, bButton_FileSave;


extern double dControlTime, dControlTime1;

extern FpsTracker* gFps;
//extern Timer *cFps;

extern DWORD dwMainThreadPri, dwContThreadPri;


// TEST
extern double dQPF;

// 모니터링용 전역 변수
char str_TEST1[size_buf], str_TEST2[size_buf], str_TEST3[size_buf];
float fTEST1, fTEST2, fTEST3, fTEST4, fTEST5, fTEST6, fTEST7, fTEST8, fTEST9, fTEST10;

float fTEST81, fTEST82, fTEST83, fTEST84, fTEST85, fTEST86, fTEST87, fTEST88, fTEST89, fTEST90;
float fTEST91, fTEST92, fTEST93, fTEST94, fTEST95, fTEST96, fTEST97, fTEST98, fTEST99, fTEST100;
char cTEST[8];
int TEST_var1, TEST_var2, TEST_var3, TEST_var4, TEST_var5, TEST_var6;

#include <limits.h>
extern Timer *cFps;



extern int wFile_Cnt;


//typedef double VECTOR[3];
//VECTOR HIP;
// TEST



bool InitGLMainWindow(HDC hDC, HGLRC hRC)
{
	wglMakeCurrent(hDC, hRC);
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);	

	glFrontFace(GL_CCW);
	glEnable(GL_LIGHTING);

	////////////////  default background light  ////////////////////
	GLfloat ambientLight0[] = { 0.6f, 0.6f, 0.6f, 1.0f };
	GLfloat diffuseLight0[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat lightPos0[] = { -10.0f, -320.0f, 550.0f, 1.0f };
	GLfloat specular0[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight0);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular0);
	/////////////////////////////////////////////////////////////////

	/////////////////////  spot light setting  //////////////////////
	GLfloat ambientLight1[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat diffuseLight1[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat specularLight1[]= { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat lightPos1[] = { 200, 200, 200.0f, 1.0f };
	GLfloat lightDre1[] = { -10.0f, -10.0f, -25.0f };

	//	glLightfv(GL_LIGHT1, GL_AMBIENT, ambientLight1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuseLight1);
	glLightfv(GL_LIGHT1, GL_SPECULAR, specularLight1);
	glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, lightDre1);

	glLightf( GL_LIGHT1, GL_SPOT_CUTOFF, 5.0f );
	glLightf( GL_LIGHT1, GL_SPOT_EXPONENT, 125.0f );
	glLightf( GL_LIGHT1, GL_CONSTANT_ATTENUATION, 2.0f);
	//	glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.0f);
	//	glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.0f);	
	/////////////////////////////////////////////////////////////////

	/////////////////////  point light setting  /////////////////////
	GLfloat ambientLight2[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat diffuseLight2[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat specularLight2[]= { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat lightPos2[] = { 0.0f, -400.0f, -500.0f, 1.0f };
	GLfloat lightDre2[] = { -10.0f, -10.0f, -25.0f };

	//	glLightfv(GL_LIGHT2, GL_AMBIENT, ambientLight2);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, diffuseLight2);
	//	glLightfv(GL_LIGHT2, GL_SPECULAR, specularLight2);
	glLightfv(GL_LIGHT2, GL_POSITION, lightPos2);
	//	glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, lightDre2);

	//	glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 5.0f );
	//	glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 125.0f );
	//	glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION, 2.0f);
	//	glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, 0.0f);
	//	glLightf(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, 0.0f);
	///////////////////////////////////////////////////////////////////

	/////////////////////  ambient light setting  /////////////////////
	GLfloat ambientLight3[] = { 0.1f, 0.1f, 0.1f, 1.0f };
	GLfloat diffuseLight3[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat specularLight3[]= { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat lightPos3[] = { 0.0f, 0.0f, 1000.0f, 0.0f };
	GLfloat lightDre3[] = { -10.0f, -10.0f, -25.0f };

	glLightfv(GL_LIGHT3, GL_AMBIENT, ambientLight3);
	glLightfv(GL_LIGHT3, GL_DIFFUSE, diffuseLight3);
	//	glLightfv(GL_LIGHT3, GL_SPECULAR, specularLight3);
	glLightfv(GL_LIGHT3, GL_POSITION, lightPos3);
	//	glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, lightDre3);

	//	glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, 5.0f );
	//	glLightf(GL_LIGHT3, GL_SPOT_EXPONENT, 125.0f );
	//	glLightf(GL_LIGHT3, GL_CONSTANT_ATTENUATION, 2.0f);
	//	glLightf(GL_LIGHT3, GL_LINEAR_ATTENUATION, 0.0f);
	//	glLightf(GL_LIGHT3, GL_QUADRATIC_ATTENUATION, 0.0f);
	///////////////////////////////////////////////////////////////////

	/////////////////////  specular light setting  /////////////////////
	GLfloat ambientLight4[] = { 0.1f, 0.1f, 0.1f, 1.0f };
	GLfloat diffuseLight4[] = { 0.3f, 0.3f, 0.3f, 1.0f };
	GLfloat specularLight4[]= { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat lightPos4[] = { -1000.0f, 1000.0f, 800.0f, 0.0f };
	GLfloat lightDre4[] = { -10.0f, -10.0f, -25.0f };

	glLightfv(GL_LIGHT4, GL_AMBIENT, ambientLight4);
	glLightfv(GL_LIGHT4, GL_DIFFUSE, diffuseLight4);
	glLightfv(GL_LIGHT4, GL_SPECULAR, specularLight4);
	glLightfv(GL_LIGHT4, GL_POSITION, lightPos4);
	//	glLightfv(GL_LIGHT4, GL_SPOT_DIRECTION, lightDre43);

	//	glLightf(GL_LIGHT4, GL_SPOT_CUTOFF, 5.0f );
	//	glLightf(GL_LIGHT4, GL_SPOT_EXPONENT, 125.0f );
	//	glLightf(GL_LIGHT4, GL_CONSTANT_ATTENUATION, 2.0f);
	//	glLightf(GL_LIGHT4, GL_LINEAR_ATTENUATION, 0.0f);
	//	glLightf(GL_LIGHT4, GL_QUADRATIC_ATTENUATION, 0.0f);
	///////////////////////////////////////////////////////////////////
	/**/
	/////////////////  default light setting  /////////////////////
	GLfloat ambientLight5[] = { 0.6f, 0.6f, 0.6f, 1.0f };
	GLfloat diffuseLight5[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat lightPos5[] = { -10.0f, -320.0f, 550.0f, 1.0f };
	GLfloat specular5[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	glLightfv(GL_LIGHT5, GL_AMBIENT, ambientLight5);
	glLightfv(GL_LIGHT5, GL_DIFFUSE, diffuseLight5);
	glLightfv(GL_LIGHT5, GL_POSITION, lightPos5);
	glLightfv(GL_LIGHT5, GL_SPECULAR, specular5);
	///////////////////////////////////////////////////////////////

	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	GLfloat mat_ambient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat mat_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };	
	GLfloat mat_specref[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specref);
	glMateriali(GL_FRONT, GL_SHININESS, 30);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
	return TRUE;										// Initialization Went OK
}





bool DrawMainWindow(HWND hWnd, HDC hDC, HGLRC hRC)
{
	gFps->timestamp();
	gRate = gFps->fpsInstant();		// Graphic rate 계산

	wglMakeCurrent(hDC, hRC);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
//	gluLookAt(0,  0,  300, 
//			  0,  0, -300, 
//		      0,  1,  0) ;
/*
	if(EnableLight1) glEnable(GL_LIGHT1);
	else glDisable(GL_LIGHT1);
	if(EnableLight2) glEnable(GL_LIGHT2);
	else glDisable(GL_LIGHT2);
	if(EnableLight3) glEnable(GL_LIGHT3);
	else glDisable(GL_LIGHT3);
	if(EnableLight4) glEnable(GL_LIGHT4);
	else glDisable(GL_LIGHT4);
	if(EnableLight5) glEnable(GL_LIGHT5);
	else glDisable(GL_LIGHT5);
*/
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);
	glEnable(GL_LIGHT3);
	glEnable(GL_LIGHT4);

////////////////////////////////////////////////


	glColor3f(WHITE);







	//////////////////////////////////////////////////////////////////////////
	//////////////////////// Display Buttons   //////////////////////////////
	/////////////////////////////////////////////////////////////////////////

	
	BHoming.draw(WSIZEX, WSIZEY);


	// F/T 센서 영점 조정
	BInitFTSensor.draw(WSIZEX, WSIZEY);		// FT 센서 초기화 버튼 렌더링!

	BFrictionComp.draw(WSIZEX, WSIZEY);
	BGravityComp.draw(WSIZEX, WSIZEY);
	BRIC.draw(WSIZEX, WSIZEY);

	BAdaptiveControl.draw(WSIZEX, WSIZEY);

	RIC_BReset.draw(WSIZEX, WSIZEY);



	BMotorOn.draw(WSIZEX, WSIZEY);

	BMotorRun.draw(WSIZEX, WSIZEY);


	BDATASAVEButton.draw(WSIZEX, WSIZEY);	// 
	BSAVEFILEButton.draw(WSIZEX, WSIZEY);


	// Save Time Slider Bar
	sSaveTime.draw(WSIZEX, WSIZEY);

	sDATASlider.draw(WSIZEX, WSIZEY);	// SaveData Duration


	sGRATE.update(gRate); 	sGRATE.draw(WSIZEX, WSIZEY);	// Graphic Rate
	
	sCRATE.update(Control_Rate); sCRATE.draw(WSIZEX, WSIZEY);	// Control Rate
	
	//////////////////////////////////////////////////////////////////////////


//	Draw_Text(Enalbe_Text_Show);		// Show Text

	Show_Information();	// Show Text and Graph


	SwapBuffers(hDC);
	return TRUE;			
}


// TEST
extern double temp_cnt1, temp_cnt2, temp_NowCnt1;

// 제어 루프 안의 코드 계산 시간
extern double dControl_CompTime;


// MMTimer timer resolution
///////////////////////////extern UINT wTimerRes;


//void Show_Information(bool bShow_Text, bool bShow_Graph)
void Show_Information()
{
//	if(bShow_Text || bShow_Graph) 
//	{
		char buffer[size_buf];

		int i;

		beginRenderText(WSIZEX, WSIZEY); 
		{

			//////////////////////////////////////////////////////////////////////////
			// Basic Information
			//////////////////////////////////////////////////////////////////////////

			



			//bButton_FileSave


			sprintf_s(buffer, size_buf, "Proc. Pri: 0x%x", GetPriorityClass(GetCurrentProcess()));
			renderText(20, 100, BITMAP_FONT_TYPE_8_BY_13, buffer);

			sprintf_s(buffer, size_buf, "mThr, gThr, cThr. Pri: 0x%x, 0x%x, 0x%x", dwMainThreadPri, GetThreadPriority(GetCurrentThread()), dwContThreadPri);
			renderText(20, 130, BITMAP_FONT_TYPE_8_BY_13, buffer);

			// TEST
			sprintf_s(buffer, size_buf, "QPF: %f", dQPF);
			renderText(20, 160, BITMAP_FONT_TYPE_8_BY_13, buffer);

			sprintf_s(buffer, size_buf, "Cnt: %f", temp_NowCnt1);
			renderText(20, 175, BITMAP_FONT_TYPE_8_BY_13, buffer);


			sprintf_s(buffer, size_buf, "CompTime [ms]: %f", dControl_CompTime);
			renderText(20, 190, BITMAP_FONT_TYPE_8_BY_13, buffer);

/*
			sprintf_s(buffer, size_buf, "MMTimer Res. [ms]: %d", wTimerRes);
			renderText(20, 205, BITMAP_FONT_TYPE_8_BY_13, buffer);
*/
			

			sprintf_s(buffer, size_buf, "Cont. Time [s]: %f, %f", dControlTime, dControlTime1);
			renderText(120, 30, BITMAP_FONT_TYPE_8_BY_13, buffer);


			sprintf_s(buffer, size_buf, "%d, %d", bButton_DataSave, bButton_FileSave);
			renderText(20, 230, BITMAP_FONT_TYPE_8_BY_13, buffer);

			sprintf_s(buffer, size_buf, "TxIndex(%2.3f)", dDSPTxIndex);
			renderText(20, 250, BITMAP_FONT_TYPE_8_BY_13, buffer);


			///////////////////////////////////////////////////////////////////////
			// SCI 통신 테스트
			//////////////////////////////////////////////////////////////////////////
			sprintf_s(buffer, size_buf, "HD Pos. (%.3f, %.3f)", fHD_Pos[0], fHD_Pos[1]);
			renderText(20, 280, BITMAP_FONT_TYPE_8_BY_13, buffer);

			sprintf_s(buffer, size_buf, "HD Vel. (%.3f, %.3f)", fHD_Vel[0], fHD_Vel[1]);
			renderText(20, 295, BITMAP_FONT_TYPE_8_BY_13, buffer);

			sprintf_s(buffer, size_buf, "HD Accel. (%.3f, %.3f)", fHD_Accel[0], fHD_Accel[1]);
			renderText(20, 310, BITMAP_FONT_TYPE_8_BY_13, buffer);

			sprintf_s(buffer, size_buf, "HD FT (%.3f, %.3f)", fHD_FT[0], fHD_FT[1]);
			renderText(20, 325, BITMAP_FONT_TYPE_8_BY_13, buffer);

			sprintf_s(buffer, size_buf, "HD Test. (%.3f)", fHD_Test);
			renderText(20, 340, BITMAP_FONT_TYPE_8_BY_13, buffer);


			sprintf_s(buffer, size_buf, "File No. (%d)", wFile_Cnt);
			renderText(20, 360, BITMAP_FONT_TYPE_8_BY_13, buffer);

						


		//	sprintf_s(buffer, size_buf, "Test. 7.987643(%.3f), 7.987643(%.4f), 7.987643(%.5f)", 7.987643,7.987643,7.987643);
		//	renderText(20, 360, BITMAP_FONT_TYPE_8_BY_13, buffer);






			// 모니터링용 전역변수


			

			sprintf_s(buffer, size_buf, "fTEST:%.0f, %.0f, %.0f, %.0f, %.0f, %.0f, %.0f, %.0f, %.0f, %.0f", fTEST81, fTEST82, fTEST83, fTEST84, fTEST85, fTEST86, fTEST87, fTEST88, fTEST89, fTEST90);
			renderText(30, 585, BITMAP_FONT_TYPE_8_BY_13, buffer);

			sprintf_s(buffer, size_buf, "fTEST:%.0f, %.0f, %.0f, %.0f, %.0f, %.0f, %.0f, %.0f, %.0f, %.0f", fTEST91, fTEST92, fTEST93, fTEST94, fTEST95, fTEST96, fTEST97, fTEST98, fTEST99, fTEST100);
			renderText(30, 600, BITMAP_FONT_TYPE_8_BY_13, buffer);



			sprintf_s(buffer, size_buf, "TEST(int):%d(%d), %d(RemainedRX), %d(RxMsg), %d(%d), %d(Rx_err), %d(Readed)", TEST_var1, BUFF_SIZE, TEST_var2, TEST_var3, TEST_var4, TX_BUF, TEST_var5, TEST_var6);
			renderText(30, 640, BITMAP_FONT_TYPE_8_BY_13, buffer);

			sprintf_s(buffer, size_buf, "TEST:%c(a), %c(b), %c(c), %c(d), %c(e), %c(f), %c(g), %c(h)", cTEST[0], cTEST[1], cTEST[2], cTEST[3], cTEST[4], cTEST[5], cTEST[6], cTEST[7]);
			renderText(30, 655, BITMAP_FONT_TYPE_8_BY_13, buffer);

			sprintf_s(buffer, size_buf, "fTEST:%.0f, %.0f, %.0f, %.0f, %.0f, %.0f, %.0f, %.0f, %.0f, %.0f", fTEST1, fTEST2, fTEST3, fTEST4, fTEST5, fTEST6, fTEST7, fTEST8, fTEST9, fTEST10);
			renderText(30, 670, BITMAP_FONT_TYPE_8_BY_13, buffer);

			sprintf_s(buffer, size_buf, "TEST(str):%s, %s, %s", str_TEST1, str_TEST2, str_TEST3);
			renderText(30, 685, BITMAP_FONT_TYPE_8_BY_13, buffer);
					


			// Mouse Position Info.
			sprintf_s(buffer, size_buf, "Mouse X(%d), Y(%d)", MouseX, MouseY);
			renderText(30, 700, BITMAP_FONT_TYPE_8_BY_13, buffer);
			
			

			//sprintf_s(buffer, size_buf, "Mouse X(%d), Y(%d)", MouseX, MouseY);
			//enderText(30, 700, BITMAP_FONT_TYPE_8_BY_13, buffer);
				


			// 1 DOF Device Information: POS & FOR
			glColor3f(WHITE);
			sprintf_s(buffer, size_buf, "1 DOF DEVICE INFORMATION");
			renderText(700, 30, BITMAP_FONT_TYPE_8_BY_13, buffer);

		


								
			glColor3f(WHITE);
			sprintf_s(buffer, size_buf, "Control Signal INFORMATION");
			renderText(700, 130, BITMAP_FONT_TYPE_8_BY_13, buffer);
			
				

			// Application Information: Rendering Rate
			glColor3f(WHITE);
			sprintf_s(buffer, size_buf, "APPLICATION INFORMATION");
			renderText(700, 700, BITMAP_FONT_TYPE_8_BY_13, buffer);


			//////////////////////////////////////////////////////////////////////////
			//////////////////////////////////////////////////////////////////////////
/*

			//if (bShow_Graph)	// Show Graph
			if (1)	// Show Graph
			{

				//////////////////////////////////
			//        Graph Drawing         //
			//////////////////////////////////

			// 그래프의 위치 및 크기 설정 //
			// 1. Position Graph [rad] // 모터 위치
			int gPosX_G1  = 50;	 int gPosY_G1  = 300;
			
			// 2. Force (Torque) Graph [Nm]
			int gPosX_G2  = 350;	 int gPosY_G2  = 300;
			
			// 3. Velocity Graph [rad/s]
			int gPosX_G3  = 50;   int gPosY_G3  = 450;
			
			// 4. Voltage Graph [V]   // Input voltage for Motor Drive
			int gPosX_G4  = 350;	 int gPosY_G4  = 450;

			
			// 5. Task Pose Signal Graph
			int gPosX_G5  = 50;	 int gPosY_G5  = 600;
			
			
			// 6. Velocity Error Graph
			int gPosX_G6  = 350;	 int gPosY_G6  = 600;

			
			// Graphs' Size
			int gSizeX = 200;   int gSizeY = 100;
			
			

			// 그래프의 축 정보를 설정한다 //
			// Max. & Min. of each graph
			// 1. Joint Pos
			double gG1DataYmax = 1.2;  double gG1DataYmin = -0.8;
			double gG1DataYmag = gG1DataYmax - gG1DataYmin;
			
			// 2. Control torque
			double gG2DataYmax = MAX_TORQUE;  double gG2DataYmin = -MAX_TORQUE;
			double gG2DataYmag = gG2DataYmax - gG2DataYmin;

			// 3. Task Pos
			double gG3DataYmax = MAX_POS;  double gG3DataYmin = 0.8;
			double gG3DataYmag = gG3DataYmax - gG3DataYmin;

			// 4. Contact Force
			double gG4DataYmax = MAX_FORCE;  double gG4DataYmin = 0.0;
			double gG4DataYmag = gG4DataYmax - gG4DataYmin;

			// 5. Estimation
			double gG5DataYmax = MAX_KEST;  double gG5DataYmin = 0.0;
			double gG5DataYmag = gG5DataYmax - gG5DataYmin;

			// 6. Pos Error
			double gG6DataYmax = MAX_JPOS_ERROR;  double gG6DataYmin = -MAX_JPOS_ERROR;
			double gG6DataYmag = gG6DataYmax - gG6DataYmin;

			
			// 갱신 속도 설정
			double speed = 4;   
			

			// 현재시간 계산
			gTime = gTime + 0.5 * speed; 
			if((gTime - gTimeP) >= 1.0) {
				gTimeP = gTime;	
				gIndex = gIndex + 1;		
			}
			if(gTime > gSizeX) {
				gCount = gIndex;
				gTime = gTimeP = 0;
				gIndex = 0;
			}


			

			// 데이터 갱신
			gData[0][gIndex] = gTime;	// Time

			gData[1][gIndex] = (((double)gSizeY)/gG1DataYmag) * (Robot_JPos[0] - gG1DataYmin);	// 1. Hip Joint Position [rad]
			gData[2][gIndex] = (((double)gSizeY)/gG2DataYmag) * (Torque_In[0] - gG2DataYmin);	// 2. Hip Motor Control Torque [Nm]
			gData[3][gIndex] = (((double)gSizeY)/gG3DataYmag) * (Robot_Pos[0] - gG3DataYmin);	// 3. X Pos [m]
			gData[4][gIndex] = (((double)gSizeY)/gG4DataYmag) * (FT_Data[0] - gG4DataYmin);	// 4. Force X
			gData[5][gIndex] = (((double)gSizeY)/gG5DataYmag) * (Kest - gG5DataYmin);	// 5. Kest at X
			gData[6][gIndex] = (((double)gSizeY)/gG6DataYmag) * (JPos_Err[0] - gG6DataYmin);	// 6. JPos Error
			gData[7][gIndex] = (((double)gSizeY)/gG1DataYmag) * (Robot_JPos[1] - gG1DataYmin);	// 1. Knee JPos [rad]
			gData[8][gIndex] = (((double)gSizeY)/gG2DataYmag) * (Torque_In[1] - gG2DataYmin);	// 2. Knee
			gData[9][gIndex] = (((double)gSizeY)/gG3DataYmag) * (Ref_Pos[0] - gG3DataYmin); // 3. Ref X Pos [m]
			gData[10][gIndex] = (((double)gSizeY)/gG4DataYmag) * (FT_Data[1] - gG4DataYmin); // 4. Force Y
			gData[11][gIndex] = (((double)gSizeY)/gG6DataYmag) * (JPos_Err[1] - gG6DataYmin); // 4. JPos Y Error
			
			// 외각의 네모부분을 그린다 //
			// 1. Motor Position Graph
			glColor3f(0.7f, 0.7f, 0.7f);
			glLineWidth(1);
			glBegin (GL_LINE_LOOP);
			glVertex2f(gPosX_G1,	   	  gPosY_G1		   );
			glVertex2f(gPosX_G1 + gSizeX, gPosY_G1		   );
			glVertex2f(gPosX_G1 + gSizeX, gPosY_G1 + gSizeY);
			glVertex2f(gPosX_G1,		  gPosY_G1 + gSizeY);
			glEnd();


			// 2. Force (Torque) Graph
			glColor3f(0.7f, 0.7f, 0.7f);
			glBegin (GL_LINE_LOOP);
			glVertex2f(gPosX_G2,	   	  gPosY_G2		   );
			glVertex2f(gPosX_G2 + gSizeX, gPosY_G2		   );
			glVertex2f(gPosX_G2 + gSizeX, gPosY_G2 + gSizeY);
			glVertex2f(gPosX_G2,		  gPosY_G2 + gSizeY);
			glEnd();

			// Zero Force
			glBegin (GL_LINE_LOOP);	
			glVertex2f(gPosX_G2,          gPosY_G2 + gSizeY * gG2DataYmax / gG2DataYmag);
			glVertex2f(gPosX_G2 + gSizeX, gPosY_G2 + gSizeY * gG2DataYmax / gG2DataYmag);	
			glEnd();			
			

			// 3. Motor Joint Velocity Graph
			glBegin (GL_LINE_LOOP);
			glVertex2f(gPosX_G3,	   	  gPosY_G3			);
			glVertex2f(gPosX_G3 + gSizeX, gPosY_G3			);
			glVertex2f(gPosX_G3 + gSizeX, gPosY_G3 + gSizeY);
			glVertex2f(gPosX_G3,		  gPosY_G3 + gSizeY);
			glEnd();

			// Zero Line
			glBegin (GL_LINE_LOOP);	
			glVertex2f(gPosX_G3,          gPosY_G3 + gSizeY * gG3DataYmax / gG3DataYmag);
			glVertex2f(gPosX_G3 + gSizeX, gPosY_G3 + gSizeY * gG3DataYmax / gG3DataYmag);	
			glEnd();


			// 4. NI-DAQ Voltage Output Graph
			glColor3f(0.7f, 0.7f, 0.7f);
			glBegin (GL_LINE_LOOP);
			glVertex2f(gPosX_G4,	   	  gPosY_G4		   );
			glVertex2f(gPosX_G4 + gSizeX, gPosY_G4		   );
			glVertex2f(gPosX_G4 + gSizeX, gPosY_G4 + gSizeY);
			glVertex2f(gPosX_G4,		  gPosY_G4 + gSizeY);
			glEnd();

			// Zero Line
			glBegin (GL_LINE_LOOP);	
			glVertex2f(gPosX_G4,          gPosY_G4 + gSizeY * gG4DataYmax / gG4DataYmag);
			glVertex2f(gPosX_G4 + gSizeX, gPosY_G4 + gSizeY * gG4DataYmax / gG4DataYmag);	
			glEnd();


			// 5. Task Pose Graph
			glColor3f(0.7f, 0.7f, 0.7f);
			glBegin (GL_LINE_LOOP);
			glVertex2f(gPosX_G5,	   	  gPosY_G5		   );
			glVertex2f(gPosX_G5 + gSizeX, gPosY_G5		   );
			glVertex2f(gPosX_G5 + gSizeX, gPosY_G5 + gSizeY);
			glVertex2f(gPosX_G5,		  gPosY_G5 + gSizeY);
			glEnd();



			// 6. Joint Velocity & Torque Graph
			glColor3f(0.7f, 0.7f, 0.7f);
			glBegin (GL_LINE_LOOP);
			glVertex2f(gPosX_G6,	   	  gPosY_G6		   );
			glVertex2f(gPosX_G6 + gSizeX, gPosY_G6		   );
			glVertex2f(gPosX_G6 + gSizeX, gPosY_G6 + gSizeY);
			glVertex2f(gPosX_G6,		  gPosY_G6 + gSizeY);
			glEnd();

			// Zero Line
			glBegin (GL_LINE_LOOP);	
			glVertex2f(gPosX_G6,          gPosY_G6 + gSizeY * gG6DataYmax / gG6DataYmag);
			glVertex2f(gPosX_G6 + gSizeX, gPosY_G6 + gSizeY * gG6DataYmax / gG6DataYmag);	
			glEnd();



//////////////////////////////////////////////////////////////////////////
			// 갱신 시간 표시
			glColor3f(0.5f, 0.1f, 0.1f);
			glLineWidth(1);

			// 1. Position
			glBegin(GL_LINE_STRIP);
			glVertex2f(gPosX_G1 + gTime, gPosY_G1         );
			glVertex2f(gPosX_G1 + gTime, gPosY_G1 + gSizeY);
			glEnd();

			// 2. Force (Torque)
			glBegin(GL_LINE_STRIP);
			glVertex2f(gPosX_G2 + gTime, gPosY_G2         );
			glVertex2f(gPosX_G2 + gTime, gPosY_G2 + gSizeY);
			glEnd();


			// 3. Motor Velocity
			glBegin(GL_LINE_STRIP);
			glVertex2f(gPosX_G3 + gTime, gPosY_G3         );
			glVertex2f(gPosX_G3 + gTime, gPosY_G3 + gSizeY);
			glEnd();

			// 4. DAQ Voltage Output
			glBegin(GL_LINE_STRIP);
			glVertex2f(gPosX_G4 + gTime, gPosY_G4         );
			glVertex2f(gPosX_G4 + gTime, gPosY_G4 + gSizeY);
			glEnd();


			// 5. Task Pose Graph
			glBegin(GL_LINE_STRIP);
			glVertex2f(gPosX_G5 + gTime, gPosY_G5         );
			glVertex2f(gPosX_G5 + gTime, gPosY_G5 + gSizeY);
			glEnd();



			// 6. Joint Velocity & Torque Graph
			glBegin(GL_LINE_STRIP);
			glVertex2f(gPosX_G6 + gTime, gPosY_G6         );
			glVertex2f(gPosX_G6 + gTime, gPosY_G6 + gSizeY);
			glEnd();


//////////////////////////////////////////////////////////////////////////
			// 데이터 표시
			glColor3f(0.1f, 0.5f, 0.5f);
			glLineWidth(1);

			// 1. JPOS Hip
			glBegin(GL_LINE_STRIP);
			for(i = 1; i < gIndex; i++) 
			{
				glVertex2f(gPosX_G1 + gData[0][i], gPosY_G1 + gSizeY - gData[1][i]);		
			}
			glEnd();

			glBegin(GL_LINE_STRIP);
			if(gCount > 0) 
			{
				for(i = gIndex + 2; i < gCount; i++) 
				{
					glVertex2f(gPosX_G1 + gData[0][i], gPosY_G1 + gSizeY - gData[1][i]);
				}
			}
			glEnd();
			

			// 7번 JPOS Knee
			glColor3f(YELLOW);
			glBegin(GL_LINE_STRIP);
			for(i = 1; i < gIndex; i++) 
			{
				glVertex2f(gPosX_G1 + gData[0][i], gPosY_G1 + gSizeY - gData[7][i]);		
			}
			glEnd();

			glBegin(GL_LINE_STRIP);
			if(gCount > 0) 
			{
				for(i = gIndex + 2; i < gCount; i++) 
				{
					glVertex2f(gPosX_G1 + gData[0][i], gPosY_G1 + gSizeY - gData[7][i]);
				}
			}
			glEnd();


		
			// 2. Force/torque data (control input)
			glColor3f(0.1f, 0.5f, 0.5f);
			glBegin(GL_LINE_STRIP);
			for(i = 1; i < gIndex; i++) 
			{		
				glVertex2f(gPosX_G2 + gData[0][i], gPosY_G2 + gSizeY - gData[2][i]);
			}
			glEnd();

			glBegin(GL_LINE_STRIP);
			if(gCount > 0) 
			{
				for(i = gIndex + 2; i < gCount; i++) 
				{
					glVertex2f(gPosX_G2 + gData[0][i], gPosY_G2 + gSizeY - gData[2][i]);
				}
			}
			glEnd();

	

			// 8번
			glColor3f(YELLOW);
			glBegin(GL_LINE_STRIP);
			for(i = 1; i < gIndex; i++) 
			{		
				glVertex2f(gPosX_G2 + gData[0][i], gPosY_G2 + gSizeY - gData[8][i]);
			}
			glEnd();

			glBegin(GL_LINE_STRIP);
			if(gCount > 0) 
			{
				for(i = gIndex + 2; i < gCount; i++) 
				{
					glVertex2f(gPosX_G2 + gData[0][i], gPosY_G2 + gSizeY - gData[8][i]);
				}
			}
			glEnd();




			

			// 3. Pos X
			glColor3f(0.1f, 0.5f, 0.5f);			
				
			glBegin(GL_LINE_STRIP);
			for(i = 1; i < gIndex; i++) 
			{		
				glVertex2f(gPosX_G3 + gData[0][i], gPosY_G3 + gSizeY - gData[3][i]);
			}
			glEnd();

			glBegin(GL_LINE_STRIP);
			if(gCount > 0) 
			{
				for(i = gIndex + 2; i < gCount; i++) 
				{
					glVertex2f(gPosX_G3 + gData[0][i], gPosY_G3 + gSizeY - gData[3][i]);
				}
			}
			glEnd();
		
			// 9번 Pos Y
			glColor3f(YELLOW);
			glBegin(GL_LINE_STRIP);
			for(i = 1; i < gIndex; i++) 
			{		
				glVertex2f(gPosX_G3 + gData[0][i], gPosY_G3 + gSizeY - gData[9][i]);
			}
			glEnd();

			glBegin(GL_LINE_STRIP);
			if(gCount > 0) 
			{
				for(i = gIndex + 2; i < gCount; i++) 
				{
					glVertex2f(gPosX_G3 + gData[0][i], gPosY_G3 + gSizeY - gData[9][i]);
				}
			}
			glEnd();


			


			// 4. FORCE X
			glColor3f(0.1f, 0.5f, 0.5f);
			glBegin(GL_LINE_STRIP);
			for(i = 1; i < gIndex; i++) 
			{		
				glVertex2f(gPosX_G4 + gData[0][i], gPosY_G4 + gSizeY - gData[4][i]);
			}
			glEnd();

			glBegin(GL_LINE_STRIP);
			if(gCount > 0) 
			{
				for(i = gIndex + 2; i < gCount; i++) 
				{
					glVertex2f(gPosX_G4 + gData[0][i], gPosY_G4 + gSizeY - gData[4][i]);
				}
			}
			glEnd();


			// 10번 FOR Y
			glColor3f(YELLOW);
			glBegin(GL_LINE_STRIP);
			for(i = 1; i < gIndex; i++) 
			{		
				glVertex2f(gPosX_G4 + gData[0][i], gPosY_G4 + gSizeY - gData[10][i]);
			}
			glEnd();

			glBegin(GL_LINE_STRIP);
			if(gCount > 0) 
			{
				for(i = gIndex + 2; i < gCount; i++) 
				{
					glVertex2f(gPosX_G4 + gData[0][i], gPosY_G4 + gSizeY - gData[10][i]);
				}
			}
			glEnd();


			

			// 5. Kest at X direction
			glColor3f(0.1f, 0.5f, 0.5f);
			glBegin(GL_LINE_STRIP);
			for(i = 1; i < gIndex; i++) 
			{		
				glVertex2f(gPosX_G5 + gData[0][i], gPosY_G5 + gSizeY - gData[5][i]);
			}
			glEnd();

			glBegin(GL_LINE_STRIP);
			if(gCount > 0) 
			{
				for(i = gIndex + 2; i < gCount; i++) 
				{
					glVertex2f(gPosX_G5 + gData[0][i], gPosY_G5 + gSizeY - gData[5][i]);
				}
			}
			glEnd();


			
			// 6. JPos Error
			glColor3f(0.1f, 0.5f, 0.5f);
			glBegin(GL_LINE_STRIP);
			for(i = 1; i < gIndex; i++) 
			{		
				glVertex2f(gPosX_G6 + gData[0][i], gPosY_G6 + gSizeY - gData[6][i]);
			}
			glEnd();

			glBegin(GL_LINE_STRIP);
			if(gCount > 0) 
			{
				for(i = gIndex + 2; i < gCount; i++) 
				{
					glVertex2f(gPosX_G6 + gData[0][i], gPosY_G6 + gSizeY - gData[6][i]);
				}
			}
			glEnd();

			// 11번 JPOS Y Err
			glColor3f(YELLOW);
			glBegin(GL_LINE_STRIP);
			for(i = 1; i < gIndex; i++) 
			{		
				glVertex2f(gPosX_G6 + gData[0][i], gPosY_G6 + gSizeY - gData[11][i]);
			}
			glEnd();

			glBegin(GL_LINE_STRIP);
			if(gCount > 0) 
			{
				for(i = gIndex + 2; i < gCount; i++) 
				{
					glVertex2f(gPosX_G6 + gData[0][i], gPosY_G6 + gSizeY - gData[11][i]);
				}
			}
			glEnd();

			
			

//////////////////////////////////////////////////////////////////////////
		
			///////////////////////////////////////////
			//           TEXT in Graph Part          //
			///////////////////////////////////////////

			//// 1. Position Graph
			// Graph Title
			glColor3f(WHITE);
			sprintf(buffer, "Angle ( Rad )");	
			renderText(gPosX_G1, gPosY_G1 - 10, BITMAP_FONT_TYPE_8_BY_13, buffer);


			// Legend: Motor Position
			glColor3f(0.5f, 0.0f, 0.5f);
			sprintf(buffer, "--: JPos.");	
			renderText(gPosX_G1, gPosY_G1 + gSizeY + 10, BITMAP_FONT_TYPE_8_BY_13, buffer);

			// Y-axis: Motor Position
			glColor3f(0.7f, 0.7f, 0.7f);
			sprintf(buffer, "%1.2f", gG1DataYmax);	// Max.
			renderText(gPosX_G1 - 25, gPosY_G1, BITMAP_FONT_TYPE_8_BY_13, buffer);

			sprintf(buffer, "0.0");	// Zero
			renderText(gPosX_G1 -25, gPosY_G1 + gSizeY * gG1DataYmax / gG1DataYmag, BITMAP_FONT_TYPE_8_BY_13, buffer);

			sprintf(buffer, "%1.2f", gG1DataYmin);	// Min.
			renderText(gPosX_G1 - 32, gPosY_G1 + gSizeY, BITMAP_FONT_TYPE_8_BY_13, buffer);


			glColor3f(0.1f, 0.5f, 0.5f);
			sprintf(buffer, "%1.2f", Robot_JPos[0]);	// Shaft side Joint Pos.
			renderText(gPosX_G1 + gSizeX + 5, gPosY_G1 + gSizeY * (gG1DataYmax - Robot_JPos[0]) / gG1DataYmag, BITMAP_FONT_TYPE_8_BY_13, buffer);

			glColor3f(YELLOW);
			sprintf(buffer, "%1.2f", Robot_JPos[1]);	// raw Shaft side Joint Pos.
			renderText(gPosX_G1 + gSizeX + 5, gPosY_G1 + 10 + gSizeY * (gG1DataYmax - Robot_JPos[1]) / gG1DataYmag, BITMAP_FONT_TYPE_8_BY_13, buffer);
			
		

			//// 2. Force (Torque) Graph
			// Graph Title
			glColor3f(WHITE);
			sprintf(buffer, "M. TOR ( Nm )");
			renderText(gPosX_G2, gPosY_G2 - 10, BITMAP_FONT_TYPE_8_BY_13, buffer);

			// Legend: Td
			glColor3f(0.1f, 0.5f, 0.5f);		
			sprintf(buffer, "--Td");
			renderText(gPosX_G2, gPosY_G2 + gSizeY + 10, BITMAP_FONT_TYPE_8_BY_13, buffer);


			// Y-axis: Max. Min. Zero Current Force (Torque)
			glColor3f(0.7f, 0.7f, 0.7f);
			sprintf(buffer, "%1.2f", gG2DataYmax);	// Max.
			renderText(gPosX_G2 - 25, gPosY_G2, BITMAP_FONT_TYPE_8_BY_13, buffer);
			
			sprintf(buffer, "0.0");	// Zero
			renderText(gPosX_G2 -25, gPosY_G2 + gSizeY * gG2DataYmax / gG2DataYmag, BITMAP_FONT_TYPE_8_BY_13, buffer);
			
			sprintf(buffer, "%1.2f", gG2DataYmin);	// Min.
			renderText(gPosX_G2 - 32, gPosY_G2 + gSizeY, BITMAP_FONT_TYPE_8_BY_13, buffer);


			glColor3f(0.1f, 0.5f, 0.5f);
			sprintf(buffer, "%1.2f", Torque_In[0]);	// Current Td
			renderText(gPosX_G2 + gSizeX, gPosY_G2 + gSizeY * (gG2DataYmax - Torque_In[0]) / gG2DataYmag, BITMAP_FONT_TYPE_8_BY_13, buffer);

			glColor3f(YELLOW);
			sprintf(buffer, "%1.2f", Torque_In[1]);	// 
			renderText(gPosX_G2 + gSizeX, gPosY_G2 + 10 + gSizeY * (gG2DataYmax - Torque_In[1]) / gG2DataYmag, BITMAP_FONT_TYPE_8_BY_13, buffer);
		




			// 3. Pos
			// Graph Title
			glColor3f(WHITE);
			sprintf(buffer, "Pos [m]");
			renderText(gPosX_G3, gPosY_G3 - 10, BITMAP_FONT_TYPE_8_BY_13, buffer);

			// Y-axis: Max. Min. Current
			glColor3f(0.7f, 0.7f, 0.7f);
			sprintf(buffer, "%1.2f", gG3DataYmax);	// Max.
			renderText(gPosX_G3 - 35, gPosY_G3, BITMAP_FONT_TYPE_8_BY_13, buffer);

			sprintf(buffer, "%1.2f", gG3DataYmin);	// Min.
			renderText(gPosX_G3 - 35, gPosY_G3 + gSizeY, BITMAP_FONT_TYPE_8_BY_13, buffer);				

			glColor3f(0.1f, 0.5f, 0.5f);
			sprintf(buffer, "%1.2f", Robot_Pos[0]);	// Current Shaft Vel.
			renderText(gPosX_G3 + gSizeX, gPosY_G3 + gSizeY * (gG3DataYmax - Robot_Pos[0]) / gG3DataYmag, BITMAP_FONT_TYPE_8_BY_13, buffer);

			glColor3f(YELLOW);
			sprintf(buffer, "%1.2f", Ref_Pos[0]);	// Reference Pos X
			renderText(gPosX_G3 + gSizeX, gPosY_G3 + 10 + gSizeY * (gG3DataYmax - Ref_Pos[0]) / gG3DataYmag, BITMAP_FONT_TYPE_8_BY_13, buffer);



			//// 4. Contact Force Graph
			// Graph Title
			glColor3f(WHITE);
			sprintf(buffer, "Force [N]");
			renderText(gPosX_G4, gPosY_G4 - 10, BITMAP_FONT_TYPE_8_BY_13, buffer);

			// Legend: Voltage Output
			glColor3f(0.1f, 0.5f, 0.5f);		
			sprintf(buffer, "Force X-Y");
			renderText(gPosX_G4, gPosY_G4 + gSizeY + 10, BITMAP_FONT_TYPE_8_BY_13, buffer);


			// Y-axis: Max. Min. Zero Current Voltage
			glColor3f(0.7f, 0.7f, 0.7f);
			sprintf(buffer, "%1.1f", gG4DataYmax);	// Max.
			renderText(gPosX_G4 - 25, gPosY_G4, BITMAP_FONT_TYPE_8_BY_13, buffer);

			sprintf(buffer, "0.0");	// Zero
			renderText(gPosX_G4 -25, gPosY_G4 + gSizeY * gG4DataYmax / gG4DataYmag, BITMAP_FONT_TYPE_8_BY_13, buffer);

			sprintf(buffer, "%1.1f", gG4DataYmin);	// Min.
			renderText(gPosX_G4 - 32, gPosY_G4 + gSizeY, BITMAP_FONT_TYPE_8_BY_13, buffer);

			glColor3f(0.1f, 0.5f, 0.5f);
			sprintf(buffer, "%1.3f", FT_Data[0]);	// Current Voltage
			renderText(gPosX_G4 + gSizeX, gPosY_G4 + gSizeY * (gG4DataYmax - FT_Data[0]) / gG4DataYmag, BITMAP_FONT_TYPE_8_BY_13, buffer);


			glColor3f(YELLOW);
			sprintf(buffer, "%1.3f", FT_Data[1]);	// Current Voltage
			renderText(gPosX_G4 + gSizeX, gPosY_G4 + 10 + gSizeY * (gG4DataYmax - FT_Data[1]) / gG4DataYmag, BITMAP_FONT_TYPE_8_BY_13, buffer);



			//// 5. Estimation: Kest at X
			// Graph Title
			glColor3f(WHITE);
			sprintf(buffer, "Kest [N/m]");
			renderText(gPosX_G5, gPosY_G5 - 10, BITMAP_FONT_TYPE_8_BY_13, buffer);

			// Legend - X:
			glColor3f(0.1f, 0.5f, 0.5f);		
			sprintf(buffer, "Time (s)");
			renderText(gPosX_G5, gPosY_G5 + gSizeY + 10, BITMAP_FONT_TYPE_8_BY_13, buffer);


			// Y-axis: Max. Min. Zero Current Task Pose
			glColor3f(0.7f, 0.7f, 0.7f);
			sprintf(buffer, "%1.2f", gG5DataYmax);	// Max.
			renderText(gPosX_G5 - 25, gPosY_G5, BITMAP_FONT_TYPE_8_BY_13, buffer);

			//		sprintf(buffer, "0.0");	// Zero
			//		renderText(gPosX_G5 - 25, gPosY_G5 + gSizeY * gG5DataYmax / gG5DataYmag, BITMAP_FONT_TYPE_8_BY_13, buffer);

			sprintf(buffer, "%1.2f", gG5DataYmin);	// Min.
			renderText(gPosX_G5 - 32, gPosY_G5 + gSizeY, BITMAP_FONT_TYPE_8_BY_13, buffer);


			glColor3f(0.1f, 0.5f, 0.5f);
			sprintf(buffer, "%1.2f", Kest);	// Current Task Pose (Px)
			renderText(gPosX_G5 + gSizeX, gPosY_G5 + gSizeY * (gG5DataYmax - Kest) / gG5DataYmag, BITMAP_FONT_TYPE_8_BY_13, buffer);





			//// 6. JPosition Error Graph
			// Graph Title
			glColor3f(WHITE);
			sprintf(buffer, "JPos. Error");
			renderText(gPosX_G6, gPosY_G6 - 10, BITMAP_FONT_TYPE_8_BY_13, buffer);

			// Legend: Vel. Error
			glColor3f(0.1f, 0.5f, 0.5f);		
			sprintf(buffer, "JPos. Error");
			renderText(gPosX_G6, gPosY_G6 + gSizeY + 10, BITMAP_FONT_TYPE_8_BY_13, buffer);


			// Y-axis: Max. Min. Zero Current Vel. Error
			glColor3f(0.7f, 0.7f, 0.7f);
			sprintf(buffer, "%1.2f", gG6DataYmax);	// Max.
			renderText(gPosX_G6 - 25, gPosY_G6, BITMAP_FONT_TYPE_8_BY_13, buffer);

			sprintf(buffer, "0.0");	// Zero
			renderText(gPosX_G6 - 25, gPosY_G6 + gSizeY * gG6DataYmax / gG6DataYmag, BITMAP_FONT_TYPE_8_BY_13, buffer);

			sprintf(buffer, "%1.2f", gG6DataYmin);	// Min.
			renderText(gPosX_G6 - 32, gPosY_G6 + gSizeY, BITMAP_FONT_TYPE_8_BY_13, buffer);


			glColor3f(0.1f, 0.5f, 0.5f);
			sprintf(buffer, "%1.3f", JPos_Err[0]);	// Current Vel. Error
			renderText(gPosX_G6 + gSizeX, gPosY_G6 + gSizeY * (gG6DataYmax - JPos_Err[0]) / gG6DataYmag, BITMAP_FONT_TYPE_8_BY_13, buffer);


			glColor3f(YELLOW);
			sprintf(buffer, "%1.3f", JPos_Err[1]);	// Current Vel. Error
			renderText(gPosX_G6 + gSizeX, gPosY_G6 + 10 + gSizeY * (gG6DataYmax - JPos_Err[1]) / gG6DataYmag, BITMAP_FONT_TYPE_8_BY_13, buffer);


			// End of Graph if line
			}
*/

		}
		endRenderText();

}





void Draw_Rectangle(float x1, float y1, float x2, float y2)
{
	glBegin(GL_TRIANGLES);
	glVertex2i(x1, y1);
	glVertex2i(x2, y1);
	glVertex2i(x1, y2);
	glVertex2i(x1, y2);
	glVertex2i(x2, y1);
	glVertex2i(x2, y2);

	glEnd();

}

void Draw_Large_Text(float x, float y, char text[])
{
	char buffer[100];

	sprintf(buffer, text);
	renderText(x, y, BITMAP_FONT_TYPE_HELVETICA_18, buffer);

}


void Draw_Text(float x, float y, char text[])
{
	char buffer[100];

	sprintf(buffer, text);
	renderText(x, y, BITMAP_FONT_TYPE_HELVETICA_12, buffer);

}

