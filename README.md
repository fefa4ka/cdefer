# C Defer

A C header library that provides resource management with defer semantics, similar to Go's defer statement.

## Overview

This library provides macros that help manage resources by automatically running cleanup code when a scope is exited. This approach helps prevent resource leaks and simplifies error handling by ensuring that cleanup code is always executed.

## Features

- Automatic resource cleanup when a scope is exited
- Simple syntax for registering cleanup code
- Stack-based execution of cleanup code (LIFO order)
- Error handling support

## Macros

| Macro | Description |
|-------|-------------|
| `scope { ... }` | Defines a scope where deferred operations can be registered |
| `scope_end;` | Ends a scope and runs all destructors registered in that scope |
| `defer(name, value, destructor)` | Declares a variable with cleanup code that runs when the scope exits |
| `defer_exit { ... }` | Runs all registered destructors and executes the provided code |
| `defer_error { ... }` | Handles error cases after destructors have run |

## Example

```c
#include <stdio.h>
#include <stdlib.h>
#include <cdefer.h>

struct bignum {
  int *data;
  size_t size;
  char *name;
};

int main() {
  scope {
    // Allocate memory and register cleanup code
    struct bignum defer(googol, 
                       {.data = malloc(sizeof(int) * 10), 
                        .size = 10, 
                        .name = "googol"},
                       {
                         printf("Cleaning up %s\n", googol.name);
                         free(googol.data);
                       });
    
    // Nested scope example
    scope {
      // Resources in nested scopes
      struct bignum defer(pi, 
                         {.data = malloc(sizeof(int) * 5), 
                          .size = 5, 
                          .name = "pi"},
                         {
                           printf("Cleaning up %s\n", pi.name);
                           free(pi.data);
                         });
      
      // Use the allocated memory
      pi.data[0] = 3;
      
      // When nested scope exits, its cleanup code runs automatically
      scope_end;
    }
    
    // Use the allocated memory
    googol.data[0] = 10;
    
    // When outer scope exits, its cleanup code runs automatically
    scope_end;
  }
  
  defer_exit { return 0; }
  defer_error { return 1; }
}
```

This example demonstrates automatic cleanup of allocated memory when the scope exits, preventing memory leaks even if errors occur.

## Building and Running

```bash
mkdir -p build && cd build
cmake ..
make
./example
```

## Use Cases

- File handling (ensuring files are closed)
- Memory management (preventing memory leaks)
- Network connections (ensuring sockets are closed)
- Database transactions (ensuring proper commit/rollback)
- Any resource that requires cleanup
