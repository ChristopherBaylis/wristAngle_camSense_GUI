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

#include "pxcsensemanager.h"
#include "pxcmetadata.h"
#include "service/pxcsessionservice.h"

#include "pxchanddata.h"
#include "pxchandconfiguration.h"

#include "handData_Definitions.h"
#include "pxccapture.h"

#include "pxcprojection.h"
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

	// begin new code
	public slots:

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

			Hmin = 32;
			Smin = 116;
			Vmin = 53;

			Hmax = 61;
			Smax = 255;
			Vmax = 218;

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
			if (initStatus == PXC_STATUS_NO_ERROR)
			{
				int max_frames_to_process = 900;
				debugDisplay("OK. BEGIN PROCESSING FRAMES!\n");
				std::sprintf(str, "Will mauto stop routine after %d frames\n", max_frames_to_process);
				set_current();
				debugDisplay(str);
				qApp->processEvents();
				int frameCounter = 0;



				// Acquiring frames from input device
				while (g_senseManager->AcquireFrame(true) == PXC_STATUS_NO_ERROR && !g_stop && frameCounter < max_frames_to_process)
				{
					frameCounter++;
					std::sprintf(str, "Frame: %d", frameCounter);
					ui.frameCount->setText(str);

					// Get current hand outputs
					if (g_handDataOutput->Update() == PXC_STATUS_NO_ERROR)
					{
						bool displayAllFrames = true;
						if (frameCounter % 5 == 0 || displayAllFrames)
						{

							///////////////////////////////////
							// Processing and work goes here //
							///////////////////////////////////

							///////////////////////////////////
							//        Display Alerts         //
							///////////////////////////////////
							PXCHandData::AlertData alertData;
							for (int i = 0; i < g_handDataOutput->QueryFiredAlertsNumber(); ++i)
							{
								if (g_handDataOutput->QueryFiredAlertData(i, alertData) == PXC_STATUS_NO_ERROR)
								{
									std::sprintf(str, "%s was fired at frame %d\n", Definitions::AlertToString(alertData.label).c_str(), alertData.frameNumber);
									debugDisplay(str);
								}
							}

							///////////////////////////////////
							//        Display Status         //
							///////////////////////////////////
							leftHand_table->detection_status(false);
							rightHand_table->detection_status(false);
							if (g_handDataOutput->QueryNumberOfHands() > 0)
							{
								
								for (int i = 0; i < g_handDataOutput->QueryNumberOfHands(); i++)
								{
									hand_status *this_table;
									PXCHandData::IHand *handX;
									g_handDataOutput->QueryHandData(PXCHandData::ACCESS_ORDER_BY_TIME, 0, handX);
									if (handX->QueryBodySide() == PXCHandData::BODY_SIDE_LEFT)
									{
										this_table = leftHand_table;
									}
									else if (handX->QueryBodySide() == PXCHandData::BODY_SIDE_RIGHT)
									{
										this_table = rightHand_table;
									}
									else
									{
										std::sprintf(str, "Cannot determine body side of detected hand %d of %d.\n", i, g_handDataOutput->QueryNumberOfHands());
										debugDisplay(str);
										break;
									}

									this_table->detection_status(true);
									
									if (handX->QueryTrackingStatus() == PXCHandData::TrackingStatusType::TRACKING_STATUS_GOOD)
										this_table->tracking_status(true);
									else
										this_table->tracking_status(false);

									if (handX->IsCalibrated())
										this_table->calibration_status(true);
									else
										this_table->calibration_status(false);

								}
							}
							
							///////////////////////////////////
							//        Display Joints         //
							///////////////////////////////////
							////////////////FRAME DATA///////////////////////////////////
							int **g_depthdata = new int*[640];
							for (size_t i = 0; i < 640; i++)
								g_depthdata[i] = new int[480];

							int **g_colordata = new int*[640];
							for (size_t i = 0; i < 640; i++)
								g_colordata[i] = new int[480];

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
							//////////////////END FRAME DATA//////////////////////////
							int nJointsToSpew = 3;
							int jointsToSpew[3];
							jointsToSpew[0] = PXCHandData::JOINT_MIDDLE_BASE; jointsToSpew[1] = PXCHandData::JOINT_CENTER; jointsToSpew[2] = PXCHandData::JOINT_WRIST;

							PXCHandData::IHand *hand;
							PXCHandData::JointData jointData;
							
							for (int i = 0; i < g_handDataOutput->QueryNumberOfHands(); ++i)
							{
								g_handDataOutput->QueryHandData(PXCHandData::ACCESS_ORDER_BY_TIME, i, hand);
								std::string handSide = "Unknown Hand";
								handSide = hand->QueryBodySide() == PXCHandData::BODY_SIDE_LEFT ? "LEFT HAND" : "RIGHT HAND";

								//std::sprintf(str, "%s | Calibrated: %s | Frame: \t%d\n============================================\n\n", handSide.c_str(), hand->IsCalibrated() ? "YES" : "NO", frameCounter);
								//debugDisplay(str);
								int jointID;
								for (int k = 0; k < nJointsToSpew; ++k)
								{
									jointID = jointsToSpew[k];
									if (hand->QueryTrackedJoint((PXCHandData::JointType)jointID, jointData) == PXC_STATUS_NO_ERROR)
									{
										//std::sprintf(str, "   %s\tX: %+08.4f\tY: %+08.4f\tZ: %+08.4f\tconf: %d%%\n", Definitions::JointToString((PXCHandData::JointType)jointID).c_str() + 6, jointData.positionWorld.x, jointData.positionWorld.y, jointData.positionWorld.z, jointData.confidence);
										//debugDisplay(str);
										//std::sprintf(str, "   %s\tX: %+08.4f\tY: %+08.4f\tZ: %+08.4f\tconf: %d%%\n", Definitions::JointToString((PXCHandData::JointType)jointID).c_str() + 6, jointData.positionImage.x, jointData.positionImage.y, jointData.positionImage.z, jointData.confidence);
										//debugDisplay(str);
										//int xxxx = jointData.positionImage.x, yyyy = jointData.positionImage.y, zzzz = jointData.positionImage.z;
										//std::printf("   %s\t\t\t\t\tZ: %d\n\n", Definitions::JointToString((PXCHandData::JointType)jointID).c_str() + 6, g_depthdata[xxxx*yyyy + yyyy]);
										
										std::sprintf(str, "%f", jointData.positionWorld.x);  ui.tableWidget->setItem(k, 0, new QTableWidgetItem(str));
										std::sprintf(str, "%f", jointData.positionWorld.y);  ui.tableWidget->setItem(k, 1, new QTableWidgetItem(str));
										std::sprintf(str, "%.0f", jointData.positionWorld.z * 1000);  ui.tableWidget->setItem(k, 2, new QTableWidgetItem(str));
										int x = (int)jointData.positionImage.x;
										int y = (int)jointData.positionImage.y;
										int raw_z = g_depthdata[x][y];
										std::sprintf(str, "%d", raw_z);  ui.tableWidget->setItem(k, 3, new QTableWidgetItem(str));
 
										PXCPoint3DF32 point_1_3d[1];
										PXCPointF32 point_2[1];
										point_1_3d[0].x = jointData.positionImage.x;
										point_1_3d[0].y = jointData.positionImage.y;
										point_1_3d[0].z = g_depthdata[(int)point_1_3d[0].x][(int)point_1_3d[0].y];
										PXCCapture::Device *device;
										device = g_senseManager->QueryCaptureManager()->QueryDevice();
										PXCProjection *projection = device->CreateProjection();
										projection->MapDepthToColor(1, point_1_3d, point_2);
										x = (int)point_2[0].x;
										y = (int)point_2[0].y;
										int map_z;
										if (x > -1 && y > -1)
											map_z = g_depthdata[x][y];
										else
											map_z = -1;
										std::sprintf(str, "%d", raw_z-int(jointData.positionWorld.z * 1000) );  ui.tableWidget->setItem(k, 4, new QTableWidgetItem(str));

									}
								}

								PXCHandData::JointData jd_wrist;
								PXCHandData::JointData jd_palm;
								PXCHandData::JointData jd_middle_base;
								hand->QueryTrackedJoint((PXCHandData::JointType)PXCHandData::JOINT_WRIST, jd_wrist);
								hand->QueryTrackedJoint((PXCHandData::JointType)PXCHandData::JOINT_CENTER, jd_palm);
								hand->QueryTrackedJoint((PXCHandData::JointType)PXCHandData::JOINT_MIDDLE_BASE, jd_middle_base);

								pxcF32 O, A;
								float theta, thetaDegrees;
								theta = thetaDegrees = 0;

								O = jd_wrist.positionWorld.z - jd_palm.positionWorld.z;
								A = abs(jd_palm.positionWorld.y - jd_wrist.positionWorld.y);
								calculateAnglesAndPrint(0, O, A, &theta, thetaDegrees);
								if (hand->QueryBodySide() == PXCHandData::BODY_SIDE_LEFT)
									leftHand_Flexation->setRadians(theta);
								else if (hand->QueryBodySide() == PXCHandData::BODY_SIDE_RIGHT)
									rightHand_Flexation->setRadians(theta);


								//g_depthdata[xxxx*yyyy + yyyy];
								//O = g_depthdata[(int)jd_wrist.positionImage.x*(int)jd_wrist.positionImage.y + (int)jd_wrist.positionImage.y] - g_depthdata[(int)jd_palm.positionImage.x*(int)jd_palm.positionImage.y + (int)jd_palm.positionImage.y];
								//A = (abs(jd_palm.positionWorld.y - jd_wrist.positionWorld.y) * 1000);
								//calculateAnglesAndPrint("PALM rs", O, A, theta, thetaDegrees);

								O = jd_wrist.positionWorld.z - jd_middle_base.positionWorld.z;
								A = abs(jd_middle_base.positionWorld.y - jd_wrist.positionWorld.y);
								calculateAnglesAndPrint(1, O, A, &theta, thetaDegrees);



							} //end iterate hands

							///////////////////////////////////
							//         Display Image         //
							///////////////////////////////////

							/* Prepare some information about tracked joints to pass along with the video stream */
							float **jointDataArr = new float*[g_handDataOutput->QueryNumberOfHands()];
							for (size_t i = 0; i < g_handDataOutput->QueryNumberOfHands(); i++)
								jointDataArr[i] = new float[15];

							int n_points = 7;
							PXCPointF32 point_1[7];
							PXCPointF32 point_2[7];
							PXCPoint3DF32 point_1_3d[7];

							for (int i = 0; i < g_handDataOutput->QueryNumberOfHands(); ++i)
							{
								g_handDataOutput->QueryHandData(PXCHandData::ACCESS_ORDER_BY_TIME, i, hand);

								PXCHandData::JointData jd_wrist;
								PXCHandData::JointData jd_middle_base;
								PXCHandData::JointData jd_THUMB_TIP;
								PXCHandData::JointData jd_INDEX_TIP;
								PXCHandData::JointData jd_MIDDLE_TIP;
								PXCHandData::JointData jd_RING_TIP;
								PXCHandData::JointData jd_PINKY_TIP;

								hand->QueryTrackedJoint((PXCHandData::JointType)PXCHandData::JOINT_WRIST, jd_wrist);
								hand->QueryTrackedJoint((PXCHandData::JointType)PXCHandData::JOINT_MIDDLE_BASE, jd_middle_base);

								hand->QueryTrackedJoint((PXCHandData::JointType)PXCHandData::JOINT_THUMB_TIP, jd_THUMB_TIP);
								hand->QueryTrackedJoint((PXCHandData::JointType)PXCHandData::JOINT_INDEX_TIP, jd_INDEX_TIP);
								hand->QueryTrackedJoint((PXCHandData::JointType)PXCHandData::JOINT_MIDDLE_TIP, jd_MIDDLE_TIP);
								hand->QueryTrackedJoint((PXCHandData::JointType)PXCHandData::JOINT_RING_TIP, jd_RING_TIP);
								hand->QueryTrackedJoint((PXCHandData::JointType)PXCHandData::JOINT_PINKY_TIP, jd_PINKY_TIP);

								jointDataArr[i][0] = jd_wrist.positionImage.x;
								jointDataArr[i][1] = jd_wrist.positionImage.y;
								jointDataArr[i][2] = jd_middle_base.positionImage.x;
								jointDataArr[i][3] = jd_middle_base.positionImage.y;

								jointDataArr[i][4] = jd_THUMB_TIP.positionImage.x;
								jointDataArr[i][5] = jd_THUMB_TIP.positionImage.y;
								jointDataArr[i][6] = jd_INDEX_TIP.positionImage.x;
								jointDataArr[i][7] = jd_INDEX_TIP.positionImage.y;
								jointDataArr[i][8] = jd_MIDDLE_TIP.positionImage.x;
								jointDataArr[i][9] = jd_MIDDLE_TIP.positionImage.y;
								jointDataArr[i][10] = jd_RING_TIP.positionImage.x;
								jointDataArr[i][11] = jd_RING_TIP.positionImage.y;
								jointDataArr[i][12] = jd_PINKY_TIP.positionImage.x;
								jointDataArr[i][13] = jd_PINKY_TIP.positionImage.y;

								if (hand->QueryBodySide() == PXCHandData::BODY_SIDE_LEFT)
									jointDataArr[i][14] = 1;
								else if (hand->QueryBodySide() == PXCHandData::BODY_SIDE_RIGHT)
									jointDataArr[i][14] = 2;
								else
									jointDataArr[i][14] = 3;

								if (i == 0)
								{
									point_1_3d[0].z = jointDataArr[0][0] = jd_wrist.positionImage.x;
									point_1_3d[1].z = jointDataArr[0][1] = jd_wrist.positionImage.y;
									point_1_3d[2].z = jointDataArr[0][2] = jd_middle_base.positionImage.x;
									point_1_3d[3].z = jointDataArr[0][3] = jd_middle_base.positionImage.y;

									point_1_3d[4].z = jointDataArr[0][4] = jd_THUMB_TIP.positionImage.x;
									point_1_3d[5].z = jointDataArr[0][5] = jd_THUMB_TIP.positionImage.y;
									point_1_3d[6].z = jointDataArr[0][6] = jd_INDEX_TIP.positionImage.x;
									point_1_3d[7].z = jointDataArr[0][7] = jd_INDEX_TIP.positionImage.y;
									point_1_3d[8].z = jointDataArr[0][8] = jd_MIDDLE_TIP.positionImage.x;
									point_1_3d[9].z = jointDataArr[0][9] = jd_MIDDLE_TIP.positionImage.y;
									point_1_3d[10].z = jointDataArr[0][10] = jd_RING_TIP.positionImage.x;
									point_1_3d[11].z = jointDataArr[0][11] = jd_RING_TIP.positionImage.y;
									point_1_3d[12].z = jointDataArr[0][12] = jd_PINKY_TIP.positionImage.x;
									point_1_3d[13].z = jointDataArr[0][13] = jd_PINKY_TIP.positionImage.y;
								}
							}

							/*           Prepare and display the actual images           */


							if (g_handDataOutput->QueryNumberOfHands() > 0)
							{
								for (int i = 0; i < n_points; i++)
								{
									point_1_3d[i].x = point_1[i].x = jointDataArr[0][i * 2];
									point_1_3d[i].y = point_1[i].y = jointDataArr[0][(i * 2) + 1];
									point_1_3d[i].z = g_depthdata[(int)point_1_3d[i].x][(int)point_1_3d[i].y];
								}

								

								PXCCapture::Device *device;
								device = g_senseManager->QueryCaptureManager()->QueryDevice();
								PXCProjection *projection = device->CreateProjection();

								//projection->MapColorToDepth(depthIm, n_points, point_1, point_2);
								projection->MapDepthToColor(n_points, point_1_3d, point_2);

								for (int i = 0; i < n_points; i++)
								{
									jointDataArr[0][i * 2] = point_2[i].x;
									jointDataArr[0][(i * 2) + 1] = point_2[i].y;
								}

								debugDisplay("PROJECTIONS\n==============================\n");
								for (int i = 0; i < n_points; i++)
								{
									std::sprintf(str, "point_1[%d].x = %f point_1[%d].y = %f\n", i, point_1[i].x, i, point_1[i].y);
									debugDisplay(str);
									std::sprintf(str, "point_2[%d].x = %f point_2[%d].y = %f\n", i, point_2[i].x, i, point_2[i].y);
									debugDisplay(str);
								}
								debugDisplay("==============================\n");
							}
								
							//pxc_status = projection.MapColorToDepth(depthIm, pxcI32 npoints, PXCPointF32 *pos_ij, PXCPointF32 *pos_uv);//bm


							tv_vid_color->updateAndPaint(g_colordata, jointDataArr, g_handDataOutput->QueryNumberOfHands(), 7, 'c');
							tv_vid_depth->updateAndPaint(g_depthdata, jointDataArr, g_handDataOutput->QueryNumberOfHands(), 7, 'd');

							/*for (size_t i = 640; i > 0;) {
								delete[] g_depthdata[--i];
								delete[] g_colordata[i];
							}
							delete[] g_depthdata;
							delete[] g_colordata;
						*/

						///////////////////////////
						//  OPEN CV             ///
						///////////////////////////

							
							using namespace cv;
							PXCImage *color;
							color = sample->color;

							cv::Mat frameColor = PXCImageDataToCVMat(color);

							const int h = 480;
							const int w = 640;
							cv::Mat mat_hsvframe, threshy;

							cv::cvtColor(frameColor, mat_hsvframe, CV_BGR2HSV);

							inRange(mat_hsvframe, cv::Scalar(Hmin, Smin, Vmin), cv::Scalar(Hmax, Smax, Vmax), threshy);
							//unsigned int result = cvb::cvLabel(threshy, labelImg, blobs);
							//invert
							bitwise_not(threshy, threshy);
								
							// Setup SimpleBlobDetector parameters.
							SimpleBlobDetector::Params params;

							// Change thresholds
							params.minThreshold = 1;
							params.maxThreshold = 200;

							// Filter by Area.
							params.filterByArea = true;
							params.minArea = 15;

							// Filter by Circularity
							params.filterByCircularity = true;
							params.minCircularity = 0.1;

							// Filter by Convexity
							params.filterByConvexity = true;
							params.minConvexity = 0.01;

							// Filter by Inertia
							params.filterByInertia = true;
							params.minInertiaRatio = 0.01;


							// Storage for blobs
							vector<KeyPoint> keypoints;


							// Set up detector with params
							SimpleBlobDetector detector(params);

							// Detect blobs
							detector.detect(threshy, keypoints);

							// Draw detected blobs as red circles.
							// DrawMatchesFlags::DRAW_RICH_KEYPOINTS flag ensures
							// the size of the circle corresponds to the size of blob

							cv::Mat im_with_keypoints;
							cv::drawKeypoints(frameColor, keypoints, im_with_keypoints, cv::Scalar(0, 0, 255), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

							cv::imshow("im_with_keypoints", im_with_keypoints); // Show our image inside it.
							
							
							cv::imshow("threshy", threshy);
							//At the end, release all the used memory
							//color->ReleaseAccess(data);
							//delete data;
							sort(keypoints.begin(), keypoints.end(), sortbypty);
							for (vector<KeyPoint>::iterator it = keypoints.begin(); it != keypoints.end(); ++it)
							{
								
								KeyPoint k = *it;
								sprintf(str, "it %d size %d pt.xF'%f' pt.yF'%f'\n", &it, keypoints.size(), k.pt.x, k.pt.y);
									debugDisplay(str);
							}
							
						///////////////////////////
						//  END OPEN CV          //
						///////////////////////////

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
			debugDisplay("Function End\n");
			releaseAll();
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
			mydialog = new myDialog(&x, this);
			mydialog->show();

			HSV_SelectorInst = new HSV_Selector(&Hmin, &Smin, &Vmin, &Hmax, &Smax, &Vmax, this);
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
