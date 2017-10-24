#pragma once
#include "stdafx.h"

//----------------------------------------
// TGMT mean: ThiGiacMayTinh
//----------------------------------------
class TGMTimage
{
public:
	//TGMTimage(void);
	//~TGMTimage(void);

	static cv::Mat ConvertToGray(cv::Mat matInput);

	static std::string GetImageType(cv::Mat);
};

