#ifndef TEST_VID_H
#define TEST_VID_H
#include <QPainter>
#include <QGraphicsItem>
#include <QDebug>

#include <Qgraphicsview>

#define _USE_MATH_DEFINES
#include <math.h>

class test_vid : public QGraphicsItem
{
public:
	test_vid(int height, int width);
	void updateAndPaint(int **arr, float **arr2, int nHands, int arr2Size, char type);
protected:
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	QRectF boundingRect() const;
	
	int **dd;
	float **jd;
	int nHands;
	int nPoints;
	char g_type;

	int height, width;

	bool dataYet;

	QImage *g_image2;
};

#endif // TEST_VID_H
