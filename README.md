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
conan remote add isnullxbh/testing https://109.195.38.72:8081/artifactory/api/conan/public-conan
```

Add ext to the _requires_ section of the conanfile.txt:

```text
[requires]
...
ext/0.1.2
```

Using CMake wrapper for the Conan:

```cmake
conan_cmake_configure(REQUIRES # other libraries...
                               ext/0.1.2
    GENERATORS cmake)
```

## License

Distributed under the MIT License. See [LICENSE](LICENSE) for more information.

## Contact

Oleg E. Vorobiov <isnullxbh@gmail.com>
