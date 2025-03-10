#pragma once

#define MAX_STACK_SIZE 100

#define scope                                                                  \
  void *destructors[MAX_STACK_SIZE];                                           \
  unsigned int destructor_index = 0;                                           \
  unsigned int defer_error_no = 0;                                             \
  unsigned int defer_runtime = 0;

#define defer(name, value, destructor)                                         \
  name = value;                                                                \
  if (defer_runtime) {                                                         \
    destructor_##name : destructor;                                            \
    goto defer_entry_freed;                                                    \
  }                                                                            \
  destructors[destructor_index] = &&destructor_##name;                         \
  destructor_index += 1

#define defer_exit                                                             \
  defer_runtime = 1;                                                           \
  while (destructor_index > 0) {                                               \
    goto *destructors[destructor_index - 1];                                   \
  defer_entry_freed:                                                           \
    destructor_index -= 1;                                                     \
  }                                                                            \
  if (defer_error_no)                                                          \
    goto defer_error_code;

#define defer_error                                                            \
  defer_error_code:                                                            \
  if (defer_error_no)

#define _(...) __VA_ARGS__
