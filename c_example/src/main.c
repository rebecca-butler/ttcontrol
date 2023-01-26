#include <stdio.h>

#include "foo.h"

void foo() {
  int i = 1;
  printf("i = %i", i);
}

int main() {
  foo();
  return 0;
}