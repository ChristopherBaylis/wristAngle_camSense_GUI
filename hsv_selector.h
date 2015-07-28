#ifndef HSV_SELECTOR_H
#define HSV_SELECTOR_H

#include <QDialog>
#include "ui_HSV_Selector.h"

class HSV_Selector : public QDialog
{
	Q_OBJECT

public slots:
	void f_Hmin(int);
	void f_Smin(int);
	void f_Vmin(int);
	void f_Hmax(int);
	void f_Smax(int);
	void f_Vmax(int);

public:
	HSV_Selector(int *Hmin, int *Smin, int *Vmin, int *Hmax, int *Smax, int *Vmax, QWidget *parent = 0);
	~HSV_Selector();

private:
	Ui::Dialog ui;

	int *g_Hmin;
	int *g_Smin;
	int *g_Vmin;
	int *g_Hmax;
	int *g_Smax;
	int *g_Vmax;
};


#endif // HSV_SELECTOR_H
