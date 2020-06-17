#include "dataset.cc"

#include "dataset.h"
#include "stdio.h"
using namespace std;

int main() {
  see::DoubleMat feature, target;
  see::DataSet::GetIris(&feature, &target);
  printf("Iris Feature:\n");
  see::MatPrint(feature);
  printf("Iris Target:\n");
  see::MatPrint(target);

  see::DataSet::MakeRandomRegression(&feature, &target, 4, 2, 20, 0.1);
  printf("Random Regression Feature:\n");
  see::MatPrint(feature);
  printf("Random Regression Target:\n");
  see::MatPrint(target);

  see::DataSet::MakeRandomClassification(&feature, &target, 4, 2, 20, true,
                                         0.2);
  printf("Random Classification Feature:\n");
  see::MatPrint(feature);
  printf("Random Classification Target:\n");
  see::MatPrint(target);
  return 0;
}
