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

#include "precomp.hpp"
using namespace cv;
using namespace cv::dnn;

#if HAVE_PROTOBUF
#include "caffe.pb.h"

#include <iostream>
#include <fstream>
#include <algorithm>
#include <google/protobuf/message.h>
#include <google/protobuf/text_format.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include "caffe_io.hpp"

using ::google::protobuf::RepeatedField;
using ::google::protobuf::RepeatedPtrField;
using ::google::protobuf::Message;
using ::google::protobuf::Descriptor;
using ::google::protobuf::FieldDescriptor;
using ::google::protobuf::Reflection;

namespace
{
    class CaffeImporter : public Importer
    {
        caffe::NetParameter net;
        caffe::NetParameter netBinary;

    public:

        CaffeImporter(const char *pototxt, const char *caffeModel)
        {
            ReadNetParamsFromTextFileOrDie(pototxt, &net);

            if (caffeModel && caffeModel[0])
                ReadNetParamsFromBinaryFileOrDie(caffeModel, &netBinary);
        }

        void addParam(const Message &msg, const FieldDescriptor *field, cv::dnn::LayerParams &params)
        {
            const Reflection *refl = msg.GetReflection();
            int type = field->cpp_type();
            bool isRepeated = field->is_repeated();
            const std::string &name = field->name();

            #define SET_UP_FILED(getter, arrayConstr, gtype)                                    \
                if (isRepeated) {                                                               \
                    const RepeatedField<gtype> &v = refl->GetRepeatedField<gtype>(msg, field);  \
                    params.set(name, DictValue::arrayConstr(v.begin(), (int)v.size()));                  \
                }                                                                               \
                else {                                                                          \
                    params.set(name, refl->getter(msg, field));                               \
                }

            switch (type)
            {
            case FieldDescriptor::CPPTYPE_INT32:
                SET_UP_FILED(GetInt32, arrayInt, ::google::protobuf::int32);
                break;
            case FieldDescriptor::CPPTYPE_UINT32:
                SET_UP_FILED(GetUInt32, arrayInt, ::google::protobuf::uint32);
                break;
            case FieldDescriptor::CPPTYPE_INT64:
                SET_UP_FILED(GetInt32, arrayInt, ::google::protobuf::int64);
                break;
            case FieldDescriptor::CPPTYPE_UINT64:
                SET_UP_FILED(GetUInt32, arrayInt, ::google::protobuf::uint64);
                break;
            case FieldDescriptor::CPPTYPE_BOOL:
                SET_UP_FILED(GetBool, arrayInt, bool);
                break;
            case FieldDescriptor::CPPTYPE_DOUBLE:
                SET_UP_FILED(GetDouble, arrayReal, double);
                break;
            case FieldDescriptor::CPPTYPE_FLOAT:
                SET_UP_FILED(GetFloat, arrayReal, float);
                break;
            case FieldDescriptor::CPPTYPE_STRING:
                if (isRepeated) {
                    const RepeatedPtrField<std::string> &v = refl->GetRepeatedPtrField<std::string>(msg, field);
                    params.set(name, DictValue::arrayString(v.begin(), (int)v.size()));
                }
                else {
                    params.set(name, refl->GetString(msg, field));
                }
                break;
            case FieldDescriptor::CPPTYPE_ENUM:
                if (isRepeated) {
                    int size = refl->FieldSize(msg, field);
                    std::vector<cv::String> buf(size);
                    for (int i = 0; i < size; i++)
                        buf[i] = refl->GetRepeatedEnum(msg, field, i)->name();
                    params.set(name, DictValue::arrayString(buf.begin(), size));
                }
                else {
                    params.set(name, refl->GetEnum(msg, field)->name());
                }
                break;
            default:
                CV_Error(Error::StsError, "Unknown type \"" + String(field->type_name()) + "\" in prototxt");
                break;
            }
        }

        inline static bool ends_with_param(const std::string &str)
        {
            static const std::string _param("_param");
            return (str.size() >= _param.size()) && str.compare(str.size() - _param.size(), _param.size(), _param) == 0;
        }

        void extractLayerParams(const Message &msg, cv::dnn::LayerParams &params, bool isInternal = false)
        {
            const Descriptor *msgDesc = msg.GetDescriptor();
            const Reflection *msgRefl = msg.GetReflection();

            for (int fieldId = 0; fieldId < msgDesc->field_count(); fieldId++)
            {
                const FieldDescriptor *fd = msgDesc->field(fieldId);

                if (!isInternal && !ends_with_param(fd->name()))
                    continue;

                bool hasData =  fd->is_required() ||
                               (fd->is_optional() && msgRefl->HasField(msg, fd)) ||
                               (fd->is_repeated() && msgRefl->FieldSize(msg, fd) > 0);
                if (!hasData)
                    continue;

                if (fd->cpp_type() == FieldDescriptor::CPPTYPE_MESSAGE)
                {
                    if (fd->is_repeated()) //Extract only first item!
                        extractLayerParams(msgRefl->GetRepeatedMessage(msg, fd, 0), params, true);
                    else
                        extractLayerParams(msgRefl->GetMessage(msg, fd), params, true);
                }
                else
                {
                    addParam(msg, fd, params);
                }
            }
        }

        BlobShape blobShapeFromProto(const caffe::BlobProto &pbBlob)
        {
            if (pbBlob.has_num() || pbBlob.has_channels() || pbBlob.has_height() || pbBlob.has_width())
            {
                return BlobShape(pbBlob.num(), pbBlob.channels(), pbBlob.height(), pbBlob.width());
            }
            else if (pbBlob.has_shape())
            {
                const caffe::BlobShape &_shape = pbBlob.shape();
                BlobShape shape(_shape.dim_size());

                for (int i = 0; i < _shape.dim_size(); i++)
                    shape[i] = (int)_shape.dim(i);

                return shape;
            }
            else
            {
                CV_Error(Error::StsError, "Unknown shape of input blob");
                return BlobShape(-1);
            }
        }

        void blobFromProto(const caffe::BlobProto &pbBlob, cv::dnn::Blob &dstBlob)
        {
            BlobShape shape = blobShapeFromProto(pbBlob);

            dstBlob.create(shape, CV_32F);
            CV_Assert(pbBlob.data_size() == (int)dstBlob.matRefConst().total());

            CV_DbgAssert(pbBlob.GetDescriptor()->FindFieldByLowercaseName("data")->cpp_type() == FieldDescriptor::CPPTYPE_FLOAT);
            float *dstData = dstBlob.matRef().ptr<float>();

            for (int i = 0; i < pbBlob.data_size(); i++)
                dstData[i] = pbBlob.data(i);
        }

        void extractBinaryLayerParms(const caffe::LayerParameter& layer, LayerParams& layerParams)
        {
            const std::string &name = layer.name();

            int li;
            for (li = 0; li != netBinary.layer_size(); li++)
            {
                if (netBinary.layer(li).name() == name)
                    break;
            }

            if (li == netBinary.layer_size() || netBinary.layer(li).blobs_size() == 0)
                return;

            const caffe::LayerParameter &binLayer = netBinary.layer(li);
            layerParams.blobs.resize(binLayer.blobs_size());
            for (int bi = 0; bi < binLayer.blobs_size(); bi++)
            {
                blobFromProto(binLayer.blobs(bi), layerParams.blobs[bi]);
            }
        }

        struct BlobNote
        {
            BlobNote(const std::string &_name, int _layerId, int _outNum) :
                name(_name.c_str()), layerId(_layerId), outNum(_outNum) {}

            const char *name;
            int layerId, outNum;
        };

        void populateNet(Net dstNet)
        {
            int layersSize = net.layer_size();
            std::vector<BlobNote> addedBlobs;
            addedBlobs.reserve(layersSize + 1);

            //setup input layer names
            {
                std::vector<String> netInputs(net.input_size());
                for (int inNum = 0; inNum < net.input_size(); inNum++)
                {
                    addedBlobs.push_back(BlobNote(net.input(inNum), 0, inNum));
                    netInputs[inNum] = net.input(inNum);
                }
                dstNet.setNetInputs(netInputs);
            }

            for (int li = 0; li < layersSize; li++)
            {
                const caffe::LayerParameter &layer = net.layer(li);
                String name = layer.name();
                String type = layer.type();
                LayerParams layerParams;

                extractLayerParams(layer, layerParams);
                extractBinaryLayerParms(layer, layerParams);

                int id = dstNet.addLayer(name, type, layerParams);

                for (int inNum = 0; inNum < layer.bottom_size(); inNum++)
                    addInput(layer.bottom(inNum), id, inNum, dstNet, addedBlobs);

                for (int outNum = 0; outNum < layer.top_size(); outNum++)
                    addOutput(layer, id, outNum, addedBlobs);
            }
        }

        void addOutput(const caffe::LayerParameter &layer, int layerId, int outNum, std::vector<BlobNote> &addedBlobs)
        {
            const std::string &name = layer.top(outNum);

            bool haveDups = false;
            for (int idx = (int)addedBlobs.size() - 1; idx >= 0; idx--)
            {
                if (addedBlobs[idx].name == name)
                {
                    haveDups = true;
                    break;
                }
            }

            if (haveDups)
            {
                bool isInplace = layer.bottom_size() > outNum && layer.bottom(outNum) == name;
                if (!isInplace)
                    CV_Error(Error::StsBadArg, "Duplicate blobs produced by multiple sources");
            }

            addedBlobs.push_back(BlobNote(name, layerId, outNum));
        }

        void addInput(const std::string &name, int layerId, int inNum, Net &dstNet, std::vector<BlobNote> &addedBlobs)
        {
            int idx;
            for (idx = (int)addedBlobs.size() - 1; idx >= 0; idx--)
            {
                if (addedBlobs[idx].name == name)
                    break;
            }

            if (idx < 0)
            {
                CV_Error(Error::StsObjectNotFound, "Can't found output blob \"" + name + "\"");
                return;
            }

            dstNet.connect(addedBlobs[idx].layerId, addedBlobs[idx].outNum, layerId, inNum);
        }

        ~CaffeImporter()
        {

        }


    };

}

Ptr<Importer> cv::dnn::createCaffeImporter(const String &prototxt, const String &caffeModel)
{
    return Ptr<Importer>(new CaffeImporter(prototxt.c_str(), caffeModel.c_str()));
}

#else //HAVE_PROTOBUF

Ptr<Importer> cv::dnn::createCaffeImporter(const String&, const String&)
{
    CV_Error(cv::Error::StsNotImplemented, "libprotobuf required to import data from Caffe models");
    return Ptr<Importer>();
}

#endif //HAVE_PROTOBUF
