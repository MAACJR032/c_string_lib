# c_string_lib: A Custom String Library in C

English | [Português](README-PT.md)

c_string_lib is a custom string manipulation library written in C that provides a dynamic and efficient way to handle strings. It abstracts away the complexity of memory allocation and null-termination (`'\0`), allowing users to focus solely on string operations. 

This library includes a wide range of functions for creating, modifying, comparing, and manipulating strings. Functions that typically accept `const char*` also have versions that work seamlessly with the `string` struct.

## Features

- Automatic memory management for string growth and resizing.
- Null-termination is always handled internally.
- Support for a wide range of operations:
  - **Append**, **Insert**, **Resize**, **Clear**, **Compare**, **Reverse**, **Convert to Upper/Lowercase**.
  - **Substring extraction**, **Splitting** strings, and **Joining** strings with delimiters.
- Error codes to handle edge cases effectively (e.g., null arguments, allocation failures).
- Consistent API for both `const char*` and `string` struct inputs.

## Clone This Repository

```
git clone https://github.com/MAACJR032/c_string_lib.git
```
