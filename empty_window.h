#include <QtWidgets/QMainWindow>
#include "ui_empty_window.h"

#include <QTableWidget>

#include <QDialog>
#include <QtCore>
#include <QtGui>

class empty_window : public QWidget
{
	Q_OBJECT

public:
	empty_window(QWidget *parent = 0);
	~empty_window();


private:
	Ui::Form ui;

	public slots:

};