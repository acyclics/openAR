#include <vector>
#include <iostream>
#include <fstream>
#include <utility>
#include "tensorflow_funcs.h"

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

using tensorflow::Flag;
using tensorflow::Tensor;
using tensorflow::Status;
using tensorflow::string;
using tensorflow::int32;
using namespace cv;
using namespace std;

Status loadGraphAndLabels(string ROOTDIR, string GRAPH, string LABELS, std::unique_ptr<tensorflow::Session> &session, std::map<int, std::string> &labelsMap)
{
	// load graph from .pb file
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
	labelsMap = std::map<int, std::string>();
	Status readLabelsMapStatus = readLabelsMapFile(tensorflow::io::JoinPath(ROOTDIR, LABELS), labelsMap);
	if (!readLabelsMapStatus.ok()) {
		LOG(ERROR) << "readLabelsMapFile(): ERROR" << loadGraphStatus;
		MessageBoxA(NULL, "Error loading tensorflow label maps. The program will now exit.", "ERROR", MB_ICONWARNING | MB_DEFBUTTON2);
		exit(-1);
	}
	else
		LOG(INFO) << "readLabelsMapFile(): labels map loaded with " << labelsMap.size() << " label(s)" << endl;

	return Status::OK();
}