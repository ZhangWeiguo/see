#pragma once
#include <time.h>

#include <random>
#include <vector>

#include "mat.h"
#include "utils.h"
using namespace std;

namespace see {
class DataSet {
 public:
  static void GetIris(DoubleMat* featture, DoubleMat* target,
                      string path = "../dat/");
  static void GetDigits(DoubleMat* featture, DoubleMat* target,
                        string path = "../dat/");
  // noise ~ [0, 1)
  static DoubleMat MakeRandomRegression(DoubleMat* featture, DoubleMat* target,
                                        int feature_dim, int target_dim,
                                        int sample_size, double noise = 0);
  static DoubleMat MakeRandomClassification(DoubleMat* featture,
                                            DoubleMat* target, int feature_dim,
                                            int target_dim, int sample_size,
                                            bool multi_label, double noise = 0);
};

}  // namespace see
