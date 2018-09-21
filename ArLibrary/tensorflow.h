#pragma once

// tensorflow
#include "matmul.h"
#include "tensorflow/core/framework/tensor.h"
#include "tensorflow/core/public/session.h"

// opencv
#include <opencv2/core/mat.hpp>

using tensorflow::Tensor;
using tensorflow::Status;
using tensorflow::string;

Status loadGraphAndLabels(string ROOTDIR, string GRAPH, string LABELS, std::unique_ptr<tensorflow::Session> &session, std::map<int, std::string> &labelsMap);