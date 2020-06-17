#pragma once
#include <vector>

#include "data.h"
using namespace std;

namespace see {

class SupervisedBaseModel {
 public:
  virtual ~SupervisedBaseModel() {}
  virtual void Train(SupervisedData* data) = 0;
  virtual void Predict(SupervisedData* data) = 0;
  virtual double Score(SupervisedData* data) = 0;
};

}  // namespace see