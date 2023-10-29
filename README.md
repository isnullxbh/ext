# ext

The C++ standard library extensions

## Build

To build the library, go to the project's root directory and execute the following commands:

```shell
# Prepare a buildsystem
cmake -DCMAKE_BUILD_TYPE=Debug \
      -DCMAKE_CXX_FLAGS="-std=c++20" \
      -S . -B build/debug

# Build
cmake --build build/debug --target ext
```

## Build options

|       Name        |               Description                | Since | Default |
|:-----------------:|:----------------------------------------:|:-----:|:-------:|
| `EXT_BUILD_TESTS` | Create a build target for the unit tests | 0.0.0 |   OFF   |

## Index

- Extensions
  - Metaprogramming
    - [Type list](include/ext/type_list.hpp)

## Author

Oleg E. Vorobiov <isnullxbh@gmail.com>

## License

This project is distributed under the MIT License. See [LICENSE](LICENSE) for more information.
