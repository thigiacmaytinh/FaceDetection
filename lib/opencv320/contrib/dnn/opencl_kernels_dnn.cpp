// This file is auto-generated. Do not edit!

#include "precomp.hpp"
#include "cvconfig.h"
#include "opencl_kernels_dnn.hpp"

#ifdef HAVE_OPENCL

namespace cv
{
namespace ocl
{
namespace dnn
{

const struct ProgramEntry im2col={"im2col",
"__kernel void im2col(__global const float *im_src, int im_src_offset,\n"
"int channels, int height_inp, int width_inp,\n"
"int kernel_h, int kernel_w, int pad_h, int pad_w, int stride_h, int stride_w,\n"
"int height_out, int width_out,\n"
"__global float *im_col, int im_col_offset\n"
")\n"
"{\n"
"int index = get_global_id(0);\n"
"if (index >= height_out * width_out * channels)\n"
"return;\n"
"int j_out = index % width_out;\n"
"int i_out = (index / width_out) % height_out;\n"
"int c_inp = (index / width_out) / height_out;\n"
"int c_out = c_inp * kernel_h * kernel_w;\n"
"int i_inp = i_out * stride_h - pad_h;\n"
"int j_inp = j_out * stride_w - pad_w;\n"
"im_src += (c_inp * height_inp + i_inp) * width_inp + j_inp + im_src_offset / sizeof(float);\n"
"im_col += (c_out * height_out + i_out) * width_out + j_out + im_col_offset / sizeof(float);\n"
"for (int ki = 0; ki < kernel_h; ++ki)\n"
"for (int kj = 0; kj < kernel_w; ++kj) {\n"
"int i = i_inp + ki;\n"
"int j = j_inp + kj;\n"
"*im_col = (i >= 0 && j >= 0 && i < height_inp && j < width_inp) ?\n"
"im_src[ki * width_inp + kj] : 0;\n"
"im_col += height_out * width_out;\n"
"}\n"
"}\n"
, "e2d7ab4beba37e0bac92fba4470aa728"};
ProgramSource im2col_oclsrc(im2col.programStr);
}
}}
#endif
