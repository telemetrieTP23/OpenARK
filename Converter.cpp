#include "Converter.h"


/***
Returns the next frame if next frame is recorded
Returns the previous frame if next frame is not recorded
***/
void Converter::ConvertPXCImageToOpenCVMat(Intel::RealSense::Image *inImg, Intel::RealSense::ImageData data, cv::Mat *outImg) {
	auto cvDataType = 0;
	auto cvDataWidth = 0;

	auto imgInfo = inImg->QueryInfo();

	switch (data.format) 
	{
		/* STREAM_TYPE_COLOR */
	case Intel::RealSense::Image::PIXEL_FORMAT_YUY2: /* YUY2 image  */
	case Intel::RealSense::Image::PIXEL_FORMAT_NV12: /* NV12 image */
		throw; // Not implemented
	case Intel::RealSense::Image::PIXEL_FORMAT_RGB32:  /* BGRA layout on a little-endian machine */
		cvDataType = CV_8UC4;
		cvDataWidth = 4;
		break;
	case Intel::RealSense::Image::PIXEL_FORMAT_RGB24:  /* BGR layout on a little-endian machine */
		cvDataType = CV_8UC3;
		cvDataWidth = 3;
		break;
	case Intel::RealSense::Image::PIXEL_FORMAT_Y8: /* 8-Bit Gray Image, or IR 8-bit */
		cvDataType = CV_8U;
		cvDataWidth = 1;
		break;
		/* STREAM_TYPE_DEPTH */
	case Intel::RealSense::Image::PIXEL_FORMAT_DEPTH:  /* 16-bit unsigned integer with precision mm. */
	case Intel::RealSense::Image::PIXEL_FORMAT_DEPTH_RAW: /* 16-bit unsigned integer with device specific precision (call device->QueryDepthUnit()) */
		cvDataType = CV_16U;
		cvDataWidth = 2;
		break;
	case Intel::RealSense::Image::PIXEL_FORMAT_DEPTH_F32: /* 32-bit float-point with precision mm. */
		cvDataType = CV_32F;
		cvDataWidth = 4;
		break;
		/* STREAM_TYPE_IR */
	case Intel::RealSense::Image::PIXEL_FORMAT_Y16: /* 16-Bit Gray Image */
		cvDataType = CV_16U;
		cvDataWidth = 2;
		break;
	case Intel::RealSense::Image::PIXEL_FORMAT_Y8_IR_RELATIVE: /* Relative IR Image */
		cvDataType = CV_8U;
		cvDataWidth = 1;
		break;
	default: 
		break;
	}

	// suppose that no other planes
	if (data.planes[1] != nullptr) throw; // not implemented
										  // suppose that no sub pixel padding needed
	if (data.pitches[0] % cvDataWidth != 0) throw; // not implemented

	outImg->create(imgInfo.height, data.pitches[0] / cvDataWidth, cvDataType);

	//memcpy(outImg->data, data.planes[0], imgInfo.height*imgInfo.width*cvDataWidth * sizeof(pxcBYTE));
	memcpy(outImg->data, data.planes[0], imgInfo.height*imgInfo.width*cvDataWidth * sizeof(uint8_t));
}

