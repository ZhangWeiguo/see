#include "word2vec.h"

#include <math.h>

#include <algorithm>
#include <functional>
#include <iostream>

#include "../basic/thread_pool.h"
#include "../basic/utils.h"
#include "mat.h"
#include "stdio.h"

using namespace std;

namespace see {

#define EXP_SIGMOID_TABLE_SIZE 1000
#define MAX_EXP 6
#define MAX_SENTENCE_LENGTH 1000
#define MAX_CODE_LENGTH 40
#define DEFAULT_NE_TABLE_SIZE 1e8
bool compare_vocab(vocab* w1, vocab* w2) { return w1->cnt > w2->cnt; };

Word2Vec::Word2Vec(bool cbow, int negative, int window, int min_count,
                   int layer_size, int epoch, double alpha, double sample,
                   int n_jobs, bool debug_mode, bool binary) {
  this->vocabs = new unordered_map<string, vocab*>;
  this->para_ne = new unordered_map<long long, vector<double>>;
  this->para_hs = new unordered_map<long long, vector<double>>;
  this->ne_table = new vector<long long>;
  this->sig_table = new vector<double>;
  this->vocab_index = new vector<vocab*>;
  this->binary = binary;
  this->cbow = cbow;
  this->skipgram = cbow;
  this->debug_mode = debug_mode;
  this->window = window;
  this->min_count = min_count;
  this->n_jobs = n_jobs;
  this->layer_size = layer_size;
  this->vocab_size = 0;
  this->epoch = epoch;
  this->curr_epoch.store(0);
  this->alpha = alpha;
  this->sample = sample;
  this->hs = negative > 0 ? false : true;
  this->negative = negative;
  if (this->debug_mode) {
    printf("=========================================\n");
    printf("Word2Vec Parameter\n");
    printf("%20s: %d\n", "CBOW", this->cbow);
    printf("%20s: %d\n", "SkipGram", this->skipgram);
    printf("%20s: %d\n", "HierarchicalSoftmax", this->hs);
    printf("%20s: %d\n", "NegativeSample", this->negative);
    printf("%20s: %d\n", "WordMinCount", this->min_count);
    printf("%20s: %d\n", "Window", this->window);
    printf("%20s: %lld\n", "LayerSize", this->layer_size);
    printf("%20s: %d\n", "Epoch", this->epoch);
    printf("%20s: %f\n", "LearnRate", this->alpha);
    printf("%20s: %f\n", "Sample", this->sample);
    printf("%20s: %d\n", "ThreadNum", this->n_jobs);
    printf("=========================================\n");
  }
  this->train_words = 0;
  this->curr_train_words.store(0);
  this->last_train_words.store(0);
  time_t now;
  time(&now);
  this->random_engine.seed(now);
  clock.Start();
}

Word2Vec::Word2Vec() {
  vocabs = new unordered_map<string, vocab*>;
  para_ne = new unordered_map<long long, vector<double>>;
  para_hs = new unordered_map<long long, vector<double>>;
  ne_table = new vector<long long>;
  sig_table = new vector<double>;
  vocab_index = new vector<vocab*>;
  binary = false;
  cbow = true;
  skipgram = false;
  debug_mode = true;
  window = 5;
  min_count = 2;
  n_jobs = 16;
  layer_size = 128;
  vocab_size = 0;
  epoch = 5;
  curr_epoch.store(0);
  alpha = 0.05;
  sample = 0.001;
  hs = false;
  negative = 5;
  if (debug_mode) {
    printf("=========================================\n");
    printf("Word2Vec Parameter\n");
    printf("%20s: %d\n", "CBOW", cbow);
    printf("%20s: %d\n", "SkipGram", skipgram);
    printf("%20s: %d\n", "HierarchicalSoftmax", hs);
    printf("%20s: %d\n", "NegativeSample", negative);
    printf("%20s: %d\n", "WordMinCount", min_count);
    printf("%20s: %d\n", "Window", window);
    printf("%20s: %lld\n", "LayerSize", layer_size);
    printf("%20s: %d\n", "Epoch", epoch);
    printf("%20s: %f\n", "LearnRate", alpha);
    printf("%20s: %f\n", "Sample", sample);
    printf("%20s: %d\n", "ThreadNum", n_jobs);
    printf("=========================================\n");
  }
  train_words = 0;
  curr_train_words.store(0);
  last_train_words.store(0);
  time_t now;
  time(&now);
  random_engine.seed(now);
  clock.Start();
};

Word2Vec::~Word2Vec() {
  for (auto it = vocabs->begin(); it != vocabs->end(); it++) {
    delete it->second;
    it->second = NULL;
  }
  vocab_index->clear();
  vocabs->clear();
  para_ne->clear();
  para_hs->clear();
  ne_table->clear();
  sig_table->clear();
  vocab_index->clear();
  clock.Stop();
  delete vocab_index, vocabs, para_ne, para_hs, ne_table, sig_table;
}

long long Word2Vec::SaveVec(string file_name) {
  if (debug_mode)
    printf("Timer %6lldS Save Vocab Vec To %s Start\n",
           (long long)(clock.GetTime()), file_name.data());
  basic::FileWrite fw(file_name, binary);
  string line_s, word, vec;
  auto end = vocab_index->end() - 1;
  for (auto it = vocab_index->begin(); it != vocab_index->end(); it++) {
    line_s.clear();
    word.clear();
    vec.clear();
    word = (*it)->word;
    vec = basic::MergeStr<double>((*it)->vec, ",");
    line_s = word + ":" + vec;
    if (it != end)
      fw.WriteLine(line_s);
    else
      fw.Write(line_s);
  }
  fw.Close();
  if (debug_mode)
    printf("Timer %6lldS Save Vocab Vec To %s End\n",
           (long long)(clock.GetTime()), file_name.data());
  return vocab_index->size();
}

long long Word2Vec::ReadVec(string file_name) {
  if (debug_mode)
    printf("Timer %6lldS Read Vocab Vec From %s Start\n",
           (long long)(clock.GetTime()), file_name.data());
  basic::FileRead fr(file_name, binary);
  string line;
  long long cnt = 0;
  while (fr.ReadLine(line)) {
    auto words = basic::SplitStr(line, ":");
    if (words.size() == 2) {
      vocab* new_word = new vocab;
      new_word->vec = basic::SplitStr<double>(words[1], ",");
      new_word->word = words[0];
      auto kv = vocabs->find(new_word->word);
      if (kv == vocabs->end()) {
        vocabs->insert(make_pair(new_word->word, new_word));
        cnt++;
      } else {
        kv->second->vec = new_word->vec;
        delete new_word;
      }
    }
  }
  if (debug_mode)
    printf("Timer %6lldS Read Vocab Vec From %s End\n",
           (long long)(clock.GetTime()), file_name.data());
  return cnt;
}

void Word2Vec::SaveModel(string file_name) {
  if (debug_mode)
    printf("Timer %6lldS Save Model To %s Start\n",
           (long long)(clock.GetTime()), file_name.data());
  basic::FileWrite fw(file_name, binary);
  string line_s, word, vec, code, path, hs, ne;
  long long index, cnt;
  auto end = vocab_index->end() - 1;
  for (auto it = vocab_index->begin(); it != vocab_index->end(); it++) {
    line_s.clear();
    hs.clear();
    ne.clear();
    word = (*it)->word;
    index = (*it)->index;
    cnt = (*it)->cnt;
    vec = basic::MergeStr<double>((*it)->vec);
    code = basic::MergeStr<int>((*it)->code);
    path = basic::MergeStr<long long>((*it)->path);
    auto ph = para_hs->find(index);
    if (ph != para_hs->end()) {
      hs = basic::MergeStr<double>(ph->second);
    }
    auto pn = para_ne->find(index);
    if (pn != para_ne->end()) {
      ne = basic::MergeStr<double>(pn->second);
    }
    line_s = word + ":" + to_string(index) + ":" + to_string(cnt) + ":" + vec +
             ":" + code + ":" + path + ":" + hs + ":" + ne;
    if (it != end)
      fw.WriteLine(line_s);
    else
      fw.Write(line_s);
  }
  fw.Close();
  if (debug_mode)
    printf("Timer %6lldS Save Model To %s End\n", (long long)(clock.GetTime()),
           file_name.data());
}

void Word2Vec::ReadModel(string file_name) {
  if (debug_mode)
    printf("Timer %6lldS Read Model From %s Start\n",
           (long long)(clock.GetTime()), file_name.data());
  basic::FileRead fr(file_name, binary);
  string line;
  while (fr.ReadLine(line)) {
    auto words = basic::SplitStr(line, ":");
    if (words.size() == 8) {
      vocab* new_word = new vocab;
      new_word->word = words[0];
      new_word->index = atoll(words[1].data());
      new_word->cnt = atoll(words[2].data());
      new_word->vec = basic::SplitStr<double>(words[3]);
      new_word->code = basic::SplitStr<int>(words[4]);
      new_word->path = basic::SplitStr<long long>(words[5]);
      (*para_hs)[new_word->index] = basic::SplitStr<double>(words[6]);
      (*para_ne)[new_word->index] = basic::SplitStr<double>(words[7]);
      (*vocabs)[new_word->word] = new_word;
    }
  }
  if (debug_mode)
    printf("Timer %6lldS Read Model From %s End\n",
           (long long)(clock.GetTime()), file_name.data());
  InitVocabIndex();
  InitNeTable();
  InitSigmoidTable();
}

long long Word2Vec::SaveVocab(string file_name) {
  if (debug_mode)
    printf("Timer %6lldS Save Model To %s Start\n",
           (long long)(clock.GetTime()), file_name.data());
  basic::FileWrite fw(file_name, binary);
  string line_s;
  auto end = vocab_index->end() - 1;
  for (auto it = vocab_index->begin(); it != vocab_index->end(); it++) {
    line_s = (*it)->word + ":" + to_string((*it)->cnt);
    if (it != (end)) {
      fw.WriteLine(line_s);
    } else {
      fw.Write(line_s);
    }
  }
  fw.Close();
  if (debug_mode)
    printf("Timer %6lldS Save Model To %s End\n", (long long)(clock.GetTime()),
           file_name.data());
  return vocab_index->size();
}

long long Word2Vec::ReadVocab(string file_name) {
  if (debug_mode)
    printf("Timer %6lldS Read Vocab From %s Start\n",
           (long long)(clock.GetTime()), file_name.data());
  basic::FileRead fr(file_name, binary);
  string line;
  long long cnt = 0;
  while (fr.ReadLine(line)) {
    auto words = basic::SplitStr(line, ":");
    if (words.size() == 2) {
      vocab* new_word = new vocab;
      new_word->cnt = atoi(words[1].c_str());
      new_word->word = words[0];
      auto kv = vocabs->find(new_word->word);
      if (kv == vocabs->end()) {
        vocabs->insert(make_pair(new_word->word, new_word));
        cnt++;
      } else {
        kv->second->cnt = new_word->cnt;
        delete new_word;
      }
    }
    if (debug_mode) {
      if (cnt % 10000 == 0)
        printf("Timer %6lldS Read Vocab: %10lld Words\n",
               (long long)(clock.GetTime()), cnt);
    }
  }
  if (debug_mode)
    printf("Timer %6lldS Read Vocab From %s End\n",
           (long long)(clock.GetTime()), file_name.data());
  return cnt;
}

long long Word2Vec::ReadVocabFromTrain(string file_name) {
  if (debug_mode)
    printf("Timer %6lldS Read Vocab From %s Start\n",
           (long long)(clock.GetTime()), file_name.data());
  basic::FileRead fr(file_name, binary);
  string line;
  long long last_all_words = 0, all_words = 0;
  while (fr.ReadLine(line)) {
    auto words = basic::SplitStr(line, " ");
    all_words += words.size();
    if (words.size() >= 1) AcceptSentences(words);
    if (debug_mode) {
      if (all_words - last_all_words > 1000000) {
        last_all_words = all_words;
        printf("Timer %6lldS Read Vocab: %10lld Words  %10lld AllWords\n",
               (long long)(clock.GetTime()), (long long)(vocabs->size()),
               all_words);
      }
    }
  }
  fr.Close();
  if (debug_mode)
    printf("Timer %6lldS Read Vocab From %s End\n",
           (long long)(clock.GetTime()), file_name.data());
  return vocabs->size();
}

void Word2Vec::AcceptSentences(vector<string> ss) {
  for (auto s = ss.begin(); s != ss.end(); s++) {
    auto it = vocabs->find(*s);
    if (it != vocabs->end()) {
      it->second->cnt++;
    } else {
      vocab* new_word = new vocab;
      new_word->cnt = 1;
      new_word->word = *s;
      vocabs->insert(make_pair(*s, new_word));
    }
  }
};

void Word2Vec::InitHsModel() {
  unsigned long long next_random = 1;
  for (int i = 0; i < vocab_size; i++) {
    (*para_hs)[i] = vector<double>(layer_size, 0);
    vector<double> v0(layer_size, 0);
    for (int j = 0; j < layer_size; j++) {
      next_random = next_random * (unsigned long long)25214903917 + 11;
      v0[j] = (((next_random & 0xFFFF) / 65536.0) - 0.5) / layer_size;
    }
    (*vocab_index)[i]->vec = v0;
  }
  InitHuffmanTree();
}

void Word2Vec::InitNeModel() {
  unsigned long long next_random = 1;
  for (int i = 0; i < vocab_size; i++) {
    (*para_ne)[i] = vector<double>(layer_size, 0);
    vector<double> v0(layer_size, 0);
    for (int j = 0; j < layer_size; j++) {
      next_random = next_random * (unsigned long long)25214903917 + 11;
      v0[j] = (((next_random & 0xFFFF) / 65536.0) - 0.5) / layer_size;
    }
    (*vocab_index)[i]->vec = v0;
  }
}

void Word2Vec::Init() {
  InitVocabIndex();
  InitNeTable();
  if (hs) {
    InitHsModel();
  }
  if (negative > 0) {
    InitNeModel();
  }
  InitSigmoidTable();
  if (debug_mode) {
    printf(
        "Timer %6lldS Model Init  Finished: NeTableSize %10lld   "
        "SigmoidTableSize %10lld\n",
        (long long)(clock.GetTime()), (long long)(ne_table->size()),
        (long long)(sig_table->size()));
  }
}

void Word2Vec::Train(string file_name) {
  basic::FileRead train_file(file_name, binary);
  basic::ThreadPool pool(n_jobs);
  for (int i = 0; i < n_jobs; i++) {
    function<void(basic::FileRead * fr)> f;
    f = bind(&Word2Vec::TrainThread, this, placeholders::_1);
    pool.AddJob(f, &train_file);
  }
}

void Word2Vec::TrainThread(basic::FileRead* fr) {
  string line;
  double neu1[layer_size], neu1e[layer_size];
  double start_alpha = alpha;
  bool read_result;
  vector<string> sentences;
  vector<long long> sentences_index;
  long long step = train_words / 10000;
  while (true) {
    while (true) {
      sentences.clear();
      sentences_index.clear();
      {
        lock_guard<mutex> u_lock(lock);
        read_result = fr->ReadLine(line);
        if (!read_result) break;
      }
      sentences = basic::SplitStr(line, " ");
      if (curr_train_words.load() - last_train_words.load() > step) {
        alpha = start_alpha * (1 - curr_train_words.load() /
                                       (double)(epoch * train_words + 100));
        if (alpha < start_alpha * 0.0005) alpha = start_alpha * 0.0005;
        if (debug_mode) {
          printf(
              "Timer %6lldS Train Vocab: Alpha %.7f  Epoch %d  Process "
              "%.2f%%\n",
              (long long)(clock.GetTime()), alpha, curr_epoch.load(),
              100 * (double)(curr_train_words.load()) /
                  (double)(epoch * train_words + 1));
        }
        last_train_words.store(curr_train_words.load());
      }
      for (auto it = sentences.begin(); it != sentences.end(); it++) {
        auto v = vocabs->find(*it);
        if (v != vocabs->end()) {
          sentences_index.push_back(v->second->index);
        }
        if (sentences_index.size() >= MAX_SENTENCE_LENGTH) {
          TrainSentences(sentences_index);
          sentences_index.clear();
        }
      }
      if (sentences_index.size() >= 2) TrainSentences(sentences_index);
    }
    if (curr_epoch.load() >= epoch - 1) break;
    {
      lock_guard<mutex> u_lock(lock);
      fr->JumpToSize(0);
      curr_epoch.fetch_add(1);
    }
  }
}

void Word2Vec::TrainSentences(vector<long long>& sentences) {
  curr_train_words.fetch_add(sentences.size());
  vector<double> neu1(layer_size, 0);
  vector<double> neu1e(layer_size, 0);
  int goal_position = 0, sentence_length = sentences.size(), target = 0;
  double f, g, l2, label;
  static long long next_random = 1;
  int cw = 0, a = 0, c = 0, d = 0, b = 0;
  while (true) {
    if (goal_position >= sentence_length) break;
    auto vocab_goal = vocab_index->at(sentences[goal_position]);
    if (sample > 0) {
      double ran = (sqrt(vocab_goal->cnt / (sample * train_words)) + 1) *
                   (sample * train_words) / vocab_goal->cnt;
      next_random = next_random * (unsigned long long)random_engine() + 11;
      if (ran < (next_random & 0xFFFF) / (double)65536) continue;
    }
    for (int i = 0; i < layer_size; i++) neu1[i] = 0;
    for (int i = 0; i < layer_size; i++) neu1e[i] = 0;
    b = random_engine() % window;
    if (cbow) {
      cw = 0;
      for (a = b; a < window * 2 + 1 - b; a++) {
        if (a != window) {
          c = goal_position - window + a;
          if (c < 0 || c >= sentence_length) continue;
          auto vec = vocab_index->at(sentences[c])->vec;
          for (c = 0; c < layer_size; c++) {
            neu1[c] += vec[c];
          }
          cw++;
        }
      }
      if (cw) {
        for (c = 0; c < layer_size; c++) neu1[c] /= cw;
        if (hs) {
          for (d = 0; d < vocab_goal->code.size(); d++) {
            l2 = vocab_goal->path.at(d);
            auto para_hs_goal = para_hs->find(l2);
            f = 0;
            // Propagate hidden -> output
            for (c = 0; c < layer_size; c++)
              f += neu1[c] * para_hs_goal->second[c];
            if (f <= -MAX_EXP || f >= MAX_EXP)
              continue;
            else
              f = sig_table->at((int)((f + MAX_EXP) *
                                      (EXP_SIGMOID_TABLE_SIZE / MAX_EXP / 2)));
            // 'g' is the gradient multiplied by the learning rate
            g = (1 - vocab_goal->code[d] - f) * alpha;
            // Propagate errors output -> hidden
            // Learn weights hidden -> output
            for (c = 0; c < layer_size; c++) {
              neu1e[c] += g * para_hs_goal->second[c];
              para_hs_goal->second[c] += g * neu1[c];
            }
          }
        }
        // NEGATIVE SAMPLING
        if (negative > 0) {
          for (d = 0; d < negative + 1; d++) {
            if (d == 0) {
              target = sentences[goal_position];
              label = 1;
            } else {
              target = ne_table->at(random_engine() %
                                    (long long)DEFAULT_NE_TABLE_SIZE);
              if (target == sentences[goal_position]) continue;
              label = 0;
            }
            auto para_ne_goal = para_ne->find(target);
            f = 0;
            for (c = 0; c < layer_size; c++)
              f += neu1[c] * para_ne_goal->second.at(c);
            if (f > MAX_EXP) {
              g = (label - 1) * alpha;
            } else if (f < -MAX_EXP) {
              g = (label - 0) * alpha;
            } else {
              g = (label -
                   sig_table->at((int)((f + MAX_EXP) * (EXP_SIGMOID_TABLE_SIZE /
                                                        MAX_EXP / 2)))) *
                  alpha;
            }
            for (c = 0; c < layer_size; c++) {
              neu1e[c] += g * para_ne_goal->second.at(c);
              para_ne_goal->second.at(c) += g * neu1[c];
            }
          }
        }
        // hidden -> in
        for (a = b; a < window * 2 + 1 - b; a++) {
          if (a != window) {
            c = goal_position - window + a;
            if (c < 0 || c >= sentence_length) continue;
            auto vec = vocab_index->at(sentences[c]);
            for (int c = 0; c < layer_size; c++) vec->vec[c] += neu1e[c];
          }
        }
      }
    } else {
      for (int a = b; a < window * 2 + 1 - b; a++) {
        if (a != window) {
          c = goal_position - window + a;
          if (c < 0 || c >= sentence_length) continue;
          auto vec = vocab_index->at(sentences[c]);
          for (c = 0; c < layer_size; c++) neu1e[c] = 0;
          if (hs) {
            for (d = 0; d < vocab_goal->code.size(); d++) {
              f = 0;
              l2 = vocab_goal->path.at(d);
              auto para_hs_goal = para_hs->find(l2);
              // Propagate hidden -> output
              for (c = 0; c < layer_size; c++)
                f += vec->vec[c] * para_hs_goal->second[c];
              if (f <= -MAX_EXP || f >= MAX_EXP)
                continue;
              else
                f = sig_table->at(
                    (int)((f + MAX_EXP) *
                          (EXP_SIGMOID_TABLE_SIZE / MAX_EXP / 2)));
              // 'g' is the gradient multiplied by the learning rate
              g = (1 - vec->code[d] - f) * alpha;
              // Propagate errors output -> hidden
              // Learn weights hidden -> output
              for (c = 0; c < layer_size; c++) {
                neu1e[c] += g * para_hs_goal->second[c];
                para_hs_goal->second[c] += g * vec->vec[c];
              }
            }
          }
          if (negative > 0) {
            for (d = 0; d < negative + 1; d++) {
              if (d == 0) {
                target = sentences[goal_position];
                label = 1;
              } else {
                target = ne_table->at(random_engine() %
                                      (long long)DEFAULT_NE_TABLE_SIZE);
                if (target == sentences[goal_position]) continue;
                label = 0;
              }
              auto para_ne_goal = para_ne->find(target);
              f = 0;
              for (c = 0; c < layer_size; c++)
                f += vec->vec[c] * para_ne_goal->second[c];
              if (f > MAX_EXP) {
                g = (label - 1) * alpha;
              } else if (f < -MAX_EXP) {
                g = (label - 0) * alpha;
              } else {
                g = (label - sig_table->at(
                                 (int)((f + MAX_EXP) * (EXP_SIGMOID_TABLE_SIZE /
                                                        MAX_EXP / 2)))) *
                    alpha;
              }
              for (c = 0; c < layer_size; c++) {
                neu1e[c] += g * para_ne_goal->second[c];
                para_ne_goal->second[c] += g * vec->vec[c];
              }
            }
          }
          for (c = 0; c < layer_size; c++) vec->vec[c] += neu1e[c];
        }
      }
    }
    goal_position++;
  }
}

void Word2Vec::InitVocabIndex() {
  vector<string> unvalid_words;
  for (auto it = vocabs->begin(); it != vocabs->end(); it++) {
    if (it->second->cnt < min_count) {
      unvalid_words.emplace_back(it->second->word);
    } else {
      vocab_index->emplace_back(it->second);
      train_words += it->second->cnt;
    }
  }
  for (auto it = unvalid_words.begin(); it != unvalid_words.end(); it++) {
    delete (*vocabs)[*it];
    vocabs->erase(*it);
  }
  vocab_size = vocab_index->size();
  sort(vocab_index->begin(), vocab_index->end(), compare_vocab);
  for (int i = 0; i < vocab_size; i++) {
    (*vocab_index)[i]->index = i;
  }
  if (debug_mode) {
    printf(
        "Timer %6lldS Learn Words Finished: VocabSize %10lld   AllWordSize "
        "%10lld\n",
        (long long)(clock.GetTime()), vocab_size, train_words);
  }
}

void Word2Vec::InitNeTable() {
  *ne_table = vector<long long>((long long)DEFAULT_NE_TABLE_SIZE, 0);
  double all_pow = 0, curr_pow = 0;
  double power = 0.75;
  long long curr_index = 0, index = 0;
  for (auto it = vocabs->begin(); it != vocabs->end(); it++)
    all_pow += pow(it->second->cnt, power);
  curr_pow += pow((*vocab_index)[0]->cnt, power);
  curr_index = DEFAULT_NE_TABLE_SIZE * curr_pow / all_pow;
  for (int i = 0; i < DEFAULT_NE_TABLE_SIZE; i++) {
    if (curr_index <= i && index < vocab_size - 1) {
      index++;
      curr_pow += pow((*vocab_index)[index]->cnt, power);
      curr_index = DEFAULT_NE_TABLE_SIZE * curr_pow / all_pow;
      (*ne_table)[i] = index;
    }
    (*ne_table)[i] = index;
  }
}

void Word2Vec::InitSigmoidTable() {
  *sig_table = vector<double>(EXP_SIGMOID_TABLE_SIZE, 0);
  for (int i = 0; i < EXP_SIGMOID_TABLE_SIZE; i++) {
    double x = exp((i / (double)EXP_SIGMOID_TABLE_SIZE * 2 - 1) * MAX_EXP);
    x = x / (x + 1);
    (*sig_table)[i] = x;
  }
}

void Word2Vec::InitHuffmanTree() {
  long long b, i, min1i, min2i, pos1, pos2;
  vector<long long> point(MAX_CODE_LENGTH, 0), code(MAX_CODE_LENGTH, 0);
  vector<long long> count(vocab_size * 2 + 1, 0), binary(vocab_size * 2 + 1, 0),
      parent_node(vocab_size * 2 + 1, 0);
  for (int i = 0; i < vocab_size; i++) count[i] = (*vocab_index)[i]->cnt;
  for (int i = vocab_size; i < vocab_size * 2; i++) count[i] = 1e15;
  pos1 = vocab_size - 1;
  pos2 = vocab_size;
  for (int i = 0; i < vocab_size - 1; i++) {
    if (pos1 >= 0) {
      if (count[pos1] < count[pos2]) {
        min1i = pos1;
        pos1--;
      } else {
        min1i = pos2;
        pos2++;
      }
    } else {
      min1i = pos2;
      pos2++;
    }
    if (pos1 >= 0) {
      if (count[pos1] < count[pos2]) {
        min2i = pos1;
        pos1--;
      } else {
        min2i = pos2;
        pos2++;
      }
    } else {
      min2i = pos2;
      pos2++;
    }
    count[vocab_size + i] = count[min1i] + count[min2i];
    parent_node[min1i] = vocab_size + i;
    parent_node[min2i] = vocab_size + i;
    binary[min2i] = 1;
  }
  for (long long a = 0; a < vocab_size; a++) {
    b = a;
    i = 0;
    while (true) {
      code[i] = binary[b];
      point[i++] = b;
      b = parent_node[b];
      if (b == vocab_size * 2 - 2) break;
    }
    vector<long long> hs_path(i + 1, 0);
    vector<int> hs_code(i, 0);
    hs_path[0] = vocab_size - 2;
    for (b = 0; b < i; b++) {
      hs_code[i - b - 1] = code[b];
      hs_path[i - b] = point[b] - vocab_size;
    }
    (*vocab_index)[a]->path = hs_path;
    (*vocab_index)[a]->code = hs_code;
  }
}

vector<double> Word2Vec::GetVec(string word) {
  vector<double> v;
  auto kv = vocabs->find(word);
  if (kv != vocabs->end()) {
    return kv->second->vec;
  }
  return v;
}

vector<pair<string, double>> Word2Vec::Similiar(string word, int n,
                                                bool cosine) {
  vector<pair<string, double>> v;
  vector<double> vec;
  double sim = 0;
  auto kv = vocabs->find(word);
  if (kv != vocabs->end()) {
    vec = kv->second->vec;
  } else
    return v;
  for (auto jt = vocabs->begin(); jt != vocabs->end(); jt++) {
    auto it = jt->second;
    if (it->word != word) {
      if (cosine)
        sim = Cosine(vec, it->vec);
      else
        sim = Euclidean(vec, it->vec);
      if (v.size() == 0) {
        v.insert(v.begin(), make_pair(it->word, sim));
      } else if (sim >= v[0].second) {
        v.insert(v.begin(), make_pair(it->word, sim));
      } else {
        for (int i = v.size() - 1; i >= 0; i--) {
          if (sim <= v[i].second) {
            v.insert(v.begin() + i + 1, make_pair(it->word, sim));
            break;
          }
        }
      }
      if (v.size() > n) v.pop_back();
    }
  }
  return v;
}

}  // namespace see