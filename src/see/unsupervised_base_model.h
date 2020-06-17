#pragma once
#include <vector>

#include "data.h"
using namespace std;

namespace see {

class UnSupervisedBaseModel {
 public:
  virtual ~UnSupervisedBaseModel() {}
  virtual void Train(SupervisedData* data) = 0;
  virtual void Predict(SupervisedData* data) = 0;
};

}  // namespace see