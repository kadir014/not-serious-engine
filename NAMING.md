# Naming & Style Conventions
This document explains the naming and styling conventions used in the project.

It's mainly for consistency - not strict enforcement - so feel free to be pragmatic as long as things stay readable and match the general vibe.

- **File names**: `snake_case.c`, `snake_case.h` (usually matching main type or feature)
- **Engine namespace**: All engine-wide symbols use the `ns` prefix to avoid collisions.
- **Trivial variables**: `i` for index, `n` for amount of data, etc is acceptable as long as it's clear enough and the scope is small.
- **Indentation**: 4 spaces, no tabs! 
- **Pointers**: Must be right-aligned, it is the only way.
- **Local variables**: `snake_case`
- **Public Functions**: `ns_snake_case`
- **Private functions**: `_ns_snake_case`
- **Const variable**: `SCREAMING_SNAKE_CASE`
- **Structs**: `nsPascalCase`
  - **Members**: `snake_case`
  - **Methods**: `nsPascalCase_snake_case`
  - **Example**:
  ```c
  typedef struct {
      int member_of_struct;
  } nsSomeStruct;

  void *nsSomeStruct_some_method(void);
  ```
- **Enums**: `nsPascalCase`
  - **Fields**: `nsPascalCase_SCREAMING_SNAKE_CASE`
  - **Example**:
  ```c
  typedef enum {
      nsSomeEnum_FIRST,
      nsSomeEnum_SECOND_FIELD,
      nsSomeEnum_LAST
  } nsSomeEnum;
  ```
- **Macros**: `NS_SCREAMING_SNAKE_CASE`
- **Comments**: Use `//` for short notes, `/* ... */` for longer explanations.
- **Documentation**: Use doxygen-style comment blocks in headers.
  - **Example**:
  ```c
  /**
   * @brief This function does something.
   *
   * Longer description on what it exactly does.
   *
   * Returns non-zero on error, use @ref ns_get_error for more information.
   *
   * @param arg1 First argument
   * @param arg2 Second argument
   * ...
   * @return int 
   */
  int ns_some_function(...);
  ```