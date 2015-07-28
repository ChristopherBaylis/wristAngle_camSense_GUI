#include "empty_window.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QCoreApplication>
#include <QHeaderView>
#include <QMessageBox>

empty_window::empty_window(QWidget *parent)
: QWidget(parent)
{
	ui.setupUi(this);


	
}

empty_window::~empty_window()
{

}