#include "linear_model.h"

#include <time.h>

#include <iostream>
#include <random>
#include <vector>

#include "data.h"
#include "data_io.h"
#include "mat.h"
using namespace std;

void TestLinearRegression() {
  default_random_engine e;
  e.seed(-10);
  see::SupervisedData all_data;
  vector<double> feature = {10, 3, 4, 8, 8, 5};
  vector<double> label = {4, 3, 1};
  for (int i = 0; i < 1000; i++) {
    feature = {(double)(e() % 100), (double)(e() % 100), (double)(e() % 100),
               (double)(e() % 100), (double)(e() % 100), (double)(e() % 100)};
    label = {0.5 * feature[0] + 0.2 * feature[1] + feature[2] + feature[3] +
                 1.3 * feature[4] + feature[5],
             0.1 * feature[0] + 0.33 * feature[1] + feature[2] +
                 2.4 * feature[3] + feature[4] + feature[5],
             0.2 * feature[0] + 0.98 * feature[1] + 10.45 * feature[2] +
                 feature[3] + feature[4] + feature[5]};
    all_data.AddOneData(&feature, &label);
  }

  see::SupervisedData train_data, test_data;
  all_data.Split(train_data, test_data, 0.2);
  see::LinearRegression lr;
  lr.Train(&train_data);
  double train_score = lr.Score(&train_data);
  double test_score = lr.Score(&test_data);
  cout << "Train Score: " << train_score << " " << train_data.GetSize() << endl;
  cout << "Test  Score: " << test_score << " " << test_data.GetSize() << endl;
  lr.Predict(&test_data);
  test_score = lr.Score(&test_data);
  cout << "Test  Score: " << test_score << " " << test_data.GetSize() << endl;
}

void TestLogisticRegression() {
  see::SupervisedData data;
  auto feature_s = see::IO::ReadCSV("/mnt/d/Code/C/dat/iris_feature.csv");
  auto target_s = see::IO::ReadCSV("/mnt/d/Code/C/dat/iris_target.csv");
  if (feature_s.size() > 0 && target_s.size() > 0) {
    auto feature = see::ZerosMat<double>(feature_s.size(), feature_s[0].size());
    auto target = see::ZerosMat<double>(target_s.size(), target_s[0].size());
    for (int i = 0; i < feature_s.size(); i++) {
      for (int j = 0; j < feature_s[i].size(); j++) {
        feature[i][j] = atof(feature_s[i][j].data());
      }
      for (int j = 0; j < target_s[i].size(); j++) {
        target[i][j] = atof(target_s[i][j].data());
      }
    }
    cout << "Feature Previes:" << endl;
    see::MatPrint<double>(feature);
    cout << "Target Preview:" << endl;
    see::MatPrint<double>(target);
    see::SupervisedData data;
    data.AddData(&feature, &target);
    cout << "FeatureDim: " << data.GetFeatureDim() << endl;
    cout << "TargetDim:  " << data.GetTargetDim() << endl;
    cout << "Size:       " << data.GetSize() << endl;
    see::SupervisedData train_data, test_data;
    data.Split(train_data, test_data, 0.2);
    see::LogisticRegression LR;
    LR.Train(&train_data);
    double train_score = LR.Score(&train_data);
    double test_score = LR.Score(&test_data);
    cout << "Train Score: " << train_score << " " << train_data.GetSize()
         << endl;
    cout << "Test  Score: " << test_score << " " << test_data.GetSize() << endl;
    LR.Predict(&test_data);
    test_score = LR.Score(&test_data);
    cout << "Test  Score: " << test_score << " " << test_data.GetSize() << endl;
  }
}

int main() {
  TestLinearRegression();
  TestLogisticRegression();
  return 0;
}
