#pragma once

#include <QTableWidget>


class hand_status
{
public:
	hand_status(QTableWidget *table);
	~hand_status();

	void detection_status(bool status);
	void tracking_status(bool status);
	void calibration_status(bool status);

protected:
	QTableWidget *table;
	QTableWidgetItem *cell_detect_status;
	QTableWidgetItem *cell_tracking_status;
	QTableWidgetItem *cell_calibration_status;
};

