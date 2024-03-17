

// FILE 함수 사용
#include <iostream>		// Definitions/declarations for iostream classes

#include "kshMenuGL.h"


#include "Data_Save.h"


//////////////			extern			/////////////////////
extern bool bButton_DataSave, bButton_FileSave;

extern lwtButton BDATASAVEButton, BSAVEFILEButton;
extern lwtSlider sDATASlider;

extern double Control_Rate_Set;



//////////////////////////////////////////////////////////////////////////
///////    File Save 관련 Thread 변수
extern DWORD ThreadID;
extern HANDLE hFileSaveThread;

extern DWORD WINAPI ThreadFunc(LPVOID temp);


//////////////////////////////////////////////////////////////////////////
//							DATA SAVE PARAMETERS						//
//////////////////////////////////////////////////////////////////////////
FILE *pFILE;


bool IsDataSaving = false;
bool IsDataSaveFinish = false;


// 총 저장 시간
double Save_Time;

unsigned int NumOfData = 0; unsigned int IndexData = 0;
unsigned int Save_Data_Count = 0;	// Count the number of save data

// Save Time Counter
double dtime = 0.0;
double *Mem_Buffer;					// Save Data 메모리 동적 할당 포인터

// Error check for file save
errno_t err_File;

// 저장 파일 인덱싱 기능
int wFile_Cnt;

// 자동저장 On/Off 플래그
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
			// 데이터 저장 완료!
			bButton_DataSave = FALSE;
			IsDataSaving = FALSE;
			IsDataSaveFinish = TRUE;
			IndexData = 0;
			Save_Data_Count = 0;
			sDATASlider.Enable = FALSE;

			BDATASAVEButton.update(bButton_DataSave);
/*
			// 새로 수정
			bButton_FileSave = TRUE;

			BSAVEFILEButton.update(bButton_FileSave);
*/

///////////////////////
			/////////		Data 자동 저장 루틴		/////////////////////////
			bButton_FileSave = false;
			

			// Create file to save data in the memory
			// 메모리에 저장된 data를 disk 에 저장하기 위해 thread 호출
			// 검사
			if (IsDataSaveFinish && !bButton_FileSave && bAutoSave)
			{
				// 저장된 data를 파일로 저장하기 위한 스레드 생성
				// HANDLE CreateThread
				// 인수1: 보안 속성 지정: 자식 프로세스로 핸들을 상속하지 않는 한 NULL 로 지정
				// 인수2: 스레드의 스택 크기. 0으로 지정하면 주 스레드 (CreateThread를 호출한 스레드)와 같은 크기를 가짐
				//      기본적으로 주 스레드의 스택은 1 M가 예약되며, 그 중 한 페이지(보통 4K)만 확정되어 있음.
				//      만약 재귀 호출의 깊이가 아주 깊다면 스택 크기를 더 늘릴 수도 있음.
				// 인수3: 스레드의 시작 함수 지정 (Entry point). 주 스레드의 WinMain 함수에 해당한다고 보면 됨.
				//		이 함수로부터 스레드의 실행을 시작하며, 시작 함수가 종료되면 스레드로 종료됨.
				//		시작함수는 다음과 같은 원형을 가져야 함.
				//		DWORD WINAPI ThreadFunc(LPVOID lpParameter);
				// 인수4: 스레드 시작함수의 인수 (lpParameter). 인수가 없을 경우 NULL 전달.
				// 인수5: 스레드의 특성 지정. 0이면, 아무 특성 없는 보통 스레드임.
				//		CREATE_SUSPENDED 플래그: 스레드를 만들기만 하고, 실행은 하지 않음.
				//		중지된 스레드를 실행할 때는 ResumeThread 함수를 호출하면 됨.
				// 인수6: 스레드의 ID를 리턴하기 위한 출력용 인수.
				//		스레드 ID가 필요한 경우는 별로 없는데, 이 경우는 NULL 전달. (비고: 9x계열은 이 인수가 NULL이면, 스레드가 생성되지 않음.)
				//		호환성을 위해서는 DWORD형의 변수를 선언한 후 번지를 넘겨야 함.

				// 결과: 에러가 발생했을 경우, NULL 을 리턴함.
				hFileSaveThread = CreateThread(NULL, 0, ThreadFunc, NULL, 0, &ThreadID);

				// 스레드의 핸들과 스레드 자체는 다르므로, 핸들을 닫는다고 해서 스레드가 종료되는 것은 아님.
				CloseHandle(hFileSaveThread);
			}

					

////////////////////////////////////////////////

		}

	}
//	else if (FALSE == IsDataSaveFinish)		// 초기화!
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


		// 새로 저장하기 위해 초기화.
		IsDataSaveFinish = FALSE;

		// 파일 저장 과정 진행 상황 모니터링을 위한 Slider 바 progress
		sDATASlider.max = 100;		// 파일 저장 완료 (100%)
		// 화면에 표시
		sDATASlider.Enable = true;


		// 문자열 할당
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
				// 21 개
				//fprintf(pFILE, "%d \t %5.8f \t %5.8f \t %5.8f \t %5.8f \t %5.8f \t %5.8f \t %5.8f \t %5.8f \t %5.8f \t %5.8f \t %5.8f \t %5.8f \t %5.8f \t %5.8f \t %5.8f \t %5.8f \t %5.8f \t %5.8f \t %5.8f \t %5.8f \t %5.8f \t\n",
				// 11 개
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



		// Data Save 진행 과정 모니터링 slider bar로 교체
		sDATASlider.Enable = false;
		sDATASlider.max = Save_Time;		// Save Time
		

		bButton_FileSave = FALSE;

		BSAVEFILEButton.update(bButton_FileSave);



//////////////////////  자동 저장 루틴 //////////////////////////////////////
		// 데이터 저장 완료!
//		bButton_DataSave = true;

//		BDATASAVEButton.update(bButton_DataSave);

///////////////////////////////////////////////////////////


	}
}