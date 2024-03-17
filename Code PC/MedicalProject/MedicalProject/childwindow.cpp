#include "childwindow.h"

childwindow::childwindow(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	this->setFixedSize(this->size());
	this->setWindowTitle("Medical Project - Graphic Display Window");
	this->move(20, 20);
	
	ui.label_rgbAngio->setScaledContents(true);
	ui.label_rgbGWSegment->setScaledContents(true);
	ui.label_rgbGWXray->setScaledContents(true);
	ui.label_rgbGWCrpTangent->setScaledContents(true);

	//Initialize image related to Graphic Window
	capture = cvCreateCameraCapture(WEBCAM);	//Capturing and closing the capture to specify image dimensions
	//capture = cvCreateCameraCapture(BUILDIN_CAM);
	bgrImg = cvQueryFrame(capture);
	imgAngio = cvCreateImage(cvGetSize(bgrImg), 8, 1);
	imgGWCrop = cvCreateImage(cvSize(63, 63), 8, 1);
	imgGWSegment = cvCreateImage(cvGetSize(bgrImg), 8, 1);
	imgXRay = cvCreateImage(cvGetSize(bgrImg), 8, 1);
	cvZero(imgAngio);
	cvZero(imgGWCrop);
	cvZero(imgGWSegment);
	cvReleaseCapture(&capture);			//Releasing the capture as all the image acquisition and processing must be done
										//in the imageProcessing class
	//Initialize ImageProcessing class
	objImageProcessing = new ImageProcessing(this);
	ImageProcessing::initializeImageProcessing();
	objImageProcessing->createImgProcThread();

	//Initialize timer
	childWindowTimer = new QTimer(this);
	connect(childWindowTimer, SIGNAL(timeout()), this, SLOT(onTimeout()));
	childWindowTimer->start(33);
}

childwindow::~childwindow()
{
	childWindowTimer->stop();
	objImageProcessing->terminateAll();
	delete objImageProcessing;
	//cvReleaseImage(&bgrImg);		//The first image grabbed by camera, releasing it shows error
	cvReleaseImage(&imgAngio);
	cvReleaseImage(&imgGWCrop);
	cvReleaseImage(&imgGWSegment);
	cvReleaseImage(&imgXRay);
}

void childwindow::onTimeout()
{
	IplImage * cimgAngio = ImageProcessing::oImgTradeAngio->getImageAccess();	//Image is released in ImageProcessingTrade
	if (cimgAngio != NULL)
	{
		cvCopy(cimgAngio, imgAngio);
		ImageProcessing::oImgTradeAngio->releaseAccess();
		tempImage = QImage((unsigned char *)imgAngio->imageDataOrigin, imgAngio->width, imgAngio->height, QImage::Format_Grayscale8);
		ui.label_rgbAngio->setPixmap(QPixmap::fromImage(tempImage));
		ui.label_rgbAngio->show();
	}
	else
	{
		ImageProcessing::oImgTradeAngio->releaseAccess();
	}

	IplImage * cimgGWSegment = ImageProcessing::oImgTradeGWSeg->getImageAccess();	//Image is released in ImageProcessingTrade
	if (cimgGWSegment != NULL)
	{
		cvCopy(cimgGWSegment, imgGWSegment);
		ImageProcessing::oImgTradeGWSeg->releaseAccess();
		tempImage = QImage((unsigned char *)imgGWSegment->imageDataOrigin, imgGWSegment->width, imgGWSegment->height, QImage::Format_Grayscale8);
		ui.label_rgbGWSegment->setPixmap(QPixmap::fromImage(tempImage));
		ui.label_rgbGWSegment->show();
	}
	else 
	{
		ImageProcessing::oImgTradeGWSeg->releaseAccess();
	}

	IplImage * cimgGWCrop = ImageProcessing::oImgTradeCrp->getImageAccess();	//Image is released in ImageProcessingTrade
	if (cimgGWCrop != NULL)
	{
		cvCopy(cimgGWCrop, imgGWCrop);
		ImageProcessing::oImgTradeCrp->releaseAccess();
		tempImage = QImage((unsigned char *)imgGWCrop->imageDataOrigin, imgGWCrop->width, imgGWCrop->height, QImage::Format_Grayscale8);
		ui.label_rgbGWCrpTangent->setPixmap(QPixmap::fromImage(tempImage));
		ui.label_rgbGWCrpTangent->show();
	}
	else
	{
		ImageProcessing::oImgTradeCrp->releaseAccess();
	}


	IplImage * cimgXRay = ImageProcessing::oImgTradeXRay->getImageAccess();	//Image is released in ImageProcessingTrade
	if (cimgXRay != NULL)
	{
		cvCopy(cimgXRay, imgXRay);
		ImageProcessing::oImgTradeXRay->releaseAccess();
		tempImage = QImage((unsigned char *)imgXRay->imageDataOrigin, imgXRay->width, imgXRay->height, QImage::Format_Grayscale8);
		ui.label_rgbGWXray->setPixmap(QPixmap::fromImage(tempImage));
		ui.label_rgbGWXray->show();
	}
	else
	{
		ImageProcessing::oImgTradeXRay->releaseAccess();
	}


}