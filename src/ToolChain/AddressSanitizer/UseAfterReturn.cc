// By default, AddressSanitizer does not try to detect
// stack-use-after-return bugs.
// It may still find such bugs occasionally
// and report them as a hard-to-explain stack-buffer-overflow.

// You need to run the test with ASAN_OPTIONS=detect_stack_use_after_return=1
#include <stdlib.h>
#include <iostream>
#include <memory>
using namespace std;

int *ptr_raw;
int *ptr_unique;
int *ptr_shared;

__attribute__((noinline))


void FunctionThatEscapesLocalObject_justfortest() {
  int local[100];
  local[0] = 150;
}

void FunctionThatEscapesLocalObject_raw() {
  int local[100];
  local[0] = 150;
  ptr_raw = &local[0];
}

void FunctionThatEscapesLocalObject_unique() {
std::unique_ptr<int> local = std::make_unique<int>(100);
*(local.get()) = 150;
ptr_unique = local.get();
}

void FunctionThatEscapesLocalObject_shared() {
std::shared_ptr<int> local = std::make_shared<int>(100);
*(local.get()) = 150;
ptr_shared = local.get();
}

int main(int argc, char **argv) {
  FunctionThatEscapesLocalObject_raw();
  cout << *(ptr_raw) << endl;
  FunctionThatEscapesLocalObject_unique();
  cout << *(ptr_unique) << endl;
  FunctionThatEscapesLocalObject_shared();
  cout << *(ptr_shared) << endl;
  FunctionThatEscapesLocalObject_justfortest();
  return 1;
}