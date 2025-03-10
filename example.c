/**
 * C Defer Example
 *
 * This example demonstrates how to use the cdefer.h library to manage resources
 * with automatic cleanup, similar to Go's defer statement.
 */

#include <cdefer.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * A structure representing a large number with dynamically allocated memory
 */
struct bignum {
    int   *data;
    size_t size;
    char  *name;
};

/**
 * A structure representing a file resource
 */
struct file_resource {
    FILE *handle;
    char *filename;
};

/**
 * Initialize a bignum with a specific value
 */
void init_bignum(struct bignum *num, int value)
{
    if (num && num->data) {
        num->data[0] = value;
        printf("Initialized %s with value %d\n", num->name, value);
    }
}

int main(int argc, char *argv[])
{
    printf("=== C Defer Example ===\n\n");

    // Create a scope where deferred operations will be registered
    scope {
        printf("Entering outer scope and allocating resources...\n");

        // Example 1: Allocate and automatically free memory for a bignum
        struct bignum defer(googol,
                            _({.data = malloc(sizeof(int) * 10),
                               .size = 10,
                               .name = strdup("googol")}),
                            {
                                printf("Cleaning up %s with value %d\n",
                                       googol.name, *googol.data);
                                free(googol.data);
                                free(googol.name);
                            });

        // Nested scope example
        scope {
            printf("\nEntering nested scope...\n");
            
            // Example 2: Another bignum with different values in nested scope
            struct bignum defer(pi,
                                _({.data = malloc(sizeof(int) * 5),
                                   .size = 5,
                                   .name = strdup("pi")}),
                                {
                                    printf("Cleaning up %s with value %d\n",
                                           pi.name, *pi.data);
                                    free(pi.data);
                                    free(pi.name);
                                });
                                
            // Use the nested scope resource
            init_bignum(&pi, 314);
            
            // We can access outer scope resources from inner scope
            init_bignum(&googol, 10000000);
            
            printf("Leaving nested scope - nested resources will be cleaned up first\n");
            scope_end;
        }

        // Example 3: File handling with automatic closing
        struct file_resource defer(
            log_file,
            _({.handle   = fopen("/tmp/defer_example.log", "w"),
               .filename = strdup("/tmp/defer_example.log")}),
            {
                printf("Closing file: %s\n", log_file.filename);
                if (log_file.handle)
                    fclose(log_file.handle);
                free(log_file.filename);
            });

        if (log_file.handle) {
            fprintf(log_file.handle, "Logged values: googol=%d\n",
                    *googol.data);
            printf("Wrote to log file: %s\n", log_file.filename);
        }

        printf("\nLeaving outer scope - remaining resources will be cleaned up\n");
        scope_end;
    }

    // All destructors are run when exiting the scope
    printf("\nAll resources have been cleaned up\n");

    defer_exit
    {
        printf("Program completed successfully\n");
        return 0;
    }

    // This code runs if an error occurred and was flagged
    defer_error
    {
        printf("An error occurred during execution\n");
        return 1;
    }
}
