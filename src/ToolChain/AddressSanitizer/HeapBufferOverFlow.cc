#include <stdlib.h>
#include <iostream>
using namespace std;
int main(int argc, char **argv) {
  int *array = new int[1];
  array[0] = 0;
  int res = array[argc + 100];  // BOOM
  delete [] array;
  return res;
}