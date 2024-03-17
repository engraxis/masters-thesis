#include <stdio.h>
#include <math.h>
#include <GL/glut.h>
#include "kshMENUGL.h"
#include "kshFontGL.h"

lwtSlider::~lwtSlider() {

}

lwtSlider::lwtSlider() {
	Enable=true;
}


lwtSlider::lwtSlider(int X, int Y, int W, int H, char* TITLE) {
	x = X; y = Y; w = W; h = H;
	sprintf(title, TITLE);
	Enable=true;
}

lwtSlider::lwtSlider(int X, int Y, int W, int H, double MIN, double MAX, char* TITLE) {
	x = X; y = Y; w = W; h = H; min = MIN, max = MAX;
	sprintf(title, TITLE);
	Enable=true;
}

lwtSlider::lwtSlider(int X, int Y, int W, int H, double MIN, double MAX, char* TITLE, double initValue) {
	x = X; y = Y; w = W; h = H; min = MIN, max = MAX;
	sprintf(title, TITLE);
	update(initValue);
	Enable=true;
}

void lwtSlider::update(double VALUE) {
	value = VALUE;
	i = w*(value-min)/(max-min);

	if(i > w) i = w;
	if(i < 0) i = 0;
}

void lwtSlider::draw(int WSIZEX, int WSIZEY) {
	if(Enable) {
		beginRenderText(WSIZEX, WSIZEY);

		//glColor3ub(204, 249, 97);
		glColor3f(YELLOW);
		renderText(x, y-5, BITMAP_FONT_TYPE_HELVETICA_12, title);

		glColor3ub(255, 177, 9);
		sprintf(buff, "%5.3f", value);
		renderText(x+w+5, y+12, BITMAP_FONT_TYPE_HELVETICA_12, buff);	

		glColor3ub(255, 177, 9);
		glBegin(GL_LINE_LOOP);
		glVertex2f(x,   y  );
		glVertex2f(x,   y+h);
		glVertex2f(x+w, y+h);
		glVertex2f(x+w, y  );
		glEnd();

		glColor3ub(249, 208, 97);
		glBegin(GL_POLYGON);
		glVertex2f(x,   y  );
		glVertex2f(x,   y+h);
		glVertex2f(x+i, y+h);
		glVertex2f(x+i, y  );
		glEnd();

		endRenderText();
	}
}

void lwtSlider::draw(int WSIZEX, int WSIZEY, int scale) {
	if(Enable) {
		beginRenderText(WSIZEX, WSIZEY);

		//glColor3ub(204, 249, 97);
		glColor3f(YELLOW);
		renderText(x, y-5, BITMAP_FONT_TYPE_HELVETICA_12, title);

		glColor3ub(255, 177, 9);
		sprintf(buff, "%5.3f", value*scale);
		renderText(x+w+5, y+12, BITMAP_FONT_TYPE_HELVETICA_12, buff);	

		glColor3ub(255, 177, 9);
		glBegin(GL_LINE_LOOP);
		glVertex2f(x,   y  );
		glVertex2f(x,   y+h);
		glVertex2f(x+w, y+h);
		glVertex2f(x+w, y  );
		glEnd();

		glColor3ub(249, 208, 97);
		glBegin(GL_POLYGON);
		glVertex2f(x,   y  );
		glVertex2f(x,   y+h);
		glVertex2f(x+i, y+h);
		glVertex2f(x+i, y  );
		glEnd();

		endRenderText();
	}
}

void lwtSlider::read(double* VALUE, LPARAM lParam, bool LBUTTONDOWN) {
	if(Enable) {
		if(LBUTTONDOWN) {
			MouseX=LOWORD(lParam);
			MouseY=HIWORD(lParam);		
			if( (MouseX >= x) && (MouseX <= x+w) && (MouseY >= y) && (MouseY <= y+h)) { 
				i = MouseX-x;			
				value = (float)(max-min)*i/w+min;
				*VALUE = value;
			}
		}
	}
}

void lwtSlider::updateLog(double VALUE) {
	value = VALUE;
	i = w*log(value/min)/log(max/min);

	if(i > w) i = w;
	if(i < 0) i = 0;
}

void lwtSlider::readLog(double* VALUE, LPARAM lParam, bool LBUTTONDOWN) {
	if(Enable) {
		if(LBUTTONDOWN) {
			MouseX=LOWORD(lParam);
			MouseY=HIWORD(lParam);		
			if( (MouseX >= x) && (MouseX <= x+w) && (MouseY >= y) && (MouseY <= y+h)) { 
				i = MouseX-x;			
				value = exp(((float)i/w)*(log(max/min)) + log(min));
				*VALUE = value;
			}
		}
	}
}





lwtSlider1::~lwtSlider1() {

}

lwtSlider1::lwtSlider1() {
	Enable=true;
}


lwtSlider1::lwtSlider1(int X, int Y, int W, int H, char* TITLE) {
	x = X; y = Y; w = W; h = H;
	sprintf(title, TITLE);
	Enable=true;
}

lwtSlider1::lwtSlider1(int X, int Y, int W, int H, double MIN, double MAX, char* TITLE) {
	x = X; y = Y; w = W; h = H; min = MIN, max = MAX;
	sprintf(title, TITLE);
	Enable=true;
}

lwtSlider1::lwtSlider1(int X, int Y, int W, int H, double MIN, double MAX, char* TITLE, double initValue) {
	x = X; y = Y; w = W; h = H; min = MIN, max = MAX;
	sprintf(title, TITLE);
	update(initValue);
	Enable=true;
}

void lwtSlider1::update(double VALUE) {
	value = VALUE;
	i = w*(value-min)/(max-min);

	if(i > w) i = w;
	if(i < 0) i = 0;
}

void lwtSlider1::draw(int WSIZEX, int WSIZEY) {
	if(Enable) {
		beginRenderText(WSIZEX, WSIZEY);

		//glColor3ub(204, 249, 97);
		glColor3f(YELLOW);
		renderText(x, y-7, BITMAP_FONT_TYPE_HELVETICA_12, title);

		glColor3ub(255, 177, 9);
		sprintf(buff, "%5.1f", value);
		renderText(x+i-5, y+h+12, BITMAP_FONT_TYPE_HELVETICA_12, buff);	

		glColor3ub(255, 177, 9);
		glBegin(GL_LINE_LOOP);
		glVertex2f(x,   y  );
		glVertex2f(x,   y+h);
		glVertex2f(x+w, y+h);
		glVertex2f(x+w, y  );
		glEnd();

		glColor3ub(249, 208, 97);
		glBegin(GL_POLYGON);
		glVertex2f(x+i,   y  );
		glVertex2f(x+i,   y+h);
		glVertex2f(x+w, y+h);
		glVertex2f(x+w, y  );
		glEnd();

		endRenderText();
	}
}

void lwtSlider1::draw(int WSIZEX, int WSIZEY, int scale) {
	if(Enable) {
		beginRenderText(WSIZEX, WSIZEY);

		//glColor3ub(204, 249, 97);
		glColor3f(YELLOW);
		renderText(x, y-7, BITMAP_FONT_TYPE_HELVETICA_12, title);

		glColor3ub(255, 177, 9);
		sprintf(buff, "%5.1f", value*scale);
		renderText(x+i-5, y+h+12, BITMAP_FONT_TYPE_HELVETICA_12, buff);	

		glColor3ub(255, 177, 9);
		glBegin(GL_LINE_LOOP);
		glVertex2f(x,   y  );
		glVertex2f(x,   y+h);
		glVertex2f(x+w, y+h);
		glVertex2f(x+w, y  );
		glEnd();

		glColor3ub(249, 208, 97);
		glBegin(GL_POLYGON);
		glVertex2f(x+i,   y  );
		glVertex2f(x+i,   y+h);
		glVertex2f(x+w, y+h);
		glVertex2f(x+w, y  );
		glEnd();

		endRenderText();
	}
}

void lwtSlider1::read(double* VALUE, LPARAM lParam, bool LBUTTONDOWN) {
	if(Enable) {
		if(LBUTTONDOWN) {
			MouseX=LOWORD(lParam);
			MouseY=HIWORD(lParam);		
			if( (MouseX >= x) && (MouseX <= x+w) && (MouseY >= y) && (MouseY <= y+h)) { 
				i = MouseX-x;			
				value = (float)(max-min)*i/w+min;
				*VALUE = value;
			}
		}
	}
}

void lwtSlider1::updateLog(double VALUE) {
	value = VALUE;
	i = w*log(value/min)/log(max/min);

	if(i > w) i = w;
	if(i < 0) i = 0;
}

void lwtSlider1::readLog(double* VALUE, LPARAM lParam, bool LBUTTONDOWN) {
	if(Enable) {
		if(LBUTTONDOWN) {
			MouseX=LOWORD(lParam);
			MouseY=HIWORD(lParam);		
			if( (MouseX >= x) && (MouseX <= x+w) && (MouseY >= y) && (MouseY <= y+h)) { 
				i = MouseX-x;			
				value = exp(((float)i/w)*(log(max/min)) + log(min));
				*VALUE = value;
			}
		}
	}
}




lwtSlider2::~lwtSlider2() {

}

lwtSlider2::lwtSlider2() {
	Enable=true;
}


lwtSlider2::lwtSlider2(int X, int Y, int W, int H, char* TITLE) {
	x = X; y = Y; w = W; h = H;
	sprintf(title, TITLE);
	Enable=true;
}

lwtSlider2::lwtSlider2(int X, int Y, int W, int H, double MIN, double MAX, char* TITLE) {
	x = X; y = Y; w = W; h = H; min = MIN, max = MAX;
	sprintf(title, TITLE);
	Enable=true;
}

lwtSlider2::lwtSlider2(int X, int Y, int W, int H, double MIN, double MAX, char* TITLE, double initValue) {
	x = X; y = Y; w = W; h = H; min = MIN, max = MAX;
	sprintf(title, TITLE);
	update(initValue);
	Enable=true;
}

void lwtSlider2::update(double VALUE) {
	value = VALUE;
	i = w*(value-min)/(max-min);

	if(i > w) i = w;
	if(i < 0) i = 0;
}

void lwtSlider2::draw(int WSIZEX, int WSIZEY) {
	if(Enable) {
		beginRenderText(WSIZEX, WSIZEY);

		//glColor3ub(204, 249, 97);
		glColor3f(YELLOW);
		renderText(x, y-7, BITMAP_FONT_TYPE_HELVETICA_12, title);

		glColor3ub(255, 177, 9);
		sprintf(buff, "%5.1f", value);
		renderText(x+i-25, y+h+12, BITMAP_FONT_TYPE_HELVETICA_12, buff);	

		glColor3ub(255, 177, 9);
		glBegin(GL_LINE_LOOP);
		glVertex2f(x,   y  );
		glVertex2f(x,   y+h);
		glVertex2f(x+w, y+h);
		glVertex2f(x+w, y  );
		glEnd();

		glColor3ub(249, 208, 97);
		glBegin(GL_POLYGON);
		glVertex2f(x,   y  );
		glVertex2f(x,   y+h);
		glVertex2f(x+i, y+h);
		glVertex2f(x+i, y  );
		glEnd();

		endRenderText();
	}
}

void lwtSlider2::draw(int WSIZEX, int WSIZEY, int scale) {
	if(Enable) {
		beginRenderText(WSIZEX, WSIZEY);

		//glColor3ub(204, 249, 97);
		glColor3f(YELLOW);
		renderText(x, y-7, BITMAP_FONT_TYPE_HELVETICA_12, title);

		glColor3ub(255, 177, 9);
		sprintf(buff, "%5.1f", value*scale);
		renderText(x+i-5, y+h+12, BITMAP_FONT_TYPE_HELVETICA_12, buff);	

		glColor3ub(255, 177, 9);
		glBegin(GL_LINE_LOOP);
		glVertex2f(x,   y  );
		glVertex2f(x,   y+h);
		glVertex2f(x+w, y+h);
		glVertex2f(x+w, y  );
		glEnd();

		glColor3ub(249, 208, 97);
		glBegin(GL_POLYGON);
		glVertex2f(x,   y  );
		glVertex2f(x,   y+h);
		glVertex2f(x+i, y+h);
		glVertex2f(x+i, y  );
		glEnd();

		endRenderText();
	}
}

void lwtSlider2::read(double* VALUE, LPARAM lParam, bool LBUTTONDOWN) {
	if(Enable) {
		if(LBUTTONDOWN) {
			MouseX=LOWORD(lParam);
			MouseY=HIWORD(lParam);		
			if( (MouseX >= x) && (MouseX <= x+w) && (MouseY >= y) && (MouseY <= y+h)) { 
				i = MouseX-x;			
				value = (float)(max-min)*i/w+min;
				*VALUE = value;
			}
		}
	}
}

void lwtSlider2::updateLog(double VALUE) {
	value = VALUE;
	i = w*log(value/min)/log(max/min);

	if(i > w) i = w;
	if(i < 0) i = 0;
}

void lwtSlider2::readLog(double* VALUE, LPARAM lParam, bool LBUTTONDOWN) {
	if(Enable) {
		if(LBUTTONDOWN) {
			MouseX=LOWORD(lParam);
			MouseY=HIWORD(lParam);		
			if( (MouseX >= x) && (MouseX <= x+w) && (MouseY >= y) && (MouseY <= y+h)) { 
				i = MouseX-x;			
				value = exp(((float)i/w)*(log(max/min)) + log(min));
				*VALUE = value;
			}
		}
	}
}





lwtButton::~lwtButton() {

}

lwtButton::lwtButton() {
	Enable=true;
	bClicked = false;
}


lwtButton::lwtButton(int X, int Y, int W, int H, char* TITLE) {
	x = X; y = Y; w = W; h = H;
	sprintf(title, TITLE);
	Enable=true;
	bClicked = false;
}

lwtButton::lwtButton(int X, int Y, int W, int H, char* TITLE, bool initValue) {
	x = X; y = Y; w = W; h = H; 
	sprintf(title, TITLE);
	update(initValue);
	Enable=true;
	bClicked = false;
}

void lwtButton::update(bool VALUE) {
	value = VALUE;
}

void lwtButton::draw(int WSIZEX, int WSIZEY) {
	if(Enable) {
		beginRenderText(WSIZEX, WSIZEY);
		if(!value) {			
			glColor3ub(180, 245, 145);
			glBegin(GL_LINE_LOOP);
			glVertex2f(x,   y  );
			glVertex2f(x,   y+h);
			glVertex2f(x+w, y+h);
			glVertex2f(x+w, y  );
			glEnd();

			glColor3ub(180, 245, 145);
			renderText(x+3, y+14, BITMAP_FONT_TYPE_HELVETICA_12, title);
		}
		else {				
			glColor3ub(180, 245, 145);
			glBegin(GL_POLYGON);
			glVertex2f(x,   y  );
			glVertex2f(x,   y+h);
			glVertex2f(x+w, y+h);
			glVertex2f(x+w, y  );
			glEnd();

			glColor3ub(0, 0, 0);
			renderText(x+3, y+14, BITMAP_FONT_TYPE_HELVETICA_12, title);
		}		
		endRenderText();
	}
}

void lwtButton::read(bool* VALUE, LPARAM lParam, bool LBUTTONDOWN) {
	if(Enable) {
		if(LBUTTONDOWN) {
			MouseX=LOWORD(lParam);
			MouseY=HIWORD(lParam);		
			if( (MouseX >= x) && (MouseX <= x+w) && (MouseY >= y) && (MouseY <= y+h)) { 
				value =! value;
				*VALUE = value;

				// 만약 이 버튼이 눌렸다면, clicked 관련 변수 true
				bClicked = true;
			}
		}
	}
}


void lwtButton::draw(int WSIZEX, int WSIZEY, int color) {
	if(Enable) {
		beginRenderText(WSIZEX, WSIZEY);
		if(color==1)
			glColor3ub(180, 245, 145);
		else if(color==2)
			glColor3ub(255, 177, 9);
		else if(color==3)
			glColor3ub(200, 100, 100);
		else if(color==4)
			glColor3ub(20, 255, 20);
		else if(color==5)
			glColor3ub(120, 120, 200);
		else if(color==6)
			glColor3ub(255, 255, 20);
		else if(color==10)
			glColor3ub(230, 230, 10);
		else
			glColor3ub(255, 255, 255);

		if(!value) {
			glBegin(GL_LINE_LOOP);
			glVertex2f(x,   y  );
			glVertex2f(x,   y+h);
			glVertex2f(x+w, y+h);
			glVertex2f(x+w, y  );
			glEnd();
			renderText(x+10, y+16, BITMAP_FONT_TYPE_HELVETICA_12, title);
		}
		else {				
			glBegin(GL_POLYGON);
			glVertex2f(x,   y  );
			glVertex2f(x,   y+h);
			glVertex2f(x+w, y+h);
			glVertex2f(x+w, y  );
			glEnd();
			glColor3ub(0, 0, 0);
			renderText(x+10, y+16, BITMAP_FONT_TYPE_HELVETICA_12, title);
		}		
		endRenderText();
	}
}