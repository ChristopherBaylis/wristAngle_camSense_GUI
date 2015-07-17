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

using namespace cv;

Mat PXCImageDataToCVMat(PXCImage *color);