#ifndef MYDIALOG_H
#define MYDIALOG_H

#include <QDialog>
#include "ui_mydialog.h"

class myDialog : public QDialog
{
	Q_OBJECT

public:
	myDialog(int *x, QWidget *parent = 0);
	~myDialog();

private:
	Ui::Dialog_myDialog ui;
};

#endif // MYDIALOG_H
