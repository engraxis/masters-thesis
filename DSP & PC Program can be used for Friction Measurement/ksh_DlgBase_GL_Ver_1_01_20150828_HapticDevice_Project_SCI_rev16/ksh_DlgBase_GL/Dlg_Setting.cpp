#include <windows.h>
#include "resource.h"

#include <stdio.h>


#include "Dlg_Setting.h"


#include "Kinematics_2DOF.h"


/////////////////			extern				///////////////////////////
extern double Robot_JPos[2], Robot_Pos[2];






/////////////////////////////////////////////////////////////////////////
//                        Setting 윈도우                               //
/////////////////////////////////////////////////////////////////////////

// Task, Joint 모드 설정 변수
bool bPathMode;
bool bRPathMode;


// edit 컨트롤 문자열 변수
char c_POSX[10], c_POSY[10], c_J1[10], c_J2[10], c_Tf[10];

char c_Kne[10], c_K1[10], c_SPRLS_gamma[10], c_SPRLS_beta[10], c_Damping_Gain[10], c_Imp_Wn_Gain[10], c_RIC_Noise[10];
char c_SPRLS_P_Limit_Adaptive[10];



// Setting 다이얼로그 위치 변수
// 목적 위치(Task, Joint), 최종 시간
double End_Pos[2], End_JPos[2], Tf;


double Kne, RIC_K1, SPRLS_gamma, SPRLS_beta, Damping_Gain, Imp_Wn_Gain, RIC_Noise;
double SPRLS_P_Limit_Adaptive;


LRESULT CALLBACK Setting(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:

		// 초기화

		// Modal 다이얼로그이므로, 제어 상태를 먼저 종료한다.
		// Control OFF

		////////////

		////////////////
		// 라디오 버튼 초기화
		if (bPathMode)
			CheckRadioButton(hDlg, IDC_RTASK, IDC_RJOINT, IDC_RTASK);
		else
			CheckRadioButton(hDlg, IDC_RTASK, IDC_RJOINT, IDC_RJOINT);


		///////////////////////////////////////////////////
		// 에디트 버튼 초기화
		// 에디트 버튼에 해당하는 값을 문자열로 변환
		
		sprintf(c_POSX, "%3.5f", End_Pos[0]);
		sprintf(c_POSY, "%3.5f", End_Pos[1]);
		sprintf(c_J1, "%3.5f", End_JPos[0]);
		sprintf(c_J2, "%3.5f", End_JPos[1]);
		sprintf(c_Tf, "%3.5f", Tf);

		sprintf(c_Kne, "%8.1f", Kne);
		sprintf(c_K1, "%8.1f", RIC_K1);
		sprintf(c_SPRLS_gamma, "%5.4f", SPRLS_gamma);
		sprintf(c_SPRLS_beta, "%8.1f", SPRLS_beta);
		sprintf(c_Damping_Gain, "%5.4f", Damping_Gain);
		sprintf(c_Imp_Wn_Gain, "%5.4f", Imp_Wn_Gain);
		sprintf(c_RIC_Noise, "%5.4f", RIC_Noise);
		sprintf(c_SPRLS_P_Limit_Adaptive, "%6.3f", SPRLS_P_Limit_Adaptive);


		

		// 컨트롤의 문자열을 설정하는 함수
		SetDlgItemText(hDlg, IDC_EPOSX, c_POSX);
		SetDlgItemText(hDlg, IDC_EPOSY, c_POSY);
		SetDlgItemText(hDlg, IDC_EJ1, c_J1);
		SetDlgItemText(hDlg, IDC_EJ2, c_J2);
		SetDlgItemText(hDlg, IDC_ETF, c_Tf);

		SetDlgItemText(hDlg, IDC_EDIT_KNE, c_Kne);
		SetDlgItemText(hDlg, IDC_EDIT_K1, c_K1);
		SetDlgItemText(hDlg, IDC_EDIT_SPRLS_GAMMA, c_SPRLS_gamma);
		SetDlgItemText(hDlg, IDC_EDIT_SPRLS_BETA, c_SPRLS_beta);
		SetDlgItemText(hDlg, IDC_EDIT_DAMPING_GAIN, c_Damping_Gain);
		SetDlgItemText(hDlg, IDC_EDIT_IMP_WN_GAIN, c_Imp_Wn_Gain);
		SetDlgItemText(hDlg, IDC_EDIT_RIC_NOISE, c_RIC_Noise);
		SetDlgItemText(hDlg, IDC_EDIT_SPRLS_P_LIMIT_ADAPTIVE, c_SPRLS_P_Limit_Adaptive);


		return TRUE;

	case WM_COMMAND:
		//if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		if (LOWORD(wParam) == IDOK)
		{

			// 선택된 Path planning 모드 파악
			bPathMode = bRPathMode;

			// 선택된 Path 모드에 따라서 해당 모드의 경로 설정값 읽기
			if (bPathMode)
			{
				// Task Mode

				// Edit 값 문자열로 읽기 (Task-space 입력)
				GetDlgItemText(hDlg, IDC_EPOSX, c_POSX, 10);
				GetDlgItemText(hDlg, IDC_EPOSY, c_POSY, 10);

				// 문자열을 float 및 double 형으로 변환
				End_Pos[0] = atof(c_POSX);
				End_Pos[1] = atof(c_POSY);


				// Inverse Kinematics 계산
				double IK_temp[2];

				// 1: input, 2: output
				//ptemp = IKine(End_Pos, ptemp);
				IKine(End_Pos, IK_temp);

				for (int i = 0; i < 2; i++)
					End_JPos[i] = IK_temp[i];

			}
			else
			{
				// Joint Mode
				GetDlgItemText(hDlg, IDC_EJ1, c_J1, 10);
				GetDlgItemText(hDlg, IDC_EJ2, c_J2, 10);

				// 문자열을 float 및 double 형으로 변환 (Joint-space 입력)
				End_JPos[0] = atof(c_J1);
				End_JPos[1] = atof(c_J2);


				// Forward Kinematics 계산
				double FK_temp[2];
				//ptemp = FKine(End_JPos);

				FKine(End_JPos, FK_temp);

				for (int i = 0; i < 2; i++)
					End_Pos[i] = FK_temp[i];


			}

			//////////////////////////////////
			//       공통                   //
			//////////////////////////////////

			// Edit 값 문자열로 읽기
			GetDlgItemText(hDlg, IDC_ETF, c_Tf, 10);
			GetDlgItemText(hDlg, IDC_EDIT_KNE, c_Kne, 10);
			GetDlgItemText(hDlg, IDC_EDIT_K1, c_K1, 10);
			GetDlgItemText(hDlg, IDC_EDIT_SPRLS_GAMMA, c_SPRLS_gamma, 10);
			GetDlgItemText(hDlg, IDC_EDIT_SPRLS_BETA, c_SPRLS_beta, 10);
			GetDlgItemText(hDlg, IDC_EDIT_DAMPING_GAIN, c_Damping_Gain, 10);
			GetDlgItemText(hDlg, IDC_EDIT_IMP_WN_GAIN, c_Imp_Wn_Gain, 10);
			GetDlgItemText(hDlg, IDC_EDIT_RIC_NOISE, c_RIC_Noise, 10);
			GetDlgItemText(hDlg, IDC_EDIT_SPRLS_P_LIMIT_ADAPTIVE, c_SPRLS_P_Limit_Adaptive, 10);

			
			// 문자열을 float 및 double 형으로 변환
			Tf = atof(c_Tf);
			Kne = atof(c_Kne);
			RIC_K1 = atof(c_K1);
			SPRLS_gamma = atof(c_SPRLS_gamma);
			SPRLS_beta = atof(c_SPRLS_gamma);
			Damping_Gain = atof(c_SPRLS_beta);
			Imp_Wn_Gain = atof(c_Imp_Wn_Gain);
			RIC_Noise = atof(c_RIC_Noise);
			SPRLS_P_Limit_Adaptive = atof(c_SPRLS_P_Limit_Adaptive);

			
			/////////// 닫기///////////
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}


		// Radio button 명령
		if (HIWORD(wParam) == BN_CLICKED)
		{
			switch (LOWORD(wParam))
			{
			case IDC_RTASK:

				// Task mode path planning
				bRPathMode = true;


				break;
			case IDC_RJOINT:

				// Joint mode path planning
				bRPathMode = false;

				break;
			}
		}



		// 취소
		if (LOWORD(wParam) == IDCANCEL)
		{
			// 취소


			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}




	}
	return FALSE;
}