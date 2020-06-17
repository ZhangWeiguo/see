#include "pca.h"
namespace see {

PCA::PCA(int n_components) {
  this->n_components = n_components > 0 ? n_components : 2;
}

PCA::~PCA() {}

void PCA::Train(DoubleMat* feature) {
  int n_smaples = feature->size();
  if (n_smaples == 0) return;
  feature_dim = feature->at(0).size();
  if (feature_dim == 0) return;
  w = ZerosMat<double>(feature_dim, feature_dim);
  n_components = min(n_components, n_smaples);
  n_components = min(n_components, feature_dim);
  DoubleMat d = *feature;
  vector<double> avg;
  DoubleMat cov = ZerosMat<double>(feature_dim, feature_dim);
  MatAvg<double>(avg, *feature, true);
  for (int i = 0; i < feature_dim; i++) avg[i] = -avg[i];
  MatAdd(d, d, avg, true);
}

void PCA::CalCov(DoubleMat* feature, DoubleMat* cov) {}

void PCA::Transform(DoubleMat* feature, DoubleMat* target) {
  int n_smaples = feature->size();
  if (n_smaples == 0) return;
  if (feature->at(0).size() != feature_dim) return;
}

}  // namespace see