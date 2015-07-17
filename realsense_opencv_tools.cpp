#include "realsense_opencv_tools.h"

using namespace cv;

Mat PXCImageDataToCVMat(PXCImage *color){

	int width = color->QueryInfo().width;
	int height = color->QueryInfo().height;
	PXCImage::ImageData * data = new PXCImage::ImageData;
	data->format = PXCImage::PIXEL_FORMAT_RGB32; // For example 4 bytes in this case (8 * 4 = 32)
	color->AcquireAccess(PXCImage::ACCESS_READ, data->format, data);

	cv::Mat frameColorInternal = cv::Mat::zeros(height, width, CV_8UC3);

	pxcBYTE *c_pixels = data->planes[0];
	int x = 0, y = 0;

	for (int i = 0; i < width * height; i++)
	{
		//BGRA
		//Read the first 3 bytes of each 4 bytes, and ignore the last one
		pxcBYTE bvalue = c_pixels[i * 4 + 0];
		pxcBYTE gvalue = c_pixels[i * 4 + 1];
		pxcBYTE rvalue = c_pixels[i * 4 + 2];
		unsigned int bvalueUchar = static_cast<unsigned int>(bvalue);
		unsigned int gvalueUchar = static_cast<unsigned int>(gvalue);
		unsigned int rvalueUchar = static_cast<unsigned int>(rvalue);

		frameColorInternal.at<cv::Vec3b>(y, x)[0] = (bvalueUchar);
		frameColorInternal.at<cv::Vec3b>(y, x)[1] = (gvalueUchar);
		frameColorInternal.at<cv::Vec3b>(y, x)[2] = (rvalueUchar);
		//Advance, one pixel at a time (can be done better of course!)
		x += 1;
		if (x >= width)
		{
			x = 0;
			y += 1;
		}
	}

	return frameColorInternal;
}