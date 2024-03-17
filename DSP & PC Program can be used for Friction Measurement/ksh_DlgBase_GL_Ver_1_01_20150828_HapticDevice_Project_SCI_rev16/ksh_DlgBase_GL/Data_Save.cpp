

// FILE �Լ� ���
#include <iostream>		// Definitions/declarations for iostream classes

#include "kshMenuGL.h"


#include "Data_Save.h"


//////////////			extern			/////////////////////
extern bool bButton_DataSave, bButton_FileSave;

extern lwtButton BDATASAVEButton, BSAVEFILEButton;
extern lwtSlider sDATASlider;

extern double Control_Rate_Set;



//////////////////////////////////////////////////////////////////////////
///////    File Save ���� Thread ����
extern DWORD ThreadID;
extern HANDLE hFileSaveThread;

extern DWORD WINAPI ThreadFunc(LPVOID temp);


//////////////////////////////////////////////////////////////////////////
//							DATA SAVE PARAMETERS						//
//////////////////////////////////////////////////////////////////////////
FILE *pFILE;


bool IsDataSaving = false;
bool IsDataSaveFinish = false;


// �� ���� �ð�
double Save_Time;

unsigned int NumOfData = 0; unsigned int IndexData = 0;
unsigned int Save_Data_Count = 0;	// Count the number of save data

// Save Time Counter
double dtime = 0.0;
double *Mem_Buffer;					// Save Data �޸� ���� �Ҵ� ������

// Error check for file save
errno_t err_File;

// ���� ���� �ε��� ���
int wFile_Cnt;

// �ڵ����� On/Off �÷���
bool bAutoSave;


void DataSave(double Input_Data[]) 
{			
	if (bButton_DataSave)
	{
		sDATASlider.Enable = true;
		NumOfData = ceil(Control_Rate_Set * Save_Time);
		dtime = (float) IndexData / Control_Rate_Set;


		if (IndexData < NumOfData)		// Enable Data Save
		{

			// Test

			//Mem_Buffer[Save_Data_Count] = dtime;

			//for (int j = 0; j < NUM_OF_DATA-1; j++)
			for (int j = 0; j < NUM_OF_DATA; j++)
			{
				//Mem_Buffer[Save_Data_Count + j + 1] = Input_Data[j];
				Mem_Buffer[Save_Data_Count + j] = Input_Data[j];

			}


			IndexData++;
			Save_Data_Count = Save_Data_Count + NUM_OF_DATA;
			sDATASlider.update(dtime);

			IsDataSaving = TRUE;
			IsDataSaveFinish = FALSE;

		}
		else
		{
			// ������ ���� �Ϸ�!
			bButton_DataSave = FALSE;
			IsDataSaving = FALSE;
			IsDataSaveFinish = TRUE;
			IndexData = 0;
			Save_Data_Count = 0;
			sDATASlider.Enable = FALSE;

			BDATASAVEButton.update(bButton_DataSave);
/*
			// ���� ����
			bButton_FileSave = TRUE;

			BSAVEFILEButton.update(bButton_FileSave);
*/

///////////////////////
			/////////		Data �ڵ� ���� ��ƾ		/////////////////////////
			bButton_FileSave = false;
			

			// Create file to save data in the memory
			// �޸𸮿� ����� data�� disk �� �����ϱ� ���� thread ȣ��
			// �˻�
			if (IsDataSaveFinish && !bButton_FileSave && bAutoSave)
			{
				// ����� data�� ���Ϸ� �����ϱ� ���� ������ ����
				// HANDLE CreateThread
				// �μ�1: ���� �Ӽ� ����: �ڽ� ���μ����� �ڵ��� ������� �ʴ� �� NULL �� ����
				// �μ�2: �������� ���� ũ��. 0���� �����ϸ� �� ������ (CreateThread�� ȣ���� ������)�� ���� ũ�⸦ ����
				//      �⺻������ �� �������� ������ 1 M�� ����Ǹ�, �� �� �� ������(���� 4K)�� Ȯ���Ǿ� ����.
				//      ���� ��� ȣ���� ���̰� ���� ��ٸ� ���� ũ�⸦ �� �ø� ���� ����.
				// �μ�3: �������� ���� �Լ� ���� (Entry point). �� �������� WinMain �Լ��� �ش��Ѵٰ� ���� ��.
				//		�� �Լ��κ��� �������� ������ �����ϸ�, ���� �Լ��� ����Ǹ� ������� �����.
				//		�����Լ��� ������ ���� ������ ������ ��.
				//		DWORD WINAPI ThreadFunc(LPVOID lpParameter);
				// �μ�4: ������ �����Լ��� �μ� (lpParameter). �μ��� ���� ��� NULL ����.
				// �μ�5: �������� Ư�� ����. 0�̸�, �ƹ� Ư�� ���� ���� ��������.
				//		CREATE_SUSPENDED �÷���: �����带 ����⸸ �ϰ�, ������ ���� ����.
				//		������ �����带 ������ ���� ResumeThread �Լ��� ȣ���ϸ� ��.
				// �μ�6: �������� ID�� �����ϱ� ���� ��¿� �μ�.
				//		������ ID�� �ʿ��� ���� ���� ���µ�, �� ���� NULL ����. (���: 9x�迭�� �� �μ��� NULL�̸�, �����尡 �������� ����.)
				//		ȣȯ���� ���ؼ��� DWORD���� ������ ������ �� ������ �Ѱܾ� ��.

				// ���: ������ �߻����� ���, NULL �� ������.
				hFileSaveThread = CreateThread(NULL, 0, ThreadFunc, NULL, 0, &ThreadID);

				// �������� �ڵ�� ������ ��ü�� �ٸ��Ƿ�, �ڵ��� �ݴ´ٰ� �ؼ� �����尡 ����Ǵ� ���� �ƴ�.
				CloseHandle(hFileSaveThread);
			}

					

////////////////////////////////////////////////

		}

	}
//	else if (FALSE == IsDataSaveFinish)		// �ʱ�ȭ!
	else
	{
		IsDataSaving = FALSE;

		
		IndexData = 0; //unsigned int DataCount = 0;
		Save_Data_Count = 0;	// Count the number of save data
	}

}

void Data_Save_File()
{	
	//if(bButton_FileSave)
	if(IsDataSaveFinish && !bButton_FileSave)
	{

		char str_wFileName[30];


		++wFile_Cnt;

		// Button On Display
		//SAVEFILEButton.update(bButton_FileSave);


		// ���� �����ϱ� ���� �ʱ�ȭ.
		IsDataSaveFinish = FALSE;

		// ���� ���� ���� ���� ��Ȳ ����͸��� ���� Slider �� progress
		sDATASlider.max = 100;		// ���� ���� �Ϸ� (100%)
		// ȭ�鿡 ǥ��
		sDATASlider.Enable = true;


		// ���ڿ� �Ҵ�
		sprintf(str_wFileName, "../Data/SaveData%d.DAT", wFile_Cnt);


		err_File = fopen_s(&pFILE, str_wFileName, "wt");
		//err_File = fopen_s(&pFILE, "../Data/SaveData.DAT", "wt");

		// error check
		if (err_File != 0)
		{
			MessageBox(NULL, "Error file open!", "File Open Error", MB_OK);

		}
		else	// Save Data to File.
		{

			for(int i = 0; i < NumOfData * NUM_OF_DATA; i = i + NUM_OF_DATA)
//			for(int i = 0; i < NumOfData * NUM_OF_DATA; i = i + 31)
			{
				//fprintf(pFILE, "%d \t %5.8f \t %5.8f \t %5.8f \t %5.8f \t %5.8f \t %5.8f \t %5.8f \t %5.8f \t %5.8f \t %5.8f \t %5.8f \t %5.8f \t %5.8f \t %5.8f \t %5.8f \t %5.8f \t %5.8f \t %5.8f \t %5.8f \t %5.8f \t %5.8f \t,// %5.8f \t %5.8f \t %5.8f \t %5.8f \t %5.8f \t %5.8f \t %5.8f \t %5.8f \t %5.8f \t %5.8f \t\n",
				// 21 ��
				//fprintf(pFILE, "%d \t %5.8f \t %5.8f \t %5.8f \t %5.8f \t %5.8f \t %5.8f \t %5.8f \t %5.8f \t %5.8f \t %5.8f \t %5.8f \t %5.8f \t %5.8f \t %5.8f \t %5.8f \t %5.8f \t %5.8f \t %5.8f \t %5.8f \t %5.8f \t %5.8f \t\n",
				// 11 ��
				fprintf(pFILE, "%d \t %5.3f \t %5.3f \t %5.3f \t %5.3f \t %5.3f \t %5.3f \t %5.3f \t %5.3f \t %5.3f \t %5.3f \t %5.3f\n",
				(i / NUM_OF_DATA),
				Mem_Buffer[i],
				Mem_Buffer[i + 1],
				Mem_Buffer[i + 2],
				Mem_Buffer[i + 3],
				Mem_Buffer[i + 4],
				Mem_Buffer[i + 5],
				Mem_Buffer[i + 6],
				Mem_Buffer[i + 7],
				Mem_Buffer[i + 8],
				Mem_Buffer[i + 9],
				Mem_Buffer[i + 10]
				
/*				,
				Mem_Buffer[i + 11],
				Mem_Buffer[i + 12],
				Mem_Buffer[i + 13],
				Mem_Buffer[i + 14],
				Mem_Buffer[i + 15],
				Mem_Buffer[i + 16],
				Mem_Buffer[i + 17],
				Mem_Buffer[i + 18],
				Mem_Buffer[i + 19],
				Mem_Buffer[i + 20],
				Mem_Buffer[i + 21],
				Mem_Buffer[i + 22],
				Mem_Buffer[i + 23],
				Mem_Buffer[i + 24],
				Mem_Buffer[i + 25],
				Mem_Buffer[i + 26],
				Mem_Buffer[i + 27],
				Mem_Buffer[i + 28],
				Mem_Buffer[i + 29],
				Mem_Buffer[i + 30]
*/
			);



			sDATASlider.update((float)i * 100 / (NumOfData * NUM_OF_DATA));

			}

		}

		fclose(pFILE);

		
//		NumOfData = 0;
		IndexData = 0; //unsigned int DataCount = 0;
		Save_Data_Count = 0;	// Count the number of save data



		// Data Save ���� ���� ����͸� slider bar�� ��ü
		sDATASlider.Enable = false;
		sDATASlider.max = Save_Time;		// Save Time
		

		bButton_FileSave = FALSE;

		BSAVEFILEButton.update(bButton_FileSave);



//////////////////////  �ڵ� ���� ��ƾ //////////////////////////////////////
		// ������ ���� �Ϸ�!
//		bButton_DataSave = true;

//		BDATASAVEButton.update(bButton_DataSave);

///////////////////////////////////////////////////////////


	}
}