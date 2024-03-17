#include <windows.h>
#include "resource.h"

#include <stdio.h>


#include "Dlg_Setting.h"


#include "Kinematics_2DOF.h"


/////////////////			extern				///////////////////////////
extern double Robot_JPos[2], Robot_Pos[2];






/////////////////////////////////////////////////////////////////////////
//                        Setting ������                               //
/////////////////////////////////////////////////////////////////////////

// Task, Joint ��� ���� ����
bool bPathMode;
bool bRPathMode;


// edit ��Ʈ�� ���ڿ� ����
char c_POSX[10], c_POSY[10], c_J1[10], c_J2[10], c_Tf[10];

char c_Kne[10], c_K1[10], c_SPRLS_gamma[10], c_SPRLS_beta[10], c_Damping_Gain[10], c_Imp_Wn_Gain[10], c_RIC_Noise[10];
char c_SPRLS_P_Limit_Adaptive[10];



// Setting ���̾�α� ��ġ ����
// ���� ��ġ(Task, Joint), ���� �ð�
double End_Pos[2], End_JPos[2], Tf;


double Kne, RIC_K1, SPRLS_gamma, SPRLS_beta, Damping_Gain, Imp_Wn_Gain, RIC_Noise;
double SPRLS_P_Limit_Adaptive;


LRESULT CALLBACK Setting(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:

		// �ʱ�ȭ

		// Modal ���̾�α��̹Ƿ�, ���� ���¸� ���� �����Ѵ�.
		// Control OFF

		////////////

		////////////////
		// ���� ��ư �ʱ�ȭ
		if (bPathMode)
			CheckRadioButton(hDlg, IDC_RTASK, IDC_RJOINT, IDC_RTASK);
		else
			CheckRadioButton(hDlg, IDC_RTASK, IDC_RJOINT, IDC_RJOINT);


		///////////////////////////////////////////////////
		// ����Ʈ ��ư �ʱ�ȭ
		// ����Ʈ ��ư�� �ش��ϴ� ���� ���ڿ��� ��ȯ
		
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


		

		// ��Ʈ���� ���ڿ��� �����ϴ� �Լ�
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

			// ���õ� Path planning ��� �ľ�
			bPathMode = bRPathMode;

			// ���õ� Path ��忡 ���� �ش� ����� ��� ������ �б�
			if (bPathMode)
			{
				// Task Mode

				// Edit �� ���ڿ��� �б� (Task-space �Է�)
				GetDlgItemText(hDlg, IDC_EPOSX, c_POSX, 10);
				GetDlgItemText(hDlg, IDC_EPOSY, c_POSY, 10);

				// ���ڿ��� float �� double ������ ��ȯ
				End_Pos[0] = atof(c_POSX);
				End_Pos[1] = atof(c_POSY);


				// Inverse Kinematics ���
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

				// ���ڿ��� float �� double ������ ��ȯ (Joint-space �Է�)
				End_JPos[0] = atof(c_J1);
				End_JPos[1] = atof(c_J2);


				// Forward Kinematics ���
				double FK_temp[2];
				//ptemp = FKine(End_JPos);

				FKine(End_JPos, FK_temp);

				for (int i = 0; i < 2; i++)
					End_Pos[i] = FK_temp[i];


			}

			//////////////////////////////////
			//       ����                   //
			//////////////////////////////////

			// Edit �� ���ڿ��� �б�
			GetDlgItemText(hDlg, IDC_ETF, c_Tf, 10);
			GetDlgItemText(hDlg, IDC_EDIT_KNE, c_Kne, 10);
			GetDlgItemText(hDlg, IDC_EDIT_K1, c_K1, 10);
			GetDlgItemText(hDlg, IDC_EDIT_SPRLS_GAMMA, c_SPRLS_gamma, 10);
			GetDlgItemText(hDlg, IDC_EDIT_SPRLS_BETA, c_SPRLS_beta, 10);
			GetDlgItemText(hDlg, IDC_EDIT_DAMPING_GAIN, c_Damping_Gain, 10);
			GetDlgItemText(hDlg, IDC_EDIT_IMP_WN_GAIN, c_Imp_Wn_Gain, 10);
			GetDlgItemText(hDlg, IDC_EDIT_RIC_NOISE, c_RIC_Noise, 10);
			GetDlgItemText(hDlg, IDC_EDIT_SPRLS_P_LIMIT_ADAPTIVE, c_SPRLS_P_Limit_Adaptive, 10);

			
			// ���ڿ��� float �� double ������ ��ȯ
			Tf = atof(c_Tf);
			Kne = atof(c_Kne);
			RIC_K1 = atof(c_K1);
			SPRLS_gamma = atof(c_SPRLS_gamma);
			SPRLS_beta = atof(c_SPRLS_gamma);
			Damping_Gain = atof(c_SPRLS_beta);
			Imp_Wn_Gain = atof(c_Imp_Wn_Gain);
			RIC_Noise = atof(c_RIC_Noise);
			SPRLS_P_Limit_Adaptive = atof(c_SPRLS_P_Limit_Adaptive);

			
			/////////// �ݱ�///////////
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}


		// Radio button ���
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



		// ���
		if (LOWORD(wParam) == IDCANCEL)
		{
			// ���


			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}




	}
	return FALSE;
}