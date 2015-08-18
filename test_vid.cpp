#include "test_vid.h"


test_vid::test_vid(int l_height, int l_width)
{
	height = l_height - 10;
	width = l_width - 10;

	dataYet = false;

	g_image2 = new QImage(640, 480, QImage::Format_RGB32);
}

QRectF test_vid::boundingRect() const
{
	return QRectF(0, 0, width, height);
}

void test_vid::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	QRectF rec = boundingRect();
	QImage *image2;
	image2 = g_image2;
	//QImage image2(width, height, QImage::Format_RGB32);
	//image2(width, height, QImage::Format_RGB32);
	QRgb value2;

	if (dataYet)
	{
		dataYet = false;
		for (int x = 0; x < width; x++)
		{
			for (int y = 0; y < height; y++)
			{
				int val;
				int thisPixel = dd[x][y];

				
				switch (g_type)
				{
					case 'c':
					{
								image2->setPixel(x, y, thisPixel);
						break;
					}
					case 'd':
					{
						if (thisPixel < 5)
							val = 0;
						else
							val = (255 - (thisPixel / 4)) - 50;

						value2 = qRgb(val,val,val);
						image2->setPixel(x, y, value2);
					}
				}
			}
		}
		painter->drawImage(rec, *image2);

		if (g_points.size() > 0)
		{
			QBrush blueBrush(Qt::blue);
			painter->setBrush(blueBrush);
			std::vector < QString > label = {"A", "B", "C", "D", "E", "F"};
			for (int i = 0; i < g_points.size(); i++)
			{
				painter->drawEllipse((int)g_points[i].x, (int)g_points[i].y, 5, 5);
				painter->drawText(g_points.at(i).x, g_points.at(i).y, label.at(i));
			}
		}
	}
	else
	{
		QImage image(3, 3, QImage::Format_RGB32);
		QRgb value;

		value = qRgb(189, 149, 39); // 0xffbd9527
		image.setPixel(1, 1, value);

		value = qRgb(122, 163, 39); // 0xff7aa327
		image.setPixel(0, 1, value);
		image.setPixel(1, 0, value);

		value = qRgb(237, 187, 51); // 0xffedba31
		image.setPixel(2, 1, value);
		painter->drawImage(rec, image);
	}

	//image2.~QImage();
	
	
}

void test_vid::updateAndPaint(int **arr, char type, std::vector<PXCPointF32> points)
{
	// Get image data
	dataYet = true;
	//TODO: optimize
	for (int x = 0; x < 640; x++)
	{
		for (int y = 0; y < 480; y++)
		{
			dd[x][y] = arr[x][y];
		}
	}
	g_type = type;

	// get point data
	g_points = points;

	//Paint it (calls paint())
	update();
}