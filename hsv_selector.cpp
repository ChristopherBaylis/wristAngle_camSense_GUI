#include "hsv_selector.h"


HSV_Selector::HSV_Selector(HSV_Range *range_1, HSV_Range *range_2, QWidget *parent)
: QDialog(parent)
{
	ui.setupUi(this);
	
	g_Hmin = &range_1->min.H;
	g_Smin = &range_1->min.S;
	g_Vmin = &range_1->min.V;
	g_Hmax = &range_1->max.H;
	g_Smax = &range_1->max.S;
	g_Vmax = &range_1->max.V;

	*g_Hmin = *g_Hmin + 1;

	HSV_Selector::update_values();

	connect(ui.horizontalSlider_Hue_min, SIGNAL(valueChanged(int)), this, SLOT(f_Hmin(int)));
	connect(ui.horizontalSlider_Saturation_min, SIGNAL(valueChanged(int)), this, SLOT(f_Smin(int)));
	connect(ui.horizontalSlider_Value_min, SIGNAL(valueChanged(int)), this, SLOT(f_Vmin(int)));

	connect(ui.horizontalSlider_Hue_max, SIGNAL(valueChanged(int)), this, SLOT(f_Hmax(int)));
	connect(ui.horizontalSlider_Saturation_max, SIGNAL(valueChanged(int)), this, SLOT(f_Smax(int)));
	connect(ui.horizontalSlider_Value_max, SIGNAL(valueChanged(int)), this, SLOT(f_Vmax(int)));
}

void HSV_Selector::update_values()
{
	ui.spinBox_Hue_min->setValue(*g_Hmin);
	ui.spinBox_Saturation_min->setValue(*g_Smin);
	ui.spinBox_Value_min->setValue(*g_Vmin);

	ui.spinBox_Hue_max->setValue(*g_Hmax);
	ui.spinBox_Saturation_max->setValue(*g_Smax);
	ui.spinBox_Value_max->setValue(*g_Vmax);
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