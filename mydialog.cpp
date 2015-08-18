#include "mydialog.h"

myDialog::myDialog(document g_myDoc, QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	
	myDoc = g_myDoc;
	displayInfo();


	scene_left_flex = new QGraphicsScene(this);
	ui.QGV_leftFlexation->setScene(scene_left_flex);
	graph_left_flex = new graph_painter(ui.QGV_leftFlexation->height(), ui.QGV_leftFlexation->width());
	scene_left_flex->addItem(graph_left_flex);

	scene_left_deviation = new QGraphicsScene(this);
	ui.QGV_leftDeviation->setScene(scene_left_deviation);
	graph_left_deviation = new graph_painter(ui.QGV_leftDeviation->height(), ui.QGV_leftDeviation->width());
	scene_left_deviation->addItem(graph_left_deviation);

	scene_right_flex = new QGraphicsScene(this);
	ui.QGV_rightFlexation->setScene(scene_right_flex);
	graph_right_flex = new graph_painter(ui.QGV_rightFlexation->height(), ui.QGV_rightFlexation->width());
	scene_right_flex->addItem(graph_right_flex);

	scene_right_deviation = new QGraphicsScene(this);
	ui.QGV_rightDeviation->setScene(scene_right_deviation);
	graph_right_deviation = new graph_painter(ui.QGV_rightDeviation->height(), ui.QGV_rightDeviation->width());
	scene_right_deviation->addItem(graph_right_deviation);


	std::vector<float> leftHand_flexactionExtension;
	std::vector<float> leftHand_Deviation;
	std::vector<float> rightHand_flexactionExtension;
	std::vector<float> rightHand_Deviation;

	for (int i = 0; i < g_myDoc.arrayOfFramedata.size(); i++)
	{
		leftHand_flexactionExtension.push_back(g_myDoc.arrayOfFramedata.at(i).leftHand.flexation);
		leftHand_Deviation.push_back(g_myDoc.arrayOfFramedata.at(i).leftHand.deviation);
		rightHand_flexactionExtension.push_back(g_myDoc.arrayOfFramedata.at(i).rightHand.flexation);
		rightHand_Deviation.push_back(g_myDoc.arrayOfFramedata.at(i).rightHand.deviation);
	}

	graph_left_flex->updateData(leftHand_flexactionExtension);
	graph_left_deviation->updateData(leftHand_Deviation);
	graph_right_flex->updateData(rightHand_flexactionExtension);
	graph_right_deviation->updateData(rightHand_Deviation);



}

myDialog::~myDialog()
{

}


void myDialog::debugDisplay(QString text)
{
	ui.debugOut->moveCursor(QTextCursor::End);
	ui.debugOut->insertPlainText(text);
	ui.debugOut->moveCursor(QTextCursor::End);

	//ui.debugOut->appendPlainText(text);
}


void myDialog::displayInfo()
{
	char str[350];
	std::sprintf(str, "\n\n =====================================\n Vector Length=%d | myDoc.numberOfFrames=%d\n=====================================\n", myDoc.arrayOfFramedata.size(), myDoc.numberOfFrames);
	debugDisplay(str);
	for (int i = 0; i < myDoc.arrayOfFramedata.size(); i++)
	{
		std::sprintf(str, "\nframeNumber = %d\n leftHand.pointsDetected = %d \n leftHand.detected %s \n rightHand.pointsDetected = %d \n rightHand.detected %s \n time = %f\n", myDoc.arrayOfFramedata[i].frameNumber, myDoc.arrayOfFramedata[i].leftHand.pointsDetected, myDoc.arrayOfFramedata[i].leftHand.detected ? "TRUE" : "FALSE", myDoc.arrayOfFramedata[i].rightHand.pointsDetected, myDoc.arrayOfFramedata[i].rightHand.detected ? "TRUE" : "FALSE", myDoc.arrayOfFramedata[i].time);
		debugDisplay(str);
		qApp->processEvents();
	}
}

void myDialog::emptyFuinc()
{/*

	}*/
}
