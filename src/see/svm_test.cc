#include "svm.h"

#include <iostream>

#include "dataset.h"
using namespace std;

int main() {
  see::DoubleMat feature, target;
  see::DataSet::GetDigits(&feature, &target);
  // see::DataSet::GetIris(&feature, &target);
  // see::SVMClassifier svm(0.9, 1e-5, 10, false);
  see::SVMClassifier svm(2.0, 1e-4, see::GaussianKernelFun, 20, false, 2, 1, 1,
                         -0.5);
  see::SupervisedData all_data;
  all_data.AddData(&feature, &target);
  svm.Train(&all_data);
  cout << "Train Score: " << svm.Score(&all_data) << endl;
  // see::SupervisedData train_data,test_data;
  // all_data.Split(train_data, test_data, 0.2);
  // svm.Train(&train_data);
  // cout << "Train Score: " << svm.Score(&train_data) << endl;
  // cout << "Test Score: " << svm.Score(&test_data) << endl;
  return 0;
}