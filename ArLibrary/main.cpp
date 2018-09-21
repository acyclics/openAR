/*
#include <vector>
#include <iostream>
#include <fstream>
#include <utility>
#include "tensorflow_funcs.h"
#include "tensorflow.h"
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

int main()
{
	string ROOTDIR = "F:/VisualStudioProjects/ArLibrary/x64/Release/tensorflowModels/oid_objectRecognition";
	string GRAPH = "frozen_inference_graph.pb";
	string LABEL = "graph.pbtxt";
	std::unique_ptr<tensorflow::Session> session;
	std::map<int, std::string> labelsMap;
	Status loadGraphAndLabelStatus = loadGraphAndLabels(ROOTDIR, GRAPH, LABEL, session, labelsMap);

	if (!loadGraphAndLabelStatus.ok()) {
		LOG(ERROR) << "loadGraphAndLabels(): ERROR" << loadGraphAndLabelStatus;
		MessageBoxA(NULL, "Error loading tensorflow graph and labels. The program will now exit.", "ERROR", MB_ICONWARNING | MB_DEFBUTTON2);
		exit(-1);
	}
	else
		LOG(INFO) << "loadGraphAndLabels(): frozen graph and labels loaded" << endl;

	// create opencv camera
	VideoCapture cam;
	Mat frame;
	cam.open(0);

	// tensorflow shape
	Tensor tensor;
	std::vector<Tensor> outputs;
	string inputLayer = "image_tensor:0";
	vector<string> outputLayer = { "detection_boxes:0", "detection_scores:0", "detection_classes:0", "num_detections:0" };
	double thresholdScore = 0.5;
	double thresholdIOU = 0.8;
	tensorflow::TensorShape shape = tensorflow::TensorShape();
	shape.AddDim(1);
	shape.AddDim((int64)cam.get(CAP_PROP_FRAME_HEIGHT));
	shape.AddDim((int64)cam.get(CAP_PROP_FRAME_WIDTH));
	shape.AddDim(3);

	for (;;)
	{
		cam >> frame;
		vector<Objects> vecObjs = objectsDetect(frame, tensor, shape, outputs, session, inputLayer, outputLayer, labelsMap);
		for (int count(0); count < vecObjs.capacity(); ++count)
		{
			if (vecObjs[count].name() == "Glasses")
				imshow("Frame", vecObjs[count].mat());
		}
		if (waitKey(1) >= 0) break;
	}

	destroyAllWindows();
}
*/