#ifndef MYDIALOG_H
#define MYDIALOG_H

#include <QDialog>
#include "ui_mydialog.h"
//#include "realsense_opencv_tools.h"
#include "myDocument.h"

#include "graph_painter.h"


class myDialog : public QDialog
{
	Q_OBJECT

public:
	myDialog(document g_myDoc, QWidget *parent = 0);
	~myDialog();

private:
	Ui::Dialog_myDialog ui;
	void myDialog::debugDisplay(QString text);

	void myDialog::emptyFuinc();
	void myDialog::displayInfo();


	document myDoc;


	QGraphicsScene *scene_left_flex; graph_painter *graph_left_flex;
	QGraphicsScene *scene_left_deviation; graph_painter *graph_left_deviation;
	QGraphicsScene *scene_right_flex; graph_painter *graph_right_flex;
	QGraphicsScene *scene_right_deviation; graph_painter *graph_right_deviation;
};

#endif // MYDIALOG_H
