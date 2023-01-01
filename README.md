# C Deffered Promise

This C header library provides a set of macros that can be used to manage resources and handle errors in C code.

## Getting Started

To use this library, you will need to include the header file in your C code. The following macros are available:

-   `scope { ... }`: This macro creates four variables that are used to store information about destructors and error handling. It should be used to define the beginning of a scope in which resources can be managed.
-   `defer(variable, { value }, destructor)`: This macro allows you to specify a code block to be run as a destructor when the scope is exited. It takes three arguments: "name", "value", and "destructor". The "name" argument is the name of the variable that will be assigned the "value" argument. The "destructor" argument is a code block that will be run as a destructor when the scope is exited.
-   `defer_exit { ... }`: This macro is used to run all of the destructors that were specified using the "defer" macro within the current scope. It should be used at the end of a scope to ensure that all resources are properly cleaned up.
-   `defer_error { ... }`: This macro defines a label that can be jumped to if an error occurs within the current scope.
    To use these macros in your code, you can follow the example below:

```c

struct bignum {
  int *data;
};

int main(int argc, char *argv[]) {
  scope {
    struct bignum defer(googol, {.data = malloc(sizeof(int) * 10)},
                        printf("free googol number %d\n", *number.data); free(googol.data));
    struct bignum defer(pi, {.data = malloc(sizeof(int) * 10)},
                        free(pi.data););
    number.data[0] = 10;
  }
  defer_exit { return 0; }
  defer_error { return 1; }
}
```
