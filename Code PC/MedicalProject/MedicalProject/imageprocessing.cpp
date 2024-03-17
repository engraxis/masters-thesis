#include "imageprocessing.h"

#pragma region StaticVariablesInitialization
CvCapture * ImageProcessing::capture = NULL;
IplImage * ImageProcessing::bgrImg = NULL;
IplImage * ImageProcessing::imgAngio = NULL;
IplImage * ImageProcessing::imgXRay = NULL;
IplImage * ImageProcessing::staticAngio = NULL;

ImageProcessingTrade * ImageProcessing::oImgTradeAngio = NULL;
ImageProcessingTrade * ImageProcessing::oImgTradeGWSeg = NULL;
ImageProcessingTrade * ImageProcessing::oImgTradeCrp = NULL;
ImageProcessingTrade * ImageProcessing::oImgTradeXRay = NULL;

CvPoint ImageProcessing::gwTipLocFImg = { 0,0 };
CvPoint ImageProcessing::gwTipLocCImg = { 0,0 };
double ImageProcessing::innerProdAng = 0.0;
double ImageProcessing::innerProdRes = 0.0;
CvScalar ImageProcessing::white;

int ImageProcessing::noOfEndPoints = 0;
bool ImageProcessing::isCrpPossible = false;
CvRect ImageProcessing::ROI;
IplImage * ImageProcessing::tipCrpd = NULL;
IplImage * ImageProcessing::tipCrpd_Prev = NULL;
IplImage * ImageProcessing::tipCrpd_Diff = NULL;
double ImageProcessing::func_val[5] = { 0.0, };
int ImageProcessing::tipBend_XYIndex = 0;
double ImageProcessing::TipBend_X[32] = { 0.0, };
double ImageProcessing::TipBend_Y[32] = { 0.0, };
double ImageProcessing::recordedSamples[5] = { 0.0, };
bool ImageProcessing::isAngioAcq = false;
int ImageProcessing::nearVFIndex = 0;
double ImageProcessing::ftFieldVect_CompX[NO_OF_FT_FIELDS] = { 0.0, };
double ImageProcessing::ftFieldVect_CompY[NO_OF_FT_FIELDS] = { 0.0, };
double ImageProcessing::ftFieldVectLen[NO_OF_FT_FIELDS] = { 0.0, };
double ImageProcessing::ftFieldVectAngD[NO_OF_FT_FIELDS] = { 0.0, };
double ImageProcessing::ftFieldVectAngR[NO_OF_FT_FIELDS] = { 0.0, };
char ImageProcessing::ftFieldType;
char ImageProcessing::ftfType[NO_OF_FT_FIELDS] = {};
double ImageProcessing::ftfStartPosX[NO_OF_FT_FIELDS] = { 0.0, };
double ImageProcessing::ftfStartPosY[NO_OF_FT_FIELDS] = { 0.0, };
double ImageProcessing::ftfEndPosX[NO_OF_FT_FIELDS] = { 0.0, };
double ImageProcessing::ftfEndPosY[NO_OF_FT_FIELDS] = { 0.0, };
double ImageProcessing::mstrTwist_PrevForIP = 0.0;
int ImageProcessing::ftfCurrentIndex = 0;

double ImageProcessing::ipAng_FrstFilt = 0.0;
double ImageProcessing::ipRes_FrstFilt = 0.0;
double ImageProcessing::ipAng_ScndFilt = 0.0;
double ImageProcessing::ipRes_ScndFilt = 0.0;
#pragma endregion

ImageProcessing::ImageProcessing(QObject *parent)
	: QObject(parent)
{
}

ImageProcessing::~ImageProcessing()
{
	delete oImgTradeAngio;
	delete oImgTradeGWSeg;
	delete oImgTradeXRay;
	cvReleaseCapture(&capture);
	cvReleaseImage(&imgAngio);
	cvReleaseImage(&imgXRay);
	cvReleaseImage(&staticAngio);
	cvReleaseImage(&tipCrpd);
	cvReleaseImage(&tipCrpd_Prev);
	cvReleaseImage(&tipCrpd_Diff);
}

void ImageProcessing::terminateAll(void)
{
	quitImgProcThread = true;
	if (quitImgProcThread)
	{
		_sleep(100);				//Wait until thread loop is finished
		myImgProcThread->join();	//Wait until thread is ended
	}
}

void ImageProcessing::initializeImageProcessing(void)
{
	//Initialize image processing related variables
	white.val[0] = 255;
	capture  = cvCreateCameraCapture(WEBCAM);
	//capture = cvCreateCameraCapture(BUILDIN_CAM);
	bgrImg   = cvQueryFrame(capture);
	imgAngio = cvCreateImage(cvGetSize(bgrImg), 8, 1);
	imgXRay    = cvCreateImage(cvGetSize(bgrImg), 8, 1);
	staticAngio = cvCreateImage(cvGetSize(bgrImg), 8, 1);
	oImgTradeAngio = new ImageProcessingTrade();
	oImgTradeGWSeg = new ImageProcessingTrade();
	oImgTradeCrp = new ImageProcessingTrade();
	oImgTradeXRay = new ImageProcessingTrade();

	ROI     = cvRect(0, 0, ((2 * wndSize) + 1), ((2 * wndSize) + 1));	//Setting dimensions of Region Of Interest ROI
	tipCrpd = cvCreateImage(cvSize(ROI.width, ROI.height), 8, 1);		//Cropped image should of the size of ROI
	tipCrpd_Prev = cvCreateImage(cvSize(ROI.width, ROI.height), 8, 1);	//Previous cropped image
	tipCrpd_Diff = cvCreateImage(cvSize(ROI.width, ROI.height), 8, 1);	//Difference of current and previous cropped image
}

void ImageProcessing::createImgProcThread(void)
{
	quitImgProcThread = false;
	myImgProcThread = new std::thread(&ImageProcessing::imgProThread, this);
}

void ImageProcessing::imgProThread() 
{
	cvNamedWindow("Static Angiogram Image 'staticAngio'");
	cvSetMouseCallback("Static Angiogram Image 'staticAngio'", mouseCallBack, NULL);
	SYSTEMTIME time;
	unsigned long int timeStart = 0;
	unsigned long int timeEnd = 0;
	unsigned long int timeElapsed = 0;
	while (!quitImgProcThread)
	{
		GetSystemTime(&time);
		timeStart = (time.wSecond * 1000) + time.wMilliseconds;

		static float mstrTrans = 0.0;
		static float mstrTwist = 0.0;
		static float mstrTrans_Prev = 0.0;
		static float mstrTwist_Prev = 0.0;
		static float mstrTwist_PrevForDiff = 0.0;

		float * motorPos = SerialCommunicationTrade::getMotorPositions();
		mstrTrans = motorPos[0];
		mstrTwist = motorPos[1];
		delete motorPos;

		//*******************Run Image Processing loop*******************//
		bgrImg = cvQueryFrame(capture);
		cvCvtColor(bgrImg, imgAngio, CV_BGR2GRAY);
		cvCopy(imgAngio, imgXRay);
		//cvShowImage("Live Cam 'imgAngio'", imgAngio);
		
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		//*******************Apply Frangi Filter
		cv::Mat frangiResult_Mat = frangiFilter(imgAngio);
		IplImage * frangiResult = new IplImage(frangiResult_Mat);				//frangiFilter() returns Mat image
		//cvShowImage("Frangi Filtered Image 'frangiResult'", frangiResult);

		/////////////////////////////////////////////////////////////////////////////////////////////////////
		//*******************Setting up staticAngio image
	//	if (!isAngioAcq)	//Show the current images until you select the angiogram image
		{
			cvCopy(imgAngio, staticAngio);
		}
		CvPoint circleCenter;
		CvPoint startPos;
		CvPoint endPos;
		for (int i = 0; i <= (ftfCurrentIndex - 1); i++)
		{
			circleCenter.x = ((int)(ftfStartPosX[i]));
			circleCenter.y = ((int)(ftfStartPosY[i]));

			startPos.x = ftfStartPosX[i];
			startPos.y = ftfStartPosY[i];
			endPos.x = ftfEndPosX[i];
			endPos.y = ftfEndPosY[i];

			if (ftfType[i] == 'A')
			{
				cvLine(staticAngio, startPos, endPos, cvScalar(255, 0, 0, 255), 3, 8, 0);
				cvDrawCircle(staticAngio, circleCenter, 30, cvScalar(255, 0, 0, 255), 1, 8, 0);
			}
			else if (ftfType[i] == 'R')
			{
				cvLine(staticAngio, startPos, endPos, cvScalar(0, 0, 0, 255), 3, 8, 0);
				cvDrawCircle(staticAngio, circleCenter, 30, cvScalar(0, 0, 0, 255), 1, 8, 0);
			}
		}
		cvShowImage("Static Angiogram Image 'staticAngio'", staticAngio);

		/////////////////////////////////////////////////////////////////////////////////////////////////////
		//*******************Setting up the x-ray image
		cvSet(imgXRay, CV_RGB(170, 170, 170));
		for (int xloop = 0; xloop < frangiResult->width; xloop++)
		{
			for (int yloop = 0; yloop < frangiResult->height; yloop++)
			{
				if (uchar(frangiResult->imageData[frangiResult->widthStep * yloop + xloop * frangiResult->nChannels]) > 200)
				{
					imgXRay->imageData[imgXRay->widthStep * yloop + xloop * imgXRay->nChannels] = 8;
				}
			}
		}
		//cvShowImage("X-ray Image 'imgXRay'", imgXRay);
		
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		//*******************Apply Gonzalez Thinning
		cvNormalize(frangiResult, frangiResult, 0.0, 255.0, CV_MINMAX);		//Normalize the values between 0 to 255
		cvThreshold(frangiResult, frangiResult, 45, 255, CV_THRESH_BINARY);	//Apply thresholding
		IplImage * thinningResult = NULL;
		thinningResult = cvCreateImage(cvSize(frangiResult->width, frangiResult->height), 8, 1);	//Create image to store output of ThinningGonzalez 
		IplImage* tmpThinningResult = thinningGonzalez(frangiResult);
		cvCopy(tmpThinningResult, thinningResult);										//Apply thining algorithm
		cvReleaseImage(&tmpThinningResult);
		//cvShowImage("Thinning Result 'thinningResult'", thinningResult);

		/////////////////////////////////////////////////////////////////////////////////////////////////////
		//*******************Guidewire tip detection
		IplImage * cthinningResult = NULL;
		cthinningResult = cvCreateImage(cvSize(thinningResult->width, thinningResult->height), 8, 1);
		cvCopy(thinningResult, cthinningResult);
		gwTipDetection(cthinningResult, 'F');								//It will also mark circles in subroutine
		cvCopy(thinningResult, cthinningResult);							//Remove those circles, as tip already detected
		cvDrawCircle(cthinningResult, gwTipLocFImg, 10, white, 1, 8, 0);
		//cvShowImage("Tip Detection 'cThinningResult'", cthinningResult);

		/////////////////////////////////////////////////////////////////////////////////////////////////////
		//*******************Crop image
		isCrpPossible = true;						//Image is cropped such that guidewire tip is in the center of ROI
		if (double(gwTipLocFImg.x) < 35 || double(gwTipLocFImg.x) > 605)
			isCrpPossible = false;
		if (double(gwTipLocFImg.y) < 35 || double(gwTipLocFImg.y) > 445)
			isCrpPossible = false;
		double width = double(gwTipLocFImg.x) - wndSize;//Setting width of ROI
		double height = double(gwTipLocFImg.y) - wndSize;//Setting height of ROI

		if (isCrpPossible)								//If window size is not zero then crop
		{
			ROI = cvRect(width, height, ((2 * wndSize) + 1), ((2 * wndSize) + 1));
			cvSetImageROI(thinningResult, ROI);
			cvCopy(thinningResult, tipCrpd);
			cvResetImageROI(thinningResult);
			//cvShowImage("Cropped Image", tipCrpd);
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////
		//*******************Acknowledge new image only when there is significant change (in Image and motor twist)
		if (isCrpPossible)
		{
			cvSub(tipCrpd, tipCrpd_Diff, tipCrpd_Diff);
			//cvShowImage("Cropped Image - Difference 'tipCrpd_Diff'", tipCrpd_Diff);
			int pxlDiff = 0;												//Counting the number of white pixels in difference image
			for (int yloop = (tipCrpd_Diff->height - 1); yloop >= 0; yloop--)//Rows
			{
				for (int xloop = (tipCrpd_Diff->width - 1); xloop >= 0; xloop--)//Columns
				{
					if (uchar(tipCrpd_Diff->imageData[tipCrpd_Diff->widthStep * yloop + xloop * tipCrpd_Diff->nChannels]) > 250)
					{
						pxlDiff++;
					}
				}
			}
			bool isRobotMvd = false;
			if (mstrTwist > mstrTwist_Prev)
			{
				if (abs(mstrTwist - mstrTwist_Prev) >= 0.035)
					isRobotMvd = true;
			}
			else if (mstrTwist < mstrTwist_Prev)
			{
				if (abs(mstrTwist_Prev - mstrTwist) >= 0.035)
					isRobotMvd = true;
			}

			if (pxlDiff > 5 && pxlDiff < 50 && isRobotMvd == true)
			{
				//Acknowlege new image, backup the image copy and twist motor position
				cvCopy(tipCrpd, tipCrpd_Diff);
				mstrTwist_PrevForDiff = mstrTwist;
			}
			else
			{
				//CAUTION: DEPENDING UPON SITUATION, COMMENT FOLLOWING INSTRUCTION DURING TROUBLESHOOTING
				//cvCopy(tipCrpd_Prev, tipCrpd);		//Use old image (backup image)
			}
		}
		

		/////////////////////////////////////////////////////////////////////////////////////////////////////
		//*******************Search the cropped image from the borders, then rotate the image accordingly
		if (isCrpPossible)
		{
			//CAUTION: THIS ALGORITHM MAY NOT WORK UNDER ALL SITUATIONS
//			setImageRotation();		//If this is not working right, probably tip is not in the image
		}
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		//*******************Curve fitting and tangent computation
		double avgSlope = 0.0;
		if (isCrpPossible)
		{
			avgSlope = curveFtng_Tangent();				//This will compute the slope values and draw the tangent on the poly fitted curve
		}
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		//*******************Jittering filteration
		double filtAvgSlope = 0.0;
		if (isCrpPossible)
		{
			filtAvgSlope = jitterFilter(avgSlope);		//This will try to minimize the abrupt change in tangent-slope
			//Compute the bending vector with LPF
			double y1 = (filtAvgSlope * (TANGENTENDY - TipBend_X[tipBend_XYIndex - 1])) + func_val[0]; 		//Equation of tangent line //25
			double y2 = (filtAvgSlope * (TANGENTSTARTY - TipBend_X[tipBend_XYIndex - 1])) + func_val[0];		//35

			CvPoint lpfTangStartPos;
			CvPoint lpfTangEndPos;

			lpfTangStartPos.y = TANGENTSTARTY;
			lpfTangStartPos.x = TANGENT_X - (int)y2;
			lpfTangEndPos.y = TANGENTENDY;
			lpfTangEndPos.x = TANGENT_X - (int)y1;

			double lpfTipBendVect_CompX = lpfTangEndPos.x - lpfTangStartPos.x;
			double lpfTipBendVect_CompY = -1.0*(lpfTangEndPos.y - lpfTangStartPos.y);
			double lpfTipBendVectLen = sqrtf((lpfTipBendVect_CompX * lpfTipBendVect_CompX) + (lpfTipBendVect_CompY * lpfTipBendVect_CompY));
			//Compute the inner product
			double lpf_ipAngle = 0.0;		//theta
			nearVFIndex = HapticRenderingTrade::getftfInsideIndex();
			lpf_ipAngle = ((ftFieldVect_CompX[nearVFIndex] * lpfTipBendVect_CompX) + (ftFieldVect_CompY[nearVFIndex] * lpfTipBendVect_CompY)) / (ftFieldVectLen[nearVFIndex] * lpfTipBendVectLen);
			lpf_ipAngle = acosf(((ftFieldVect_CompX[nearVFIndex] * lpfTipBendVect_CompX) + (ftFieldVect_CompY[nearVFIndex] * lpfTipBendVect_CompY)) / (ftFieldVectLen[nearVFIndex] * lpfTipBendVectLen));
			lpf_ipAngle = lpf_ipAngle * (180.0 / 3.1416);
			if (lpf_ipAngle < 0)
				lpf_ipAngle = 360.0 + lpf_ipAngle;

			float ipRes = cosf(((float(int(lpf_ipAngle)))) * (3.14159 / 180.0));
			int ipRes_2Deci = (int)(ipRes * 100.0);
			double ipResult = ((float)ipRes_2Deci) / 100.0;

//			if (abs(mstrTwist - mstrTrans_Prev) >= 0.006)
			{
				ipRes_FrstFilt = ipResult;
				ipAng_FrstFilt = acosf((float)ipRes_FrstFilt);
				mstrTwist_PrevForIP = mstrTwist;
			}

			jitterFilter();	//Operates on inner product result
			innerProdRes = ipRes_ScndFilt;
			innerProdAng = ipAng_ScndFilt;


		}

		mstrTrans_Prev = mstrTrans;
		mstrTwist_Prev = mstrTwist;

		oImgTradeAngio->writeImage(imgAngio);
		oImgTradeGWSeg->writeImage(cthinningResult);
		oImgTradeXRay->writeImage(imgXRay);
		cvReleaseImage(&thinningResult);
		cvReleaseImage(&cthinningResult);
		ImageProcessingTrade::setUpdateStatus(true);
		ImageProcessingTrade::setIPAngVal(innerProdAng);
		ImageProcessingTrade::setIPResVal(innerProdRes);
		ImageProcessingTrade::setGWPos(double(gwTipLocFImg.x), double(gwTipLocFImg.y));
		cvWaitKey(1);
		GetSystemTime(&time);
		timeEnd = (time.wSecond * 1000) + time.wMilliseconds;
		timeElapsed = timeEnd - timeStart;

		static int cnt = 0;
		const int NumOfSamples = 10;
		static unsigned long int timeBuf[NumOfSamples] = { 0, };
		timeBuf[cnt] = timeElapsed;
		cnt++;
		if (cnt == NumOfSamples)
		{
			for (int loop = 0; loop < NumOfSamples; loop++)
				timeElapsed += timeBuf[loop];
			timeElapsed /= NumOfSamples;
			cnt = 0;
			ImageProcessingTrade::setImgProcRate(1000.0 / ((double)timeElapsed));
		}
		
	}
}

void ImageProcessing::mouseCallBack(int event, int x, int y, int flags, void* userdata)
{
	static IplImage * cStaticAngio = cvCreateImage(cvGetSize(staticAngio), 8, 1);
	static bool isLButtonDown = false;
	static CvPoint ftfStartPos;
	static CvPoint ftfEndPos;
	static CvPoint ftfMidPos;
	static bool acquiringAngio = false;
	
	if (event == CV_EVENT_LBUTTONDOWN)
	{
		if (!isAngioAcq)
		{
			isAngioAcq = true;
			acquiringAngio = true;
		}
		else
		{
			ftfStartPos.x = x;
			ftfStartPos.y = y;
			isLButtonDown = true;
		}
	}
	else if (event == CV_EVENT_MOUSEMOVE)
	{
		if (isLButtonDown)
		{
			cvCopy(staticAngio, cStaticAngio);
			ftfMidPos.x = x;
			ftfMidPos.y = y;
			cvLine(staticAngio, ftfStartPos, ftfMidPos, cvScalar(255, 0, 0, 255), 3, 8, 0);
			cvShowImage("Static Angiogram Image 'staticAngio'", staticAngio);
			cvCopy(cStaticAngio, staticAngio);
		}
	}
	else if (event == CV_EVENT_LBUTTONUP)
	{
		isLButtonDown = false;
		if (acquiringAngio)
		{
			acquiringAngio = false;
		}
		else if(!acquiringAngio)
		{
			ftfEndPos.x = x;
			ftfEndPos.y = y;

			ftFieldVect_CompX[ftfCurrentIndex] = ftfEndPos.x - ftfStartPos.x;
			ftFieldVect_CompY[ftfCurrentIndex] = -1.0 * (ftfEndPos.y - ftfStartPos.y);
			ftFieldVectLen[ftfCurrentIndex] = sqrtf((float)(ftFieldVect_CompX[ftfCurrentIndex] * ftFieldVect_CompX[ftfCurrentIndex]) + (float)(ftFieldVect_CompY[ftfCurrentIndex] * ftFieldVect_CompY[ftfCurrentIndex]));
			ftFieldVectAngD[ftfCurrentIndex] = atan2(ftFieldVect_CompY[ftfCurrentIndex], ftFieldVect_CompX[ftfCurrentIndex]) * (180.0 / 3.1416);
			if (ftFieldVectAngD[ftfCurrentIndex] < 0.0)
				ftFieldVectAngD[ftfCurrentIndex] = 360.0 + ftFieldVectAngD[ftfCurrentIndex];
			ftFieldVectAngR[ftfCurrentIndex] = atan2(ftFieldVect_CompY[ftfCurrentIndex], ftFieldVect_CompX[ftfCurrentIndex]);
			if (ftFieldVectAngR[ftfCurrentIndex] < 0.0)
				ftFieldVectAngR[ftfCurrentIndex] = 360.0 + ftFieldVectAngR[ftfCurrentIndex];

			char keyboardButton;
			keyboardButton = cvWaitKey();
			if (keyboardButton == 'A' || keyboardButton == 'a')
			{
				ftFieldType = 'A';
				cvLine(staticAngio, ftfStartPos, ftfEndPos, cvScalar(255, 0, 0, 255), 3, 8, 0);
				cvDrawCircle(staticAngio, ftfStartPos, 30, cvScalar(255, 0, 0, 255), 1, 8, 0);
				ftfStartPosX[ftfCurrentIndex] = ftfStartPos.x;
				ftfStartPosY[ftfCurrentIndex] = ftfStartPos.y;
				ftfEndPosX[ftfCurrentIndex] = ftfEndPos.x;
				ftfEndPosY[ftfCurrentIndex] = ftfEndPos.y;
				ftfType[ftfCurrentIndex] = 'A';
			}
			else if (keyboardButton == 'R' || keyboardButton == 'r')
			{
				ftFieldType = 'R';
				cvLine(staticAngio, ftfStartPos, ftfEndPos, cvScalar(0, 0, 0, 255), 3, 8, 0);
				cvDrawCircle(staticAngio, ftfStartPos, 30, cvScalar(0, 0, 0, 255), 1, 8, 0);
				ftfStartPosX[ftfCurrentIndex] = ftfStartPos.x;
				ftfStartPosY[ftfCurrentIndex] = ftfStartPos.y;
				ftfEndPosX[ftfCurrentIndex] = ftfEndPos.x;
				ftfEndPosY[ftfCurrentIndex] = ftfEndPos.y;
				ftfType[ftfCurrentIndex] = 'R';
			}
			else if (keyboardButton == 'D' || keyboardButton == 'd')
			{
				ImageProcessingTrade::setFTFStrtPosProfile_X(ftfStartPosX);
				ImageProcessingTrade::setFTFStrtPosProfile_Y(ftfStartPosY);
				ImageProcessingTrade::setFTFTypeProfile(ftfType);
				ImageProcessingTrade::setFTFAngleProfile(ftFieldVectAngD);
				ImageProcessingTrade::setFTDrawStatus(true);
			}
			if (!ImageProcessingTrade::getFTDrawStatus())
			{
				ImageProcessingTrade::setFTFMaxIndex(ftfCurrentIndex);
				ImageProcessingTrade::setFTFAngle(ftFieldVectAngD[ftfCurrentIndex]);
				ImageProcessingTrade::setFTFType(ftFieldType);
			}
			cvShowImage("Static Angiogram Image 'staticAngio'", staticAngio);
			ftfCurrentIndex++;
		}
	}
}

cv::Mat ImageProcessing::frangiFilter(IplImage * frangiInput)
{
	frangi2d_opts_t opts;
	frangi2d_createopts(&opts);
	cv::Mat input_img = cv::cvarrToMat(frangiInput);	//Convert IplImage* to Mat
	cv::Mat input_img_fl;								//Make a new Mat for frangi Input
	input_img.convertTo(input_img_fl, CV_32FC1);		//Convert it to 32bit depth
	cv::Mat vesselness, scale, angles;					//Create new Mat for frangi arguments
	frangi2d(input_img_fl, vesselness, scale, angles, opts);	//Apply farngi filter for segmentation			
	vesselness = vesselness * 255 * 255;				//frangi filter Output image in vesselness image
	vesselness.convertTo(vesselness, CV_8UC1);
	input_img_fl.release();
	input_img.release();
	return vesselness;
}

IplImage * ImageProcessing::thinningGonzalez(IplImage * inImg)
{
	IplImage * outImg = cvCreateImage(cvGetSize(inImg), inImg->depth, inImg->nChannels);
	int width = cvGetSize(inImg).width;
	int height = cvGetSize(inImg).height;

	IplImage* tmpImg = cvCreateImage(cvSize(width, height), 8, 1);

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			PIXEL_YX(outImg, y, x) = PIXEL_YX(inImg, y, x) ? 255 : 0;
		}
	}

	bool remain = true;
	for (int count = 0; remain; count++) {
		cvSetZero(tmpImg);

		remain = false;
		int check = count % 2;

		for (int y = 1, ny = height - 1; y<ny; y++) {
			for (int x = 1, nx = width - 1; x<nx; x++) {
				unsigned char &out_00 = PIXEL_YX(outImg, y, x);

				if (out_00) {
					unsigned char &out_0p = PIXEL_YX(outImg, y, x + 1);
					unsigned char &out_0m = PIXEL_YX(outImg, y, x - 1);
					unsigned char &out_m0 = PIXEL_YX(outImg, y - 1, x);
					unsigned char &out_mm = PIXEL_YX(outImg, y - 1, x - 1);
					unsigned char &out_mp = PIXEL_YX(outImg, y - 1, x + 1);
					unsigned char &out_p0 = PIXEL_YX(outImg, y + 1, x);
					unsigned char &out_pm = PIXEL_YX(outImg, y + 1, x - 1);
					unsigned char &out_pp = PIXEL_YX(outImg, y + 1, x + 1);

					unsigned char &tmp_00 = PIXEL_YX(tmpImg, y, x);

					bool cc = (check == 0) ? out_m0 && out_0p && out_p0 : out_m0 && out_0p && out_0m;
					if (cc) {
						tmp_00 = 255;
						continue;
					}

					bool cd = (check == 0) ? out_0p && out_p0 && out_0m : out_m0 && out_p0 && out_0m;
					if (cd) {
						tmp_00 = 255;
						continue;
					}

					int np1 = out_mm + out_m0 + out_mp + out_0m + out_0p + out_pm + out_p0 + out_0p;
					if (np1 < 2 * 255 || np1 > 6 * 255) {
						tmp_00 = 255;
						continue;
					}

					int sp1 = 0;
					if (!out_m0 && out_mp) sp1++;
					if (!out_mp && out_0p) sp1++;
					if (!out_0p && out_pp) sp1++;
					if (!out_pp && out_p0) sp1++;
					if (!out_p0 && out_pm) sp1++;
					if (!out_pm && out_0m) sp1++;
					if (!out_0m && out_mm) sp1++;
					if (!out_mm && out_m0) sp1++;

					if (sp1 != 1) {
						tmp_00 = 255;
						continue;
					}

					remain = true;
				}
			}
		}
		cvCopy(tmpImg, outImg);
	}

	for (int y = 1, ny = height - 1; y<ny; y++) {
		for (int x = 1, nx = width - 1; x<nx; x++) {
			unsigned char &out_00 = PIXEL_YX(outImg, y, x);
			unsigned char &out_0p = PIXEL_YX(outImg, y, x + 1);
			unsigned char &out_0m = PIXEL_YX(outImg, y, x - 1);
			unsigned char &out_m0 = PIXEL_YX(outImg, y - 1, x);
			unsigned char &out_p0 = PIXEL_YX(outImg, y + 1, x);

			int hv = 0;
			if (out_00) {
				if (out_m0 && out_0p) hv++;
				if (out_0p && out_p0) hv++;
				if (out_p0 && out_0m) hv++;
				if (out_0m && out_m0) hv++;

				if (hv == 1) {
					out_00 = 0;
				}
			}
		}
	}
	cvReleaseImage(&tmpImg);
	return(outImg);
}

void ImageProcessing::gwTipDetection(IplImage * inImg, char imgSize)
{
	int count;					// Declare variable to count neighbourhood pixels
	uchar pix;					// To store a pixel intensity
	std::vector<int> coords;	// To store the ending co-ordinates
	noOfEndPoints = 0;
								// For each pixel in our image...
	for (int i = 1; i < inImg->height - 1; i++)
	{
		for (int j = 1; j < inImg->width - 1; j++)
		{
			pix = CV_IMAGE_ELEM(inImg, unsigned char, i, j);

			if (pix == 0)		// If not a skeleton point, skip
				continue;
			count = 0;			// Reset counter  

								// For each pixel in the neighbourhood centered at this skeleton location...
			for (int y = -1; y <= 1; y++)
			{
				for (int x = -1; x <= 1; x++)
				{
					pix = CV_IMAGE_ELEM(inImg, unsigned char, i + y, j + x);
					if (pix != 0)
						count++;
				}
			}

			if (count == 2)		// If count is exactly 2, add co-ordinates to vector
			{
				coords.push_back(i);
				coords.push_back(j);
				noOfEndPoints++;
			}
		}
	}
	/*TO DO: Specify where is guidewire tip here*/
	if (imgSize == 'F')
	{
		CvPoint CenterLoc;
		for (int i = 0; i < coords.size() / 2; i++)
		{
			CenterLoc.y = coords.at(2 * i);
			CenterLoc.x = coords.at(2 * i + 1);
			cvDrawCircle(inImg, CenterLoc, 10, white, 1, 8, 0);
			gwTipLocFImg.y = coords.at(0);
			gwTipLocFImg.x = coords.at(1);
		}
		//cvShowImage("All Detected Ends", inImg);
	}
	else if (imgSize == 'C')
	{
		CvPoint CenterLoc;
		for (int i = 0; i < coords.size() / 2; i++)
		{
			CenterLoc.y = coords.at(2 * i);
			CenterLoc.x = coords.at(2 * i + 1);
			cvDrawCircle(inImg, CenterLoc, 10, white, 1, 8, 0);
			gwTipLocCImg.y = coords.at(0);
			gwTipLocCImg.x = coords.at(1);
		}
		//cvShowImage("All Detected Ends", inImg);
	}
	/*TO DO: Add tip detection code here*/
	
}

void ImageProcessing::setImageRotation(void)
{
	//Search the cropped image from the borders, then rotate the image accordingly
	int rowSearch = 0;
	int colSearch = 0;
	int tipQuadrant = 0;
	//First Search the Bottom Border: Row = Last Row, All columns
	if (tipQuadrant == 0)
	{
		rowSearch = tipCrpd->height - 1;
		for (int xloop = 0; xloop <= (tipCrpd->width - 1); xloop++)//Columns
		{
			if (uchar(tipCrpd->imageData[tipCrpd->widthStep * rowSearch + xloop * tipCrpd->nChannels]) > 200)
			{
				tipQuadrant = 1;
				break;
			}
		}
	}
	//Second: Search the Right Border: Column = Last Column, All rows
	if (tipQuadrant == 0)
	{
		colSearch = tipCrpd->width - 1;
		for (int yloop = 0; yloop <= (tipCrpd->height - 1); yloop++)//Rows
		{
			if (uchar(tipCrpd->imageData[tipCrpd->widthStep * yloop + colSearch * tipCrpd->nChannels]) > 200)
			{
				tipQuadrant = 2;
				break;
			}
		}
	}
	//Third: Search the Top Border: Row = 0, All columns
	if (tipQuadrant == 0)
	{
		rowSearch = 0;
		for (int xloop = 0; xloop <= (tipCrpd->width - 1); xloop++)//Columns
		{
			if (uchar(tipCrpd->imageData[tipCrpd->widthStep * rowSearch + xloop * tipCrpd->nChannels]) > 200)
			{
				tipQuadrant = 3;
				break;
			}
		}
	}
	//Fourth: Search the Left Border: Column = 0, All rows
	if (tipQuadrant == 0)
	{
		colSearch = 0;
		for (int yloop = 0; yloop <= (tipCrpd->height - 1); yloop++)//Rows
		{
			if (uchar(tipCrpd->imageData[tipCrpd->widthStep * yloop + colSearch * tipCrpd->nChannels]) > 200)
			{
				tipQuadrant = 4;
				break;
			}
		}
	}
	IplImage * setRot_tipCrpd = NULL;
	setRot_tipCrpd = cvCreateImage(cvGetSize(tipCrpd), 8, 1);
	cvCopy(tipCrpd, setRot_tipCrpd);
	if (tipQuadrant == 1)
	{
		//Do nothing
	}
	else if (tipQuadrant == 2)
	{
		cvTranspose(setRot_tipCrpd, setRot_tipCrpd);
		cvFlip(setRot_tipCrpd, setRot_tipCrpd, 1);
	}
	else if (tipQuadrant == 3)
	{
		cvFlip(setRot_tipCrpd, setRot_tipCrpd, 0);
	}
	else if (tipQuadrant == 4)
	{
		cvTranspose(setRot_tipCrpd, setRot_tipCrpd);
		cvFlip(setRot_tipCrpd, setRot_tipCrpd, 0);
	}
	//cvShowImage("Manipulated Image 'setRot_tipCrpd'", setRot_tipCrpd);
	cvCopy(setRot_tipCrpd, tipCrpd);
	cvReleaseImage(&setRot_tipCrpd);
}

double ImageProcessing::curveFtng_Tangent(void)
{
	//*******************Storing the coordinates of white pixels in the cropped image
	for (int loop = 0; loop <= 32; loop++)							//Resetting the variables
	{
		TipBend_X[loop] = 0.0;
		TipBend_Y[loop] = 0.0;
	}
	tipBend_XYIndex = 0;
	for (int yloop = (tipCrpd->height - 1); yloop > 0; yloop--)		//Rows
	{
		for (int xloop = 0; xloop < (tipCrpd->width - 1); xloop++)	//Columns
		{
			TipBend_Y[tipBend_XYIndex] = yloop;
			if (uchar(tipCrpd->imageData[tipCrpd->widthStep * yloop + xloop * tipCrpd->nChannels]) > 250)
			{
				TipBend_X[tipBend_XYIndex] = xloop;
				tipBend_XYIndex++;
				break;
			}
		}
	}
	//*******************Invert the y-coordinate (Make the image upside down)
	for (int iLOOP = 0; iLOOP <= (tipBend_XYIndex - 1); iLOOP++)
	{
		TipBend_Y[iLOOP] = (tipCrpd->height - 1) - TipBend_Y[iLOOP]; //Just for inverting
	}
	
	//*******************Finding the tip location in cropped image
	//As rotation is already adjusted so, you may also use row and column searching algorithm!
	IplImage * ctipCrpd = NULL;
	ctipCrpd = cvCreateImage(cvSize(tipCrpd->width, tipCrpd->height), 8, 1);
	cvCopy(tipCrpd, ctipCrpd);
	gwTipDetection(ctipCrpd, 'C');									//It will also mark circles in subroutine
	cvCopy(tipCrpd, ctipCrpd);										//Remove those circles, as tip already detected
	cvDrawCircle(ctipCrpd, gwTipLocCImg, 10, white, 1, 8, 0);
	//cvShowImage("Tip in cropped image 'ctipCrpd'", ctipCrpd);
	
	
	//*******************Performing the curve fitting
	double polyFitCoefficient[3] = { 0.0, };
	double func_val[5] = { 0.0, };
	double d_func_val[5] = { 0.0, };
	double d_func_valavg = 0.0; 
	polynomialfit(32, 3, TipBend_Y, TipBend_X, polyFitCoefficient);			//polynomialfit from GSL Scientific Library
				
	//*******************Getting derivative of 2nd order (quadratic) polynomial			
	func_val[0] = (polyFitCoefficient[2] * (TipBend_X[tipBend_XYIndex - 1])*(TipBend_X[tipBend_XYIndex - 1])) + (polyFitCoefficient[1] * (TipBend_X[tipBend_XYIndex - 1])) + (polyFitCoefficient[0]); //func value at max value of x
	d_func_val[0] = (2.0*polyFitCoefficient[2] * (TipBend_X[tipBend_XYIndex - 1])) + (polyFitCoefficient[1]); //derivative of func at max value of X
	func_val[1] = (polyFitCoefficient[2] * (TipBend_X[tipBend_XYIndex - 3])*(TipBend_X[tipBend_XYIndex - 3])) + (polyFitCoefficient[1] * (TipBend_X[tipBend_XYIndex - 3])) + (polyFitCoefficient[0]); //func value at max value of x
	d_func_val[1] = (2.0*polyFitCoefficient[2] * (TipBend_X[tipBend_XYIndex - 3])) + (polyFitCoefficient[1]); //derivative of func at max value of X
	func_val[2] = (polyFitCoefficient[2] * (TipBend_X[tipBend_XYIndex - 5])*(TipBend_X[tipBend_XYIndex - 5])) + (polyFitCoefficient[1] * (TipBend_X[tipBend_XYIndex - 5])) + (polyFitCoefficient[0]); //func value at max value of x
	d_func_val[2] = (2.0*polyFitCoefficient[2] * (TipBend_X[tipBend_XYIndex - 5])) + (polyFitCoefficient[1]); //derivative of func at max value of X
	func_val[3] = (polyFitCoefficient[2] * (TipBend_X[tipBend_XYIndex - 7])*(TipBend_X[tipBend_XYIndex - 7])) + (polyFitCoefficient[1] * (TipBend_X[tipBend_XYIndex - 7])) + (polyFitCoefficient[0]); //func value at max value of x
	d_func_val[3] = (2.0*polyFitCoefficient[2] * (TipBend_X[tipBend_XYIndex - 7])) + (polyFitCoefficient[1]); //derivative of func at max value of X
	func_val[4] = (polyFitCoefficient[2] * (TipBend_X[tipBend_XYIndex - 9])*(TipBend_X[tipBend_XYIndex - 9])) + (polyFitCoefficient[1] * (TipBend_X[tipBend_XYIndex - 9])) + (polyFitCoefficient[0]); //func value at max value of x
	d_func_val[4] = (2.0*polyFitCoefficient[2] * (TipBend_X[tipBend_XYIndex - 9])) + (polyFitCoefficient[1]); //derivative of func at max value of X

	//*******************Calculate function value at different locations and take average of tangent line slopes
	d_func_valavg = (d_func_val[0] + d_func_val[1] + d_func_val[2] + d_func_val[3] + d_func_val[4]) / 5.0;
	d_func_valavg = (2.0*polyFitCoefficient[2] * (TipBend_X[tipBend_XYIndex - 1])) + (polyFitCoefficient[1]); //derivative of func at max value of X
	
	//*******************Creating an image to visualize the curve fitting results
	IplImage * crvFitResult = NULL;
	crvFitResult = cvCreateImage(cvSize(tipCrpd->width, tipCrpd->height), 8, 1);
	cvSet(crvFitResult, CV_RGB(0, 0, 0));

	double xPolyFuncVal[32] = { 0.0, };
	double yPolyFuncVal[32] = { 0.0, };
	int xTipBend = gwTipLocCImg.x;
	int yTipBend = gwTipLocCImg.y;
	for (int loop = 0; loop <= 31; loop++)
	{
		xPolyFuncVal[loop] = (polyFitCoefficient[2] * (loop)*(loop)) + (polyFitCoefficient[1] * (loop)) + (polyFitCoefficient[0]); //func value at x
		xTipBend = loop;
		yTipBend = 61 - xPolyFuncVal[loop];
		crvFitResult->imageData[crvFitResult->widthStep * (61 - xTipBend) + (61 - yTipBend)] = 255;
	}
	//cvShowImage("Curve Fitting Results 'CrvFitResult'", crvFitResult);		//Draw tangent on the curve fit result

	//*******************Now computing the tangent line (tip bend vector)
	//First point of tangent line
	double y1 = (d_func_valavg * (TANGENTENDY - TipBend_X[tipBend_XYIndex - 1])) + func_val[0]; 		//Equation of tangent line 
	double y2 = (d_func_valavg * (TANGENTSTARTY - TipBend_X[tipBend_XYIndex - 1])) + func_val[0];

	CvPoint tangStartPos;
	CvPoint tangEndPos;

	tangStartPos.y = TANGENTSTARTY;
	tangStartPos.x = TANGENT_X - int(y2);
	tangEndPos.y = TANGENTENDY;
	tangEndPos.x = TANGENT_X - int(y1);

	//Draw the tangent on the curFitResult image
	cvLine(crvFitResult, tangStartPos, tangEndPos, white);
	cvLine(ctipCrpd, tangStartPos, tangEndPos, white);
	//cvShowImage("Curve Fitting Results 'crvFitResult'", crvFitResult);
	//cvShowImage("Tip Detection and Tangent 'ctipCrpd'", ctipCrpd);
	oImgTradeCrp->writeImage(ctipCrpd);
	cvReleaseImage(&crvFitResult);
	cvReleaseImage(&ctipCrpd);
	//return d_func_valavg;
	//*******************Tangent length and angle
	double tipBendVect_CompX = tangEndPos.x - tangStartPos.x;
	double tipBendVect_CompY = -1.0*(tangEndPos.y - tangStartPos.y);

	double tipBendVectLen = sqrtf((tipBendVect_CompX * tipBendVect_CompX) + (tipBendVect_CompY * tipBendVect_CompY));
	double tipBendVectAngR = atan2(tipBendVect_CompY, tipBendVect_CompX);
	if (tipBendVectAngR < 0.0)
	{
		tipBendVectAngR = (2 * 3.1416) + tipBendVectAngR;
	}
	double tipBendVectAngD = (atan2(tipBendVect_CompY, tipBendVect_CompX)) * (180.0 / 3.1416);
	if (tipBendVectAngD < 0.0)
		tipBendVectAngD = 360 + tipBendVectAngD;

	ImageProcessingTrade::setTipBendAng(tipBendVectAngD);

	return d_func_valavg;
}

bool ImageProcessing::polynomialfit(int obs, int degree, double * dx, double * dy, double * store)
{
	gsl_multifit_linear_workspace *ws;
	gsl_matrix *cov, *X;
	gsl_vector *y, *c;
	double chisq;

	int i, j;

	X = gsl_matrix_alloc(obs, degree);
	y = gsl_vector_alloc(obs);
	c = gsl_vector_alloc(degree);
	cov = gsl_matrix_alloc(degree, degree);

	for (i = 0; i < obs; i++) {
		for (j = 0; j < degree; j++) {
			gsl_matrix_set(X, i, j, pow(dx[i], j));
		}
		gsl_vector_set(y, i, dy[i]);
	}

	ws = gsl_multifit_linear_alloc(obs, degree);
	gsl_multifit_linear(X, y, c, cov, &chisq, ws);


	for (i = 0; i < degree; i++)
	{
		store[i] = gsl_vector_get(c, i);
	}

	gsl_multifit_linear_free(ws);
	gsl_matrix_free(X);
	gsl_matrix_free(cov);
	gsl_vector_free(y);
	gsl_vector_free(c);

	return true; /* we do not "analyse" the result (cov matrix mainly)
				 to know if the fit is "good" */
}

double ImageProcessing::jitterFilter(double d_func_valavg)
{
	//For FIR - Low Pass Filter
	float RC = 1.0 / (3 * 2 * 3.14);	//RC = 1.0/(CUTOFF*2*3.14);
	float dt = 1.0 / 12.5;				//dt = 1.0/SAMPLE_RATE;
	float alpha = dt / (RC + dt);		//alpha = dt / (RC + dt);
	const int sampleSize = 5;
	float filteredArray[sampleSize];
	//With sample size <5 it is too noisy
	alpha = 0.2; //noisy
	alpha = 0.8; //fast but noisy
	alpha = 0.4; //seems reasonable
	alpha = 0.5; //noisy but fast
	alpha = 0.45; //will be used further for filtering
	alpha = 0.000826; //very small weightage to current sample

	recordedSamples[0] = d_func_valavg;
	filteredArray[0] = recordedSamples[0];
	for (int i = 1; i < sampleSize; i++)
	{
		filteredArray[i] = filteredArray[i - 1] + (alpha*(recordedSamples[i] - filteredArray[i - 1]));
	}
	for (int i = 0; i < sampleSize; i++)
	{
		recordedSamples[i] = filteredArray[i];
	}
	//recordedSamples[4] = d_func_valavg;	//Uncomment to bypass the filteration
	return recordedSamples[4];
}

void ImageProcessing::jitterFilter(void)
{
	static bool isInitialValAcq = false;
	static int initialValCnt = 0;
	static float InnerProductPrePreVal = 0.0;
	static float InnerProductPreVal = 0.0;
	static float InnerProductCurrVal = 0.0;
	static float InnerProductAvgPre = 0.0;
	static float InnerProductAvgCurr = 0.0;
	static float fInnerProductResult = 0.0;
	static float fInnerProductAngle = 0.0;
	static float fInnerProductAnglePrev = 0.0;
	static float fInnerProductResultPrev = 0.0;

	float fInnerProductAvgResult = ipRes_FrstFilt;
	if (!isInitialValAcq)
	{
		if (initialValCnt == 0)
		{
			InnerProductPrePreVal = fInnerProductAvgResult;
		}
		if (initialValCnt == 1)
		{
			InnerProductAvgPre = fInnerProductAvgResult;
			InnerProductPreVal = fInnerProductAvgResult;
			isInitialValAcq = true;
		}
		initialValCnt++;
	}
	if (isInitialValAcq)
	{
		InnerProductCurrVal = fInnerProductAvgResult;
		if (InnerProductPreVal == InnerProductAvgPre)
		{
			InnerProductAvgCurr = InnerProductAvgPre;
		}
		else if (InnerProductPreVal > InnerProductAvgPre)
		{
			if (InnerProductCurrVal >= InnerProductPreVal)
				InnerProductAvgCurr = InnerProductPreVal;
			else
				InnerProductAvgCurr = (InnerProductCurrVal + InnerProductAvgPre) / 2.0;
		}
		else if (InnerProductPreVal < InnerProductAvgPre)
		{
			if (InnerProductCurrVal <= InnerProductPreVal)
				InnerProductAvgCurr = InnerProductPreVal;
			else
				InnerProductAvgCurr = (InnerProductCurrVal + InnerProductAvgPre) / 2.0;
		}
		InnerProductPreVal = InnerProductCurrVal;
		InnerProductAvgPre = InnerProductAvgCurr;

		fInnerProductResult = InnerProductAvgPre;							//This contains the innerproduct value //With new algorithm it is Pre
		fInnerProductAngle = (acosf(InnerProductAvgPre)) * (180 / 3.1416);	//This contains the innerproduct angle in degrees


		if (fInnerProductAngle >= fInnerProductAnglePrev)
		{
			if (abs(fInnerProductAngle - fInnerProductAnglePrev) >= 2.0)// && abs(fInnerProductAngle - fInnerProductAnglePrev) <= 10.0)
			{
				fInnerProductAnglePrev = fInnerProductAngle;
				fInnerProductResultPrev = fInnerProductResult;
			}
			else
			{
				fInnerProductAngle = fInnerProductAnglePrev;
				fInnerProductResult = fInnerProductResultPrev;
			}
		}
		else if (fInnerProductAnglePrev > fInnerProductAngle)
		{
			if (abs(fInnerProductAnglePrev - fInnerProductAngle) >= 2.0)// && abs(fInnerProductAnglePrev - fInnerProductAngle) <= 10.0)
			{
				fInnerProductAnglePrev = fInnerProductAngle;
				fInnerProductResultPrev = fInnerProductResult;
			}
			else
			{
				fInnerProductAngle = fInnerProductAnglePrev;
				fInnerProductResult = fInnerProductResultPrev;
			}
		}

		ipAng_ScndFilt = fInnerProductAngle;
		ipRes_ScndFilt = fInnerProductResult;
	}
}