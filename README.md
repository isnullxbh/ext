# ext

The C++ standard library extensions

## Index

- Algorithms
    - [On tuples](include/ext/tuple.hpp)
- Common
    - [Holder\<T>](include/ext/holder.hpp)
    - [Exception](include/ext/exceptions.hpp)
    - [Reference\<T>](include/ext/reference.hpp)
- Error handling
    - [Result<T, E>](include/ext/result.hpp)
    - [Maybe\<T>](include/ext/maybe.hpp)

## Getting started

### Prerequisites

- C++ compiler supported C++20
- Conan (optional)

### Dependencies

| Target | Dependencies |
| :----- | ------------ |
| ext | None |
| ext.tests | [Google Test](https://conan.io/center/gtest)

### Build options

| Option | Description | Default |
| :-----: | ----------- | ------- |
| **WITH_TESTS** | Compile library with unit tests | OFF (disabled) |
| **HEADER_ONLY** | Use as a header-only library | OFF (enabled) |

### Build

Example:

```shell
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Debug \
      -DCMAKE_CXX_FLAGS="-std=c++2a -Wall -pedantic -Wextra -Weffc++ -Wconversion" \
      -DHEADER_ONLY=OFF \
      ..
make -j $(nproc)
```

### Installation

Add repository:

```shell
conan remote add isnullxbh/testing https://artifactory.isnullxbh.me/artifactory/api/conan/public-conan
#                ^
#                You can use any other name here.
```

Add ext to the _requires_ section of the conanfile.txt:

```text
[requires]
...
ext/0.1.4
```

Using CMake wrapper for the Conan:

```cmake
conan_cmake_configure(REQUIRES # other libraries...
                               ext/0.1.4
    GENERATORS cmake)
```

## License

Distributed under the MIT License. See [LICENSE](LICENSE) for more information.

## Author

Oleg E. Vorobiov <isnullxbh@gmail.com>
