#include "ksh_Func.h"



unsigned int Avg_Count;	// data count.
double avg_Init_Sensor_data[6], prevAvg[6];



void AvgSensorFilter(double *iFTdata)
{
	double alpha;
	
	alpha = (Avg_Count - 1) / Avg_Count;
	
	for (int i = 0; i < 6; i++)
	{
		avg_Init_Sensor_data[i] = alpha*prevAvg[i] + (1 - alpha)*iFTdata[i];
		prevAvg[i] = avg_Init_Sensor_data[i];
	}
	
	//	for (i = 0; i < 6; i++)
	//		prevAvg[i] = avg_Init_Sensor_data[i];
	
	Avg_Count++;
	
	//	return avg_Init_Sensor_data;
	
}





// 1�� ������ ����
double lpf_alpha = 0.4;	//  ����ġ :  alpha �� ������, ����� ����, alpha �� ũ��, �ð� ������ Ŀ��.


//double prev_Force[6];
//double temp_Force[6];

double prev_FilteredValue1, prev_FilteredValue2;


double LPF_1st(double dFilterInput)
{
	double temp_Output;

	temp_Output = lpf_alpha * prev_FilteredValue1 + (1 - lpf_alpha)*dFilterInput;


	prev_FilteredValue1 = temp_Output;

	
	return temp_Output;
}

// 1�� ������ ����
double lpf_alpha2 = 0.4;	//  ����ġ :  alpha �� ������, ����� ����, alpha �� ũ��, �ð� ������ Ŀ��.


double LPF_1st_2(double dFilterInput)
{
	double temp_Output;

	temp_Output = lpf_alpha2 * prev_FilteredValue2 + (1 - lpf_alpha2)*dFilterInput;


	prev_FilteredValue2 = temp_Output;

	
	return temp_Output;

}








// sign function
double signum(const double x)
{
	return (x > 0) ? 1 : ((x < 0) ? -1 : 0);
}