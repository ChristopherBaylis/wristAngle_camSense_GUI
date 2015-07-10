#ifndef SAMPLE_LAYOUT_1_H
#define SAMPLE_LAYOUT_1_H

#include <QtWidgets/QMainWindow>
#include "ui_sample_layout_1.h"

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
//END Realsense

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

	// begin new code
	public slots:

		void on_stopCamera_clicked() {
			debugDisplay("User has selected to stop camera");
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
		
		void on_startCamera_clicked() {
			char str[350];
			g_stop = FALSE;

			debugDisplay("button pushed");
			// Setup
			g_session = PXCSession::CreateInstance();
			if (!g_session)
			{
				debugDisplay("Failed Creating PXCSession");
				return;
			}
			else
			{
				debugDisplay("Created PXCSession.");
			}
			qApp->processEvents();

			/* Optional steps to send feedback to Intel Corporation to understand how often each SDK sample is used. */
			PXCMetadata * md = g_session->QueryInstance<PXCMetadata>();
			if (md) {
				pxcCHAR sample_name[] = L"Hands Console";
				md->AttachBuffer(PXCSessionService::FEEDBACK_SAMPLE_INFO, (pxcBYTE*)sample_name, sizeof(sample_name));
			}

			g_senseManager = g_session->CreateSenseManager();
			if (!g_senseManager)
			{
				releaseAll();
				debugDisplay("Failed Creating PXCSenseManager");
				return;
			}
			else
			{
				debugDisplay("Created PXCSenseManager.");
			} qApp->processEvents();

			pxcStatus streamStatus;
			streamStatus = g_senseManager->EnableStream(PXCCapture::STREAM_TYPE_DEPTH, 640, 480);
			if (streamStatus == PXC_STATUS_NO_ERROR)
			{
				
				debugDisplay("Enabled DepthStream");
			}
			else
			{
				std::sprintf(str, "Failed to Enable DepthStream error code %d %s", streamStatus, Definitions::pxcStatusToString(streamStatus).c_str());
				debugDisplay(str);
				releaseAll();
				return;
			}
			streamStatus = g_senseManager->EnableStream(PXCCapture::STREAM_TYPE_COLOR, 640, 480);
			if (streamStatus == PXC_STATUS_NO_ERROR)
			{
				debugDisplay("Enabled ColorStream");
			}
			else
			{
				std::sprintf(str, "Failed to Enable ColorStream error code %d %s", streamStatus, Definitions::pxcStatusToString(streamStatus).c_str());
				debugDisplay(str);
				releaseAll();
				return;
			} qApp->processEvents();

			int errorMsg = g_senseManager->EnableHand();
			if (errorMsg != PXC_STATUS_NO_ERROR)
			{
				releaseAll();
				std::sprintf(str, "Failed Enabling Hand Module %d", errorMsg);
				debugDisplay(str);
				return;
			}
			else
			{
				debugDisplay("Enabled Hand Module.");
			} qApp->processEvents();

			g_handModule = g_senseManager->QueryHand();
			if (!g_handModule)
			{
				releaseAll();
				debugDisplay("Failed Creating PXCHandModule");
				return;
			}
			else
			{
				debugDisplay("Created PXCHandModule.");
			} qApp->processEvents();

			g_handDataOutput = g_handModule->CreateOutput();
			if (!g_handDataOutput)
			{
				releaseAll();
				debugDisplay("Failed Creating PXCHandData");
				return;
			}
			else
			{
				debugDisplay("Created PXCHandData.");
			} qApp->processEvents();

			g_handConfiguration = g_handModule->CreateActiveConfiguration();
			if (!g_handConfiguration)
			{
				releaseAll();
				debugDisplay("Failed Creating PXCHandConfiguration");
				return;
			}
			else
			{
				debugDisplay("Created PXCHandConfiguration.");
			} qApp->processEvents();

			g_handConfiguration->EnableAllAlerts();

			// Apply configuration setup
			g_handConfiguration->ApplyChanges();

			hand_status *leftHand_table;
			hand_status *rightHand_table;
			leftHand_table = new hand_status(ui.table_HandStatus_Left);
			rightHand_table = new hand_status(ui.table_HandStatus_Right);

			// First Initializing the sense manager
			pxcStatus initStatus = g_senseManager->Init();
			if (initStatus == PXC_STATUS_NO_ERROR)
			{
				int max_frames_to_process = 10000;
				debugDisplay("PXCSenseManager Initializing OK. BEGIN PROCESSING FRAMES!");
				std::sprintf(str, "Will stop after auto stop routine after %d frames", max_frames_to_process);
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
									std::sprintf(str, "%s was fired at frame %d ", Definitions::AlertToString(alertData.label).c_str(), alertData.frameNumber);
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
										std::sprintf(str, "Cannot determine body side of detected hand %d of %d.", i, g_handDataOutput->QueryNumberOfHands());
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
										std::sprintf(str, "%f", jointData.positionWorld.z);  ui.tableWidget->setItem(k, 2, new QTableWidgetItem(str));
										std::sprintf(str, "k=%d", k);
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
							}

							/*           Prepare and display the actual images           */
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

							tv_vid_color->updateAndPaint(g_colordata, jointDataArr, g_handDataOutput->QueryNumberOfHands(), 7, 'c');
							tv_vid_depth->updateAndPaint(g_depthdata, jointDataArr, g_handDataOutput->QueryNumberOfHands(), 7, 'd');

						} //end fram counter

					} // end if update

					g_senseManager->ReleaseFrame();
					qApp->processEvents();
				} // end while acquire frame
			} // end if Init
			else
			{
				std::sprintf(str, "Failed to Init PXCSense Manager with error code %d %s", initStatus, Definitions::pxcStatusToString(initStatus).c_str() );
				debugDisplay(str);
			}
			debugDisplay("Function End");
			releaseAll();
		} // End startCamera


	// end new code
};


#endif // SAMPLE_LAYOUT_1_H