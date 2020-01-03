#include <stdio.h>
//ASAN_OPTIONS=check_initialization_order=true ./a.out
//https://isocpp.org/wiki/faq/ctors#static-init-order
extern int extern_global;
int __attribute__((noinline)) read_extern_global() {
  return extern_global;
}
int x = read_extern_global() + 1;
int main() {
  printf("%d\n", x);
  return 0;
}