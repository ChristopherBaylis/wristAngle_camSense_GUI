// Definitly DOESN NOT need all those includes
// done in a hurry

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/opencv.hpp"

#include <windows.h>
#include <iostream>

#include "pxcsensemanager.h"
#include "pxcmetadata.h"
#include "service/pxcsessionservice.h"

#include "pxchanddata.h"
#include "pxchandconfiguration.h"

#include "handData_Definitions.h"
#include "pxccapture.h"

#include "pxcprojection.h"

#include <vector>

using namespace cv;

Mat PXCImageDataToCVMat(PXCImage *color);
void colorXY_to_realXYZ(PXCSenseManager *senseManager, PXCImage *depthIm, int **depthData, PXCPointF32 *colorXY, PXCPoint3DF32 *realXYZ, int nPoints);

std::vector<PXCPoint3DF32> depthXYZ_to_realXYZ(PXCSenseManager *senseManager, std::vector <PXCPoint3DF32> depthImCoords);

std::vector<PXCPointF32> colorXY_to_depthXY(PXCSenseManager *senseManager, PXCImage *depthIm, std::vector <PXCPointF32> pos_uv);

std::vector<PXCPointF32> depthXY_to_colorXY(PXCSenseManager *senseManager, std::vector <PXCPoint3DF32> vec_pos_uvz);


float points3DtoAngle(PXCPoint3DF32 A, PXCPoint3DF32 B, PXCPoint3DF32 C);

PXCPoint3DF32 midpoint3D(PXCPoint3DF32 A, PXCPoint3DF32 B);
PXCPoint3DF32 globalToLocalCoord(PXCPoint3DF32 B, PXCPoint3DF32 C, PXCPoint3DF32 E, PXCPoint3DF32 P);

PXCPoint3DF32 extendpoint3D(PXCPoint3DF32 A, PXCPoint3DF32 C);