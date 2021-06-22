# ext

The C++ standard library extensions

## Index

- Common
    - [Holder<T>](include/ext/holder.hpp)
- Error handling
    - [Result<T, E>](include/ext/result.hpp)
    - [Maybe\<T>](include/ext/maybe.hpp)

## Getting started

### Prerequisites

- C++ compiler supported C++20
- Conan (optional)

### Build

It is a header-only library.

### Install (using Conan)

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
ext/0.1.3
```

Using CMake wrapper for the Conan:

```cmake
conan_cmake_configure(REQUIRES # other libraries...
                               ext/0.1.3
    GENERATORS cmake)
```

## License

Distributed under the MIT License. See [LICENSE](LICENSE) for more information.

## Author

Oleg E. Vorobiov <isnullxbh@gmail.com>
