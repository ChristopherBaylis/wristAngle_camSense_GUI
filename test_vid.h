#ifndef TEST_VID_H
#define TEST_VID_H
#include <QPainter>
#include <QGraphicsItem>
#include <QDebug>

#include <Qgraphicsview>

#define _USE_MATH_DEFINES
#include <math.h>

//PXCPointF32
#include "pxcsensemanager.h"

class test_vid : public QGraphicsItem
{
public:
	test_vid(int height, int width);
	void updateAndPaint(int **arr, char type, std::vector<PXCPointF32> points);
protected:
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	QRectF boundingRect() const;
	
	int dd[640][480];
	float **jd;
	int nHands;
	int g_nPoints;
	char g_type;

	int height, width;

	bool dataYet;

	QImage *g_image2;

	std::vector <PXCPointF32> g_points;
};

#endif // TEST_VID_H
