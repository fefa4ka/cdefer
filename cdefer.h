#pragma once

#define MAX_STACK_SIZE 100
#define MAX_SCOPE_DEPTH 10

#define scope                                                                  \
  static void *_destructors[MAX_STACK_SIZE];                                   \
  static unsigned int _destructor_index = 0;                                   \
  static unsigned int _defer_error_no = 0;                                     \
  static unsigned int _defer_runtime = 0;                                      \
  static unsigned int _scope_stack[MAX_SCOPE_DEPTH] = {0};                     \
  static unsigned int _scope_depth = 0;                                        \
  _scope_stack[_scope_depth++] = _destructor_index;                            \
  for(int _scope_once = 1; _scope_once; _scope_once = 0)

#define scope_end                                                              \
  _defer_runtime = 1;                                                          \
  while (_destructor_index > _scope_stack[_scope_depth-1]) {                   \
    goto *_destructors[_destructor_index - 1];                                 \
  }                                                                            \
  _scope_depth--;

#define defer(name, value, destructor)                                         \
  name = value;                                                                \
  if (_defer_runtime) {                                                        \
    destructor_##name : destructor;                                            \
    _destructor_index -= 1;                                                    \
    continue;                                                                  \
  }                                                                            \
  _destructors[_destructor_index] = &&destructor_##name;                       \
  _destructor_index += 1

#define defer_exit                                                             \
  _defer_runtime = 1;                                                          \
  while (_destructor_index > 0) {                                              \
    goto *_destructors[_destructor_index - 1];                                 \
  defer_entry_freed:                                                           \
    _destructor_index -= 1;                                                    \
  }                                                                            \
  if (_defer_error_no)                                                         \
    goto defer_error_code;

#define defer_error                                                            \
  defer_error_code:                                                            \
  if (_defer_error_no)

#define _(...) __VA_ARGS__
