#ifndef DATA_H
#define DATA_H
#include <map>
#include <vector>
using namespace std;

namespace see {

class DMatrix {
 public:
  DMatrix();
  ~DMatrix();

  int AddOneData(vector<double>* d);
  int AddData(vector<vector<double>>* d);
  int AddData(DMatrix* d);
  int SetData(vector<vector<double>>* d);
  int SetData(DMatrix* d);
  int ShareData(DMatrix* d);
  int ShareData(vector<vector<double>>* d);

  int Clear();

  int GetSize();
  int GetDim();
  vector<vector<double>>* GetData();

 private:
  vector<vector<double>>* data;
  int dim;
};

// 有监督样本集
class SupervisedData {
 public:
  SupervisedData();
  ~SupervisedData();

  int AddOneData(vector<double>* d, vector<double>* t);
  int AddOneDataWithoutTarget(vector<double>* d);
  int AddDataWithoutTarget(vector<vector<double>>* d);
  int AddOneDataWithoutFeature(vector<double>* d);
  int AddDataWithoutFeature(vector<vector<double>>* d);
  int AddData(vector<vector<double>>* d, vector<vector<double>>* t);
  int AddData(SupervisedData* d);

  int SetFeature(vector<vector<double>>* d);
  int SetTarget(vector<vector<double>>* d);
  int SetData(SupervisedData* d);

  int ShareFeature(vector<vector<double>>* d);
  int ShareTarget(vector<vector<double>>* d);
  int ShareData(SupervisedData* d);

  int Clear();

  int GetSize();
  int GetFeatureSize();
  int GetTargetSize();
  int GetFeatureDim();
  int GetTargetDim();
  int GetTrainData(vector<vector<double>>& f, vector<vector<double>>& t);
  int GetBatchData(SupervisedData& d, int batch_size);
  vector<vector<double>>* GetFeature();
  vector<vector<double>>* GetTarget();
  int Split(SupervisedData& train_d, SupervisedData& test_d, double test_size,
            int random_state = -1);

 private:
  DMatrix* feature;
  DMatrix* target;
  vector<int> order;
  int current_locate;
};

class OneHotEncoder {
 public:
  OneHotEncoder();
  ~OneHotEncoder();
  void Train(vector<vector<double>>* f);
  void Transform(vector<vector<double>>* f, vector<vector<double>>* t);
  void InvTransform(vector<vector<double>>* f, vector<vector<double>>* t);

 private:
  int dim;
  map<int, int> dict;
  map<int, int> inv_dict;
};

}  // namespace see
#endif
