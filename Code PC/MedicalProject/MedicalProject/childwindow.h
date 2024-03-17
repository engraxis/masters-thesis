#ifndef CHILDWINDOW_H
#define CHILDWINDOW_H

#include <QWidget>
#include "ui_childwindow.h"
#include "imageprocessing.h"
#include "qtimer.h"
#include "opencv2\imgproc\imgproc.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\photo\photo.hpp"
#include "opencv\cv.h"
#include "opencv\highgui.h"

//using namespace cv;

class childwindow : public QWidget
{
	Q_OBJECT

public:
	childwindow(QWidget *parent = 0);
	~childwindow();

private:
	Ui::childwindow ui;

	QTimer * childWindowTimer;
private:
	#define BUILDIN_CAM 0
	#define WEBCAM 1

	CvCapture * capture;
	IplImage * bgrImg;
	IplImage * imgAngio;
	IplImage * imgGWCrop;
	IplImage * imgGWSegment;
	IplImage * imgXRay;

	ImageProcessing * objImageProcessing;

	QImage tempImage;

private slots:
	void onTimeout();
};

#endif // CHILDWINDOW_H
