#include "hand_status.h"


hand_status::hand_status(QTableWidget *table_)
{
	table = table_;
	cell_detect_status = new QTableWidgetItem;
	cell_tracking_status = new QTableWidgetItem;
	cell_calibration_status = new QTableWidgetItem;

	detection_status(false);
}


hand_status::~hand_status()
{
}

void hand_status::detection_status(bool status)
{
	if (status == true)
	{
		cell_detect_status->setText("YES");
		cell_detect_status->setTextColor(Qt::green);
		cell_detect_status->setBackgroundColor(Qt::darkGreen);
	}
	else
	{
		cell_detect_status->setText("NO");
		cell_detect_status->setTextColor(Qt::red);
		cell_detect_status->setBackgroundColor(Qt::darkRed);
	}
	table->setItem(0, 0, cell_detect_status);
}

void hand_status::tracking_status(bool status)
{
	if (status == true)
	{
		cell_tracking_status->setText("GOOD");
		cell_tracking_status->setTextColor(Qt::green);
		cell_tracking_status->setBackgroundColor(Qt::darkGreen);
	}
	else
	{
		cell_tracking_status->setText("BAD");
		cell_tracking_status->setTextColor(Qt::yellow);
		cell_tracking_status->setBackgroundColor(Qt::darkYellow);
	}
	table->setItem(0, 1, cell_tracking_status);
}

void hand_status::calibration_status(bool status)
{
	if (status == true)
	{
		cell_calibration_status->setText("YES");
		cell_calibration_status->setTextColor(Qt::green);
		cell_calibration_status->setBackgroundColor(Qt::darkGreen);
	}
	else
	{
		cell_calibration_status->setText("NO");
		cell_calibration_status->setTextColor(Qt::yellow);
		cell_calibration_status->setBackgroundColor(Qt::darkYellow);
	}
	table->setItem(0, 2, cell_calibration_status);
}