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

#ifndef __OPENCV_DNN_DNN_DICT_HPP__
#define __OPENCV_DNN_DNN_DICT_HPP__

#include <opencv2/core.hpp>
#include <map>
#include <ostream>

namespace cv
{
namespace dnn
{
//! @addtogroup dnn
//! @{

/** @brief This struct stores the scalar value (or array) of one of the following type: double, cv::String or int64.
 *  @todo Maybe int64 is useless because double type exactly stores at least 2^52 integers.
 */
struct DictValue
{
    DictValue(const DictValue &r);
    DictValue(int p = 0)        : type(Param::INT), pi(new AutoBuffer<int64,1>) { (*pi)[0] = p; }       //!< Constructs integer scalar
    DictValue(unsigned p)       : type(Param::INT), pi(new AutoBuffer<int64,1>) { (*pi)[0] = p; }       //!< Constructs integer scalar
    DictValue(double p)         : type(Param::REAL), pd(new AutoBuffer<double,1>) { (*pd)[0] = p; }     //!< Constructs floating point scalar
    DictValue(const String &p)  : type(Param::STRING), ps(new AutoBuffer<String,1>) { (*ps)[0] = p; }   //!< Constructs string scalar

    template<typename TypeIter>
    static DictValue arrayInt(TypeIter begin, int size);    //!< Constructs integer array
    template<typename TypeIter>
    static DictValue arrayReal(TypeIter begin, int size);   //!< Constructs floating point array
    template<typename TypeIter>
    static DictValue arrayString(TypeIter begin, int size); //!< Constructs array of strings

    template<typename T>
    T get(int idx = -1) const; //!< Tries to convert array element with specified index to requested type and returns its.

    int size() const;

    bool isInt() const;
    bool isString() const;
    bool isReal() const;

    DictValue &operator=(const DictValue &r);

    friend std::ostream &operator<<(std::ostream &stream, const DictValue &dictv);

    ~DictValue();

private:

    int type;

    union
    {
        AutoBuffer<int64, 1> *pi;
        AutoBuffer<double, 1> *pd;
        AutoBuffer<String, 1> *ps;
        void *p;
    };

    DictValue(int _type, void *_p) : type(_type), p(_p) {}
    void release();
};

/** @brief This class implements name-value dictionary, values are instances of DictValue. */
class CV_EXPORTS Dict
{
    typedef std::map<String, DictValue> _Dict;
    _Dict dict;

public:

    //! Checks a presence of the @p key in the dictionary.
    bool has(const String &key);

    //! If the @p key in the dictionary then returns pointer to its value, else returns NULL.
    DictValue *ptr(const String &key);

    //! If the @p key in the dictionary then returns its value, else an error will be generated.
    const DictValue &get(const String &key) const;

    /** @overload */
    template <typename T>
    T get(const String &key) const;

    //! If the @p key in the dictionary then returns its value, else returns @p defaultValue.
    template <typename T>
    T get(const String &key, const T &defaultValue) const;

    //! Sets new @p value for the @p key, or adds new key-value pair into the dictionary.
    template<typename T>
    const T &set(const String &key, const T &value);

    friend std::ostream &operator<<(std::ostream &stream, const Dict &dict);
};

//! @}
}
}

#endif
