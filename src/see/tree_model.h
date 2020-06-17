#pragma once
#include <vector>

#include "data.h"
#include "supervised_base_model.h"
using namespace std;

namespace see {

// 决策树类型
enum TreeType { GINI = 0, GAIN = 1, CART = 2 };

// 特征类型, 离散/连续
enum FeatureType { FEATURE_STR = 0, FEATURE_NUM = 1 };

class BaseNode {
 public:
  BaseNode();
  BaseNode(int i, double v);
  BaseNode(int i, double v, bool left);
  BaseNode(int i, double b, double u);
  ~BaseNode();
  bool IsIn(vector<double>& d);
  int GetDepth();
  int GetSize();
  void SetData(vector<vector<double>>* f, vector<vector<double>>* t,
               vector<vector<double>>* ff, vector<vector<double>>* tt);
  vector<double> GetTarget();
  vector<BaseNode*> children;

 private:
  int index;
  double val;
  double bottom;
  double upper;
  int limit;  // limit = 0: x = val; limit = 1: x <= upper; limit = 2: bottom <
              // x <= upper; limit = 3: x > bottom
  vector<double> target;
};

// 只能作单标签分类 Target 必须是OneHot形式
class BaseTree : public SupervisedBaseModel {
 public:
  BaseTree(TreeType tree_type = GINI, int max_depth = -1, int min_leaves = 1);
  ~BaseTree();
  void SetFeatureType(vector<FeatureType>& ft);
  void Train(SupervisedData* data);
  void Predict(SupervisedData* data);           // 预测标签
  double Score(SupervisedData* data);           // 准确性,越大越好
  vector<double> Scores(SupervisedData* data);  // 准确性,越大越好
  void PredictProb(SupervisedData* data);  // 预测概率或者回归数值
  double ScoreProb(SupervisedData* data);  // 误差, 越小越好
  vector<double> ScoresProb(SupervisedData* data);  // 误差, 越小越好

  void TrainByVec(vector<vector<double>>* feature,
                  vector<vector<double>>* target);
  void Predict(vector<vector<double>>* feature, vector<vector<double>>* target);
  double Score(vector<vector<double>>* feature, vector<vector<double>>* target);
  vector<double> Scores(vector<vector<double>>* feature,
                        vector<vector<double>>* target);
  void PredictProb(vector<vector<double>>* feature,
                   vector<vector<double>>* target);
  double ScoreProb(vector<vector<double>>* feature,
                   vector<vector<double>>* target);
  vector<double> ScoresProb(vector<vector<double>>* feature,
                            vector<vector<double>>* target);

 private:
  BaseNode* root;
  TreeType tree_type;
  vector<FeatureType> feature_type;
  int max_depth;
  int min_leaves;
  int feature_dim;
  int target_dim;
  vector<vector<double>> predict(vector<vector<double>>& f);
  vector<double> predict(vector<double>& f, BaseNode* node);
  vector<vector<double>> predictLabel(vector<vector<double>>& f);
  void buildTree(vector<vector<double>>& feature,
                 vector<vector<double>>& target, BaseNode* node);
  void split(vector<vector<double>>& feature, vector<vector<double>>& target,
             int index, double& score, vector<BaseNode*>& nodes);
  void gain(vector<vector<double>>& feature, vector<vector<double>>& target,
            double& score, vector<BaseNode*>& nodes);
  void gini(vector<vector<double>>& feature, vector<vector<double>>& target,
            double& score, vector<BaseNode*>& nodes);
  void cart(vector<vector<double>>& feature, vector<vector<double>>& target,
            double& score, vector<BaseNode*>& nodes);
};

class TreeClassifier : public SupervisedBaseModel {
 public:
  TreeClassifier(TreeType tree_type = GINI, int max_depth = -1,
                 int min_leaves = 1, bool multi_label = false, int n_jobs = 4);
  ~TreeClassifier();
  void SetFeatureType(vector<FeatureType>& ft);
  void Train(SupervisedData* data);
  void Predict(SupervisedData* data);
  void PredictProb(SupervisedData* data);
  double Score(SupervisedData* data);
  vector<double> Scores(SupervisedData* data);

  void Train(vector<vector<double>>* feature, vector<vector<double>>* target);
  void Predict(vector<vector<double>>* feature, vector<vector<double>>* target);
  void PredictProb(vector<vector<double>>* feature,
                   vector<vector<double>>* target);
  double Score(vector<vector<double>>* feature, vector<vector<double>>* target);
  vector<double> Scores(vector<vector<double>>* feature,
                        vector<vector<double>>* target);

 private:
  vector<BaseTree*> trees;
  vector<FeatureType> feature_type;
  bool multi_label;
  int n_jobs;
  TreeType tree_type;
  int max_depth;
  int min_leaves;
};

class TreeRegression : public SupervisedBaseModel {
 public:
  TreeRegression(int max_depth = -1, int min_leaves = 1, int n_jobs = 4);
  ~TreeRegression();
  void SetFeatureType(vector<FeatureType>& ft);
  void Train(SupervisedData* data);
  void Predict(SupervisedData* data);
  double Score(SupervisedData* data);
  vector<double> Scores(SupervisedData* data);

  void Train(vector<vector<double>>* feature, vector<vector<double>>* target);
  void Predict(vector<vector<double>>* feature, vector<vector<double>>* target);
  double Score(vector<vector<double>>* feature, vector<vector<double>>* target);
  vector<double> Scores(vector<vector<double>>* feature,
                        vector<vector<double>>* target);

 private:
  vector<BaseTree*> trees;
  vector<FeatureType> feature_type;
  int n_jobs;
  int max_depth;
  int min_leaves;
};
}  // namespace see