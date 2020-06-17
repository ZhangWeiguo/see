#include "word2vec.h"

#include <stdio.h>

#include <iostream>
using namespace std;

int main() {
  string file_name = "/mnt/d/Code/C/dat/wiki_small";
  bool b_cbow = true;
  int i_negative = 0;
  int i_window = 5;
  int i_min_count = 5;
  int i_layer_size = 128;
  int i_epoch = 2;
  double d_alpha = 0.05;
  double d_sample = 0.001;
  int i_n_jobs = 8;
  bool b_debug_mode = true;
  bool b_binary = true;
  see::Word2Vec WV(b_cbow, i_negative, i_window, i_min_count, i_layer_size,
                   i_epoch, d_alpha, d_sample, i_n_jobs, b_debug_mode,
                   b_binary);
  WV.ReadVocabFromTrain(file_name + ".txt");
  WV.Init();
  WV.Train(file_name + ".txt");
  WV.SaveVocab(file_name + ".vocab");
  WV.SaveVec(file_name + ".vec");
  WV.SaveModel(file_name + ".model");

  // WV.ReadVocab(file_name + ".vocab");
  // WV.ReadVec(file_name + ".vec");
  // WV.Init();

  // WV.ReadModel(file_name + ".model");

  // WV.ReadVocab(file_name + ".vocab");
  // WV.ReadVec(file_name + ".vec");
  // auto result = WV.Similiar("football", 100, true);
  // for (auto it = result.begin(); it != result.end(); it ++)
  // {
  //     printf("%20s %f\n", it->first.data(), it->second);
  // }
  return 0;
}