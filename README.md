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

|       Name        |               Description                | Since | Default |
|:-----------------:|:----------------------------------------:|:-----:|:-------:|
| `EXT_BUILD_TESTS` | Create a build target for the unit tests | 0.1.0 |   OFF   |

## Installation

To install the library, build it and then go to the project root directory and execute
the following command:

```shell
cmake --install build/debug
```

## Index

- Extensions
  - Metaprogramming
    - [Type list](include/ext/type_list.hpp)
    - [Type holder](include/ext/type_holder.hpp)
    - [Indexed type holder](include/ext/type_holder.hpp)

## Author

Oleg E. Vorobiov <isnullxbh@gmail.com>

## License

This project is distributed under the MIT License. See [LICENSE](LICENSE) for more information.
