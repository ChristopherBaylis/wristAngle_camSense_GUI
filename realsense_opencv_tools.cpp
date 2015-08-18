#include "realsense_opencv_tools.h"

using namespace cv;

Mat PXCImageDataToCVMat(PXCImage *color){

	int width = color->QueryInfo().width;
	int height = color->QueryInfo().height;
	PXCImage::ImageData * data = new PXCImage::ImageData;
	data->format = PXCImage::PIXEL_FORMAT_RGB32; // For example 4 bytes in this case (8 * 4 = 32)
	color->AcquireAccess(PXCImage::ACCESS_READ, data->format, data);

	int mat_height = height / 2;
	int mat_width = width / 2;
	cv::Mat frameColorInternal = cv::Mat::zeros(mat_height, mat_width, CV_8UC3);

	pxcBYTE *c_pixels = data->planes[0];
	int x = 0, y = 0, k=0;
	const int jumpRow = width * 4;

	for (y = 0; y < mat_height; y++)
	{
		for (x = 0; x < mat_width; x++)
		{
			k += 8;
			pxcBYTE bvalue = c_pixels[k + 0];
			pxcBYTE gvalue = c_pixels[k + 1];
			pxcBYTE rvalue = c_pixels[k + 2];
			unsigned int bvalueUchar = static_cast<unsigned int>(bvalue);
			unsigned int gvalueUchar = static_cast<unsigned int>(gvalue);
			unsigned int rvalueUchar = static_cast<unsigned int>(rvalue);


			frameColorInternal.at<cv::Vec3b>(y, x)[0] = (bvalueUchar);
			frameColorInternal.at<cv::Vec3b>(y, x)[1] = (gvalueUchar);
			frameColorInternal.at<cv::Vec3b>(y, x)[2] = (rvalueUchar);
		}
		k += jumpRow;
	}


	color->ReleaseAccess(data);
	delete data;

	return frameColorInternal;
}

void colorXY_to_realXYZ(PXCSenseManager *senseManager, PXCImage *depthIm, int **depthData, PXCPointF32 *colorXY, PXCPoint3DF32 *realXYZ, int nPoints)
{
	PXCCapture::Device *device;
	device = senseManager->QueryCaptureManager()->QueryDevice();
	PXCProjection *projection = device->CreateProjection();

	PXCPoint3DF32 *colorXYZ = new PXCPoint3DF32[nPoints];
	PXCPointF32 *depthXY = new PXCPointF32[nPoints];
	
	// Map XY of color to XY of depth. Store in depthXY
	projection->MapColorToDepth(depthIm, nPoints, colorXY, depthXY);
	
	// Get depth value and combine with XY of depth im. Store in colorXYZ[]
	for (int i = 0; i < nPoints; i++)
	{
		colorXYZ[i].x = colorXY[i].x;
		colorXYZ[i].y = colorXY[i].y;

		// Get depth value if a valid mapping exists
		if (depthXY[i].x > 0 && depthXY[i].y > 0)
			colorXYZ[i].z = depthData[(int)depthXY[i].x][(int)depthXY[i].y];
		else
			colorXYZ[i].z = -1;
	}

	// Map color to realword coords in mm
	projection->ProjectColorToCamera(1, colorXYZ, realXYZ);

	// Substitute in raw z value (in mm)
	// ProjectColorToCamera() uses a format for depth that I do not understand
	for (int i = 0; i < nPoints; i++)
		realXYZ[i].z = colorXYZ[i].z;

	// Clean up after ourselves
	delete colorXYZ;
	delete depthXY;
	projection->Release(); 
}

std::vector<PXCPointF32> colorXY_to_depthXY(PXCSenseManager *senseManager, PXCImage *depthIm, std::vector <PXCPointF32> vec_pos_ij)
{
	PXCCapture::Device *device;
	device = senseManager->QueryCaptureManager()->QueryDevice();
	PXCProjection *projection = device->CreateProjection();

	int nPoints = vec_pos_ij.size();

	PXCPointF32 *arr_pos_ij = new PXCPointF32[nPoints];
	PXCPointF32 *arr_pos_uv = new PXCPointF32[nPoints];

	for (int i=0; i < nPoints; i++)
		arr_pos_ij[i] = vec_pos_ij.at(i);

	pxcStatus status = projection->MapColorToDepth(depthIm, nPoints, arr_pos_ij, arr_pos_uv);

	projection->Release();

	std::vector<PXCPointF32> vec_pos_uv;
	for (int i=0; i < nPoints; i++)
		vec_pos_uv.push_back(arr_pos_uv[i]);

	return vec_pos_uv;
}

std::vector<PXCPointF32> depthXY_to_colorXY(PXCSenseManager *senseManager, std::vector <PXCPoint3DF32> vec_pos_uvz)
{
	PXCCapture::Device *device;
	device = senseManager->QueryCaptureManager()->QueryDevice();
	PXCProjection *projection = device->CreateProjection();

	int nPoints = vec_pos_uvz.size();

	PXCPointF32 *arr_pos_ij = new PXCPointF32[nPoints];
	PXCPoint3DF32 *arr_pos_uvz = new PXCPoint3DF32[nPoints];

	for (int i = 0; i < nPoints; i++)
		arr_pos_uvz[i] = vec_pos_uvz.at(i);

	pxcStatus status = projection->MapDepthToColor(nPoints, arr_pos_uvz, arr_pos_ij);

	projection->Release();

	std::vector<PXCPointF32> vec_pos_ij;
	for (int i = 0; i < nPoints; i++)
		vec_pos_ij.push_back(arr_pos_ij[i]);

	return vec_pos_ij;
}

std::vector<PXCPoint3DF32> depthXYZ_to_realXYZ(PXCSenseManager *senseManager, std::vector <PXCPoint3DF32> depthImCoords)
{
	PXCCapture::Device *device;
	device = senseManager->QueryCaptureManager()->QueryDevice();
	PXCProjection *projection = device->CreateProjection();

	int nPoints = depthImCoords.size();
	PXCPoint3DF32 *pos_uvz = new PXCPoint3DF32[nPoints];
	PXCPoint3DF32 *pos3d = new PXCPoint3DF32[nPoints];

	for (int i = 0; i != depthImCoords.size(); i++)
		pos_uvz[i] = depthImCoords.at(i);
	
	pxcStatus status = projection->ProjectDepthToCamera(nPoints, pos_uvz, pos3d);
	
	std::vector<PXCPoint3DF32> realCoord(nPoints); // ### Detect points via mapped subtracted image.
	for (int i = 0; i < nPoints; i++)
	{
		pos3d[i].z = depthImCoords.at(i).z;
		realCoord.push_back(pos3d[i]);
	}

	projection->Release();

	return realCoord;
}

float points3DtoAngle(PXCPoint3DF32 A, PXCPoint3DF32 B, PXCPoint3DF32 C)
{
	// implements the following formular.
	// acos(dot(normalize(AB), normalize(BC)))
	
	PXCPoint3DF32 v1, v2;
	// Vector BA
	v1.x = A.x - B.x; v1.y = A.y - B.y; v1.z = A.z - B.z;
	// Vctor BC
	v2.x = C.x - B.x; v2.y = C.y - B.y; v2.z = C.z - B.z;

	// The dot product of v1 and v2 is a function of the cosine of the angle between them(it's scaled by the product of their magnitudes). So first normalize v1 and v2:
	float v1mag = sqrt(v1.x * v1.x + v1.y * v1.y + v1.z * v1.z);
	PXCPoint3DF32 v1norm;
	v1norm.x = v1.x / v1mag; v1norm.y = v1.y / v1mag; v1norm.z = v1.z / v1mag;

	float v2mag = sqrt(v2.x * v2.x + v2.y * v2.y + v2.z * v2.z);
	PXCPoint3DF32 v2norm;
	v2norm.x = v2.x / v2mag; v2norm.y = v2.y / v2mag; v2norm.z = v2.z / v2mag;

	// Then calculate the dot product:
	float res = v1norm.x * v2norm.x + v1norm.y * v2norm.y + v1norm.z * v2norm.z;

	// And finally, recover the angle:
	float angle = acos(res);

	return angle;
}

PXCPoint3DF32 midpoint3D(PXCPoint3DF32 A, PXCPoint3DF32 B)
{
	PXCPoint3DF32 midpoint;

	midpoint.x = A.x + B.x / 2;
	midpoint.y = A.y + B.y / 2;
	midpoint.z = A.z + B.z / 2;

	return midpoint;

	// if midpoint C = ((A + B) / 2) then
	// B = 2C - A
}

PXCPoint3DF32 extendpoint3D(PXCPoint3DF32 A, PXCPoint3DF32 C)
{
	PXCPoint3DF32 extendedPoint;

	extendedPoint.x = (C.x * 2) - A.x;
	extendedPoint.y = (C.y * 2) - A.y;
	extendedPoint.z = (C.z * 2) - A.z;

	return extendedPoint;
}


PXCPoint3DF32 globalToLocalCoord(PXCPoint3DF32 B, PXCPoint3DF32 C, PXCPoint3DF32 E, PXCPoint3DF32 P)
{
	float B_x = B.x; float B_y = B.y; float B_z = B.z;
	float C_x = C.x; float C_y = C.y; float C_z = C.z;
	float E_x = E.x; float E_y = E.y; float E_z = E.z;

	// 1)
	float d_BC = sqrt(pow((B_x - C_x), 2) + pow((B_y - C_x), 2) + pow((B_z - C_z), 2));
	float i_x = (C_x - B_x) / d_BC;
	float i_y = (C_y - B_y) / d_BC;
	float i_z = (C_z - B_z) / d_BC;

	// 2)
	float d_BE = sqrt(pow((B_x - E_x), 2) + pow((B_y - E_x), 2) + pow((B_z - E_z), 2));
	float j_x = (E_x - B_x) / d_BE;
	float j_y = (E_y - B_y) / d_BE;
	float j_z = (E_z - B_z) / d_BE;

	// 3)
	float k_x = i_y*j_z - i_z*j_y;
	float k_y = i_z*j_x - i_x*j_z;
	float k_z = i_x*j_y - i_y*j_x;
	
	//global P
	float x_P = P.x; float y_P = P.y; float z_P = P.z;

	float i_P = i_x*(x_P - B_x) + i_y*(y_P - B_y) + i_z*(z_P - B_z);
	float j_P = j_x*(x_P - B_x) + j_y*(y_P - B_y) + j_z*(z_P - B_z);
	float k_P = k_x*(x_P - B_x) + k_y*(y_P - B_y) + k_z*(z_P - B_z);

	// COnceptually it is composed of i,j,k - in practice this structure is x,y,z
	PXCPoint3DF32 local_P;
	local_P.x = i_P;
	local_P.y = j_P;
	local_P.z = k_P;

	return local_P;
}


