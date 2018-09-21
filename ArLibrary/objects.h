#pragma once

#include <math.h>
#include <numeric>
#include <fstream>
#include <utility>
#include <vector>
#include <iostream>
#include <string>
#include <regex>
#include <ostream>

// tensorflow
#include "matmul.h"
#include "tensorflow/cc/ops/const_op.h"
#include "tensorflow/cc/ops/image_ops.h"
#include "tensorflow/cc/ops/standard_ops.h"
#include "tensorflow/core/framework/graph.pb.h"
#include "tensorflow/core/framework/tensor.h"
#include "tensorflow/core/graph/default_device.h"
#include "tensorflow/core/graph/graph_def_builder.h"
#include "tensorflow/core/lib/core/errors.h"
#include "tensorflow/core/lib/core/stringpiece.h"
#include "tensorflow/core/lib/core/threadpool.h"
#include "tensorflow/core/lib/io/path.h"
#include "tensorflow/core/lib/strings/stringprintf.h"
#include "tensorflow/core/platform/env.h"
#include "tensorflow/core/platform/init_main.h"
#include "tensorflow/core/platform/logging.h"
#include "tensorflow/core/platform/types.h"
#include "tensorflow/core/public/session.h"
#include "tensorflow/core/util/command_line_flags.h"

#include <opencv2/opencv.hpp>

using namespace cv;

class Objects
{
	Mat m_mat;
	std::string m_name;

public:

	// functions for ease of access
	Objects(Mat& mat, std::string name);
	Objects();
	~Objects();
	Mat mat();
	std::string name();

	// overloads
	Objects& operator=(const Objects obj);
	bool operator==(const Objects& obj);
	std::ostream& operator<<(std::ostream &out);
};