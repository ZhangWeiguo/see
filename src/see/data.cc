#include "data.h"

#include <time.h>

#include <algorithm>
#include <iostream>
#include <random>
#include <set>
#include <vector>
using namespace std;

namespace see {

DMatrix::DMatrix() {
  data = new vector<vector<double>>;
  dim = 0;
}

DMatrix::~DMatrix() { delete data; }

int DMatrix::AddOneData(vector<double>* d) {
  if (d->size() == 0) return 0;
  if (dim == 0) dim = d->size();
  if (dim == d->size()) {
    data->push_back(*d);
    return 1;
  }
  return 0;
}

int DMatrix::AddData(vector<vector<double>>* d) {
  if (d->size() == 0) return 0;
  if (dim == 0 && (*d)[0].size() != 0) dim = (*d)[0].size();
  int add_num = 0;
  for (auto it = d->begin(); it != d->end(); it++) {
    if (it->size() == dim) {
      data->push_back(*it);
      add_num++;
    }
  }
  return add_num;
}

int DMatrix::AddData(DMatrix* d) {
  if (dim == 0 && d->dim != 0) dim = d->dim;
  int add_num = 0;
  if (dim == d->dim) {
    auto dd = d->GetData();
    data->insert(data->end(), dd->begin(), dd->end());
    add_num = d->GetSize();
  }
  return add_num;
}

int DMatrix::SetData(vector<vector<double>>* d) {
  int add_num = 0;
  if (d->size() == 0) return add_num;
  if ((*d)[0].size() == 0) return add_num;
  Clear();
  add_num = AddData(d);
  return add_num;
}

int DMatrix::SetData(DMatrix* d) {
  *data = *(d->GetData());
  dim = d->GetDim();
  return d->GetSize();
}

int DMatrix::ShareData(DMatrix* d) {
  data = d->GetData();
  dim = d->GetDim();
  return d->GetSize();
}

int DMatrix::ShareData(vector<vector<double>>* d) {
  data = d;
  if (d->size() > 0) dim = (*d)[0].size();
  return GetSize();
}

int DMatrix::Clear() {
  dim = 0;
  data->clear();
}

int DMatrix::GetSize() { return data->size(); }

int DMatrix::GetDim() { return dim; }

vector<vector<double>>* DMatrix::GetData() { return data; }

SupervisedData::SupervisedData() {
  feature = new DMatrix;
  target = new DMatrix;
  current_locate = 0;
}

SupervisedData::~SupervisedData() { delete feature, target; }

int SupervisedData::AddDataWithoutTarget(vector<vector<double>>* d) {
  int add_num = 0;
  if (d->size() == 0) return add_num;
  if ((*d)[0].size() == 0) return add_num;
  add_num = feature->AddData(d);
  return add_num;
}

int SupervisedData::AddOneDataWithoutTarget(vector<double>* d) {
  if (d->size() == 0) return 0;
  return feature->AddOneData(d);
}

int SupervisedData::AddDataWithoutFeature(vector<vector<double>>* d) {
  int add_num = 0;
  if (d->size() == 0) return add_num;
  if ((*d)[0].size() == 0) return add_num;
  add_num = target->AddData(d);
  return add_num;
}

int SupervisedData::AddOneDataWithoutFeature(vector<double>* d) {
  if (d->size() == 0) return 0;
  return target->AddOneData(d);
}

int SupervisedData::AddData(vector<vector<double>>* d,
                            vector<vector<double>>* t) {
  int add_num = 0;
  if (d->size() != t->size() || d->size() == 0 || t->size() == 0)
    return add_num;
  if ((*d)[0].size() == 0 || (*t)[0].size() == 0) return add_num;
  add_num = feature->AddData(d);
  add_num += target->AddData(t);
  return add_num;
}

int SupervisedData::AddOneData(vector<double>* d, vector<double>* t) {
  int add_num = 0;
  if (d->size() > 0 && t->size() > 0) {
    add_num = feature->AddOneData(d);
    add_num += target->AddOneData(t);
  }
  return add_num;
}

int SupervisedData::AddData(SupervisedData* d) {
  auto f = d->GetFeature();
  auto t = d->GetTarget();
  return AddData(f, t);
}

int SupervisedData::SetTarget(vector<vector<double>>* d) {
  int add_num = 0;
  add_num = target->SetData(d);
  return add_num;
}

int SupervisedData::SetFeature(vector<vector<double>>* f) {
  int add_num = 0;
  add_num = feature->SetData(f);
  return add_num;
}

int SupervisedData::SetData(SupervisedData* d) {
  auto f = d->GetFeature();
  auto t = d->GetTarget();
  feature->SetData(f);
  target->SetData(t);
  return GetFeatureSize() + GetTargetSize();
}

int SupervisedData::ShareFeature(vector<vector<double>>* d) {
  return feature->ShareData(d);
}

int SupervisedData::ShareTarget(vector<vector<double>>* d) {
  return target->ShareData(d);
}

int SupervisedData::ShareData(SupervisedData* d) {
  auto f = d->GetFeature();
  auto t = d->GetTarget();
  feature->ShareData(f);
  target->ShareData(t);
  return GetFeatureSize() + GetTargetSize();
}

int SupervisedData::Clear() {
  int num = GetFeatureSize() + GetTargetSize();
  feature->Clear();
  target->Clear();
  return num;
}

int SupervisedData::GetSize() {
  if (feature->GetSize() == target->GetSize()) {
    return feature->GetSize();
  }
  return -1;
}

int SupervisedData::GetFeatureSize() { return feature->GetSize(); }

int SupervisedData::GetTargetSize() { return target->GetSize(); }

int SupervisedData::GetFeatureDim() { return feature->GetDim(); }

int SupervisedData::GetTargetDim() { return target->GetDim(); }

int SupervisedData::GetTrainData(vector<vector<double>>& f,
                                 vector<vector<double>>& t) {
  int num = 0;
  f.clear();
  t.clear();
  if (GetFeatureSize() == GetFeatureSize()) {
    f = *(feature->GetData());
    t = *(target->GetData());
    num = GetFeatureSize();
  }
  return num;
}

vector<vector<double>>* SupervisedData::GetFeature() {
  return feature->GetData();
}

vector<vector<double>>* SupervisedData::GetTarget() {
  return target->GetData();
}

int SupervisedData::GetBatchData(SupervisedData& d, int batch_size) {
  int all_num = GetFeatureSize();
  int get_num = 0;
  if (GetFeatureSize() != GetTargetSize()) return get_num;
  if (current_locate == 0) {
    order = vector<int>(all_num, 0);
    for (int i = 0; i < all_num; i++) order[i] = i;
    random_shuffle(order.begin(), order.end());
  }
  if (current_locate >= all_num) {
    current_locate = 0;
    return 0;
  }
  d.Clear();
  if (current_locate >= all_num) {
    return get_num;
  }
  auto feature_data = feature->GetData();
  auto target_data = target->GetData();
  while (current_locate < all_num && get_num < batch_size) {
    get_num += d.AddOneData(&((*feature_data)[order[current_locate]]),
                            &((*target_data)[order[current_locate]]));
    current_locate++;
  }
  return get_num;
}

int SupervisedData::Split(SupervisedData& train_d, SupervisedData& test_d,
                          double test_size, int random_state) {
  int test_num = 0;
  int all_num = GetFeatureSize();
  if (GetFeatureSize() != GetTargetSize()) return test_num;
  default_random_engine e;
  if (random_state != -1) {
    e.seed(random_state);
  } else {
    time_t now;
    e.seed(now);
  }
  int all_test_num = (int)((double)all_num * test_size);
  set<int> test_index;
  while (test_index.size() < all_test_num) {
    int index = e() % all_num;
    if (test_index.count(index) == 0) {
      test_index.insert(index);
    }
  }
  auto feature_data = feature->GetData();
  auto target_data = target->GetData();
  for (int i = 0; i <= all_num; i++) {
    if (test_index.count(i) == 0) {
      train_d.AddOneData(&((*feature_data)[i]), &((*target_data)[i]));
    } else {
      test_d.AddOneData(&((*feature_data)[i]), &((*target_data)[i]));
      test_num++;
      test_index.erase(i);
    }
  }
  return test_num;
}

OneHotEncoder::OneHotEncoder() {
  dim = 0;
  dict.clear();
  inv_dict.clear();
}

OneHotEncoder::~OneHotEncoder() {}

void OneHotEncoder::Train(vector<vector<double>>* f) {
  if (f->size() == 0) return;
  if ((*f).size() == 0) return;
  int index = 0;
  for (auto it = f->begin(); it != f->end(); it++) {
    for (auto jt = it->begin(); jt != it->end(); jt++) {
      int k = (int)(*jt);
      auto kt = dict.find(k);
      if (kt == dict.end()) {
        inv_dict[index] = k;
        dict[k] = index++;
      }
    }
  }
  dim = dict.size();
}

void OneHotEncoder::Transform(vector<vector<double>>* f,
                              vector<vector<double>>* t) {
  if (f->size() == 0 || dim == 0) return;
  t->clear();
  for (auto it = f->begin(); it != f->end(); it++) {
    vector<double> t0 = vector<double>(dim, 0);
    for (auto jt = it->begin(); jt != it->end(); jt++) {
      int k = (int)(*jt);
      auto kt = dict.find(k);
      if (kt != dict.end()) {
        t0[dict[k]] = 1;
      }
    }
    t->push_back(t0);
  }
}

void OneHotEncoder::InvTransform(vector<vector<double>>* f,
                                 vector<vector<double>>* t) {
  if (f->size() == 0 || dim == 0) return;
  t->clear();
  for (auto it = f->begin(); it != f->end(); it++) {
    vector<double> t0;
    for (int j = 0; j < dim; j++) {
      if ((*it)[j] == 1) {
        t0.push_back(inv_dict[j]);
      }
    }
    t->push_back(t0);
  }
}

}  // namespace see
