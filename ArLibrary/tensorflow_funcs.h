#pragma once

#include <vector>

// tensorflow
#include "matmul.h"
#include "tensorflow/core/framework/tensor.h"
#include "tensorflow/core/public/session.h"

// opencv
#include <opencv2/core/mat.hpp>

using tensorflow::Tensor;
using tensorflow::Status;
using tensorflow::string;

Status loadGraph(const string &graph_file_name, std::unique_ptr<tensorflow::Session> *session);

Status readLabelsMapFile(const string &fileName, std::map<int, string> &labelsMap);

Status readTensorFromMat(const cv::Mat &mat, Tensor &outTensor);

double IOU(cv::Rect2f box1, cv::Rect2f box2);

std::vector<size_t> filterBoxes(tensorflow::TTypes<float>::Flat &scores, tensorflow::TTypes<float, 3>::Tensor &boxes, double thresholdIOU, double thresholdScore);

void drawBoundingBoxOnImage(cv::Mat &image, double xMin, double yMin, double xMax, double yMax, double score, std::string label, bool scaled);

void drawBoundingBoxesOnImage(cv::Mat &image, tensorflow::TTypes<float>::Flat &scores, tensorflow::TTypes<float>::Flat &classes, tensorflow::TTypes<float, 3>::Tensor &boxes, std::map<int, string> &labelsMap, std::vector<size_t> &idxs);

cv::Mat createMatFromObject(cv::Mat &image, double yMin, double xMin, double yMax, double xMax, double score, std::string label, bool scaled);

std::vector<cv::Mat> createMatsFromObjects(cv::Mat &image, tensorflow::TTypes<float>::Flat &scores, tensorflow::TTypes<float>::Flat &classes, tensorflow::TTypes<float, 3>::Tensor &boxes, std::map<int, string> &labelsMap, std::vector<size_t> &idxs);
