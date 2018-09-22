#include <math.h>
#include <numeric>
#include <fstream>
#include <utility>
#include <vector>
#include <string>
#include <iostream>
#include <regex>
#include "objects.h"

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

Objects::Objects(Mat& mat, std::string name)
	: m_name(name)
{
	m_mat = mat.clone();
}

Objects::Objects()
{
}

Objects::~Objects()
{
	m_mat.deallocate();
}

Mat Objects::mat()
{
	return m_mat;
}

std::string Objects::name()
{
	return m_name;
}

Objects& Objects::operator=(const Objects obj)
{
	if (this == &obj)
		return *this;
	this->m_mat = obj.m_mat.clone();
	this->m_name = obj.m_name;
	return *this;
}

bool Objects::operator==(const Objects& obj)
{
	if (sum(this->m_mat != obj.m_mat) == Scalar(0, 0, 0, 0) && this->m_name == obj.m_name)
		return true;
	else
		return false;
}

std::ostream& Objects::operator<<(std::ostream &out)
{
	out << this->m_name;
	return out;
}