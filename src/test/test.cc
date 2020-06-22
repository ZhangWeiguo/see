#include <iostream>

#include "thirdparty/eigen3/Eigen/Core"
#include "thirdparty/eigen3/Eigen/Dense"
#include "thirdparty/gflags/gflags.h"
#include "thirdparty/glog/logging.h"


using namespace std;

DEFINE_int32(qps, 10, "test qps");

int main(int argc, char* argv[]) {
  google::InitGoogleLogging(argv[0]);
  google::ParseCommandLineFlags(&argc, &argv, false);
  cout << "test qps:" << FLAGS_qps << endl;
  LOG(INFO) << "INFO" << endl;
  LOG(ERROR) << "ERROR" << endl;
  LOG(WARNING) << "WARNING" << endl;
  Eigen::MatrixXd m1 = Eigen::MatrixXd::Random(3, 3);
  Eigen::MatrixXd m2 = Eigen::MatrixXd::Constant(3, 3, 1.2);
  auto m = m1 + m2;
  cout << m << endl;
  return 0;
}
