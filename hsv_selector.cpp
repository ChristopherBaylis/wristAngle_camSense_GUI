#include "hsv_selector.h"


HSV_Selector::HSV_Selector(int *Hmin, int *Smin, int *Vmin, int *Hmax, int *Smax, int *Vmax, QWidget *parent)
: QDialog(parent)
{
	ui.setupUi(this);

	g_Hmin = Hmin;
	g_Smin = Smin;
	g_Vmin = Vmin;
	g_Hmax = Hmax;
	g_Smax = Smax;
	g_Vmax = Vmax;

	*g_Hmin = *g_Hmin + 1;

	ui.spinBox_Hue_min->setValue(*g_Hmin);
	ui.spinBox_Saturation_min->setValue(*g_Smin);
	ui.spinBox_Value_min->setValue(*g_Vmin);

	ui.spinBox_Hue_max->setValue(*g_Hmax);
	ui.spinBox_Saturation_max->setValue(*g_Smax);
	ui.spinBox_Value_max->setValue(*g_Vmax);

	connect(ui.horizontalSlider_Hue_min, SIGNAL(valueChanged(int)), this, SLOT(f_Hmin(int)));
	connect(ui.horizontalSlider_Saturation_min, SIGNAL(valueChanged(int)), this, SLOT(f_Smin(int)));
	connect(ui.horizontalSlider_Value_min, SIGNAL(valueChanged(int)), this, SLOT(f_Vmin(int)));

	connect(ui.horizontalSlider_Hue_max, SIGNAL(valueChanged(int)), this, SLOT(f_Hmax(int)));
	connect(ui.horizontalSlider_Saturation_max, SIGNAL(valueChanged(int)), this, SLOT(f_Smax(int)));
	connect(ui.horizontalSlider_Value_max, SIGNAL(valueChanged(int)), this, SLOT(f_Vmax(int)));
}

HSV_Selector::~HSV_Selector()
{

}

void HSV_Selector::f_Hmin(int val) {
	*g_Hmin = val;
}
void HSV_Selector::f_Smin(int val) {
	*g_Smin = val;
}
void HSV_Selector::f_Vmin(int val) {
	*g_Vmin = val;
}

void HSV_Selector::f_Hmax(int val) {
	*g_Hmax = val;
}
void HSV_Selector::f_Smax(int val) {
	*g_Smax = val;
}
void HSV_Selector::f_Vmax(int val) {
	*g_Vmax = val;
}