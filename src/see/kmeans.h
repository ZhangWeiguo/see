#pragma once
#include <time.h>

#include <random>
#include <stack>
#include <vector>

#include "../basic/thread_pool.h"
#include "unsupervised_base_model.h"

namespace see {
class Kmeans : public UnSupervisedBaseModel {
 public:
  Kmeans(int cluster_num = 8, int epoch = 20, int n_jobs = 4,
         bool random_init = true, int init_num = 8, bool sse = true);
  ~Kmeans();
  void Train(SupervisedData* data);
  void Train(vector<vector<double>>* feature);
  void Predict(SupervisedData* data);
  void Predict(vector<vector<double>>* feature, vector<vector<double>>* target);
  double Score(SupervisedData* data);
  double Score(vector<vector<double>>* feature);
  vector<vector<double>> GetClusterPoint();

 private:
  void TrainThread(vector<vector<double>>* feature, int id);
  double SSEScore(vector<vector<double>>* feature,
                  vector<vector<double>> cluster_center);
  double SCScore(vector<vector<double>>* feature,
                 vector<vector<double>> cluster_center);
  pair<vector<vector<double>>, double> cluster_points;
  int epoch;
  int cluster_num;
  int n_jobs;
  int feature_dim;
  bool random_init;
  bool sse;  // true为手肘法评价, false为轮廓系数评价
  int init_num;
  default_random_engine random_engine;
  mutex lock;
};

}  // namespace see
