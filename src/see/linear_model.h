#ifndef LINEAR_MODEL_H
#define LINEAR_MODEL_H
#include <vector>

#include "data.h"
#include "supervised_base_model.h"
using namespace std;

namespace see {

class LinearRegression : public SupervisedBaseModel {
 public:
  LinearRegression(double learning_rate = 0.000001, double reg_w = 1e-4,
                   int batch_size = 30, int epoch = 20, double min_err = 1e-4);
  ~LinearRegression();
  void Train(SupervisedData* data);
  void Predict(SupervisedData* data);
  double Score(SupervisedData* data);
  vector<double> Scores(SupervisedData* data);
  void Predict(vector<vector<double>>* feature, vector<vector<double>>* target);
  double Score(vector<vector<double>>* feature, vector<vector<double>>* target);
  vector<double> Scores(vector<vector<double>>* feature,
                        vector<vector<double>>* target);
  void sgd(vector<vector<double>>& feature, vector<vector<double>>& target);

 private:
  double learning_rate;
  double reg_w;
  double min_err;
  int batch_size;
  int epoch;
  int feature_dim;
  int target_dim;
  vector<vector<double>> w;
  vector<vector<double>> predict(vector<vector<double>>& f);
  vector<vector<double>> gradient(vector<vector<double>>& f,
                                  vector<vector<double>>& t);
  vector<vector<double>> err(vector<vector<double>>& f,
                             vector<vector<double>>& t);
};

class LogisticRegression : public SupervisedBaseModel {
 public:
  LogisticRegression(double learning_rate = 0.012, double reg_w = 1e-3,
                     int batch_size = 30, int epoch = 800,
                     double min_err = 1e-4, bool multi_label = false);
  ~LogisticRegression();
  void Train(SupervisedData* data);
  void Predict(SupervisedData* data);
  void PredictProb(SupervisedData* data);
  double Score(SupervisedData* data);
  vector<double> Scores(SupervisedData* data);
  void Predict(vector<vector<double>>* feature, vector<vector<double>>* target);
  void PredictProb(vector<vector<double>>* feature,
                   vector<vector<double>>* target);
  double Score(vector<vector<double>>* feature, vector<vector<double>>* target);
  vector<double> Scores(vector<vector<double>>* feature,
                        vector<vector<double>>* target);
  void sgd(vector<vector<double>>& feature, vector<vector<double>>& target);

 private:
  double learning_rate;
  double reg_w;
  double min_err;
  int batch_size;
  int epoch;
  int feature_dim;
  int target_dim;
  bool multi_label;
  vector<vector<double>> w;
  vector<vector<double>> predict(vector<vector<double>>& f);
  vector<vector<double>> predictLabel(vector<vector<double>>& f);
  vector<vector<double>> gradient(vector<vector<double>>& f,
                                  vector<vector<double>>& t);
  vector<vector<double>> err(vector<vector<double>>& f,
                             vector<vector<double>>& t);
};

}  // namespace see
#endif