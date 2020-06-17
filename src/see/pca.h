#include <vector>

#include "data.h"
#include "mat.h"
using namespace std;

namespace see {
class PCA {
 public:
  PCA(int n_components = 2);
  ~PCA();
  void Train(DoubleMat* feature);
  void Transform(DoubleMat* feature, DoubleMat* target);

 private:
  void CalCov(DoubleMat* feature, DoubleMat* cov);
  int feature_dim;
  int n_components;
  DoubleMat w;
};

}  // namespace see