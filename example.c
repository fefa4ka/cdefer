#include <stdio.h>
#include <stdlib.h>
#include <cdefer.h>

struct bignum {
  int *data;
};

int main(int argc, char *argv[]) {
  scope {
    struct bignum defer(googol, {.data = malloc(sizeof(int) * 10)},
                        printf("free googol number %d\n", *googol.data); free(googol.data));
    struct bignum defer(pi, {.data = malloc(sizeof(int) * 10)},
                        printf("free pi number %d\n", *pi.data); free(pi.data));
    googol.data[0] = 10;
    pi.data[0] = 3;
  }
  defer_exit { return 0; }
  defer_error { return 1; }
}
