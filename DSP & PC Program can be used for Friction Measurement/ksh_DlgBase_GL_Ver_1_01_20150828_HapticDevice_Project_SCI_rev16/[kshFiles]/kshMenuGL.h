#ifndef __KSHMENUGL_H_
#define __KSHMENUGL_H_

#include <windows.h>
/*
Usage: 

definition 
	lwtSlider KeSlider(positionX, positionY, width, height, minimum value, maximum value, "title", value);
	ex) lwtSlider KeSlider(10, 400, 100, 15, 100, 5000, "(1) Virtual Wall Stiffness", Ke);

graphic rendering
	KeSlider.draw(WSIZEX, WSIZEY);  

update value

	case WM_LBUTTONDOWN:
		IsLeftMouseOn = true;
		KeSlider.read(&Ke, lParam, IsLeftMouseOn);
		return 0;
		
	case WM_LBUTTONUP:
		IsLeftMouseOn = false;
		return 0;

	case WM_MOUSEMOVE:
		KeSlider.read(&Ke, lParam, IsLeftMouseOn);
*/

class lwtSlider {
public:
	
	bool Enable;

	int MouseX, MouseY;
	int x, y, w, h, i;
	char title[100], buff[100];
	double min, max, value;

	~lwtSlider();
	lwtSlider();
	lwtSlider(int x, int y, int w, int h, char* title);
	lwtSlider(int x, int y, int w, int h, double min, double max, char* title);
	lwtSlider(int X, int Y, int W, int H, double MIN, double MAX, char* TITLE, double initValue);

	void update(double VALUE);
	void draw(int WSIZEX, int WSIZEY);
	void draw(int WSIZEX, int WSIZEY, int scale);
	void read(double* VALUE, LPARAM lParam, bool LBUTTONDOWN);

	void updateLog(double VALUE);
	void readLog(double* VALUE, LPARAM lParam, bool LBUTTONDOWN);
};

class lwtSlider1 {
public:
	
	bool Enable;

	int MouseX, MouseY;
	int x, y, w, h, i;
	char title[100], buff[100];
	double min, max, value;

	~lwtSlider1();
	lwtSlider1();
	lwtSlider1(int x, int y, int w, int h, char* title);
	lwtSlider1(int x, int y, int w, int h, double min, double max, char* title);
	lwtSlider1(int X, int Y, int W, int H, double MIN, double MAX, char* TITLE, double initValue);

	void update(double VALUE);
	void draw(int WSIZEX, int WSIZEY);
	void draw(int WSIZEX, int WSIZEY, int scale);
	void read(double* VALUE, LPARAM lParam, bool LBUTTONDOWN);

	void updateLog(double VALUE);
	void readLog(double* VALUE, LPARAM lParam, bool LBUTTONDOWN);
};

class lwtSlider2 {
public:
	
	bool Enable;

	int MouseX, MouseY;
	int x, y, w, h, i;
	char title[100], buff[100];
	double min, max, value;

	~lwtSlider2();
	lwtSlider2();
	lwtSlider2(int x, int y, int w, int h, char* title);
	lwtSlider2(int x, int y, int w, int h, double min, double max, char* title);
	lwtSlider2(int X, int Y, int W, int H, double MIN, double MAX, char* TITLE, double initValue);

	void update(double VALUE);
	void draw(int WSIZEX, int WSIZEY);
	void draw(int WSIZEX, int WSIZEY, int scale);
	void read(double* VALUE, LPARAM lParam, bool LBUTTONDOWN);

	void updateLog(double VALUE);
	void readLog(double* VALUE, LPARAM lParam, bool LBUTTONDOWN);
};


class lwtButton {
public:
	
	bool Enable;
	// 한번 클릭 명령에 대한 내부 변수
	bool bClicked;

	int MouseX, MouseY;
	int x, y, w, h;
	char title[100];
	bool value;
		
	~lwtButton();
	lwtButton();
	lwtButton(int x, int y, int w, int h, char* title);
	lwtButton(int X, int Y, int W, int H, char* TITLE, bool initValue);

	void update(bool VALUE);
	void draw(int WSIZEX, int WSIZEY);
	void draw(int WSIZEX, int WSIZEY, int color);
	void read(bool* VALUE, LPARAM lParam, bool LBUTTONDOWN);
	
};











#endif