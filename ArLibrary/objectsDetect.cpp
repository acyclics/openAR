#include <vector>
#include <iostream>
#include <fstream>
#include <utility>
#include "tensorflow_funcs.h"
#include "objectDetection.h"
#include "objects.h"

// tensorflow
#include <eigen/Dense>
#include "matmul.h"
#include "tensorflow/core/public/session.h"
#include "tensorflow/cc/client/client_session.h"
#include "tensorflow/cc/ops/standard_ops.h"
#include "tensorflow/core/framework/tensor.h"
#include "tensorflow/core/framework/graph.pb.h"
#include "tensorflow/cc/ops/const_op.h"
#include "tensorflow/cc/ops/image_ops.h"
#include "tensorflow/core/graph/default_device.h"
#include "tensorflow/core/graph/graph_def_builder.h"
#include "tensorflow/core/lib/core/threadpool.h"
#include "tensorflow/core/lib/io/path.h"
#include "tensorflow/core/lib/strings/stringprintf.h"
#include "tensorflow/core/platform/init_main.h"
#include "tensorflow/core/util/command_line_flags.h"

// opencv
#include <opencv2/opencv.hpp>

using tensorflow::Flag;
using tensorflow::Tensor;
using tensorflow::Status;
using tensorflow::string;
using tensorflow::int32;
using namespace cv;
using namespace std;

vector<Objects> objectsDetect(Mat frame, Tensor tensor, tensorflow::TensorShape shape, std::vector<Tensor> outputs
	, std::unique_ptr<tensorflow::Session> &session, string inputLayer, vector<string> outputLayer
	, std::map<int, std::string> &labelsMap, double thresholdScore, double thresholdIOU)
{
	cvtColor(frame, frame, COLOR_BGR2RGB);

	// convert mat to tensor
	tensor = Tensor(tensorflow::DT_FLOAT, shape);
	Status readTensorStatus = readTensorFromMat(frame, tensor);
	if (!readTensorStatus.ok()) {
		LOG(ERROR) << "Mat->Tensor conversion failed: " << readTensorStatus;
		MessageBoxA(NULL, "Error converting tensor to mat. The program will now exit.", "ERROR", MB_ICONWARNING | MB_DEFBUTTON2);
		exit(-1);
	}

	// run the graph on tensor
	outputs.clear();
	Status runStatus = session->Run({ { inputLayer, tensor } }, outputLayer, {}, &outputs);
	if (!runStatus.ok()) {
		LOG(ERROR) << "Running model failed: " << runStatus;
		MessageBoxA(NULL, "Error loading tensorflow model. The program will now exit.", "ERROR", MB_ICONWARNING | MB_DEFBUTTON2);
		exit(-1);
	}

	// extract results from the outputs vector
	tensorflow::TTypes<float>::Flat scores = outputs[1].flat<float>();
	tensorflow::TTypes<float>::Flat classes = outputs[2].flat<float>();
	tensorflow::TTypes<float>::Flat numDetections = outputs[3].flat<float>();
	tensorflow::TTypes<float, 3>::Tensor boxes = outputs[0].flat_outer_dims<float, 3>();

	vector<size_t> goodIdxs = filterBoxes(scores, boxes, thresholdIOU, thresholdScore);
	for (size_t i = 0; i < goodIdxs.size(); i++)
		LOG(INFO) << "score:" << scores(goodIdxs.at(i)) << ",class:" << labelsMap[classes(goodIdxs.at(i))]
		<< " (" << classes(goodIdxs.at(i)) << "), box:" << "," << boxes(0, goodIdxs.at(i), 0) << ","
		<< boxes(0, goodIdxs.at(i), 1) << "," << boxes(0, goodIdxs.at(i), 2) << ","
		<< boxes(0, goodIdxs.at(i), 3);

	// create mats
	cvtColor(frame, frame, COLOR_BGR2RGB);
	vector<Mat> objectMats = createMatsFromObjects(frame, scores, classes, boxes, labelsMap, goodIdxs);

	vector<Objects> vecObjs;

	for (int count(0); count < goodIdxs.size(); ++count)
	{
		Objects obj(objectMats[count], labelsMap[classes(goodIdxs.at(count))]);
		vecObjs.push_back(obj);
	}

	return vecObjs;
}