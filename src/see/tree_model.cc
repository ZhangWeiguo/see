#include "tree_model.h"

#include <stdio.h>

#include <algorithm>
#include <functional>
#include <iomanip>
#include <iostream>
#include <map>
#include <set>
#include <thread>
#include <vector>

#include "../basic/thread_pool.h"
#include "../basic/utils.h"
#include "mat.h"
#include "math.h"
using namespace std;

namespace see {

// 踩坑, 这里不需要递归的去delete
// 因为delete关键字本身就调用了析构函数, 天然的递归
BaseNode::~BaseNode() {
  for (int i = 0; i < children.size(); i++) {
    delete children[i];
  }
  children.clear();
}

BaseNode::BaseNode() {
  limit = -1;
  val = 0;
}

BaseNode::BaseNode(int i, double v) {
  limit = 0;
  val = v;
  index = i;
}

BaseNode::BaseNode(int i, double v, bool left) {
  index = i;
  if (left) {
    limit = 1;
    upper = v;
  } else {
    limit = 3;
    bottom = v;
  }
}
BaseNode::BaseNode(int i, double b, double u) {
  index = i;
  limit = 2;
  upper = u;
  bottom = b;
}

bool BaseNode::IsIn(vector<double>& d) {
  bool result = false;
  double dd = d[index];
  switch (limit) {
    case -1:
      result = true;
      break;
    case 0:
      if (dd == val) result = true;
      break;
    case 1:
      if (dd <= upper) result = true;
      break;
    case 2:
      if (dd <= upper && dd > bottom) result = true;
      break;
    case 3:
      if (dd > bottom) result = true;
      break;
    default:
      break;
  }
  return result;
}

int BaseNode::GetDepth() {
  if (children.size() == 0) return 0;
  int depth = 0;
  for (auto it = children.begin(); it != children.end(); it++) {
    depth = max(depth, (*it)->GetDepth() + 1);
  }
  return depth;
}

int BaseNode::GetSize() {
  int size = 1;
  if (children.size() == 0) return size;
  for (auto it = children.begin(); it != children.end(); it++) {
    size += (*it)->GetSize();
  }
  return size;
}

void BaseNode::SetData(vector<vector<double>>* f, vector<vector<double>>* t,
                       vector<vector<double>>* ff, vector<vector<double>>* tt) {
  if (f->size() == 0 || t->size() == 0 || f->size() != f->size()) return;
  int dim = (*t)[0].size();
  if (dim == 0) return;
  target = vector<double>(dim, 0);
  ff->clear();
  tt->clear();
  for (int i = 0; i < f->size(); i++) {
    if (IsIn((*f)[i])) {
      for (int j = 0; j < dim; j++) {
        target[j] += (*t)[i][j];
      }
      ff->push_back((*f)[i]);
      tt->push_back((*t)[i]);
    }
  }
  for (int j = 0; j < dim; j++) {
    target[j] = target[j] / ff->size();
  }
}

vector<double> BaseNode::GetTarget() { return target; }

//////////////////////////////////////////////////////////////////////////////////////////////////

BaseTree::BaseTree(TreeType tree_type, int max_depth, int min_leaves) {
  this->tree_type = (tree_type >= 0 && tree_type <= 3) ? tree_type : GINI;
  this->max_depth = max_depth >= 0 ? max_depth : -1;
  this->min_leaves = min_leaves >= 1 ? min_leaves : 1;
  this->feature_dim = 0;
  this->target_dim = 0;
  this->root = new BaseNode;
  this->feature_type.clear();
}

BaseTree::~BaseTree() { delete root; }

void BaseTree::SetFeatureType(vector<FeatureType>& ft) { feature_type = ft; }

void BaseTree::TrainByVec(vector<vector<double>>* feature,
                          vector<vector<double>>* target) {
  if (feature->size() == 0 || target->size() == 0 ||
      feature->size() != target->size())
    return;
  feature_dim = (*feature)[0].size();
  target_dim = (*target)[0].size();
  if (feature_dim == 0 || target_dim == 0) return;
  if (feature_type.size() == 0)
    feature_type = vector<FeatureType>(feature_dim, FEATURE_NUM);
  if (feature_type.size() != feature_dim) return;
  printf("=========================================\n");
  printf("BaseTree Parameter\n");
  printf("%20s: %d\n", "Tree Type", tree_type);
  printf("%20s: %d\n", "Max Depth", max_depth);
  printf("%20s: %d\n", "Min Leaves", min_leaves);
  printf("%20s: %d\n", "Feature Dim", feature_dim);
  printf("%20s: %d\n", "Target Dim", target_dim);
  printf("=========================================\n");
  buildTree(*feature, *target, root);
}

void BaseTree::Predict(vector<vector<double>>* feature,
                       vector<vector<double>>* target) {
  target->clear();
  *target = predictLabel(*feature);
}

void BaseTree::PredictProb(vector<vector<double>>* feature,
                           vector<vector<double>>* target) {
  target->clear();
  *target = predict(*feature);
}

double BaseTree::Score(vector<vector<double>>* feature,
                       vector<vector<double>>* target) {
  double score = 0;
  if (feature->size() == 0) return score;
  auto t_pre = predictLabel(*feature);
  for (int i = 0; i < target->size(); i++) {
    for (int j = 0; j < (*target)[i].size(); j++) {
      if (t_pre[i][j] != (*target)[i][j]) {
        score++;
        break;
      }
    }
  }
  score = 1.0 - score / feature->size();
  return score;
}

vector<double> BaseTree::Scores(vector<vector<double>>* feature,
                                vector<vector<double>>* target) {
  vector<double> scores(target_dim, 0.0);
  if (feature->size() == 0) return scores;
  auto t_pre = predictLabel(*feature);
  for (int i = 0; i < target->size(); i++) {
    for (int j = 0; j < (*target)[i].size(); j++) {
      if (t_pre[i][j] != (*target)[i][j]) {
        scores[j]++;
        break;
      }
    }
  }
  for (int i = 0; i < target_dim; i++) {
    scores[i] = 1 - (scores[i]) / feature->size();
  }
  return scores;
}

void BaseTree::Train(SupervisedData* data) {
  auto feature = data->GetFeature();
  auto target = data->GetTarget();
  TrainByVec(feature, target);
}

void BaseTree::Predict(SupervisedData* data) {
  auto f = data->GetFeature();
  auto t = data->GetTarget();
  Predict(f, t);
}

void BaseTree::PredictProb(SupervisedData* data) {
  auto f = data->GetFeature();
  auto t = data->GetTarget();
  PredictProb(f, t);
}

double BaseTree::ScoreProb(SupervisedData* data) {
  auto f = data->GetFeature();
  auto t = data->GetTarget();
  return ScoreProb(f, t);
}

vector<double> BaseTree::ScoresProb(SupervisedData* data) {
  auto f = data->GetFeature();
  auto t = data->GetTarget();
  return ScoresProb(f, t);
}

double BaseTree::ScoreProb(vector<vector<double>>* feature,
                           vector<vector<double>>* target) {
  double score = 0;
  auto scores = ScoresProb(feature, target);
  for (int i = 0; i < target_dim; i++) {
    score += scores[i];
  }
  score = score / target_dim;
  return score;
}

vector<double> BaseTree::ScoresProb(vector<vector<double>>* feature,
                                    vector<vector<double>>* target) {
  vector<double> scores(target_dim, 0.0);
  if (feature->size() == 0) return scores;
  auto t_pre = predict(*feature);
  for (int i = 0; i < target->size(); i++) {
    for (int j = 0; j < (*target)[i].size(); j++) {
      scores[j] += abs(t_pre[i][j] - (*target)[i][j]);
    }
  }
  for (int i = 0; i < target_dim; i++) {
    scores[i] = (scores[i]) / feature->size();
  }
  return scores;
}

double BaseTree::Score(SupervisedData* data) {
  auto f = data->GetFeature();
  auto t = data->GetTarget();
  return Score(f, t);
}

vector<double> BaseTree::Scores(SupervisedData* data) {
  auto f = data->GetFeature();
  auto t = data->GetTarget();
  return Scores(f, t);
}

vector<vector<double>> BaseTree::predict(vector<vector<double>>& f) {
  vector<vector<double>> t;
  for (auto it = f.begin(); it != f.end(); it++) {
    t.push_back(predict(*it, root));
  }
  return t;
}

vector<double> BaseTree::predict(vector<double>& f, BaseNode* node) {
  for (auto it = node->children.begin(); it != node->children.end(); it++) {
    if ((*it)->IsIn(f)) {
      return predict(f, *it);
    }
  }
  return node->GetTarget();
}

vector<vector<double>> BaseTree::predictLabel(vector<vector<double>>& f) {
  auto t = predict(f);
  if (target_dim > 1) {
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
  } else {
    for (int i = 0; i < t.size(); i++) {
      t[i][0] = t[i][0] >= 0.5 ? 1 : 0;
    }
  }

  return t;
}

// 两种特殊情况
// ①所有特征相同但目标不同, 这时所有特征的score相同, 但err不等于0,
// 选第一个作为分裂特征 ②某特征只有一种取值, 则不在此节点分裂
void BaseTree::buildTree(vector<vector<double>>& feature,
                         vector<vector<double>>& target, BaseNode* node) {
  vector<vector<double>> feature_t, target_t;
  node->SetData(&feature, &target, &feature_t, &target_t);
  int sample_size = target_t.size();
  double score = 0;
  double e = ScoreProb(&feature_t, &target_t);
  vector<BaseNode*> nodes;
  printf(
      "Depth: %4d    Nodes: %8d  InputData: %5d  RetainData: %5d  Error: "
      "%-5.6f  \n",
      root->GetDepth(), root->GetSize(), (int)(feature.size()),
      (int)(feature_t.size()), e);
  if ((root->GetDepth() >= max_depth && max_depth != -1) ||
      sample_size <= min_leaves)
    return;
  if (e <= 1e-8) return;
  for (int i = 0; i < feature_dim; i++) {
    double score_i = 0;
    vector<BaseNode*> nodes_i;
    //  根据树的类型决定是用GINI系数还是用信息增益GAIN
    split(feature_t, target_t, i, score_i, nodes_i);
    if (i == 0) {
      score = score_i;
      nodes = nodes_i;
      continue;
    }
    if (score_i < score) {
      score = score_i;
      for (auto it = nodes.begin(); it != nodes.end(); it++) {
        delete *it;
      }
      nodes = nodes_i;
    } else {
      for (auto it = nodes_i.begin(); it != nodes_i.end(); it++) {
        delete *it;
      }
    }
  }
  for (auto it = nodes.begin(); it != nodes.end(); it++) {
    node->children.push_back(*it);
    buildTree(feature_t, target_t, *it);
  }
}

void BaseTree::split(vector<vector<double>>& feature,
                     vector<vector<double>>& target, int index, double& score,
                     vector<BaseNode*>& nodes) {
  if (feature_type[index] == FEATURE_STR) {
    set<double> range_set;
    for (int i = 0; i < target.size(); i++) {
      range_set.insert(feature[i][index]);
    }
    if (range_set.size() <= 1) return;
    for (auto i = range_set.begin(); i != range_set.end(); i++) {
      BaseNode* node = new BaseNode(index, *i);
      nodes.push_back(node);
    }
    if (tree_type == GAIN) {
      gain(feature, target, score, nodes);
    } else if (tree_type == GINI) {
      gini(feature, target, score, nodes);
    } else if (tree_type == CART) {
      cart(feature, target, score, nodes);
    }
  } else {
    set<double> range_set;
    vector<double> range_vec;
    for (int i = 0; i < target.size(); i++) {
      range_set.insert(feature[i][index]);
    }
    if (range_set.size() <= 1) return;
    range_vec.assign(range_set.begin(), range_set.end());
    sort(range_vec.begin(), range_vec.end());
    for (int i = 0; i < range_vec.size() - 1; i++) {
      double score_i = 0;
      vector<BaseNode*> nodes_i;
      double intercept = (range_vec[i] + range_vec[i + 1]) / 2;
      BaseNode* n1 = new BaseNode(index, intercept, true);
      BaseNode* n2 = new BaseNode(index, intercept, false);
      nodes_i.push_back(n1);
      nodes_i.push_back(n2);
      if (tree_type == GAIN) {
        gain(feature, target, score_i, nodes_i);
      } else if (tree_type == GINI) {
        gini(feature, target, score_i, nodes_i);
      } else if (tree_type == CART) {
        cart(feature, target, score_i, nodes_i);
      }
      if (i == 0) {
        score = score_i;
        nodes = nodes_i;
        continue;
      }
      if (score_i < score) {
        score = score_i;
        delete nodes[0], nodes[1];
        nodes = nodes_i;
      } else {
        delete n1, n2;
      }
    }
  }
}

void BaseTree::gain(vector<vector<double>>& feature,
                    vector<vector<double>>& target, double& score,
                    vector<BaseNode*>& nodes) {
  int sample_size = target.size();
  score = 0;
  map<int, pair<int, map<int, int>>> feature_classes;
  for (int i = 0; i < nodes.size(); i++) {
    map<int, int> m;
    for (int j = 0; j < target_dim; j++) m[j] = 0;
    feature_classes[i] = make_pair(0, m);
  }
  for (int i = 0; i < sample_size; i++) {
    int target_t = 0;
    int feature_index = -1;
    for (int n = 0; n < nodes.size(); n++) {
      if (nodes[n]->IsIn(feature[i])) {
        feature_index = n;
        break;
      }
    }
    if (feature_index == -1) continue;
    auto it = feature_classes.find(feature_index);
    it->second.first++;
    for (int j = 0; j < target_dim; j++) {
      if (target[i][j] != 0) {
        target_t = j;
        auto jt = it->second.second.find(target_t);
        jt->second++;
        break;
      }
    }
  }
  for (auto it = feature_classes.begin(); it != feature_classes.end(); it++) {
    double all_num = (double)(it->second.first);
    for (auto jt = it->second.second.begin(); jt != it->second.second.end();
         jt++) {
      double p1_num = (double)(jt->second);
      if (p1_num > 0) {
        score -= p1_num * log(p1_num / all_num);
      }
      if (target_dim == 1 && p1_num < all_num) {
        score -= (all_num - p1_num) * log((all_num - p1_num) / all_num);
      }
    }
  }
}

void BaseTree::gini(vector<vector<double>>& feature,
                    vector<vector<double>>& target, double& score,
                    vector<BaseNode*>& nodes) {
  int sample_size = target.size();
  score = 0;
  map<int, pair<int, map<int, int>>> feature_classes;
  for (int i = 0; i < nodes.size(); i++) {
    map<int, int> m;
    for (int j = 0; j < target_dim; j++) m[j] = 0;
    feature_classes[i] = make_pair(0, m);
  }
  for (int i = 0; i < sample_size; i++) {
    int target_t = 0;
    int feature_index = -1;
    for (int n = 0; n < nodes.size(); n++) {
      if (nodes[n]->IsIn(feature[i])) {
        feature_index = n;
        break;
      }
    }
    if (feature_index == -1) continue;
    auto it = feature_classes.find(feature_index);
    it->second.first++;
    for (int j = 0; j < target_dim; j++) {
      if (target[i][j] != 0) {
        target_t = j;
        auto jt = it->second.second.find(target_t);
        jt->second++;
        break;
      }
    }
  }
  int i = -1;
  for (auto it = feature_classes.begin(); it != feature_classes.end(); it++) {
    double score_it = 1;
    i++;
    double all_num = (double)(it->second.first);
    for (auto jt = it->second.second.begin(); jt != it->second.second.end();
         jt++) {
      double p1_num = (double)(jt->second);
      if (p1_num > 0) {
        score_it -= pow((p1_num / all_num), 2);
      }
      if (target_dim == 1 && p1_num < all_num) {
        score_it -= pow(((all_num - p1_num) / all_num), 2);
      }
    }
    score += (score_it)*all_num / feature.size();
  }
}

void BaseTree::cart(vector<vector<double>>& feature,
                    vector<vector<double>>& target, double& score,
                    vector<BaseNode*>& nodes) {
  int sample_size = target.size();
  score = 0;
  map<int, pair<int, vector<double>>> feature_classes;
  for (int i = 0; i < nodes.size(); i++) {
    feature_classes[i] = make_pair(0, vector<double>(target_dim, 0));
  }
  for (int i = 0; i < sample_size; i++) {
    int target_t = 0;
    int feature_index = -1;
    for (int n = 0; n < nodes.size(); n++) {
      if (nodes[n]->IsIn(feature[i])) {
        feature_index = n;
        break;
      }
    }
    if (feature_index == -1) continue;
    auto it = feature_classes.find(feature_index);
    it->second.first++;
    for (int j = 0; j < target_dim; j++) {
      it->second.second[j] += target[i][j];
    }
  }
  for (int i = 0; i < nodes.size(); i++) {
    for (int j = 0; j < target_dim; j++) {
      if (feature_classes[i].first > 0) {
        feature_classes[i].second[j] =
            feature_classes[i].second[j] / feature_classes[i].first;
      }
    }
  }
  for (int i = 0; i < sample_size; i++) {
    int target_t = 0;
    int feature_index = -1;
    for (int n = 0; n < nodes.size(); n++) {
      if (nodes[n]->IsIn(feature[i])) {
        feature_index = n;
        break;
      }
    }
    if (feature_index == -1) continue;
    auto it = feature_classes.find(feature_index);
    it->second.first++;
    for (int j = 0; j < target_dim; j++) {
      score += pow((it->second.second[j] - target[i][j]), 2);
    }
  }
  score = score / feature.size();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
TreeClassifier::TreeClassifier(TreeType tree_type, int max_depth,
                               int min_leaves, bool multi_label, int n_jobs) {
  this->tree_type = tree_type >= 0 && tree_type <= 1 ? tree_type : GINI;
  this->max_depth = max_depth;
  this->min_leaves = min_leaves;
  this->multi_label = multi_label;
  this->n_jobs = n_jobs <= 0 ? basic::GetCPUNum() : n_jobs;
  this->trees.clear();
  this->feature_type.clear();
}

TreeClassifier::~TreeClassifier() {
  for (auto it = trees.begin(); it != trees.end(); it++) delete *it;
  trees.clear();
}

void TreeClassifier::SetFeatureType(vector<FeatureType>& ft) {
  feature_type = ft;
}

void TreeClassifier::Train(vector<vector<double>>* feature,
                           vector<vector<double>>* target) {
  if (feature->size() == 0 || target->size() == 0 ||
      feature->size() != target->size())
    return;
  int feature_dim = (*feature)[0].size();
  int target_dim = (*target)[0].size();
  if (feature_dim == 0 || target_dim == 0) return;
  if (feature_type.size() == 0)
    feature_type = vector<FeatureType>(feature_dim, FEATURE_NUM);
  if (feature_type.size() != feature_dim) return;
  if (multi_label) {
    for (int i = 0; i < target_dim; i++)
      trees.push_back(new BaseTree(tree_type, max_depth, min_leaves));
  } else {
    trees.push_back(new BaseTree(tree_type, max_depth, min_leaves));
  }
  printf("=========================================\n");
  printf("TreeClassifier Parameter\n");
  printf("%20s: %d\n", "Tree Type", tree_type);
  printf("%20s: %d\n", "Max Depth", max_depth);
  printf("%20s: %d\n", "Min Leaves", min_leaves);
  printf("%20s: %d\n", "Multi Label", multi_label);
  printf("%20s: %d\n", "Feature Dim", feature_dim);
  printf("%20s: %d\n", "Target Dim", target_dim);
  printf("=========================================\n");
  if (trees.size() == 0) return;
  if (multi_label) {
    vector<vector<vector<double>>> target_copys;
    for (int i = 0; i < target_dim; i++)
      target_copys.push_back(vector<vector<double>>(feature->size(), {0}));
    for (int i = 0; i < target->size(); i++) {
      for (int j = 0; j < target_dim; j++) {
        target_copys[j][i][0] = (*target)[i][j];
      }
    }
    basic::ThreadPool pool(n_jobs);
    for (int i = 0; i < target_dim; i++) {
      function<void()> f;
      f = bind(&BaseTree::TrainByVec, &(*(trees[i])), feature,
               &(target_copys[i]));
      pool.AddJob(f);
    }
  } else {
    trees[0]->TrainByVec(feature, target);
  }
}

void TreeClassifier::Predict(vector<vector<double>>* feature,
                             vector<vector<double>>* target) {
  target->clear();
  *target = vector<vector<double>>(feature->size(), vector<double>{});
  if (trees.size() == 0) return;
  if (multi_label) {
    vector<vector<double>> t0;
    for (auto it = trees.begin(); it != trees.end(); it++) {
      (*it)->Predict(feature, &t0);
      auto kt = t0.begin();
      for (auto jt = target->begin(); jt != target->end(); jt++, kt++) {
        jt->push_back((*kt)[0]);
      }
    }
  } else {
    if (trees.size() == 1) {
      trees[0]->Predict(feature, target);
    }
  }
}

void TreeClassifier::PredictProb(vector<vector<double>>* feature,
                                 vector<vector<double>>* target) {
  target->clear();
  *target = vector<vector<double>>(feature->size(), vector<double>{});
  if (trees.size() == 0) return;
  if (multi_label) {
    vector<vector<double>> t0;
    for (auto it = trees.begin(); it != trees.end(); it++) {
      (*it)->PredictProb(feature, &t0);
      auto kt = t0.begin();
      for (auto jt = target->begin(); jt != target->end(); jt++, kt++) {
        jt->push_back((*kt)[0]);
      }
    }
  } else {
    if (trees.size() == 1) {
      trees[0]->PredictProb(feature, target);
    }
  }
}

double TreeClassifier::Score(vector<vector<double>>* feature,
                             vector<vector<double>>* target) {
  double score = 0;
  if (trees.size() == 0) return score;
  if (multi_label) {
    vector<vector<vector<double>>> target_copys;
    for (int i = 0; i < trees.size(); i++)
      target_copys.push_back(vector<vector<double>>(feature->size(), {0}));
    for (int i = 0; i < target->size(); i++) {
      for (int j = 0; j < trees.size(); j++) {
        target_copys[j][i][0] = (*target)[i][j];
      }
    }
    for (int i = 0; i < trees.size(); i++) {
      score += (trees[i])->Score(feature, &(target_copys[i]));
    }
    score /= trees.size();
  } else {
    if (trees.size() == 1) {
      score = trees[0]->Score(feature, target);
    }
  }
  return score;
}

vector<double> TreeClassifier::Scores(vector<vector<double>>* feature,
                                      vector<vector<double>>* target) {
  vector<double> scores;
  if (trees.size() == 0) return scores;
  if (multi_label) {
    vector<vector<vector<double>>> target_copys;
    for (int i = 0; i < trees.size(); i++)
      target_copys.push_back(vector<vector<double>>(feature->size(), {0}));
    for (int i = 0; i < target->size(); i++) {
      for (int j = 0; j < trees.size(); j++) {
        target_copys[j][i][0] = (*target)[i][j];
      }
    }
    for (int i = 0; i < trees.size(); i++) {
      auto scores0 = (trees[i])->Scores(feature, &(target_copys[i]));
      scores.push_back(scores0[0]);
    }
  } else {
    if (trees.size() == 1) {
      scores = trees[0]->Scores(feature, target);
    }
  }
  return scores;
}

void TreeClassifier::Train(SupervisedData* data) {
  auto feature = data->GetFeature();
  auto target = data->GetTarget();
  Train(feature, target);
}

void TreeClassifier::Predict(SupervisedData* data) {
  auto feature = data->GetFeature();
  auto target = data->GetTarget();
  Predict(feature, target);
}

void TreeClassifier::PredictProb(SupervisedData* data) {
  auto feature = data->GetFeature();
  auto target = data->GetTarget();
  PredictProb(feature, target);
}

double TreeClassifier::Score(SupervisedData* data) {
  auto feature = data->GetFeature();
  auto target = data->GetTarget();
  return Score(feature, target);
}

vector<double> TreeClassifier::Scores(SupervisedData* data) {
  auto feature = data->GetFeature();
  auto target = data->GetTarget();
  return Scores(feature, target);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
TreeRegression::TreeRegression(int max_depth, int min_leaves, int n_jobs) {
  this->max_depth = max_depth;
  this->min_leaves = min_leaves;
  this->n_jobs = n_jobs <= 0 ? basic::GetCPUNum() : n_jobs;
  this->trees.clear();
  this->feature_type.clear();
}

TreeRegression::~TreeRegression() {
  for (auto it = trees.begin(); it != trees.end(); it++) delete *it;
  trees.clear();
}

void TreeRegression::SetFeatureType(vector<FeatureType>& ft) {
  feature_type = ft;
}

void TreeRegression::Train(vector<vector<double>>* feature,
                           vector<vector<double>>* target) {
  if (feature->size() == 0 || target->size() == 0 ||
      feature->size() != target->size())
    return;
  int feature_dim = (*feature)[0].size();
  int target_dim = (*target)[0].size();
  if (feature_dim == 0 || target_dim == 0) return;
  if (feature_type.size() == 0)
    feature_type = vector<FeatureType>(feature_dim, FEATURE_NUM);
  if (feature_type.size() != feature_dim) return;
  for (int i = 0; i < target_dim; i++)
    trees.push_back(new BaseTree(CART, max_depth, min_leaves));
  printf("=========================================\n");
  printf("TreeRegression Parameter\n");
  printf("%20s: %d\n", "Max Depth", max_depth);
  printf("%20s: %d\n", "Min Leaves", min_leaves);
  printf("%20s: %d\n", "Feature Dim", feature_dim);
  printf("%20s: %d\n", "Target Dim", target_dim);
  printf("=========================================\n");
  if (trees.size() == 0) return;
  vector<vector<vector<double>>> target_copys;
  for (int i = 0; i < target_dim; i++)
    target_copys.push_back(vector<vector<double>>(feature->size(), {0}));
  for (int i = 0; i < target->size(); i++) {
    for (int j = 0; j < target_dim; j++) {
      target_copys[j][i][0] = (*target)[i][j];
    }
  }
  basic::ThreadPool pool(n_jobs);
  for (int i = 0; i < target_dim; i++) {
    function<void()> f;
    f = bind(&BaseTree::TrainByVec, &(*(trees[i])), feature,
             &(target_copys[i]));
    pool.AddJob(f);
  }
}

void TreeRegression::Predict(vector<vector<double>>* feature,
                             vector<vector<double>>* target) {
  target->clear();
  *target = vector<vector<double>>(feature->size(), vector<double>{});
  if (trees.size() == 0) return;
  vector<vector<double>> t0;
  for (auto it = trees.begin(); it != trees.end(); it++) {
    (*it)->PredictProb(feature, &t0);
    auto kt = t0.begin();
    for (auto jt = target->begin(); jt != target->end(); jt++, kt++) {
      jt->push_back((*kt)[0]);
    }
  }
}

double TreeRegression::Score(vector<vector<double>>* feature,
                             vector<vector<double>>* target) {
  double score = 0;
  if (trees.size() == 0) return score;
  vector<vector<vector<double>>> target_copys;
  for (int i = 0; i < trees.size(); i++)
    target_copys.push_back(vector<vector<double>>(feature->size(), {0}));
  for (int i = 0; i < target->size(); i++) {
    for (int j = 0; j < trees.size(); j++) {
      target_copys[j][i][0] = (*target)[i][j];
    }
  }
  for (int i = 0; i < trees.size(); i++) {
    score += (trees[i])->ScoreProb(feature, &(target_copys[i]));
  }
  score /= trees.size();
  return score;
}

vector<double> TreeRegression::Scores(vector<vector<double>>* feature,
                                      vector<vector<double>>* target) {
  vector<double> scores;
  if (trees.size() == 0) return scores;
  vector<vector<vector<double>>> target_copys;
  for (int i = 0; i < trees.size(); i++)
    target_copys.push_back(vector<vector<double>>(feature->size(), {0}));
  for (int i = 0; i < target->size(); i++) {
    for (int j = 0; j < trees.size(); j++) {
      target_copys[j][i][0] = (*target)[i][j];
    }
  }
  for (int i = 0; i < trees.size(); i++) {
    auto scores0 = (trees[i])->ScoresProb(feature, &(target_copys[i]));
    scores.push_back(scores0[0]);
  }
  return scores;
}

void TreeRegression::Train(SupervisedData* data) {
  auto feature = data->GetFeature();
  auto target = data->GetTarget();
  Train(feature, target);
}

void TreeRegression::Predict(SupervisedData* data) {
  auto feature = data->GetFeature();
  auto target = data->GetTarget();
  Predict(feature, target);
}

double TreeRegression::Score(SupervisedData* data) {
  auto feature = data->GetFeature();
  auto target = data->GetTarget();
  return Score(feature, target);
}

vector<double> TreeRegression::Scores(SupervisedData* data) {
  auto feature = data->GetFeature();
  auto target = data->GetTarget();
  return Scores(feature, target);
}
}  // namespace see
