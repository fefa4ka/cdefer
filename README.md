# C Deffered Promise

This C header library provides a set of macros that can be used to manage resources and handle errors in C code.

It could be really helpful for avoiding the pain of forgetting to free resources.

## Documentation

To use this library, you will need to include the header file in your C code. The following macros are available:

-   `scope { ... }`: This macro creates four variables that are used to store information about destructors and error handling. It should be used to define the beginning of a scope in which resources can be managed.
-   `defer(variable, { value }, destructor)`: This macro allows you to specify a code block to be run as a destructor when the scope is exited. It takes three arguments: `name`, `value`, and `destructor`. The `name` argument is the name of the variable that will be assigned the "value" argument. The `destructor` argument is a code block that will be run as a destructor when the scope is exited.
-   `defer_exit { ... }`: This macro is used to run all of the destructors that were specified using the "defer" macro within the current scope. It should be used at the end of a scope to ensure that all resources are properly cleaned up.
-   `defer_error { ... }`: This macro defines a label that can be jumped to if an error occurs within the current scope.
    To use these macros in your code, you can follow the example below:

## Usage Example

```c
struct bignum {
  int *data;
};

int main(int argc, char *argv[]) {
  scope {
    /* Allocate resource and defer for freeing */
    struct bignum defer(googol, {.data = malloc(sizeof(int) * 10)},
                        printf("free googol number %d\n", *googol.data); free(googol.data));
    struct bignum defer(pi, {.data = malloc(sizeof(int) * 10)},
                        printf("free pi number %d\n", *pi.data); free(pi.data););
                        
    /* A lot of code */

    googol.data[0] = 10;
    pi.data[0] = 3;
    
    /* A lot of code */

  }
  defer_exit { return 0; }
  defer_error { return 1; }
}
```

The `defer` macro is used to declare and assign two variables called `googol` and `pi` of type `bignum`. Variables are assigned values with a dynamically allocated array of integers and the `destructor` for this variable prints out the value of the first element in the array and frees the memory.

At the end of the main function, the `defer_exit` macro is used to run the destructors for the `googol` and `pi` variables and then `return 0`. If `defer_error_no` is set, it will jump to the `defer_error_code` label and `return 1`.

Overall, this code uses the `scope` and `defer` macros to manage resources and handle errors within the main function. The `googol` and `pi` variables are dynamically allocated and their memory is properly freed using the destructors specified in the `defer` macros. If an error occurs within the scope of the main function, the program will `return 1`.

## Contributions

We welcome any contributions to the "C Defer" library! If you have ideas for new features or improvements, please don't hesitate to open an issue or pull request.

Here are a few ideas for ways to enrich the library:

-   [Nested scopes](https://github.com/fefa4ka/cdefer/issues/1): It would be really useful to add support for nested scopes in the "C Defer" library. This would allow for more complex resource management and error handling in C code.
-   Improved error handling: Currently, the "C Defer" library provides a basic error handling mechanism through the `defer_error` and `defer_error_no` variables. Adding additional features, such as the ability to throw and catch specific error codes or clear error flags, would enhance the error handling capabilities of the library.
-   More powerful destructors: It might be helpful to allow destructors to take arguments or to have more control over the execution of destructors, such as the ability to specify the order in which they are run.
-   Portability: Making the "C Defer" library more portable across different C compilers could make it more widely used and useful to more developers.

Overall, we are open to any ideas for improving the "C Defer" library and making it a more powerful and useful tool for C programmers. If you have any ideas or suggestions, please don't hesitate to contribute!
