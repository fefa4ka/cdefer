/**
 * C Defer Example
 * 
 * This example demonstrates how to use the cdefer.h library to manage resources
 * with automatic cleanup, similar to Go's defer statement.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cdefer.h>

/**
 * A structure representing a large number with dynamically allocated memory
 */
struct bignum {
  int* data;
  size_t size;
  char* name;
};

/**
 * A structure representing a file resource
 */
struct file_resource {
  FILE* handle;
  char* filename;
};

/**
 * Initialize a bignum with a specific value
 */
void init_bignum(struct bignum* num, int value) {
  if (num && num->data) {
    num->data[0] = value;
    printf("Initialized %s with value %d\n", num->name, value);
  }
}

int main(int argc, char *argv[]) {
  printf("=== C Defer Example ===\n\n");
  
  // Create a scope where deferred operations will be registered
  scope {
    printf("Entering scope and allocating resources...\n");
    
    // Example 1: Allocate and automatically free memory for a bignum
    struct bignum defer(googol, 
                       {.data = malloc(sizeof(int) * 10), 
                        .size = 10, 
                        .name = strdup("googol")},
                       {
                         printf("Cleaning up %s with value %d\n", googol.name, *googol.data); 
                         free(googol.data);
                         free(googol.name);
                       });
    
    // Example 2: Another bignum with different values
    struct bignum defer(pi, 
                       {.data = malloc(sizeof(int) * 5), 
                        .size = 5, 
                        .name = strdup("pi")},
                       {
                         printf("Cleaning up %s with value %d\n", pi.name, *pi.data); 
                         free(pi.data);
                         free(pi.name);
                       });
    
    // Example 3: File handling with automatic closing
    struct file_resource defer(log_file,
                              {.handle = fopen("/tmp/defer_example.log", "w"),
                               .filename = strdup("/tmp/defer_example.log")},
                              {
                                printf("Closing file: %s\n", log_file.filename);
                                if (log_file.handle) fclose(log_file.handle);
                                free(log_file.filename);
                              });
    
    // Use the resources
    init_bignum(&googol, 10000000);
    init_bignum(&pi, 314);
    
    if (log_file.handle) {
      fprintf(log_file.handle, "Logged values: googol=%d, pi=%d\n", 
              *googol.data, *pi.data);
      printf("Wrote to log file: %s\n", log_file.filename);
    }
    
    printf("\nLeaving scope - resources will be automatically cleaned up\n");
  }
  
  // All destructors are run when exiting the scope
  printf("\nAll resources have been cleaned up\n");
  
  defer_exit { 
    printf("Program completed successfully\n");
    return 0; 
  }
  
  // This code runs if an error occurred and was flagged
  defer_error { 
    printf("An error occurred during execution\n");
    return 1; 
  }
}
