/*
void objectsDetect()
{
using tensorflow::Flag;
using tensorflow::Tensor;
using tensorflow::Status;
using tensorflow::string;
using tensorflow::int32;
using namespace cv;
using namespace std;

// load graph from .pb file
string ROOTDIR = "F:/VisualStudioProjects/ArLibrary/x64/Release/tensorflowModels/oid_objectRecognition";
string GRAPH = "frozen_inference_graph.pb";
std::unique_ptr<tensorflow::Session> session;
string graphPath = tensorflow::io::JoinPath(ROOTDIR, GRAPH);
LOG(INFO) << "graphPath:" << graphPath;
Status loadGraphStatus = loadGraph(graphPath, &session);
if (!loadGraphStatus.ok()) {
LOG(ERROR) << "loadGraph(): ERROR" << loadGraphStatus;
MessageBoxA(NULL, "Error loading tensorflow graph. The program will now exit.", "ERROR", MB_ICONWARNING | MB_DEFBUTTON2);
exit(-1);
}
else
LOG(INFO) << "loadGraph(): frozen graph loaded" << endl;

// load .pbtxt file
string LABELS = "graph.pbtxt";
std::map<int, std::string> labelsMap = std::map<int, std::string>();
Status readLabelsMapStatus = readLabelsMapFile(tensorflow::io::JoinPath(ROOTDIR, LABELS), labelsMap);
if (!readLabelsMapStatus.ok()) {
LOG(ERROR) << "readLabelsMapFile(): ERROR" << loadGraphStatus;
MessageBoxA(NULL, "Error loading tensorflow label maps. The program will now exit.", "ERROR", MB_ICONWARNING | MB_DEFBUTTON2);
exit(-1);
}
else
LOG(INFO) << "readLabelsMapFile(): labels map loaded with " << labelsMap.size() << " label(s)" << endl;

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

while (cam.isOpened())
{
cam >> frame;
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

// draw bboxes and captions
cvtColor(frame, frame, COLOR_BGR2RGB);
drawBoundingBoxesOnImage(frame, scores, classes, boxes, labelsMap, goodIdxs);

imshow("stream", frame);
if (waitKey(1) >= 0) break;
}

destroyAllWindows();
}


#include <vector>
#include <iostream>
#include <fstream>
#include <utility>
#include "tensorflow_funcs.h"
#include "objectDetection.h"

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

void objectsDetect(Mat frame, Tensor tensor, tensorflow::TensorShape shape, std::vector<Tensor> outputs
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

// draw bboxes and captions
cvtColor(frame, frame, COLOR_BGR2RGB);
drawBoundingBoxesOnImage(frame, scores, classes, boxes, labelsMap, goodIdxs);

imshow("stream", frame);
}
*/

/*
std::ofstream mytempfile;
mytempfile.open("execheck.txt", std::ios::app | std::ios::in);
mytempfile << len;
mytempfile.close();
*/