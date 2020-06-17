#include "kmeans.h"

#include <iostream>

#include "mat.h"
using namespace std;

int main() {
  see::SupervisedData data;
  default_random_engine e;
  uniform_real_distribution<double> mean_random(-2, 2);
  uniform_real_distribution<double> cov_random(1, 3);
  int sample_size = 2000;
  int cluster_num = 4;
  int feature_dim = 5;
  vector<int> classes_num(cluster_num, 0), preclasses_num(cluster_num, 0);
  vector<vector<normal_distribution<double>>> cluster_random;
  for (int i = 0; i < cluster_num; i++) {
    vector<double> point;
    vector<normal_distribution<double>> point_random;
    for (int j = 0; j < feature_dim; j++) {
      double m = mean_random(e);
      double c = cov_random(e);
      point_random.push_back(normal_distribution<double>(m, c));
    }
    cluster_random.push_back(point_random);
  }
  for (int i = 0; i < sample_size; i++) {
    vector<double> sample;
    int classes = e() % cluster_num;
    classes_num[classes]++;
    for (int j = 0; j < feature_dim; j++) {
      sample.push_back(cluster_random[classes][j](e));
    }
    data.AddOneDataWithoutTarget(&sample);
  }
  for (int i = 0; i < cluster_num; i++) {
    cout << "Class " << i << " : " << classes_num[i] << endl;
  }
  see::Kmeans km(cluster_num, 20, 4, true, 1, false);
  km.Train(&data);
  km.Predict(&data);
  auto target = data.GetTarget();
  for (int i = 0; i < target->size(); i++) {
    for (int j = 0; j < feature_dim; j++) {
      if (target->at(i)[j] == 1) {
        preclasses_num[j]++;
        break;
      }
    }
  }
  for (int i = 0; i < cluster_num; i++) {
    cout << "PreClass " << i << " : " << preclasses_num[i] << endl;
  }
  return 0;
}