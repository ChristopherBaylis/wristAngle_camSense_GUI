#include "mydialog.h"

myDialog::myDialog(int *x, QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	ui.spinBox_Area->setValue(*x);
	*x = *x + 1;
	ui.spinBox_maxThreshold->setValue(*x);
	//ui.horizontalSlider_Area->valueChanged()
}

myDialog::~myDialog()
{

}
