#include <vector>
#include "pxcsensemanager.h"

#ifndef  MYDOCUMENT
#define  MYDOCUMENT
struct pointDataV1
{
	PXCPoint3DF32 A;
	PXCPoint3DF32 B;
	PXCPoint3DF32 C;
	PXCPoint3DF32 E;
	PXCPoint3DF32 F;

	//const std::string description = "A = forarm, B = center wrist computed from midpoint of E anf F. C = 3rd metacarpus meets proximal phalange E = left wrist, F = right Wrist";
};

struct handDataV1
{
	int pointsDetected;
	bool detected;
	float flexation;
	float deviation;
	//pointDataV1 pointData;

	//const std::string description = "POINTSDETECTED: number of points on hand that are detected, DETECTED: TRUE if all pointsDetected is 5, FLEXATION: if positive degree of wrist flexation, if negative degree of extension, DEVIATION: if positive degree of ulnar deviation, if negative degree of radial deviation, POINTDATA: coordinates of detected points.";
};
struct frameDataV1
{
	int frameNumber;
	float time;
	handDataV1 leftHand;
	handDataV1 rightHand;
};

struct document
{
	//std::string name;
	//std::string frameDataFormat;

	time_t time;
	float runtime_seconds;
	int numberOfFrames;

	std::vector<frameDataV1> arrayOfFramedata;
};

struct HSV
{
	int H; int S; int V;
};
struct HSV_Range
{
	HSV max; HSV min;
};

#endif