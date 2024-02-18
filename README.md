# ext

The C++ standard library extensions

## Prerequisites

- C++ compiler with `C++20` support
- CMake
- GTest (for unit tests only)

## Build

To build the library, go to the project root directory and execute the following commands:

```shell
# Prepare a buildsystem
cmake -DCMAKE_BUILD_TYPE=Debug \
      -DCMAKE_CXX_FLAGS="-std=c++20" \
      -S . -B build/debug

# Build
cmake --build build/debug --target ext
```

### Options

| Name                 |               Description                | Since | Default |
|:---------------------|:----------------------------------------:|:-----:|:-------:|
| `EXT_BUILD_TESTS`    | Create a build target for the unit tests | 0.1.0 |   OFF   |
| `EXT_BUILD_EXAMPLES` |    Create build targets for examples     | 0.1.0 |   OFF   |

## Installation

To install the library, build it and then go to the project root directory and execute
the following command:

```shell
cmake --install build/debug
```

## Index

- Extensions
  - Metaprogramming
    - [ext::type_list](include/ext/type_list.hpp)
    - [ext::type_holder](include/ext/type_holder.hpp)
    - [ext::indexed_type_holder](include/ext/type_holder.hpp)
    - [ext::ignore](include/ext/ignore.hpp)
  - Strings
    - [ext::fixed_string](include/ext/fixed_string.hpp)
  - Utility
    - [ext::value_wrapper](include/ext/value_wrapper.hpp)
      - Extensions:
        - [ext::value_wrapper_extensions::addition](include/ext/value_wrapper.hpp)
        - [ext::value_wrapper_extensions::subtraction](include/ext/value_wrapper.hpp)
        - [ext::value_wrapper_extensions::multiplication](include/ext/value_wrapper.hpp)
        - [ext::value_wrapper_extensions::division](include/ext/value_wrapper.hpp)
        - [ext::value_wrapper_extensions::remainder](include/ext/value_wrapper.hpp)
        - [ext::value_wrapper_extensions::bitwise](include/ext/value_wrapper.hpp)
        - [ext::value_wrapper_extensions::increment](include/ext/value_wrapper.hpp)
        - [ext::value_wrapper_extensions::decrement](include/ext/value_wrapper.hpp)
        - [ext::value_wrapper_extensions::arithmetic](include/ext/value_wrapper.hpp)
        - [ext::value_wrapper_extensions::logical](include/ext/value_wrapper.hpp)

## Examples

All examples are in [this](./examples) directory. It contains a [file](examples/README.md) with a brief description of 
each of them.

## Author

Oleg E. Vorobiov <isnullxbh@gmail.com>

## License

This project is distributed under the MIT License. See [LICENSE](LICENSE) for more information.
