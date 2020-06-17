#pragma once
#include <time.h>

#include <algorithm>
#include <functional>
#include <random>
#include <set>
#include <vector>

#include "../basic/thread_pool.h"
#include "../basic/utils.h"
#include "data.h"
#include "mat.h"
#include "supervised_base_model.h"
#include "utils.h"
using namespace std;

namespace see {

enum KernelFun {
  LinearKernelFun = 0,
  PolynomialKernelFun = 1,
  GaussianKernelFun = 2,
  LaplaceKernelFun = 3,
  SigmoidKernelFun = 4
};

// SGD未实现
// squared_hinge未实现
// 正则未实现
class SVMClassifier : public SupervisedBaseModel {
 public:
  SVMClassifier(double c = 1.0, double tolerance = 1e-4, int epoch = 50,
                bool multi_label = false, int n_jobs = -1);  // nokernel + smo
  SVMClassifier(double c = 0.5, double tolerance = 1e-4,
                KernelFun kf = GaussianKernelFun, int epoch = 50,
                bool multi_label = false, double d = 2, double delta = 1,
                double beta = 1, double sita = -0.5,
                int n_jobs = -1);  // kernel + smo
  ~SVMClassifier();
  void Train(SupervisedData* data);
  void Train(vector<vector<double>>* feature, vector<vector<double>>* target);
  void Predict(SupervisedData* data);
  void Predict(vector<vector<double>>* feature, vector<vector<double>>* target);
  double Score(SupervisedData* data);
  double Score(vector<vector<double>>* feature, vector<vector<double>>* target);
  vector<double> Scores(SupervisedData* data);
  vector<double> Scores(vector<vector<double>>* feature,
                        vector<vector<double>>* target);

 private:
  void SMO(vector<vector<double>>* feature, vector<vector<double>>* target,
           vector<vector<double>>* cross_feature, int index);
  void KernelTransform(vector<vector<double>>* feature,
                       vector<vector<double>>* feature_t);
  void Predict(vector<double>* f, vector<double>* t);
  double Predict(int n, int index, vector<vector<double>>* target, double* b,
                 vector<double>* alpha, vector<vector<double>>* cross_feature);
  double Error(int n, int index, vector<vector<double>>* target, double* b,
               vector<double>* alpha, vector<vector<double>>* cross_feature);
  double c;
  int feature_dim;
  int target_dim;
  int w_dim;
  bool multi_label;
  double tolerance;
  int epoch;
  bool kernel;
  int n_jobs;
  function<double(vector<double>, vector<double>)> kernel_fun;
  vector<vector<double>>* w;
  vector<vector<double>>* kernel_sample;
  default_random_engine random_engine;
};

class SVMRegression : public SupervisedBaseModel {
 public:
  SVMRegression(double c = 1.0, double tolerance = 1e-4, int epoch = 50,
                bool multi_label = false, int n_jobs = -1);  // nokernel + smo
  SVMRegression(double c = 0.5, double tolerance = 1e-4,
                KernelFun kf = GaussianKernelFun, int epoch = 50,
                bool multi_label = false, double d = 2, double delta = 1,
                double beta = 1, double sita = -0.5,
                int n_jobs = -1);  // kernel + smo
  ~SVMRegression();
  void Train(SupervisedData* data);
  void Train(vector<vector<double>>* feature, vector<vector<double>>* target);
  void Predict(SupervisedData* data);
  void Predict(vector<vector<double>>* feature, vector<vector<double>>* target);
  double Score(SupervisedData* data);
  double Score(vector<vector<double>>* feature, vector<vector<double>>* target);
  vector<double> Scores(SupervisedData* data);
  vector<double> Scores(vector<vector<double>>* feature,
                        vector<vector<double>>* target);

 private:
  void SMO(vector<vector<double>>* feature, vector<vector<double>>* target,
           vector<vector<double>>* cross_feature, int index);
  void KernelTransform(vector<vector<double>>* feature,
                       vector<vector<double>>* feature_t);
  void Predict(vector<double>* f, vector<double>* t);
  double Predict(int n, int index, vector<vector<double>>* target, double* b,
                 vector<double>* alpha, vector<vector<double>>* cross_feature);
  double Error(int n, int index, vector<vector<double>>* target, double* b,
               vector<double>* alpha, vector<vector<double>>* cross_feature);
  double c;
  int feature_dim;
  int target_dim;
  int w_dim;
  bool multi_label;
  double tolerance;
  int epoch;
  bool kernel;
  int n_jobs;
  function<double(vector<double>, vector<double>)> kernel_fun;
  vector<vector<double>>* w;
  vector<vector<double>>* kernel_sample;
  default_random_engine random_engine;
};

}  // namespace see