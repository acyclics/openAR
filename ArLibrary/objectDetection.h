#pragma once

#include <vector>
#include "objects.h"

// tensorflow
#include "matmul.h"
#include "tensorflow/core/framework/tensor.h"
#include "tensorflow/core/public/session.h"

// opencv
#include <opencv2/core/mat.hpp>

using tensorflow::Tensor;
using tensorflow::Status;
using tensorflow::string;
using namespace cv;
using namespace std;

vector<Objects> objectsDetect(Mat frame, Tensor tensor, tensorflow::TensorShape shape, std::vector<Tensor> outputs
	, std::unique_ptr<tensorflow::Session> &session, string inputLayer, vector<string> outputLayer
	, std::map<int, std::string> &labelsMap, double thresholdScore = 0.5, double thresholdIOU = 0.8);