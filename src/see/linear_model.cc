#include "linear_model.h"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <random>
#include <vector>

#include "data.h"
#include "mat.h"
#include "math.h"
#include "utils.h"
using namespace std;

namespace see {

LinearRegression::LinearRegression(double learning_rate, double reg_w,
                                   int batch_size, int epoch, double min_err) {
  this->learning_rate = learning_rate;
  this->reg_w = reg_w;
  this->batch_size = batch_size;
  this->epoch = epoch;
  this->min_err = min_err;
  this->feature_dim = -1;
  this->target_dim = -1;
}

LinearRegression::~LinearRegression() {}

void LinearRegression::Predict(vector<vector<double>>* feature,
                               vector<vector<double>>* target) {
  *target = predict(*feature);
}

double LinearRegression::Score(vector<vector<double>>* feature,
                               vector<vector<double>>* target) {
  auto scores = Scores(feature, target);
  double score = 0;
  for (int i = 0; i < scores.size(); i++) {
    score += scores[i];
  }
  score = score / scores.size();
}

vector<double> LinearRegression::Scores(vector<vector<double>>* feature,
                                        vector<vector<double>>* target) {
  vector<vector<double>> e = err(*feature, *target);
  vector<double> scores;
  for (int i = 0; i < e.size(); i++) {
    for (int j = 0; j < e[i].size(); j++) {
      scores.push_back(pow(e[i][j], 2));
    }
    scores[i] = scores[i] / e.size();
  }
  return scores;
}

void LinearRegression::Train(SupervisedData* data) {
  feature_dim = data->GetFeatureDim();
  target_dim = data->GetTargetDim();
  if (data->GetSize() == -1) return;
  if (feature_dim <= 0 || target_dim <= 0) return;
  w.clear();
  w = RandomMat<double>(target_dim, feature_dim + 1);
  if (batch_size > data->GetSize()) {
    batch_size = min((int)data->GetSize(), (int)feature_dim * 5);
  }
  cout << "=========================================" << endl;
  cout << "LinearRegression Parameter" << endl;
  cout << left << setw(20) << "Learning Rate:" << learning_rate << endl;
  cout << left << setw(20) << "Reg W:" << reg_w << endl;
  cout << left << setw(20) << "Batch Size:" << batch_size << endl;
  cout << left << setw(20) << "Epoch:" << epoch << endl;
  cout << left << setw(20) << "Min Err:" << min_err << endl;
  cout << left << setw(20) << "Feature Dim:" << feature_dim << endl;
  cout << left << setw(20) << "Target Dim:" << target_dim << endl;
  cout << "=========================================" << endl;
  SupervisedData batch_data;
  vector<vector<double>> feature;
  vector<vector<double>> target;
  int num, all_num = 0;
  for (int i = 0; i < epoch; i++) {
    while (true) {
      num = data->GetBatchData(batch_data, batch_size);
      auto feature = batch_data.GetFeature();
      auto target = batch_data.GetTarget();
      if (num == 0) break;
      sgd(*feature, *target);
      all_num += num;
    }
    double score = Score(&batch_data);
    cout << "Epoch: " << left << setw(5) << i << "Batch: " << left << setw(8)
         << all_num << "Score: " << left << setw(10) << score << endl;
  }
}

void LinearRegression::Predict(SupervisedData* data) {
  auto f = data->GetFeature();
  auto t = data->GetTarget();
  *t = predict(*f);
}

vector<double> LinearRegression::Scores(SupervisedData* data) {
  auto feature = data->GetFeature();
  auto target = data->GetTarget();
  return Scores(feature, target);
}

double LinearRegression::Score(SupervisedData* data) {
  auto feature = data->GetFeature();
  auto target = data->GetTarget();
  return Score(feature, target);
}

vector<vector<double>> LinearRegression::gradient(vector<vector<double>>& f,
                                                  vector<vector<double>>& t) {
  vector<vector<double>> gd;             // t * (f + 1)
  vector<vector<double>> e = err(f, t);  // b * t
  for (int i = 0; i < target_dim; i++) {
    vector<double> gdd;
    double gddb = 0;
    for (int j = 0; j < feature_dim; j++) {
      double gddw = 0;
      for (int k = 0; k < f.size(); k++) {
        gddw += e[k][i] * f[k][j];
        if (j == 0) gddb += e[k][i];
      }
      gdd.push_back(gddw);
    }
    gdd.push_back(gddb);
    gd.push_back(gdd);
  }
  return gd;
}

vector<vector<double>> LinearRegression::predict(vector<vector<double>>& f) {
  vector<vector<double>> t, w1;
  if (f.size() == 0) return t;
  if (f[0].size() == 0) return t;
  auto ff = f;
  t = ZerosMat<double>((int)(ff.size()), target_dim);
  for (auto it = ff.begin(); it != ff.end(); it++) it->push_back(1.0);
  w1 = ZerosMat<double>(ff[0].size() + 1, target_dim);
  MatTranspose<double>(w1, w);
  MatDot<double>(t, ff, w1);
  return t;
}

vector<vector<double>> LinearRegression::err(vector<vector<double>>& f,
                                             vector<vector<double>>& t) {
  vector<vector<double>> e;  // b * t
  for (int i = 0; i < f.size(); i++) {
    vector<double> ee;
    for (int j = 0; j < target_dim; j++) {
      double eee = w[j][feature_dim];
      for (int k = 0; k < feature_dim; k++) {
        eee += w[j][k] * f[i][k];
      }
      eee -= t[i][j];
      ee.push_back(eee);
    }
    e.push_back(ee);
  }
  return e;
}

void LinearRegression::sgd(vector<vector<double>>& feature,
                           vector<vector<double>>& target) {
  vector<vector<double>> gd = gradient(feature, target);
  MatAdd<double>(w, w, gd, w, 0.0, 1.0, -learning_rate, -learning_rate * reg_w);
}

LogisticRegression::LogisticRegression(double learning_rate, double reg_w,
                                       int batch_size, int epoch,
                                       double min_err, bool multi_label) {
  this->learning_rate = learning_rate;
  this->reg_w = reg_w;
  this->batch_size = batch_size;
  this->epoch = epoch;
  this->min_err = min_err;
  this->feature_dim = -1;
  this->target_dim = -1;
  this->multi_label = multi_label;
}

LogisticRegression::~LogisticRegression() {}

void LogisticRegression::Predict(vector<vector<double>>* feature,
                                 vector<vector<double>>* target) {
  *target = predictLabel(*feature);
}

void LogisticRegression::PredictProb(vector<vector<double>>* feature,
                                     vector<vector<double>>* target) {
  *target = predict(*feature);
}

double LogisticRegression::Score(vector<vector<double>>* feature,
                                 vector<vector<double>>* target) {
  double score = 0.0;
  if (feature->size() == 0) return score;
  auto t_pre = predictLabel(*feature);
  for (int i = 0; i < target->size(); i++) {
    for (int j = 0; j < (*target)[i].size(); j++) {
      if (t_pre[i][j] != (*target)[i][j]) {
        score++;
        if (!multi_label) break;
      }
    }
  }
  if (multi_label)
    score = 1.0 - score / feature->size() / target_dim;
  else
    score = 1.0 - score / feature->size();
  return score;
}

vector<double> LogisticRegression::Scores(vector<vector<double>>* feature,
                                          vector<vector<double>>* target) {
  vector<double> scores;
  if (feature->size() == 0) return scores;
  auto t_pre = predictLabel(*feature);
  for (int i = 0; i < target->size(); i++) {
    for (int j = 0; j < (*target)[i].size(); j++) {
      scores[j] += abs(t_pre[i][j] - (*target)[i][j]);
    }
  }
  for (int i = 0; i < target_dim; i++) {
    scores[i] = 1 - (scores[i]) / feature->size();
  }
  return scores;
}

void LogisticRegression::Train(SupervisedData* data) {
  feature_dim = data->GetFeatureDim();
  target_dim = data->GetTargetDim();
  if (data->GetSize() == -1) return;
  if (feature_dim <= 0 || target_dim <= 0) return;
  w.clear();
  w = RandomMat<double>(target_dim, feature_dim + 1);
  if (batch_size > data->GetSize()) {
    batch_size = min((int)data->GetSize(), (int)feature_dim * 5);
  }
  cout << "=========================================" << endl;
  cout << "LogisticRegression Parameter" << endl;
  cout << left << setw(20) << "Learning Rate:" << learning_rate << endl;
  cout << left << setw(20) << "Reg W:" << reg_w << endl;
  cout << left << setw(20) << "Batch Size:" << batch_size << endl;
  cout << left << setw(20) << "Epoch:" << epoch << endl;
  cout << left << setw(20) << "Min Err:" << min_err << endl;
  cout << left << setw(20) << "Feature Dim:" << feature_dim << endl;
  cout << left << setw(20) << "Target Dim:" << target_dim << endl;
  cout << "=========================================" << endl;
  SupervisedData batch_data;
  vector<vector<double>> feature, target;
  int num, all_num = 0;
  for (int i = 0; i < epoch; i++) {
    while (true) {
      num = data->GetBatchData(batch_data, batch_size);
      batch_data.GetTrainData(feature, target);
      if (num == 0) break;
      sgd(feature, target);
      all_num += num;
      ;
    }
    double score = Score(&batch_data);
    cout << "Epoch: " << left << setw(5) << i << "Batch: " << left << setw(8)
         << all_num << "Score: " << left << setw(10) << score << endl;
  }
}

void LogisticRegression::Predict(SupervisedData* data) {
  auto f = data->GetFeature();
  auto t = data->GetTarget();
  Predict(f, t);
}

void LogisticRegression::PredictProb(SupervisedData* data) {
  auto f = data->GetFeature();
  auto t = data->GetTarget();
  PredictProb(f, t);
}

double LogisticRegression::Score(SupervisedData* data) {
  auto f = data->GetFeature();
  auto t = data->GetTarget();
  return Score(f, t);
}

vector<double> LogisticRegression::Scores(SupervisedData* data) {
  auto f = data->GetFeature();
  auto t = data->GetTarget();
  return Scores(f, t);
}

vector<vector<double>> LogisticRegression::predict(vector<vector<double>>& f) {
  vector<vector<double>> t, w1;
  if (f.size() == 0) return t;
  if (f[0].size() == 0) return t;
  auto ff = f;
  t = ZerosMat<double>((int)(ff.size()), target_dim);
  for (auto it = ff.begin(); it != ff.end(); it++) it->push_back(1.0);
  w1 = ZerosMat<double>(feature_dim + 1, target_dim);
  MatTranspose<double>(w1, w);
  MatDot<double>(t, ff, w1);
  MatApply<double>(t, sigmoid);
  return t;
}

vector<vector<double>> LogisticRegression::predictLabel(
    vector<vector<double>>& f) {
  auto t = predict(f);
  if (multi_label) {
    for (int i = 0; i < t.size(); i++) {
      for (int j = 0; j < t[i].size(); j++) {
        t[i][j] = relu(t[i][j], 0.5);
      }
    }
  } else {
    for (int i = 0; i < t.size(); i++) {
      double mx = -1;
      int index = -1;
      for (int j = 0; j < t[i].size(); j++) {
        if (t[i][j] > mx) {
          mx = t[i][j];
          index = j;
        }
        t[i][j] = 0;
      }
      if (index >= 0) t[i][index] = 1.0;
    }
  }
  return t;
}

vector<vector<double>> LogisticRegression::gradient(vector<vector<double>>& f,
                                                    vector<vector<double>>& t) {
  int bs = f.size();
  vector<vector<double>> gd = ZerosMat<double>(target_dim, feature_dim + 1);
  if (bs == 0) return gd;
  vector<vector<double>> e = err(f, t);  // b * t
  auto f1 = f;
  for (auto it = f1.begin(); it != f1.end(); it++) it->push_back(1.0);
  auto e1 = ZerosMat<double>(target_dim, f1.size());
  MatTranspose<double>(e1, e);
  MatDot<double>(gd, e1, f1);
  MatMut<double>(gd, 1.0 / (double)(bs));
  return gd;
}

vector<vector<double>> LogisticRegression::err(vector<vector<double>>& f,
                                               vector<vector<double>>& t) {
  auto t_pre = predict(f);
  MatAdd<double>(t_pre, t_pre, t, 0.0, 1.0, -1.0);
  return t_pre;
}

void LogisticRegression::sgd(vector<vector<double>>& feature,
                             vector<vector<double>>& target) {
  vector<vector<double>> gd = gradient(feature, target);
  MatAdd<double>(w, w, gd, w, 0.0, 1.0, -learning_rate, -learning_rate * reg_w);
}

}  // namespace see