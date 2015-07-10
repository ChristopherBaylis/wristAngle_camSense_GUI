#include "sample_layout_1.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QCoreApplication>
#include <QHeaderView>
#include <QMessageBox>

sample_layout_1::sample_layout_1(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	// Prepare "speedo's" for flexation angle display and link to widget.
	scene_LeftHand = new QGraphicsScene(this);	//Left Flexation dial
	ui.QGV_FlexationDial_Left->setScene(scene_LeftHand);
	leftHand_Flexation = new AnalogDial(ui.QGV_FlexationDial_Left->height(), ui.QGV_FlexationDial_Left->width());
	scene_LeftHand->addItem(leftHand_Flexation);

	scene_RightHand = new QGraphicsScene(this);	//Right Flexation dial
	ui.QGV_FlexationDial_Right->setScene(scene_RightHand);
	rightHand_Flexation = new AnalogDial(ui.QGV_FlexationDial_Right->height(), ui.QGV_FlexationDial_Right->width());
	scene_RightHand->addItem(rightHand_Flexation);

	// Prepare video streams for display and link to widget.
	scene_Video_depth = new QGraphicsScene(this); //Depth image
	ui.QGV_Video_depth->setScene(scene_Video_depth);
	tv_vid_depth = new test_vid(ui.QGV_Video_depth->height(), ui.QGV_Video_depth->width());
	scene_Video_depth->addItem(tv_vid_depth);
	
	scene_Video_color = new QGraphicsScene(this); //color image
	ui.QGV_Video_color->setScene(scene_Video_color);
	tv_vid_color = new test_vid(ui.QGV_Video_color->height(), ui.QGV_Video_color->width());
	scene_Video_color->addItem(tv_vid_color);
}

// PRIVATE FUNCTIONS

void sample_layout_1::debugDisplay(QString text)
{
	ui.debugOut->appendPlainText(text);
}

void sample_layout_1::calculateAngles(float O, float A, float &theta, float &thetaDegrees){
	theta = atan(O / A);
	thetaDegrees = theta * 180 / float(3.14159);
}

void sample_layout_1::printAngles(int label, float O, float A, float theta, float thetaDegrees){
	char str[350];
	//std::sprintf(str, "   %s:\tO: %+08.4f\tA: %+08.4f\t%c: %+08.4f\t%c: %+08.4f%c", label, O, A, 233, theta, 233, thetaDegrees, 248);
	//debugDisplay(str);
	std::sprintf(str, "%f", O); ui.tableWidget_2->setItem(label, 0, new QTableWidgetItem(str));
	std::sprintf(str, "%f", A); ui.tableWidget_2->setItem(label, 1, new QTableWidgetItem(str));
	std::sprintf(str, "%f", theta); ui.tableWidget_2->setItem(label, 2, new QTableWidgetItem(str));
	std::sprintf(str, "%f", thetaDegrees); ui.tableWidget_2->setItem(label, 3, new QTableWidgetItem(str));
	//if (label == 0)
	//	leftHand_Flexation->setRadians(theta);
}

void sample_layout_1::calculateAnglesAndPrint(int label, float O, float A, float *theta, float thetaDegrees){
	calculateAngles(O, A, *theta, thetaDegrees);
	printAngles(label, O, A, *theta, thetaDegrees);
}

sample_layout_1::~sample_layout_1()
{

}
