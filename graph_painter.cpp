#include "graph_painter.h"


graph_painter::graph_painter(int l_height, int l_width)
{
	height = l_height - 30;
	width = l_width - 30;
	setFlag(ItemIsMovable);
	update();
}

QRectF graph_painter::boundingRect() const
{
	return QRectF(0, 0, width, height);
}

QRectF graph_painter::graphRect()
{
	QRectF r_boundingRect = boundingRect();
	return QRectF(0, 0, width, r_boundingRect.height()-10);
}

QRectF graph_painter::colorBarRect()
{
	QRectF r_boundingRect = boundingRect();
	QRectF r_graphRect = graphRect();
	return QRectF(0, r_graphRect.y() + r_graphRect.height() + 5, width, 10);
}

void graph_painter::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	QRectF r_boundingRect = boundingRect();
	QRectF r_graphRect = graphRect();
	QRectF r_colorBarRect = colorBarRect();

	float max_flexaction_degrees = 45;
	float max_extension_degrees = 45;

	float max_radialDeviation_degrees = 25;
	float max_ulnarDeviation_degrees = 25;

	int graphHeight_pix = r_graphRect.height();
	int graphWidth_pix = r_graphRect.width();
	int graph_mid_y = r_graphRect.y() + (graphHeight_pix / 2);

	float flexationExtension_scale = graphHeight_pix / (max_flexaction_degrees + max_extension_degrees);
	float deviation_scale = graphHeight_pix / (max_radialDeviation_degrees + max_ulnarDeviation_degrees);

	float widthScale = (float)graphWidth_pix / (float)g_angles.size();

	QBrush blueBrush(Qt::blue);
	QBrush redBrush(Qt::red);
	QBrush greenBrush(Qt::green);
	QPen bluePen(Qt::blue);
	QPen redPen(Qt::red);
	QPen greenPen(Qt::green);
	QPen smallBlack(Qt::black);

	QColor myRed; myRed.setRgb(255, 0, 0, 20);
	QColor myGreen; myGreen.setRgb(0, 255, 0, 20);
	QBrush lightRed(myRed);
	QBrush lightGreen(myGreen);
	//painter->setPen(pen);

	// Highlight bounding box
	painter->setPen(smallBlack);
	painter->drawRect(r_boundingRect);
	painter->drawRect(r_graphRect);
	painter->drawRect(r_colorBarRect);



	for (int i = 0; i < g_angles.size(); i++)
	{
		static int p1_x, p1_y;
		int p2_x, p2_y;

		float flexationExtension_degree = g_angles.at(i);//myDoc.arrayOfFramedata.at(i).leftHand.flexation;
		//float deviation_degree = myDoc.arrayOfFramedata.at(i).leftHand.deviation;

		p2_x = widthScale * i;
		p2_y = graph_mid_y + (flexationExtension_degree * flexationExtension_scale);

		if (i > 0)
		{
			painter->setPen(smallBlack);
			painter->drawLine(p1_x, p1_y, p2_x, p2_y);


		}


		if (flexationExtension_degree > 22 || flexationExtension_degree < -22)
		{
			painter->setPen(redPen); painter->setBrush(redBrush);
		}
		else
		{
			painter->setPen(greenPen); painter->setBrush(greenBrush);
		}

		painter->drawRect(i*widthScale, r_colorBarRect.y(), widthScale, r_colorBarRect.height());

		painter->drawEllipse(p2_x, p2_y, 3, 3);

		p1_x = p2_x;
		p1_y = p2_y;
	}

	// Midline on graph
	painter->setPen(smallBlack);
	painter->drawLine(0, graph_mid_y, graphWidth_pix, graph_mid_y);
	// Upper threshold line
	painter->setPen(redPen);
	int liney = graph_mid_y + (22 * flexationExtension_scale);
	painter->drawLine(0, liney, graphWidth_pix, liney);
	// Lower threshold line
	liney = graph_mid_y + (-22 * flexationExtension_scale);
	painter->drawLine(0, liney, graphWidth_pix, liney);

	painter->setBrush(lightRed);
	painter->drawRect(r_graphRect.x(), r_graphRect.y(), r_graphRect.width(), r_graphRect.height()/4);
	painter->drawRect(r_graphRect.x(), r_graphRect.y() + ((r_graphRect.height() / 4) * 3), r_graphRect.width(), r_graphRect.height() / 4);
	painter->setBrush(lightGreen);
	painter->drawRect(r_graphRect.x(), r_graphRect.y() + (r_graphRect.height() / 4), r_graphRect.width(), r_graphRect.height() / 2);
}

/*void graph_painter::updateData(document g_myDoc)
{
	myDoc = g_myDoc;
	update();
}*/

void graph_painter::updateData(std::vector<float> angles)
{
	g_angles = angles;
	update();
}