#include "svm.h"
using namespace std;

namespace see {
SVMClassifier::SVMClassifier(double c, double tolerance, int epoch,
                             bool multi_label, int n_jobs) {
  this->c = c > 0 ? c : 1.0;
  this->epoch = epoch >= 1 ? epoch : 1000;
  this->multi_label = multi_label;
  this->tolerance = tolerance > 0 ? tolerance : 1e-4;
  this->kernel = false;
  this->n_jobs = n_jobs <= 0 ? basic::GetCPUNum() : n_jobs;
  w = new DoubleMat;
  kernel_sample = new DoubleMat;
}

SVMClassifier::SVMClassifier(double c, double tolerance, KernelFun kf,
                             int epoch, bool multi_label, double d,
                             double delta, double beta, double sita,
                             int n_jobs) {
  this->c = c > 0 ? c : 0.5;
  this->epoch = epoch >= 1 ? epoch : 1000;
  this->multi_label = multi_label;
  this->tolerance = tolerance > 0 ? tolerance : 1e-4;
  this->kernel = true;
  this->n_jobs = n_jobs <= 0 ? basic::GetCPUNum() : n_jobs;
  w = new DoubleMat;
  kernel_sample = new DoubleMat;
  d = d >= 1 ? d : 2;
  delta = delta > 0 ? delta : 1;
  beta = beta > 0 ? beta : 1;
  sita = sita < 0 ? sita : -0.5;
  switch (kf) {
    case 0:
      kernel_fun = bind(linearKernel, placeholders::_1, placeholders::_2);
      break;
    case 1:
      kernel_fun = bind(polyKernel, placeholders::_1, placeholders::_2, d);
      break;
    case 2:
      kernel_fun = bind(gaussKernel, placeholders::_1, placeholders::_2, delta);
      break;
    case 3:
      kernel_fun =
          bind(laplaceKernel, placeholders::_1, placeholders::_2, delta);
      break;
    case 4:
      kernel_fun =
          bind(sigmoidKernel, placeholders::_1, placeholders::_2, beta, sita);
      break;
    default:
      break;
  }
}

SVMClassifier::~SVMClassifier() { delete w, kernel_sample; }

void SVMClassifier::Train(SupervisedData* data) {
  auto feature = data->GetFeature();
  auto target = data->GetTarget();
  Train(feature, target);
}

void SVMClassifier::Train(vector<vector<double>>* feature,
                          vector<vector<double>>* target) {
  time_t now;
  random_engine.seed(now);
  if (feature->size() == 0 || feature->size() != target->size()) return;
  if (feature->at(0).size() == 0 || target->at(0).size() == 0) return;
  int sample_size = feature->size();
  feature_dim = feature->at(0).size();
  target_dim = target->at(0).size();
  if (!kernel)
    w_dim = feature_dim + 1;
  else
    w_dim = sample_size + 1;
  *w = ZerosMat<double>(target_dim, w_dim);
  *kernel_sample = *feature;
  DoubleMat feature_t;
  KernelTransform(feature, &feature_t);
  basic::ThreadPool pool(n_jobs);
  for (int i = 0; i < target_dim; i++) {
    function<void()> f;
    f = bind(&SVMClassifier::SMO, this, feature, target, &feature_t, i);
    pool.AddJob(f);
  }
}

void SVMClassifier::SMO(vector<vector<double>>* feature,
                        vector<vector<double>>* target,
                        vector<vector<double>>* cross_feature, int index) {
  uniform_real_distribution<double> re(0, c);
  vector<double> alpha(feature->size(), 0);
  vector<double> err(feature->size(), 0);
  vector<int> alpha_b;
  set<int> nobound;
  int k1, k2, k;
  double L, H, f1, f2, E1, E2, eta, v2old, v1old, bold;
  double Ek, yk, fk, alphak, alphaknew, gapk, maxigap, kv, maxjv;
  double b = re(random_engine), b1, b2, y1, y2, c11, c22, c12;
  for (int j = 0; j < target->size(); j++) {
    err[j] = b - (target->at(j)[index] > 0.5 ? 1 : -1);
  }
  for (int i = 0; i < epoch; i++) {
    alpha_b.clear();
    // 违反KKT条件 不在边界上
    for (auto p = nobound.begin(); p != nobound.end(); p++) {
      k = *p;
      alphak = alpha.at(k);
      yk = target->at(k)[index] > 0.5 ? 1 : -1;
      fk = err.at(k) + yk;
      if ((alphak == 0 && yk * fk < 1) ||
          (alphak >= 0 && alphak <= c && yk * fk != 1) ||
          (alphak == c && yk * fk > 1)) {
        alpha_b.emplace_back(k);
      }
    }
    // 违反KKT条件 在边界上
    if (alpha_b.size() == 0) {
      for (k = 0; k < target->size(); k++) {
        if (nobound.count(k) > 0) continue;
        alphak = alpha.at(k);
        yk = target->at(k)[index] > 0.5 ? 1 : -1;
        fk = err.at(k) + yk;
        if ((alphak == 0 && yk * fk < 1) ||
            (alphak >= 0 && alphak <= c && yk * fk != 1) ||
            (alphak == c && yk * fk > 1)) {
          alpha_b.emplace_back(k);
        }
      }
    }
    if (alpha_b.size() == 0) {
      for (k = 0; k < target->size(); k++) alpha_b.emplace_back(k);
    }
    for (int p = 0; p < alpha_b.size(); p++) {
      k1 = alpha_b[p];
      y1 = target->at(k1)[index] > 0.5 ? 1 : -1;
      E1 = err.at(k1);
      f1 = E1 + y1;
      v1old = alpha[k1];
      c11 = cross_feature->at(k1)[k1];
      bold = b;
      k2 = -1;
      // 违反KKT条件
      if (k2 == -1) {
        for (k = 0; k < target->size(); k++) {
          // cout << index << ":" << k1 << ";" << k << endl;
          if (k == i) continue;
          alphak = alpha.at(k);
          yk = target->at(k)[index] > 0.5 ? 1 : -1;
          Ek = err.at(k);
          fk = Ek + yk;
          if (k1 >= k)
            eta = 2 * cross_feature->at(k1)[k] - c11 - cross_feature->at(k)[k];
          else
            eta = 2 * cross_feature->at(k)[k1] - c11 - cross_feature->at(k)[k];
          if (y1 != yk) {
            L = alphak - v1old > 0 ? alphak - v1old : 0;
            H = c + alphak - v1old > c ? c : c + alphak - v1old;
          } else {
            L = alphak + v1old - c > 0 ? alphak + v1old - c : 0;
            H = alphak + v1old > c ? c : alphak + v1old;
          }
          if (eta == 0 || L == H) continue;
          alphaknew = alphak - yk * (E1 - Ek) / eta;
          if (alphaknew > H) alphaknew = H;
          if (alphaknew < L) alphaknew = L;
          kv = abs(alphaknew - alphak);
          if (k2 == -1 || kv > maxjv) {
            k2 = k;
            maxjv = kv;
          }
        }
      }
      if (k2 == -1) k2 = random_engine() % target->size();
      if (k1 == k2) continue;
      v2old = alpha[k2];
      y2 = target->at(k2)[index] > 0.5 ? 1 : -1;
      c22 = cross_feature->at(k2)[k2];
      if (k1 <= k2)
        c12 = cross_feature->at(k2)[k1];
      else
        c12 = cross_feature->at(k1)[k2];
      E2 = err[k2];
      f2 = E2 + y2;
      if (y1 != y2) {
        L = v2old - v1old > 0 ? v2old - v1old : 0;
        H = c + v2old - v1old > c ? c : c + v2old - v1old;
      } else {
        L = v2old + v1old - c > 0 ? v2old + v1old - c : 0;
        H = v2old + v1old > c ? c : v2old + v1old;
      }
      eta = 2 * c12 - c11 - c22;
      if (eta >= 0 || L == H) continue;
      alpha[k2] -= y2 * (E1 - E2) / eta;
      if (alpha[k2] > H) alpha[k2] = H;
      if (alpha[k2] < L) alpha[k2] = L;
      alpha[k1] += y1 * y2 * (v2old - alpha[k2]);
      b1 = b - E1 - y1 * (alpha[k1] - v1old) * c11 -
           y2 * (alpha[k2] - v2old) * c12;
      b2 = b - E2 - y1 * (alpha[k1] - v1old) * c12 -
           y2 * (alpha[k2] - v2old) * c22;
      if (alpha[k1] > 0 && alpha[k1] < c)
        b = b1;
      else if (alpha[k2] > 0 && alpha[k2] < c)
        b = b2;
      else
        b = (b1 + b2) / 2;
      // 更新alpha是否在边界
      {
        if (alpha[k1] > 0 && alpha[k1] < c) {
          nobound.insert(k1);
        } else {
          if (nobound.count(k1) > 0) nobound.erase(k1);
        }
        if (alpha[k2] > 0 && alpha[k2] < c) {
          nobound.insert(k2);
        } else {
          if (nobound.count(k2) > 0) nobound.erase(k2);
        }
      }
      // 更新每个样本的误差
      {
        for (int j = 0; j < target->size(); j++) {
          err[j] += (b - bold);
          if (j >= k1)
            err[j] += cross_feature->at(j)[k1] * y1 * (alpha[k1] - v1old);
          else
            err[j] += cross_feature->at(k1)[j] * y1 * (alpha[k1] - v1old);
          if (j >= k2)
            err[j] += cross_feature->at(j)[k2] * y2 * (alpha[k2] - v2old);
          else
            err[j] += cross_feature->at(k2)[j] * y2 * (alpha[k2] - v2old);
        }
      }
    }
  }
  if (kernel) {
    for (int k = 0; k < feature->size(); k++) {
      (*w)[index][k] = alpha[k] * (target->at(k)[index] > 0.5 ? 1 : -1);
    }
    (*w)[index][w_dim - 1] = b;
  } else {
    for (int k = 0; k < feature->size(); k++) {
      for (int i = 0; i < feature_dim; i++) {
        (*w)[index][i] += alpha[k] * feature->at(k)[i] *
                          (target->at(k)[index] > 0.5 ? 1 : -1);
      }
    }
    (*w)[index][w_dim - 1] = b;
  }
}

void SVMClassifier::KernelTransform(vector<vector<double>>* feature,
                                    vector<vector<double>>* feature_t) {
  feature_t->clear();
  double temp = 0;
  for (int i = 0; i < feature->size(); i++) {
    vector<double> v0(i + 1, 0);
    for (int j = 0; j <= i; j++) {
      if (!kernel) {
        temp = linearKernel(feature->at(i), feature->at(j));  // 直接内积
      } else {
        temp = kernel_fun(feature->at(i), feature->at(j));  // 核函数
      }
      v0[j] = temp;
    }
    feature_t->emplace_back(v0);
  }
}

void SVMClassifier::Predict(SupervisedData* data) {
  auto feature = data->GetFeature();
  auto target = data->GetTarget();
  Predict(feature, target);
}

void SVMClassifier::Predict(vector<double>* f, vector<double>* t) {
  double maxdis, dis;
  int index;
  *t = vector<double>(target_dim, 0);
  for (int j = 0; j < target_dim; j++) {
    dis = w->at(j)[w_dim - 1];
    for (int p = 0; p < w_dim - 1; p++) {
      if (w->at(j)[p] != 0) {
        if (kernel)
          dis += w->at(j)[p] * kernel_fun(*f, kernel_sample->at(p));
        else
          dis += w->at(j)[p] * f->at(p);
      }
    }
    if (multi_label || target_dim == 1) {
      if (dis > 0) (*t)[j] = 1;
    } else {
      if (j == 0 || dis > maxdis) {
        maxdis = dis;
        index = j;
      }
    }
  }
  if (!multi_label && target_dim != 1) (*t)[index] = 1;
}

void SVMClassifier::Predict(vector<vector<double>>* feature,
                            vector<vector<double>>* target) {
  target->clear();
  for (int i = 0; i < feature->size(); i++) {
    vector<double> t0(target_dim, 0);
    Predict(&(feature->at(i)), &t0);
    target->emplace_back(t0);
  }
}

double SVMClassifier::Predict(int n, int index, vector<vector<double>>* target,
                              double* b, vector<double>* alpha,
                              vector<vector<double>>* cross_feature) {
  double dis = *b;
  for (int p = 0; p < alpha->size(); p++) {
    if (p >= n)
      dis += alpha->at(p) * (target->at(p)[index] > 0.5 ? 1 : -1) *
             cross_feature->at(p)[n];
    else
      dis += alpha->at(p) * (target->at(p)[index] > 0.5 ? 1 : -1) *
             cross_feature->at(n)[p];
  }
  return dis;
}

double SVMClassifier::Error(int n, int index, vector<vector<double>>* target,
                            double* b, vector<double>* alpha,
                            vector<vector<double>>* cross_feature) {
  double dis = Predict(n, index, target, b, alpha, cross_feature);
  return dis - (target->at(n)[index] > 0.5 ? 1 : -1);
}

double SVMClassifier::Score(SupervisedData* data) {
  auto feature = data->GetFeature();
  auto target = data->GetTarget();
  return Score(feature, target);
}

double SVMClassifier::Score(vector<vector<double>>* feature,
                            vector<vector<double>>* target) {
  double score = 0;
  if (feature->size() == 0 || feature->size() != target->size()) return score;
  DoubleMat target_pre;
  Predict(feature, &target_pre);
  for (int i = 0; i < target->size(); i++) {
    for (int j = 0; j < (*target)[i].size(); j++) {
      if (target_pre[i][j] != (*target)[i][j]) {
        score++;
        if (!multi_label) break;
      }
    }
  }
  if (multi_label)
    score = 1.0 - score / feature->size() / target_dim;
  else
    score = 1.0 - score / feature->size();
  return score;
}

vector<double> SVMClassifier::Scores(SupervisedData* data) {
  auto feature = data->GetFeature();
  auto target = data->GetTarget();
  return Scores(feature, target);
}

vector<double> SVMClassifier::Scores(vector<vector<double>>* feature,
                                     vector<vector<double>>* target) {
  vector<double> scores;
  DoubleMat target_pre;
  if (feature->size() == 0) return scores;
  Predict(feature, &target_pre);
  for (int i = 0; i < target->size(); i++) {
    for (int j = 0; j < (*target)[i].size(); j++) {
      scores[j] += abs(target_pre[i][j] - (*target)[i][j]);
    }
  }
  for (int i = 0; i < target_dim; i++) {
    scores[i] = 1 - (scores[i]) / feature->size();
  }
  return scores;
}
}  // namespace see