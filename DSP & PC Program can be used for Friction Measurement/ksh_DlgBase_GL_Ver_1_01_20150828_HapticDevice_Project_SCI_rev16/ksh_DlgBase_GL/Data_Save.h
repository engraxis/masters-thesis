#ifndef __DATA_SAVE_H_
#define __DATA_SAVE_H_


// Number of Save data
//#define NUM_OF_DATA 31
//#define NUM_OF_DATA 21
#define NUM_OF_DATA 11





// 동적 메모리에 DATA 저장
//void DataSave(void);
void DataSave(double Input_Data[]);

// 하드 디스크에 DATA File 생성및 저장
void Data_Save_File();


#endif