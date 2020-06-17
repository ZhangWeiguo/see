#include "dataset.h"

#include "data_io.h"

namespace see {
void DataSet::GetIris(DoubleMat* feature, DoubleMat* target, string path) {
  auto feature_s = see::IO::ReadCSV(path + "iris_feature.csv");
  auto target_s = see::IO::ReadCSV(path + "iris_target.csv");
  if (feature_s.size() > 0 && target_s.size() > 0) {
    *feature = see::ZerosMat<double>(feature_s.size(), feature_s[0].size());
    *target = see::ZerosMat<double>(target_s.size(), target_s[0].size());
    for (int i = 0; i < feature_s.size(); i++) {
      for (int j = 0; j < feature_s[i].size(); j++) {
        (*feature)[i][j] = atof(feature_s[i][j].data());
      }
      for (int j = 0; j < target_s[i].size(); j++) {
        (*target)[i][j] = atof(target_s[i][j].data());
      }
    }
  }
}

void DataSet::GetDigits(DoubleMat* feature, DoubleMat* target, string path) {
  auto feature_s = see::IO::ReadCSV(path + "digits_feature.csv");
  auto target_s = see::IO::ReadCSV(path + "digits_target.csv");
  if (feature_s.size() > 0 && target_s.size() > 0) {
    *feature = see::ZerosMat<double>(feature_s.size(), feature_s[0].size());
    *target = see::ZerosMat<double>(target_s.size(), target_s[0].size());
    for (int i = 0; i < feature_s.size(); i++) {
      for (int j = 0; j < feature_s[i].size(); j++) {
        (*feature)[i][j] = atof(feature_s[i][j].data());
      }
      for (int j = 0; j < target_s[i].size(); j++) {
        (*target)[i][j] = atof(target_s[i][j].data());
      }
    }
  }
}

DoubleMat DataSet::MakeRandomRegression(DoubleMat* feature, DoubleMat* target,
                                        int feature_dim, int target_dim,
                                        int sample_size, double noise) {
  feature->clear();
  target->clear();
  DoubleMat w = ZerosMat<double>(target_dim, feature_dim + 1);
  if (feature_dim <= 0 || target_dim <= 0 || noise < 0 || noise >= 1) return w;
  default_random_engine e;
  time_t now;
  e.seed(now);
  double rannge = 10, x = 0;
  uniform_real_distribution<double> coef_generator(-rannge, rannge);
  uniform_real_distribution<double> noise_generator(-rannge * noise,
                                                    rannge * noise);
  vector<double> f(feature_dim, 0), t(target_dim, 0);
  for (int i = 0; i < target_dim; i++) {
    for (int j = 0; j <= feature_dim; j++) {
      w[i][j] = coef_generator(e);
    }
  }
  for (int i = 0; i < sample_size; i++) {
    for (int j = 0; i < target_dim; i++) t[j] = 0;
    for (int j = 0; i < feature_dim; i++) f[j] = 0;
    for (int j = 0; j <= feature_dim + 1; j++) {
      if (j < feature_dim) {
        x = coef_generator(e);
        f[j] = x;
      } else if (j == feature_dim) {
        x = 1;
      } else {
        if (noise > 0) {
          x = noise_generator(e);
        }
      }
      for (int k = 0; k < target_dim; k++) {
        if (j <= feature_dim)
          t[k] += x * w[k][j];
        else
          t[k] += x;
      }
    }
    feature->emplace_back(f);
    target->emplace_back(t);
  }
}

DoubleMat DataSet::MakeRandomClassification(DoubleMat* feature,
                                            DoubleMat* target, int feature_dim,
                                            int target_dim, int sample_size,
                                            bool multi_label, double noise) {
  feature->clear();
  target->clear();
  DoubleMat w = ZerosMat<double>(target_dim, feature_dim + 1);
  if (feature_dim <= 0 || target_dim <= 0 || noise < 0 || noise >= 1) return w;
  default_random_engine e;
  time_t now;
  e.seed(now);
  double rannge = 10, x = 0;
  uniform_real_distribution<double> coef_generator(-rannge, rannge);
  uniform_real_distribution<double> noise_generator(-rannge * noise,
                                                    rannge * noise);
  for (int i = 0; i < target_dim; i++) {
    for (int j = 0; j <= feature_dim; j++) {
      w[i][j] = coef_generator(e);
    }
  }
  for (int i = 0; i < sample_size; i++) {
    vector<double> f(feature_dim, 0);
    vector<double> t(target_dim, 0);
    for (int j = 0; j <= feature_dim + 1; j++) {
      if (j < feature_dim) {
        x = coef_generator(e);
        f[j] = x;
      } else if (j == feature_dim) {
        x = 1;
      } else {
        if (noise > 0) {
          x = noise_generator(e);
        }
      }
      for (int k = 0; k < target_dim; k++) {
        if (j <= feature_dim)
          t[k] += x * w[k][j];
        else
          t[k] += x;
      }
    }
    for (int k = 0; k < target_dim; k++) {
      t[k] = sigmoid(t[k]);
    }
    if (multi_label) {
      for (int k = 0; k < target_dim; k++) {
        t[k] = relu((t[k]), 0.5);
      }
    } else {
      int index;
      double max_prob;
      for (int k = 0; k < target_dim; k++) {
        if (k == 0 || t[k] > max_prob) {
          index = k;
          max_prob = t[k];
        }
        t[k] = 0;
      }
      t[index] = 1;
    }
    feature->emplace_back(f);
    target->emplace_back(t);
  }
}

}  // namespace see