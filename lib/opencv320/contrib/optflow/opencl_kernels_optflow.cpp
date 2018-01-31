// This file is auto-generated. Do not edit!

#include "precomp.hpp"
#include "cvconfig.h"
#include "opencl_kernels_optflow.hpp"

#ifdef HAVE_OPENCL

namespace cv
{
namespace ocl
{
namespace optflow
{

const struct ProgramEntry updatemotionhistory={"updatemotionhistory",
"__kernel void updateMotionHistory(__global const uchar * silh, int silh_step, int silh_offset,\n"
"__global uchar * mhiptr, int mhi_step, int mhi_offset, int mhi_rows, int mhi_cols,\n"
"float timestamp, float delbound)\n"
"{\n"
"int x = get_global_id(0);\n"
"int y = get_global_id(1);\n"
"if (x < mhi_cols && y < mhi_rows)\n"
"{\n"
"int silh_index = mad24(y, silh_step, silh_offset + x);\n"
"int mhi_index = mad24(y, mhi_step, mhi_offset + x * (int)sizeof(float));\n"
"silh += silh_index;\n"
"__global float * mhi = (__global float *)(mhiptr + mhi_index);\n"
"float val = mhi[0];\n"
"val = silh[0] ? timestamp : val < delbound ? 0 : val;\n"
"mhi[0] = val;\n"
"}\n"
"}\n"
, "b19beb01d0c6052524049341b55a2be5"};
ProgramSource updatemotionhistory_oclsrc(updatemotionhistory.programStr);
}
}}
#endif
