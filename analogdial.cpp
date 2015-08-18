#include "analogdial.h"


AnalogDial::AnalogDial(int l_height, int l_width)
{
	height = l_height - 10;
	width = l_width - 10;
	thetaDeg = thetaRad = 0;
    setFlag(ItemIsMovable);
} 

QRectF AnalogDial::boundingRect() const
{
    return QRectF(0,0,width,height);
}

void AnalogDial::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) 
{
	// 0 degrees will point right horrizontal.
	float needleRadians = thetaRad - M_PI_2;

	static const float	totalDegrees	= 150;
	static const float	offset			= (180 - totalDegrees) / 2;
	static const float	r				= height;
	static const int	cx				= width/ 2;
	static const int	cy				= height;

	QRectF rec = boundingRect();
	QBrush brush(Qt::blue);
	brush.setColor(Qt::blue);

	// Highlight bounding box
	painter->drawRect(rec);
	// Arcs
	painter->drawPie(0, 0, rec.width(), rec.height()*2, offset * 16, totalDegrees * 16);
	// Draw needle
	painter->drawLine(cx, cy, (r*cos(needleRadians)) + cx, (r*sin(needleRadians)) + cy);
	QString my_formatted_string = QString("Radian %1 Deg %2").arg(QString::number(thetaRad), QString::number(thetaDeg));
	painter->drawText(20, 20, my_formatted_string);
}

float AnalogDial::degreesToRadians(float degrees)
{
	return degrees * (M_PI / 180);
	// return degrees * 0.01745329251;
}
float AnalogDial::radiansToDegrees(float radians)
{
	return radians * (180 / M_PI);
	// return radians * 57.2957795131;
}

void AnalogDial::setDegrees(float degrees)
{
	thetaDeg = degrees;
	thetaRad = degreesToRadians(degrees);
	update();
}
void AnalogDial::setRadians(float radians)
{
	thetaRad = radians;
	thetaDeg = radiansToDegrees(radians);
	update();
}
