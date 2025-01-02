/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                           License Agreement
//                For Open Source Computer Vision Library
//
// Copyright (C) 2013, OpenCV Foundation, all rights reserved.
// Third party copyrights are property of their respective owners.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   * Redistribution's of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//   * Redistribution's in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
//   * The name of the copyright holders may not be used to endorse or promote products
//     derived from this software without specific prior written permission.
//
// This software is provided by the copyright holders and contributors "as is" and
// any express or implied warranties, including, but not limited to, the implied
// warranties of merchantability and fitness for a particular purpose are disclaimed.
// In no event shall the Intel Corporation or contributors be liable for any direct,
// indirect, incidental, special, exemplary, or consequential damages
// (including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//
//M*/

#ifndef __OPENCV_DNN_CAFFE_GLOG_EMULATOR_HPP__
#define __OPENCV_DNN_CAFFE_GLOG_EMULATOR_HPP__
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <opencv2/core.hpp>

#define CHECK(cond)     cv::GLogWrapper(__FILE__, CV_Func, __LINE__, "CHECK", #cond, cond)
#define CHECK_EQ(a, b)  cv::GLogWrapper(__FILE__, CV_Func, __LINE__, "CHECK", #a"="#b, ((a) == (b)))
#define LOG(TYPE)       cv::GLogWrapper(__FILE__, CV_Func, __LINE__, #TYPE)

namespace cv
{

class GLogWrapper
{
    std::stringstream stream;
    const char *file, *func, *type, *cond_str;
    int line;
    bool cond_staus;

public:

    GLogWrapper(const char *_file, const char *_func, int _line,
                const char *_type,
                const char *_cond_str = NULL, bool _cond_status = true
               ) :
               file(_file), func(_func), type(_type), cond_str(_cond_str),
               line(_line), cond_staus(_cond_status) {}

    template<typename T>
    GLogWrapper &operator<<(const T &v)
    {
        if (!cond_str || cond_str && !cond_staus)
            stream << v;
        return *this;
    }

    ~GLogWrapper()
    {
        if (cond_str && !cond_staus)
        {
            cv::error(cv::Error::StsError, "FAILED: " + String(cond_str) + "." + stream.str(), func, file, line);
        }
        else if (!cond_str && strcmp(type, "CHECK"))
        {
            if (!std::strcmp(type, "INFO"))
                std::cout << stream.str() << std::endl;
            else
                std::cerr << stream.str() << std::endl;
        }
    }
};

}
#endif
