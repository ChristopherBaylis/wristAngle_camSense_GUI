#ifndef GRAPH_PAINTER_H
#define GRAPH_PAINTER_H
#include <QPainter>
#include <QGraphicsItem>
#include <QDebug>

#include <Qgraphicsview>

#define _USE_MATH_DEFINES
#include <math.h>

#include "myDocument.h"
#include <vector>

class graph_painter : public QGraphicsItem
{
public:
	graph_painter(int height, int width);

	//void updateData(document g_myDoc);
	void updateData(std::vector<float> angles);
protected:
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	QRectF boundingRect() const;
	QRectF graphRect();
	QRectF colorBarRect();

	int height, width; 

	//document myDoc;
	std::vector<float> g_angles;
};

#endif // ANALOGDIAL_H
