#include "kmeans.h"

#include <stdio.h>

#include <algorithm>
#include <functional>
#include <set>

#include "mat.h"
#include "matc.h"
using namespace std;

namespace see {

Kmeans::Kmeans(int cluster_num, int epoch, int n_jobs, bool random_init,
               int init_num, bool sse) {
  this->cluster_num = cluster_num > 1 ? cluster_num : 8;
  this->epoch = epoch >= 1 ? epoch : 50;
  this->n_jobs = n_jobs >= 1 ? n_jobs : 4;
  this->random_init = random_init;
  this->init_num = init_num >= 1 ? init_num : 8;
  this->cluster_points.second = -1;
  this->sse = sse;
  time_t now;
  time(&now);
  random_engine.seed(now);
}

Kmeans::~Kmeans() {}

void Kmeans::Train(SupervisedData* data) {
  auto feature = data->GetFeature();
  Train(feature);
}

void Kmeans::Predict(SupervisedData* data) {
  auto feature = data->GetFeature();
  auto target = data->GetTarget();
  Predict(feature, target);
}

double Kmeans::Score(SupervisedData* data) {
  auto feature = data->GetFeature();
  return Score(feature);
}

vector<vector<double>> Kmeans::GetClusterPoint() {
  return cluster_points.first;
}

void Kmeans::Train(vector<vector<double>>* feature) {
  int sample_size = feature->size();
  if (sample_size == 0 || sample_size < cluster_num) return;
  feature_dim = (*feature)[0].size();
  printf("=========================================\n");
  printf("KMeans Parameter\n");
  printf("%20s: %d\n", "Cluster Num", cluster_num);
  printf("%20s: %d\n", "Epoch", epoch);
  printf("%20s: %d\n", "Random Init", random_init);
  printf("%20s: %d\n", "Init Num", init_num);
  printf("%20s: %d\n", "N Jobs", n_jobs);
  printf("%20s: %d\n", "Feature Dim", feature_dim);
  printf("=========================================\n");
  if (init_num > 1) {
    basic::ThreadPool pool(n_jobs);
    for (int i = 0; i < init_num; i++) {
      function<void()> f = bind(&Kmeans::TrainThread, this, feature, i);
      pool.AddJob(f);
    }
  } else if (init_num == 1) {
    TrainThread(feature, 0);
  }
}

void Kmeans::Predict(vector<vector<double>>* feature,
                     vector<vector<double>>* target) {
  double distance = 0;
  int cluster_class = 0;
  target->clear();
  for (int j = 0; j < feature->size(); j++) {
    auto sample = feature->at(j);
    for (int k = 0; k < cluster_num; k++) {
      double dis = Euclidean(sample, cluster_points.first[k]);
      if (k == 0 || (k > 0 && distance > dis)) {
        distance = dis;
        cluster_class = k;
      }
    }
    vector<double> target0(cluster_num, 0);
    target0[cluster_class] = 1;
    target->emplace_back(target0);
  }
}

// 轮廓系数 || 手肘法
double Kmeans::Score(vector<vector<double>>* feature) {
  if (sse)
    return SSEScore(feature, cluster_points.first);
  else
    return SCScore(feature, cluster_points.first);
}

// 手肘法
double Kmeans::SSEScore(vector<vector<double>>* feature,
                        vector<vector<double>> cluster_center) {
  double score = 0, distance = 0;
  if (feature->size() == 0) return score;
  for (int j = 0; j < feature->size(); j++) {
    auto sample = feature->at(j);
    for (int k = 0; k < cluster_num; k++) {
      double dis = Euclidean(sample, cluster_center[k]);
      if (k == 0 || (k > 0 && distance > dis)) distance = dis;
    }
    score += distance;
  }
  return score / feature->size();
}

// 轮廓系数
double Kmeans::SCScore(vector<vector<double>>* feature,
                       vector<vector<double>> cluster_center) {
  double score = 0, distance = 0, ai = 0, bi = 0;
  vector<double> all_distance;
  if (feature->size() == 0) return score;
  int cluster_class = 0;
  for (int j = 0; j < feature->size(); j++) {
    auto sample = feature->at(j);
    all_distance.clear();
    for (int k = 0; k < cluster_num; k++) {
      double dis = Euclidean(sample, cluster_center[k]);
      if (k == 0 || (k > 0 && distance > dis)) {
        distance = dis;
        cluster_class = k;
      }
      all_distance.push_back(dis);
    }
    sort(all_distance.begin(), all_distance.end());
    ai = all_distance[0];
    bi = all_distance[1];
    if (ai < bi && bi > 0) {
      score += (1 - ai / bi);
    } else if (ai > bi && ai > 0) {
      score += (bi / ai - 1);
    }
  }
  return score / feature->size();
}

void Kmeans::TrainThread(vector<vector<double>>* feature, int id) {
  vector<vector<double>> clusters;
  vector<pair<vector<double>, int>> clusters_sum;
  int sample_size = feature->size();
  int index = 0;
  set<int> init_indexs;
  double distance = 0, cluster_class = 0, score = 0.0;
  vector<pair<int, double>> random_max_point;
  if (random_init) {
    // Random Init Point
    while (init_indexs.size() < cluster_num) {
      index = random_engine() % sample_size;
      if (init_indexs.count(index) == 0) {
        clusters.emplace_back(feature->at(index));
        clusters_sum.emplace_back(make_pair(vector<double>(feature_dim, 0), 0));
        init_indexs.emplace(index);
      }
    }
  } else {
    // Random First Point, Max Distance Other Point
    index = random_engine() % sample_size;
    clusters.emplace_back(feature->at(index));
    clusters_sum.emplace_back(make_pair(vector<double>(feature_dim, 0), 0));
    init_indexs.emplace(index);
    while (init_indexs.size() < cluster_num - 1) {
      vector<double> avg_center;
      MatAvg<double>(avg_center, clusters, true);
      for (int j = 0; j < sample_size; j++) {
        if (init_indexs.count(j) == 0) {
          auto sample = feature->at(j);
          distance = Euclidean(sample, avg_center);
          random_max_point.emplace_back(make_pair(j, distance));
          sort(random_max_point.begin(), random_max_point.end(),
               [](pair<int, double> a, pair<int, double> b) {
                 return (a.second > a.second);
               });
          if (random_max_point.size() > cluster_num) {
            random_max_point.pop_back();
          }
        }
      }
      index = random_engine() % (random_max_point.size());
      index = random_max_point[index].first;
      clusters.emplace_back(feature->at(index));
      clusters_sum.emplace_back(make_pair(vector<double>(feature_dim, 0), 0));
      init_indexs.emplace(index);
    }
  }
  for (int i = 0; i < epoch; i++) {
    for (int j = 0; j < sample_size; j++) {
      auto sample = feature->at(j);
      for (int k = 0; k < cluster_num; k++) {
        double dis = Euclidean(sample, clusters[k]);
        if (k == 0 || (k > 0 && distance > dis)) {
          distance = dis;
          cluster_class = k;
        }
      }
      for (int k = 0; k < feature_dim; k++) {
        clusters_sum[cluster_class].first[k] += sample[k];
      }
      clusters_sum[cluster_class].second++;
    }
    for (int j = 0; j < cluster_num; j++) {
      double num = clusters_sum[j].second;
      for (int k = 0; k < feature_dim; k++) {
        clusters[j][k] = clusters_sum[j].first[k] / num;
      }
      clusters_sum[j].first = vector<double>(feature_dim, 0);
      clusters_sum[j].second = 0;
    }
    if (sse)
      score = SSEScore(feature, clusters);
    else
      score = SCScore(feature, clusters);
    printf("Init %d Epoch %5d: Score %f\n", id, i, score);
  }
  {
    lock_guard<mutex> g_lock(lock);
    if (cluster_points.second == -1 || score <= cluster_points.second) {
      cluster_points.second = score;
      cluster_points.first = clusters;
    }
  }
}
}  // namespace see