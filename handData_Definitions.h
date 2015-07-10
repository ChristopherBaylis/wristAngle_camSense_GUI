/*******************************************************************************

INTEL CORPORATION PROPRIETARY INFORMATION
This software is supplied under the terms of a license agreement or nondisclosure
agreement with Intel Corporation and may not be copied or disclosed except in
accordance with the terms of that agreement
Copyright(c) 2011-2015 Intel Corporation. All Rights Reserved.

*******************************************************************************/
#pragma once
#include <iostream>
#include "pxcsensemanager.h"
#include "pxchanddata.h"
#include "pxchandconfiguration.h"

namespace Definitions
{
	static std::string appName = "";

	inline void WriteHelpMessage()
	{
		std::cout << "Usage in live streaming: " << appName.c_str() << " -live [optional parameters... (see below)] \n\n";
		std::cout << "Usage in sequence streaming: " << appName.c_str() << " -seq <seqPath> [optional parameters... (see below)] \n\n";

		std::cout << "Optional Parameters:\n\n";

		std::cout << "  -skeleton		Prints out joint world position information\n\n";
		std::cout << "  -alerts		Prints out fired alert information \n\n";
		std::cout << "  -gestures		Prints out fired gesture information \n\n";
	}

	inline const std::string AlertToString(PXCHandData::AlertType label)
	{
		std::string alertLabel = "";
		switch (label)
		{
		case PXCHandData::AlertType::ALERT_HAND_DETECTED:			    {alertLabel = "ALERT_HAND_DETECTED"; break; }
		case PXCHandData::AlertType::ALERT_HAND_NOT_DETECTED:			{alertLabel = "ALERT_HAND_NOT_DETECTED"; break; }
		case PXCHandData::AlertType::ALERT_HAND_TRACKED:				{alertLabel = "ALERT_HAND_TRACKED"; break; }
		case PXCHandData::AlertType::ALERT_HAND_NOT_TRACKED:			{alertLabel = "ALERT_HAND_NOT_TRACKED"; break; }
		case PXCHandData::AlertType::ALERT_HAND_CALIBRATED:			    {alertLabel = "ALERT_HAND_CALIBRATED"; break; }
		case PXCHandData::AlertType::ALERT_HAND_NOT_CALIBRATED:		    {alertLabel = "ALERT_HAND_NOT_CALIBRATED"; break; }
		case PXCHandData::AlertType::ALERT_HAND_OUT_OF_BORDERS:		    {alertLabel = "ALERT_HAND_OUT_OF_BORDERS"; break; }
		case PXCHandData::AlertType::ALERT_HAND_INSIDE_BORDERS:		    {alertLabel = "ALERT_HAND_INSIDE_BORDERS"; break; }
		case PXCHandData::AlertType::ALERT_HAND_OUT_OF_LEFT_BORDER:		{alertLabel = "ALERT_HAND_OUT_OF_LEFT_BORDER"; break; }
		case PXCHandData::AlertType::ALERT_HAND_OUT_OF_RIGHT_BORDER:	{alertLabel = "ALERT_HAND_OUT_OF_RIGHT_BORDER"; break; }
		case PXCHandData::AlertType::ALERT_HAND_OUT_OF_TOP_BORDER:		{alertLabel = "ALERT_HAND_OUT_OF_TOP_BORDER"; break; }
		case PXCHandData::AlertType::ALERT_HAND_OUT_OF_BOTTOM_BORDER:   {alertLabel = "ALERT_HAND_OUT_OF_BOTTOM_BORDER"; break; }
		case PXCHandData::AlertType::ALERT_HAND_TOO_FAR:				{alertLabel = "ALERT_HAND_TOO_FAR"; break; }
		case PXCHandData::AlertType::ALERT_HAND_TOO_CLOSE:			    {alertLabel = "ALERT_HAND_TOO_CLOSE"; break; }
		case PXCHandData::AlertType::ALERT_HAND_LOW_CONFIDENCE:			{alertLabel = "ALERT_HAND_LOW_CONFIDENCE"; break; }
		}
		return alertLabel;
	}

	inline const std::string JointToString(PXCHandData::JointType label)
	{
		std::string jointLabel = "";
		switch (label)
		{
		case PXCHandData::JointType::JOINT_WRIST:			        {jointLabel = "JOINT_WRIST"; break; }
		case PXCHandData::JointType::JOINT_CENTER:			        {jointLabel = "JOINT_CENTER"; break; }
		case PXCHandData::JointType::JOINT_THUMB_BASE:				{jointLabel = "JOINT_THUMB_BASE"; break; }
		case PXCHandData::JointType::JOINT_THUMB_JT1:			    {jointLabel = "JOINT_THUMB_JT1"; break; }
		case PXCHandData::JointType::JOINT_THUMB_JT2:			    {jointLabel = "JOINT_THUMB_JT2"; break; }
		case PXCHandData::JointType::JOINT_THUMB_TIP:		        {jointLabel = "JOINT_THUMB_TIP"; break; }
		case PXCHandData::JointType::JOINT_INDEX_BASE:		        {jointLabel = "JOINT_INDEX_BASE"; break; }
		case PXCHandData::JointType::JOINT_INDEX_JT1:		        {jointLabel = "JOINT_INDEX_JT1"; break; }
		case PXCHandData::JointType::JOINT_INDEX_JT2:		        {jointLabel = "JOINT_INDEX_JT2"; break; }
		case PXCHandData::JointType::JOINT_INDEX_TIP:	            {jointLabel = "JOINT_INDEX_TIP"; break; }
		case PXCHandData::JointType::JOINT_MIDDLE_BASE:		        {jointLabel = "JOINT_MIDDLE_BASE"; break; }
		case PXCHandData::JointType::JOINT_MIDDLE_JT1:              {jointLabel = "JOINT_MIDDLE_JT1"; break; }
		case PXCHandData::JointType::JOINT_MIDDLE_JT2:				{jointLabel = "JOINT_MIDDLE_JT2"; break; }
		case PXCHandData::JointType::JOINT_MIDDLE_TIP:				{jointLabel = "JOINT_MIDDLE_TIP"; break; }
		case PXCHandData::JointType::JOINT_RING_BASE:				{jointLabel = "JOINT_RING_BASE"; break; }
		case PXCHandData::JointType::JOINT_RING_JT1:				{jointLabel = "JOINT_RING_JT1"; break; }
		case PXCHandData::JointType::JOINT_RING_JT2:				{jointLabel = "JOINT_RING_JT2"; break; }
		case PXCHandData::JointType::JOINT_RING_TIP:				{jointLabel = "JOINT_RING_TIP"; break; }
		case PXCHandData::JointType::JOINT_PINKY_BASE:				{jointLabel = "JOINT_PINKY_BASE"; break; }
		case PXCHandData::JointType::JOINT_PINKY_JT1:				{jointLabel = "JOINT_PINKY_JT1"; break; }
		case PXCHandData::JointType::JOINT_PINKY_JT2:				{jointLabel = "JOINT_PINKY_JT2"; break; }
		case PXCHandData::JointType::JOINT_PINKY_TIP:				{jointLabel = "JOINT_PINKY_TIP"; break; }
		}
		return jointLabel;
	}

	inline const pxcCHAR* GestureStateToString(PXCHandData::GestureStateType label)
	{
		pxcCHAR* gestureState = L"";
		switch (label)
		{
		case PXCHandData::GESTURE_STATE_START:			        {gestureState = L"GESTURE_STATE_START"; break; }
		case PXCHandData::GESTURE_STATE_IN_PROGRESS:			{gestureState = L"GESTURE_STATE_IN_PROGRESS"; break; }
		case PXCHandData::GESTURE_STATE_END:				    {gestureState = L"GESTURE_STATE_END"; break; }
		}
		return gestureState;
	}

	inline const std::string PixelFormatToString(PXCImage::PixelFormat label)
	{
		std::string formatLabel = "";
		switch (label)
		{
		case PXCImage::PixelFormat::PIXEL_FORMAT_ANY: { formatLabel = "PIXEL_FORMAT_ANY"; break; }
		case PXCImage::PixelFormat::PIXEL_FORMAT_YUY2: { formatLabel = "PIXEL_FORMAT_YUY2"; break; }
		case PXCImage::PixelFormat::PIXEL_FORMAT_NV12: { formatLabel = "PIXEL_FORMAT_NV12"; break; }
		case PXCImage::PixelFormat::PIXEL_FORMAT_RGB32: { formatLabel = "PIXEL_FORMAT_RGB32"; break; }
		case PXCImage::PixelFormat::PIXEL_FORMAT_RGB24: { formatLabel = "PIXEL_FORMAT_RGB24"; break; }
		case PXCImage::PixelFormat::PIXEL_FORMAT_Y8: { formatLabel = "PIXEL_FORMAT_Y8"; break; }
		case PXCImage::PixelFormat::PIXEL_FORMAT_DEPTH: { formatLabel = "PIXEL_FORMAT_DEPTH"; break; }
		case PXCImage::PixelFormat::PIXEL_FORMAT_DEPTH_RAW: { formatLabel = "PIXEL_FORMAT_DEPTH_RAW"; break; }
		case PXCImage::PixelFormat::PIXEL_FORMAT_DEPTH_F32: { formatLabel = "PIXEL_FORMAT_DEPTH_F32"; break; }
		case PXCImage::PixelFormat::PIXEL_FORMAT_Y16: { formatLabel = "PIXEL_FORMAT_Y16"; break; }
		case PXCImage::PixelFormat::PIXEL_FORMAT_Y8_IR_RELATIVE: { formatLabel = "PIXEL_FORMAT_Y8_IR_RELATIVE"; break; }
		}
		return formatLabel;
	}

	inline const std::string pxcStatusToString(pxcStatus label)
	{
		std::string pxcStatusString = "";
		switch (label)
		{
		case PXC_STATUS_NO_ERROR:						{ pxcStatusString = "PXC_STATUS_NO_ERROR"; break; }
		case PXC_STATUS_FEATURE_UNSUPPORTED:			{ pxcStatusString = "PXC_STATUS_FEATURE_UNSUPPORTED"; break; }
		case PXC_STATUS_PARAM_UNSUPPORTED:				{ pxcStatusString = "PXC_STATUS_PARAM_UNSUPPORTED"; break; }
		case PXC_STATUS_ITEM_UNAVAILABLE:				{ pxcStatusString = "PXC_STATUS_ITEM_UNAVAILABLE"; break; }
		case PXC_STATUS_HANDLE_INVALID:					{ pxcStatusString = "PXC_STATUS_HANDLE_INVALID"; break; }
		case PXC_STATUS_ALLOC_FAILED:					{ pxcStatusString = "PXC_STATUS_ALLOC_FAILED"; break; }
		case PXC_STATUS_DEVICE_FAILED:					{ pxcStatusString = "PXC_STATUS_DEVICE_FAILED"; break; }
		case PXC_STATUS_DEVICE_BUSY:					{ pxcStatusString = "PXC_STATUS_DEVICE_BUSY"; break; }
		case PXC_STATUS_EXEC_ABORTED:					{ pxcStatusString = "PXC_STATUS_EXEC_ABORTED"; break; }
		case PXC_STATUS_EXEC_INPROGRESS:				{ pxcStatusString = "PXC_STATUS_EXEC_INPROGRESS"; break; }
		case PXC_STATUS_EXEC_TIMEOUT:					{ pxcStatusString = "PXC_STATUS_EXEC_TIMEOUT"; break; }
		case PXC_STATUS_FILE_WRITE_FAILED:				{ pxcStatusString = "PXC_STATUS_FILE_WRITE_FAILED"; break; }
		case PXC_STATUS_FILE_READ_FAILED:				{ pxcStatusString = "PXC_STATUS_FILE_READ_FAILED"; break; }
		case PXC_STATUS_FILE_CLOSE_FAILED:				{ pxcStatusString = "PXC_STATUS_FILE_CLOSE_FAILED"; break; }
		case PXC_STATUS_DATA_UNAVAILABLE:				{ pxcStatusString = "PXC_STATUS_DATA_UNAVAILABLE"; break; }
		case PXC_STATUS_DATA_NOT_INITIALIZED:			{ pxcStatusString = "PXC_STATUS_DATA_NOT_INITIALIZED"; break; }
		case PXC_STATUS_INIT_FAILED:					{ pxcStatusString = "PXC_STATUS_INIT_FAILED"; break; }
		case PXC_STATUS_STREAM_CONFIG_CHANGED:			{ pxcStatusString = "PXC_STATUS_STREAM_CONFIG_CHANGED"; break; }
		case PXC_STATUS_POWER_UID_ALREADY_REGISTERED:	{ pxcStatusString = "PXC_STATUS_POWER_UID_ALREADY_REGISTERED"; break; }
		case PXC_STATUS_POWER_UID_NOT_REGISTERED:		{ pxcStatusString = "PXC_STATUS_POWER_UID_NOT_REGISTERED"; break; }
		case PXC_STATUS_POWER_ILLEGAL_STATE:			{ pxcStatusString = "PXC_STATUS_POWER_ILLEGAL_STATE"; break; }
		case PXC_STATUS_POWER_PROVIDER_NOT_EXISTS:		{ pxcStatusString = "PXC_STATUS_POWER_PROVIDER_NOT_EXISTS"; break; }
		case PXC_STATUS_CAPTURE_CONFIG_ALREADY_SET:		{ pxcStatusString = "PXC_STATUS_CAPTURE_CONFIG_ALREADY_SET"; break; }
		case PXC_STATUS_COORDINATE_SYSTEM_CONFLICT:		{ pxcStatusString = "PXC_STATUS_COORDINATE_SYSTEM_CONFLICT"; break; }
		case PXC_STATUS_TIME_GAP:						{ pxcStatusString = "PXC_STATUS_TIME_GAP"; break; }
		case PXC_STATUS_PARAM_INPLACE:					{ pxcStatusString = "PXC_STATUS_PARAM_INPLACE"; break; }
		case PXC_STATUS_DATA_NOT_CHANGED:				{ pxcStatusString = "PXC_STATUS_DATA_NOT_CHANGED"; break; }
		case PXC_STATUS_PROCESS_FAILED:					{ pxcStatusString = "PXC_STATUS_PROCESS_FAILED"; break; }
		case PXC_STATUS_VALUE_OUT_OF_RANGE:				{ pxcStatusString = "PXC_STATUS_VALUE_OUT_OF_RANGE"; break; }
		}
		return pxcStatusString;
	}
}