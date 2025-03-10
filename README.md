# C Defer

A C header library that provides resource management with defer semantics, similar to Go's defer statement.

## Overview

This library provides macros that help manage resources by automatically running cleanup code when a scope is exited.

## Macros

- `scope { ... }`: Defines a scope where deferred operations can be registered
- `defer(name, value, destructor)`: Declares a variable with cleanup code that runs when the scope exits
- `defer_exit { ... }`: Runs all registered destructors and executes the provided code
- `defer_error { ... }`: Handles error cases after destructors have run

## Example

```c
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
                        printf("free pi number %d\n", *pi.data); free(pi.data););
    googol.data[0] = 10;
    pi.data[0] = 3;
  }
  defer_exit { return 0; }
  defer_error { return 1; }
}
```

This example demonstrates automatic cleanup of allocated memory when the scope exits.
