#ifndef ANALOGDIAL_H
#define ANALOGDIAL_H
#include <QPainter>
#include <QGraphicsItem>
#include <QDebug>

#include <Qgraphicsview>

#define _USE_MATH_DEFINES
#include <math.h>

class AnalogDial : public QGraphicsItem
{
public:
	AnalogDial(int height, int width);

	void setDegrees(float degrees);
	void setRadians(float radians);
protected:
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	QRectF boundingRect() const;

	float AnalogDial::degreesToRadians(float degrees);
	float AnalogDial::radiansToDegrees(float radians);

	float thetaDeg;
	float thetaRad;

	int height, width;
};

#endif // ANALOGDIAL_H
