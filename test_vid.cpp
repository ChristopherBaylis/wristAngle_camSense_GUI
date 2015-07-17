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

		
		int x, y;
		for (int i = 0; i < nHands; i++)
		{
			if (jd[i][14] == 1)
				painter->setBrush(Qt::green);
			else if (jd[i][14] == 2)
				painter->setBrush(Qt::blue);
			else
				painter->setBrush(Qt::black);
			for (int j = 0; j < nPoints; j++)
			{
				x = jd[i][j*2+0];
				y = jd[i][j*2+1];
				painter->drawEllipse(x-3, y-3, 6, 6);
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

void test_vid::updateAndPaint(int **arr, float  **arr2, int arrSize, int arr2Size, char type)
{
	dataYet = true;
	dd = arr;
	jd = arr2;
	nHands = arrSize;
	nPoints = arr2Size;
	g_type = type;

	update();
}