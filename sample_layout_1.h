#ifndef SAMPLE_LAYOUT_1_H
#define SAMPLE_LAYOUT_1_H

#include <QtWidgets/QMainWindow>
#include "ui_sample_layout_1.h"
#include "empty_window.h"
#include "mydialog.h"
#include "HSV_Selector.h"


#include <QTableWidget>

#include <QDialog>
#include <QtCore>
#include <QtGui>
#include "analogdial.h"
#include "hand_status.h"
#include "test_vid.h"

// START RS
#include <windows.h>
#include <iostream>
#include <vector>


#include "pxcsensemanager.h"
#include "pxcmetadata.h"
#include "service/pxcsessionservice.h"

#include "pxchanddata.h"
#include "pxchandconfiguration.h"

#include "handData_Definitions.h"
#include "pxccapture.h"

#include "pxcprojection.h"

#include "utilities\pxcsmoother.h"
//END Realsense

////



#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/opencv.hpp"
#include "cvBlob\cvBlob.h"

////

#include <algorithm>
#include <vector>

//custome
#include "realsense_opencv_tools.h"
#include "myDocument.h"

//

class sample_layout_1 : public QMainWindow
{
	Q_OBJECT

public:
	sample_layout_1(QWidget *parent = 0);
	~sample_layout_1();

	// START RS
	bool g_stop = false; // user closes application
	std::wstring g_sequencePath;
	PXCSession *g_session;
	PXCSenseManager *g_senseManager;
	PXCHandModule *g_handModule;
	PXCHandData *g_handDataOutput;
	PXCHandConfiguration *g_handConfiguration;

	// END RS

private:
	Ui::sample_layout_1Class ui;

	bool updateColors_1;
	bool updateColors_2;
	PXCPointF32 colorPoint;

	myDialog *mydialog;
	HSV_Selector *HSV_SelectorInst;

	QGraphicsScene *scene_LeftHand;
	QGraphicsScene *scene_RightHand;
	AnalogDial *leftHand_Flexation;
	AnalogDial *rightHand_Flexation;

	QGraphicsScene *scene_Video_depth;
	test_vid *tv_vid_depth;

	QGraphicsScene *scene_Video_color;
	test_vid *tv_vid_color;

	void sample_layout_1::debugDisplay(QString text);
	void sample_layout_1::calculateAngles(float O, float A, float &theta, float &thetaDegrees);
	void sample_layout_1::printAngles(int label, float O, float A, float theta, float thetaDegrees);
	void sample_layout_1::calculateAnglesAndPrint(int label, float O, float A, float *theta, float thetaDegrees);

	int Hmin, Smin, Vmin, Hmax, Smax, Vmax;

	
	HSV_Range blobHSVrange_1;
	HSV_Range blobHSVrange_2;

	// begin new code
	public slots:

		void HSV_ranger(HSV_Range *ranged_HSV, HSV base_HSV, int width)
		{
			ranged_HSV->min.H = subtract_floor(base_HSV.H, width, 0);
			ranged_HSV->min.S = subtract_floor(base_HSV.S, width, 0);
			ranged_HSV->min.V = subtract_floor(base_HSV.V, width, 0);

			ranged_HSV->max.H = add_ceiling(base_HSV.H, width, 179);
			ranged_HSV->max.S = add_ceiling(base_HSV.S, width, 254);
			ranged_HSV->max.V = add_ceiling(base_HSV.V, width, 254);
		};

		int subtract_floor(int minuend, int subtrahend, int floor)
		{
			int result = minuend - subtrahend;
			if (result < floor)
				result = floor;
			return result;
		};
		int add_ceiling(int a, int b, int ceiling)
		{
			int result = a + b;
			if (result > ceiling)
				result = ceiling;
			return result;
		};

		void mousePressEvent(QMouseEvent *event)
		{

			char str[250];
			//std::sprintf(str, "x=%d\ty=%d (xy)\n x=%d\ty=%d (globalX)\n x=%d\ty=%d (localPos)\n x=%d\ty=%d (pos)\n\n", event->x(), event->y(), event->globalX(), event->globalY(), event->localPos().x(), event->localPos().y(), event->pos().x(), event->pos().y());
			//debugDisplay(str);
			
			// Make the if statement more readable
			QGraphicsView *frame = ui.QGV_Video_color;
			// If click iside of video frame
			if (event->x() > frame->x() && event->pos().x() < frame->x() + frame->width() && event->y() >frame->y() && event->pos().y() < frame->x() + frame->height())
			{
				// flag pixel for color sample
				if (event->button() == Qt::MouseButton::LeftButton)
					updateColors_1 = true;
				else if (event->button() == Qt::MouseButton::RightButton)
					updateColors_2 = true;
				colorPoint.x = event->x() - frame->x();
				colorPoint.y = event->y() - frame->y();
				std::sprintf(str, "x=%f y=%f\n", colorPoint.x, colorPoint.y);
				debugDisplay(str);
			}
			

			
			qApp->processEvents();
		}

		void on_stopCamera_clicked() {
			debugDisplay("The user has selected to stop camera\n");
			g_stop = TRUE;
		}
		
		void releaseAll()
		{
			if (g_senseManager)
			{
				g_senseManager->Close();
				g_senseManager->Release();
				g_senseManager = NULL;
			}
			if (g_session)
			{
				g_session->Release();
				g_session = NULL;
			}
		}

		bool sortbypty(const KeyPoint &lhs, const KeyPoint &rhs)
		{
			return lhs.pt.y < rhs.pt.y;
		}
		
		void on_startCamera_clicked() {

			//cell_detect_status = new QTableWidgetItem;
			QTableWidgetItem xReal;
			QTableWidgetItem yReal;
			QTableWidgetItem zReal;

			QTableWidgetItem xCoord;
			QTableWidgetItem yCoord;
			QTableWidgetItem zCoord;

			QTableWidgetItem twi11;
			QTableWidgetItem twi12;
			QTableWidgetItem twi13;
			QTableWidgetItem twi21;
			QTableWidgetItem twi22;
			QTableWidgetItem twi23;
			QTableWidgetItem twi31;
			QTableWidgetItem twi32;
			QTableWidgetItem twi33;

			ui.table_coords->setItem(0, 0, &xCoord); ui.table_coords->setItem(0, 1, &yCoord); ui.table_coords->setItem(0, 2, &zCoord);
			ui.table_coords->setItem(1, 0, &xReal); ui.table_coords->setItem(1, 1, &yReal); ui.table_coords->setItem(1, 2, &zReal);
			
			ui.table_test1->setItem(0, 0, &twi11); ui.table_test1->setItem(0, 1, &twi12); ui.table_test1->setItem(0, 2, &twi13);
			ui.table_test1->setItem(1, 0, &twi21); ui.table_test1->setItem(1, 1, &twi22); ui.table_test1->setItem(1, 2, &twi23);
			ui.table_test1->setItem(2, 0, &twi31); ui.table_test1->setItem(2, 1, &twi32); ui.table_test1->setItem(2, 2, &twi33);

			int **referenceDepth = new int*[640];
			for (size_t i = 0; i < 640; i++)
				referenceDepth[i] = new int[480];
			bool referenceDepthExists = FALSE;
			updateColors_1 = updateColors_2 = false;

			/*
			Hmin = 32;
			Smin = 116;
			Vmin = 53;

			Hmax = 61;
			Smax = 255;
			Vmax = 218;
			*/
			
			//Light red sticker
			/*
			Hmin = 160;
			Smin = 174;
			Vmin = 101;

			Hmax = 179;
			Smax = 255;
			Vmax = 255;
			*/

			// Bright green paint
			Hmin = 24;
			Smin = 108;
			Vmin = 0;
			Hmax = 96;
			Smax = 197;
			Vmax = 87;
			blobHSVrange_1.max.H = 24;
			blobHSVrange_1.max.S = 108;
			blobHSVrange_1.max.V = 0;

			blobHSVrange_1.min.H = 96;
			blobHSVrange_1.min.S = 197;
			blobHSVrange_1.min.V = 187;

			document myDoc;
			myDoc.arrayOfFramedata.reserve(100);

			/*	Hmin = 76;
				Smin = 116;
				Vmin = 52;

				Hmax = 111;
				Smax = 255;
				Vmax = 195;*/

			/*blobHSVrange.min.H = 32;
			blobHSVrange.min.S = 116;
			blobHSVrange.min.V = 53;
			blobHSVrange.max.H = 61;
			blobHSVrange.max.S = 255;
			blobHSVrange.max.V = 218;*/



			char str[350];
			g_stop = FALSE;

			debugDisplay("button pushed\n");


			//////////////////////////////////////////////////
			bool record = ui.radio_record->isChecked();
			bool recordOrPlayback = (ui.radio_record->isChecked() || ui.radio_playback->isChecked());
			setup_sensM(640, 480, true, true, record);
			//////////////////////////////////////////////////////
			if (recordOrPlayback)
			{
				pxcStatus pxc_status;

				
				QString QS_fileName;
				if (record)
					QS_fileName = ui.te_recordFile->toPlainText();
				else
					QS_fileName = ui.te_paybackFile->toPlainText();
				
				std::string ss_fileName = QS_fileName.toStdString();
				std::wstring ws_fileName = QS_fileName.toStdWString();

				std::sprintf(str, "Opening file %s for %s...", ss_fileName.c_str(), record ? "recording" : "playback");
				debugDisplay(str); qApp->processEvents();

				pxc_status = g_senseManager->QueryCaptureManager()->SetFileName(ws_fileName.c_str(), record);
				if (pxc_status == PXC_STATUS_NO_ERROR)
				{
					debugDisplay("OK.\n");
				}
				else
				{
					std::sprintf(str, "\nFailed to open %s for %s error %d %s", ss_fileName.c_str(), record ? "recording" : "playback", pxc_status, Definitions::pxcStatusToString(pxc_status).c_str());
					debugDisplay(str); qApp->processEvents();
				}
			}
			//////////////////////////////////////////////////////

			g_handConfiguration->EnableAllAlerts();

			// Apply configuration setup
			g_handConfiguration->ApplyChanges();

			hand_status *leftHand_table;
			hand_status *rightHand_table;
			leftHand_table = new hand_status(ui.table_HandStatus_Left);
			rightHand_table = new hand_status(ui.table_HandStatus_Right);

			// First Initializing the sense manager
			debugDisplay("Initializing PXCSenseManager...");
			pxcStatus initStatus = g_senseManager->Init();
			clock_t t_startTime = clock();
			if (initStatus == PXC_STATUS_NO_ERROR)
			{
				int max_frames_to_process = 2000;
				debugDisplay("OK. BEGIN PROCESSING FRAMES!\n");
				std::sprintf(str, "Will auto stop routine after %d frames\n", max_frames_to_process);
				set_current();
				debugDisplay(str);
				qApp->processEvents();
				int frameCounter = 0;
				myDoc.numberOfFrames = 0;

				clock_t t_startTime = clock();

				// Record start time
				myDoc.time = time(0);


				// Create an instance of PXCSmoother
				PXCSmoother* smoother = NULL;
				g_session->CreateImpl<PXCSmoother>(&smoother);

				// Create a 3D smoother, using quadratic algorithm, with smooth strength [0-1]
				PXCSmoother::Smoother3D *smoother3D_A = smoother->Create3DSpring(0.8f);
				PXCSmoother::Smoother3D *smoother3D_B = smoother->Create3DSpring(0.8f);
				PXCSmoother::Smoother3D *smoother3D_C = smoother->Create3DSpring(0.8f);



				// Acquiring frames from input device
				clock_t t_lastFrame = clock();
				
				float accumulatedTime = 0;
				int framesSinceLastCount = 0;
				while (g_senseManager->AcquireFrame(true) == PXC_STATUS_NO_ERROR && !g_stop && frameCounter < max_frames_to_process)
				{
					frameCounter++;
					std::sprintf(str, "Frame: %d", frameCounter);
					ui.frameCount->setText(str);
					// Get current hand outputs
					if (g_handDataOutput->Update() == PXC_STATUS_NO_ERROR)
					{
						bool displayAllFrames = true;
						if (displayAllFrames || frameCounter % 5 == 0)
						{

							//////////////////START READ FRAME DATA//////////////////////////

							// 2D array to hold depth frame
							int **g_depthdata = new int*[640];
							for (size_t i = 0; i < 640; i++)
								g_depthdata[i] = new int[480];

							// 2D array to hold color frame
							int **g_colordata = new int*[640];
							for (size_t i = 0; i < 640; i++)
								g_colordata[i] = new int[480];




							// Get frames

							PXCImage *colorIm, *depthIm;
							PXCImage::ImageData depthData, colorData;
							PXCCapture::Sample *sample;
							
							if (g_senseManager->AcquireFrame(true) < PXC_STATUS_NO_ERROR) break;

							sample = g_senseManager->QuerySample();

							depthIm = sample->depth;
							colorIm = sample->color;

							colorIm->AcquireAccess(PXCImage::ACCESS_READ, PXCImage::PixelFormat::PIXEL_FORMAT_RGB32, &colorData);
							depthIm->AcquireAccess(PXCImage::ACCESS_READ, &depthData);

							// Pack color frame into 2D pixel array
							int *cpixels = (int*)colorData.planes[0];
							int cpitch = colorData.pitches[0] / sizeof(int);
							for (int y = 0; y < (int)480; y++)
							{
								for (int x = 0; x < (int)640; x++)
								{
									int c = cpixels[y*cpitch + x];
									g_colordata[x][y] = c;
								}
							}

							// Pack depth frame into 2D pixel array
							// Depth frame in mm from camera
							pxcU16 *dpixels = (pxcU16*)depthData.planes[0];
							int dpitch = depthData.pitches[0] / sizeof(pxcU16);
							for (int y = 0; y < (int)480; y++)
							{
								for (int x = 0; x < (int)640; x++)
								{
									pxcU16 d = dpixels[y*dpitch + x];
									//if (d == invalidDepth) continue;
									g_depthdata[x][y] = d;
								}
							}




							
							
							//////////////////END READ FRAME DATA//////////////////////////


							///////////////////////////
							//  OPEN CV             ///
							///////////////////////////


							using namespace cv;
							PXCImage *color;
							color = sample->color;

							cv::Mat frameColor = PXCImageDataToCVMat(color);
							//cv::flip(frameColor, frameColor,1);
							
							//cv::imshow("frameColor", frameColor);

							PXCCapture::Device *device;
							device = g_senseManager->QueryCaptureManager()->QueryDevice();
							PXCProjection *projection = device->CreateProjection();
							PXCImage *mappedIm;
							mappedIm = projection->CreateColorImageMappedToDepth(depthIm, colorIm);
							cv::Mat cv_mappedIM = PXCImageDataToCVMat(mappedIm);
							cv::imshow("cv_mappedIM", cv_mappedIM);

							cv::Mat cvm_subtractedBackground(cv_mappedIM.rows, cv_mappedIM.cols, CV_8UC3, Scalar(0, 0, 0));
							if (referenceDepthExists == TRUE)
							{
								for (int x = 0; x < 320; x++)
								{
									for (int y = 0; y < 240; y++)
									{
										if (g_depthdata[x * 2][y * 2] < (referenceDepth[x * 2][y * 2] - 5) || (referenceDepth[x * 2][y * 2] < 1 && g_depthdata[x * 2][y * 2] > 0))
										{
											cvm_subtractedBackground.at<cv::Vec3b>(y, x)[0] = (cv_mappedIM.at<cv::Vec3b>(y, x)[0]);
											cvm_subtractedBackground.at<cv::Vec3b>(y, x)[1] = (cv_mappedIM.at<cv::Vec3b>(y, x)[1]);
											cvm_subtractedBackground.at<cv::Vec3b>(y, x)[2] = (cv_mappedIM.at<cv::Vec3b>(y, x)[2]);
										}
									}
								}
								cv::imshow("cvm_subtractedBackground", cvm_subtractedBackground);
							}

							const int h = 480;
							const int w = 640;
							cv::Mat mat_hsvframe, threshy_2, threshy;

							cv::cvtColor(cvm_subtractedBackground, mat_hsvframe, CV_BGR2HSV);
							
							inRange(mat_hsvframe, cv::Scalar(blobHSVrange_1.min.H, blobHSVrange_1.min.S, blobHSVrange_1.min.V), cv::Scalar(blobHSVrange_1.max.H, blobHSVrange_1.max.S, blobHSVrange_1.max.V), threshy);
							inRange(mat_hsvframe, cv::Scalar(blobHSVrange_2.min.H, blobHSVrange_2.min.S, blobHSVrange_2.min.V), cv::Scalar(blobHSVrange_2.max.H, blobHSVrange_2.max.S, blobHSVrange_2.max.V), threshy_2);
							//unsigned int result = cvb::cvLabel(threshy, labelImg, blobs);
							//invert
							bitwise_not(threshy, threshy);
							bitwise_not(threshy_2, threshy_2);
							
							// Setup SimpleBlobDetector parameters.
							SimpleBlobDetector::Params params;

							// Change thresholds
							params.minThreshold = 100;
							params.maxThreshold = 200;

							// Filter by Area.
							params.filterByArea = true;
							params.minArea = 40;

							// Filter by Circularity
							//params.filterByCircularity = true;
							//params.minCircularity = 0.1;
							params.filterByCircularity = false;

							// Filter by Convexity
							//params.filterByConvexity = true;
							//params.minConvexity = 0.01;
							params.filterByConvexity = false;

							// Filter by Inertia
							//params.filterByInertia = true;
							//params.minInertiaRatio = 0.01;
							params.filterByInertia = false;

							// Storage for blobs
							vector<KeyPoint> keypoints;
							vector<KeyPoint> other_keypoints;


							// Set up detector with params
							SimpleBlobDetector detector(params);

							cv::Mat unBlurred = threshy;
							cv::imshow("unBlurred", unBlurred);

							medianBlur(threshy, threshy, 7);
							medianBlur(threshy_2, threshy_2, 7);

							// Detect blobs
							detector.detect(threshy, keypoints);
							detector.detect(threshy_2, other_keypoints);

							std::vector<PXCPoint3DF32> kp3d;
							for (int i = 0; i < keypoints.size(); i++)
							{
								PXCPoint3DF32 temp;
								temp.x = keypoints.at(i).pt.x * 2;
								temp.y = keypoints.at(i).pt.y * 2;
								temp.z = (float)g_depthdata[(int)temp.x][(int)temp.y];
								kp3d.push_back(temp);
							}
							std::vector<PXCPoint3DF32> other_kp3d;
							for (int i = 0; i < other_keypoints.size(); i++)
							{
								PXCPoint3DF32 temp;
								temp.x = other_keypoints.at(i).pt.x * 2;
								temp.y = other_keypoints.at(i).pt.y * 2;
								temp.z = (float)g_depthdata[(int)temp.x][(int)temp.y];
								other_kp3d.push_back(temp);
							}
							std::vector<PXCPointF32> kp2d = depthXY_to_colorXY(g_senseManager, kp3d);
							std::vector<PXCPointF32> other_kp2d = depthXY_to_colorXY(g_senseManager, other_kp3d);
							for (int i = 0; i < kp3d.size(); i++)
							{
								std::sprintf(str, "kp3d x=%f y=%f z=%f\n", kp3d.at(i).x, kp3d.at(i).y, kp3d.at(i).z);
								debugDisplay(str);
							}
							debugDisplay("\n");
							for (int i = 0; i < kp2d.size(); i++)
							{
								keypoints.at(i).pt.x = kp2d.at(i).x / 2;
								keypoints.at(i).pt.y = kp2d.at(i).y / 2;
							}
							for (int i = 0; i < other_kp2d.size(); i++)
							{
								other_keypoints.at(i).pt.x = other_kp2d.at(i).x / 2;
								other_keypoints.at(i).pt.y = other_kp2d.at(i).y / 2;
							}


							// Draw detected blobs as red circles.
							// DrawMatchesFlags::DRAW_RICH_KEYPOINTS flag ensures
							// the size of the circle corresponds to the size of blob

							cv::Mat im_with_keypoints;
							cv::drawKeypoints(frameColor, keypoints, im_with_keypoints, cv::Scalar(0, 0, 255), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
							cv::drawKeypoints(im_with_keypoints, other_keypoints, im_with_keypoints, cv::Scalar(0, 255, 0), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
							

							cv::imshow("aaa im_with_keypoints", im_with_keypoints); // Show our image inside it.


							cv::imshow("threshy", threshy);
							cv::imshow("threshy_2", threshy_2);
							cv::Mat combined_threshy;
							cv::bitwise_and(threshy, threshy_2, combined_threshy);
							cv::imshow("combined_threshy", combined_threshy);
							cv::Mat threshedColor, combined_threshy_in;
							cv::bitwise_not(combined_threshy, combined_threshy);
							cvm_subtractedBackground.copyTo(threshedColor, combined_threshy);
							cv::imshow("threshedColor", threshedColor);
							//At the end, release all the used memory
							//color->ReleaseAccess(data);
							//delete data;
							for (vector<KeyPoint>::iterator it = keypoints.begin(); it != keypoints.end(); ++it)
							{

								//KeyPoint k = *it;
								//sprintf(str, "it %d size %d pt.xF'%f' pt.yF'%f'\n", &it, keypoints.size(), k.pt.x, k.pt.y);
								//debugDisplay(str);
							}



							if (keypoints.size() > 0)
							{

								PXCPoint3DF32 pos_ijz[1], pos3d[1];
								// Get XY of point from COLOR image
								pos_ijz[0].x = keypoints[0].pt.x *2;
								pos_ijz[0].y = keypoints[0].pt.y *2;
								
								// Map XY of color to XY of DEPTH
								PXCPointF32 pos_ij[1], pos_uv[1];
								pos_ij[0].x = pos_ijz[0].x; pos_ij[0].y = pos_ijz[0].y;
								projection->MapColorToDepth(depthIm, 1, pos_ij, pos_uv);
								
								// Get depth value if a valid mapping exists
								if (pos_uv[0].x > 0 && pos_uv[0].y > 0)
									pos_ijz[0].z = g_depthdata[(int)pos_uv[0].x][(int)pos_uv[0].y];
								else
									pos_ijz[0].z = -1;

								// Get real world coordinates in mm
								if (pos_ijz[0].z > -1)
									projection->ProjectColorToCamera(1, pos_ijz, pos3d);
								else
									pos3d[0].x = pos3d[0].y = pos3d[0].z = -1;

								// Pretty up and display in table
								char sxc[350]; char syc[350]; char szc[350];
								char sxr[350]; char syr[350]; char szr[350];

								std::sprintf(sxc, "%f", pos_ijz[0].x); std::sprintf(syc, "%f", pos_ijz[0].y); std::sprintf(szc, "%f", pos_ijz[0].z);
								std::sprintf(sxr, "%f", pos3d[0].x); std::sprintf(syr, "%f", pos3d[0].y); std::sprintf(szr, "%f", pos3d[0].z);



								xCoord.setText(sxc); yCoord.setText(syc); zCoord.setText(szc);
								xReal.setText(sxr); yReal.setText(syr); zReal.setText(syr);

								//std::sprintf(str, "x=%.0f y=%.0f z=%.0f", pos_ijz[0].x, pos_ijz[0].y, pos_ijz[0].z);
								//std::sprintf(str, "%s\nx=%.0f y=%.0f z=%.0f \n ", str, pos3d[0].x, pos3d[0].y, pos3d[0].z);
								//debugDisplay(str);

								
								//test function
								PXCPointF32 colorXY[1];
								colorXY[0].x = keypoints[0].pt.x * 2;
								colorXY[0].y = keypoints[0].pt.y * 2;
								PXCPoint3DF32 realXYZ[1];
								colorXY_to_realXYZ(g_senseManager, depthIm, g_depthdata, colorXY, realXYZ, 1);

							}

							if (keypoints.size() == 3 && false)
							{
								// Get real world coordingates of points
								PXCPointF32 colorXY[3];
								PXCPoint3DF32 realXYZ[3];
								for (int i = 0; i < 3; i++)
								{
									colorXY[i].x = keypoints[i].pt.x * 2;
									colorXY[i].y = keypoints[i].pt.y * 2;
									colorXY_to_realXYZ(g_senseManager, depthIm, g_depthdata, &colorXY[i], &realXYZ[i], 1);
									//TODO: should be able to process full array in one call
								}
								






								// Calculate flexation

								// http://stackoverflow.com/questions/19729831/angle-between-3-points-in-3d-space
								PXCPoint3DF32 A, B, C;
								A = realXYZ[0];
								//A.x = realXYZ[0].x; A.y = realXYZ[0].y;  A.z = realXYZ[0].z;
								B.x = realXYZ[1].x; B.y = realXYZ[1].y;  B.z = realXYZ[1].z;
								C.x = realXYZ[2].x; C.y = realXYZ[2].y;  C.z = realXYZ[2].z;


								// SMooth data



								char sxc[350]; char syc[350]; char szc[350];
								std::sprintf(sxc, "%f", A.x); std::sprintf(syc, "%f", A.y); std::sprintf(szc, "%f", A.z);
								twi11.setText(sxc); twi12.setText(syc); twi13.setText(szc);
								std::sprintf(sxc, "%f", B.x); std::sprintf(syc, "%f", B.y); std::sprintf(szc, "%f", B.z);
								twi21.setText(sxc); twi22.setText(syc); twi23.setText(szc);
								std::sprintf(sxc, "%f", C.x); std::sprintf(syc, "%f", C.y); std::sprintf(szc, "%f", C.z);
								twi31.setText(sxc); twi32.setText(syc); twi33.setText(szc);
								
								

								float angle_raw = points3DtoAngle(A, B, C);
								float flexation_extension_raw;
								if (A.z > C.z)
									flexation_extension_raw = M_PI - angle_raw;
								else
									flexation_extension_raw = angle_raw - M_PI;

								A = smoother3D_A->SmoothValue(A);
								B = smoother3D_B->SmoothValue(B);
								C = smoother3D_C->SmoothValue(C);

								float angle_smooth = points3DtoAngle(A, B, C);
								float flexation_extension_smooth;
								if (A.z > C.z)
									flexation_extension_smooth = M_PI - angle_smooth;
								else
									flexation_extension_smooth = angle_smooth - M_PI;
								



								rightHand_Flexation->setRadians(flexation_extension_raw);
								leftHand_Flexation->setRadians(flexation_extension_smooth);

								//std::sprintf(str, "angle rad=%f deg=%f\n", flexation_extension, flexation_extension * (180 / M_PI));

								
								debugDisplay(str);

								

							} // End Keybpoints = 3
							PXCPoint3DF32 A, B, C, D, E, F;
							if (keypoints.size() == 3 && other_keypoints.size() == 1)
							{
								// Get real world coordingates of points
								PXCPointF32 colorXY[3];
								PXCPoint3DF32 realXYZ[3];
								
								for (int i = 0; i < 3; i++)
								{
									colorXY[i].x = keypoints[i].pt.x * 2;
									colorXY[i].y = keypoints[i].pt.y * 2;
									colorXY_to_realXYZ(g_senseManager, depthIm, g_depthdata, &colorXY[i], &realXYZ[i], 1);
									//TODO: should be able to process full array in one call
								}
								PXCPoint3DF32 other_realXYZ[1];
								PXCPointF32 other_colorXY[1];
								other_colorXY[0].x = other_keypoints[0].pt.x * 2;
								other_colorXY[0].y = other_keypoints[0].pt.y * 2;
								colorXY_to_realXYZ(g_senseManager, depthIm, g_depthdata, &other_colorXY[0], &other_realXYZ[0], 1);

								//color 1
								A = realXYZ[0];
								F = realXYZ[1];
								C = realXYZ[2];

								// Color 2
								E = other_realXYZ[0];

								//Virtual
								B = midpoint3D(E, F);
								D = extendpoint3D(A, B);

								PXCPoint3DF32 local_B = { 0, 0, 0 };
								PXCPoint3DF32 local_D = { 5, 0, 0 };
								PXCPoint3DF32 local_C = globalToLocalCoord(B, D, E, C);

								PXCPoint3DF32 local_G = { local_C.x, local_C.y, 0 };

								if (true /*frameCounter % 5 == 0*/)
								{
									std::sprintf(str, "D = {%f, %f, %f} local_G = {%f, %f, %f}", D.x, D.y, D.z, local_G.x, local_G.y, local_G.z);
									debugDisplay(str);
								}


								float angle = points3DtoAngle(local_G, local_B, local_C);
								float flexation;
								if (A.z > C.z)
									flexation = M_PI - angle;
								else
									flexation = angle - M_PI;

								float deviation = points3DtoAngle(local_G, local_B, local_D);


								leftHand_Flexation->setRadians(angle);
								rightHand_Flexation->setRadians(deviation);

							}


							
							// Record data
							frameDataV1 temp_frameData;

							temp_frameData.frameNumber = frameCounter;

							temp_frameData.leftHand.pointsDetected = 4;
							temp_frameData.leftHand.detected = TRUE;
							//temp_frameData.leftHand.flexation = 1.1;
							temp_frameData.leftHand.flexation = sin((float)frameCounter/10.0)*45;

							temp_frameData.leftHand.deviation = 2.1;

							temp_frameData.rightHand.pointsDetected = 4;
							temp_frameData.rightHand.detected = TRUE;
							temp_frameData.rightHand.flexation = 3.1;
							temp_frameData.rightHand.deviation = 4.4;

							temp_frameData.time = (clock() - t_startTime) / (float)CLOCKS_PER_SEC;



							
							
							myDoc.arrayOfFramedata.push_back(temp_frameData);
							myDoc.numberOfFrames++;
							




							// Creating reference depth frame
							if (frameCounter == 5)
							{
								referenceDepthExists = TRUE;
								for (int x = 0; x < 640; x++)
									for (int y = 0; y < 480; y++)
										referenceDepth[x][y] = g_depthdata[x][y];
							}


							
								
							/*
							if (referenceDepthExists == TRUE)
							{
								Mat cvm_subtractedBackground(cv_mappedIM.rows, cv_mappedIM.cols, CV_8UC3, Scalar(0, 0, 0));
								for (int x = 0; x < 240; x++)
								{
									for (int y = 0; y < 320; x++)
									{
										if (g_depthdata[x][y] < referenceDepth[x][y])
										{
											cvm_subtractedBackground.at<cv::Vec3b>(y, x)[0] = cv_mappedIM.at<cv::Vec3b>(y, x)[0];
											cvm_subtractedBackground.at<cv::Vec3b>(y, x)[1] = cv_mappedIM.at<cv::Vec3b>(y, x)[1];
											cvm_subtractedBackground.at<cv::Vec3b>(y, x)[2] = cv_mappedIM.at<cv::Vec3b>(y, x)[2];
										}
									}
								}
								cv::imshow("cvm_subtractedBackground", cvm_subtractedBackground);
							}
							*/
														
							///////////////////////////
							//  END OPEN CV          //
							///////////////////////////

							///////////////////////////////////
							//         Display Image         //
							///////////////////////////////////

							std::vector<PXCPointF32> points;
							int npoints = 6;
							PXCPointF32 pos_ij[6];
							PXCPoint3DF32 pos3d[6];
							pos3d[0] = A; pos3d[1] = B; pos3d[2] = C; pos3d[3] = D; pos3d[4] = E; pos3d[5] = F;
							//tv_vid_depth->updateAndPaint(g_depthdata, 'd', points);
							PXCCapture::Device *device2;
							device2 = g_senseManager->QueryCaptureManager()->QueryDevice();
							PXCProjection *projection2 = device2->CreateProjection();
							projection2->ProjectCameraToColor(npoints, pos3d, pos_ij);
							projection2->Release();
							//device->Release();
							
							for (int i=0; i < npoints;i++)
								points.push_back(pos_ij[i]);


							tv_vid_color->updateAndPaint(g_colordata, 'c', points);

							// Clean up after ourselves
							for (size_t i = 640; i > 0;) {
								delete[] g_depthdata[--i];
								delete[] g_colordata[i];

							}
							delete[] g_depthdata;
							delete[] g_colordata;
							colorIm->ReleaseAccess(&colorData);
							depthIm->ReleaseAccess(&depthData);


							// Measure the frame rate
							clock_t t_now = clock();
							float clocksSinceLastFrame = t_now - t_lastFrame;
							t_lastFrame = t_now;
							float secondsSinceLastFrame = clocksSinceLastFrame / CLOCKS_PER_SEC;
							accumulatedTime += secondsSinceLastFrame;
							framesSinceLastCount++;
							// Visually update the fps twice per second
							if (accumulatedTime > 0.5)
							{
								float average_secondsPerFrame = accumulatedTime / framesSinceLastCount;
								float fps = 1 / average_secondsPerFrame;
								std::sprintf(str, "fps %.1f", fps);
								ui.fps->setText(str);
								accumulatedTime = 0;
								framesSinceLastCount = 0;
							}


							//projection->MapDepthToColor(1, point_1_3d, point_2);

							/*
							pxcStatus status;
							PXCPoint3DF32 vertices[640*480];
							status = PXCProjection::QueryVertices(depthIm, *vertices);
							*/

							// Update colors to track
							if (updateColors_1)
							{
								updateColors_1 = false;
								std::vector <PXCPointF32> t_colorPoint = { colorPoint };
								std::vector <PXCPointF32> depth_colorPoint = colorXY_to_depthXY(g_senseManager, depthIm, t_colorPoint);
								std::sprintf(str, "t_colorPoint=%f %f\n depth_colorPoint=%f %f\n depth_colorPoint.size()=%d\n", t_colorPoint.at(0).x, t_colorPoint.at(0).y, depth_colorPoint.at(0).x, depth_colorPoint.at(0).y, depth_colorPoint.size());
								debugDisplay(str);
								qApp->processEvents();
								
								
								HSV base_HSV;
								base_HSV.H = mat_hsvframe.at<cv::Vec3b>(depth_colorPoint.at(0).y / 2, depth_colorPoint.at(0).x / 2)[0];
								base_HSV.S = mat_hsvframe.at<cv::Vec3b>(depth_colorPoint.at(0).y / 2, depth_colorPoint.at(0).x / 2)[1];
								base_HSV.V = mat_hsvframe.at<cv::Vec3b>(depth_colorPoint.at(0).y / 2, depth_colorPoint.at(0).x / 2)[2];
								HSV_ranger(&blobHSVrange_1, base_HSV, 20);

							}
							if (updateColors_2)
							{
								updateColors_2 = false;
								std::vector <PXCPointF32> t_colorPoint = { colorPoint };

								std::vector <PXCPointF32> depth_colorPoint = colorXY_to_depthXY(g_senseManager, depthIm, t_colorPoint);

								HSV base_HSV;
								base_HSV.H = mat_hsvframe.at<cv::Vec3b>(depth_colorPoint.at(0).y / 2, depth_colorPoint.at(0).x / 2)[0];
								base_HSV.S = mat_hsvframe.at<cv::Vec3b>(depth_colorPoint.at(0).y / 2, depth_colorPoint.at(0).x / 2)[1];
								base_HSV.V = mat_hsvframe.at<cv::Vec3b>(depth_colorPoint.at(0).y / 2, depth_colorPoint.at(0).x / 2)[2];
								HSV_ranger(&blobHSVrange_2, base_HSV, 20);

							}


						} //end fram counter

					} // end if update

					g_senseManager->ReleaseFrame();
					qApp->processEvents();


				} // end while acquire frame
			} // end if Init
			else
			{
				std::sprintf(str, "\nFailed to Init PXCSense Manager with error code %d %s\n", initStatus, Definitions::pxcStatusToString(initStatus).c_str() );
				debugDisplay(str);
			}
			float a = clock();
			float b = t_startTime;
			float c = a - b;
			std::sprintf(str, "Total clocks = %f %f %f", a, b, c);
			debugDisplay(str);
			debugDisplay("Function End\n");
			releaseAll();
		
			//myDialog *aDialog;
			mydialog = new myDialog(myDoc,this);
			mydialog->exec();


		} // End startCamera


		

		void on_btnX_clicked()
		{
			/*QString fileName = "vid\\";
			fileName.append(ui.te_recordFile->toPlainText());
			
			// TODO: Check valid and writable
			RecordORPlayback(fileName, true);*/

			//empty_window w;
			
			//w.show();
			/*myDialog d;
			d.setModal(true);
			d.exec();*/
			static int x = 5;

				//mydialog = new myDialog(&x, this);
				//mydialog->show();
			
			HSV_SelectorInst->update_values();
			HSV_SelectorInst->show();
			
		}

		void RecordORPlayback(QString QS_fileName, bool record)
		{		
			std::wstring ws_fileName = QS_fileName.toStdWString();
			std::string ss_fileName = QS_fileName.toStdString();
			char str[350];

			pxcStatus pxc_status;
			setup_sensM(640, 480, true, true, true);

			std::sprintf(str, "Opening file %s for %s...", ss_fileName.c_str(), record ? "recording" : "playback");
			debugDisplay(str); qApp->processEvents();

			pxc_status = g_senseManager->QueryCaptureManager()->SetFileName(ws_fileName.c_str(), record);
			if (pxc_status == PXC_STATUS_NO_ERROR)
			{
				debugDisplay("OK\nInitialising SenseManager..."); qApp->processEvents();
				pxc_status = g_senseManager->Init();
				if (pxc_status == PXC_STATUS_NO_ERROR)
				{
					debugDisplay("OK\n"); qApp->processEvents();
					for (int i = 0; i<24; i++) {
						// This function blocks until a color sample is ready
						if (g_senseManager->AcquireFrame(true)<PXC_STATUS_NO_ERROR) break;

						PXCCapture::Sample *sample = g_senseManager->QuerySample();

						g_senseManager->ReleaseFrame();
					}
				}
				else
				{
					std::sprintf(str, "\nFailed to initialize SenseManager %d %s", pxc_status, Definitions::pxcStatusToString(pxc_status).c_str());
					debugDisplay(str); qApp->processEvents();
				}
			}
			else
			{
				std::sprintf(str, "\nFailed to open %s for %s error %d %s", ss_fileName.c_str(), record ? "recording" : "playback", pxc_status, Definitions::pxcStatusToString(pxc_status).c_str());
				debugDisplay(str); qApp->processEvents();
			}

			// close down
			releaseAll();
		}

	// end new code


		void setup_sensM(int width, int height, bool colorStream, bool depthStream, bool IRStream)
		{
			char str[350];
			// Setup
			debugDisplay("Creating PXCSession..."); qApp->processEvents();
			g_session = PXCSession::CreateInstance();
			if (!g_session)
			{
				debugDisplay("Failed Creating PXCSession\n");
				return;
			}
			else
			{
				debugDisplay("OK.\n");
			}
			qApp->processEvents();

			/* Optional steps to send feedback to Intel Corporation to understand how often each SDK sample is used. */
			PXCMetadata * md = g_session->QueryInstance<PXCMetadata>();
			if (md) {
				pxcCHAR sample_name[] = L"Hands Console";
				md->AttachBuffer(PXCSessionService::FEEDBACK_SAMPLE_INFO, (pxcBYTE*)sample_name, sizeof(sample_name));
			}

			debugDisplay("Creating PXCSenseManager..."); qApp->processEvents();
			g_senseManager = g_session->CreateSenseManager();
			if (!g_senseManager)
			{
				releaseAll();
				debugDisplay("Failed Creating PXCSenseManager\n");
				return;
			}
			else
			{
				debugDisplay("OK.\n");
			} qApp->processEvents();

			pxcStatus streamStatus;
			if (depthStream)
			{
				debugDisplay("Enabling DepthStream..."); qApp->processEvents();
				streamStatus = g_senseManager->EnableStream(PXCCapture::STREAM_TYPE_DEPTH, width, height);
				if (streamStatus == PXC_STATUS_NO_ERROR)
				{

					debugDisplay("OK\n");
				}
				else
				{
					std::sprintf(str, "\nFailed to Enable DepthStream error code %d %s\n", streamStatus, Definitions::pxcStatusToString(streamStatus).c_str());
					debugDisplay(str);
					releaseAll();
					return;
				} qApp->processEvents();
			}
			if (colorStream)
			{
				debugDisplay("Enabling ColorStream"); qApp->processEvents();
				streamStatus = g_senseManager->EnableStream(PXCCapture::STREAM_TYPE_COLOR, width, height);
				if (streamStatus == PXC_STATUS_NO_ERROR)
				{
					debugDisplay("OK\n");
				}
				else
				{
					std::sprintf(str, "\nFailed to Enable ColorStream error code %d %s\n", streamStatus, Definitions::pxcStatusToString(streamStatus).c_str());
					debugDisplay(str);
					releaseAll();
					return;
				} qApp->processEvents();
			}
			if (IRStream)
			{
				debugDisplay("Enabling IRStream"); qApp->processEvents();
				streamStatus = g_senseManager->EnableStream(PXCCapture::STREAM_TYPE_IR, width, height);
				if (streamStatus == PXC_STATUS_NO_ERROR)
				{
					debugDisplay("OK\n");
				}
				else
				{
					std::sprintf(str, "\nFailed to Enable IRStream error code %d %s\n", streamStatus, Definitions::pxcStatusToString(streamStatus).c_str());
					debugDisplay(str);
					releaseAll();
					return;
				} qApp->processEvents();
			}

			debugDisplay("Enabling Hand Module..."); qApp->processEvents();
			int errorMsg = g_senseManager->EnableHand();
			if (errorMsg != PXC_STATUS_NO_ERROR)
			{
				releaseAll();
				std::sprintf(str, "\nFailed Enabling Hand Module %d\n", errorMsg);
				debugDisplay(str);
				return;
			}
			else
			{
				debugDisplay("OK.\n");
			} qApp->processEvents();

			debugDisplay("Creating PXCHandModule..."); qApp->processEvents();
			g_handModule = g_senseManager->QueryHand();
			if (!g_handModule)
			{
				releaseAll();
				debugDisplay("\nFailed Creating PXCHandModule\n");
				return;
			}
			else
			{
				debugDisplay("OK.\n");
			} qApp->processEvents();

			debugDisplay("Creating PXCHandData..."); qApp->processEvents();
			g_handDataOutput = g_handModule->CreateOutput();
			if (!g_handDataOutput)
			{
				releaseAll();
				debugDisplay("\nFailed Creating PXCHandData\n");
				return;
			}
			else
			{
				debugDisplay("OK.\n");
			} qApp->processEvents();

			debugDisplay("Creating PXCHandConfiguration..."); qApp->processEvents();
			g_handConfiguration = g_handModule->CreateActiveConfiguration();
			if (!g_handConfiguration)
			{
				releaseAll();
				debugDisplay("\nFailed Creating PXCHandConfiguration\n");
				return;
			}
			else
			{
				debugDisplay("OK.\n");
			} qApp->processEvents();
		}




		void set_current()
		{
			PXCCapture::Device *device;
			device = g_senseManager->QueryCaptureManager()->QueryDevice();

			ui.sb_IVCAMAccuracy->setValue(device->QueryIVCAMAccuracy());
			ui.sb_IVCAMFilter->setValue(device->QueryIVCAMFilterOption());
			ui.sb_IVCAMLaserPower->setValue(device->QueryIVCAMLaserPower());
			ui.sb_IVCAMMotionRangeTradeOff->setValue(device->QueryIVCAMMotionRangeTradeOff());
		}

		void on_sb_IVCAMAccuracy_valueChanged()
		{
			int value = ui.sb_IVCAMAccuracy->value();
			IVCAM_Accuracy_update(value);
		}
		void on_sb_IVCAMFilter_valueChanged()
		{
			int value = ui.sb_IVCAMFilter->value();
			IVCAM_Filter_update(value);
		}
		void on_sb_IVCAMLaserPower_valueChanged()
		{
			int value = ui.sb_IVCAMLaserPower->value();
			IVCAM_LaserPower_update(value);
		}
		void on_sb_IVCAMMotionRangeTradeOff_valueChanged()
		{
			int value = ui.sb_IVCAMMotionRangeTradeOff->value();
			IVCAM_MotionRangeTradeOff_update(value);
		}

		void on_pb_default_all_clicked()
		{

			IVCAM_Accuracy_restoreDefault();
			IVCAM_Filter_restoreDefault();
			IVCAM_LaserPower_restoreDefault();
			IVCAM_MotionRangeTradeOff_restoreDefault();

			if (g_senseManager)
			{
				PXCCapture::Device *device;
				device = g_senseManager->QueryCaptureManager()->QueryDevice();
				device->QueryIVCAMAccuracy(), device->QueryIVCAMFilterOption(), device->QueryIVCAMLaserPower(), device->QueryIVCAMMotionRangeTradeOff();
			}

		}

		void IVCAM_Accuracy_update(int newValue)
		{
			ui.sb_IVCAMAccuracy->setValue(newValue);
			if (g_senseManager)
			{
				PXCCapture::Device *device;
				device = g_senseManager->QueryCaptureManager()->QueryDevice();
				PXCCapture::Device::IVCAMAccuracy val = static_cast<PXCCapture::Device::IVCAMAccuracy>(newValue);
				device->SetIVCAMAccuracy(val);
			}
		}
		void IVCAM_Filter_update(int newValue)
		{
			ui.sb_IVCAMFilter->setValue(newValue);
			if (g_senseManager)
			{
				PXCCapture::Device *device;
				device = g_senseManager->QueryCaptureManager()->QueryDevice();
				device->SetIVCAMFilterOption(newValue);
			}
		}
		void IVCAM_LaserPower_update(int newValue)
		{
			ui.sb_IVCAMLaserPower->setValue(newValue);
			if (g_senseManager)
			{
				PXCCapture::Device *device;
				device = g_senseManager->QueryCaptureManager()->QueryDevice();
				device->SetIVCAMLaserPower(newValue);
			}
		}
		void IVCAM_MotionRangeTradeOff_update(int newValue)
		{
			ui.sb_IVCAMMotionRangeTradeOff->setValue(newValue);
			if (g_senseManager)
			{
				PXCCapture::Device *device;
				device = g_senseManager->QueryCaptureManager()->QueryDevice();
				device->SetIVCAMMotionRangeTradeOff(newValue);
			}
		}

		void IVCAM_Accuracy_restoreDefault()
		{
			if (g_senseManager)
			{
				PXCCapture::Device *device;
				device = g_senseManager->QueryCaptureManager()->QueryDevice();
				IVCAM_Accuracy_update(device->QueryIVCAMAccuracyDefaultValue());
			}
		}
		void IVCAM_Filter_restoreDefault()
		{
			/*if (g_senseManager)
			{
				PXCCapture::Device *device;
				device = g_senseManager->QueryCaptureManager()->QueryDevice();
				IVCAM_Filter_update(device->QueryIVCAMFilterDefaultValue());
			}*/
		}
		void IVCAM_LaserPower_restoreDefault()
		{
			if (g_senseManager)
			{
				PXCCapture::Device *device;
				device = g_senseManager->QueryCaptureManager()->QueryDevice();
				IVCAM_LaserPower_update(16);
			}
		}
		void IVCAM_MotionRangeTradeOff_restoreDefault()
		{
			/*if (g_senseManager)
			{
				PXCCapture::Device *device;
				device = g_senseManager->QueryCaptureManager()->QueryDevice();
				IVCAM_Accuracy_update(device->QueryIVCAMAccuracyDefaultValue());
			}*/
		}

};


#endif // SAMPLE_LAYOUT_1_H



