#pragma once
#include <atomic>
#include <mutex>
#include <random>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

#include "../basic/file.h"
#include "../basic/forever.h"
using namespace std;

namespace see {

struct vocab {
  string word;
  long long cnt;
  vector<long long> path;  // hs路径
  vector<int> code;        // hs编码
  long long index;         // 按频率逆序编号
  vector<double> vec;      // 学习向量
};

bool compare_vocab(vocab* w1, vocab* w2);

class Word2Vec {
 public:
  Word2Vec();
  Word2Vec(bool cbow, int negative, int window, int min_count, int layer_size,
           int epoch, double alpha, double sample, int n_jobs, bool debug_mode,
           bool binary);
  ~Word2Vec();

  void AcceptSentences(vector<string> ss);
  void Init();
  void Train(string file_name);

  long long ReadVocabFromTrain(string file_name);
  long long ReadVocab(string file_name);
  long long SaveVocab(string file_name);
  long long SaveVec(string file_name);
  long long ReadVec(string file_name);
  void SaveModel(string file_name);
  void ReadModel(string file_name);

  vector<double> GetVec(string word);
  vector<pair<string, double>> Similiar(string word, int n = 50,
                                        bool cosine = true);

 private:
  void InitVocabIndex();
  void InitNeTable();
  void InitSigmoidTable();
  void InitHuffmanTree();
  void InitHsModel();
  void InitNeModel();
  void TrainThread(basic::FileRead* fr);
  void TrainSentences(vector<long long>& sentences);
  bool binary;
  bool cbow;
  bool skipgram;
  bool debug_mode;
  int window;
  int min_count;
  int n_jobs;
  long long layer_size;
  long long vocab_size;
  long long train_words;
  atomic<long long> curr_train_words;
  atomic<long long> last_train_words;
  int epoch;
  atomic<int> curr_epoch;
  double alpha;
  double sample;
  bool hs;
  int negative;
  unordered_map<string, vocab*>* vocabs;              // 词汇表
  vector<vocab*>* vocab_index;                        // 词汇排序后的index
  unordered_map<long long, vector<double>>* para_ne;  // NegativeSample模型参数
  unordered_map<long long, vector<double>>* para_hs;  // Hsoftmax模型参数
  vector<long long>* ne_table;                        // 负采样概率表
  vector<double>* sig_table;  // Sigmoid函数预计算对应表

  default_random_engine random_engine;
  mutex lock;
  basic::Clock clock;
};
}  // namespace see
