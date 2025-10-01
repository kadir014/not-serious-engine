# Naming & Style Conventions

- **Trivial variables**: `i` for index, `n` for amount of data, etc is acceptable as long as it's clear enough and the scope is small.
- **Local variables**: `snake_case`
- **Engine namespace**: `ns`
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